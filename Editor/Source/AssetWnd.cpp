//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		AssetWnd.cpp
//! \brief	Asset's windows
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
//			Includes
//-----------------------------------------------------------------
#include "CoreLibPkg.h"
#include "AssetWnd.h"

#include "EditorApp.h"
#include "EventsList.h"

#include "AssetModel.h"
#include "OpGraphModel.h"

//-----------------------------------------------------------------
//			Defines
//-----------------------------------------------------------------
#define ID_ADDFOLDER	100
#define ID_ADDGRAPH		101
#define ID_DELETE			102
#define ID_RENAME			103
#define ID_ADDASSET		104
#define ID_OPENASSET	105

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NAssetWnd Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NAssetWnd::NAssetWnd(void)
{
	m_popsAsset = null;
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NAssetWnd::~NAssetWnd(void)
{
}

//-----------------------------------------------------------------
//!	\brief	control Creation
//-----------------------------------------------------------------
bool NAssetWnd::Create(const char* name, const NRect& rect, NGUIWnd* parent)
{
	NTreeNodeCtrl::Create(name, rect, parent);

	//Init context Menu
	InitCtxMenu();

	//Register Notifications Messages
	OnSelChange = FDelegate(this, (TDelegate)&NAssetWnd::OnTreeSelChange);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Display an asset
//!	\param	_popsAsset	Asset to display
//-----------------------------------------------------------------
void NAssetWnd::DisplayAssetModel(NAssetModel* _popsAsset)
{
	EVT_EXECUTE(EVT_PAGESELECTED, 0, 0 );

	m_popsAsset = _popsAsset;

	//Display
	NTreeNode* pRoot = m_popsAsset->GetRootGroup();
	DisplayTreeNode(pRoot);

	//Screen Update
	Update();

}

//-----------------------------------------------------------------
//!	\brief	control right button down message
//-----------------------------------------------------------------
void NAssetWnd::OnRButtonDown(NPoint pos)
{
	ClientToScreen(pos);
	m_wndMenu.TrackPopupMenu(pos);
}

//-----------------------------------------------------------------
//!	\brief	Init context menu
//-----------------------------------------------------------------
void NAssetWnd::InitCtxMenu()
{
	//Menu creation
	m_wndMenu.Create("Asset:", this);
	m_wndMenu.OnItemClick=FDelegate(this, (TDelegate)&NAssetWnd::OnMenuClick);
	m_wndMenu.AddItem("New Asset",	ID_ADDASSET,	0);
	m_wndMenu.AddItem("Open Asset",	ID_OPENASSET,	0);
	m_wndMenu.AddItem("New Folder",	ID_ADDFOLDER,	0);
	m_wndMenu.AddItem("New Graph",	ID_ADDGRAPH,	0);
	m_wndMenu.AddItem("Delete",			ID_DELETE,		0);
	m_wndMenu.AddItem("Rename",			ID_RENAME,		0);
}

//-----------------------------------------------------------------
//!	\brief	Command message
//-----------------------------------------------------------------
void NAssetWnd::OnMenuClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	udword dwID = pmenu->GetClickedCmdID();
	switch (dwID)
	{
		case ID_ADDFOLDER:AddFolder();	break;
		case ID_ADDGRAPH:	AddGraph();		break;
		case ID_DELETE:		Delete();			break;
		case ID_RENAME:		Rename();			break;
	}

}

//-----------------------------------------------------------------
//!	\brief	Return selected folder
//!	\return	Pointer on selected folder else null
//-----------------------------------------------------------------
NTreeNode* NAssetWnd::GetSelectedFolder()
{
	//Get Selected Object
	NObject* pobj = GetSelectedItemObj();

	//Check if selected item is a TreeNode
	if (pobj!=null)
	{
		if (strcmp(pobj->GetRTClass()->m_pszClassName, "NTreeNode")==0 )
			return (NTreeNode*)pobj;
	}

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Return selected graph
//!	\return	Pointer on selected page else null
//-----------------------------------------------------------------
NOpGraphModel* NAssetWnd::GetSelectedGraph()
{
	//Get Selected Object
	NObject* pobj = GetSelectedItemObj();

	//Check if selected item is a TreeNode
	if (pobj!=null)
	{
		if (strcmp(pobj->GetRTClass()->m_pszClassName, "NOpGraphModel")==0 )
			return (NOpGraphModel*)pobj;
	}

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Add a new folder at selected node
//-----------------------------------------------------------------
void NAssetWnd::AddFolder()
{
	//Get Selected Folder
	NTreeNode* pnodeFolder=GetSelectedFolder();

	if (pnodeFolder==null)	return;

	//Check Parent Folder
	//if (pnodeFolder==null)		pnodeFolder = m_popsAsset->GetRootFolder();

	//Attach a new folder to Parent Folder
	NTreeNode* pNewFolderNode = new NTreeNode;
	pNewFolderNode->SetName("Folder");
	pnodeFolder->AddSon(pNewFolderNode);
	DisplayAssetModel(m_popsAsset);

}

//-----------------------------------------------------------------
//!	\brief	Add a new graph at selected node
//-----------------------------------------------------------------
void NAssetWnd::AddGraph()
{
	//Get Selected Folder
	NTreeNode* pnodeFolder=GetSelectedFolder();
	if (pnodeFolder==null)
		return;

	//Add NOpGraphModel object to this TreeNode
	NObjectArray& array = pnodeFolder->GetObjsArray();

	NOpGraphModel* pgraph = new NOpGraphModel;
	pgraph->AttachToAsset(m_popsAsset);
	pgraph->SetName("Graph");
	array.AddItem(pgraph);

	DisplayAssetModel(m_popsAsset);
}

//-----------------------------------------------------------------
//!	\brief	Delete a folder or a page
//-----------------------------------------------------------------
void NAssetWnd::Delete()
{
	/////////////////////////////////////////////
	// Delete a graph from project
	NOpGraphModel* pgraph = GetSelectedGraph();
	if (pgraph)
	{
		udword dwRet = GetApp()->MessageBox("Do you really want to delete this graph ?", NMB_YESNO);
		if (dwRet==NIDYES)
		{
			//Get Folder for this page
			NTreeNode* pnodefolder = (NTreeNode*)GetParent(pgraph);

			assert(strcmp(pnodefolder->GetRTClass()->m_pszClassName, "NTreeNode")==0 );

			//Delete graph from TreeNode ObjectArray
			NObjectArray& array = pnodefolder->GetObjsArray();
			udword idx = array.Find(pgraph);
			if (idx!=-1)
			{
				array.RemoveItem(idx);
				DisplayAssetModel(m_popsAsset);
			}

		}
		return;
	}

	/////////////////////////////////////////////
	// Delete a folder from project
	NTreeNode* pfolder =	GetSelectedFolder();
	if (pfolder)
	{
		//Get parent for this folder
		NTreeNode* pnodeparent = (NTreeNode*)GetParent(pfolder);
		if (pnodeparent)
		{
			udword dwRet = GetApp()->MessageBox("Do you really want to delete this folder ?", NMB_YESNO);
			if (dwRet==NIDYES)
			{
				udword idx = pnodeparent->FindSon(pfolder);
				if (idx!=-1)
				{
					pnodeparent->DeleteSon(idx);
					DisplayAssetModel(m_popsAsset);
				}
			}
		}

	}

}

//-----------------------------------------------------------------
//!	\brief	Rename a node ###TODO###
//-----------------------------------------------------------------
void NAssetWnd::Rename()
{
	//Get Selected Object
	NObject* pobj = GetSelectedItemObj();

	//Edit label
	if (pobj)
	{
		/*NEnterNameDlg dlg(IDD_ENTERNAME, this);
		dlg.SetName( pobj->GetName() );

		if (IDOK==dlg.DoModal())
		{
			pobj->SetName(dlg.GetName());
			DisplayOperatorsAsset(m_popsAsset);
		}
		*/
	}
}

//-----------------------------------------------------------------
//!	\brief	Event when user change selected node
//-----------------------------------------------------------------
void NAssetWnd::OnTreeSelChange(NObject* _psender)
{
	//TRACE("NAssetWnd::OnSelChange\n");

	//Get Page Selected
	NOpGraphModel* pcurpage = GetSelectedGraph();

	//Send notification message
	EVT_EXECUTE(EVT_PAGESELECTED, (udword)pcurpage, 0 );
}

//-----------------------------------------------------------------
//!	\brief	Select first page found from root
//-----------------------------------------------------------------
void NAssetWnd::SelectFirstGraph()
{
	if (m_popsAsset)
	{
		NTreeNode* pRoot = m_popsAsset->GetRootGroup();
		NObject* pobj = _FindNodeFromClassName(pRoot, "NOpGraphModel");
		if (pobj)
		{
			udword idx = GetItemIdxFromObject(pobj);
			if (idx!=-1)
			{
				SelectItemFromIdx(idx);

				//Expand parent node
				pobj = GetParent(pobj);
				idx = GetItemIdxFromObject(pobj);
				ExpandItemFromIdx(idx, true);

				//Refresh control
				Update();
			}
		}

	}

}

//-----------------------------------------------------------------
//!	\brief	Select first page found from a root node
//-----------------------------------------------------------------
NObject* NAssetWnd::_FindNodeFromClassName(NTreeNode* _pParent, char* _pszClassName)
{
	//Sons
	NTreeNode* pnode = _pParent->GetSon();
	while (pnode)
	{
		//Objects array
		NObjectArray& array = pnode->GetObjsArray();
		for (udword i=0; i<array.Count(); i++)
		{
			NObject* pobj = array[i];
			if (strcmp(pobj->GetRTClass()->m_pszClassName, _pszClassName) == 0)
				return pobj;
		}

		//Son Nodes
		NObject* pfind = _FindNodeFromClassName(pnode, _pszClassName);
		if (pfind)
			return pfind;

		//Next brothers nodes
		pnode = pnode->GetBrother();
	}

	return null;
}
