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
#include "../../Include/CoreLib.h"
#include "EngineOp.h"

//-----------------------------------------------------------------
//                   Variables
//-----------------------------------------------------------------
NEngineOp* gpengineOp = null;



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NOperatorFx class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NOperatorFx::NOperatorFx()
{
	m_pObj = null;
	m_pnextOpToProcess = m_pprevOpToProcess= null;
	m_bInvalided = true;
	m_pcvarsBloc = null;
	
	m_bError = false;
	m_byDepth = 0;
	m_byInputs = 0;
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NOperatorFx::~NOperatorFx()
{
 NEngineOp::GetInstance()->GetBitmapGarbage()->RemoveEntry(&m_pObj);
}



//-----------------------------------------------------------------
//!	\brief	Save object into archive
//!	\param	s archive
//!	\return True if success
//-----------------------------------------------------------------
bool NOperatorFx::Save(NArchive* _s)
{
	NObject::Save(_s);

	//*_s<<m_wPosX;
	//*_s<<m_wPosY;
	//*_s<<m_wWidth;

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Load object from archive
//!	\param	l archive
//!	\return True if success
//-----------------------------------------------------------------
bool NOperatorFx::Load(NArchive* _l)
{
	NObject::Load(_l);

	//*_l>>m_wPosX;
	//*_l>>m_wPosY;
	//*_l>>m_wWidth;

	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCompiledAsset class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NCompiledAsset::NCompiledAsset()
{
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NCompiledAsset::~NCompiledAsset()
{
}


//-----------------------------------------------------------------
//!	\brief	Save object into archive
//!	\param	s archive
//!	\return True if success
//-----------------------------------------------------------------
bool NCompiledAsset::Save(NArchive* _s)
{
	NObject::Save(_s);


	return true;
}


//-----------------------------------------------------------------
//!	\brief	Load object from archive
//!	\param	l archive
//!	\return True if success
//-----------------------------------------------------------------
bool NCompiledAsset::Load(NArchive* _l)
{
	NObject::Load(_l);


	return true;
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
	m_nCurContext		= 0;

	gpengineOp			= this;

	m_bitmapsAlloc.SetManagedClass( "NBitmap" );

	memset(&m_achannels, 0, sizeof(m_achannels));
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NEngineOp::~NEngineOp()
{
}

//-----------------------------------------------------------------
//!	\brief	Static Methods that return an unique Instance
//-----------------------------------------------------------------
NEngineOp*	NEngineOp::GetInstance()
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
}

//-----------------------------------------------------------------
//!	\brief	Compute invalid operators
//! \param	_popFinal	final operator for result
//!	\note		If top stack is invalid, all the stack in the same branch
//!					will been invalided
//-----------------------------------------------------------------
void NEngineOp::ComputeInvaliddOps(NOperatorFx* _popFinal)
{
	if (_popFinal==null)		return;

	//Init
	m_nCurContext = 0;
	memset(m_aStacks, 0, sizeof(m_aStacks));

	//Get Root operator
	NOperatorFx* pcRootOP = GetRootOperator(_popFinal);
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
void NEngineOp::_ComputeInvaliddOps(NOperatorFx* _pop)
{
	NOperatorFx* pccurOP = _pop;	//Start
	while (pccurOP)
	{
		//Invalidate Referenced operators (refTarget)
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperatorFx* prefop = (NOperatorFx*)pccurOP->GetRef(j);
			if (!prefop->m_bParsed)	//Only if not already parsed
			{
				//TRACE("RefTarget Must be invalided <%s> !\n", prefop->GetUserName());

				NOperatorFx* pcrootOpToProcess = GetRootOperator(prefop);
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
		NOperatorFx** pOpsIns = &m_aStacks[m_nCurContext][pccurOP->m_byDepth];
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
			NOperatorFx* preftomeop = (NOperatorFx*)pccurOP->GetRefToMe(j);
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
void NEngineOp::ClearParsedOpsFlags(NOperatorFx* _pop)
{
	//Clear Parsed flags
	NOperatorFx* pccurOP = _pop;	//Start
	while (pccurOP)
	{
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperatorFx* prefop = (NOperatorFx*)pccurOP->GetRef(j);
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
void NEngineOp::Execute(float _ftime, NOperatorFx* _popFinal, float _fDetailFactor/*=1.0f*/, FXGEN_OPSPROCESSCB* _cbOpsProcess/*=NULL*/)
{
	if (_popFinal!=null)
	{

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
			NOperatorFx* pcRootOP = GetRootOperator(_popFinal);
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
void NEngineOp::_Execute(float _ftime, NOperatorFx* _popFinal, float _fDetailFactor)
{
	if (_popFinal==null)		return;

	//Get Root operator
	NOperatorFx* pccurOP = GetRootOperator(_popFinal);
	//TRACE("Root operator for execute <%s>\n", pccurOP->GetName());

	//Execute from root
	while (pccurOP && pccurOP != m_popFinal->m_pnextOpToProcess)
	{
		//Process Referenced operators (refTarget) if marked invalid
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperatorFx* prefop = (NOperatorFx*)pccurOP->GetRef(j);
			if (prefop->m_bInvalided)
			{
				//TRACE("RefTarget Must be executed <%s> !\n", prefop->GetName());
				NOperatorFx* pcrootOpToProcess = GetRootOperator(prefop);
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

				NOperatorFx** pOpsIns = &m_aStacks[m_nCurContext][pccurOP->m_byDepth];
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
void NEngineOp::_ComputeToProcessOpsCount(NOperatorFx* _popFinal)
{
	if (_popFinal==null)		return;

	//Get Root operator
	NOperatorFx* pccurOP = GetRootOperator(_popFinal);

	//Execute from root
	while (pccurOP && pccurOP != m_popFinal->m_pnextOpToProcess)
	{
		//Process Referenced operators (refTarget) if marked invalid
		udword dwRefCount = pccurOP->GetRefCount();
		for (udword j=0; j<dwRefCount; j++)
		{
			NOperatorFx* prefop = (NOperatorFx*)pccurOP->GetRef(j);
			if (prefop->m_bInvalided && !prefop->m_bParsed)
			{
				//TRACE("RefTarget Must be executed <%s> !\n", prefop->GetName());
				NOperatorFx* pcrootOpToProcess = GetRootOperator(prefop);
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
void NEngineOp::InvalidateOp(NOperatorFx* _pop)
{
	//Invalidate operator
	_pop->m_bInvalided = true;

	//Invalidate linked ops
	ComputeInvaliddOps(_pop);
}


//-----------------------------------------------------------------
//!	\brief	Return root operator from an operator
//!	\param	_pop operator
//!	\return root operator
//!	\note		Links must have been computed first
//-----------------------------------------------------------------
NOperatorFx* NEngineOp::GetRootOperator(NOperatorFx* pop)
{
	if (pop==null)		return null;

	NOperatorFx* pccurOP = pop;
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
	m_bitmapsAlloc.Asset(_ppobj, _byObjType);
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
		Execute(_ftime, (NOperatorFx*)m_arrayFinalsOp[i], _fDetailFactor, _cbOpsProcess);
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
		NOperatorFx* pop = (NOperatorFx*)m_arrayFinalsOp[_idx];
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
/*void NEngineOp::GetFinalsResultsList(NObjectArray& _carray, const char* _pszFromGroup, bool _bRecurse)
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

}*/


//-----------------------------------------------------------------
//!	\brief	Return finals ops from a treenode
//!	\param	_pnodeFrom	root tree node
//!	\param	_finalsOp		finals operators array
//!	\param	_bRecurse		true if recurse into sons
//-----------------------------------------------------------------
/*void NEngineOp::GetFinalOps(NTreeNode* _pnodeFrom, NObjectArray& _finalsOp, bool _bRecurse)
{
	//Parse Alls Pages to add 'NStoreResultOp'
	NObjectArray& arrayObjs = _pnodeFrom->GetObjsArray();
	udword dwCount = arrayObjs.Count();
	while (dwCount--)
	{
		NOpGraphModel* ppage = (NOpGraphModel*)arrayObjs[dwCount];
		ppage->GetOpsFromClassName("NStoreResultOp", _finalsOp);
	}

	//Childs
	if (_bRecurse)
	{
		_pnodeFrom = _pnodeFrom->GetSon();
		if (_pnodeFrom)
			GetFinalOps(_pnodeFrom, _finalsOp, _bRecurse);
	}
}*/

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
		Execute(_ftime, (NOperatorFx*)_pFinalResultOp, _fDetailFactor, _cbOpsProcess);
}
