//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		CompilerOp.cpp
//! \brief	Operators Compiler
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		02-07-2009
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
#include "CompilerOp.h"
#include "EngineOp.h"

//-----------------------------------------------------------------
//                   Variables
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//!	\brief	Sort operators from left to right position
//-----------------------------------------------------------------
int SortOpsLeftToRight(const void *elem1, const void *elem2)
{
	SOperatorDesc* e1 = *(SOperatorDesc**)elem1;
	SOperatorDesc* e2 = *(SOperatorDesc**)elem2;
	if (e1->x<e2->x)	return +1;
	if (e1->x>e2->x)	return -1;
	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCompilerOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NCompilerOp::NCompilerOp()
{

}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NCompilerOp::~NCompilerOp()
{

}

//-----------------------------------------------------------------
//!	\brief	Compile graph to linear form
//!	\return	True if success
//-----------------------------------------------------------------
bool NCompilerOp::Compile(SCompiledOp* _inoutCompOps, SOperatorDesc* _paops, udword _dwCount, bool _bIncremential)
{
	bool bRet = true;
	
	//Init
	_inoutCompOps->dwTotalOpsCount	= 0;
	_inoutCompOps->pfirstOpsSeq			= null;
	_inoutCompOps->plastOpsSeq			= null;

	//Make Array of to compile operators
	m_arrayOps.SetSize(_dwCount);
	for (udword i=0; i<_dwCount; i++)
		m_arrayOps.AddItem(&_paops[i]);

	//Compute Graphical links between operators
	ComputeLinks();

	//Compute operators sequences
	ComputeOpsSequences(_inoutCompOps);
	
	//Compute links beetween operators, if Graph modified
	// Rk. already done in Move, Add, Delete
	//if (m_bGraphModified)
	//	ComputeLinks();

	//Errors checking
	// .Roots operators must be 'render categorie' or 'LoadOp'
	// .Control operators inputs count
	// .Control cyclic referencing

	//Get All Finals result Operators List
	// ###TODO### attention tous les operators ne se finissent pas par 'finalresult' en mode designer !
	// il faut gerer le 'mark show' !


	return bRet;
}


//-----------------------------------------------------------------
//!	\brief	Compute links between operators
//-----------------------------------------------------------------
void NCompilerOp::ComputeLinks()
{
	//Create stack for unlinked operators
	m_arrayOpsUnlinked.Clear();
	m_arrayOpsUnlinked.AddArray(m_arrayOps);

	//Process all ops from stack
	while (m_arrayOpsUnlinked.Count())
	{
		SOperatorDesc* pop = (SOperatorDesc*)m_arrayOpsUnlinked[0];
		SOperatorDesc* popFinal = GetFinalOpFrom(pop);	//From Graphical position

		m_pprevOp = null;
		_ComputeLinks(popFinal, null, 0);
	}

}

//-----------------------------------------------------------------
//!	\brief	Compute links between operators
//!
//!	\param	_popLnk			current operator link
//!	\param	_dwCurDepth	current depth in tree
//!
//! \note Recurse method used by ComputeLinks()
//!				ComputeLinks scan operators from graphical position (bottom -> top)
//-----------------------------------------------------------------
void NCompilerOp::_ComputeLinks(SOperatorDesc* _pop, SOperatorDesc* _pprevop, udword _dwCurDepth)
{
/*	if (_pprevop)
		TRACE("\t_ComputeLinks %s PrevOP %s dwCurDepth %d\n", _pop->GetName(), _pprevop->GetName(), _dwCurDepth );
	else
		TRACE("\t_ComputeLinks %s PrevOP -- dwCurDepth %d\n", _pop->GetName(), _dwCurDepth );*/

	//Update current operator execution link
	_pop->m_pnextOpToProcess = m_pprevOp;
	_pop->m_pprevOpToProcess = null;

	if (m_pprevOp)
		m_pprevOp->m_pprevOpToProcess = _pop;

	m_pprevOp = _pop;

	//Remove it from unlinked
	udword dwIdx = m_arrayOpsUnlinked.Find(_pop);
	if (dwIdx!=(udword)-1)
		m_arrayOpsUnlinked.RemoveItem(dwIdx);

	//Link previous operators at same horizontal level
	NOpsDescArray	carray;
	GetPrevOperators(_pop, carray);

	_pop->m_byDepth		= (ubyte)_dwCurDepth;
	_pop->m_byInputs	= (ubyte)carray.Count();

	_dwCurDepth+=carray.Count();

	for (udword i=0 ;i<carray.Count(); i++)
	{
		_dwCurDepth--;
		SOperatorDesc* pprevOp = (SOperatorDesc*)carray[i];

		//Get next op from prev
		NOpsDescArray	carray2;
		GetNextOperators(pprevOp, carray2);

		//Compute previous operators
		if (carray2[0] == _pop)
			_ComputeLinks(pprevOp, _pop, _dwCurDepth);
	}

}


//-----------------------------------------------------------------
//!	\brief	Return Prev operators from graphical position
//!
//!	\param	_pop						operator
//!	\param	_carrayPrevOpS	previous operators
//!
//! \note Computed from graphical position
//-----------------------------------------------------------------
void NCompilerOp::GetPrevOperators(SOperatorDesc* _pop, NOpsDescArray& _carrayPrevOpS)
{
	udword dwTop		= _pop->y;
	udword dwLeft	= _pop->x;
	udword dwRight	= _pop->x + _pop->w;

	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		SOperatorDesc* pccurOP = (SOperatorDesc*)m_arrayOps[i];
		if ( pccurOP->y+1 != dwTop
			||(dwLeft<=pccurOP->x && dwRight<=pccurOP->x)
			||(dwLeft>=(pccurOP->x+pccurOP->w) && dwRight>=(pccurOP->x+pccurOP->w)))
			continue;

		_carrayPrevOpS.AddItem(pccurOP);
	}

	//Sort Ops from left to right
	_carrayPrevOpS.Sort(SortOpsLeftToRight);
}


//-----------------------------------------------------------------
//!	\brief	Return Next operators from graphical position
//!
//!	\param	_pop						operator
//!	\param	_carrayPrevOpS	next operators
//!
//! \note Computed from graphical position
//-----------------------------------------------------------------
void NCompilerOp::GetNextOperators(SOperatorDesc* pop, NOpsDescArray& carrayNextOpS)
{
	udword dwBottom	= pop->y + 1;
	udword dwLeft		= pop->x;
	udword dwRight	= pop->x + pop->w;

	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		SOperatorDesc* pccurOP = (SOperatorDesc*)m_arrayOps[i];
		if ( pccurOP->y != dwBottom
			||(dwLeft<=pccurOP->x && dwRight<=pccurOP->x)
			||(dwLeft>=(pccurOP->x+pccurOP->w) && dwRight>=(pccurOP->x+pccurOP->w)))
			continue;

		carrayNextOpS.AddItem(pccurOP);
	}

}

//-----------------------------------------------------------------
//!	\brief	Return final operator (lower down) from graphical position
//!	\param	_pop	operator
//!	\return final operator
//-----------------------------------------------------------------
SOperatorDesc* NCompilerOp::GetFinalOpFrom(SOperatorDesc* pop)
{
	if (!pop)		return null;

	NOpsDescArray	carray;
	for(;;)
	{
		carray.Clear();
		GetNextOperators(pop, carray);
		if (carray.Count()==0)	break;
		pop=(SOperatorDesc*)carray[0];
	}

	return pop;
}


//-----------------------------------------------------------------
//!	\brief	Compute operators sequences
//!	\note		A sequence is a suite of linked operators, splitted for each store
//-----------------------------------------------------------------
void NCompilerOp::ComputeOpsSequences(SCompiledOp* _inoutCompOps)
{
	udword dwIdx;

	//Clear Sequence to Load Global array
	// in order to update sequences toLoad ptr after sequences computing
	m_arrayLoadToSeq.Clear();

	//Create stack for not processed operators
	m_arrayOpsUnlinked.Clear();
	m_arrayOpsUnlinked.AddArray(m_arrayOps);

	///////////////////////////////////////////////////////
	//Process all ops from stack
	while (m_arrayOpsUnlinked.Count())
	{
		SOpsSequence *pstSeq = new SOpsSequence();
		pstSeq->dwOpsCount				= 0;
		pstSeq->dwSeqsToLoadCount	= 0;
		pstSeq->pResult						= null;

		SOperatorDesc* pop = (SOperatorDesc*)m_arrayOpsUnlinked[0];
		//Search Start
		SOperatorDesc* pStartOP = pop;
		while (pStartOP->m_pprevOpToProcess)
			pStartOP = pStartOP->m_pprevOpToProcess;

		//Parse while End or 'Store' operator reached
		SOperatorDesc* pEndOP = pStartOP;
		do
		{
			CompileOperator(pstSeq, pEndOP);
			pEndOP = pEndOP->m_pnextOpToProcess;

			//Break on store operator
			if (pEndOP && pEndOP->pszStoredName && strlen(pEndOP->pszStoredName)!=0)
			{
				//Remove it from unlinked
				udword dwIdx = m_arrayOpsUnlinked.Find(pEndOP);
				if (dwIdx!=(udword)-1)
					m_arrayOpsUnlinked.RemoveItem(dwIdx);		

				break;
			}
		} while (pEndOP->m_pnextOpToProcess);

		//Set Sequence Name For Debug
		if (pEndOP->pszStoredName && strlen(pEndOP->pszStoredName)!=0)
			strcpy(pstSeq->szName, pEndOP->pszStoredName);
		else
			strcpy(pstSeq->szName, "Not Stored!");

		//Add this operators sequence
		pstSeq->pnextOpsSeq = null;
		_inoutCompOps->dwTotalOpsCount+=pstSeq->dwOpsCount;

		//Update sequences link
		if (_inoutCompOps->pfirstOpsSeq==null)
		{
			_inoutCompOps->pfirstOpsSeq = pstSeq; 
			_inoutCompOps->plastOpsSeq  = pstSeq; 
		}
		else 
		{
			_inoutCompOps->plastOpsSeq->pnextOpsSeq = pstSeq;
			_inoutCompOps->plastOpsSeq = pstSeq;
		}

	}
	
	///////////////////////////////////////////////////////
	//Update apSeqsToLoad array
	// take toLoad name then search sequence stored ptr
	for (dwIdx=0; dwIdx<m_arrayLoadToSeq.Count(); dwIdx++)
	{
		SLoadOpToSeqPtr* ploadseqptr = &m_arrayLoadToSeq[dwIdx];

		//Get Stored sequence ptr from stored name
		SOpsSequence *popsSequence = _inoutCompOps->pfirstOpsSeq;
		while (popsSequence)
		{

			if (strcmp(popsSequence->szName, ploadseqptr->pszName)==0)
			{
				//Updating ptr directly in SOpsSequence::apSeqsToLoad
				//*m_arrayLoadToSeq[dwIdx].ppToLoadStoredSeq = popsSequence;	
				//Add 'Load' parameter to SOpsSequence
				SOpsSequence*		pseqLoader = ploadseqptr->pLoaderSeq;
				ploadseqptr->pLoaderSeq->apSeqsToLoad[ploadseqptr->dwLoaderSeqIdx] = popsSequence;
				SOpCallDesc* pcd = &pseqLoader->apopCallDesc[ ploadseqptr->dwLoaderCallDescIdx ];
				pcd->byParamsCount = 1;
				pcd->adwParams[0] = (udword)popsSequence;
			}
			popsSequence = popsSequence->pnextOpsSeq;
		};

	}

}


void NCompilerOp::CompileOperator(SOpsSequence *_pstSeq, SOperatorDesc* _pop)
{
	//Make SOpCallDesc
	_pstSeq->apopCallDesc = (SOpCallDesc*)realloc(_pstSeq->apopCallDesc, sizeof(SOpCallDesc) * (_pstSeq->dwOpsCount+1));
	SOpCallDesc* pcd = &_pstSeq->apopCallDesc[_pstSeq->dwOpsCount];

	pcd->byDepth				= _pop->m_byDepth;
	pcd->byInputsCount	= _pop->m_byInputs;
	pcd->pfncI					= GetOpsInterfaceFromName(_pop->pszIFnc);
	if (pcd->pfncI==null)
		pcd->pfncI=null;	//Debug ##TODO## add error management

	//Copy Parameters
	pcd->byParamsCount	= _pop->byParamsCount;
	NMemCopy(&pcd->adwParams, _pop->adwParams, sizeof(pcd->adwParams));

	// Add ToLoad Operator in m_arrayLoadToSeq
	// in order to update sequences toLoad ptr after sequences computing
	if (_pop->pszToLoad && strlen(_pop->pszToLoad)!=0)
	{
		_pstSeq->apSeqsToLoad[_pstSeq->dwSeqsToLoadCount] = null;

		SLoadOpToSeqPtr st;
		st.pszName = _pop->pszToLoad;
		//st.ppToLoadStoredSeq = &_pstSeq->apSeqsToLoad[_pstSeq->dwSeqsToLoadCount];
		st.pLoaderSeq	= _pstSeq;
		st.dwLoaderSeqIdx = _pstSeq->dwSeqsToLoadCount;
		st.dwLoaderCallDescIdx = _pstSeq->dwOpsCount;
		m_arrayLoadToSeq.AddItem(st);

		_pstSeq->dwSeqsToLoadCount++;
	}


	//Remove it from unlinked
	udword dwIdx = m_arrayOpsUnlinked.Find(_pop);
	if (dwIdx!=(udword)-1)
		m_arrayOpsUnlinked.RemoveItem(dwIdx);		

	//Add one
	_pstSeq->dwOpsCount++;
}


SOpFuncInterface* NCompilerOp::GetOpsInterfaceFromName(const char* _pszName)
{
	NEngineOp* peng = fgCreateOpEngine();
	udword dwcount = peng->GetOpsInterfacesCount();
	while (dwcount--)
	{
		SOpFuncInterface* pfi = peng->GetOpsInterfaceFromIdx(dwcount);
		if (strcmp(pfi->szName, _pszName)==0)
		{
			return pfi;
		}
	}
	return null;
}
