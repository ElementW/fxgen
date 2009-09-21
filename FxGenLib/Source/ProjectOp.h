//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ProjectOp.h
//! \brief	Operators Project
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		05-01-2009
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef PROJECTOP_H
#define PROJECTOP_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "Core.h"

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------
#ifndef STATIC_BUILD
#ifdef FXGEN_EXPORTS
	#define FXGEN_API __declspec(dllexport)
#else
	#define FXGEN_API __declspec(dllimport)
#endif
#else // STATIC_BUILD
#define FXGEN_API
#endif // STATIC_BUILD

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define	MAX_CONTEXTS	256		//!< Max Stack contexts
#define	MAX_DEPTH			64		//!< Max Stack Hierarchical depth
#define	MAX_CHANNELS	255		//!< Max channels animation (1-255)

#define	OBJRES_TYPE_INTERMEDIATE	1		//!< Intermediate allocated bitmaps
#define	OBJRES_TYPE_STORED				2		//!< Stored bitmaps
#define	OBJRES_TYPE_FINALSTORED		4		//!< Final Stored bitmaps

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NObject;
	class NOperatorNode;
		class	NStoreResultOp;

	class NOperatorsPage;
	class NOperatorsProject;

	struct SCompiledOp;
	struct SOpsSequence;

//-----------------------------------------------------------------
//!	\class		NOperatorNode
//!	\brief		Base class for an operator
//-----------------------------------------------------------------
class FXGEN_API NOperatorNode :	public NObject
{
public:
	NOperatorNode();
	virtual ~NOperatorNode();

	//Methods
	virtual NObject* Duplicate();						//!< Duplicate this object (used for copy-paste)
	virtual	udword GetColor()	= 0;				//!< Operator color
	virtual const char* GetName()				{ return ""; }			//!< Operator's Name
	virtual const char* GetCategory()		{ return "Misc"; }	//!< Operator's Category
	virtual const char* GetUserName()		{ return null; }		//!< Operator's User Name

	//Editing Methods
	//virtual void BeginEdit()	{}
	//virtual void Events(mouse, keys...)	{}
	//virtual void EndEdit()	{}

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	//Datas GUI
	sword	m_wPosX, m_wPosY;			//!< Position (grid unit)
	sword	m_wWidth;							//!< Width (grid unit)

	//Datas RT
	bool				m_bInvalided;				//!< Invalid operator (must be processed for result)
	bool				m_bError;						//!< Error while process
	bool				m_bParsed;					//!< Operators scan optimization
	float				m_fProcessedTime;		//!< Time for last process() call
	NObject*		m_pObj;							//!< Object generated (Texture, Mesh ...)
	NOperatorNode*	m_pnextOpToProcess;	//!< Next operator to execute
	NOperatorNode*	m_pprevOpToProcess;	//!< Previous operator executed

	//Compiler
	SOpsSequence* m_pSequence;			//!< Sequence attached to this operator

	// Variables Bloc
	NVarsBloc* m_pcvarsBloc;
};



//-----------------------------------------------------------------
//!	\class		NOperatorsPage
//!	\brief		one operators page
//-----------------------------------------------------------------
class FXGEN_API NOperatorsPage : public NObject
{
public:
	FDECLARE_CLASS();

	NOperatorsPage();
	virtual ~NOperatorsPage();

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	udword			GetOpsCount()							{ return m_arrayOps.Count();					}
	NOperatorNode*	GetOpFromIdx(udword _idx)	{ return (NOperatorNode*)m_arrayOps[_idx];}

	udword			AddOp(NOperatorNode* _pop);
	void				DeleteAllOps();
	udword			DeleteOp(NOperatorNode* _pop);
	void				MoveOp(NOperatorNode* _pop, sword _x, sword _y);
	void				InvalidateAllOps();

	//Methodes de recherche
	void GetOpsFromClassName(const char* _pszClassName, NObjectArray& _carray);

	//Methodes pour la generation des liens entre les ops
	void ComputeLinks();
	void _ComputeLinks(NOperatorNode* _pop, NOperatorNode* _pprevop, udword _dwCurDepth);

	void GetPrevOperators(NOperatorNode* _pop, NObjectArray& _carrayPrevOpS);
	void GetNextOperators(NOperatorNode* _pop, NObjectArray& _carrayNextOpS);
	NOperatorNode* GetFinalOpFrom(NOperatorNode* _pop);

	void ComputeOpsSequences(SCompiledOp* _inoutCompOps);
	void CompileOperator(SOpsSequence *_pstSeq, NOperatorNode* _pop);

	//Datas	GUI
	NObjectArray	m_arrayOps;						//!< Operators array

	//Datas for compilation and linkage
	NOperatorNode*		m_pprevOp;
	NObjectArray	m_arrayOpsUnlinked;		//!< Operators unlinked array
};

//-----------------------------------------------------------------
//!	\class		NOperatorsProject
//!	\brief		One Operators project
//-----------------------------------------------------------------
class FXGEN_API NOperatorsProject
{
public:
	//Constructors-Destructor
	NOperatorsProject();
	virtual ~NOperatorsProject();

	//Methods
	void Clear();
	bool LoadProject(const char* _pszFullFileName);	//!< Load project
	bool SaveProject(const char* _pszFullFileName);	//!< Save project

	bool CompileGraph(SCompiledOp* _inoutCompOps, bool _bOnlyFinalsResults);
	void GetFinalsResultsList(NObjectArray& _carray, const char* _pszFromGroup=NULL, bool _bRecurse=true);

	//Membres access
	NTreeNode* GetRootGroup()				{ return m_pRootGroup;		}

protected:
	//Internal Methods
	void GetFinalOps(NTreeNode* _pnodeFrom, NObjectArray& _finalsOp, bool _bRecurse);
	void _GetFinalOps(NTreeNode* _pnode, NObjectArray& _finalsOp, bool _bRecurse);
	void ClearParsedOpsFlags(NOperatorNode* _pop);
	NOperatorNode* GetRootOperator(NOperatorNode* _pop);
	void ComputeOpsSequences(SCompiledOp* _inoutCompOps, NTreeNode* _pnodeFrom);

	//Datas
	NTreeNode* m_pRootGroup;	//!< Root	Groups

	//Datas for project compilation
	NOperatorNode*	m_aStacks[MAX_CONTEXTS][MAX_DEPTH];	//!< Inputs Stack for operators process
	udword			m_nCurContext;				//!< indice (see m_aStacks[m_nCurContext, dwCurLevel])
	NOperatorNode*	m_popFinal;
	bool				m_bError;							//!< Error while process
};

#endif //PROJECTOP_H
