//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		SDLAppli.h
//! \brief	Win32 specific implementation
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		16-06-2014
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
#include "SDLApplication.h"

#include "SDL.h"

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
//					SDL Application class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NSDLApplication::NSDLApplication()
{
	m_pwnd = NULL;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NSDLApplication::~NSDLApplication()
{
}

//-----------------------------------------------------------------
//!	\brief	Initialize application
//!	\return	true if success
//-----------------------------------------------------------------
bool NSDLApplication::Init()
{
	udword w = 800;		udword h = 600;

	//Create Window
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Unable to Init SDL: %s", SDL_GetError());
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("Unable to Init hinting: %s", SDL_GetError());
	}

	if ((m_pwnd = SDL_CreateWindow(
		"My SDL",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)
		) == NULL) {
		printf("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}
	SDL_GLContext context = SDL_GL_CreateContext(m_pwnd);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Main loop
//-----------------------------------------------------------------
void NSDLApplication::Run()
{
	SDL_Event Event;
	NGUISubSystem* pgui = GetGUISubSystem();
	NPoint pt;

	bool bRunning = true;

	while (bRunning)
	{
		while (SDL_PollEvent(&Event) != 0)
		{
			if (Event.type == SDL_QUIT) bRunning = false;

			else if (Event.type == SDL_WINDOWEVENT)
			{
				switch (Event.window.event)
				{
				 case SDL_WINDOWEVENT_RESIZED:
					 pgui->ProcessMsgs_OnSize(Event.window.data1, Event.window.data2);
					 //SDL_Log("Window %d resized to %dx%d",
						// Event.window.windowID,
						// Event.window.data1,
						// Event.window.data2);
					 break;
				 default:
					 break;
				}
			}
			else if (Event.type == SDL_MOUSEBUTTONDOWN)
			{
				
				pt.x = Event.button.x; pt.y = Event.button.y;
				if (Event.button.clicks == 1)
				{
					if (Event.button.button == 1)
						pgui->ProcessMsgs_MouseButtonDown(pt, NM_LBUTTON);
					else if (Event.button.button == 2)
						pgui->ProcessMsgs_MouseButtonDown(pt, NM_MBUTTON);
					else if (Event.button.button == 3)
						pgui->ProcessMsgs_MouseButtonDown(pt, NM_RBUTTON);
				}
				else {
					pgui->ProcessMsgs_MouseButtonDblClick(pt, NM_LBUTTON);
				}

			}
			else if (Event.type == SDL_MOUSEBUTTONUP)
			{
				pt.x = Event.button.x; pt.y = Event.button.y;
				if (Event.button.button == 1)
					pgui->ProcessMsgs_MouseButtonUp(pt, NM_LBUTTON);
				else if (Event.button.button == 2)
					pgui->ProcessMsgs_MouseButtonUp(pt, NM_MBUTTON);
				else if (Event.button.button == 3)
					pgui->ProcessMsgs_MouseButtonUp(pt, NM_RBUTTON);

			}
			else if (Event.type == SDL_MOUSEMOTION)
			{
				pt.x = Event.motion.x; pt.y = Event.motion.y;
				pgui->ProcessMsgs_MouseMove(pt);
			}
			else if (Event.type == SDL_MOUSEWHEEL)
			{
				pt.x = 0; pt.y = 0;
				pgui->ProcessMsgs_MouseWheel(pt, Event.wheel.y);
			}
			else if (Event.type == SDL_KEYDOWN)
			{
				pgui->ProcessMsgs_KeyDown(SDLKeyCodeToFxGen(Event.key));
			}
			else if (Event.type == SDL_KEYUP)
			{
				pgui->ProcessMsgs_KeyUp(SDLKeyCodeToFxGen(Event.key));
			}

			//else if (Event.type == SDL_TEXTEDITING)
			//{
			//	pgui->ProcessMsgs_Text(Event.edit.)
			//}

		}

		//SDL_RenderClear(m_prenderer);

		Idle();
		Update();

		SDL_Delay(1); // Breath
	}


}

void NSDLApplication::Present()
{
	//SDL_RenderPresent(m_prenderer);
	SDL_GL_SwapWindow(m_pwnd);
}



//-----------------------------------------------------------------
//!	\brief	Exit application
//!	\return	true if success
//-----------------------------------------------------------------
bool NSDLApplication::Exit()
{
	return true;
}

//-----------------------------------------------------------------
//!	\brief	Ask exit
//-----------------------------------------------------------------
void NSDLApplication::AskExit()
{
	PostQuitMessage(0);
}

//-----------------------------------------------------------------
//!	\brief	Return mouse cursor position
//-----------------------------------------------------------------
void NSDLApplication::GetCursorPos(NPoint& _pos)
{
	//POINT pt;
	//::GetCursorPos(&pt);
	//::ScreenToClient(m_hW32Wnd, &pt);
	//_pos.x = (sdword)pt.x;		_pos.y = (sdword)pt.y;
}

//-----------------------------------------------------------------
//!	\brief	Set mouse cursor position
//-----------------------------------------------------------------
void NSDLApplication::SetCursorPos(const NPoint& _pos)
{
	//POINT pt;
	//pt.x = (sdword)_pos.x;		pt.y = (sdword)_pos.y;
	//::ClientToScreen(m_hW32Wnd, &pt);
	//::SetCursorPos(pt.x, pt.y);
}


//-----------------------------------------------------------------
//!	\brief	MessageBox
//-----------------------------------------------------------------
udword NSDLApplication::MessageBox(char* _pszText, udword _dwStyle)
{
	//udword dwW32Style = 0;
	//if (_dwStyle&NMB_OK)			dwW32Style |= MB_OK;
	//if (_dwStyle&NMB_YESNO)		dwW32Style |= MB_YESNO;
	//udword dwW32Ret = ::MessageBox(m_hW32Wnd, _pszText, "FxGen", dwW32Style);
	//if (dwW32Ret == IDOK)		return NIDOK;
	//if (dwW32Ret == IDYES)	return NIDYES;
	//if (dwW32Ret == IDNO)		return NIDNO;

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	W32KeyCodeToFxGen
//-----------------------------------------------------------------
NKey::Code NSDLApplication::SDLKeyCodeToFxGen(SDL_KeyboardEvent _sdlkey)
{
	//switch (key)
	//{
	//	// Check the scancode to distinguish between left and right shift
	//case VK_SHIFT:
	//{
	//	static UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
	//	UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
	//	return scancode == lShift ? NKey::Code::LShift : NKey::Code::RShift;
	//}

	//	// Check the "extended" flag to distinguish between left and right alt
	//case VK_MENU: return (HIWORD(flags) & KF_EXTENDED) ? NKey::Code::RAlt : NKey::Code::LAlt;

	//	// Check the "extended" flag to distinguish between left and right control
	//case VK_CONTROL: return (HIWORD(flags) & KF_EXTENDED) ? NKey::Code::RControl : NKey::Code::LControl;

	//	// Other keys are reported properly
	//case VK_LWIN:       return NKey::Code::LSystem;
	//case VK_RWIN:       return NKey::Code::RSystem;
	//case VK_APPS:       return NKey::Code::Menu;
	//case VK_OEM_1:      return NKey::Code::SemiColon;
	//case VK_OEM_2:      return NKey::Code::Slash;
	//case VK_OEM_PLUS:   return NKey::Code::Equal;
	//case VK_OEM_MINUS:  return NKey::Code::Dash;
	//case VK_OEM_4:      return NKey::Code::LBracket;
	//case VK_OEM_6:      return NKey::Code::RBracket;
	//case VK_OEM_COMMA:  return NKey::Code::Comma;
	//case VK_OEM_PERIOD: return NKey::Code::Period;
	//case VK_OEM_7:      return NKey::Code::Quote;
	//case VK_OEM_5:      return NKey::Code::BackSlash;
	//case VK_OEM_3:      return NKey::Code::Tilde;
	//case VK_ESCAPE:     return NKey::Code::Escape;
	//case VK_SPACE:      return NKey::Code::Space;
	//case VK_RETURN:     return NKey::Code::Return;
	//case VK_BACK:       return NKey::Code::Back;
	//case VK_TAB:        return NKey::Code::Tab;
	//case VK_PRIOR:      return NKey::Code::PageUp;
	//case VK_NEXT:       return NKey::Code::PageDown;
	//case VK_END:        return NKey::Code::End;
	//case VK_HOME:       return NKey::Code::Home;
	//case VK_INSERT:     return NKey::Code::Insert;
	//case VK_DELETE:     return NKey::Code::Delete;
	//case VK_ADD:        return NKey::Code::Add;
	//case VK_SUBTRACT:   return NKey::Code::Subtract;
	//case VK_MULTIPLY:   return NKey::Code::Multiply;
	//case VK_DIVIDE:     return NKey::Code::Divide;
	//case VK_PAUSE:      return NKey::Code::Pause;
	//case VK_F1:         return NKey::Code::F1;
	//case VK_F2:         return NKey::Code::F2;
	//case VK_F3:         return NKey::Code::F3;
	//case VK_F4:         return NKey::Code::F4;
	//case VK_F5:         return NKey::Code::F5;
	//case VK_F6:         return NKey::Code::F6;
	//case VK_F7:         return NKey::Code::F7;
	//case VK_F8:         return NKey::Code::F8;
	//case VK_F9:         return NKey::Code::F9;
	//case VK_F10:        return NKey::Code::F10;
	//case VK_F11:        return NKey::Code::F11;
	//case VK_F12:        return NKey::Code::F12;
	//case VK_F13:        return NKey::Code::F13;
	//case VK_F14:        return NKey::Code::F14;
	//case VK_F15:        return NKey::Code::F15;
	//case VK_LEFT:       return NKey::Code::Left;
	//case VK_RIGHT:      return NKey::Code::Right;
	//case VK_UP:         return NKey::Code::Up;
	//case VK_DOWN:       return NKey::Code::Down;
	//case VK_NUMPAD0:    return NKey::Code::Numpad0;
	//case VK_NUMPAD1:    return NKey::Code::Numpad1;
	//case VK_NUMPAD2:    return NKey::Code::Numpad2;
	//case VK_NUMPAD3:    return NKey::Code::Numpad3;
	//case VK_NUMPAD4:    return NKey::Code::Numpad4;
	//case VK_NUMPAD5:    return NKey::Code::Numpad5;
	//case VK_NUMPAD6:    return NKey::Code::Numpad6;
	//case VK_NUMPAD7:    return NKey::Code::Numpad7;
	//case VK_NUMPAD8:    return NKey::Code::Numpad8;
	//case VK_NUMPAD9:    return NKey::Code::Numpad9;
	//case 'A':           return NKey::Code::A;
	//case 'Z':           return NKey::Code::Z;
	//case 'E':           return NKey::Code::E;
	//case 'R':           return NKey::Code::R;
	//case 'T':           return NKey::Code::T;
	//case 'Y':           return NKey::Code::Y;
	//case 'U':           return NKey::Code::U;
	//case 'I':           return NKey::Code::I;
	//case 'O':           return NKey::Code::O;
	//case 'P':           return NKey::Code::P;
	//case 'Q':           return NKey::Code::Q;
	//case 'S':           return NKey::Code::S;
	//case 'D':           return NKey::Code::D;
	//case 'F':           return NKey::Code::F;
	//case 'G':           return NKey::Code::G;
	//case 'H':           return NKey::Code::H;
	//case 'J':           return NKey::Code::J;
	//case 'K':           return NKey::Code::K;
	//case 'L':           return NKey::Code::L;
	//case 'M':           return NKey::Code::M;
	//case 'W':           return NKey::Code::W;
	//case 'X':           return NKey::Code::X;
	//case 'C':           return NKey::Code::C;
	//case 'V':           return NKey::Code::V;
	//case 'B':           return NKey::Code::B;
	//case 'N':           return NKey::Code::N;
	//case '0':           return NKey::Code::Num0;
	//case '1':           return NKey::Code::Num1;
	//case '2':           return NKey::Code::Num2;
	//case '3':           return NKey::Code::Num3;
	//case '4':           return NKey::Code::Num4;
	//case '5':           return NKey::Code::Num5;
	//case '6':           return NKey::Code::Num6;
	//case '7':           return NKey::Code::Num7;
	//case '8':           return NKey::Code::Num8;
	//case '9':           return NKey::Code::Num9;
	//}

	return NKey::Code(0);

}
