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
#define	MENU_SAVEPROJECT	102
#define	MENU_EXIT					103


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NMainFrm Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NMainFrm::NMainFrm(void)
{
	m_bExecuteLocked	= false;
	m_popMarkedShow		= null;
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
bool NMainFrm::Create(char* name, NRect& rect)
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
	pmenu->InsertItem(dwParent,1,"New Project...",	MENU_NEWPROJECT);
	pmenu->InsertItem(dwParent,2,"Open Project...", MENU_OPENPROJECT);
	pmenu->InsertItem(dwParent,3,"Save Project...", MENU_SAVEPROJECT);
	pmenu->InsertItem(dwParent,4,"Exit...",					MENU_EXIT);

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
		case MENU_NEWPROJECT:		OnNewProject();				break;
		case MENU_OPENPROJECT:	OnOpenProject();			break;
		case MENU_SAVEPROJECT:	OnSaveProject();			break;
		case MENU_EXIT:					GetApp()->AskExit();	break;
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

	m_opswnd->DisplayOperatorsPage(null);
	m_opswnd->Update();

	NString strTitle(CAPTION);
	strTitle+="Untitled";
	SetWindowText(strTitle.Buffer());

	m_bExecuteLocked = false;
}

//-----------------------------------------------------------------
//!	\brief	Project loading
//-----------------------------------------------------------------
void NMainFrm::OnOpenProject()
{
	m_popMarkedShow = null;
	m_bExecuteLocked = true;

	//Open File Dialog
	NFileDialog dlg;
	dlg.Create("Opening Project...", this);
	if (dlg.DoModal())
	{
		NString str = dlg.GetPathName();

		if (gNFxGen_GetEngine()->LoadProject(str.Buffer()))
		{
			NString strTitle(CAPTION);
			strTitle+=str.Buffer();
			SetWindowText(strTitle.Buffer());
		}

		//Display Projects's Pages
		m_pprojectwnd->DisplayOperatorsProject(gNFxGen_GetEngine());
	}

	m_bExecuteLocked = false;
}

//-----------------------------------------------------------------
//!	\brief	Project saving
//-----------------------------------------------------------------
void NMainFrm::OnSaveProject()
{
	m_bExecuteLocked = true;

	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Saving Project...", this, false);
	if (dlg.DoModal())
	{
		NString str = dlg.GetPathName();
		if (gNFxGen_GetEngine()->SaveProject(str.Buffer()))
		{
			NString strTitle(CAPTION);
			strTitle+=str.Buffer();
			SetWindowText(strTitle.Buffer());
		}
	}

	m_bExecuteLocked = false;
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
		gNFxGen_GetEngine()->Execute(_ftime, m_popMarkedShow);
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
