//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EngineOp.cpp
//! \brief	Operators Engine
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		20-09-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "EngineOp.h"
#include "Bitmap.h"		//For LoadOp

//-----------------------------------------------------------------
//                   Variables
//-----------------------------------------------------------------
NEngineOp* gpengineOp = null;
//udword NEngineOp::m_dwOpFuncInterfacesCount;
//SOpFuncInterface	NEngineOp::m_aOpFuncInterfaces[MAX_MAXOPINTERFACE];

//-----------------------------------------------------------------
//                   Functions
//-----------------------------------------------------------------
void CallSTDCallFunction(SEngineState* _pstate, const udword *args, int paramSize, fxOPFUNCTION* func);

//-----------------------------------------------------------------
//!	\func	return an unique Engine Instance
//-----------------------------------------------------------------
FXGEN_API NEngineOp* fgCreateOpEngine()
{
	if (gpengineOp==null)
		gpengineOp = NNEW(NEngineOp);

	return gpengineOp;
}

//-----------------------------------------------------------------
//!	\brief	Static Methods that release Engine Instance
//-----------------------------------------------------------------
/*void NEngineOp::Release()
{
	if (gpengineOp!=null)
	{
		NDELETE(gpengineOp, NEngineOp);
		gpengineOp=null;
	}
}*/


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NEngineOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NEngineOp::NEngineOp()
{
	m_dwOpFuncInterfacesCount = 0;
	m_nCurContext		= 0;

	gpengineOp			= this;

	memset(&m_achannels, 0, sizeof(m_achannels));
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NEngineOp::~NEngineOp()
{
}

//-----------------------------------------------------------------
//!	\brief	Compute invalid operators
//! \param	_popFinal	final operator for result
//!	\note		If top stack is invalid, all the stack in the same branch
//!					will been invalided
//-----------------------------------------------------------------
/*void NEngineOp::ComputeInvaliddOps(NOperatorNode* _popFinal)
{
}*/


//-----------------------------------------------------------------
//!	\brief	Compute operators number to process for final result
//!	\param	_popFinal	final operator for result
//!	\note		Invalid operators MUST have been computed first
//!					Updating m_dwTotalProcessOpsCount variable
//!					ClearParsedOpsFlags() must have been called first
//-----------------------------------------------------------------
/*void NEngineOp::_ComputeToProcessOpsCount(NOperatorNode* _popFinal)
{

	if (_popFinal==null)		return;

	//Get Root operator
	NOperatorNode* pccurOP = GetRootOperator(_popFinal);

	//Execute from root
	while (pccurOP && pccurOP != m_popFinal->m_pnextOpToProcess)
	{
		//Process Referenced operators (refTarget) if marked invalid
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperatorNode* prefop = (NOperatorNode*)pccurOP->GetRef(j);
			if (prefop->m_bInvalided && !prefop->m_bParsed)
			{
				//TRACE("RefTarget Must be executed <%s> !\n", prefop->GetName());
				NOperatorNode* pcrootOpToProcess = GetRootOperator(prefop);
				if (pcrootOpToProcess)
				{
					//TRACE(".Jumping To <%s> !\n", pcrootOpToProcess->GetName());

					m_nCurContext++;
					_ComputeToProcessOpsCount(prefop);
					m_nCurContext--;
				}
				prefop->m_bParsed = true;
			}

		}

		//Process operator if marked invalid
		if (pccurOP->m_bInvalided)
		{
			m_dwTotalProcessOpsCount++;
		}

		//Update Stack
		m_aStacks[m_nCurContext][pccurOP->m_byDepth] = pccurOP;

		pccurOP = pccurOP->m_pnextOpToProcess;
	}
}*/

//-----------------------------------------------------------------
//!	\brief	Return root operator from an operator
//!	\param	_pop operator
//!	\return root operator
//!	\note		Links must have been computed first
//-----------------------------------------------------------------
/*NOperatorNode* NEngineOp::GetRootOperator(NOperatorNode* pop)
{
	if (pop==null)		return null;

	NOperatorNode* pccurOP = pop;
	while (pccurOP->m_pprevOpToProcess)
	{
		pccurOP = pccurOP->m_pprevOpToProcess;
	}

	return pccurOP;
}*/


//-----------------------------------------------------------------
//!	\brief	Change a Channel value
//!	\param	_byChannel	channel idx
//!	\param	_etype			type of value
//!	\param	_value			value
//-----------------------------------------------------------------
void NEngineOp::SetChannelValue(ubyte _byChannel, float _value)
{
	if (_byChannel<MAX_CHANNELS)
		m_achannels[_byChannel] = _value;
}

//-----------------------------------------------------------------
//!	\brief	Return a Channel value
//!	\param	_byChannel	channel idx
//!	\param	_etype			type of value
//!	\param	_value			value
//-----------------------------------------------------------------
void NEngineOp::GetChannelValue(ubyte _byChannel, float& _outValue)
{
	if (_byChannel<MAX_CHANNELS)
		_outValue = m_achannels[_byChannel];
}


//-----------------------------------------------------------------
//!	\brief	Process one sequence
//!	\param	
//!	\return Generated ressource
//-----------------------------------------------------------------
SResource* NEngineOp::ProcessSequence(SOpsSequence* _psequence, float _ftime, float _fDetailFactor/*=1.0f*/)
{
	//Flag operators that need process calls
	//ComputeInvaliddOps(popFinal);

	//Init
	m_nCurContext							= 0;
	m_dwCurProcessOpsCount		= 1;
	m_dwTotalProcessOpsCount	= 0;
	m_cbOpsProcess						= null;

	//Init one engine State per context
	for (udword i=0; i<MAX_CONTEXTS; i++)
	{
		m_astates[i].fDetailFactor = _fDetailFactor;
		m_astates[i].pcurCall			 = null;
		NMemFill(m_astates[i].apResourcesLayers, 0, sizeof(m_astates[i].apResourcesLayers));

		for (udword j=0; j<MAX_DEPTH; j++)
		{
			m_astates[i].apResourcesLayers[j] = NNEW(SResource);
			NMemFill(m_astates[i].apResourcesLayers[j], 0, sizeof(SResource));
		}
	}

	//Process
	_ProcessSequence(_ftime, _psequence, _fDetailFactor);

	return _psequence->pResourceResult;
}

//-----------------------------------------------------------------
//!	\brief	Execute this project from root and until final operator
//!	\param	_ftime			time in ms
//!	\param	_psequence	sequence to execute
//!	\param	_fDetailFactor	Result Detail (Factor x0.5, x1 , x2)
//!	\note		Invalid operators MUST have been computed first
//-----------------------------------------------------------------
void NEngineOp::_ProcessSequence(float _ftime, SOpsSequence* _psequence, float _fDetailFactor)
{
	if (_psequence==null)		return;

	//////////////////////////////////////////////////
	//Execute ToLoad sequences
	// MultiThreading can be done here ! See later...
	udword j;
	udword dwRefCount = _psequence->dwSeqsToLoadCount;
	for (j=0; j<dwRefCount; j++)
	{
		SOpsSequence* pseq = _psequence->apSeqsToLoad[j];

		//NOperatorNode* prefop = pseq->popStart;
		if (pseq->byEtat==0)	//Invalid	//###TODO### wait if Compute in progress
		{
			//m_nCurContext++;	//###TODO### MultiThread here :-) see openML ...
			_ProcessSequence(_ftime, pseq, _fDetailFactor);
			pseq->byEtat = 3;	//Computed
			//m_nCurContext--;
		}
	}

	//////////////////////////////////////////////////
	//Execute this sequence calls
	SOpCallDesc* pcurCall = _psequence->apopCallDesc;
	udword dwCount = _psequence->dwOpsCount;
	while (dwCount--)
	{
		m_astates[m_nCurContext].pcurCall = pcurCall;

		//Process operator if marked invalid
		//if (pccurOP->m_bInvalided)	//###TODO### 
		{
				//TRACE("Process %d/%d\n", m_dwCurProcessOpsCount, m_dwTotalProcessOpsCount);
				//if (m_cbOpsProcess)		(*m_cbOpsProcess)(m_dwCurProcessOpsCount, m_dwTotalProcessOpsCount);

				//pccurOP->Process(_ftime, pOpsIns, _fDetailFactor);
				CallSTDCallFunction(&m_astates[m_nCurContext], pcurCall->adwParams, pcurCall->pfncI->dwParamsSize, pcurCall->pfncI->pfnc);

				m_dwCurProcessOpsCount++;

			/*if (pccurOP->m_pcvarsBloc && pccurOP->m_pcvarsBloc->IsAnimated())
				pccurOP->m_bInvalided = true;
			else
				pccurOP->m_bInvalided = false;*/

			//pccurOP->m_fProcessedTime	= _ftime;

			//TRACE("Process <%s> Ctxt<%d>\n", pccurOP->GetName(), m_nCurContext);
		}

		//Next Call
		pcurCall++;
	}
	
	//Store sequence result
	_psequence->pResourceResult = new SResource();
	Res_CopyBmp(m_astates[m_nCurContext].apResourcesLayers[0], _psequence->pResourceResult);

}


//-----------------------------------------------------------------
//!	\brief	Register one or more operators interfaces
//-----------------------------------------------------------------
udword NEngineOp::RegisterOpsInterfaces(udword _dwCount, SOpFuncInterface* _parray)
{
	if (m_dwOpFuncInterfacesCount+_dwCount<MAX_MAXOPINTERFACE)
	{
		for (udword i=0; i<_dwCount; i++)
			m_aOpFuncInterfaces[m_dwOpFuncInterfacesCount++] = _parray[i];
	}
	return m_dwOpFuncInterfacesCount;
}

//-----------------------------------------------------------------
//!	\brief	Return Register operator interface from idx
//!	\return null if not found
//-----------------------------------------------------------------
SOpFuncInterface* NEngineOp::GetOpsInterfaceFromIdx(udword _idx)
{
	if (_idx<m_dwOpFuncInterfacesCount)
		return &m_aOpFuncInterfaces[_idx];
	return null;
}


//-----------------------------------------------------------------
//!	\brief	Call C function
//-----------------------------------------------------------------
void CallSTDCallFunction(SEngineState* _pstate, const udword *args, int paramSize, fxOPFUNCTION* func)
{
#if defined ASM_INTEL

	// Copy the data to the real stack. If we fail to do
	// this we may run into trouble in case of exceptions.
	__asm
	{
		// We must save registers that are used
	    push ecx

		// Copy arguments from script
		// stack to application stack
        mov  ecx, paramSize
		mov  eax, args
		add  eax, ecx
		cmp  ecx, 0
		je   endcopy
copyloop:
		sub  eax, 4
		push dword ptr [eax]
		sub  ecx, 4
		jne  copyloop
endcopy:

		push dword ptr _pstate

		// Call function
		call [func]

		// The callee already removed parameters from the stack

		// Restore registers
		pop  ecx

		// return value in EAX or EAX:EDX
	}

#elif defined ASM_AT_N_T

	asm("pushl %ecx           \n"
	
		// Need to align the stack pointer so that it is aligned to 16 bytes when making the function call.
		// It is assumed that when entering this function, the stack pointer is already aligned, so we need
		// to calculate how much we will put on the stack during this call.
		"movl  12(%ebp), %eax \n" // paramSize
		"addl  $4, %eax       \n" // counting esp that we will push on the stack
		"movl  %esp, %ecx     \n"
		"subl  %eax, %ecx     \n"
		"andl  $15, %ecx      \n"
		"movl  %esp, %eax     \n"
		"subl  %ecx, %esp     \n"
		"pushl %eax           \n" // Store the original stack pointer
	
		"movl  12(%ebp), %ecx \n" // paramSize
		"movl  8(%ebp), %eax  \n" // args
		"addl  %ecx, %eax     \n" // push arguments on the stack
		"cmp   $0, %ecx       \n"
		"je    endcopy2       \n"
		"copyloop2:           \n"
		"subl  $4, %eax       \n"
		"pushl (%eax)         \n"
		"subl  $4, %ecx       \n"
		"jne   copyloop2      \n"
		"endcopy2:            \n"
		"call  *16(%ebp)      \n" // callee pops the arguments
		
		// Pop the alignment bytes
		"popl  %esp           \n" 
		
		"popl  %ecx           \n");

#endif
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//							NLoadOp implementation
//-----------------------------------------------------------------
//-----------------------------------------------------------------
void NLoadOp_Process(SEngineState* _state, SOpsSequence* _popToLoad)
{
	SResource* pDst = _state->apResourcesLayers[_state->pcurCall->byDepth];

	//loaded sequences as been already processed at this sequence start :-)

	//Copy Bitmap
	SResource* pRef = _popToLoad->pResourceResult;
	if (pRef==null)
		pRef=null;	//Debug

	Res_CopyBmp(pRef, pDst);
}


SOpFuncInterface IOpsMain[] = {
	"Load",			1*4,  (fxOPFUNCTION*)&NLoadOp_Process
};

udword dwCountOpsMain = fgCreateOpEngine()->RegisterOpsInterfaces(1, IOpsMain);
