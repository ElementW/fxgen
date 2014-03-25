//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		GUI_Win32.h
//! \brief	Win32 specific implementation
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
#include "CoreLibPkg.h"
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
NW32Application::NW32Application()
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

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Main loop
//-----------------------------------------------------------------
void NW32Application::Run()
{
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

		/*if (m_bMustDrawWindows)
		{
			_RedrawWindowChild(null);
			_RedrawWindowPopup(null);

			glFlush();
	    SwapBuffers( m_hDC );

			m_bMustDrawWindows=false;
		}*/

	}

}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Name:	Windows Proc()
// Desc:	...
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK NW32Application::StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	NW32Application* _this = (NW32Application*)::GetWindowLong (hwnd, GWL_USERDATA);
	NGUIWnd* pwnd;
	NPoint pt;
	udword dwKey;
	NGUISubSystem* pgui = GetGUISubSystem();

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
				pgui->ProcessMsgs_OnSize(LOWORD(lparam), HIWORD(lparam));
			break;

			//Mouse Messages
			case WM_LBUTTONDOWN:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				pgui->ProcessMsgs_MouseButtonDown(pt, NM_LBUTTON);
				break;
				
			case WM_RBUTTONDOWN:
					pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
					pgui->ProcessMsgs_MouseButtonDown(pt, NM_RBUTTON);
				break;

			case WM_MBUTTONDOWN:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				pgui->ProcessMsgs_MouseButtonDown(pt, NM_MBUTTON);
				break;


			case WM_LBUTTONUP:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				pgui->ProcessMsgs_MouseButtonUp(pt, NM_LBUTTON);
				break;


			case WM_LBUTTONDBLCLK:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				pgui->ProcessMsgs_MouseButtonDblClick(pt, NM_LBUTTON);
				break;

			case WM_RBUTTONUP:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				pgui->ProcessMsgs_MouseButtonUp(pt, NM_RBUTTON);
				break;

			case WM_MBUTTONUP:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				pgui->ProcessMsgs_MouseButtonUp(pt, NM_MBUTTON);
				break;

			case WM_MOUSEMOVE:
				//TRACE("WM_MOUSEMOVE %d\n", wparam);
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				pgui->ProcessMsgs_MouseMove(pt);
				break;

			case WM_MOUSEWHEEL:
				pt.x = LOWORD(lparam);	pt.y=HIWORD(lparam);
				pgui->ProcessMsgs_MouseWheel(pt, (sword)HIWORD(wparam));
				break;

			//Keyboard Messages
			case WM_KEYDOWN:
				pgui->ProcessMsgs_KeyDown(_this->W32KeyCodeToFxGen(wparam, lparam) );
				break;

			case WM_KEYUP:
				pgui->ProcessMsgs_KeyUp(_this->W32KeyCodeToFxGen(wparam, lparam) );
				break;

			case WM_CHAR:
				pgui->ProcessMsgs_Text(wparam);
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
NKey::Code NW32Application::W32KeyCodeToFxGen(WPARAM key, LPARAM flags)
{
		switch (key)
		{
			// Check the scancode to distinguish between left and right shift
		case VK_SHIFT :
			{
				static UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
				UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
				return scancode == lShift ? NKey::Code::LShift : NKey::Code::RShift;
			}

			// Check the "extended" flag to distinguish between left and right alt
		case VK_MENU : return (HIWORD(flags) & KF_EXTENDED) ? NKey::Code::RAlt : NKey::Code::LAlt;

			// Check the "extended" flag to distinguish between left and right control
		case VK_CONTROL : return (HIWORD(flags) & KF_EXTENDED) ? NKey::Code::RControl : NKey::Code::LControl;

			// Other keys are reported properly
		case VK_LWIN :       return NKey::Code::LSystem;
		case VK_RWIN :       return NKey::Code::RSystem;
		case VK_APPS :       return NKey::Code::Menu;
		case VK_OEM_1 :      return NKey::Code::SemiColon;
		case VK_OEM_2 :      return NKey::Code::Slash;
		case VK_OEM_PLUS :   return NKey::Code::Equal;
		case VK_OEM_MINUS :  return NKey::Code::Dash;
		case VK_OEM_4 :      return NKey::Code::LBracket;
		case VK_OEM_6 :      return NKey::Code::RBracket;
		case VK_OEM_COMMA :  return NKey::Code::Comma;
		case VK_OEM_PERIOD : return NKey::Code::Period;
		case VK_OEM_7 :      return NKey::Code::Quote;
		case VK_OEM_5 :      return NKey::Code::BackSlash;
		case VK_OEM_3 :      return NKey::Code::Tilde;
		case VK_ESCAPE :     return NKey::Code::Escape;
		case VK_SPACE :      return NKey::Code::Space;
		case VK_RETURN :     return NKey::Code::Return;
		case VK_BACK :       return NKey::Code::Back;
		case VK_TAB :        return NKey::Code::Tab;
		case VK_PRIOR :      return NKey::Code::PageUp;
		case VK_NEXT :       return NKey::Code::PageDown;
		case VK_END :        return NKey::Code::End;
		case VK_HOME :       return NKey::Code::Home;
		case VK_INSERT :     return NKey::Code::Insert;
		case VK_DELETE :     return NKey::Code::Delete;
		case VK_ADD :        return NKey::Code::Add;
		case VK_SUBTRACT :   return NKey::Code::Subtract;
		case VK_MULTIPLY :   return NKey::Code::Multiply;
		case VK_DIVIDE :     return NKey::Code::Divide;
		case VK_PAUSE :      return NKey::Code::Pause;
		case VK_F1 :         return NKey::Code::F1;
		case VK_F2 :         return NKey::Code::F2;
		case VK_F3 :         return NKey::Code::F3;
		case VK_F4 :         return NKey::Code::F4;
		case VK_F5 :         return NKey::Code::F5;
		case VK_F6 :         return NKey::Code::F6;
		case VK_F7 :         return NKey::Code::F7;
		case VK_F8 :         return NKey::Code::F8;
		case VK_F9 :         return NKey::Code::F9;
		case VK_F10 :        return NKey::Code::F10;
		case VK_F11 :        return NKey::Code::F11;
		case VK_F12 :        return NKey::Code::F12;
		case VK_F13 :        return NKey::Code::F13;
		case VK_F14 :        return NKey::Code::F14;
		case VK_F15 :        return NKey::Code::F15;
		case VK_LEFT :       return NKey::Code::Left;
		case VK_RIGHT :      return NKey::Code::Right;
		case VK_UP :         return NKey::Code::Up;
		case VK_DOWN :       return NKey::Code::Down;
		case VK_NUMPAD0 :    return NKey::Code::Numpad0;
		case VK_NUMPAD1 :    return NKey::Code::Numpad1;
		case VK_NUMPAD2 :    return NKey::Code::Numpad2;
		case VK_NUMPAD3 :    return NKey::Code::Numpad3;
		case VK_NUMPAD4 :    return NKey::Code::Numpad4;
		case VK_NUMPAD5 :    return NKey::Code::Numpad5;
		case VK_NUMPAD6 :    return NKey::Code::Numpad6;
		case VK_NUMPAD7 :    return NKey::Code::Numpad7;
		case VK_NUMPAD8 :    return NKey::Code::Numpad8;
		case VK_NUMPAD9 :    return NKey::Code::Numpad9;
		case 'A' :           return NKey::Code::A;
		case 'Z' :           return NKey::Code::Z;
		case 'E' :           return NKey::Code::E;
		case 'R' :           return NKey::Code::R;
		case 'T' :           return NKey::Code::T;
		case 'Y' :           return NKey::Code::Y;
		case 'U' :           return NKey::Code::U;
		case 'I' :           return NKey::Code::I;
		case 'O' :           return NKey::Code::O;
		case 'P' :           return NKey::Code::P;
		case 'Q' :           return NKey::Code::Q;
		case 'S' :           return NKey::Code::S;
		case 'D' :           return NKey::Code::D;
		case 'F' :           return NKey::Code::F;
		case 'G' :           return NKey::Code::G;
		case 'H' :           return NKey::Code::H;
		case 'J' :           return NKey::Code::J;
		case 'K' :           return NKey::Code::K;
		case 'L' :           return NKey::Code::L;
		case 'M' :           return NKey::Code::M;
		case 'W' :           return NKey::Code::W;
		case 'X' :           return NKey::Code::X;
		case 'C' :           return NKey::Code::C;
		case 'V' :           return NKey::Code::V;
		case 'B' :           return NKey::Code::B;
		case 'N' :           return NKey::Code::N;
		case '0' :           return NKey::Code::Num0;
		case '1' :           return NKey::Code::Num1;
		case '2' :           return NKey::Code::Num2;
		case '3' :           return NKey::Code::Num3;
		case '4' :           return NKey::Code::Num4;
		case '5' :           return NKey::Code::Num5;
		case '6' :           return NKey::Code::Num6;
		case '7' :           return NKey::Code::Num7;
		case '8' :           return NKey::Code::Num8;
		case '9' :           return NKey::Code::Num9;
		}

		return NKey::Code(0);

}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Win32 File Dialog Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
/*
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

 // if (parent!=null)		mOfn.hwndOwner = parent->m_W32HWnd;
	//else
	mOfn.hwndOwner = null;

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

*/