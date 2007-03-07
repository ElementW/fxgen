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
	NNopOp op;		//Force Register	###TOFIX###
	NStoreOp op1;	//Force Register	###TOFIX###

	g_pceventsMgr = new NEventsMgr;
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
