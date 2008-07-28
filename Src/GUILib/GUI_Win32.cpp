//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		GUI_Win32.h
//! \brief	Win32 specific implementation
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		23-06-2008
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "GUI_Win32.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Variables
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Win32 Application class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NW32Application::NW32Application() : NbaseApplication()
{
	m_hDC=null;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NW32Application::~NW32Application()
{
}

//-----------------------------------------------------------------
//!	\brief	Initialize application
//!	\return	true if success
//-----------------------------------------------------------------
bool NW32Application::Init()
{
	//Base class
	NbaseApplication::Init();

	// Windows class registration
	WNDCLASS				wc;
	wc.style				= CS_BYTEALIGNCLIENT | CS_DBLCLKS;
	wc.lpfnWndProc	= (WNDPROC) NW32Application::StaticWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= GetModuleHandle (NULL); 
	wc.hIcon				= NULL;
	wc.hCursor			= LoadCursor ((HINSTANCE)NULL, IDC_ARROW);
	wc.hbrBackground= (HBRUSH)(COLOR_APPWORKSPACE);
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= "FXGEN_WNGraphicsLASS";
	RegisterClass(&wc);	

	//Create Window
	udword w=800;		udword h=600;

	m_hW32Wnd = CreateWindowEx(0, "FXGEN_WNGraphicsLASS", "FxGen",
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
    0, 0, w, h,
		NULL, NULL, GetModuleHandle(NULL), NULL );

	//Set user data in param for retrieve this class pointer
	::SetWindowLong (m_hW32Wnd, GWL_USERDATA, (LONG)this);


	//////////////////////////////////////////////
	// OpenGL Initialisation
  PIXELFORMATDESCRIPTOR pfd;
  pfd.cColorBits = pfd.cDepthBits = 32;
  pfd.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	m_hDC = GetDC( m_hW32Wnd );
	SetPixelFormat( m_hDC, ChoosePixelFormat ( m_hDC, &pfd) , &pfd );
	wglMakeCurrent( m_hDC, wglCreateContext(m_hDC) );

	//OpenGL
	InitGLState();


	/////////////////////////////////////////////
	//Map VK keys
	for (udword i=0; i<NK_LAST; ++i )
		m_keymap[i] = NK_UNKNOWN;

	m_keymap[VK_BACK] = NK_BACKSPACE;
	m_keymap[VK_TAB] = NK_TAB;
	m_keymap[VK_CLEAR] = NK_CLEAR;
	m_keymap[VK_RETURN] = NK_RETURN;
	m_keymap[VK_PAUSE] = NK_PAUSE;
	m_keymap[VK_ESCAPE] = NK_ESCAPE;
	m_keymap[VK_SPACE] = NK_SPACE;
	m_keymap[VK_APOSTROPHE] = NK_QUOTE;
	m_keymap[VK_COMMA] = NK_COMMA;
	m_keymap[VK_MINUS] = NK_MINUS;
	m_keymap[VK_PERIOD] = NK_PERIOD;
	m_keymap[VK_SLASH] = NK_SLASH;
	m_keymap[VK_0] = NK_0;
	m_keymap[VK_1] = NK_1;
	m_keymap[VK_2] = NK_2;
	m_keymap[VK_3] = NK_3;
	m_keymap[VK_4] = NK_4;
	m_keymap[VK_5] = NK_5;
	m_keymap[VK_6] = NK_6;
	m_keymap[VK_7] = NK_7;
	m_keymap[VK_8] = NK_8;
	m_keymap[VK_9] = NK_9;
	m_keymap[VK_SEMICOLON] = NK_SEMICOLON;
	m_keymap[VK_EQUALS] = NK_EQUALS;
	m_keymap[VK_LBRACKET] = NK_LEFTBRACKET;
	m_keymap[VK_BACKSLASH] = NK_BACKSLASH;
	m_keymap[VK_OEM_102] = NK_LESS;
	m_keymap[VK_RBRACKET] = NK_RIGHTBRACKET;
	m_keymap[VK_GRAVE] = NK_BACKQUOTE;
	m_keymap[VK_BACKTICK] = NK_BACKQUOTE;
	m_keymap[VK_A] = NK_a;
	m_keymap[VK_B] = NK_b;
	m_keymap[VK_C] = NK_c;
	m_keymap[VK_D] = NK_d;
	m_keymap[VK_E] = NK_e;
	m_keymap[VK_F] = NK_f;
	m_keymap[VK_G] = NK_g;
	m_keymap[VK_H] = NK_h;
	m_keymap[VK_I] = NK_i;
	m_keymap[VK_J] = NK_j;
	m_keymap[VK_K] = NK_k;
	m_keymap[VK_L] = NK_l;
	m_keymap[VK_M] = NK_m;
	m_keymap[VK_N] = NK_n;
	m_keymap[VK_O] = NK_o;
	m_keymap[VK_P] = NK_p;
	m_keymap[VK_Q] = NK_q;
	m_keymap[VK_R] = NK_r;
	m_keymap[VK_S] = NK_s;
	m_keymap[VK_T] = NK_t;
	m_keymap[VK_U] = NK_u;
	m_keymap[VK_V] = NK_v;
	m_keymap[VK_W] = NK_w;
	m_keymap[VK_X] = NK_x;
	m_keymap[VK_Y] = NK_y;
	m_keymap[VK_Z] = NK_z;
	m_keymap[VK_DELETE] = NK_DELETE;

	m_keymap[VK_NUMPAD0] = NK_KP0;
	m_keymap[VK_NUMPAD1] = NK_KP1;
	m_keymap[VK_NUMPAD2] = NK_KP2;
	m_keymap[VK_NUMPAD3] = NK_KP3;
	m_keymap[VK_NUMPAD4] = NK_KP4;
	m_keymap[VK_NUMPAD5] = NK_KP5;
	m_keymap[VK_NUMPAD6] = NK_KP6;
	m_keymap[VK_NUMPAD7] = NK_KP7;
	m_keymap[VK_NUMPAD8] = NK_KP8;
	m_keymap[VK_NUMPAD9] = NK_KP9;
	m_keymap[VK_DECIMAL] = NK_KP_PERIOD;
	m_keymap[VK_DIVIDE] = NK_KP_DIVIDE;
	m_keymap[VK_MULTIPLY] = NK_KP_MULTIPLY;
	m_keymap[VK_SUBTRACT] = NK_KP_MINUS;
	m_keymap[VK_ADD] = NK_KP_PLUS;

	m_keymap[VK_UP] = NK_UP;
	m_keymap[VK_DOWN] = NK_DOWN;
	m_keymap[VK_RIGHT] = NK_RIGHT;
	m_keymap[VK_LEFT] = NK_LEFT;
	m_keymap[VK_INSERT] = NK_INSERT;
	m_keymap[VK_HOME] = NK_HOME;
	m_keymap[VK_END] = NK_END;
	m_keymap[VK_PRIOR] = NK_PAGEUP;
	m_keymap[VK_NEXT] = NK_PAGEDOWN;

	m_keymap[VK_F1] = NK_F1;
	m_keymap[VK_F2] = NK_F2;
	m_keymap[VK_F3] = NK_F3;
	m_keymap[VK_F4] = NK_F4;
	m_keymap[VK_F5] = NK_F5;
	m_keymap[VK_F6] = NK_F6;
	m_keymap[VK_F7] = NK_F7;
	m_keymap[VK_F8] = NK_F8;
	m_keymap[VK_F9] = NK_F9;
	m_keymap[VK_F10] = NK_F10;
	m_keymap[VK_F11] = NK_F11;
	m_keymap[VK_F12] = NK_F12;
	m_keymap[VK_F13] = NK_F13;
	m_keymap[VK_F14] = NK_F14;
	m_keymap[VK_F15] = NK_F15;

	m_keymap[VK_NUMLOCK] = NK_NUMLOCK;
	m_keymap[VK_CAPITAL] = NK_CAPSLOCK;
	m_keymap[VK_SCROLL] = NK_SCROLLOCK;
	m_keymap[VK_RSHIFT] = NK_RSHIFT;
	m_keymap[VK_LSHIFT] = NK_LSHIFT;
	m_keymap[VK_RCONTROL] = NK_RCTRL;
	m_keymap[VK_LCONTROL] = NK_LCTRL;
	m_keymap[VK_RMENU] = NK_RALT;
	m_keymap[VK_LMENU] = NK_LALT;
	m_keymap[VK_RWIN] = NK_RSUPER;
	m_keymap[VK_LWIN] = NK_LSUPER;

	m_keymap[VK_HELP] = NK_HELP;
#ifdef VK_PRINT
	m_keymap[VK_PRINT] = NK_PRINT;
#endif
	m_keymap[VK_SNAPSHOT] = NK_PRINT;
	m_keymap[VK_CANCEL] = NK_BREAK;
	m_keymap[VK_APPS] = NK_MENU;

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Main loop
//-----------------------------------------------------------------
void NW32Application::Run()
{
	NbaseApplication::Run();

	//
	RECT rc;
	::GetClientRect(m_hW32Wnd, &rc);
	SendMessage(m_hW32Wnd, WM_SIZE, 0, rc.bottom<<16|rc.right);

	//Polling messages from event queue until quit
	MSG	msg;
	while(1)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
		{
			while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				switch (msg.message)
				{
					//Exit
					case WM_QUIT:
						return;
				}

				::TranslateMessage(&msg);		//Translates virtual-key messages into character messages
				::DispatchMessage(&msg);		//Dispatches a message to window proc
			}
		} else {
		 Idle();
		}

		Update();

		if (m_bMustDrawWindows)
		{
			_RedrawWindowChild(null);
			_RedrawWindowPopup(null);

			glFlush();
	    SwapBuffers( m_hDC );

			m_bMustDrawWindows=false;
		}

	}

}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Name:	Windows Proc()
// Desc:	...
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK NW32Application::StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	NW32Application* _this = (NW32Application*)::GetWindowLong (hwnd, GWL_USERDATA);
	NWnd* pwnd;
	NPoint pt;
	udword dwKey;

	//Check W32 Messages
	if (_this)
	{
		switch (msg)
		{
			//Paint Messages
			//case WM_PAINT:				OnPaint();					break;

			case WM_CLOSE:
				PostQuitMessage(0);
			break;

			//Resize Messages
			case WM_SIZE:
				_this->ProcessMsgs_OnSize(LOWORD(lparam), HIWORD(lparam));
			break;

			//Mouse Messages
			case WM_LBUTTONDOWN:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				TRACE("%d, %d\n", pt.x, pt.y);

				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);

				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					pwnd->OnLeftButtonDown(0, pt);
				}

				break;

			case WM_RBUTTONDOWN:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);

				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);

				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					pwnd->OnRightButtonDown(0, pt);
				}

				break;

			case WM_MBUTTONDOWN:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);

				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);

				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					pwnd->OnMButtonDown(0, pt);
				}

				break;


			case WM_LBUTTONUP:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);

				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);

				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					pwnd->OnLeftButtonUp(0, pt);
				}

				break;

			case WM_LBUTTONDBLCLK:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);

				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);

				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					pwnd->OnLeftButtonDblClk(0, pt);
				}

				break;

			case WM_RBUTTONUP:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);

				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);

				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					pwnd->OnRightButtonUp(0, pt);
				}

				break;

			case WM_MBUTTONUP:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);

				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);

				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					pwnd->OnMButtonUp(0, pt);
				}

				break;

			case WM_MOUSEMOVE:
				//TRACE("WM_MOUSEMOVE %d\n", wparam);
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);

				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);
				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					//TRACE("Mouse %s %dx%d\n", pwnd->GetText(), pt.x, pt.y);

					pwnd->OnMouseMove(0, pt);
				}
				if (_this->m_pOldWndUnderMouse && pwnd!=_this->m_pOldWndUnderMouse)
					_this->m_pOldWndUnderMouse->OnMouseLeave();
				_this->m_pOldWndUnderMouse = pwnd;
				break;


			case WM_MOUSEWHEEL:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				if (_this->m_pCapturedWnd)	pwnd=_this->m_pCapturedWnd;
				else												pwnd = _this->GetWndUnderMouse(pt.x, pt.y);

				if (pwnd)
				{
					pwnd->ScreenToClient(pt);
					pwnd->OnMouseWheel(wparam, (sword)HIWORD(wparam), pt);
				}
				break;

			//Keyboard Messages
			case WM_KEYDOWN:
				dwKey = _this->W32KeyCodeToFxGen(wparam);
				pwnd = _this->GetFocus();
				if (pwnd)	pwnd->OnKeyDown(dwKey);
				break;

			case WM_KEYUP:
				dwKey = _this->W32KeyCodeToFxGen(wparam);
				pwnd = _this->GetFocus();
				if (pwnd)	pwnd->OnKeyUp(dwKey);
				break;

		}

	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}


//-----------------------------------------------------------------
//!	\brief	Exit application
//!	\return	true if success
//-----------------------------------------------------------------
bool NW32Application::Exit()
{
	NbaseApplication::Exit();


	return true;
}

//-----------------------------------------------------------------
//!	\brief	Ask exit
//-----------------------------------------------------------------
void NW32Application::AskExit()
{
	PostQuitMessage(0);
}

//-----------------------------------------------------------------
//!	\brief	Return mouse cursor position
//-----------------------------------------------------------------
void NW32Application::GetCursorPos(NPoint& _pos)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(m_hW32Wnd, &pt);
	_pos.x=(sdword)pt.x;		_pos.y=(sdword)pt.y;
}

//-----------------------------------------------------------------
//!	\brief	Set mouse cursor position
//-----------------------------------------------------------------
void NW32Application::SetCursorPos(const NPoint& _pos)
{
	POINT pt;
	pt.x=(sdword)_pos.x;		pt.y=(sdword)_pos.y;
	::ClientToScreen(m_hW32Wnd, &pt);
	::SetCursorPos(pt.x, pt.y);
}


//-----------------------------------------------------------------
//!	\brief	MessageBox
//-----------------------------------------------------------------
udword NW32Application::MessageBox(char* _pszText, udword _dwStyle)
{
	udword dwW32Style = 0;
	if (_dwStyle&NMB_OK)			dwW32Style|=MB_OK;
	if (_dwStyle&NMB_YESNO)		dwW32Style|=MB_YESNO;
	udword dwW32Ret = ::MessageBox(m_hW32Wnd, _pszText, "FxGen", dwW32Style);
	if (dwW32Ret==IDOK)		return NIDOK;
	if (dwW32Ret==IDYES)	return NIDYES;
	if (dwW32Ret==IDNO)		return NIDNO;

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	W32KeyCodeToFxGen
//-----------------------------------------------------------------
udword NW32Application::W32KeyCodeToFxGen(udword _w32keycode)
{
	udword fxgenKey = m_keymap[_w32keycode];
	//TRACE("_w32keycode %d\n", _w32keycode);
	//if (_w32keycode==VK_DELETE)	fxgenKey=VK_DELETE;
	return fxgenKey;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Win32 File Dialog Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NW32FileDialog::Create(char* name, NWnd* parent, bool open, bool multiselect)
{

	//Open or Save Display
	mOpenFileDialog = open;

	//Win32 Open Dialog Box Structure
	memset( &mOfn, 0, sizeof(OPENFILENAME));

	mOfn.lStructSize		= sizeof(OPENFILENAME);
	mOfn.hInstance			= GetModuleHandle (null);

 /* if (parent!=null)		mOfn.hwndOwner = parent->m_W32HWnd;
	else*/								mOfn.hwndOwner = null;

	//To Do...
	NString Filename	= "";
	NString DefExt		= "";
	NString Filter		= "All Files (*.*)|*.*||";
	NString InitialDir	= "";

	//Filters
		//Translate string filter into OPENFILENAME format (lots of \0)
	sdword idx;
	if (Filter.Length() )
	{
		mTransfilter = Filter;
		while ((idx = mTransfilter.ReverseFind('|')) != -1)		mTransfilter.Buffer()[idx]=0;		//Change '|' with '\0'
	}
	//Fill Struct
	mOfn.lpstrFilter				= (LPCTSTR)mTransfilter.Buffer();
	mOfn.lpstrCustomFilter	= NULL;
	mOfn.nMaxCustFilter			= 0;
	mOfn.nFilterIndex				= 0;

	//Default File Name edit control (return)
	// setup initial file name
	ZeroMemory(mFileName, sizeof(mFileName));
	if (Filename.Length())	lstrcpyn((LPTSTR)mFileName, (char*)Filename.Buffer(), sizeof(mFileName));
	//Fill Struct
	mOfn.lpstrFile			= (LPTSTR)mFileName;
	mOfn.nMaxFile				= sizeof(mFileName);

	//Selected File Name (return)
	mOfn.lpstrFileTitle		= NULL;
	mOfn.nMaxFileTitle		= 0;

	//Default File Directory
   mOfn.lpstrInitialDir	= null;	//(LPCTSTR)InitialDir.Buffer();

	//Dialog Title Name
	mOfn.lpstrTitle			= name;

	//Offset
	mOfn.nFileOffset		= 0;
  mOfn.nFileExtension	= 0;

	//Default extension
	mOfn.lpstrDefExt		= null;	//(LPCTSTR)DefExt.Buffer();

	//Flags
  mOfn.Flags					= OFN_EXPLORER;
	if (multiselect)		mOfn.Flags|=OFN_ALLOWMULTISELECT;

	//...
  mOfn.lCustData			= 0;
	mOfn.lpfnHook				= NULL;
  mOfn.lpTemplateName	= NULL;

	return true;
}


//-----------------------------------------------------------------
// Name:	DoModal()
// Desc:	...
//-----------------------------------------------------------------
udword NW32FileDialog::DoModal()
{
	udword Result;

	if	(mOpenFileDialog)			{

		//If the Win32 method returns 0, it means that the opening failed, so DoModal has to returns 0 too
		if (::GetOpenFileName(&mOfn))		{
			//if the Win32 method returns 1, the opening worked well, an then, DoModal returns the number of opened files
			return GetFileNumber();
		}	else	return 0;
	}
	else	Result = ::GetSaveFileName(&mOfn);	//Save Dialog

	return Result;
}


