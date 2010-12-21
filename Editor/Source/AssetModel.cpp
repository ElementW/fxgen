//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		AssetModel.cpp
//! \brief	Asset Data Model 
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		23-11-2010
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "AssetModel.h"
#include "OpGraphModel.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NAssetModel Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
NAssetModel::NAssetModel(void)
{
	m_pRootGroup = NNEW(NTreeNode);

	//Asset Name
	NTreeNode* pNewNode = NNEW(NTreeNode);
	pNewNode->SetName("Untitled Asset");
	m_pRootGroup->AddSon(pNewNode);

	//Create a Default Graph
	NOpGraphModel* pgraph = NNEW(NOpGraphModel);
	pgraph->SetName("Untitled Graph");
	pNewNode->GetObjsArray().AddItem(pgraph);

}

NAssetModel::~NAssetModel(void)
{
	
}

//-----------------------------------------------------------------
//!	\brief	Load an asset from a file
//!	\param	_pszFullFileName	file name to load
//!	\return	True if success
//-----------------------------------------------------------------
/*bool NAssetModel::LoadProject(const char* _pszFullFileName)
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
}*/

//-----------------------------------------------------------------
//!	\brief	Save asset to file
//!	\param	_pszFullFileName	file name for save
//!	\return	True if success
//-----------------------------------------------------------------
/*bool NAssetModel::SaveProject(const char* _pszFullFileName)
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
}*/
