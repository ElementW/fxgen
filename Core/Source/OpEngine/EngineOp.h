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
//!	\class		SOpFuncInterface
//!	\brief		C Function interface
//-----------------------------------------------------------------
struct SOpFuncInterface
{
	char					szName[MAX_NAMELEN];	//!< Function Name
	udword				dwParamsSize;					//!< Parameters size in bytes
	fxOPFUNCTION*	pfnc;									//!< C function to call
};

//-----------------------------------------------------------------
//!	\class		SOpCallDesc
//!	\brief		Operator call desc
//-----------------------------------------------------------------
struct SOpCallDesc
{
	//ubyte byResType //!< Ressource Type ###TOADD### later...
	ubyte			byInputsCount;							//!< Input Operators count
	ubyte			byDepth;										//!< Depth in tree
	ubyte			byParamsCount;							//!< Parameters count
	udword		adwParams[MAX_PARAMS];			//!< Parameters datas
	SOpFuncInterface* pfncI;							//!< Operator function to call
	udword		dwTag;
};

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

	udword				dwOpsCount;			//!< Operators count to call for this sequence
	SOpCallDesc*	apopCallDesc;		//!< ArrayPtr of Operators to call

	//'Load' Operators used by this sequence
	// later will be used for multi threading
	udword				dwSeqsToLoadCount;	//!< How many 'LoadOp' or Sequences to process before this sequence
	SOpsSequence*	apSeqsToLoad[MAX_SEQSTOLOAD];

	SOpsSequence*		pnextOpsSeq;	//!< Link toward next sequence

	//Cache
	SResource*			pResourceResult;	//!< Sequence ressource Result
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
//!	\class		SOpCallDesc
//!	\brief		Compiled operators graph
//-----------------------------------------------------------------
struct SEngineState
{
	float					fDetailFactor;
	SResource*		apResourcesLayers[MAX_DEPTH];
	SOpCallDesc*	pcurCall;
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

	//API Methods
	SResource* ProcessSequence(SOpsSequence* _psequence, float _ftime, float _fDetailFactor=1.0f);

	//Operators Registration
	udword RegisterOpsInterfaces(udword _dwCount, SOpFuncInterface* _parray);
	udword GetOpsInterfacesCount()	{	return m_dwOpFuncInterfacesCount; }
	SOpFuncInterface* GetOpsInterfaceFromIdx(udword _idx);	//!< return null if not found

	//Animation Channels methods
	void SetChannelValue(ubyte _byChannel, float _value);
	void GetChannelValue(ubyte _byChannel, float& _outValue);

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

	//Registered Operator Interfaces
	udword						m_dwOpFuncInterfacesCount;
	SOpFuncInterface	m_aOpFuncInterfaces[MAX_MAXOPINTERFACE];
};


#endif //ENGINEOP_H
