#include "AssetModel.h"
#include "OpGraphModel.h"

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
FIMPLEMENT_CLASS(NOperatorNode, NObject);

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NOperatorNode::NOperatorNode()
{
	m_op = null;
	m_pprevOpToProcess = m_pnextOpToProcess = NULL;
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NOperatorNode::~NOperatorNode()
{
	if (m_op!=null)
		NDELETE(m_op, NOperatorFx);

}

//-----------------------------------------------------------------
//!	\brief	duplicate this operator
//!	\return object duplicated
//!	\note		used for copy-cut-paste operations
//-----------------------------------------------------------------
NObject* NOperatorNode::Duplicate()
{
	NOperatorNode* pobj			= (NOperatorNode*)NObject::Duplicate();
	//pobj->m_bInvalided	= true;
	//pobj->m_byDepth			= 0;
	pobj->m_wPosX				= m_wPosX;
	pobj->m_wPosY				= m_wPosY;
	pobj->m_wWidth			= m_wWidth;
	//pobj->m_byInputs		= 0;

	pobj->m_op = (NOperatorFx*)m_op->Duplicate();

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

	_s->PutClass(m_op);

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

	m_op = (NOperatorFx*)_l->GetClass();
	
	return true;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NOpGraphModel class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NOpGraphModel, NObject);

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NOpGraphModel::NOpGraphModel()
{
	m_pprevOp = null;
	m_passet = null;
	m_arrayNodes.SetManageDelete(true);
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NOpGraphModel::~NOpGraphModel()
{
}

//-----------------------------------------------------------------
//!	\brief	Attach to asset
//-----------------------------------------------------------------
void NOpGraphModel::AttachToAsset(NAssetModel* _passet)
{
	m_passet = _passet;
}

//-----------------------------------------------------------------
//!	\brief	Save object into archive
//!	\param	_s archive
//!	\return True if success
//-----------------------------------------------------------------
bool NOpGraphModel::Save(NArchive* _s)
{
	NObject::Save(_s);

	//Save Operators array
	m_arrayNodes.Save(_s);

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Load object from archive
//!	\param	_l archive
//!	\return True if success
//-----------------------------------------------------------------
bool NOpGraphModel::Load(NArchive* _l)
{
	NObject::Load(_l);

	//Load Operators array
	if (!m_arrayNodes.Load(_l))
		return false;

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
udword NOpGraphModel::AddOp(NOperatorNode* _pop)
{
	udword idx = m_arrayNodes.AddItem(_pop);

	_pop->m_op->m_bInvalided = true;

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
udword NOpGraphModel::DeleteOp(NOperatorNode* _pop)
{
	//Invalidate linked operators
	NObjectArray	carray;
	GetNextOperators(_pop, carray);

	for ( udword i=0; i<carray.Count(); i++)
	{
		NOperatorNode* pop = (NOperatorNode*)carray[i];
		pop->m_op->m_bInvalided = true;
		pop->m_pprevOpToProcess = null;
	}

	//Remove From Array
	udword idx = m_arrayNodes.Find(_pop);
	if (idx!=(udword)-1)
	{
		m_arrayNodes.RemoveItem(idx);
		delete _pop;
	}

	//Recomputing links
	ComputeLinks();

	return idx;
}


//-----------------------------------------------------------------
//!	\brief	Delete all operator
//-----------------------------------------------------------------
void NOpGraphModel::DeleteAllOps()
{
	m_arrayNodes.Clear();
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
void NOpGraphModel::MoveOp(NOperatorNode* _pop, sword _x, sword _y)
{
	//Invalidate moved operator
	_pop->m_op->m_bInvalided				= true;
	_pop->m_pnextOpToProcess	= null;
	_pop->m_pprevOpToProcess	= null;

	//Invalidate linked operators
	NObjectArray	carray;
	GetNextOperators(_pop, carray);

	for ( udword i=0; i<carray.Count(); i++)
	{
		NOperatorNode* pop = (NOperatorNode*)carray[i];
		pop->m_pprevOpToProcess = null;
		pop->m_op->m_bInvalided				= true;
	}

	//Move Operator
	_pop->m_wPosX = _x;
	_pop->m_wPosY = _y;

	//Recomputing links
	ComputeLinks();

}


//-----------------------------------------------------------------
//!	\brief	Compute links between operators node
//-----------------------------------------------------------------
void NOpGraphModel::ComputeLinks()
{
	//Create stack for unlinked operators
	m_arrayOpsUnlinked.Clear();
	m_arrayOpsUnlinked.AddArray(m_arrayNodes);

	//Process all ops from stack
	while (m_arrayOpsUnlinked.Count())
	{
		NOperatorNode* pop = (NOperatorNode*)m_arrayOpsUnlinked[0];

		NOperatorNode* popStart = GetStartOpFrom(pop);	//From Graphical position
		NOperatorNode* popFinal = GetFinalOpFrom(pop);	//From Graphical position

		m_pprevOp = null;
		_ComputeLinks(popStart, popFinal, null, 0);
	}

	//Recompile Asset
	if (m_passet)
	{
		m_passet->CompileAsset();
	}

	/////////////////////////////////////////////////////
	//										###Debug###
	/*m_arrayOpsUnlinked.Clear();
	m_arrayOpsUnlinked.AddArray(m_arrayNodes);

	while(m_arrayOpsUnlinked.Count())
	{
		TRACE("SubGraph:\n");
		NOperatorFx* pop = (NOperatorFx*)m_arrayOpsUnlinked[0];

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
//!	\param	__popStart	start operator
//!	\param	_pop				current operator
//!	\param	_pprevop		previous operator
//!	\param	_dwCurDepth	current depth in tree
//!
//! \note Recurse method used by ComputeLinks()
//!				ComputeLinks scan operators from graphical position (bottom -> top)
//-----------------------------------------------------------------
void NOpGraphModel::_ComputeLinks(NOperatorNode* _popStart, NOperatorNode* _pop, NOperatorNode* _pprevop, udword _dwCurDepth)
{
/*	if (_pprevop)
		TRACE("\t_ComputeLinks %s PrevOP %s dwCurDepth %d\n", _pop->GetName(), _pprevop->GetName(), _dwCurDepth );
	else
		TRACE("\t_ComputeLinks %s PrevOP -- dwCurDepth %d\n", _pop->GetName(), _dwCurDepth );*/

	//Update current operator execution link
	if (m_pprevOp!=null)
		_pop->m_pnextOpToProcess = m_pprevOp;
	else
		_pop->m_pnextOpToProcess = null;

	_pop->m_pprevOpToProcess = null;

	/*_pop->m_op->m_proot = null;
	if (_popStart)
		_pop->m_op->m_proot = _popStart->m_op;*/

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

	_pop->m_op->m_byDepth		= (ubyte)_dwCurDepth;
	_pop->m_op->m_byInputs	= (ubyte)carray.Count();

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
			_ComputeLinks(_popStart, pprevOp, _pop, _dwCurDepth);
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
void NOpGraphModel::GetPrevOperators(NOperatorNode* _pop, NObjectArray& _carrayPrevOpS)
{
	int nTop		= _pop->m_wPosY;
	int nLeft		= _pop->m_wPosX;
	int nRight	= _pop->m_wPosX + _pop->m_wWidth;

	for (udword i=0; i<m_arrayNodes.Count(); i++)
	{
		NOperatorNode* pccurOP = (NOperatorNode*)m_arrayNodes[i];
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
void NOpGraphModel::GetNextOperators(NOperatorNode* pop, NObjectArray& carrayNextOpS)
{
	int nBottom	= pop->m_wPosY + 1;
	int nLeft		= pop->m_wPosX;
	int nRight	= pop->m_wPosX + pop->m_wWidth;

	for (udword i=0; i<m_arrayNodes.Count(); i++)
	{
		NOperatorNode* pccurOP = (NOperatorNode*)m_arrayNodes[i];
		if ( pccurOP->m_wPosY != nBottom
			||(nLeft<=pccurOP->m_wPosX && nRight<=pccurOP->m_wPosX)
			||(nLeft>=(pccurOP->m_wPosX+pccurOP->m_wWidth) && nRight>=(pccurOP->m_wPosX+pccurOP->m_wWidth)))
			continue;

		carrayNextOpS.AddItem(pccurOP);
	}

}

//-----------------------------------------------------------------
//!	\brief	Return start operator from graphical position
//!	\param	_pop	operator
//!	\return final operator
//-----------------------------------------------------------------
NOperatorNode* NOpGraphModel::GetStartOpFrom(NOperatorNode* _pop)
{
	if (!_pop)		return null;

	NObjectArray	carray;
	for(;;)
	{
		carray.Clear();
		GetPrevOperators(_pop, carray);
		if (carray.Count()==0)	break;
		_pop=(NOperatorNode*)carray[0];
	}

	return _pop;
}


//-----------------------------------------------------------------
//!	\brief	Return final operator (lower down) from graphical position
//!	\param	_pop	operator
//!	\return final operator
//-----------------------------------------------------------------
NOperatorNode* NOpGraphModel::GetFinalOpFrom(NOperatorNode* pop)
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
/*void NOpGraphModel::GetOpsFromClassName(const char* _pszClassName, NObjectArray& _carray)
{
	for (udword i=0; i<m_arrayNodes.Count(); i++)
	{
		NOperatorFx* pccurOP = (NOperatorFx*)m_arrayNodes[i];
		if (strcmp(pccurOP->GetRTClass()->m_pszClassName, _pszClassName) == 0)
			_carray.AddItem(pccurOP);
	}
}*/

//-----------------------------------------------------------------
//!	\brief	Invalidate all operators to force re-processing
//-----------------------------------------------------------------
void NOpGraphModel::InvalidateAllOps()
{
	for (udword i=0; i<m_arrayNodes.Count(); i++)
	{
		NOperatorFx* pccurOP = (NOperatorFx*)m_arrayNodes[i];
		pccurOP->m_bInvalided			= true;
		pccurOP->m_dwLastUsedTime	= 0;
	}
}

