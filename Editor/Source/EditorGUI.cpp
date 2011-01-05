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
#include "PropertiesWnd.h"
#include "ViewportsWnd.h"
#include "FileChooserDialog.h"
#include "AssetWnd.h"

#include "AssetModel.h"
#include "OpGraphModel.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------

#define	MENU_NEWASSET			100
#define	MENU_OPENASSET		101
#define	MENU_SAVEASSETAS	102
#define	MENU_EXIT					103
#define	MENU_SAVEASSET		104
#define	MENU_RELOADASSET	105

#define	MENU_DETAILLOW			200
#define	MENU_DETAILNORMAL		201
#define	MENU_DETAILHIGH			202
#define	MENU_DETAILFINE			203
#define	MENU_DETAILREALISTIC	204
#define	MENU_DETAILULTRA		205

//-----------------------------------------------------------------
//	Variables
//-----------------------------------------------------------------
NEditorGUI* gpeditorGUI = null;

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
	gpeditorGUI				= this;
	m_bExecuteLocked	= false;
	m_popMarkedShow		= null;
	m_fDetailFactor		= 1.0f;
	m_pcurAsset				= null;
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NEditorGUI::~NEditorGUI(void)
{

}

//-----------------------------------------------------------------
//!	\brief	Static Methods that return an unique Instance
//-----------------------------------------------------------------
NEditorGUI*	NEditorGUI::GetInstance()
{
	if (gpeditorGUI==null)
		gpeditorGUI = new NEditorGUI;

	return gpeditorGUI;
}


//-----------------------------------------------------------------
//!	\brief	Main frame creation
//!	\param	name	windows name
//!	\param	rect	windows rect
//!	\return	True if success
//-----------------------------------------------------------------
bool NEditorGUI::Create(char* name, const NRect& rect)
{
	NLayoutDock::Create("wks", rect, null);

	///////////////////////////////////////////////
	// Create panes

	//Create Operators window
  m_opswnd = NNEW(NOperatorsWnd);
	m_opswnd->Create("Stacked Operators", GetClientRect(), this);

	//Create properties window
	NPropertiesWnd* propswnd = NNEW(NPropertiesWnd);
	propswnd->Create("Properties", GetClientRect(), this);

	//Create Viewport window
	NViewportsWnd* viewportswnd = NNEW(NViewportsWnd);
	viewportswnd->Create("View", GetClientRect(), this);

	//Create Asset window
	//m_pprojectwnd = NNEW(NAssetWnd);
	//m_pprojectwnd->Create("Asset", GetClientRect(), this);

	//Create Assets window
	m_passetswnd = NNEW(NAssetWnd);
	m_passetswnd->Create("Assets", GetClientRect(), this);

	///////////////////////////////////////////////
	// Mainframe's Split
	udword dwViewportsPaneID	= 0;
	udword dwAssetsPaneID		= SplitH(dwViewportsPaneID, 50);
	udword dwPropertiesPaneID = SplitV(dwViewportsPaneID, 70);
	udword dwOperatorsPaneID	= SplitV(dwAssetsPaneID, 20);

	AddWndToLayout(dwViewportsPaneID,	viewportswnd);
	AddWndToLayout(dwOperatorsPaneID,	m_opswnd);
	AddWndToLayout(dwPropertiesPaneID,	propswnd);
	AddWndToLayout(dwAssetsPaneID,	m_passetswnd);

	RecalLayout();

	/*udword dwViewportsPaneID	= SPLITPANE_DEFAULT_ID;
	udword dwAssetPaneID		= SplitRow(dwViewportsPaneID, 50);
	udword dwPropertiesPaneID = SplitColumn(dwViewportsPaneID, 70);
	udword dwOperatorsPaneID	= SplitColumn(dwAssetPaneID, 20);
	SetPaneWnd("View", dwViewportsPaneID,	viewportswnd);
	SetPaneWnd("Stacked Operators", dwOperatorsPaneID,	m_opswnd);
//	SetPaneWnd("Properties", dwPropertiesPaneID,	propswnd);
	SetPaneWnd("Asset", dwAssetPaneID,	m_pprojectwnd);

	//Init StripBar
	NPANEINFO* pinfo = GetPaneInfo(dwViewportsPaneID);
	NButtonCtrl* pbuttonFile		= pinfo->pstripBar->InsertItemButton("File", 4*8);
	NButtonCtrl* pbuttonOption	= pinfo->pstripBar->InsertItemButton("Option", 6*8);
	pinfo->pstripBar->InsertItemText("Synthesis Texture Generator", 256);

	pinfo = GetPaneInfo(dwOperatorsPaneID);
	pinfo->pstripBar->InsertItemText("Stacked Operators", 17*8);

//	pinfo = GetPaneInfo(dwPropertiesPaneID);
//	pinfo->pstripBar->InsertItemText("Properties", 10*8);

	pinfo = GetPaneInfo(dwAssetPaneID);
	pinfo->pstripBar->InsertItemText("Asset", 7*8);

	//Init File Menu
	m_wndFileMenu.Create("", pbuttonFile);
	m_wndFileMenu.AddItem("New", MENU_NEWASSET, 0);
	m_wndFileMenu.AddItem("Open", MENU_OPENASSET, 0);
	m_wndFileMenu.AddItem("Save", MENU_SAVEASSET, 0);
	m_wndFileMenu.AddItem("Save as...", MENU_SAVEASSETAS, 0);
	m_wndFileMenu.AddItem("Exit", MENU_EXIT, 0);
	m_wndFileMenu.OnItemClick=FDelegate(this, (TDelegate)&NEditorGUI::OnFileMenuClick);

	//Init Option Menu
	m_wndOptionMenu.Create("", pbuttonOption);
	m_wndOptionMenu.AddItem("Low x0.5",			MENU_DETAILLOW, 0);
	m_wndOptionMenu.AddItem("Normal x1.0",	MENU_DETAILNORMAL, 0);
	m_wndOptionMenu.AddItem("High x2.0",		MENU_DETAILHIGH, 0);
	m_wndOptionMenu.OnItemClick=FDelegate(this, (TDelegate)&NEditorGUI::OnOptionMenuClick);

	//m_dwWidth
	pbuttonFile->OnClick=FDelegate(this, (TDelegate)&NEditorGUI::OnFile);
	pbuttonOption->OnClick=FDelegate(this, (TDelegate)&NEditorGUI::OnOption);
*/



	///////////////////////////////////////////////
	// Create Start project
	OnNewAsset();

	return true;
}

//-----------------------------------------------------------------
//!	\brief	###TOFIX###
//-----------------------------------------------------------------
bool NEditorGUI::AnnoyUserAssetMayBeLost()
{
/*
	static int firsttime = 2;

	if (firsttime)
	{
		--firsttime;
		return true;
	}

	NTreeNode* prootNode = NEngineOp::GetAsset()->GetRootGroup();

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
//!	\brief	Asset creation
//-----------------------------------------------------------------
void NEditorGUI::OnNewAsset()
{
//	NTreeNode* prootNode = NEngineOp::GetAsset()->GetRootGroup();
//	if(!AnnoyUserAssetMayBeLost())
//		return;

	m_popMarkedShow = null;
	m_bExecuteLocked = true;
	projectname = "";

	m_pcurAsset = NNEW(NAssetModel);

	m_passetswnd->DisplayAssetModel(m_pcurAsset);
	
	//New project
//	NEngineOp::GetAsset()->Clear();

	//Create empty project
	//prootNode = NEngineOp::GetAsset()->GetRootGroup();

/*	NTreeNode* pNewGrpNode = NNEW(NTreeNode);
	pNewGrpNode->SetName("Group");
	prootNode->AddSon(pNewGrpNode);

	NOpGraphModel* ppage = NNEW(NOpGraphModel);
	ppage->SetName("Page");
	pNewGrpNode->GetObjsArray().AddItem(ppage);*/


	//Clear project
/*	if (m_pcurAsset==null)
		m_pcurAsset = new NOperatorsAsset();

	m_pcurAsset->Clear();

	//Display Assets's Pages
	m_pprojectwnd->DisplayOperatorsAsset(m_pcurAsset);
	m_pprojectwnd->SelectFirstPage();

	//m_opswnd->DisplayOperatorsPage(null);
	//m_opswnd->Update();

	NString strTitle(CAPTION);
	strTitle+="Untitled";
	SetText(strTitle.Buffer());
	*/
	m_bExecuteLocked = false;

}

void NEditorGUI::LoadAsset(NString str)
{
/*
	if(!AnnoyUserAssetMayBeLost())
		return;

	if (!str.Length())
		str = projectname;

	if(!str.Length())
	{
		OnOpenAsset();
		return;
	}

	m_popMarkedShow = null;
	m_bExecuteLocked = true;

	NEngineOp::GetAsset()->Clear();

	m_pprojectwnd->DisplayOperatorsAsset(NEngineOp::GetAsset());
	m_pprojectwnd->SelectFirstPage();

	if (NEngineOp::GetAsset()->LoadAsset(str.Buffer()))
	{
		projectname = str;
		NString strTitle(CAPTION);
		strTitle+=str.Buffer();
		SetText(strTitle.Buffer());

		//###TEST###
		//NEngineOp::GetAsset()->ProcessOperators(0.0, staticProcess);

	} else {
		//Error Message
		GetApp()->MessageBox(gGetErrors()->GetErrors(), NMB_OK);
		//Clean up
		OnNewAsset();
	}

	//Display Assets's Pages
	m_pprojectwnd->DisplayOperatorsAsset(NEngineOp::GetAsset());
	m_pprojectwnd->SelectFirstPage();

	m_bExecuteLocked = false;
*/
}

//-----------------------------------------------------------------
//!	\brief	Asset loading
//-----------------------------------------------------------------
void NEditorGUI::OnOpenAsset()
{
	//Open File Dialog
  /*NFileChooserDialog dlg;
	dlg.Create("Opening Asset...", this);
	if (dlg.DoModal())
	{
		LoadAsset(dlg.GetPathName());
  }*/
}

void NEditorGUI::SaveAsset(NString path)
{
/*
//	bool show_message = false;

	if (!path.Length())
		path = projectname;

	if (!path.Length())
	{
		OnSaveAssetAs();
		return;
	}

	m_bExecuteLocked = true;

	if (NEngineOp::GetAsset()->SaveAsset(path.Buffer()))
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
//!	\brief	Asset saving
//-----------------------------------------------------------------
void NEditorGUI::OnSaveAssetAs()
{
	//Save File Dialog
  /*NFileChooserDialog dlg;
	dlg.Create("Saving Asset...", this, false);
	if (dlg.DoModal())
    SaveAsset(dlg.GetPathName());*/
}


//-----------------------------------------------------------------
//!	\brief	Execute operators processing
//!	\param	_ftime	time
//!	\return	Operator result
//-----------------------------------------------------------------
NOperatorNode* NEditorGUI::Execute(float _ftime)
{

	//TODO Put this code into another thread...
	if (!m_bExecuteLocked)
	{
		m_bExecuteLocked = true;

		//m_wndProgress.TrackPopup(NPoint(256,256), RGBA(240,100,0,190));

		//Process operators
		if (m_popMarkedShow!=null)
			NEngineOp::GetInstance()->Execute(_ftime, m_popMarkedShow->m_op, m_fDetailFactor, staticOperatorsProcessCB);

		//Rendering
		EVT_EXECUTE(EVT_RENDER, (udword)m_popMarkedShow, (udword)&_ftime);

		//m_wndProgress.ShowWindow(false);

		m_bExecuteLocked = false;
	}

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
void NEditorGUI::EmitPropertiesChanged(NOperatorFx* pop)
{
	//Invalidate operator
	NEngineOp::GetInstance()->InvalidateOp(pop);
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
		case VK_F6: SaveAsset();	break;
		case VK_F9:	LoadAsset(); break;
		case VK_F2:	OnSaveAssetAs(); break;
		case VK_F3:	OnOpenAsset(); break;
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
		case MENU_NEWASSET:			OnNewAsset();				break;
		case MENU_OPENASSET:		OnOpenAsset();			break;
		//case MENU_RELOADASSET:	LoadAsset();				break;
		case MENU_SAVEASSETAS:	OnSaveAssetAs();		break;
		case MENU_SAVEASSET:		SaveAsset();				break;
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
			NEngineOp::GetAsset()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetAsset()->InvalidateAllOps();
			m_fDetailFactor = 0.5f;
			m_bExecuteLocked = false;
			break;

		case MENU_DETAILNORMAL:
			m_bExecuteLocked = true;
			NEngineOp::GetAsset()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetAsset()->InvalidateAllOps();
			m_fDetailFactor = 1.0f;
			m_bExecuteLocked = false;
			break;

		case MENU_DETAILHIGH:
			m_bExecuteLocked = true;
			NEngineOp::GetAsset()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			NEngineOp::GetAsset()->InvalidateAllOps();
			m_fDetailFactor = 2.0f;
			m_bExecuteLocked = false;
			break;

		//case MENU_DETAILFINE:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetAsset()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetAsset()->InvalidateAllOps();
		//	m_fDetailFactor = 4.0f;
		//	m_bExecuteLocked = false;
		//	break;

		//case MENU_DETAILREALISTIC:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetAsset()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetAsset()->InvalidateAllOps();
		//	m_fDetailFactor = 8.0f;
		//	m_bExecuteLocked = false;
		//	break;

		//case MENU_DETAILULTRA:
		//	m_bExecuteLocked = true;
		//	NEngineOp::GetAsset()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
		//	NEngineOp::GetAsset()->InvalidateAllOps();
		//	m_fDetailFactor = 16.0f;
		//	m_bExecuteLocked = false;
		//	break;
	};
*/
}
