//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Operator.h
//! \brief	Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		23-09-2009
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
#include "Operator.h"

#ifdef __GNUC__
//#include "gcccompat/gcccompat.h"
#endif

//-----------------------------------------------------------------
//                   Variables
//-----------------------------------------------------------------


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
	m_pcvarsBloc = null;
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NOperatorNode::~NOperatorNode()
{
}

//-----------------------------------------------------------------
//!	\brief	duplicate this operator
//!	\return object duplicated
//!	\note		used for copy-cut-paste operations
//-----------------------------------------------------------------
NObject* NOperatorNode::Duplicate()
{
	NOperatorNode* pobj			= (NOperatorNode*)NObject::Duplicate();
	pobj->m_wPosX				= m_wPosX;
	pobj->m_wPosY				= m_wPosY;
	pobj->m_wWidth			= m_wWidth;
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

	//Links
	//ComputeLinks();

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

	//_pop->m_bInvalided = true;

	//Recomputing links
	//ComputeLinks();

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
	udword idx = 0;
/*
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
		delete _pop;
	}

	//Recomputing links
	//ComputeLinks();
*/
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
/*
	//Invalidate moved operator
	_pop->m_bInvalided				= true;

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

	//Recomputing links
	ComputeLinks();
*/
}



//-----------------------------------------------------------------
//!	\brief	Invalidate all operators to force re-processing
//-----------------------------------------------------------------
void NOperatorsPage::InvalidateAllOps()
{
/*	for (udword i=0; i<m_arrayOps.Count(); i++)
	{
		NOperatorNode* pccurOP = (NOperatorNode*)m_arrayOps[i];
		pccurOP->m_bInvalided			= true;
		pccurOP->m_dwLastUsedTime	= 0;
	}*/
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
	m_pRootGroup		= new NTreeNode;
	m_pRootGroup->SetName("Root");

	m_bitmapsAlloc.SetManagedClassID( NRTClass::MakeClassID("NBitmap") );
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NOperatorsProject::~NOperatorsProject()
{
}

//-----------------------------------------------------------------
//!	\brief	Static Methods that return an unique Engine Instance
//-----------------------------------------------------------------
NOperatorsProject*	NOperatorsProject::GetEngine()
{
/*	if (gpengineOp==null)
		gpengineOp = new NOperatorsProject;

	return gpengineOp;*/
	return null;
}

//-----------------------------------------------------------------
//!	\brief	Clear current project
//! \note	Warning clear delete generated medias (bitmaps...)
//-----------------------------------------------------------------
void NOperatorsProject::Clear()
{
	m_bitmapsAlloc.Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);

	if (m_pRootGroup)		delete m_pRootGroup;
	m_pRootGroup = new NTreeNode;
	m_pRootGroup->SetName("Root");
}



//-----------------------------------------------------------------
//!	\brief	Execute this project from root of operator marked 'show'
//!	\param	_ftime time in ms
//!	\param	_popFinal	final operator for result
//!	\param	_fDetailFactor	Result Detail (Factor x0.5, x1 , x2)
//!	\param	_cbOpsProcess	CallBack
//!	\note		Invalid operators are computed first
//-----------------------------------------------------------------
void NOperatorsProject::Execute(float _ftime, NOperatorNode* _popFinal, float _fDetailFactor)
{

}




//-----------------------------------------------------------------
//!	\brief	Invalidate an operator to force re-processing this and its children
//!	\param	_pop operator to invalid
//!	\note		all dependent operators are invalided too
//-----------------------------------------------------------------
void NOperatorsProject::InvalidateOp(NOperatorNode* _pop)
{
	//Invalidate operator
	//_pop->m_bInvalided = true;

	//Invalidate linked ops
	//ComputeInvaliddOps(_pop);
}

//-----------------------------------------------------------------
//!	\brief	Invalidate all operators to force re-processing
//-----------------------------------------------------------------
void NOperatorsProject::InvalidateAllOps()
{
	NTreeNode* pnode = GetRootGroup();
	_InvalidateAllOps(pnode);
}

void NOperatorsProject::_InvalidateAllOps(NTreeNode* _pnode)
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
NOperatorNode* NOperatorsProject::GetRootOperator(NOperatorNode* pop)
{
/*	if (pop==null)		return null;

	NOperatorNode* pccurOP = pop;
	while (pccurOP->m_pprevOpToProcess)
	{
		pccurOP = pccurOP->m_pprevOpToProcess;
	}

	return pccurOP;*/
	return null;
}

//-----------------------------------------------------------------
//!	\brief	Return a bitmap from ressource management
//!	\param	_ppobj			object pointeur
//!	\param	_byObjType	object allocation type
//-----------------------------------------------------------------
void NOperatorsProject::GetBitmap(NObject** _ppobj, ubyte _byObjType)
{
	m_bitmapsAlloc.GetInstance(_ppobj, _byObjType);
}

//-----------------------------------------------------------------
//!	\brief	Change a Channel value
//!	\param	_byChannel	channel idx
//!	\param	_etype			type of value
//!	\param	_value			value
//-----------------------------------------------------------------
void NOperatorsProject::SetChannelValue(ubyte _byChannel, NVarValue& _value)
{
//	if (_byChannel<MAX_CHANNELS)
//		memcpy(&m_achannels[_byChannel], &_value, sizeof(NVarValue));
}

//-----------------------------------------------------------------
//!	\brief	Return a Channel value
//!	\param	_byChannel	channel idx
//!	\param	_etype			type of value
//!	\param	_value			value
//-----------------------------------------------------------------
void NOperatorsProject::GetChannelValue(ubyte _byChannel, NVarValue& _outValue)
{
//	if (_byChannel<MAX_CHANNELS)
//		memcpy(&_outValue, &m_achannels[_byChannel], sizeof(NVarValue));
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
//!	\brief	Keep just final result medias in memory (bitmaps ...)
//-----------------------------------------------------------------
//void NOperatorsProject::CompactMemory(ubyte _byTypeMask/*=OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED*/)
//{
//	m_bitmapsAlloc.Compact(_byTypeMask,0);
//}
