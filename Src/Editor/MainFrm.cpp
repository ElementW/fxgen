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
#include ".\mainfrm.h"

#include "OperatorsWnd.h"
#include "PropertiesWnd.h"
#include "ViewportsWnd.h"
#include "ProjectWnd.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------

#define	MENU_NEWPROJECT		100
#define	MENU_OPENPROJECT	101
#define	MENU_SAVEPROJECTAS	102
#define	MENU_EXIT			103
#define	MENU_SAVEPROJECT	104
#define	MENU_RELOADPROJECT	105

#define	MENU_DETAILLOW		200
#define	MENU_DETAILNORMAL	201
#define	MENU_DETAILHIGH		202


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NMainFrm Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//###TEST###
/*void staticProcess(udword _dwCurrentOp, udword _dwTotalOps)
{
	TRACE("Current %d Total %d\n", _dwCurrentOp, _dwTotalOps);
}*/


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
	// Creation des fenetres dans la split window
	NSplitWnd* pwrkspace = (NSplitWnd*)GetWorkspace();

	//Creation de la fenetre pour les operators
	m_opswnd = new NOperatorsWnd;
	m_opswnd->Create("", GetClientRect(), pwrkspace);

	//Creation de la fenetre pour les properties
	NPropertiesWnd* propswnd = new NPropertiesWnd;
	propswnd->Create("", GetClientRect(), pwrkspace);

	//Creation de la fenetre pour les viewports
	NViewportsWnd* viewportswnd = new NViewportsWnd;
	viewportswnd->Create("", GetClientRect(), pwrkspace);

	//Creation des tabs pour les pages d'operateurs et les operateurs stored
	//NTabCtrl* ptabRes = new NTabCtrl;
	//ptabRes->Create("", GetClientRect(), pwrkspace);

	m_pprojectwnd = new NProjectWnd;
	m_pprojectwnd->Create("Project", GetClientRect(), pwrkspace/*ptabRes*/);

	//m_pstoredwnd = new NStoredWnd;
	//m_pstoredwnd->Create("Stored", GetClientRect(), ptabRes);

	//ptabRes->InsertItem(0, "Project", m_pprojectwnd);
	//ptabRes->InsertItem(1, "Stored", m_pstoredwnd);
	//ptabRes->SelectItem(0);

	///////////////////////////////////////////////
	// Split du mainframe
	udword dwViewportsPaneID	= SPLITPANE_DEFAULT_ID;
	udword dwResourcesPaneID	= pwrkspace->SplitRow(dwViewportsPaneID, 50);
	udword dwPropertiesPaneID = pwrkspace->SplitColumn(dwViewportsPaneID, 70);
	udword dwOperatorsPaneID	= pwrkspace->SplitColumn(dwResourcesPaneID, 20);
	pwrkspace->SetPaneWnd("Viewport", dwViewportsPaneID,	viewportswnd);
	pwrkspace->SetPaneWnd("Operators", dwOperatorsPaneID,	m_opswnd);
	pwrkspace->SetPaneWnd("Properties", dwPropertiesPaneID,	propswnd);
	pwrkspace->SetPaneWnd("Project", dwResourcesPaneID,	m_pprojectwnd);

	///////////////////////////////////////////////
	// Creation du Menu
	NMenuBar*	pmenu = GetMenuBar();
	udword dwParent = pmenu->InsertPopupItem(0, 0, "File");

	pmenu->InsertItem(dwParent,1,"New...",	MENU_NEWPROJECT);
	pmenu->InsertItem(dwParent,2,"Reload	F9", MENU_RELOADPROJECT);
	pmenu->InsertItem(dwParent,3,"Open...	F3", MENU_OPENPROJECT);
	pmenu->InsertItem(dwParent,4,"Save	F6", MENU_SAVEPROJECT);
	pmenu->InsertItem(dwParent,5,"Save as...	F2", MENU_SAVEPROJECTAS);
	pmenu->InsertItem(dwParent,6,"Exit...",					MENU_EXIT);


	dwParent = pmenu->InsertPopupItem(0,				1, "Options");
	dwParent = pmenu->InsertPopupItem(dwParent, 0, "Details");

	pmenu->InsertItem(dwParent, 1, "Low\t50%",			MENU_DETAILLOW);
	pmenu->InsertItem(dwParent, 2, "Normal\t100%",	MENU_DETAILNORMAL);
	pmenu->InsertItem(dwParent, 3, "High\t200%",		MENU_DETAILHIGH);

	///////////////////////////////////////////////
	// Creation d'un nouveau projet par defaut
	//				###TEST###
	OnNewProject();

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Command message
//!	\param	id	command identifier
//-----------------------------------------------------------------
void NMainFrm::OnCommand(udword id)
{
	switch (id)
	{
		case MENU_NEWPROJECT:			OnNewProject();				break;
		case MENU_OPENPROJECT:		OnOpenProject();			break;
		case MENU_RELOADPROJECT:	LoadProject();			break;
		case MENU_SAVEPROJECTAS:	OnSaveProjectAs();		break;
		case MENU_SAVEPROJECT:		SaveProject();				break;
		case MENU_EXIT:						GetApp()->AskExit();	break;

		case MENU_DETAILLOW:
			m_bExecuteLocked = true;
			gNFxGen_GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			gNFxGen_GetEngine()->InvalidateAllOps();
			m_fDetailFactor = 0.5f;
			m_bExecuteLocked = false;
			break;

		case MENU_DETAILNORMAL:
			m_bExecuteLocked = true;
			gNFxGen_GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			gNFxGen_GetEngine()->InvalidateAllOps();
			m_fDetailFactor = 1.0f;
			m_bExecuteLocked = false;
			break;

		case MENU_DETAILHIGH:
			m_bExecuteLocked = true;
			gNFxGen_GetEngine()->GetBitmapGarbage()->Compact(OBJRES_TYPE_INTERMEDIATE|OBJRES_TYPE_STORED|OBJRES_TYPE_FINALSTORED,0);
			gNFxGen_GetEngine()->InvalidateAllOps();
			m_fDetailFactor = 2.0f;
			m_bExecuteLocked = false;
			break;

	};

}


//-----------------------------------------------------------------
//!	\brief	Project creation
//-----------------------------------------------------------------
void NMainFrm::OnNewProject()
{
	NTreeNode* prootNode = gNFxGen_GetEngine()->GetRootGroup();
	udword dwCount = prootNode->GetSonsCount();

	if (dwCount!=0)
	{
		udword dwRet = GetApp()->MessageBox("You will lost current project ! do you want to continue ?", MB_YESNO|MB_DEFBUTTON2);
		if (dwRet==IDNO)		return;
	}

	m_popMarkedShow = null;
	m_bExecuteLocked = true;
	projectname = "";

	//New project
	gNFxGen_GetEngine()->Clear();

	//Create empty project
	prootNode = gNFxGen_GetEngine()->GetRootGroup();

	NTreeNode* pNewGrpNode = new NTreeNode;
	pNewGrpNode->SetName("Group");
	prootNode->AddSon(pNewGrpNode);

	NOperatorsPage* ppage = new NOperatorsPage;
	ppage->SetName("Page");
	pNewGrpNode->GetObjsArray().AddItem(ppage);


	//Display Projects's Pages
	m_pprojectwnd->DisplayOperatorsProject(gNFxGen_GetEngine());
	m_pprojectwnd->SelectFirstPage();

	//m_opswnd->DisplayOperatorsPage(null);
	//m_opswnd->Update();

	NString strTitle(CAPTION);
	strTitle+="Untitled";
	SetWindowText(strTitle.Buffer());

	m_bExecuteLocked = false;
}

void NMainFrm::LoadProject(NString str)
{
	if(!str.Length())
		str = projectname;
	if(!str.Length())
	{
		OnOpenProject();
		return;
	}

		m_popMarkedShow = null;
		m_bExecuteLocked = true;

		gNFxGen_GetEngine()->Clear();

		m_pprojectwnd->DisplayOperatorsProject(gNFxGen_GetEngine());
		m_pprojectwnd->SelectFirstPage();

		if (gNFxGen_GetEngine()->LoadProject(str.Buffer()))
		{
			projectname = str;
			NString strTitle(CAPTION);
			strTitle+=str.Buffer();
			SetWindowText(strTitle.Buffer());

			//###TEST###
			//gNFxGen_GetEngine()->ProcessOperators(0.0, staticProcess);

		} else {
			gNFxGen_GetEngine()->Clear();
		}

		//Display Projects's Pages
		m_pprojectwnd->DisplayOperatorsProject(gNFxGen_GetEngine());
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
		LoadProject(dlg.GetPathName());
}

void NMainFrm::SaveProject(NString path)
{
	if(!path.Length())
		path = projectname;
	if(!path.Length())
	{
		OnSaveProjectAs();
		return;
	}

	m_bExecuteLocked = true;

	if (gNFxGen_GetEngine()->SaveProject(path.Buffer()))
	{
		projectname = path;
		NString strTitle(CAPTION);
		strTitle+=path.Buffer();
		SetWindowText(strTitle.Buffer());
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
	if (!m_bExecuteLocked)
	{
		m_bExecuteLocked = true;
		gNFxGen_GetEngine()->Execute(_ftime, m_popMarkedShow, m_fDetailFactor);
		//Rendering
		EVT_EXECUTE(EVT_RENDER, (udword)m_popMarkedShow, (udword)&_ftime);
		m_bExecuteLocked = false;
	}

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
	//Time
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

void NMainFrm::OnKeyDown(udword dwchar)
{
	switch(dwchar)
	{
		case VK_F6:	SaveProject();			break;
		case VK_F9:	LoadProject();			break;
		case VK_F2:	OnSaveProjectAs();	break;
		case VK_F3:	OnOpenProject();		break;
		default: break;
	}
}
