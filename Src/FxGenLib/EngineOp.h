//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EngineOp.h
//! \brief	Operators Engine
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "core.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define	MAX_CONTEXTS	256		//!< Max Stack contexts
#define	MAX_DEPTH			64		//!< Max Stack Hierarchical depth
#define	MAX_CHANNELS	256		//!< Max channels animation (1-256)

#define	OBJRES_TYPE_INTERMEDIATE	1		//!< Intermediate allocated bitmaps
#define	OBJRES_TYPE_STORED				2		//!< Stored bitmaps
#define	OBJRES_TYPE_FINALSTORED		4		//!< Final Stored bitmaps

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NObject;
	class NOperator;
	class NOperatorsPage;
	class NEngineOp;

//-----------------------------------------------------------------
//                   TypesDef
//-----------------------------------------------------------------
typedef	void (__cdecl FXGEN_PROCESSCB)(udword _dwCurrentOp, udword _dwTotalOps);

//-----------------------------------------------------------------
//!	\class		NOperator
//!	\brief		Base class for an operator
//-----------------------------------------------------------------
class FXGEN_API NOperator :	public NObject
{
public:
	NOperator();
	virtual ~NOperator();

	//Methods
	virtual NObject* Duplicate();						//!< Duplicate this object (used for copy-paste)
	virtual	COLORREF GetColor()	= 0;				//!< Operator color
	virtual char* GetName()				{ return ""; }			//!< Operator's Name
	virtual char* GetCategory()		{ return "Misc"; }	//!< Operator's Category
	virtual char* GetUserName()		{ return null; }		//!< Operator's User Name

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts) = 0;	//!< object processing (texture, mesh ...)

	//Datas GUI
	sword	m_wPosX, m_wPosY;			//!< Position (grid unit)
	sword	m_wWidth;							//!< Width (grid unit)

	//Datas Execution
	ubyte	m_byDepth;						//!< Depth in tree
	ubyte	m_byInputs;						//!< Input Operators numbers

	//Datas RT
	bool				m_bInvalided;				//!< Invalid operator (must be processed for result)
	bool				m_bError;						//!< Error while process
	bool				m_bParsed;					//!< Operators scan optimization
	float				m_fProcessedTime;		//!< Time for last process() call
	NObject*		m_pObj;							//!< Object generated (Texture, Mesh ...)
	NOperator*	m_pnextOpToProcess;	//!< Next operator to execute
	NOperator*	m_pprevOpToProcess;	//!< Previous operator executed

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
	DECLARE_CLASS();

	NOperatorsPage();
	virtual ~NOperatorsPage();

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	udword			GetOpsCount()							{ return m_arrayOps.Count();					}
	NOperator*	GetOpFromIdx(udword _idx)	{ return (NOperator*)m_arrayOps[_idx];}

	udword			AddOp(NOperator* _pop);
	void				DeleteAllOps();
	udword			DeleteOp(NOperator* _pop);
	void				MoveOp(NOperator* _pop, sword _x, sword _y);


	//Methodes de recherche
	void GetOpsFromClassName(char* _pszClassName, NObjectArray& _carray);

	//Methodes pour la generation des liens entre les ops
	void ComputeLinks();
	void _ComputeLinks(NOperator* _pop, NOperator* _pprevop, udword _dwCurDepth);

	void GetPrevOperators(NOperator* _pop, NObjectArray& _carrayPrevOpS);
	void GetNextOperators(NOperator* _pop, NObjectArray& _carrayNextOpS);
	NOperator* GetFinalOpFrom(NOperator* _pop);

	//Datas	GUI
	NObjectArray	m_arrayOps;						//!< Operators array

	//Datas for compilation and linkage
	NOperator*		m_pprevOp;
	NObjectArray	m_arrayOpsUnlinked;		//!< Operators unlinked array
};

//-----------------------------------------------------------------
//!	\class		NEngineOp
//!	\brief		Engine operators process
//-----------------------------------------------------------------
class FXGEN_API NEngineOp
{
public:
	//Constructors-Destructor
	NEngineOp();
	virtual ~NEngineOp();

	//API		###TODO###
	void ProcessOperators(float _ftime, FXGEN_PROCESSCB* _cbProcess);
	void CompactMemory();

	udword		GetFinalResultCount();	//##NEW###
	NBitmap*	GetFinalResultBitmapByIdx(udword _idx);	//##NEW###


	//Methods
	void Clear();
	bool LoadProject(char* _pszFullFileName);
	bool SaveProject(char* _pszFullFileName);

	//Operators access
	void GetFinalOps(NTreeNode* _pnodeFrom, NObjectArray& _finalsOp, bool _bRecurse);

	//Execution
	void				InvalidateOp(NOperator* _pop);
	void				Execute(float _ftime, NOperator* _popFinal);

	//Page's Group Methods
	NTreeNode*	GetRootGroup()				{ return m_pRootGroup; }

	//Resources management
	void	GetBitmap(NObject** _ppobj, ubyte _byObjType=OBJRES_TYPE_INTERMEDIATE);

	//Channels methods
	void SetChannelValue(ubyte _byChannel, NVarValue& _value);
	void GetChannelValue(ubyte _byChannel, NVarValue& _outValue);

	//Membres access
	NObjectGarbage* GetBitmapGarbage()	{ return &m_bitmapsAlloc; }

protected:
	//Internal Methods
	void _GetFinalOps(NTreeNode* _pnode, NObjectArray& _finalsOp, bool _bRecurse);
	void ClearParsedOpsFlags(NOperator* _pop);

	//Methods for execution
	void _Execute(float _fTime, NOperator* _popFinal);
	void ComputeInvaliddOps(NOperator* _popFinal);
	void _ComputeInvaliddOps(NOperator* _pop);
	void _ComputeToProcessOpsCount(NOperator* _popFinal);

	NOperator* GetRootOperator(NOperator* _pop);

	//Datas
	NTreeNode*		m_pRootGroup;								//!< Root	Groups
	NVarValue			m_achannels[MAX_CHANNELS];	//!< Values for animation channels

	//Datas for compilation and execution
	NOperator*	m_aStacks[MAX_CONTEXTS][MAX_DEPTH];	//!< Inputs Stack for operators process
	udword			m_nCurContext;				//!< indice (see m_aStacks[m_nCurContext, dwCurLevel])
	NOperator*	m_popFinal;
	bool				m_bError;							//!< Error while process
	udword			m_dwTotalProcessOpsCount, m_dwCurProcessOpsCount;

	//Garbage
	NObjectGarbage	m_bitmapsAlloc;

	//Datas API Interface
	NObjectArray m_arrayFinalsOp;		//##NEW###
};


//-----------------------------------------------------------------
//	Externs
//-----------------------------------------------------------------
FXGEN_API extern NEngineOp* gNFxGen_GetEngine();
