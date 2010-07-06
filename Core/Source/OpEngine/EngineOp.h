//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EngineOp.h
//! \brief	Operators Engine
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		06-07-2010
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//! \note		Todo: Separate compiler and operators machine from Engine
//!								can save compiled form only if no error (separate validate and process from process())
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
#define	MAX_CONTEXTS				16		//!< Max Stack contexts
#define	MAX_DEPTH						32		//!< Max Stack Hierarchical depth
#define	MAX_CHANNELS				255		//!< Max channels animation (1-255)
#define	MAX_OPSTOLOAD				64		//!< Max operators that can be loaded by one sequence
#define	MAX_SEQSTOLOAD			64		//!< Max sequences that can be loaded by one sequence
#define	MAX_MAXOPINTERFACE	255		//!< Max operators interface
#define	MAX_PARAMS					32		//!< Max parameters
#define	TPS_LAYER						MAX_DEPTH-1		//!< Temp Ressource Layer

/*#define	OBJRES_TYPE_INTERMEDIATE	1		//!< Intermediate allocated bitmaps
#define	OBJRES_TYPE_STORED				2		//!< Stored bitmaps
#define	OBJRES_TYPE_FINALSTORED		4		//!< Final Stored bitmaps*/

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NEngineOp;
class NOperatorFx;
struct NRGBA;
struct SEngineState;

//-----------------------------------------------------------------
//                   Functions
//-----------------------------------------------------------------
CORELIB_API NEngineOp * fgCreateOpEngine();

//-----------------------------------------------------------------
//                   TypesDef
//-----------------------------------------------------------------
typedef	void (FXGEN_OPSPROCESSCB)(udword _dwCurrentOp, udword _dwTotalOps);
typedef void (fxOPFUNCTION)(SEngineState* _state, void* _params);


//-----------------------------------------------------------------
//!	\class		NResource
//!	\brief		Resource
//!	\note			###TODO### mesh, voxel... see SOpCallDesc::byResType
//-----------------------------------------------------------------
struct SResource
{
	//Bitmap
	NRGBA*		pbyPixels;
	udword		dwWidth, dwHeight;
};


//-----------------------------------------------------------------
//!	\class		SOpsSequence
//!	\brief		One operators sequence
//! \note			There is one sequence per 'Store' Operator
//-----------------------------------------------------------------
struct SOpsSequence
{
	char					szName[MAX_NAMELEN];	//!< Sequence Name (usualy stored operator name)
	ubyte					byEtat;								//!< 0-Invalid, 1-Compute in progress, 3-Computed

	udword				dwOpsCount;				//!< Operators count to call for this sequence ####TOREMOVE###
	NOperatorFx*	pfirstOpsCall;		//!< First operator to call, then use next

	//'Load' Operators used by this sequence
	// later will be used for multi threading
	udword				dwSeqsToLoadCount;	//!< How many 'LoadOp' (or Sequences) to process before this sequence
	SOpsSequence*	apSeqsToLoad[MAX_SEQSTOLOAD];

	SOpsSequence*		pnextOpsSeq;	//!< Link toward next sequence

	//Cache
	SResource*			pResourceResult;	//!< Sequence ressource Result

	//###TODO### serialize

};

//-----------------------------------------------------------------
//!	\class		SCompiledOp
//!	\brief		Compiled operators graph
//-----------------------------------------------------------------
struct SCompiledOp
{
	udword dwTotalOpsCount;		//!< sum of operators for all sequences
	SOpsSequence *pfirstOpsSeq, *plastOpsSeq;		//!< First and last sequences pointers
};

//-----------------------------------------------------------------
//!	\class		SEngineState
//!	\brief		Engine state
//-----------------------------------------------------------------
struct SEngineState
{
	float					fDetailFactor;
	SResource*		apResourcesLayers[MAX_DEPTH];
	NOperatorFx*	pcurCall;
};

//-----------------------------------------------------------------
//!	\class		NOperatorFx
//!	\brief		Operator
//-----------------------------------------------------------------
class CORELIB_API NOperatorFx : NObject
{
public:
	virtual void Process(SEngineState* _state) = 0;

	//Serialization (like parameters)
	virtual	bool Save(NArchive* _s);	//!< Save object
	virtual	bool Load(NArchive* _l);	//!< Load object

protected:
	//Datas
	ubyte					m_byInputsCount;		//!< Input Operators count
	ubyte					m_byDepth;					//!< Depth in tree

public:
	NOperatorFx*	m_pnext;						//!< Next op to call
	//Params...
};


//-----------------------------------------------------------------
//!	\class		NEngineOp
//!	\brief		Operators Engine 
//-----------------------------------------------------------------
class CORELIB_API NEngineOp
{
public:
	//Constructors-Destructor
	NEngineOp();
	virtual ~NEngineOp();

	//API Methods
	SResource* ProcessSequence(SOpsSequence* _psequence, float _ftime, float _fDetailFactor=1.0f);

	//Animation Channels methods
	void SetChannelValue(ubyte _byChannel, float _value);
	void GetChannelValue(ubyte _byChannel, float& _outValue);

	//Assets management
	//void AddAsset(SCompiledOp* _pops);	//###TODO###
	//void RemoveAsset(); //###TODO###

protected:
	//Methods for execution
	void _ProcessSequence(float _ftime, SOpsSequence* _psequence, float _fDetailFactor);
	//void ComputeInvaliddOps(NOperatorNode* _popFinal);

	//Datas
	float  m_achannels[MAX_CHANNELS];	//!< Values for animation channels

	SEngineState	m_astates[MAX_CONTEXTS];
	udword				m_nCurContext;				//!< indice (see m_astates[m_nCurContext])
	udword				m_dwTotalProcessOpsCount, m_dwCurProcessOpsCount;
	FXGEN_OPSPROCESSCB* m_cbOpsProcess;

};


#endif //ENGINEOP_H
