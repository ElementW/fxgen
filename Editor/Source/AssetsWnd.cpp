//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		AssetsWnd.cpp
//! \brief	Assets's windows
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		13-03-2010
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
#include "AssetsWnd.h"


//-----------------------------------------------------------------
//			Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NAssetsWnd Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NAssetsWnd::NAssetsWnd(void)
{
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NAssetsWnd::~NAssetsWnd(void)
{
	EVT_UNREGISTERALL();
}

//-----------------------------------------------------------------
//!	\brief	control Creation
//-----------------------------------------------------------------
bool NAssetsWnd::Create(const char* name, const NRect& rect, NGUIWnd* parent)
{
	NTreeNodeCtrl::Create(name, rect, parent);

	//Init context Menu
	InitCtxMenu();

	//Register Notifications Messages
	OnSelChange = FDelegate(this, (TDelegate)&NAssetsWnd::OnTreeSelChange);

	return true;
}


//-----------------------------------------------------------------
//!	\brief	control right button down message
//-----------------------------------------------------------------
void NAssetsWnd::OnRButtonDown(NPoint pos)
{
	ClientToScreen(pos);
	m_wndMenu.TrackPopupMenu(pos);
}

//-----------------------------------------------------------------
//!	\brief	Init context menu
//-----------------------------------------------------------------
void NAssetsWnd::InitCtxMenu()
{
	//Menu creation
	m_wndMenu.Create("Assets:", this);
	m_wndMenu.OnItemClick=FDelegate(this, (TDelegate)&NAssetsWnd::OnMenuClick);
/*	m_wndMenu.AddItem("Add Group",		ID_ADDGROUP, 0);
	m_wndMenu.AddItem("Add Page",		ID_ADDPAGE,		 0);
	m_wndMenu.AddItem("Delete",			ID_DELETE,		 0);
	m_wndMenu.AddItem("Rename",			ID_RENAME,		 0);*/

}

//-----------------------------------------------------------------
//!	\brief	Command message
//-----------------------------------------------------------------
void NAssetsWnd::OnMenuClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	udword dwID = pmenu->GetClickedCmdID();
/*	switch (dwID)
	{
		case ID_ADDGROUP:	AddGroup();		break;
		case ID_ADDPAGE:	AddPage();		break;
		case ID_DELETE:		Delete();			break;
		case ID_RENAME:		Rename();			break;
	}*/

}

//-----------------------------------------------------------------
//!	\brief	Event when user change selected node
//-----------------------------------------------------------------
void NAssetsWnd::OnTreeSelChange(NObject* _psender)
{
	//TRACE("NProjectWnd::OnSelChange\n");

	//Get Page Selected
	//NOpGraphModel* pcurpage = GetSelectedPage();

	//Send notification message
	//EVT_EXECUTE(EVT_PAGESELECTED, (udword)pcurpage, 0 );
}
