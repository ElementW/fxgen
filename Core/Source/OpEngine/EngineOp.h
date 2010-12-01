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
#ifndef ENGINEOP_H
#define ENGINEOP_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "../../Include/CoreLib.h"

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
	class NOperator;
		class	NStoreResultOp;
	class NOpGraphModel;
	class NEngineOp;

//-----------------------------------------------------------------
//                   TypesDef
//-----------------------------------------------------------------
typedef	void (__cdecl FXGEN_OPSPROCESSCB)(udword _dwCurrentOp, udword _dwTotalOps);

//-----------------------------------------------------------------
//!	\class		NOperator
//!	\brief		Base class for an operator
//!	\note			TODO will been split in NOperatorFX(Engine) and NOperatorNode(Designer)
//-----------------------------------------------------------------
class CORELIB_API NOperator :	public NObject
{
public:
	NOperator();
	virtual ~NOperator();

	//Methods
	virtual NObject* Duplicate();						//!< Duplicate this object (used for copy-paste)
	virtual	udword GetColor()	= 0;				//!< Operator color
	virtual const char* GetName()				{ return ""; }			//!< Operator's Name
	virtual const char* GetCategory()		{ return "Misc"; }	//!< Operator's Category
	virtual const char* GetUserName()		{ return null; }		//!< Operator's User Name
	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor) = 0;	//!< object processing (texture, mesh ...)

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
//!	\class		NCompiledAsset
//!	\brief		
//-----------------------------------------------------------------
class CORELIB_API NCompiledAsset :	public NObject
{
public:
	NCompiledAsset();
	virtual ~NCompiledAsset();

	//Serialization
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

	//Methods
	//NStoreResultOp* GetFinalOpByName(pszName)	//Name => Graph:FinalOutput

protected:
	//Datas
	NObjectArray	m_arrayOps;				//!< Operators array
	NObjectArray	m_arrayFinalOps;	//!< Final Operators array
};


//-----------------------------------------------------------------
//!	\class		NEngineOp
//!	\brief		Engine operators process
//-----------------------------------------------------------------
class CORELIB_API NEngineOp
{
public:
	//Constructors-Destructor
	NEngineOp();
	virtual ~NEngineOp();

	//Get unique Instance
	static	NEngineOp* GetInstance();

	//API Methods
	void	Clear();

	//void	GetFinalsResultsList(NObjectArray& _carray, const char* _pszFromGroup=NULL, bool _bRecurse=true);
	void	ProcessFinalResult(NStoreResultOp* _pFinalResultOp, float _ftime, float _fDetailFactor=1.0f, FXGEN_OPSPROCESSCB* _cbOpsProcess=NULL);
	void	CompactMemory(ubyte _byTypeMask=OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED);

	//Editor Execution
	//void InvalidateAllOps();
	void InvalidateOp(NOperator* _pop);
	void Execute(float _ftime, NOperator* _popFinal, float _fDetailFactor=1.0f, FXGEN_OPSPROCESSCB* _cbProcess=NULL);

	//Editor Channels methods
	void SetChannelValue(ubyte _byChannel, NVarValue& _value);
	void GetChannelValue(ubyte _byChannel, NVarValue& _outValue);

	//Editor Membres access
	//NTreeNode*			GetRootGroup()				{ return m_pRootGroup;		}
	NObjectGarbage* GetBitmapGarbage()		{ return &m_bitmapsAlloc; }

	//Editor Operators Resources management
	void GetBitmap(NObject** _ppobj, ubyte _byObjType=OBJRES_TYPE_INTERMEDIATE);

protected:
	//Internal Methods	
	//void GetFinalOps(NTreeNode* _pnodeFrom, NObjectArray& _finalsOp, bool _bRecurse);	//###TOMOVE### to compiler
	//void _GetFinalOps(NTreeNode* _pnode, NObjectArray& _finalsOp, bool _bRecurse); //###TOMOVE### to compiler
	void ClearParsedOpsFlags(NOperator* _pop); //###TOMOVE### to compiler
	//void _InvalidateAllOps(NTreeNode* _pnode); //###TOMOVE### to compiler
	NOperator* GetRootOperator(NOperator* _pop); //###TOMOVE### to compiler

	//Methods for execution
	void _Execute(float _fTime, NOperator* _popFinal, float _fDetailFactor);
	void ComputeInvaliddOps(NOperator* _popFinal);
	void _ComputeInvaliddOps(NOperator* _pop);
	void _ComputeToProcessOpsCount(NOperator* _popFinal);

	//Datas
	//NTreeNode* m_pRootGroup;							//!< Root	Groups	//###TOMOVE### to compiler and replace by CompiledAsset

	NVarValue  m_achannels[MAX_CHANNELS];	//!< Values for animation channels

	//Datas for compilation and execution
	NOperator*	m_aStacks[MAX_CONTEXTS][MAX_DEPTH];	//!< Inputs Stack for operators process
	udword			m_nCurContext;				//!< indice (see m_aStacks[m_nCurContext, dwCurLevel])
	NOperator*	m_popFinal;
	bool				m_bError;							//!< Error while process
	udword			m_dwTotalProcessOpsCount, m_dwCurProcessOpsCount;
	FXGEN_OPSPROCESSCB* m_cbOpsProcess;

	//Garbages for media (bitmaps ...)
	NObjectGarbage	m_bitmapsAlloc;
};

#endif //ENGINEOP_H
