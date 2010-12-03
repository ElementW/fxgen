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

	//Default Graph
	NOpGraphModel* pgraph = NNEW(NOpGraphModel);
	pgraph->SetName("Untitled Graph");
	pNewNode->GetObjsArray().AddItem(pgraph);

}

NAssetModel::~NAssetModel(void)
{
	
}
