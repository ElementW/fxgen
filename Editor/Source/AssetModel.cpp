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
	pgraph->AttachToAsset(this);
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

//-----------------------------------------------------------------
//!	\brief	Compile graph for engine asset
//-----------------------------------------------------------------
void NAssetModel::CompileAsset()
{
	m_comp.Clear();
	m_arrayOpsUnlinked.Clear();

	//Parse all operators nodes and make compiled asset
	if (m_pRootGroup!=null)
		_CompileAsset(m_pRootGroup);

	//Compute Compiled version
	ComputeOpsSequences();
}

void NAssetModel::_CompileAsset(NTreeNode* _pParent)
{
	//Sons
	NTreeNode* pnode = _pParent->GetSon();
	while (pnode)
	{
		//For each GrapModels...
		NObjectArray& array = pnode->GetObjsArray();
		for (udword i=0; i<array.Count(); i++)
		{
			NOpGraphModel* pmodel = (NOpGraphModel*)array[i];

			//For each nodes
			udword dwCount = pmodel->GetNodesCount();
			for (udword j=0; j<dwCount; j--)
			{
				NOperatorNode* node = pmodel->GetNodeFromIdx(j);
				m_arrayOpsUnlinked.AddItem(node);
			}


		}

		//Son Nodes
		_CompileAsset(pnode);

		//Next brothers nodes
		pnode = pnode->GetBrother();
	}

}

void NAssetModel::ComputeOpsSequences()
{

	//Create stack for not processed operators
	
	//m_arrayOpsUnlinked.AddArray(m_arrayOps);

	///////////////////////////////////////////////////////
	//Process all ops from stack
	while (m_arrayOpsUnlinked.Count())
	{
		NOperatorNode* pop = (NOperatorNode*)m_arrayOpsUnlinked[0];

		//Search Start
		NOperatorNode* pStartOP = pop;
		while (pStartOP->m_pprevOpToProcess)
			pStartOP = pStartOP->m_pprevOpToProcess;

		//Parse while End
		NOperatorNode* pEndOP = pStartOP;
		do
		{
			//CompileOperator(pstSeq, pEndOP);
			NOperatorFx *prev = null;
			if (pEndOP->m_pprevOpToProcess)
				prev = pEndOP->m_pprevOpToProcess->m_op;

			bool bOutput=false;
			if (pEndOP->m_op->GetUserName() && strlen(pEndOP->m_op->GetUserName())!=0)
				bOutput=true;

			m_comp.AddOpFx(pEndOP->m_op, pStartOP->m_op, prev, bOutput);

			pEndOP = pEndOP->m_pnextOpToProcess;

			//Break on store operator
			/*if (pEndOP && pEndOP->pszStoredName && strlen(pEndOP->pszStoredName)!=0)
			{
				//Remove it from unlinked
				udword dwIdx = m_arrayOpsUnlinked.Find(pEndOP);
				if (dwIdx!=(udword)-1)
					m_arrayOpsUnlinked.RemoveItem(dwIdx);		

				break;
			}*/
		} while (pEndOP);

		//Set Sequence Name For Debug
		/*if (pEndOP && pEndOP->pszStoredName && strlen(pEndOP->pszStoredName)!=0)
			strcpy(pstSeq->szName, pEndOP->pszStoredName);
		else
			strcpy(pstSeq->szName, "Not Stored!");*/


	}

}