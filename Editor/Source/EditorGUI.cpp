//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EditorGUI.cpp
//! \brief	Editor GUI
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		23-10-2009
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
#include "EditorGUI.h"

#include "OperatorsWnd.h"
//#include "PropertiesWnd.h"
#include "ViewportsWnd.h"
#include "ProjectWnd.h"
#include "FileChooserDialog.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------

#define	MENU_NEWPROJECT			100
#define	MENU_OPENPROJECT		101
#define	MENU_SAVEPROJECTAS	102
#define	MENU_EXIT						103
#define	MENU_SAVEPROJECT		104
#define	MENU_RELOADPROJECT	105

#define	MENU_DETAILLOW			200
#define	MENU_DETAILNORMAL		201
#define	MENU_DETAILHIGH			202
#define	MENU_DETAILFINE			203
#define	MENU_DETAILREALISTIC	204
#define	MENU_DETAILULTRA		205



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NEditorGUI Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//###TEST###
void staticOperatorsProcessCB(udword _dwCurrentOp, udword _dwTotalOps)
{
	TRACE("CallBack: Current %d Total %d\n", _dwCurrentOp, _dwTotalOps);
	//SetCursor(::LoadCursor(NULL, IDC_WAIT));
}


//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NEditorGUI::NEditorGUI(void)
{
	m_bExecuteLocked	= false;
	m_popMarkedShow		= null;
	m_fDetailFactor		= 1.0f;
	m_pcurProject			= null;
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NEditorGUI::~NEditorGUI(void)
{

}

//-----------------------------------------------------------------
//!	\brief	Main frame creation
//!	\param	name	windows name
//!	\param	rect	windows rect
//!	\return	True if success
//-----------------------------------------------------------------
bool NEditorGUI::Create(char* name, const NRect& rect)
{
	NGUILayout::Create("wks", rect, null);

	///////////////////////////////////////////////
	// Create panes

	//Create Operators window
  m_opswnd = NNEW(NOperatorsWnd);
	m_opswnd->Create("Operators", GetClientRect(), this);

	//Create properties window
//	NPropertiesWnd* propswnd = NNEW(NPropertiesWnd);
//	propswnd->Create("Properties", GetClientRect(), this);

	//Create Viewport window
	NViewportsWnd* viewportswnd = NNEW(NViewportsWnd);
	viewportswnd->Create("Viewport", GetClientRect(), this);

	//Create Project window
	m_pprojectwnd = NNEW(NProjectWnd);
	m_pprojectwnd->Create("Project", GetClientRect(), this);

	///////////////////////////////////////////////
	// Mainframe's Split
	udword dwViewportsPaneID	= SPLITPANE_DEFAULT_ID;
	udword dwProjectPaneID		= SplitRow(dwViewportsPaneID, 50);
	udword dwPropertiesPaneID = SplitColumn(dwViewportsPaneID, 70);
	udword dwOperatorsPaneID	= SplitColumn(dwProjectPaneID, 20);
	SetPaneWnd("View", dwViewportsPaneID,	viewportswnd);
	SetPaneWnd("Stacked Operators", dwOperatorsPaneID,	m_opswnd);
//	SetPaneWnd("Properties", dwPropertiesPaneID,	propswnd);
	SetPaneWnd("Project", dwProjectPaneID,	m_pprojectwnd);

	//Init StripBar
	NPANEINFO* pinfo = GetPaneInfo(dwViewportsPaneID);
	NButtonCtrl* pbuttonFile		= pinfo->pstripBar->InsertItemButton("File", 4*8);
	NButtonCtrl* pbuttonOption	= pinfo->pstripBar->InsertItemButton("Option", 6*8);
	pinfo->pstripBar->InsertItemText("Synthesis Texture Generator", 256);

	pinfo = GetPaneInfo(dwOperatorsPaneID);
	pinfo->pstripBar->InsertItemText("Stacked Operators", 17*8);

//	pinfo = GetPaneInfo(dwPropertiesPaneID);
//	pinfo->pstripBar->InsertItemText("Properties", 10*8);

	pinfo = GetPaneInfo(dwProjectPaneID);
	pinfo->pstripBar->InsertItemText("Project", 7*8);

	//Init File Menu
	m_wndFileMenu.Create("", pbuttonFile);
	m_wndFileMenu.AddItem("New", MENU_NEWPROJECT, 0);
	m_wndFileMenu.AddItem("Open", MENU_OPENPROJECT, 0);
	m_wndFileMenu.AddItem("Save", MENU_SAVEPROJECT, 0);
	m_wndFileMenu.AddItem("Save as...", MENU_SAVEPROJECTAS, 0);
	m_wndFileMenu.AddItem("Exit", MENU_EXIT, 0);
	m_wndFileMenu.OnItemClick=FDelegate(this, (TDelegate)&NEditorGUI::OnFileMenuClick);

	//Init Option Menu
	m_wndOptionMenu.Create("", pbuttonOption);
	m_wndOptionMenu.AddItem("Low x0.5",			MENU_DETAILLOW, 0);
	m_wndOptionMenu.AddItem("Normal x1.0",	MENU_DETAILNORMAL, 0);
	m_wndOptionMenu.AddItem("High x2.0",		MENU_DETAILHIGH, 0);
	m_wndOptionMenu.OnItemClick=FDelegate(this, (TDelegate)&NEditorGUI::OnOptionMenuClick);

	//Events
	pbuttonFile->OnClick=FDelegate(this, (TDelegate)&NEditorGUI::OnFile);
	pbuttonOption->OnClick=FDelegate(this, (TDelegate)&NEditorGUI::OnOption);

	///////////////////////////////////////////////
	// Create Start project
	OnNewProject();

	return true;
}

//-----------------------------------------------------------------
//!	\brief	###TOFIX###
//-----------------------------------------------------------------
bool NEditorGUI::AnnoyUserProjectMayBeLost()
{
/*
	static int firsttime = 2;

	if (firsttime)
	{
		--firsttime;
		return true;
	}

	NTreeNode* prootNode = NEngineOp::GetProject()->GetRootGroup();

	udword dwCount = prootNode->GetSonsCount();

	if (dwCount != 0)
	{
		udword dwRet = GetApp()->MessageBox("You will lose your current project! Do you want to continue ?", NMB_YESNO);
		if (dwRet == NIDNO)		return false;
	}
*/
	return true;
}


//-----------------------------------------------------------------
//!	\brief	Project creation
//-----------------------------------------------------------------
void NEditorGUI::OnNewProject()
{
//	NTreeNode* prootNode = NEngineOp::GetProject()->GetRootGroup();
//	if(!AnnoyUserProjectMayBeLost())
//		return;

	m_popMarkedShow = null;
	m_bExecuteLocked = true;
	projectname = "";

	//New project
//	NEngineOp::GetProject()->Clear();

	//Create empty project
	//prootNode = NEngineOp::GetProject()->GetRootGroup();

/*	NTreeNode* pNewGrpNode = NNEW(NTreeNode);
	pNewGrpNode->SetName("Group");
	prootNode->AddSon(pNewGrpNode);

	NOperatorsPage* ppage = NNEW(NOperatorsPage);
	ppage->SetName("Page");
	pNewGrpNode->GetObjsArray().AddItem(ppage);*/


	//Clear project
	if (m_pcurProject==null)
		m_pcurProject = new NOperatorsProject();

	m_pcurProject->Clear();

	//Display Projects's Pages
	m_pprojectwnd->DisplayOperatorsProject(m_pcurProject);
	m_pprojectwnd->SelectFirstPage();

	//m_opswnd->DisplayOperatorsPage(null);
	//m_opswnd->Update();

	NString strTitle(CAPTION);
	strTitle+="Untitled";
	SetText(strTitle.Buffer());

	m_bExecuteLocked = false;

}

void NEditorGUI::LoadProject(NString str)
{
/*
	if(!AnnoyUserProjectMayBeLost())
		return;

	if (!str.Length())
		str = projectname;

	if(!str.Length())
	{
		OnOpenProject();
		return;
	}

	m_popMarkedShow = null;
	m_bExecuteLocked = true;

	NEngineOp::GetProject()->Clear();

	m_pprojectwnd->DisplayOperatorsProject(NEngineOp::GetProject());
	m_pprojectwnd->SelectFirstPage();

	if (NEngineOp::GetProject()->LoadProject(str.Buffer()))
	{
		projectname = str;
		NString strTitle(CAPTION);
		strTitle+=str.Buffer();
		SetText(strTitle.Buffer());

		//###TEST###
		//NEngineOp::GetProject()->ProcessOperators(0.0, staticProcess);

	} else {
		//Error Message
		GetApp()->MessageBox(gGetErrors()->GetErrors(), NMB_OK);
		//Clean up
		OnNewProject();
	}

	//Display Projects's Pages
	m_pprojectwnd->DisplayOperatorsProject(NEngineOp::GetProject());
	m_pprojectwnd->SelectFirstPage();

	m_bExecuteLocked = false;
*/
}

//-----------------------------------------------------------------
//!	\brief	Project loading
//-----------------------------------------------------------------
void NEditorGUI::OnOpenProject()
{
	//Open File Dialog
  /*NFileChooserDialog dlg;
	dlg.Create("Opening Project...", this);
	if (dlg.DoModal())
	{
		LoadProject(dlg.GetPathName());
  }*/
}

void NEditorGUI::SaveProject(NString path)
{
/*
//	bool show_message = false;

	if (!path.Length())
		path = projectname;

	if (!path.Length())
	{
		OnSaveProjectAs();
		return;
	}

	m_bExecuteLocked = true;

	if (NEngineOp::GetProject()->SaveProject(path.Buffer()))
	{
		projectname = path;
		NString strTitle(CAPTION);
		strTitle+=path.Buffer();
		SetText(strTitle.Buffer());
		GetApp()->MessageBox("File saved.", NMB_OK);
	}

	m_bExecuteLocked = false;
	*/
}

//-----------------------------------------------------------------
//!	\brief	Project saving
//-----------------------------------------------------------------
void NEditorGUI::OnSaveProjectAs()
{
	//Save File Dialog
  /*NFileChooserDialog dlg;
	dlg.Create("Saving Project...", this, false);
	if (dlg.DoModal())
    SaveProject(dlg.GetPathName());*/
}


//-----------------------------------------------------------------
//!	\brief	Execute operators processing
//!	\param	_ftime	time
//!	\return	Operator result
//-----------------------------------------------------------------
NOperatorNode* NEditorGUI::Execute(float _ftime)
{
/*
	if (!m_bExecuteLocked)
	{
		m_bExecuteLocked = true;

		//m_wndProgress.TrackPopup(NPoint(256,256), RGBA(240,100,0,190));

		//Process operators
		NEngineOp::GetProject()->Execute(_ftime, m_popMarkedShow, m_fDetailFactor, staticOperatorsProcessCB);

		//Rendering
		EVT_EXECUTE(EVT_RENDER, (udword)m_popMarkedShow, (udword)&_ftime);

		//m_wndProgress.ShowWindow(false);

		m_bExecuteLocked = false;
	}
*/
	return m_popMarkedShow;
}


//-----------------------------------------------------------------
//!	\brief	Show an operator to viewport
//!	\param	pop		operator to show
//-----------------------------------------------------------------
void NEditorGUI::MarkShowOperator(NOperatorNode* _pop)
{
	m_popMarkedShow = _pop;

	//###DEBUG###
	//float ftime = (float)GetTickCount() * 60.0f / 1000.0f;
	//NOperatorNode* pop = Execute(ftime);

}

//-----------------------------------------------------------------
//!	\brief	Delete an operator
//!	\param	pop		operator to delete
//-----------------------------------------------------------------
void NEditorGUI::DeletingOperator(NOperatorNode* _pop)
{
	m_bExecuteLocked = true;

	if (m_popMarkedShow == _pop)
	{
		m_popMarkedShow = null;
	}

	//m_bExecuteLocked = true;	###see DeletedOperator()###
}

//-----------------------------------------------------------------
//!	\brief	operator Deleted
//!	\param	pop		operator deleted
//-----------------------------------------------------------------
void NEditorGUI::DeletedOperator(NOperatorNode* pop)
{
	m_bExecuteLocked = false;
}

//-----------------------------------------------------------------
//!	\brief	operator properties changed
//!	\param	pop		operator
//-----------------------------------------------------------------
void NEditorGUI::EmitPropertiesChanged(NOperatorNode* pop)
{
	
}

//-----------------------------------------------------------------
//!	\brief	Keys Down
//-----------------------------------------------------------------
void NEditorGUI::OnKeyDown(udword dwchar)
{
// temporary implementation
//#ifdef USE_QUAKE_KEYS		//###JN### ok for now... :-)
/*	switch (dwchar)
	{
		case VK_F6: SaveProject();	break;
		case VK_F9:	LoadProject(); break;
		case VK_F2:	OnSaveProjectAs(); break;
		case VK_F3:	OnOpenProject(); break;
		default: break;
	}*/
//#endif
}

//-----------------------------------------------------------------
//!	\brief	Event when user select 'File' Button
//-----------------------------------------------------------------
void NEditorGUI::OnFile(NObject* _psender)
{
	NButtonCtrl* pbut = (NButtonCtrl*)_psender;
	NRect rc = pbut->GetWindowRect();
	NPoint pt(rc.left, rc.bottom);
	m_wndFileMenu.TrackPopupMenu(pt, 0);

}

//-----------------------------------------------------------------
//!	\brief	Event when user select 'Option' Button
//-----------------------------------------------------------------
void NEditorGUI::OnOption(NObject* _psender)
{
	NButtonCtrl* pbut = (NButtonCtrl*)_psender;
	NRect rc = pbut->GetWindowRect();
	NPoint pt(rc.left, rc.bottom);
	m_wndOptionMenu.TrackPopupMenu(pt, 0);
}


void NEditorGUI::OnFileMenuClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	switch (pmenu->GetClickedCmdID())
	{
		case MENU_NEWPROJECT:			OnNewProject();				break;
		case MENU_OPENPROJECT:		OnOpenProject();			break;
		//case MENU_RELOADPROJECT:	LoadProject();				break;
		case MENU_SAVEPROJECTAS:	OnSaveProjectAs();		break;
		case MENU_SAVEPROJECT:		SaveProject();				break;
//		case MENU_EXIT:						GetGUISubSystem()->AskExit();	break;
	};

}

void NEditorGUI::OnOptionMenuClick(NObject* _psender)
{
/*
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	switch (pmenu->GetClickedCmdID())
	{
		case MENU_DETAILLOW:
			m_bExecuteLocked = true;
			NEngineOp::GetProject()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetProject()->InvalidateAllOps();
			m_fDetailFactor = 0.5f;
			m_bExecuteLocked = false;
			break;

		case MENU_DETAILNORMAL:
			m_bExecuteLocked = true;
			NEngineOp::GetProject()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetProject()->InvalidateAllOps();
			m_fDetailFactor = 1.0f;
			m_bExecuteLocked = false;
			break;

		case MENU_DETAILHIGH:
			m_bExecuteLocked = true;
			NEngineOp::GetProject()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetProject()->InvalidateAllOps();
			m_fDetailFactor = 2.0f;
			m_bExecuteLocked = false;
			break;

		//case MENU_DETAILFINE:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetProject()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetProject()->InvalidateAllOps();
		//	m_fDetailFactor = 4.0f;
		//	m_bExecuteLocked = false;
		//	break;

		//case MENU_DETAILREALISTIC:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetProject()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetProject()->InvalidateAllOps();
		//	m_fDetailFactor = 8.0f;
		//	m_bExecuteLocked = false;
		//	break;

		//case MENU_DETAILULTRA:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetProject()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetProject()->InvalidateAllOps();
		//	m_fDetailFactor = 16.0f;
		//	m_bExecuteLocked = false;
		//	break;
	};
*/
}
