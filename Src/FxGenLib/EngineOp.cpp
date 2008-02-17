//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EngineOp.cpp
//! \brief	Operators Engine
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!				Sebastian Olter (qduaty@gmail.com)
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

#ifdef __GNUC__
#include "gcccompat/gcccompat.h"
#endif

//-----------------------------------------------------------------
//                   Variables
//-----------------------------------------------------------------
NEngineOp* gpengineOp = null;

//-----------------------------------------------------------------
//!	\brief	Sort operators from left to right position
//-----------------------------------------------------------------
int SortOpsLeftToRight(const void *elem1, const void *elem2)
{
	NOperator* e1 = *(NOperator**)elem1;
	NOperator* e2 = *(NOperator**)elem2;
	if (e1->m_wPosX<e2->m_wPosX)	return +1;
	if (e1->m_wPosX>e2->m_wPosX)	return -1;
	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NOperator class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NOperator::NOperator()
{
	m_pObj = null;
	m_pnextOpToProcess = null;
	m_bInvalided = true;
	m_pcvarsBloc = null;


	m_pprevOpToProcess = null;
	m_bError = false;

}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NOperator::~NOperator()
{
 NEngineOp::GetEngine()->GetBitmapGarbage()->RemoveEntry(&m_pObj);
}

//-----------------------------------------------------------------
//!	\brief	duplicate this operator
//!	\return object duplicated
//!	\note		used for copy-cut-paste operations
//-----------------------------------------------------------------

NObject* NOperator::Duplicate()
{
	NOperator* pobj			= (NOperator*)NObject::Duplicate();
	pobj->m_bInvalided	= true;
	pobj->m_byDepth			= 0;
	pobj->m_wPosX				= m_wPosX;
	pobj->m_wPosY				= m_wPosY;
	pobj->m_wWidth			= m_wWidth;
	pobj->m_byInputs		= 0;

	return pobj;
}


//-----------------------------------------------------------------
//!	\brief	Save object into archive
//!	\param	s archive
//!	\return True if success
//-----------------------------------------------------------------

bool NOperator::Save(NArchive* _s)
{
	NObject::Save(_s);

	*_s<<m_wPosX;
	*_s<<m_wPosY;
	*_s<<m_wWidth;

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Load object from archive
//!	\param	l archive
//!	\return True if success
//-----------------------------------------------------------------
bool NOperator::Load(NArchive* _l)
{
	NObject::Load(_l);

	*_l>>m_wPosX;
	*_l>>m_wPosY;
	*_l>>m_wWidth;

	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NOperatorsPage class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NOperatorsPage, NObject);

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NOperatorsPage::NOperatorsPage()
{
	m_pprevOp = null;
	m_arrayOps.SetManageDelete(true);
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NOperatorsPage::~NOperatorsPage()
{
}

//-----------------------------------------------------------------
//!	\brief	Save object into archive
//!	\param	_s archive
//!	\return True if success
//-----------------------------------------------------------------
bool NOperatorsPage::Save(NArchive* _s)
{
	NObject::Save(_s);

	//Save Operators array
	m_arrayOps.Save(_s);

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Load object from archive
//!	\param	_l archive
//!	\return True if success
//-----------------------------------------------------------------
bool NOperatorsPage::Load(NArchive* _l)
{
	NObject::Load(_l);

	//Load Operators array
	m_arrayOps.Load(_l);

	//Links
	ComputeLinks();

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Add an operator into page
//!	\param	_pop	operator to add
//!	\return operator index
//!	\note Links between operators are recomputed
//-----------------------------------------------------------------
udword NOperatorsPage::AddOp(NOperator* _pop)
{
	udword idx = m_arrayOps.AddItem(_pop);

	_pop->m_bInvalided = true;

	//Recomputing links
	ComputeLinks();

	return idx;
}


//-----------------------------------------------------------------
//!	\brief	Delete an operator
//!	\param	_pop	operator to delete
//!	\return operator index
//!	\note Links between operators are recomputed
//-----------------------------------------------------------------
udword NOperatorsPage::DeleteOp(NOperator* _pop)
{
	//Invalidate linked operators
	NObjectArray	carray;
	GetNextOperators(_pop, carray);

	for ( udword i=0; i<carray.Count(); i++)
	{
		NOperator* pop = (NOperator*)carray[i];
		pop->m_bInvalided = true;
		pop->m_pprevOpToProcess = null;
	}

	//Remove From Array
	udword idx = m_arrayOps.Find(_pop);
	if (idx!=(udword)-1)
	{
		m_arrayOps.RemoveItem(idx);
		delete _pop;
	}

	//Recomputing links
	ComputeLinks();

	return idx;
}


//-----------------------------------------------------------------
//!	\brief	Delete all operator
//-----------------------------------------------------------------
void NOperatorsPage::DeleteAllOps()
{
	m_arrayOps.Clear();
}

//-----------------------------------------------------------------
//!	\brief	Move an operator into graph
//!
//!	\param	_pop	operator to move
//!	\param	_x	new coordinate
//!	\param	_y	new coordinate
//!
//!	\note Links between operators are recomputed
//-----------------------------------------------------------------
void NOperatorsPage::MoveOp(NOperator* _pop, sword _x, sword _y)
{
	//Invalidate moved operator
	_pop->m_bInvalided				= true;
	_pop->m_pnextOpToProcess	= null;
	_pop->m_pprevOpToProcess	= null;

	//Invalidate linked operators
	NObjectArray	carray;
	GetNextOperators(_pop, carray);

	for ( udword i=0; i<carray.Count(); i++)
	{
		NOperator* pop = (NOperator*)carray[i];
		pop->m_pprevOpToProcess = null;
		pop->m_bInvalided				= true;
	}

	//Move Operator
	_pop->m_wPosX = _x;
	_pop->m_wPosY = _y;

	//Recomputing links
	ComputeLinks();

}


//-----------------------------------------------------------------
//!	\brief	Compute links between operators
//-----------------------------------------------------------------
void NOperatorsPage::ComputeLinks()
{
	//Create stack for unlinked operators
	m_arrayOpsUnlinked.Clear();
	m_arrayOpsUnlinked.AddArray(m_arrayOps);

	//Process all ops from stack
	while (m_arrayOpsUnlinked.Count())
	{
		NOperator* pop = (NOperator*)m_arrayOpsUnlinked[0];
		NOperator* popFinal = GetFinalOpFrom(pop);	//From Graphical position

		m_pprevOp = null;
		_ComputeLinks(popFinal, null, 0);
	}

	/////////////////////////////////////////////////////
	//										###Debug###
	/*m_arrayOpsUnlinked.Clear();
	m_arrayOpsUnlinked.AddArray(m_arrayOps);

	while(m_arrayOpsUnlinked.Count())
	{
		TRACE("SubGraph:\n");
		NOperator* pop = (NOperator*)m_arrayOpsUnlinked[0];

		while (pop)
		{
			TRACE("\tLinks %s\n", pop->GetName());

			udword dwIdx = m_arrayOpsUnlinked.Find(pop);
			if (dwIdx!=-1)			m_arrayOpsUnlinked.RemoveItem(dwIdx);
			pop=pop->m_pnextOpToProcess;
		}
	}*/

}

//-----------------------------------------------------------------
//!	\brief	Compute links between operators
//!
//!	\param	_pop				current operator
//!	\param	_pprevop		previous operator
//!	\param	_dwCurDepth	current depth in tree
//!
//! \note Recurse method used by ComputeLinks()
//!				ComputeLinks scan operators from graphical position (bottom -> top)
//-----------------------------------------------------------------
void NOperatorsPage::_ComputeLinks(NOperator* _pop, NOperator* _pprevop, udword _dwCurDepth)
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
	NObjectArray	carray;
	GetPrevOperators(_pop, carray);

	_pop->m_byDepth		= (ubyte)_dwCurDepth;
	_pop->m_byInputs	= (ubyte)carray.Count();

	_dwCurDepth+=carray.Count();

	for (udword i=0 ;i<carray.Count(); i++)
	{
		_dwCurDepth--;
		NOperator* pprevOp = (NOperator*)carray[i];

		//Get next op from prev
		NObjectArray	carray2;
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
void NOperatorsPage::GetPrevOperators(NOperator* _pop, NObjectArray& _carrayPrevOpS)
{
	int nTop		= _pop->m_wPosY;
	int nLeft		= _pop->m_wPosX;
	int nRight	= _pop->m_wPosX + _pop->m_wWidth;

	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		NOperator* pccurOP = (NOperator*)m_arrayOps[i];
		if ( pccurOP->m_wPosY+1 != nTop
			||(nLeft<=pccurOP->m_wPosX && nRight<=pccurOP->m_wPosX)
			||(nLeft>=(pccurOP->m_wPosX+pccurOP->m_wWidth) && nRight>=(pccurOP->m_wPosX+pccurOP->m_wWidth)))
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
void NOperatorsPage::GetNextOperators(NOperator* pop, NObjectArray& carrayNextOpS)
{
	int nBottom	= pop->m_wPosY + 1;
	int nLeft		= pop->m_wPosX;
	int nRight	= pop->m_wPosX + pop->m_wWidth;

	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		NOperator* pccurOP = (NOperator*)m_arrayOps[i];
		if ( pccurOP->m_wPosY != nBottom
			||(nLeft<=pccurOP->m_wPosX && nRight<=pccurOP->m_wPosX)
			||(nLeft>=(pccurOP->m_wPosX+pccurOP->m_wWidth) && nRight>=(pccurOP->m_wPosX+pccurOP->m_wWidth)))
			continue;

		carrayNextOpS.AddItem(pccurOP);
	}

}


//-----------------------------------------------------------------
//!	\brief	Return final operator (lower down) from graphical position
//!	\param	_pop	operator
//!	\return final operator
//-----------------------------------------------------------------
NOperator* NOperatorsPage::GetFinalOpFrom(NOperator* pop)
{
	if (!pop)		return null;

	NObjectArray	carray;
	for(;;)
	{
		carray.Clear();
		GetNextOperators(pop, carray);
		if (carray.Count()==0)	break;
		pop=(NOperator*)carray[0];
	}

	return pop;
}

//-----------------------------------------------------------------
//!	\brief	Return operators from a class name
//!	\param	_pszClassName	class name
//!	\param	_carray	operators returned
//-----------------------------------------------------------------
void NOperatorsPage::GetOpsFromClassName(const char* _pszClassName, NObjectArray& _carray)
{
	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		NOperator* pccurOP = (NOperator*)m_arrayOps[i];
		if (strcmp(pccurOP->GetRTClass()->m_pszClassName, _pszClassName) == 0)
			_carray.AddItem(pccurOP);
	}
}

//-----------------------------------------------------------------
//!	\brief	Invalidate all operators to force re-processing
//-----------------------------------------------------------------
void NOperatorsPage::InvalidateAllOps()
{
	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		NOperator* pccurOP = (NOperator*)m_arrayOps[i];
		pccurOP->m_bInvalided			= true;
		pccurOP->m_dwLastUsedTime	= 0;
	}
}




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
	m_pRootGroup		= new NTreeNode;
	m_pRootGroup->SetName("Root");

	m_nCurContext		= 0;

	gpengineOp			= this;

	m_bitmapsAlloc.SetManagedClassID( NRTClass::MakeClassID("NBitmap") );

	memset(&m_achannels, 0, sizeof(m_achannels));
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NEngineOp::~NEngineOp()
{
}

//-----------------------------------------------------------------
//!	\brief	Static Methods that return an unique Engine Instance
//-----------------------------------------------------------------
NEngineOp*	NEngineOp::GetEngine()
{
	if (gpengineOp==null)
		gpengineOp = new NEngineOp;

	return gpengineOp;
}

//-----------------------------------------------------------------
//!	\brief	Clear current project
//! \note	Warning clear delete generated medias (bitmaps...)
//-----------------------------------------------------------------
void NEngineOp::Clear()
{
	m_bitmapsAlloc.Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);

	if (m_pRootGroup)		delete m_pRootGroup;
	m_pRootGroup = new NTreeNode;
	m_pRootGroup->SetName("Root");
}

//-----------------------------------------------------------------
//!	\brief	Compute invalid operators
//! \param	_popFinal	final operator for result
//!	\note		If top stack is invalid, all the stack in the same branch
//!					will been invalided
//-----------------------------------------------------------------
void NEngineOp::ComputeInvaliddOps(NOperator* _popFinal)
{
	if (_popFinal==null)		return;

	//Init
	m_nCurContext = 0;
	memset(m_aStacks, 0, sizeof(m_aStacks));

	//Get Root operator
	NOperator* pcRootOP = GetRootOperator(_popFinal);
	//TRACE("Root operator for invalidation <%s>\n", pcRootOP->GetName());

	//Clear Parsed Flags in order to optimize RefTarget that Must be invalided
	ClearParsedOpsFlags(pcRootOP);

	//Compute invalid
	_ComputeInvaliddOps(pcRootOP);
}

//-----------------------------------------------------------------
//!	\brief	Compute invalid operators from one operator
//!	\param	_pop Start Operator
//!	\note		If top stack is invalid, all the stack in the same branch
//!					will been invalided
//!					ClearParsedOpsFlags() must have been called first
//-----------------------------------------------------------------
void NEngineOp::_ComputeInvaliddOps(NOperator* _pop)
{
	NOperator* pccurOP = _pop;	//Start
	while (pccurOP)
	{
		//Invalidate Referenced operators (refTarget)
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperator* prefop = (NOperator*)pccurOP->GetRef(j);
			if (!prefop->m_bParsed)	//Only if not already parsed
			{
				//TRACE("RefTarget Must be invalided <%s> !\n", prefop->GetUserName());

				NOperator* pcrootOpToProcess = GetRootOperator(prefop);
				if (pcrootOpToProcess)
				{
					//TRACE(".Jumping To <%s> !\n", pcrootOpToProcess->GetName());
					m_nCurContext++;
					_ComputeInvaliddOps(pcrootOpToProcess);
					m_nCurContext--;

					if (prefop->m_bInvalided)
					{
						pccurOP->m_bInvalided=true;
					//TRACE("\t%s Invalided from RefTarget\n", pccurOP->GetName());
					}
				}

				prefop->m_bParsed = true;
			}
		}

		//Check if one inputs	is invalid
		// if yes -> invalid current op
		NOperator** pOpsIns = &m_aStacks[m_nCurContext][pccurOP->m_byDepth];
		for (ubyte i=0; i<pccurOP->m_byInputs; i++)
		{
			if (pOpsIns[i]->m_bInvalided)
			{
				pccurOP->m_bInvalided=true;
				//TRACE("\t%s Invalided\n", pccurOP->GetName());
				break;
			}
		}

		//Invalidate operators that reference me(refMaker)
		udword dwRefToMeCount = pccurOP->GetRefToMeCount();
		for (udword j=0; j<dwRefToMeCount; j++)
		{
			NOperator* preftomeop = (NOperator*)pccurOP->GetRefToMe(j);
			//TRACE("RefToMe Must be invalided <%s> !\n", preftomeop->GetUserName());
			if (pccurOP->m_bInvalided)
			{
				preftomeop->m_bInvalided=true;
			}

		}

		//Update Stack
		m_aStacks[m_nCurContext][pccurOP->m_byDepth] = pccurOP;

		pccurOP = pccurOP->m_pnextOpToProcess;
	}

}

//-----------------------------------------------------------------
//!	\brief	Clear Parsed Flags in order to optimize RefTarget that Must be invalided
//! \param	_pop	Start operator
//-----------------------------------------------------------------
void NEngineOp::ClearParsedOpsFlags(NOperator* _pop)
{
	//Clear Parsed flags
	NOperator* pccurOP = _pop;	//Start
	while (pccurOP)
	{
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperator* prefop = (NOperator*)pccurOP->GetRef(j);
			prefop->m_bParsed = false;
		}

		//Update Stack
		m_aStacks[m_nCurContext][pccurOP->m_byDepth] = pccurOP;

		pccurOP = pccurOP->m_pnextOpToProcess;
	}
}


//-----------------------------------------------------------------
//!	\brief	Execute this project from root of operator marked 'show'
//!	\param	_ftime time in ms
//!	\param	_popFinal	final operator for result
//!	\param	_fDetailFactor	Result Detail (Factor x0.5, x1 , x2)
//!	\param	_cbOpsProcess	CallBack
//!	\note		Invalid operators are computed first
//-----------------------------------------------------------------
void NEngineOp::Execute(float _ftime, NOperator* _popFinal, float _fDetailFactor/*=1.0f*/, FXGEN_OPSPROCESSCB* _cbOpsProcess/*=NULL*/)
{
	if (_popFinal!=null)
	{
		NMutexLock lock(m_bEngineLock);
		//Flag operators that need process calls
		ComputeInvaliddOps(_popFinal);

		//Init
		m_nCurContext							= 0;
		m_dwCurProcessOpsCount		= 1;
		m_dwTotalProcessOpsCount	= 0;
		m_popFinal								= _popFinal;
		m_bError									= false;
		m_cbOpsProcess						= _cbOpsProcess;

		memset(m_aStacks, 0, sizeof(m_aStacks));

		//Operators count if a callback is specified
		if (m_cbOpsProcess!=NULL)
		{
			NOperator* pcRootOP = GetRootOperator(_popFinal);
			ClearParsedOpsFlags(pcRootOP);
			_ComputeToProcessOpsCount(_popFinal);
		}

		//Execute
		_Execute(_ftime, _popFinal, _fDetailFactor);
	}

}

//-----------------------------------------------------------------
//!	\brief	Execute this project from root and until final operator
//!	\param	_ftime		time in ms
//!	\param	_popFinal	final operator for result
//!	\param	_fDetailFactor	Result Detail (Factor x0.5, x1 , x2)
//!	\note		Invalid operators MUST have been computed first
//-----------------------------------------------------------------
void NEngineOp::_Execute(float _ftime, NOperator* _popFinal, float _fDetailFactor)
{
	if (_popFinal==null)		return;

	//Get Root operator
	NOperator* pccurOP = GetRootOperator(_popFinal);
	//TRACE("Root operator for execute <%s>\n", pccurOP->GetName());

	//Execute from root
	while (pccurOP && pccurOP != m_popFinal->m_pnextOpToProcess)
	{
		//Process Referenced operators (refTarget) if marked invalid
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperator* prefop = (NOperator*)pccurOP->GetRef(j);
			if (prefop->m_bInvalided)
			{
				//TRACE("RefTarget Must be executed <%s> !\n", prefop->GetName());
				NOperator* pcrootOpToProcess = GetRootOperator(prefop);
				if (pcrootOpToProcess)
				{
					//TRACE(".Jumping To <%s> !\n", pcrootOpToProcess->GetName());

					m_nCurContext++;
					_Execute(_ftime, prefop, _fDetailFactor);
					m_nCurContext--;
				}
			}

		}

		//Process operator if marked invalid
		if (pccurOP->m_bInvalided || pccurOP->m_bError)
		{
			if (!m_bError)
			{
				//TRACE("Process %d/%d\n", m_dwCurProcessOpsCount, m_dwTotalProcessOpsCount);
				if (m_cbOpsProcess)		(*m_cbOpsProcess)(m_dwCurProcessOpsCount, m_dwTotalProcessOpsCount);

				NOperator** pOpsIns = &m_aStacks[m_nCurContext][pccurOP->m_byDepth];
				if (pccurOP->Process(_ftime, pOpsIns, _fDetailFactor)==(udword)-1)
					m_bError = true;

				m_dwCurProcessOpsCount++;
			}

			if (pccurOP->m_pcvarsBloc && pccurOP->m_pcvarsBloc->IsAnimated())
				pccurOP->m_bInvalided = true;
			else
				pccurOP->m_bInvalided = false;

			pccurOP->m_fProcessedTime	= _ftime;

			//TRACE("Process <%s> Ctxt<%d>\n", pccurOP->GetName(), m_nCurContext);
		}

		//Update error state
		pccurOP->m_bError = m_bError;

		//Update Stack
		m_aStacks[m_nCurContext][pccurOP->m_byDepth] = pccurOP;

		pccurOP = pccurOP->m_pnextOpToProcess;
	}
}

//-----------------------------------------------------------------
//!	\brief	Compute operators number to process for final result
//!	\param	_popFinal	final operator for result
//!	\note		Invalid operators MUST have been computed first
//!					Updating m_dwTotalProcessOpsCount variable
//!					ClearParsedOpsFlags() must have been called first
//-----------------------------------------------------------------
void NEngineOp::_ComputeToProcessOpsCount(NOperator* _popFinal)
{
	if (_popFinal==null)		return;

	//Get Root operator
	NOperator* pccurOP = GetRootOperator(_popFinal);

	//Execute from root
	while (pccurOP && pccurOP != m_popFinal->m_pnextOpToProcess)
	{
		//Process Referenced operators (refTarget) if marked invalid
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperator* prefop = (NOperator*)pccurOP->GetRef(j);
			if (prefop->m_bInvalided && !prefop->m_bParsed)
			{
				//TRACE("RefTarget Must be executed <%s> !\n", prefop->GetName());
				NOperator* pcrootOpToProcess = GetRootOperator(prefop);
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
}


//-----------------------------------------------------------------
//!	\brief	Invalidate an operator to force re-processing this and its children
//!	\param	_pop operator to invalid
//!	\note		all dependent operators are invalided too
//-----------------------------------------------------------------
void NEngineOp::InvalidateOp(NOperator* _pop)
{
	//Invalidate operator
	_pop->m_bInvalided = true;

	//Invalidate linked ops
	ComputeInvaliddOps(_pop);
}

//-----------------------------------------------------------------
//!	\brief	Invalidate all operators to force re-processing
//-----------------------------------------------------------------
void NEngineOp::InvalidateAllOps()
{
	NTreeNode* pnode = GetRootGroup();
	_InvalidateAllOps(pnode);
}

void NEngineOp::_InvalidateAllOps(NTreeNode* _pnode)
{
	//Parse Alls Pages...
	NObjectArray& arrayObjs = _pnode->GetObjsArray();
	udword dwCount = arrayObjs.Count();
	while (dwCount--)
	{
		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[dwCount];
		ppage->InvalidateAllOps();
	}

	_pnode = _pnode->GetSon();
	if (_pnode)
		_InvalidateAllOps(_pnode);

}


//-----------------------------------------------------------------
//!	\brief	Return root operator from an operator
//!	\param	_pop operator
//!	\return root operator
//!	\note		Links must have been computed first
//-----------------------------------------------------------------
NOperator* NEngineOp::GetRootOperator(NOperator* pop)
{
	if (pop==null)		return null;

	NOperator* pccurOP = pop;
	while (pccurOP->m_pprevOpToProcess)
	{
		pccurOP = pccurOP->m_pprevOpToProcess;
	}

	return pccurOP;
}

//-----------------------------------------------------------------
//!	\brief	Return a bitmap from ressource management
//!	\param	_ppobj			object pointeur
//!	\param	_byObjType	object allocation type
//-----------------------------------------------------------------
void NEngineOp::GetBitmap(NObject** _ppobj, ubyte _byObjType)
{
	m_bitmapsAlloc.GetInstance(_ppobj, _byObjType);
}

//-----------------------------------------------------------------
//!	\brief	Change a Channel value
//!	\param	_byChannel	channel idx
//!	\param	_etype			type of value
//!	\param	_value			value
//-----------------------------------------------------------------
void NEngineOp::SetChannelValue(ubyte _byChannel, NVarValue& _value)
{
	if (_byChannel<MAX_CHANNELS)
		memcpy(&m_achannels[_byChannel], &_value, sizeof(NVarValue));
}

//-----------------------------------------------------------------
//!	\brief	Return a Channel value
//!	\param	_byChannel	channel idx
//!	\param	_etype			type of value
//!	\param	_value			value
//-----------------------------------------------------------------
void NEngineOp::GetChannelValue(ubyte _byChannel, NVarValue& _outValue)
{
	if (_byChannel<MAX_CHANNELS)
		memcpy(&_outValue, &m_achannels[_byChannel], sizeof(NVarValue));
}


//-----------------------------------------------------------------
//!	\brief	Load a project from a file
//!	\param	_pszFullFileName	file name to load
//!	\return	True if success
//-----------------------------------------------------------------
bool NEngineOp::LoadProject(const char* _pszFullFileName)
{
	//Open Archive
	NFileStream fileStream;
	if(!fileStream.Open(_pszFullFileName)) // non-existing file
		return false;
	NArchive ar(&fileStream);
	if (!ar.Read())
		return false;

	//Clear Project
	Clear();

	//Load Flag (Graph,Compiled...)
	udword dwFlag=0;	//###RESERVED###
	ar>>dwFlag;

	//Load Groups and Pages
	m_pRootGroup->Load(&ar);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Save project to file
//!	\param	_pszFullFileName	file name for save
//!	\return	True if success
//-----------------------------------------------------------------
bool NEngineOp::SaveProject(const char* _pszFullFileName)
{
	bool bRet = false;

	//Open Archive
  NFileStream fileStream;

  if(!fileStream.Open(_pszFullFileName, true))
	return false;

  NArchive ar(&fileStream);
	if (ar.PrepareSave())
	{
		//Save Flag (Graph,Compiled...)
		udword dwFlag=0;	//###RESERVED###
		ar<<dwFlag;

		//Save Groups and Pages
		m_pRootGroup->Save(&ar);

		if (ar.FinalizeSave())			bRet = true;
	}

	return bRet;
}

//-----------------------------------------------------------------
//!	\brief	Keep just final result medias in memory (bitmaps ...)
//-----------------------------------------------------------------
void NEngineOp::CompactMemory(ubyte _byTypeMask/*=OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED*/)
{
	m_bitmapsAlloc.Compact(_byTypeMask,0);
}

//-----------------------------------------------------------------
//!	\brief	Process all operators
//-----------------------------------------------------------------
/*void NEngineOp::ProcessOperators(float _ftime, float _fDetailFactor, FXGEN_RESULTSPROCESSCB* _cbResultsProcess, FXGEN_OPSPROCESSCB* _cbOpsProcess)
{
	//Get finals operators type from opened project (at first time only)
	if (m_arrayFinalsOp.Count()==0)
	{
		m_arrayFinalsOp.SetManageDelete(false);
		GetFinalOps(m_pRootGroup, m_arrayFinalsOp, true);
	}

	//Execute processing at time t
	for (udword i=0; i<m_arrayFinalsOp.Count(); i++)
	{
		if (_cbResultsProcess)	(*_cbResultsProcess)(i+1, m_arrayFinalsOp.Count());
		Execute(_ftime, (NOperator*)m_arrayFinalsOp[i], _fDetailFactor, _cbOpsProcess);
	}
}*/

//-----------------------------------------------------------------
//!	\brief	Return Final result operator count
//!	\return	operators count
//-----------------------------------------------------------------
/*udword NEngineOp::GetFinalResultCount()
{
	return (m_arrayFinalsOp.Count());
}

//-----------------------------------------------------------------
//!	\brief	Return Final result operator bitmap
//! \param	_idx	operator indice
//! \return	Bitmap Ptr if success else null
//-----------------------------------------------------------------
NBitmap* NEngineOp::GetFinalResultBitmapByIdx(udword _idx)
{
	if (_idx<m_arrayFinalsOp.Count())
	{
		NOperator* pop = (NOperator*)m_arrayFinalsOp[_idx];
		NBitmap* bmp = (NBitmap*)pop->m_pObj;
		bmp->SetName(pop->GetUserName());
		return bmp;
	}

	return null;
}*/


//-----------------------------------------------------------------
//!	\brief		Return Final Result array
//!	\param	_carray					Final Result Operators Array
//!	\param	_pszFromGroup	Start Group Name (can be null for Root)
//!	\param	_bRecurse			true if recurse into sons
//-----------------------------------------------------------------
void NEngineOp::GetFinalsResultsList(NObjectArray& _carray, const char* _pszFromGroup, bool _bRecurse)
{
	if (m_pRootGroup==null)		return;

	//Do Not delete final results Operators on destructor !
	_carray.SetManageDelete(false);

	//Get Start Node
	NTreeNode* pFromGroup = m_pRootGroup;

	if (_pszFromGroup!=NULL)
		pFromGroup = m_pRootGroup->GetSonFromName(_pszFromGroup);

	//Get Final Ops List
	GetFinalOps(pFromGroup, _carray, _bRecurse);

}


//-----------------------------------------------------------------
//!	\brief	Return finals ops from a treenode
//!	\param	_pnodeFrom	root tree node
//!	\param	_finalsOp		finals operators array
//!	\param	_bRecurse		true if recurse into sons
//-----------------------------------------------------------------
void NEngineOp::GetFinalOps(NTreeNode* _pnodeFrom, NObjectArray& _finalsOp, bool _bRecurse)
{
	//Parse Alls Pages to add 'NStoreResultOp'
	NObjectArray& arrayObjs = _pnodeFrom->GetObjsArray();
	udword dwCount = arrayObjs.Count();
	while (dwCount--)
	{
		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[dwCount];
		ppage->GetOpsFromClassName("NStoreResultOp", _finalsOp);
	}

	//Childs
	if (_bRecurse)
	{
		_pnodeFrom = _pnodeFrom->GetSon();
		if (_pnodeFrom)
			GetFinalOps(_pnodeFrom, _finalsOp, _bRecurse);
	}
}

//-----------------------------------------------------------------
//!	\brief	Process a final result operator
//!	\param	_pFinalResultOp			Operator to process
//!	\param	_ftime								Time in ms
//!	\param	_fDetailFactor				Result Detail (Factor x0.5, x1 , x2)
//!	\param	_cbOpsProcess				CallBack
//-----------------------------------------------------------------
void NEngineOp::ProcessFinalResult(NStoreResultOp* _pFinalResultOp, float _ftime, float _fDetailFactor, FXGEN_OPSPROCESSCB* _cbOpsProcess)
{
	if (_pFinalResultOp)
		Execute(_ftime, (NOperator*)_pFinalResultOp, _fDetailFactor, _cbOpsProcess);
}
