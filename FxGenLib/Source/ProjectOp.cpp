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
#include "ProjectOp.h"
#include "EngineOp.h"
//#include "MainOps.h"

//-----------------------------------------------------------------
//                   Variables
//-----------------------------------------------------------------
NOperatorsProject* gpengineOp = null;

//-----------------------------------------------------------------
//!	\brief	Sort operators from left to right position
//-----------------------------------------------------------------
int SortOpsLeftToRight(const void *elem1, const void *elem2)
{
	NOperatorNode* e1 = *(NOperatorNode**)elem1;
	NOperatorNode* e2 = *(NOperatorNode**)elem2;
	if (e1->m_wPosX<e2->m_wPosX)	return +1;
	if (e1->m_wPosX>e2->m_wPosX)	return -1;
	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NOperatorNode class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NOperatorNode::NOperatorNode()
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
NOperatorNode::~NOperatorNode()
{
 NEngineOp::GetEngine()->GetBitmapGarbage()->RemoveEntry(&m_pObj);
}

//-----------------------------------------------------------------
//!	\brief	duplicate this operator
//!	\return object duplicated
//!	\note		used for copy-cut-paste operations
//-----------------------------------------------------------------
NObject* NOperatorNode::Duplicate()
{
	NOperatorNode* pobj			= (NOperatorNode*)NObject::Duplicate();
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
bool NOperatorNode::Save(NArchive* _s)
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
bool NOperatorNode::Load(NArchive* _l)
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
FIMPLEMENT_CLASS(NOperatorsPage, NObject);

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
	if (!m_arrayOps.Load(_l))
		return false;

	//Compute Links
	ComputeLinks();

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Add an operator into page
//!	\param	_pop	operator to add
//!	\return operator index
//!	\note Links between operators are recomputed
//-----------------------------------------------------------------
udword NOperatorsPage::AddOp(NOperatorNode* _pop)
{
	udword idx = m_arrayOps.AddItem(_pop);

	_pop->m_bInvalided = true;

	//Compute Links
	ComputeLinks();

	return idx;
}


//-----------------------------------------------------------------
//!	\brief	Delete an operator
//!	\param	_pop	operator to delete
//!	\return operator index
//!	\note Links between operators are recomputed
//-----------------------------------------------------------------
udword NOperatorsPage::DeleteOp(NOperatorNode* _pop)
{
	//Invalidate linked operators
	NObjectArray	carray;
	GetNextOperators(_pop, carray);

	for ( udword i=0; i<carray.Count(); i++)
	{
		NOperatorNode* pop = (NOperatorNode*)carray[i];
		pop->m_bInvalided = true;
		pop->m_pprevOpToProcess = null;
	}

	//Remove From Array
	udword idx = m_arrayOps.Find(_pop);
	if (idx!=(udword)-1)
	{
		m_arrayOps.RemoveItem(idx);
		NDELETE(_pop, NOperatorNode);
	}

	//Compute Links
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
void NOperatorsPage::MoveOp(NOperatorNode* _pop, sword _x, sword _y)
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
		NOperatorNode* pop = (NOperatorNode*)carray[i];
		pop->m_pprevOpToProcess = null;
		pop->m_bInvalided				= true;
	}

	//Move Operator
	_pop->m_wPosX = _x;
	_pop->m_wPosY = _y;

	//Compute Links
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
		NOperatorNode* pop = (NOperatorNode*)m_arrayOpsUnlinked[0];
		NOperatorNode* popFinal = GetFinalOpFrom(pop);	//From Graphical position

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
		NOperatorNode* pop = (NOperatorNode*)m_arrayOpsUnlinked[0];

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
void NOperatorsPage::_ComputeLinks(NOperatorNode* _pop, NOperatorNode* _pprevop, udword _dwCurDepth)
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
		NOperatorNode* pprevOp = (NOperatorNode*)carray[i];

		//Get next op from prev
		NObjectArray	carray2;
		GetNextOperators(pprevOp, carray2);

		//Compute previous operators
		if (carray2[0] == _pop)
			_ComputeLinks(pprevOp, _pop, _dwCurDepth);
	}

}

//-----------------------------------------------------------------
//!	\brief	Compute operators sequences
//!	\note		A sequence is a suite of linked operators
//-----------------------------------------------------------------
void NOperatorsPage::ComputeOpsSequences(SCompiledOp* _inoutCompOps)
{
	udword dwIdx;

	//Create stack for not processed operators
	m_arrayOpsUnlinked.Clear();
	m_arrayOpsUnlinked.AddArray(m_arrayOps);

	//Process all ops from stack
	while (m_arrayOpsUnlinked.Count())
	{
		SOpsSequence *pstSeq = new SOpsSequence();
		pstSeq->dwOpsCount				= 0;
		pstSeq->dwSeqsToLoadCount	= 0;
		pstSeq->dwOpsToLoadCount	= 0;

		NOperatorNode* pop = (NOperatorNode*)m_arrayOpsUnlinked[0];
		//Search Start
		NOperatorNode* pStartOP = pop;
		while (pStartOP->m_pprevOpToProcess)
		{
			CompileOperator(pstSeq, pStartOP);
			pStartOP = pStartOP->m_pprevOpToProcess;
		}

		//Parse and search End or 'Store' operator
		NOperatorNode* pEndOP = pop;
		while (pEndOP->m_pnextOpToProcess)
		{
			CompileOperator(pstSeq, pEndOP);
			pEndOP = pEndOP->m_pnextOpToProcess;

			//Break on store operator
			if (strcmp(pEndOP->GetRTClass()->m_pszClassName, "NStoreOp")==0)
			{
				break;
			}
		}

		//Set Sequence Name For Debug
		if (pEndOP->GetUserName()!=null)
			strcpy(pstSeq->szName, pEndOP->GetUserName());
		else
			strcpy(pstSeq->szName, "Not Stored!");

		//Remove Start op
		dwIdx = m_arrayOpsUnlinked.Find(pStartOP);
		if (dwIdx!=(udword)-1)
		{
			CompileOperator(pstSeq, pStartOP);
		}

		//Remove End op
		dwIdx = m_arrayOpsUnlinked.Find(pEndOP);
		if (dwIdx!=(udword)-1)
		{
			CompileOperator(pstSeq, pEndOP);
		}

		//Add this operators sequence
		pstSeq->popStart = pStartOP;
		pstSeq->popEnd 	= pEndOP;
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
	
	//Now Replace NOperatorNode* by SOpsSequence* (apopsToLoad->apSeqsToLoad)
	SOpsSequence *popsSequence = _inoutCompOps->pfirstOpsSeq;
	while (popsSequence)
	{
		popsSequence->dwSeqsToLoadCount = popsSequence->dwOpsToLoadCount;

		for (udword i=0; i<popsSequence->dwOpsToLoadCount; i++)
		{
			popsSequence->apSeqsToLoad[i] = popsSequence->apopsToLoad[i]->m_pSequence;
		}
		popsSequence = popsSequence->pnextOpsSeq;
	};
	

}

void NOperatorsPage::CompileOperator(SOpsSequence *_pstSeq, NOperatorNode* _pop)
{
	_pstSeq->dwOpsCount++;
	_pop->m_pSequence = _pstSeq;

	// Store ToLoad Operator
	if (strcmp(_pop->GetRTClass()->m_pszClassName, "NLoadOp")==0)
	{
		NOperatorNode* prefop = (NOperatorNode*)_pop->GetRef(0);
		//prefop->m_pSequence	= _pstSeq;

		_pstSeq->apopsToLoad[_pstSeq->dwOpsToLoadCount] = prefop;
		_pstSeq->dwOpsToLoadCount++;
	}

	//Remove it from unlinked
	udword dwIdx = m_arrayOpsUnlinked.Find(_pop);
	if (dwIdx!=(udword)-1)
		m_arrayOpsUnlinked.RemoveItem(dwIdx);		
}


//-----------------------------------------------------------------
//!	\brief	Return Prev operators from graphical position
//!
//!	\param	_pop						operator
//!	\param	_carrayPrevOpS	previous operators
//!
//! \note Computed from graphical position
//-----------------------------------------------------------------
void NOperatorsPage::GetPrevOperators(NOperatorNode* _pop, NObjectArray& _carrayPrevOpS)
{
	int nTop		= _pop->m_wPosY;
	int nLeft		= _pop->m_wPosX;
	int nRight	= _pop->m_wPosX + _pop->m_wWidth;

	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		NOperatorNode* pccurOP = (NOperatorNode*)m_arrayOps[i];
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
void NOperatorsPage::GetNextOperators(NOperatorNode* pop, NObjectArray& carrayNextOpS)
{
	int nBottom	= pop->m_wPosY + 1;
	int nLeft		= pop->m_wPosX;
	int nRight	= pop->m_wPosX + pop->m_wWidth;

	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		NOperatorNode* pccurOP = (NOperatorNode*)m_arrayOps[i];
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
NOperatorNode* NOperatorsPage::GetFinalOpFrom(NOperatorNode* pop)
{
	if (!pop)		return null;

	NObjectArray	carray;
	for(;;)
	{
		carray.Clear();
		GetNextOperators(pop, carray);
		if (carray.Count()==0)	break;
		pop=(NOperatorNode*)carray[0];
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
		NOperatorNode* pccurOP = (NOperatorNode*)m_arrayOps[i];
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
		NOperatorNode* pccurOP = (NOperatorNode*)m_arrayOps[i];
		pccurOP->m_bInvalided			= true;
		pccurOP->m_dwLastUsedTime	= 0;
	}
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NOperatorsProject class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NOperatorsProject::NOperatorsProject()
{
	m_pRootGroup		= NNEW(NTreeNode);
	m_pRootGroup->SetName("Root");

	m_nCurContext		= 0;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NOperatorsProject::~NOperatorsProject()
{
	if (m_pRootGroup)
		NDELETE(m_pRootGroup, NTreeNode);
}

//-----------------------------------------------------------------
//!	\brief	Clear current project
//! \note	Warning clear delete generated medias (bitmaps...)
//-----------------------------------------------------------------
void NOperatorsProject::Clear()
{
	if (m_pRootGroup)		NDELETE(m_pRootGroup, NTreeNode);
	m_pRootGroup = NNEW(NTreeNode);
	m_pRootGroup->SetName("Root");
}



//-----------------------------------------------------------------
//!	\brief	Return root operator from an operator
//!	\param	_pop operator
//!	\return root operator
//!	\note		Links must have been computed first
//-----------------------------------------------------------------
NOperatorNode* NOperatorsProject::GetRootOperator(NOperatorNode* pop)
{
	if (pop==null)		return null;

	NOperatorNode* pccurOP = pop;
	while (pccurOP->m_pprevOpToProcess)
	{
		pccurOP = pccurOP->m_pprevOpToProcess;
	}

	return pccurOP;
}


//-----------------------------------------------------------------
//!	\brief	Load a project from a file
//!	\param	_pszFullFileName	file name to load
//!	\return	True if success
//-----------------------------------------------------------------
bool NOperatorsProject::LoadProject(const char* _pszFullFileName)
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
	return m_pRootGroup->Load(&ar);
}

//-----------------------------------------------------------------
//!	\brief	Save project to file
//!	\param	_pszFullFileName	file name for save
//!	\return	True if success
//-----------------------------------------------------------------
bool NOperatorsProject::SaveProject(const char* _pszFullFileName)
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
//!	\brief		Return Final Result array
//!	\param	_carray					Final Result Operators Array
//!	\param	_pszFromGroup	Start Group Name (can be null for Root)
//!	\param	_bRecurse			true if recurse into sons
//-----------------------------------------------------------------
void NOperatorsProject::GetFinalsResultsList(NObjectArray& _carray, const char* _pszFromGroup, bool _bRecurse)
{
	if (m_pRootGroup==null)		return;

	//Do Not delete final results Operators on destructor !
	_carray.SetManageDelete(false);

	//Get Start Node
	NTreeNode* pFromGroup = m_pRootGroup;

	if (_pszFromGroup!=null)
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
void NOperatorsProject::GetFinalOps(NTreeNode* _pnodeFrom, NObjectArray& _finalsOp, bool _bRecurse)
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
//!	\brief	Compile graph to linear form
//!	\return	True if success
//-----------------------------------------------------------------
bool NOperatorsProject::CompileGraph(SCompiledOp* _inoutCompOps, bool _bOnlyFinalsResults)
{
	bool bRet = false;
	//udword i;
	
	//Init
	_inoutCompOps->dwTotalOpsCount	= 0;
	_inoutCompOps->pfirstOpsSeq		= null;
	_inoutCompOps->plastOpsSeq			= null;
	
	//For each pages compute operators sequences
	ComputeOpsSequences(_inoutCompOps, m_pRootGroup);
	
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

	//Designer Mode => Get Root operator
	/*if (_popFinalSelected!=null)
	{
		NOperatorNode* pccurOP = GetRootOperator(_popFinalSelected);
		_inoutCompOps->arrayFinals.Add(_popFinalSelected);
	} else {
		GetFinalsResultsList(_inoutCompOps->arrayFinals, NULL, true);
	}*/

	//Precompile reference target (LoadOp that reference a SaveOp)
	//Dans NEngineOp::Execute, faire un callop (avec un contexte d'execution different) si savedop processtime!=t
	//Dans NEngineOp::Execute, gerer un point d'arret (pour le markshow) !
	//Optimiser le GetRootOp
	//	.solution1, mettre un pointeur vers root dans le SavedOp
	//	.solution2, le loadop contient le pointeur sur root pour faire le callop et sur le result pour savoir si le graph est invalide.
	//	.solution3, je pense la meilleure, creer une structure de description par graphs ou un tableau de pointeur des ops a executer (donc plus de next* et prev*)
	
	//Compile sub-graph for each finals operators
	/*for (i=0; i<_inoutCompOps->arrayFinals.Count(); i++)
	{
		NStoreResultOp* pFinalResultOp = (NStoreResultOp*)_inoutCompOps->arrayFinals[i];
		//Get Root Operator
		
		//
		udword dwOpsCount = 0;
		NOperatorNode* pccurOP = pFinalResultOp;
		while (pccurOP->m_pprevOpToProcess)
		{
			dwOpsCount++;
			pccurOP = pccurOP->m_pprevOpToProcess;
		}

		_inoutCompOps->dwTotalOpsCount+=dwOpsCount;
	}*/


	return bRet;
}

//-----------------------------------------------------------------
//!	\brief	Compute operators sequences for each pages
//!	\note		A node is a suite of linked operators
//-----------------------------------------------------------------
void NOperatorsProject::ComputeOpsSequences(SCompiledOp* _inoutCompOps, NTreeNode* _pnodeFrom)
{
	//Parse Alls Pages for this node
	NObjectArray& arrayObjs = _pnodeFrom->GetObjsArray();
	udword dwCount = arrayObjs.Count();
	while (dwCount--)
	{
		NOperatorsPage* ppage = (NOperatorsPage*)arrayObjs[dwCount];
		ppage->ComputeOpsSequences(_inoutCompOps);
	}

	//Parse Children
	_pnodeFrom = _pnodeFrom->GetSon();
	if (_pnodeFrom)
		ComputeOpsSequences(_inoutCompOps, _pnodeFrom);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*
a faire
---------
x creer une classe NOperatorsProject
- compilation du graph vers une forme exploitable sans operateurs...
	.voir les operateurs comme des nodes d'un graph
	.generer un tableau de machines executables en series
	.supprimer la notion de reference entre les machines
		.LoadOp recupere sa reference depuis la resource
		.Partir du final result puis Enregister les stored references en
		les mettant a chaquefois en haut d'une pile d'un tableau stored
		.Un LoadOP => reference un opstored par ID
	.supprime le Next et Prev Operator pour le mode compile
	.plus besoin d'instancer un op par node
		.faire un tableau de machines instancees
		.les op sont sauvegardes par ID machine avec le tableau des variables
- Attention il faut pouvoir recuperer la resource par operateur pour le mode edition
	.prevoir pour le mode compile, une technique plus optimale pour l'usage memoire
	.retirer la methode getResource() de l'operator::Process

Solution 1
-------------
- garder la classe NOperatorNode mais faire des defines pour le mode edition
- rajouter un contexte pour la serialization dans le mode compile ou edition

*/