//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ProjectWnd.cpp
//! \brief	Project's windows
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
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
#include "pch.h"
#include "projectwnd.h"
#include "resource.h"
#include "EnterNameDlg.h"

//-----------------------------------------------------------------
//			Defines
//-----------------------------------------------------------------
#define ID_ADDGROUP		100
#define ID_ADDPAGE		101
#define ID_DELETE			102
#define ID_RENAME			103

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NProjectWnd Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NProjectWnd::NProjectWnd(void)
{
	m_popsProject = null;
	m_hMenu=null;
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NProjectWnd::~NProjectWnd(void)
{
	if (m_hMenu)				::DestroyMenu(m_hMenu);
}

//-----------------------------------------------------------------
//!	\brief	control Creation
//-----------------------------------------------------------------
bool NProjectWnd::Create(char* name, NRect& rect, NWnd* parent)
{
	NTreeNodeCtrl::Create(name, rect, parent);

	//Init context Menu
	InitCtxMenu();

	//Messages
	OnSelChange = FDelegate(this, (TDelegate)&NProjectWnd::OnTreeSelChange);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Display a new operators project
//!	\param	_popsProject	Project to display
//-----------------------------------------------------------------
void NProjectWnd::DisplayOperatorsProject(NEngineOp* _popsProject)
{
	EVT_EXECUTE(EVT_PAGESELECTED, 0, 0 );

	m_popsProject = _popsProject;

	//Display
	NTreeNode* pRoot = m_popsProject->GetRootGroup();
	DisplayTreeNode(pRoot);

	//Screen Update
	Update();

}

//-----------------------------------------------------------------
//!	\brief	control right button down message
//-----------------------------------------------------------------
void NProjectWnd::OnRightButtonDown(udword flags, NPoint pos)
{
	POINT pt;	::GetCursorPos(&pt);	//Cursor position even with keyboard 'Context key'

	NPoint pT(pt.x, pt.y);
	m_wndMenu.TrackPopupMenu(pT);
}

//-----------------------------------------------------------------
//!	\brief	Init context menu
//-----------------------------------------------------------------
void NProjectWnd::InitCtxMenu()
{
	//Menu creation
	m_wndMenu.Create("Project:", this);
	m_wndMenu.AddItem("Add Group",		ID_ADDGROUP, 0);
	m_wndMenu.AddItem("Add Page",		ID_ADDPAGE,		 0);
	m_wndMenu.AddItem("Delete",			ID_DELETE,		 0);
	m_wndMenu.AddItem("Rename",			ID_RENAME,		 0);

}

//-----------------------------------------------------------------
//!	\brief	Command message
//!	\param	_id	Command ID
//-----------------------------------------------------------------
void NProjectWnd::OnCommand(udword _id)
{
	switch (_id)
	{
		case ID_ADDGROUP:	AddGroup();		break;
		case ID_ADDPAGE:	AddPage();		break;
		case ID_DELETE:		Delete();			break;
		case ID_RENAME:		Rename();			break;
	}

}

//-----------------------------------------------------------------
//!	\brief	Return selected group
//!	\return	Pointer on selected group else null
//-----------------------------------------------------------------
NTreeNode* NProjectWnd::GetSelectedGroup()
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
//!	\brief	Return selected page
//!	\return	Pointer on selected page else null
//-----------------------------------------------------------------
NOperatorsPage* NProjectWnd::GetSelectedPage()
{
	//Get Selected Object
	NObject* pobj = GetSelectedItemObj();

	//Check if selected item is a TreeNode
	if (pobj!=null)
	{
		if (strcmp(pobj->GetRTClass()->m_pszClassName, "NOperatorsPage")==0 )
			return (NOperatorsPage*)pobj;
	}

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Add a new group at selected node
//-----------------------------------------------------------------
void NProjectWnd::AddGroup()
{
	//Get Selected Group
	NTreeNode* pnodeGroup=GetSelectedGroup();

	if (pnodeGroup==null)	return;

	//Check Parent Group
	//if (pnodeGroup==null)		pnodeGroup = m_popsProject->GetRootGroup();

	//Attach a new group to Parent Group
	NTreeNode* pNewGrpNode = new NTreeNode;
	pNewGrpNode->SetName("Group");
	pnodeGroup->AddSon(pNewGrpNode);
	DisplayOperatorsProject(m_popsProject);

}

//-----------------------------------------------------------------
//!	\brief	Add a new page at selected node
//-----------------------------------------------------------------
void NProjectWnd::AddPage()
{
	//Get Selected Group
	NTreeNode* pnodeGroup=GetSelectedGroup();
	if (pnodeGroup==null)
		return;

	//Add NOperatorsPage object to this TreeNode
	NObjectArray& array = pnodeGroup->GetObjsArray();

	NOperatorsPage* ppage = new NOperatorsPage;
	ppage->SetName("Page");
	array.AddItem(ppage);

	DisplayOperatorsProject(m_popsProject);
}

//-----------------------------------------------------------------
//!	\brief	Delete a group or a page
//-----------------------------------------------------------------
void NProjectWnd::Delete()
{
	/////////////////////////////////////////////
	// Delete a page from project
	NOperatorsPage* ppage = GetSelectedPage();
	if (ppage)
	{
		udword dwRet = GetApp()->MessageBox("Do you really want to delete this page ?", MB_YESNO|MB_DEFBUTTON2);
		if (dwRet==IDYES)
		{
			//Get Group for this page
			NTreeNode* pnodegroup = (NTreeNode*)GetParent(ppage);

			assert(strcmp(pnodegroup->GetRTClass()->m_pszClassName, "NTreeNode")==0 );

			//Delete page from TreeNode ObjectArray
			NObjectArray& array = pnodegroup->GetObjsArray();
			udword idx = array.Find(ppage);
			if (idx!=-1)
			{
				array.RemoveItem(idx);
				DisplayOperatorsProject(m_popsProject);
			}

		}
		return;
	}

	/////////////////////////////////////////////
	// Delete a group from project
	NTreeNode* pgroup =	GetSelectedGroup();
	if (pgroup)
	{
		//Get parent for this group
		NTreeNode* pnodeparent = (NTreeNode*)GetParent(pgroup);
		if (pnodeparent)
		{
			udword dwRet = GetApp()->MessageBox("Do you really want to delete this group ?", MB_YESNO|MB_DEFBUTTON2);
			if (dwRet==IDYES)
			{
				udword idx = pnodeparent->FindSon(pgroup);
				if (idx!=-1)
				{
					pnodeparent->DeleteSon(idx);
					DisplayOperatorsProject(m_popsProject);
				}
			}
		}

	}

}

//-----------------------------------------------------------------
//!	\brief	Rename a node
//-----------------------------------------------------------------
void NProjectWnd::Rename()
{
	//Get Selected Object
	NObject* pobj = GetSelectedItemObj();

	//Edit label
	if (pobj)
	{
		NEnterNameDlg dlg(IDD_ENTERNAME, this);
		dlg.SetName( pobj->GetName() );

		if (IDOK==dlg.DoModal())
		{
			pobj->SetName(dlg.GetName());
			DisplayOperatorsProject(m_popsProject);
		}
	}
}

//-----------------------------------------------------------------
//!	\brief	Event when user change selected node
//-----------------------------------------------------------------
void NProjectWnd::OnTreeSelChange()
{
	//TRACE("NProjectWnd::OnSelChange\n");

	//Get Page Selected
	NOperatorsPage* pcurpage = GetSelectedPage();

	//Send notification message
	EVT_EXECUTE(EVT_PAGESELECTED, (udword)pcurpage, 0 );
}

//-----------------------------------------------------------------
//!	\brief	Select first page found from root
//-----------------------------------------------------------------
void NProjectWnd::SelectFirstPage()
{
	if (m_popsProject)
	{
		NTreeNode* pRoot = m_popsProject->GetRootGroup();
		NObject* pobj = _FindNodeFromClassName(pRoot, "NOperatorsPage");
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
NObject* NProjectWnd::_FindNodeFromClassName(NTreeNode* _pParent, char* _pszClassName)
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
