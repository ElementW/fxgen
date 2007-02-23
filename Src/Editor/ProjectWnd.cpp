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

bool NProjectWnd::Create(char* name, NRect& rect, NWnd* parent)
{
	NTreeNodeCtrl::Create(name, rect, parent);

	//Init context Menu
	InitCtxMenu();

	//Messages
	OnSelChange = FDelegate(this, (TDelegate)&NProjectWnd::OnTreeSelChange);

	return true;
}

void NProjectWnd::DisplayOperatorsProject(NEngineOp* _popsProject)
{
	EVT_EXECUTE(EVT_PAGESELECTED, 0, 0 );

	m_popsProject = _popsProject;

	//Display
	NTreeNode* pRoot = m_popsProject->GetRootGroup();
	DisplayTreeNode(pRoot);
	Update();
}

void NProjectWnd::OnRightButtonDown(udword flags, NPoint pos)
{
	if (m_hMenu)
	{
		POINT pt;
		::GetCursorPos(&pt);	//Cursor position even with keyboard 'Context key'
		::TrackPopupMenu(m_hMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON, pt.x, pt.y, null, m_W32HWnd, null);
	}
}

void NProjectWnd::InitCtxMenu()
{
	//Creation du menu
	m_hMenu = CreatePopupMenu();
	::AppendMenu(m_hMenu, MF_STRING, (DWORD)ID_ADDGROUP,	"Add Group"	);
	::AppendMenu(m_hMenu, MF_STRING, (DWORD)ID_ADDPAGE,		"Add Page"	);
	::AppendMenu(m_hMenu, MF_STRING, (DWORD)ID_DELETE,		"Delete"		);
	::AppendMenu(m_hMenu, MF_STRING, (DWORD)ID_RENAME,		"Rename"		);

}

void NProjectWnd::OnCommand(udword id)
{
	switch (id)
	{
		case ID_ADDGROUP:	AddGroup();		break;
		case ID_ADDPAGE:	AddPage();		break;
		case ID_DELETE:		Delete();			break;
		case ID_RENAME:		Rename();			break;
	}

}

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


void NProjectWnd::AddGroup()
{
	//Get Selected Group
	NTreeNode* pnodeGroup=GetSelectedGroup();

	//Check Parent Group
	if (pnodeGroup==null)
		pnodeGroup = m_popsProject->GetRootGroup();

	//Attach a new group to Parent Group
	NTreeNode* pNewGrpNode = new NTreeNode;
	pNewGrpNode->SetName("Group");
	pnodeGroup->AddSon(pNewGrpNode);
	DisplayOperatorsProject(m_popsProject);

}

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

void NProjectWnd::Delete()
{
	/////////////////////////////////////////////
	// Delete a page from project
	NOperatorsPage* ppage = GetSelectedPage();
	if (ppage)
	{
		udword dwRet = GetApp()->MessageBox("Do you really want to delete this page ?", MB_YESNO);
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
/*
	/////////////////////////////////////////////
	// Delete a group from project
	NTreeNode* pgroup =	GetSelectedGroup();
	if (pgroup)
	{
		udword dwRet = GetApp()->MessageBox("Do you really want to delete this group ?", MB_YESNO);
		if (dwRet==IDYES)
		{
			//Get parent for this group
			HTREEITEM hitemSel		= GetSelection();
			HTREEITEM hitemParent = GetParent(hitemSel);
			NTreeNode* pnodeparent = (NTreeNode*)GetItemData(hitemParent);

			udword idx = pnodeparent->FindSon(pgroup);
			if (idx!=-1)
			{
				pnodeparent->DeleteSon(idx);
				Update();
			}

		}
	}*/

}


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


void NProjectWnd::OnTreeSelChange()
{
	//TRACE("NProjectWnd::OnSelChange\n");

	//Get Page Selected
	NOperatorsPage* pcurpage = GetSelectedPage();

	//Send notification message
	EVT_EXECUTE(EVT_PAGESELECTED, (udword)pcurpage, 0 );
}

