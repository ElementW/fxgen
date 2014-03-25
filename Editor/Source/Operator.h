//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Operator.h
//! \brief	Operators
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		23-09-2009
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef OPERATOR_H
#define OPERATOR_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "CoreLibPkg.h"

#ifdef GetUserName
#undef GetUserName
#endif

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define	OBJRES_TYPE_INTERMEDIATE	1		//!< Intermediate allocated bitmaps
#define	OBJRES_TYPE_STORED				2		//!< Stored bitmaps
#define	OBJRES_TYPE_FINALSTORED		4		//!< Final Stored bitmaps

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NObject;
	class NOperatorNode;


//-----------------------------------------------------------------
//!	\class		NOperatorNode
//!	\brief		Base class for an operator's node
//-----------------------------------------------------------------
class NOperatorNode :	public NObject
{
public:
	NOperatorNode();
	virtual ~NOperatorNode();

	//Methods
	virtual NObject* Duplicate();						//!< Duplicate this object (used for copy-paste)
	virtual	udword GetColor()	= 0;				//!< Operator color
	virtual const char* GetName()				{ return ""; }			//!< Operator's class Name
	virtual const char* GetCategory()		{ return "Misc"; }	//!< Operator's Category
  virtual const char* GetGUIName()		{ return null; }		//!< Operator's User Name

	NObject* GetResource()	{ return m_pObj; }	//!< Return generated resource object
	NOperator* GetOperator() = 0;

	//Editing
	//###TODO###
	//	MouseEvent
	//	KeyEvent

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

public:	//###TODO### create methods
	//Datas GUI
	udword	m_dwPosX, m_dwPosY;			//!< Position (grid unit)
	udword	m_dwWidth;							//!< Width (grid unit)

	bool m_bError;
	bool m_bInvalided;
	float m_fProcessedTime;

	NOperatorFx* m_opFX;	//!< FX Operator attached at this node

protected:

	NObject *m_pObj;	//!< Ressource

	friend class NOperatorsPage;
};

//-----------------------------------------------------------------
//!	\class		NOperatorsPage
//!	\brief		one operators page
//-----------------------------------------------------------------
class NOperatorsPage : public NObject
{
public:
	FDECLARE_CLASS();

	NOperatorsPage();
	virtual ~NOperatorsPage();

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	virtual const char*	GetName()							{ return "Page";}	//!< Return object name

	udword					GetOpsCount()							{ return m_arrayOps.Count();					}
	NOperatorNode*	GetOpFromIdx(udword _idx)	{ return (NOperatorNode*)m_arrayOps[_idx];}

	//Operators Methods
	udword			AddOp(NOperatorNode* _pop);
	void				DeleteAllOps();
	udword			DeleteOp(NOperatorNode* _pop);
	void				MoveOp(NOperatorNode* _pop, sword _x, sword _y);
	void				InvalidateAllOps();

	//Methodes de recherche
	//void GetOpsFromClassName(const char* _pszClassName, NObjectArray& _carray);
	NOperatorNode* GetFinalOpFrom(NOperatorNode* _pop);

protected:
	//Methods Compiler
	void Compile();

	NCompilerOp m_comp;
	SCompiledOp m_stCompiled;

	//Datas	GUI
	NObjectArray	m_arrayOps;						//!< Operators array
};

//-----------------------------------------------------------------
//!	\class		NOperatorsProject
//!	\brief		Operators project
//-----------------------------------------------------------------
class NOperatorsProject
{
public:
	//Constructors-Destructor
	NOperatorsProject();
	virtual ~NOperatorsProject();

	//Get unique Engine Instance
	static	NOperatorsProject* GetProject();

	//Methods
	void	Clear();
	bool	LoadProject(const char* _pszFullFileName);

//	void	CompactMemory(ubyte _byTypeMask=OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED);

	//Editor Methods
	bool SaveProject(const char* _pszFullFileName);

	//Editor Execution
	void InvalidateAllOps();
	void InvalidateOp(NOperatorNode* _pop);
	void Execute(float _ftime, NOperatorNode* _popFinal, float _fDetailFactor=1.0f/*, FXGEN_OPSPROCESSCB* _cbProcess=NULL*/);

	//Editor Channels methods
//	void SetChannelValue(ubyte _byChannel, NVarValue& _value);
//	void GetChannelValue(ubyte _byChannel, NVarValue& _outValue);

	//Editor Membres access
	NTreeNode*			GetRootGroup()				{ return m_pRootGroup;		}
	//NObjectGarbage* GetBitmapGarbage()		{ return &m_bitmapsAlloc; }

	//Editor Operators Resources management
	//void GetBitmap(NObject** _ppobj, ubyte _byObjType=OBJRES_TYPE_INTERMEDIATE);

protected:


	//Datas
	NTreeNode* m_pRootGroup;							//!< Root	Groups
	//NVarValue  m_achannels[MAX_CHANNELS];	//!< Values for animation channels

	//Datas for compilation and execution
	udword			m_dwTotalProcessOpsCount, m_dwCurProcessOpsCount;
//	FXGEN_OPSPROCESSCB* m_cbOpsProcess;

	//Garbages for media (bitmaps ...)
	NObjectGarbage	m_bitmapsAlloc;
};

#endif //OPERATOR
