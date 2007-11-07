//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Editor.cpp
//! \brief	FxGen Editor application
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
#include "WinMain.h"
#include "Editor.h"



//-----------------------------------------------------------------
//										Variables
//-----------------------------------------------------------------
NEventsMgr*	g_pceventsMgr;
NFxGenApp		theNApp;

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NFxGenApp Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NFxGenApp::NFxGenApp()
{
	g_pceventsMgr = new NEventsMgr;

	//###TEST### NTreeNode class
/*	NTreeNode* proot	= new NTreeNode;
	proot->SetName("Root");

	NTreeNode* pgroup1 = new NTreeNode;
	pgroup1->SetName("Group1");

	NTreeNode* pgroup2 = new NTreeNode;
	pgroup2->SetName("Group2");

	proot->AddSon(pgroup1, -1);
	proot->AddSon(pgroup2, -1);

	proot->DeleteSon(1);

	pgroup2 = new NTreeNode;
	pgroup2->SetName("Group2");

	proot->AddSon(pgroup2, -1);
	TRACE("");*/

	//###TEST### NObject reference from variables
/*
	NObject* pobjRefTarget = new NObject();
	pobjRefTarget->SetName(".Target");

	NObject* pobjRefMaker01 = new NObject();
	pobjRefMaker01->SetName(".Maker01");

	NObject* pobjRefMaker02 = new NObject();
	pobjRefMaker02->SetName(".Maker02");

	NVarsBlocDesc blocdescOp[] =
	{
		VAR(erefobj,	false, "Load",	"0", "NUseStoredOpsProp")	//0
	};

	NVarsBloc* pcvarsBloc01 = pobjRefMaker01->AddVarsBloc(1, blocdescOp, 1);
	NVarsBloc* pcvarsBloc02 = pobjRefMaker02->AddVarsBloc(1, blocdescOp, 1);

	pcvarsBloc01->SetValue(0, 0.0, pobjRefTarget);
	pcvarsBloc02->SetValue(0, 0.0, pobjRefTarget);

	NOperator* popRef = null;
	pcvarsBloc01->GetValue(0, 0.0, (NObject*&)popRef);
	pcvarsBloc02->GetValue(0, 0.0, (NObject*&)popRef);

	delete pobjRefTarget;

	pcvarsBloc01->GetValue(0, 0.0, (NObject*&)popRef);
	pcvarsBloc02->GetValue(0, 0.0, (NObject*&)popRef);

	TRACE("");
*/
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NFxGenApp::~NFxGenApp()
{
	delete g_pceventsMgr;
	g_pceventsMgr = null;
}

//-----------------------------------------------------------------
//!	\brief	Init Application
//!	\return	True if success
//-----------------------------------------------------------------
bool NFxGenApp::Init()
{
	NApplication::Init();

	//Create Main Frame Window
	NMainFrm* frame = new NMainFrm();
	frame->Create(CAPTION, NRect(200,100,200+WIDTH,100+HEIGHT));
	m_pMainWnd = frame;
	if(lpCmdLine.Length())
		frame->LoadProject(lpCmdLine);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Exit application
//!	\return	True if success
//-----------------------------------------------------------------
bool NFxGenApp::Exit()
{

	return NApplication::Exit();
}

//-----------------------------------------------------------------
//!	\brief	IDLE
//-----------------------------------------------------------------
void NFxGenApp::Idle()
{
	NMainFrm* pfrm = (NMainFrm*)m_pMainWnd;

	//Time
	float ftime = (float)GetTickCount() * 60.0f / 1000.0f;

	//Execute operators rendering
	NOperator* pop = pfrm->Execute(ftime);

	Sleep(1);	//???

	NApplication::Idle();
}
