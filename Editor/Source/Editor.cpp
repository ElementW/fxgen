//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Editor.cpp
//! \brief	FxGen Editor application
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		01-09-2008
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
#include "Editor.h"

//-----------------------------------------------------------------
//										Variables
//-----------------------------------------------------------------
const char* GetModuleName()  { return "Editor"; }
NEventsMgr*	g_pceventsMgr;
NFxGenApp		theNApp;
NGUISubSystem theGUI;

NFxGenApp*		GetApp()	{ return &theNApp; }

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
	g_pceventsMgr = NNEW(NEventsMgr);
	m_fOldTime = 0;
}

//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NFxGenApp::~NFxGenApp()
{
	NWnd* pwnd = GetGUISubSystem()->GetMainWnd();
	if (pwnd)
		NDELETE(pwnd, NWnd);

	if (g_pceventsMgr)
		NDELETE(g_pceventsMgr, NEventsMgr);


}

//-----------------------------------------------------------------
//!	\brief	Init Application
//!	\return	True if success
//-----------------------------------------------------------------
bool NFxGenApp::Init()
{
	// Create main window
	m_appWnd.Create(sf::VideoMode(WIDTH, HEIGHT), CAPTION);

	// GUI SubSystem
	NGUISubSystem* pgui = GetGUISubSystem();
	pgui->Init();

	//Create Main Frame Window
	NMainFrm* frame = NNEW( NMainFrm );
	pgui->SetMainWnd(frame);

	frame->Create(CAPTION, NRect(0,0,WIDTH,HEIGHT));	//###TOFIX### not reel client size (see wnd caption...)

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Run application
//!	\return	True if success
//-----------------------------------------------------------------
void NFxGenApp::Run()
{
	NGUISubSystem* pgui = GetGUISubSystem();
	NPoint pt;

	// Create a clock for measuring the time elapsed
  sf::Clock clock;
	bool bExist = false;

  // Start game loop
  while (m_appWnd.IsOpened() && !bExist)
  {
		//Sleep(1);

    // Process events
    sf::Event Event;
    while (m_appWnd.GetEvent(Event))
    {
      // Close window
      if (Event.Type == sf::Event::Closed)
				bExist=true;

			//Mouse Move
			if (Event.Type == sf::Event::MouseMoved)
			{
				pt.x=m_appWnd.GetInput().GetMouseX();
				pt.y=m_appWnd.GetInput().GetMouseY();
				pgui->ProcessMsgs_MouseMove(pt);
			}

			//Mouse Wheel
			if (Event.Type == sf::Event::MouseWheelMoved)
			{
				pt.x=m_appWnd.GetInput().GetMouseX();
				pt.y=m_appWnd.GetInput().GetMouseY();
				pgui->ProcessMsgs_MouseWheel(pt, Event.MouseWheel.Delta);
			}


			//Mouse Button Down
			if (Event.Type == sf::Event::MouseButtonPressed)
			{
				udword dwBut = 0;
				switch (Event.MouseButton.Button)
				{
					case sf::Mouse::Left:			dwBut = NM_LBUTTON; break;
					case sf::Mouse::Middle:		dwBut = NM_MBUTTON; break;
					case sf::Mouse::Right:		dwBut = NM_RBUTTON; break;
				}

				pt.x=m_appWnd.GetInput().GetMouseX();
				pt.y=m_appWnd.GetInput().GetMouseY();

				float Time = clock.GetElapsedTime() * 1000.0f;
				if (Time<250.0f)	//ms ###TODO## Get system Pref
				{
					pgui->ProcessMsgs_MouseButtonDblClick(pt, dwBut);

				} else {

					clock.Reset();
					pgui->ProcessMsgs_MouseButtonDown(pt, dwBut);
				}

			}

			//Mouse Button Up
			if (Event.Type == sf::Event::MouseButtonReleased)
			{
				udword dwBut = 0;
				switch (Event.MouseButton.Button)
				{
					case sf::Mouse::Left:		dwBut = NM_LBUTTON; break;
					case sf::Mouse::Middle:	dwBut = NM_MBUTTON; break;
					case sf::Mouse::Right:	dwBut = NM_RBUTTON; break;
				}
				pt.x=m_appWnd.GetInput().GetMouseX();
				pt.y=m_appWnd.GetInput().GetMouseY();
				pgui->ProcessMsgs_MouseButtonUp(pt, dwBut);
			}

			// Text entry
			if ((Event.Type == sf::Event::TextEntered) )
			{
				pgui->ProcessMsgs_Text(Event.Text.Unicode);
			}

			// Key Down
			if ((Event.Type == sf::Event::KeyPressed) )
			{
				NKey::Code key = (NKey::Code)Event.Key.Code;	//KeyCode are identical
				pgui->ProcessMsgs_KeyDown(key);
			}

			// Key Up
			if ((Event.Type == sf::Event::KeyReleased) )
			{
				NKey::Code key = (NKey::Code)Event.Key.Code;	//KeyCode are identical
				pgui->ProcessMsgs_KeyUp(key);
			}

      // Resize
      if (Event.Type == sf::Event::Resized)
				pgui->ProcessMsgs_OnSize(Event.Size.Width, Event.Size.Height);

    }

		//Double Click TimeOut
		/*if (bDblClickStarted)
		{
			float Time = clock.GetElapsedTime() * 1000.0f;
			if (Time>150.0f)	//50ms
			{
				pgui->ProcessMsgs_MouseButtonDown(ptDblClick, dwDblClickButton);

				bDblClickStarted = false;

				if (bMouseReleased)
				{
					pgui->ProcessMsgs_MouseButtonUp(ptDblClick, dwDblClickButton);
				}
			}

		}*/


		//Update Windows drawing
		pgui->Update();

		//Update Application
		Update();

    // Finally, display the rendered frame on screen
    m_appWnd.Display();
  }


}

//-----------------------------------------------------------------
//!	\brief	Exit application
//!	\return	True if success
//-----------------------------------------------------------------
bool NFxGenApp::Exit()
{
	GetGUISubSystem()->ShutDown();
	m_appWnd.Close();

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Update
//-----------------------------------------------------------------
void NFxGenApp::Update()
{
	NMainFrm* pfrm = (NMainFrm*)GetGUISubSystem()->GetMainWnd();

	//Time 60 Images/Seconds
	float ftime = (float)clock() * 60.0f / 1000.0f;
	if (m_fOldTime==0)		m_fOldTime = ftime;

	if (ftime-m_fOldTime>1.0f)
	{
		//Execute operators rendering
		NOperatorNode* pop = pfrm->Execute(ftime);

	}

}

//-----------------------------------------------------------------
//!	\brief	MessageBox
//-----------------------------------------------------------------
#ifdef WIN32
udword NFxGenApp::MessageBox(char* _pszText, udword _dwStyle)
{
	udword dwW32Style = 0;
	if (_dwStyle&NMB_OK)			dwW32Style|=MB_OK;
	if (_dwStyle&NMB_YESNO)		dwW32Style|=MB_YESNO;
	udword dwW32Ret = ::MessageBox(NULL, _pszText, "FxGen", dwW32Style);
	if (dwW32Ret==IDOK)		return NIDOK;
	if (dwW32Ret==IDYES)	return NIDYES;
	if (dwW32Ret==IDNO)		return NIDNO;

	return 0;
}
#else	//###TODO### Linux, Mac...
udword NFxGenApp::MessageBox(char* _szTest, udword _dwFlag)
{

	return 0;
}
#endif
