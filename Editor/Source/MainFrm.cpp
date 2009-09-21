//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MainFrm.cpp
//! \brief	Main frame
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
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "MainFrm.h"

#include "OperatorsWnd.h"
#include "PropertiesWnd.h"
#include "ViewportsWnd.h"
#include "ProjectWnd.h"


//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------

#define	MENU_NEWPROJECT			100
#define	MENU_OPENPROJECT		101
#define	MENU_SAVEPROJECTAS	102
#define	MENU_EXIT						103
#define	MENU_SAVEPROJECT		104
#define	MENU_RELOADPROJECT	105
#define	MENU_EXPORTTOFX2PRJ	106

#define	MENU_DETAILLOW			200
#define	MENU_DETAILNORMAL		201
#define	MENU_DETAILHIGH			202
#define	MENU_DETAILFINE			203
#define	MENU_DETAILREALISTIC	204
#define	MENU_DETAILULTRA		205

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NMainFrm Class Implementation
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
NMainFrm::NMainFrm(void)
{
	m_bExecuteLocked	= false;
	m_popMarkedShow		= null;
	m_fDetailFactor		= 1.0f;
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NMainFrm::~NMainFrm(void)
{
}

//-----------------------------------------------------------------
//!	\brief	Main frame creation
//!	\param	name	windows name
//!	\param	rect	windows rect
//!	\return	True if success
//-----------------------------------------------------------------
bool NMainFrm::Create(char* name, const NRect& rect)
{
	NFrmWnd::Create(name, rect);

	///////////////////////////////////////////////
	// Create Splitter
	NSplitWnd* pwrkspace = (NSplitWnd*)GetWorkspace();

	//Create Operators window
	m_opswnd = new NOperatorsWnd;
	m_opswnd->Create("Operators", GetClientRect(), pwrkspace);

	//Create properties window
	NPropertiesWnd* propswnd = new NPropertiesWnd;
	propswnd->Create("Properties", GetClientRect(), pwrkspace);

	//Create Viewport window
	NViewportsWnd* viewportswnd = new NViewportsWnd;
	viewportswnd->Create("Viewport", GetClientRect(), pwrkspace);

	//Create Project window
	m_pprojectwnd = new NProjectWnd;
	m_pprojectwnd->Create("Project", GetClientRect(), pwrkspace);

	///////////////////////////////////////////////
	// Mainframe's Split
	udword dwViewportsPaneID	= SPLITPANE_DEFAULT_ID;
	udword dwProjectPaneID		= pwrkspace->SplitRow(dwViewportsPaneID, 50);
	udword dwPropertiesPaneID = pwrkspace->SplitColumn(dwViewportsPaneID, 70);
	udword dwOperatorsPaneID	= pwrkspace->SplitColumn(dwProjectPaneID, 20);
	pwrkspace->SetPaneWnd("Viewport", dwViewportsPaneID,	viewportswnd);
	pwrkspace->SetPaneWnd("Operators", dwOperatorsPaneID,	m_opswnd);
	pwrkspace->SetPaneWnd("Properties", dwPropertiesPaneID,	propswnd);
	pwrkspace->SetPaneWnd("Project", dwProjectPaneID,	m_pprojectwnd);

	//Init StripBar
	NPANEINFO* pinfo = pwrkspace->GetPaneInfo(dwViewportsPaneID);
	NButtonCtrl* pbuttonFile		= pinfo->pstripBar->InsertItemButton("File", 4*8);
	NButtonCtrl* pbuttonOption	= pinfo->pstripBar->InsertItemButton("Option", 6*8);
	pinfo->pstripBar->InsertItemText("Synthesis Texture Generator", 256);

	pinfo = pwrkspace->GetPaneInfo(dwOperatorsPaneID);
	pinfo->pstripBar->InsertItemText("Operators", 9*8);

	pinfo = pwrkspace->GetPaneInfo(dwPropertiesPaneID);
	pinfo->pstripBar->InsertItemText("Properties", 10*8);

	pinfo = pwrkspace->GetPaneInfo(dwProjectPaneID);
	pinfo->pstripBar->InsertItemText("Project", 7*8);

	//Init File Menu
	m_wndFileMenu.Create("", pbuttonFile);
	m_wndFileMenu.AddItem("New", MENU_NEWPROJECT, 0);
	m_wndFileMenu.AddItem("Open", MENU_OPENPROJECT, 0);
	m_wndFileMenu.AddItem("Save", MENU_SAVEPROJECT, 0);
	m_wndFileMenu.AddItem("Save as...", MENU_SAVEPROJECTAS, 0);
	m_wndFileMenu.AddItem("Export to Fx2...", MENU_EXPORTTOFX2PRJ, 0);
	m_wndFileMenu.AddItem("Exit", MENU_EXIT, 0);
	m_wndFileMenu.OnItemClick=FDelegate(this, (TDelegate)&NMainFrm::OnFileMenuClick);

	//Init Option Menu
	m_wndOptionMenu.Create("", pbuttonOption);
	m_wndOptionMenu.AddItem("Low x0.5",			MENU_DETAILLOW, 0);
	m_wndOptionMenu.AddItem("Normal x1.0",	MENU_DETAILNORMAL, 0);
	m_wndOptionMenu.AddItem("High x2.0",		MENU_DETAILHIGH, 0);
	m_wndOptionMenu.OnItemClick=FDelegate(this, (TDelegate)&NMainFrm::OnOptionMenuClick);

	//Events
	pbuttonFile->OnClick=FDelegate(this, (TDelegate)&NMainFrm::OnFile);
	pbuttonOption->OnClick=FDelegate(this, (TDelegate)&NMainFrm::OnOption);

	///////////////////////////////////////////////
	// Create Start project
	OnNewProject();

	return true;
}

//-----------------------------------------------------------------
//!	\brief	###TOFIX###
//-----------------------------------------------------------------
bool NMainFrm::AnnoyUserProjectMayBeLost()
{
	static int firsttime = 2;

	if (firsttime)
	{
		--firsttime;
		return true;
	}

	NTreeNode* prootNode = NEngineOp::GetEngine()->GetRootGroup();

	udword dwCount = prootNode->GetSonsCount();

	if (dwCount != 0)
	{
		udword dwRet = GetApp()->MessageBox("You will lose your current project! Do you want to continue ?", NMB_YESNO);

		if (dwRet == NIDNO)		return false;
	}
	return true;
}


//-----------------------------------------------------------------
//!	\brief	Project creation
//-----------------------------------------------------------------
void NMainFrm::OnNewProject()
{
	NTreeNode* prootNode = NEngineOp::GetEngine()->GetRootGroup();
	if(!AnnoyUserProjectMayBeLost())
		return;

	m_popMarkedShow = null;
	m_bExecuteLocked = true;
	projectname = "";

	//New project
	NEngineOp::GetEngine()->Clear();

	//Create empty project
	prootNode = NEngineOp::GetEngine()->GetRootGroup();

	NTreeNode* pNewGrpNode = new NTreeNode;
	pNewGrpNode->SetName("Group");
	prootNode->AddSon(pNewGrpNode);

	NOperatorsPage* ppage = new NOperatorsPage;
	ppage->SetName("Page");
	pNewGrpNode->GetObjsArray().AddItem(ppage);


	//Display Projects's Pages
	m_pprojectwnd->DisplayOperatorsProject(NEngineOp::GetEngine());
	m_pprojectwnd->SelectFirstPage();

	//m_opswnd->DisplayOperatorsPage(null);
	//m_opswnd->Update();

	NString strTitle(CAPTION);
	strTitle+="Untitled";
	SetText(strTitle.Buffer());

	m_bExecuteLocked = false;
}

void NMainFrm::LoadProject(NString str)
{
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

	NEngineOp::GetEngine()->Clear();

	m_pprojectwnd->DisplayOperatorsProject(NEngineOp::GetEngine());
	m_pprojectwnd->SelectFirstPage();

	if (NEngineOp::GetEngine()->LoadProject(str.Buffer()))
	{
		projectname = str;
		NString strTitle(CAPTION);
		strTitle+=str.Buffer();
		SetText(strTitle.Buffer());

		//###TEST###
		//NEngineOp::GetEngine()->ProcessOperators(0.0, staticProcess);

	} else {
		//Error Message
		GetApp()->MessageBox(gGetErrors()->GetErrors());
		//Clean up
		OnNewProject();
	}

	//Display Projects's Pages
	m_pprojectwnd->DisplayOperatorsProject(NEngineOp::GetEngine());
	m_pprojectwnd->SelectFirstPage();

	m_bExecuteLocked = false;
}

//-----------------------------------------------------------------
//!	\brief	Project loading
//-----------------------------------------------------------------
void NMainFrm::OnOpenProject()
{
	//Open File Dialog
	NFileDialog dlg;
	dlg.Create("Opening Project...", this);
	if (dlg.DoModal())
	{
		LoadProject(dlg.GetPathName());
	}
}

void NMainFrm::SaveProject(NString path)
{
//	bool show_message = false;

	if (!path.Length())
		path = projectname;

	if (!path.Length())
	{
		OnSaveProjectAs();
		return;
	}

	m_bExecuteLocked = true;

	if (NEngineOp::GetEngine()->SaveProject(path.Buffer()))
	{
		projectname = path;
		NString strTitle(CAPTION);
		strTitle+=path.Buffer();
		SetText(strTitle.Buffer());
		GetApp()->MessageBox("File saved.", NMB_OK);
	}

	m_bExecuteLocked = false;
}

//-----------------------------------------------------------------
//!	\brief	Project saving
//-----------------------------------------------------------------
void NMainFrm::OnSaveProjectAs()
{
	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Saving Project...", this, false);
	if (dlg.DoModal())
		SaveProject(dlg.GetPathName());
}


//-----------------------------------------------------------------
//!	\brief	Execute operators processing
//!	\param	_ftime	time
//!	\return	Operator result
//-----------------------------------------------------------------
NOperator* NMainFrm::Execute(float _ftime)
{
	/*if (!m_bExecuteLocked)
	{
		m_bExecuteLocked = true;

		//m_wndProgress.TrackPopup(NPoint(256,256), RGBA(240,100,0,190));

		//Process operators
		NEngineOp::GetEngine()->Execute(_ftime, m_popMarkedShow, m_fDetailFactor, staticOperatorsProcessCB);

		//Rendering
		EVT_EXECUTE(EVT_RENDER, (udword)m_popMarkedShow, (udword)&_ftime);

		//m_wndProgress.ShowWindow(false);

		m_bExecuteLocked = false;
	}*/

	return m_popMarkedShow;
}


//-----------------------------------------------------------------
//!	\brief	Show an operator to viewport
//!	\param	pop		operator to show
//-----------------------------------------------------------------
void NMainFrm::MarkShowOperator(NOperator* _pop)
{
	m_popMarkedShow = _pop;

	//###DEBUG###
	//float ftime = (float)GetTickCount() * 60.0f / 1000.0f;
	//NOperator* pop = Execute(ftime);

}

//-----------------------------------------------------------------
//!	\brief	Delete an operator
//!	\param	pop		operator to delete
//-----------------------------------------------------------------
void NMainFrm::DeletingOperator(NOperator* _pop)
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
void NMainFrm::DeletedOperator(NOperator* pop)
{
	m_bExecuteLocked = false;
}

//-----------------------------------------------------------------
//!	\brief	Keys Down
//-----------------------------------------------------------------
void NMainFrm::OnKeyDown(udword dwchar)
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
void NMainFrm::OnFile(NObject* _psender)
{
	NButtonCtrl* pbut = (NButtonCtrl*)_psender;
	NRect rc = pbut->GetWindowRect();
	NPoint pt(rc.left, rc.bottom);
	m_wndFileMenu.TrackPopupMenu(pt, 0);

}

//-----------------------------------------------------------------
//!	\brief	Event when user select 'Option' Button
//-----------------------------------------------------------------
void NMainFrm::OnOption(NObject* _psender)
{
	NButtonCtrl* pbut = (NButtonCtrl*)_psender;
	NRect rc = pbut->GetWindowRect();
	NPoint pt(rc.left, rc.bottom);
	m_wndOptionMenu.TrackPopupMenu(pt, 0);
}


void NMainFrm::OnFileMenuClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	switch (pmenu->GetClickedCmdID())
	{
		case MENU_NEWPROJECT:			OnNewProject();				break;
		case MENU_OPENPROJECT:		OnOpenProject();			break;
		//case MENU_RELOADPROJECT:	LoadProject();				break;
		case MENU_SAVEPROJECTAS:	OnSaveProjectAs();		break;
		case MENU_SAVEPROJECT:		SaveProject();				break;
		case MENU_EXPORTTOFX2PRJ:	ExportToFx2();				break;
		case MENU_EXIT:						GetApp()->AskExit();	break;
	};

}

void NMainFrm::OnOptionMenuClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	switch (pmenu->GetClickedCmdID())
	{
		case MENU_DETAILLOW:
			m_bExecuteLocked = true;
			NEngineOp::GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetEngine()->InvalidateAllOps();
			m_fDetailFactor = 0.5f;
			m_bExecuteLocked = false;
			break;

		case MENU_DETAILNORMAL:
			m_bExecuteLocked = true;
			NEngineOp::GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetEngine()->InvalidateAllOps();
			m_fDetailFactor = 1.0f;
			m_bExecuteLocked = false;
			break;

		case MENU_DETAILHIGH:
			m_bExecuteLocked = true;
			NEngineOp::GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetEngine()->InvalidateAllOps();
			m_fDetailFactor = 2.0f;
			m_bExecuteLocked = false;
			break;

		//case MENU_DETAILFINE:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetEngine()->InvalidateAllOps();
		//	m_fDetailFactor = 4.0f;
		//	m_bExecuteLocked = false;
		//	break;

		//case MENU_DETAILREALISTIC:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetEngine()->InvalidateAllOps();
		//	m_fDetailFactor = 8.0f;
		//	m_bExecuteLocked = false;
		//	break;

		//case MENU_DETAILULTRA:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetEngine()->InvalidateAllOps();
		//	m_fDetailFactor = 16.0f;
		//	m_bExecuteLocked = false;
		//	break;
	};
}



void NMainFrm::ExportToFx2()
{
	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Export Project to Fx2...", this, false);
	if (dlg.DoModal())
	{
		if (!NEngineOp::GetEngine()->ExportToFxGen2(dlg.GetPathName().Buffer()))
			GetApp()->MessageBox("Error while exporting !", NMB_OK);
	}

}