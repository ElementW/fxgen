//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		GUI.cpp
//! \brief	Serializer for datas loading and saving
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
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
#include "SplitWnd.h"
#include "GUI.h"
#include "p3dmathspline.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

//-----------------------------------------------------------------
// Variables
//-----------------------------------------------------------------
NApplication*			g_pApp;

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
NApplication*			GetApp()		{ return g_pApp;}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NApplication class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NApplication::NApplication()
{
	m_pMainWnd	= NULL;
	g_pApp			= this;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NApplication::~NApplication()
{
	if (m_pMainWnd)	delete (m_pMainWnd);

	FreeLibrary(m_hRichEditMod);
}

//-----------------------------------------------------------------
//!	\brief	Initialize application
//!	\return	true if success
//-----------------------------------------------------------------
bool NApplication::Init()
{
	// Registration de la classe windows
	WNDCLASS				wc;
	wc.style				= CS_BYTEALIGNCLIENT | CS_DBLCLKS;
	wc.lpfnWndProc	= (WNDPROC) NWnd::StaticWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= GetModuleHandle (NULL);
	wc.hIcon				= NULL;
	wc.hCursor			= LoadCursor ((HINSTANCE)NULL, IDC_ARROW);
	wc.hbrBackground= (HBRUSH)(COLOR_APPWORKSPACE);
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= "FXGen_WNDCLASS";
	RegisterClass(&wc);

	// Load the RichEdit DLL to activate the RichEdit classes	###TOMOVE###
	m_hRichEditMod = LoadLibrary("RichEd32.Dll");

	//Make sure that the common Win32 control library read to rock
	INITCOMMONCONTROLSEX ccex;
	ccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ccex.dwICC	= ICC_USEREX_CLASSES | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&ccex);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Main loop
//-----------------------------------------------------------------
void NApplication::Run()
{
	if ( m_pMainWnd == NULL ) {
		//Haaa ! No main window ! Exit !!!
		PostQuitMessage(0);
	}

	//Polling messages from event queue until quit
	MSG	msg;
	while(1)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
		{
			while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)		return;
				::TranslateMessage(&msg);		//Translates virtual-key messages into character messages
				::DispatchMessage(&msg);		//Dispatches a message to window proc
			}
		} else {
		 Idle();
		}
	}

}

//-----------------------------------------------------------------
//!	\brief	Exit application
//!	\return	true if success
//-----------------------------------------------------------------
bool NApplication::Exit()
{
	return true;
}

//-----------------------------------------------------------------
//!	\brief	Ask exit
//-----------------------------------------------------------------
void NApplication::AskExit()
{
	PostQuitMessage(0);
}

//-----------------------------------------------------------------
//!	\brief	Display a message box
//!	\param	_pszText	message to display
//!	\param	_dwType		type of message box
//!	\return	Clicked button code
//-----------------------------------------------------------------
udword NApplication::MessageBox(char* _pszText, uword _dwType)
{
	return (udword)::MessageBox(m_pMainWnd->m_W32HWnd, _pszText, "FxGen", _dwType);
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NGraphics class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NGraphics::NGraphics(NWnd* pwnd)
{
	m_rc			= pwnd->GetClientRect();
	m_hWnd		= pwnd->m_W32HWnd;
	m_hDC			= ::GetDC(m_hWnd);
	m_hPen		= null;
	m_hBrush	= null;

	// Create a Memory DC for double buffeur
	m_hMemDC = ::CreateCompatibleDC(m_hDC);
	m_hBmp = CreateCompatibleBitmap(m_hDC, m_rc.Width(), m_rc.Height() );
  SelectObject(m_hMemDC, m_hBmp);
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NGraphics::~NGraphics()
{
	// Copy MemDC to Screen DC
	::BitBlt(m_hDC, 0, 0, m_rc.Width(), m_rc.Height(), m_hMemDC, 0, 0, SRCCOPY);

	// Destroy MemDC
	::DeleteObject(m_hBmp);
	::DeleteDC(m_hMemDC);

	// Delete DC
	if (m_hDC)		::ReleaseDC(m_hWnd, m_hDC);

	// Delete Objects
	if (m_hPen)		::DeletePen(m_hPen);
	if (m_hBrush)	::DeleteBrush(m_hBrush);

}

void NGraphics::FillSolidRect(NRect& rc, COLORREF clr)
{
	::SetBkColor(m_hMemDC, clr);
	::ExtTextOut(m_hMemDC, 0, 0, ETO_OPAQUE, (const RECT*)&rc, NULL, 0, NULL);
}

void NGraphics::Draw3dRect(NRect& rc, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	RECT r;

	::SetBkColor(m_hMemDC, clrTopLeft);

	r.left = rc.left; r.top = rc.top; r.right = rc.right-1;  r.bottom = r.top+1;
	::ExtTextOut(m_hMemDC, 0, 0, ETO_OPAQUE, (const RECT*)&r, NULL, 0, NULL);
	r.left = rc.left; r.top = rc.top; r.right = r.left+1;  r.bottom = rc.bottom - 1;
	::ExtTextOut(m_hMemDC, 0, 0, ETO_OPAQUE, (const RECT*)&r, NULL, 0, NULL);

	::SetBkColor(m_hMemDC, clrBottomRight);

	r.left = rc.right; r.top = rc.top; r.right = rc.right-1;  r.bottom = rc.bottom;
	::ExtTextOut(m_hMemDC, 0, 0, ETO_OPAQUE, (const RECT*)&r, NULL, 0, NULL);
	r.left = rc.left; r.top = rc.bottom; r.right = rc.right;  r.bottom = rc.bottom-1;
	::ExtTextOut(m_hMemDC, 0, 0, ETO_OPAQUE, (const RECT*)&r, NULL, 0, NULL);
}

void NGraphics::RoundRect(NRect& rc, sdword ellipseW, sdword ellipseH)
{
	::RoundRect(m_hMemDC, rc.left, rc.top, rc.right, rc.bottom, ellipseW, ellipseH);
}

void NGraphics::GradientVRect(NRect& rc, COLORREF clrStart, COLORREF clrEnd)
{
	int r = GetRValue(clrEnd)-GetRValue(clrStart);
	int g = GetGValue(clrEnd)-GetGValue(clrStart);
	int b = GetBValue(clrEnd)-GetBValue(clrStart);

	TRIVERTEX        vert[2] ;
	GRADIENT_RECT    gRect;
	vert [0] .x      = rc.left;
	vert [0] .y      = rc.top;
	vert [0] .Red    = GetRValue(clrStart)<<8;
	vert [0] .Green  = GetGValue(clrStart)<<8;
	vert [0] .Blue   = GetBValue(clrStart)<<8;
	vert [0] .Alpha  = 0x0000;

	vert [1] .x      = rc.right;
	vert [1] .y      = rc.bottom;
	vert [1] .Red    = GetRValue(clrEnd)<<8;
	vert [1] .Green  = GetGValue(clrEnd)<<8;
	vert [1] .Blue   = GetBValue(clrEnd)<<8;
	vert [1] .Alpha  = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;
	::GradientFill(m_hMemDC,vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
}

void NGraphics::GradientHRect(NRect& rc, COLORREF clrStart, COLORREF clrEnd)
{
	int r = GetRValue(clrEnd)-GetRValue(clrStart);
	int g = GetGValue(clrEnd)-GetGValue(clrStart);
	int b = GetBValue(clrEnd)-GetBValue(clrStart);

	TRIVERTEX        vert[2] ;
	GRADIENT_RECT    gRect;
	vert [0] .x      = rc.left;
	vert [0] .y      = rc.top;
	vert [0] .Red    = GetRValue(clrStart)<<8;
	vert [0] .Green  = GetGValue(clrStart)<<8;
	vert [0] .Blue   = GetBValue(clrStart)<<8;
	vert [0] .Alpha  = 0x0000;

	vert [1] .x      = rc.right;
	vert [1] .y      = rc.bottom;
	vert [1] .Red    = GetRValue(clrEnd)<<8;
	vert [1] .Green  = GetGValue(clrEnd)<<8;
	vert [1] .Blue   = GetBValue(clrEnd)<<8;
	vert [1] .Alpha  = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;
	::GradientFill(m_hMemDC,vert,2,&gRect,1,GRADIENT_FILL_RECT_H);
}

void NGraphics::RoundRect(int _roundboxtype, float _minx, float _miny, float _maxx, float _maxy, float _rad)
{
	float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
	                  {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
	int a;
	int i=0;
	NPoint pts[256];

	/* mult */
	for(a=0; a<7; a++) {
		vec[a][0]*= _rad; vec[a][1]*= _rad;
	}

	/* start with corner right-bottom */
	if(_roundboxtype & 4)
	{
		pts[i].x = _maxx-_rad;
		pts[i].y = _miny;
		i++;

		for(a=0; a<7; a++)
		{
			pts[i].x = _maxx-_rad+vec[a][0];
			pts[i].y = _miny+vec[a][1];
			i++;
		}
		pts[i].x = _maxx;
		pts[i].y = _miny+_rad;
		i++;
	}	else {
		pts[i].x = _maxx;
		pts[i].y = _miny;
		i++;
	}

	/* corner right-top */
	if(_roundboxtype & 2)
	{
		pts[i].x = _maxx;
		pts[i].y = _maxy-_rad;
		i++;

		for(a=0; a<7; a++)
		{
			pts[i].x = _maxx-vec[a][1];
			pts[i].y = _maxy-_rad+vec[a][0];
			i++;
		}
		pts[i].x = _maxx-_rad;
		pts[i].y = _maxy;
		i++;
	}	else {
		pts[i].x = _maxx;
		pts[i].y = _maxy;
		i++;
	}

	/* corner left-top */
	if(_roundboxtype & 1)
	{
		pts[i].x = _minx+_rad;
		pts[i].y = _maxy;
		i++;

		for(a=0; a<7; a++)
		{
			pts[i].x = _minx+_rad-vec[a][0];
			pts[i].y = _maxy-vec[a][1];
			i++;
		}
		pts[i].x = _minx;
		pts[i].y = _maxy-_rad;
		i++;
	}	else {
		pts[i].x = _minx;
		pts[i].y = _maxy;
		i++;
	}

	/* corner left-bottom */
	if(_roundboxtype & 8)
	{
		pts[i].x = _minx;
		pts[i].y = _miny+_rad;
		i++;

		for(a=0; a<7; a++)
		{
			pts[i].x = _minx+vec[a][1];
			pts[i].y = _miny+_rad-vec[a][0];
			i++;
		}
		pts[i].x = _minx+_rad;
		pts[i].y = _miny;
		i++;
	}	else {
		pts[i].x = _minx;
		pts[i].y = _miny;
		i++;
	}

	::Polygon(m_hMemDC, (POINT*)pts, (int)i);

}



void NGraphics::SetPen(sdword width, COLORREF clr)
{
	if (m_hPen)		::DeleteObject (m_hPen);
	m_hPen = CreatePen(0,width,clr);
	::SelectObject(m_hMemDC, m_hPen);
}

void NGraphics::SetBrush(COLORREF clr)
{
	if (m_hBrush)		::DeleteObject (m_hBrush);
	m_hBrush = ::CreateSolidBrush(clr);
	::SelectObject(m_hMemDC, m_hBrush);
}

void NGraphics::MoveTo(sdword x, sdword y)
{
	::MoveToEx(m_hMemDC, x, y, null);
}

void NGraphics::LineTo(sdword x, sdword y)
{
	::LineTo(m_hMemDC, x, y);
}

void NGraphics::SetFont(HFONT hfont)
{
	::SelectObject(m_hMemDC, hfont);
}

void NGraphics::SetBkMode(udword dwMode)
{
	::SetBkMode(m_hMemDC, dwMode);
}

void NGraphics::SetTextColor(COLORREF clr)
{
	::SetTextColor(m_hMemDC, clr);
}
void NGraphics::SetTextBkColor(COLORREF clr)
{
	::SetBkColor(m_hMemDC, clr);
}


sdword NGraphics::DrawText(const char* szString, NRect& rc, udword nFormat)
{
	return (sdword)::DrawText(m_hMemDC, szString, strlen(szString), (RECT*)&rc, nFormat);
}

void NGraphics::Polygon(NPoint* ptsArray, udword dwCount)
{
	::Polygon(m_hMemDC, (POINT*)ptsArray, (int)dwCount);
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NWnd class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NWnd::NWnd()
{
	m_pParentWnd	= null;
	m_W32HWnd			= null;
	m_OldWndProc	= null;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NWnd::~NWnd()
{
	//Destroy window
	if (m_W32HWnd)	DestroyWindow(m_W32HWnd);
}

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NWnd::Create(NWNDCREATE &c)
{
	//Save Creation struct datas
	m_pParentWnd	= c.Parent;
	cstrWindowName = c.Name;

	//Get Parent if any
	HWND W32HWndParent = null;
	if (c.Parent)		W32HWndParent = c.Parent->m_W32HWnd;

	//Create Window
	m_W32HWnd = CreateWindowEx(c.W32StyleEx, c.W32ClassName, c.Name,
		c.W32Style | WS_VISIBLE,
    c.Rect.left, c.Rect.top, c.Rect.Width(), c.Rect.Height(),
		W32HWndParent, NULL, GetModuleHandle(NULL), NULL );

	//Set user data in param for retrieve this class pointer
	::SetWindowLong (m_W32HWnd, GWL_USERDATA, (LONG)this);

	//If it's not a FXGen_WNDCLASS then we subclass its WndProc (useful for all the controls contained in a dialog box for example, in order to handle all the keyboard messages to control the focus...)
	if (strcmp((LPCTSTR) c.W32ClassName, (LPCTSTR) "FXGen_WNDCLASS"))		m_OldWndProc = (WNDPROC) ::SetWindowLong(m_W32HWnd, GWL_WNDPROC, (LONG) StaticWndProc);

	//Set Default Font
	::SendMessage(m_W32HWnd, WM_SETFONT, (WPARAM) ::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));

	return true;
}

//-----------------------------------------------------------------
// Name:	GetClientRect()
// Desc:	...
//-----------------------------------------------------------------
NRect NWnd::GetClientRect()
{
	RECT w32rect;
	::GetClientRect(m_W32HWnd, &w32rect);
	NRect rect(w32rect.left, w32rect.top, w32rect.right, w32rect.bottom);
	return rect;
}

//-----------------------------------------------------------------
// Name:	GetWindowRect()
// Desc:	...
//-----------------------------------------------------------------
NRect NWnd::GetWindowRect()
{
	RECT w32rect;
	::GetWindowRect(m_W32HWnd, &w32rect);
	NRect rect(w32rect.left, w32rect.top, w32rect.right, w32rect.bottom);
	return rect;
}


//-----------------------------------------------------------------
// Name:	SetWindowRect()
// Desc:	...
//-----------------------------------------------------------------
void NWnd::SetWindowRect(const NRect& r)
{
	::MoveWindow(m_W32HWnd, r.left, r.top, r.Width(), r.Height(), true );
}

void NWnd::ScreenToClient(NPoint& _pt)
{
	POINT pt;
	pt.x=_pt.x; pt.y=_pt.y;
	::ScreenToClient(m_W32HWnd, &pt);
	_pt.x = pt.x; _pt.y=pt.y;
}

void NWnd::ClientToScreen(NPoint& _pt)
{
	POINT pt;
	pt.x=_pt.x; pt.y=_pt.y;
	::ClientToScreen(m_W32HWnd, &pt);
	_pt.x = pt.x; _pt.y=pt.y;
}



void NWnd::SetFocus()
{
	::SetFocus(m_W32HWnd);
}

void NWnd::SetCapture()
{
	::SetCapture(m_W32HWnd);
}

void NWnd::ReleaseCapture()
{
	::ReleaseCapture();
}

void NWnd::SetWindowText(char* _pszText)
{
	::SetWindowText(m_W32HWnd, _pszText);
}


//-----------------------------------------------------------------
// Name:	Windows Proc()
// Desc:	...
//-----------------------------------------------------------------
LRESULT CALLBACK NWnd::StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//Get NWnd Class pointer
	NWnd* nwnd = (NWnd*)::GetWindowLong(hwnd, GWL_USERDATA);
	if (nwnd==null)
		return (::DefWindowProc(hwnd, msg, wparam, lparam));

	//Check W32 Messages
	switch (msg) {

		//Keyboard message
		case WM_KEYDOWN:
			//if (wparam==VK_ESCAPE)	{ PostMessage(hwnd, WM_CLOSE, 0, 0);	 break; }
			break;

		//Close Message
		case WM_CLOSE:
			PostMessage(hwnd, WM_QUIT, 0, 0);
			break;
	}

	return nwnd->WndProc(msg, wparam, lparam);
}


LRESULT NWnd::WndProc( UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		//Dispath Messages
		case WM_PAINT:				OnPaint();					break;
		case WM_SIZE:					OnSize();					break;
		case WM_MOUSEMOVE:		OnMouseMove( wparam, NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)) );				return 0;
		case WM_LBUTTONDOWN:	OnLeftButtonDown(wparam, NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));			break;
		case WM_RBUTTONDOWN:	OnRightButtonDown(wparam, NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));		break;
		case WM_MBUTTONDOWN:	OnMButtonDown(wparam, NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));			break;
		case WM_LBUTTONUP:		OnLeftButtonUp(wparam, NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));			break;
		case WM_RBUTTONUP:		OnRightButtonUp(wparam, NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));			break;
		case WM_MBUTTONUP:		OnMButtonUp(wparam, NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));			break;
		case WM_LBUTTONDBLCLK:OnLeftButtonDblClk(wparam, NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));			break;
		case WM_MOUSEWHEEL:		OnMouseWheel(wparam, (sword)HIWORD(wparam), NPoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)));			break;
		case WM_KEYDOWN:			OnKeyDown(wparam);	break;
		case WM_KEYUP:				OnKeyUp(wparam);		break;
		case WM_KILLFOCUS:		OnKillFocus((NWnd*)::GetWindowLong((HWND)wparam, GWL_USERDATA));			break;
		case WM_TIMER:				OnTimer(wparam);		break;

		case WM_COMMAND: {
			HWND HWNDCtrl	= (HWND)lparam;	  // handle of control

			//Intercept control message
			if (HWNDCtrl) {
				for (sdword i=mControlsList.Count()-1; i>=0; i--)
					if (mControlsList[i]->m_W32HWnd == HWNDCtrl &&
						mControlsList[i]->CatchControlCommand(HIWORD(wparam), LOWORD(wparam)))	 return 1;
			}

			//Menu
			OnCommand(wparam);

			break;
		}

		case WM_NOTIFY: {
			HWND HWNDCtrl	= ((NMHDR*)lparam)->hwndFrom;	  // handle of control

			//Intercept control notification
			if (HWNDCtrl)
			{
				for (sdword i=mControlsList.Count()-1; i>=0; i--)
					if (mControlsList[i]->m_W32HWnd == HWNDCtrl &&
						mControlsList[i]->CatchControlNotify( (udword) ((NMHDR*)lparam)->code) ) return 1;
			}
			break;
		}

	}

	if (m_OldWndProc)	return CallWindowProc(m_OldWndProc, m_W32HWnd, msg, wparam, lparam);
	else							return DefWindowProc(m_W32HWnd, msg, wparam, lparam);
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Frame Wnd class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NFrmWnd::NFrmWnd() : NWnd()
{
	mWorkspace	= null;
	mMenuBar	= null;
	mStatusBar	= null;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NFrmWnd::~NFrmWnd()
{
	if (mWorkspace)		delete mWorkspace;
	if (mMenuBar)		delete mMenuBar;
	if (mStatusBar)		delete mStatusBar;
}

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NFrmWnd::Create(char* name, const NRect& rect)
{
	NWNDCREATE			wc;
	wc.Id						= 1;
	wc.Name					= name;
	wc.Parent				= null;
	wc.Rect					= rect;
	wc.W32ClassName	= "FXGen_WNDCLASS";
	wc.W32Style			= WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN;
	wc.W32StyleEx		= 0;
	NWnd::Create(wc);

	//Create Menu bar
	mMenuBar = new NMenuBar();
	mMenuBar->Create(this);

	//Create workspace
	NRect r = GetClientRect();
	NSplitWnd* splitwnd = new NSplitWnd;
	splitwnd->Create("wks", r, this);

	mWorkspace = splitwnd;

	//Create Status bar
	mStatusBar = new NStatusBar;
	mStatusBar->Create(this);

	return true;
}

//-----------------------------------------------------------------
// Name:	OnSize()
// Desc:	...
//-----------------------------------------------------------------
void NFrmWnd::OnSize()
{
	NRect r = GetClientRect();

	//Resize status bar
	if (mStatusBar) {
		udword h = mStatusBar->GetHeight();
		mStatusBar->SetWindowRect(NRect(r.left, r.bottom-h, r.right, r.bottom ));
		r.bottom-=h;
	}

	//Resize workspace
	if (mWorkspace)		mWorkspace->SetWindowRect(r);

}

//-----------------------------------------------------------------
// Name:	ReplaceWorkspace()
// Desc:	...
//-----------------------------------------------------------------
void NFrmWnd::ReplaceWorkspace(NWnd* wnd)
{
	if (mWorkspace)		delete mWorkspace;
	mWorkspace = wnd;
	OnSize();
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Control base class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NWControl::Create(NWNDCREATE &c)
{
	//Add This control at parent's control list
	NWnd* parent = c.Parent;
	if (parent)		((NWControl*)parent)->mControlsList.AddItem((NWControl*)this);

	return NWnd::Create(c);
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Edit Control Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NEditCtrl::Create(char* name, const NRect& rect, NWnd* parent, bool bMultiLine)
{
	udword dwStyle=0;
	if (bMultiLine)		dwStyle=WS_VSCROLL|ES_AUTOVSCROLL|ES_MULTILINE;

	NWNDCREATE			wc;
	wc.Id						= 1;
	wc.Name					= name;
	wc.Parent				= parent;
	wc.Rect					= rect;
	wc.W32ClassName	= RICHEDIT_CLASS;
	wc.W32Style			= WS_VISIBLE | WS_CHILD | dwStyle;
	wc.W32StyleEx		= 0/*WS_EX_CLIENTEDGE*/;
	NWControl::Create(wc);

	//Set the event mask for a rich edit control
	::SendMessage(m_W32HWnd,	EM_SETEVENTMASK, 0, ENM_CHANGE/*|ENM_KEYEVENTS*/);

	//Set the background color
	::SendMessage(m_W32HWnd,	EM_SETBKGNDCOLOR, 0, RGB(255,255,255));

	return true;
}

//-----------------------------------------------------------------
// Name:	CatchControlCommand()
// Desc:	...
//-----------------------------------------------------------------
udword NEditCtrl::CatchControlCommand(uword notifycode, uword id)
{
//	if( notifycode==EN_CHANGE ) {	ExecNotifMsg(NEN_CHANGE, 0);	return 1; }
	return 0;
}


//-----------------------------------------------------------------
// Name:	CatchControlNotify()
// Desc:	...
//-----------------------------------------------------------------
udword NEditCtrl::CatchControlNotify(udword code)
{
//	if( code==EN_MSGFILTER )  {	ExecNotifMsg(NEN_CHANGE, 0);	return 0; }
	return 0;
}


//-----------------------------------------------------------------
// Name:	SetText()
// Desc:	...
//-----------------------------------------------------------------
void NEditCtrl::SetText(char* text)
{
	::SetWindowText(m_W32HWnd, text);
}


//-----------------------------------------------------------------
// Name:	GetText()
// Desc:	...
//-----------------------------------------------------------------
NString NEditCtrl::GetText()
{
	GETTEXTLENGTHEX gtl;
	gtl.flags		= GTL_DEFAULT;
	gtl.codepage	= CP_ACP;
	udword length = ::SendMessage(m_W32HWnd, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, null);

	if (length) {
		NString str; str.SetLength(length + 1);
		GETTEXTEX			gt;
		gt.cb				= length+1;
		gt.flags			= GT_DEFAULT;
		gt.codepage			= CP_ACP;
		gt.lpDefaultChar	= null;
#ifdef __GNUC__
		gt.lpUsedDefaultChar= null;
#else
		gt.lpUsedDefChar	= null;
#endif
		udword len = ::SendMessage(m_W32HWnd, EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)str.Buffer());
		assert(len==length);
		return str;
	} else return NString("");

}


//-----------------------------------------------------------------
// Name:	GetCaretPos()
// Desc:	...
//-----------------------------------------------------------------
void NEditCtrl::GetCaretPos(udword& cx, udword& cy)
{
	udword start, end;
	::SendMessage(m_W32HWnd, EM_GETSEL, (WPARAM) &start, (LPARAM) &end);

	cy	= ::SendMessage(m_W32HWnd, EM_LINEFROMCHAR, (WPARAM) end, (LPARAM) 0);
	cx	= end - ::SendMessage(m_W32HWnd, EM_LINEINDEX, (WPARAM) cy, (LPARAM) 0);
}

//-----------------------------------------------------------------
// Name:	GetLineCount()
// Desc:	...
//-----------------------------------------------------------------
udword NEditCtrl::GetLineCount()
{
	return SendMessage(m_W32HWnd, EM_GETLINECOUNT, 0, 0);
}

//-----------------------------------------------------------------
// Name:	GetLine()
// Desc:	...
//-----------------------------------------------------------------
NString NEditCtrl::GetLine(udword line)
{
	udword lineidx	  =	SendMessage(m_W32HWnd, EM_LINEINDEX, line, 0);
	udword linelength = SendMessage(m_W32HWnd, EM_LINELENGTH, lineidx, 0);

	if (linelength) {
		NString str; str.SetLength(linelength + 1);
		udword len = SendMessage(m_W32HWnd, EM_GETLINE, line, (LPARAM)str.Buffer() );
		assert(len==linelength+1);
		return str;
	} else return NString("");
}


//-----------------------------------------------------------------
// Name:	GetLineIdx()
// Desc:	...
//-----------------------------------------------------------------
udword NEditCtrl::GetLineIdx(udword line)
{
	return SendMessage(m_W32HWnd, EM_LINEINDEX, line, 0);
}



//-----------------------------------------------------------------
// Name:	SetSel()
// Desc:	...
//-----------------------------------------------------------------
void NEditCtrl::SetSel(udword startchar, udword eNGraphicshar)
{
	CHARRANGE	cr;
	cr.cpMin	= startchar;
	cr.cpMax	= eNGraphicshar;
	SendMessage(m_W32HWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
}

//-----------------------------------------------------------------
// Name:	GetSel()
// Desc:	...
//-----------------------------------------------------------------
void NEditCtrl::GetSel(udword& startchar, udword& eNGraphicshar)
{
	SendMessage(m_W32HWnd, EM_GETSEL, (WPARAM)&startchar, (LPARAM)&eNGraphicshar);
}


//-----------------------------------------------------------------
// Name:	SelectLine()
// Desc:	...
//-----------------------------------------------------------------
void NEditCtrl::SelectLine(udword line)
{
	udword charidx		= SendMessage(m_W32HWnd, EM_LINEINDEX,  line, 0);
	udword linelength	= SendMessage(m_W32HWnd, EM_LINELENGTH, charidx, 0);
	SetSel(charidx, charidx+linelength);
}

//-----------------------------------------------------------------
// Name:	ReplaceSel()
// Desc:	...
//-----------------------------------------------------------------
void NEditCtrl::ReplaceSel(char* str)
{
	SendMessage(m_W32HWnd, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)str);
}

//-----------------------------------------------------------------
// Name:	SetSelCharFormat()
// Desc:	...
//-----------------------------------------------------------------
void NEditCtrl::SetSelCharFormat(NColor& col, bool bold, bool italic, bool underline)
{
	CHARFORMAT cf;
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
	cf.crTextColor	= col.GetARGB();
	cf.dwEffects	= (bold?CFE_BOLD:0) | (italic?CFE_ITALIC:0) | (underline?CFE_UNDERLINE:0);
	SendMessage(m_W32HWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}


//-----------------------------------------------------------------
// Name:	HideSelection()
// Desc:	...
//-----------------------------------------------------------------
void NEditCtrl::HideSelection(bool hide)
{
	SendMessage(m_W32HWnd, EM_HIDESELECTION, hide, 0);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Tab Control Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NTabCtrl::Create(char* name, NRect& rect, NWnd* parent)
{
	NWNDCREATE		wc;
	wc.Id			= 1;
	wc.Name			= name;
	wc.Parent		= parent;
	wc.Rect			= rect;
	wc.W32ClassName	= WC_TABCONTROL;
	wc.W32Style		= WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|TCS_FOCUSNEVER/*|TCS_BOTTOM*/;
	wc.W32StyleEx	= 0;
	NWControl::Create(wc);

	//Set tab control style
	TabCtrl_SetItemSize(m_W32HWnd, 8,13);	//for x add TCS_FIXEDWIDTH at w32style
	TabCtrl_SetPadding(m_W32HWnd,  0,2);

	return true;
}

//-----------------------------------------------------------------
// Name:	InsertItem()
// Desc:	...
//-----------------------------------------------------------------
void NTabCtrl::InsertItem(udword pos, char* name, NWnd* wnd)
{
	assert(wnd);

	//Hide selected wnd if any
	udword oldidx = TabCtrl_GetCurSel(m_W32HWnd);
	if (oldidx!=-1)
	{
		TCITEM ti;
		ti.mask	= TCIF_PARAM;
		TabCtrl_GetItem(m_W32HWnd, oldidx, &ti);
		NWnd* selwnd = (NWnd*)ti.lParam;
		ShowWindow(selwnd->m_W32HWnd, SW_HIDE);
		::RedrawWindow(selwnd->m_W32HWnd, null, null, RDW_INVALIDATE);
	}

	//Insert new item
	TCITEM				ti;
	ti.mask				= TCIF_TEXT | TCIF_PARAM;
	ti.pszText		= name;
	ti.cchTextMax	= strlen(name);
	ti.lParam			= (udword)wnd;
	udword newidx	= TabCtrl_InsertItem(m_W32HWnd, pos, &ti);
	assert(newidx!=-1);

	//Select it
	SendMessage(m_W32HWnd, TCM_SETCURSEL, newidx, 0);

	//update new item's wnd
	ShowWindow(wnd->m_W32HWnd, SW_SHOW);
	OnSize();
	RedrawWindow(wnd->m_W32HWnd, null, null, RDW_INVALIDATE);
}

//-----------------------------------------------------------------
// Name:	SelectItem()
// Desc:	...
//-----------------------------------------------------------------
void NTabCtrl::SelectItem(udword idx)
{
	//Hide selected wnd if any
	udword oldidx = TabCtrl_GetCurSel(m_W32HWnd);
	if (oldidx!=-1)
	{
		TCITEM ti;
		ti.mask	= TCIF_PARAM;
		TabCtrl_GetItem(m_W32HWnd, oldidx, &ti);
		NWnd* selwnd = (NWnd*)ti.lParam;
		ShowWindow(selwnd->m_W32HWnd, SW_HIDE);
		::RedrawWindow(selwnd->m_W32HWnd, null, null, RDW_INVALIDATE);
	}

	//Select it
	SendMessage(m_W32HWnd, TCM_SETCURSEL, idx, 0);

	//Show selected item
	TCITEM ti;
	ti.mask	= TCIF_PARAM;
	TabCtrl_GetItem(m_W32HWnd, idx, &ti);
	NWnd* selwnd = (NWnd*)ti.lParam;
	ShowWindow(selwnd->m_W32HWnd, SW_SHOW);
	OnSize();
	RedrawWindow(m_W32HWnd, null, null, RDW_INVALIDATE);
}


//-----------------------------------------------------------------
// Name:	GetSelectedItem()
// Desc:	...
//-----------------------------------------------------------------
udword NTabCtrl::GetSelectedItem()
{
	return TabCtrl_GetCurSel(m_W32HWnd);
}



//-----------------------------------------------------------------
// Name:	CatchControlNotify()
// Desc:	...
//-----------------------------------------------------------------
udword NTabCtrl::CatchControlNotify(udword code)
{
	//Hide old wnd
	if (code==TCN_SELCHANGING)
	{
		udword idx = TabCtrl_GetCurSel(m_W32HWnd);
		TCITEM ti;
		ti.mask	= TCIF_PARAM;
		TabCtrl_GetItem(m_W32HWnd, idx, &ti);
		NWnd* selwnd = (NWnd*)ti.lParam;
		ShowWindow(selwnd->m_W32HWnd, SW_HIDE);

	//Show new wnd
	} else if (code==TCN_SELCHANGE) {

		udword idx = TabCtrl_GetCurSel(m_W32HWnd);
		TCITEM ti;
		ti.mask	= TCIF_PARAM;
		TabCtrl_GetItem(m_W32HWnd, idx, &ti);
		NWnd* selwnd = (NWnd*)ti.lParam;
		ShowWindow(selwnd->m_W32HWnd, SW_SHOW);
		OnSize();

		OnSelChange();
	}

	return 0;
}


//-----------------------------------------------------------------
// Name:	OnSize()
// Desc:	...
//-----------------------------------------------------------------
void NTabCtrl::OnSize()
{
	//Get New Client Rect
	NRect r = GetClientRect();

	//Compute the right tab size for a template of size <size>
	RECT			TabRect;
	TabRect.left	= 0;
	TabRect.right	= r.right;
	TabRect.top		= 0;
	TabRect.bottom	= r.bottom;
	::SendMessage(m_W32HWnd, TCM_ADJUSTRECT, (WPARAM) FALSE, (LPARAM) &TabRect);

	//Resize selected wnd
	udword idx = TabCtrl_GetCurSel(m_W32HWnd);
	if (idx!=-1)
	{
		TCITEM ti;
		ti.mask	= TCIF_PARAM;
		TabCtrl_GetItem(m_W32HWnd, idx, &ti);
		NWnd* selwnd = (NWnd*)ti.lParam;
		selwnd->SetWindowRect(NRect(TabRect.left, TabRect.top, TabRect.right, TabRect.bottom));
	}
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Menu bar Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NMenuBar::NMenuBar()
{
	mW32Menu	= null;
	mOwner		= null;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NMenuBar::~NMenuBar()
{
}

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NMenuBar::Create(NWnd* owner)
{
	assert(owner);
	mW32Menu	= CreateMenu();
	mOwner		= owner;
	SetMenu(mOwner->m_W32HWnd, mW32Menu);
	DrawMenuBar(mOwner->m_W32HWnd);

	return true;
}

//-----------------------------------------------------------------
// Name:	InsertItem()
// Desc:	...
//-----------------------------------------------------------------
void NMenuBar::InsertItem(udword parent, udword pos, char* name, udword id)
{
	if (parent==null)		parent=(udword)mW32Menu;

	//Insert the item into the Win32 menu
	MENUITEMINFO	mii;
	mii.cbSize		= sizeof(MENUITEMINFO);
	mii.fMask			= MIIM_TYPE | MIIM_ID | MIIM_DATA;
	mii.fType			= MFT_STRING;
	mii.dwTypeData= name;
	mii.dwItemData= 0;
	mii.wID				= id;

	//Win32 menu item insertion
	InsertMenuItem((HMENU)parent, pos, true, &mii);
	DrawMenuBar(mOwner->m_W32HWnd);
}


//-----------------------------------------------------------------
// Name:	InsertPopupItem()
// Desc:	...
//-----------------------------------------------------------------
udword NMenuBar::InsertPopupItem(udword parent, udword pos, char* name)
{
	if (parent==null)		parent=(udword)mW32Menu;

	MENUITEMINFO	mii;
	mii.cbSize		= sizeof(MENUITEMINFO);
	mii.fMask			= MIIM_TYPE | MIIM_ID | MIIM_SUBMENU | MIIM_DATA;
	mii.fType			= MFT_STRING;
	mii.dwTypeData= name;
	mii.dwItemData= 0;
	mii.wID				= 1;
	mii.hSubMenu	= CreateMenu();

	InsertMenuItem((HMENU)parent, pos, true, &mii);
	DrawMenuBar(mOwner->m_W32HWnd);

	return (udword)mii.hSubMenu;
}

//-----------------------------------------------------------------
// Name:	InsertFromResource()
// Desc:	...
//-----------------------------------------------------------------
void NMenuBar::InsertFromResource(char* name)
{
	HMENU hm = LoadMenu(GetModuleHandle(null), name);
	if (hm) {
		_InsertFromResource(hm, name, mW32Menu);
		DrawMenuBar(mOwner->m_W32HWnd);
	}
}

void NMenuBar::_InsertFromResource(HMENU submenu, char* name, HMENU parent)
{
	char Buffer[1024];
	sdword c = GetMenuItemCount(submenu);
	for (sdword i=0; i<c; i++){

		MENUITEMINFO mii;
		ZeroMemory( &mii, sizeof(MENUITEMINFO) );
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask      = MIIM_TYPE | MIIM_SUBMENU;
		mii.cch        = 1024;
		mii.dwTypeData = Buffer;
		GetMenuItemInfo(submenu, i, 1, &mii );

		InsertMenuItem((HMENU)parent, i, true, &mii);

		if( mii.hSubMenu )		_InsertFromResource( mii.hSubMenu, name, submenu);
	}
}

//-----------------------------------------------------------------
// Name:	RemoveItem()
// Desc:	...
//-----------------------------------------------------------------
void NMenuBar::RemoveItem(udword pos)
{
	RemoveMenu(mW32Menu, pos, MF_BYPOSITION);
	DrawMenuBar(mOwner->m_W32HWnd);
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					File Dialog Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NFileDialog::Create(char* name, NWnd* parent, bool open, bool multiselect)
{
	//Open or Save Display
	mOpenFileDialog = open;

	//Win32 Open Dialog Box Structure
	memset( &mOfn, 0, sizeof(OPENFILENAME));

	mOfn.lStructSize		= sizeof(OPENFILENAME);
	mOfn.hInstance			= GetModuleHandle (null);
	SetCurrentDirectory("../");

  if (parent!=null)		mOfn.hwndOwner = parent->m_W32HWnd;
	else								mOfn.hwndOwner = null;

	//To Do...
	NString Filename	= "";
	NString DefExt		= "";
	NString Filter		= "All Files (*.*)|*.*||";
	NString InitialDir	= "";

	//Filters
		//Translate string filter into OPENFILENAME format (lots of \0)
		sdword idx;
		if (Filter.Length() )	{
			mTransfilter = Filter;
			while ((idx = mTransfilter.ReverseFind('|')) != -1)		mTransfilter.Buffer()[idx]=0;		//Change '|' with '\0'
		}
		//Fill Struct
		mOfn.lpstrFilter		= (LPCTSTR)mTransfilter.Buffer();
		mOfn.lpstrCustomFilter	= NULL;
		mOfn.nMaxCustFilter		= 0;
		mOfn.nFilterIndex		= 0;

	//Default File Name edit control (return)
		// setup initial file name
		ZeroMemory(mFileName, sizeof(mFileName));
		if (Filename.Length())	lstrcpyn((LPTSTR)mFileName, (char*)Filename.Buffer(), sizeof(mFileName));
		//Fill Struct
		mOfn.lpstrFile			= (LPTSTR)mFileName;
		mOfn.nMaxFile			= sizeof(mFileName);

	//Selected File Name (return)
    mOfn.lpstrFileTitle		= NULL;
	mOfn.nMaxFileTitle		= 0;

	//Default File Directory
    mOfn.lpstrInitialDir	= null;	//(LPCTSTR)InitialDir.Buffer();

	//Dialog Title Name
	mOfn.lpstrTitle			= name;

	//Offset
	mOfn.nFileOffset		= 0;
    mOfn.nFileExtension		= 0;

	//Default extension
	mOfn.lpstrDefExt		= null;	//(LPCTSTR)DefExt.Buffer();

	//Flags
    mOfn.Flags				= OFN_EXPLORER;
	if (multiselect)		mOfn.Flags|=OFN_ALLOWMULTISELECT;

	//...
    mOfn.lCustData			= 0;
	mOfn.lpfnHook			= NULL;
    mOfn.lpTemplateName		= NULL;

	return true;
}


//-----------------------------------------------------------------
// Name:	DoModal()
// Desc:	...
//-----------------------------------------------------------------
udword NFileDialog::DoModal()
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

//-----------------------------------------------------------------
// Name:	GetFileNumber()
// Desc:	...
//-----------------------------------------------------------------
udword NFileDialog::GetFileNumber()
{
	udword	ret = 1, i = 0;

	while (i < _MAX_PATH)		{
		while (mFileName[i])		i++;

		if (!mFileName[i+1])		return ret == 1 ? ret : ret - 1;
		else						ret++;

		i++;
	}

	return ret;
}


//-----------------------------------------------------------------
// Name:	GetPathName()
// Desc:	...
//-----------------------------------------------------------------
NString NFileDialog::GetPathName(udword Index) const
{
	//Get File Path
	NString path = mFileName;

	//If User Select only one file
	if (mFileName[path.Length()+1]==0 || mFileName[path.Length()+2]==0)	return path;

	//Get FileName
	udword i=0, lpos=0;

	//If More than 1 file selected
	while (mFileName[i] | mFileName[i+1] ) {
		if (mFileName[i] == 0)	{
			if (lpos==Index)									return path+"\\"+(mFileName+i+1);
			lpos++;
		}
		i++;
	}

	//Return an Empty String if no pathname found at the given index
	return NString("");
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Status bar Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NStatusBar::NStatusBar() : NWnd()
{
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NStatusBar::~NStatusBar()
{
}

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NStatusBar::Create(NWnd* parent)
{
	NWNDCREATE				wc;
	wc.Id							= 1;
	wc.Name						= "statusbar";
	wc.Parent					= parent;
	wc.Rect						= NRect(0,0,1,1);
	wc.W32ClassName	= "FXGen_WNDCLASS";
	wc.W32Style				= WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN;
	wc.W32StyleEx			= WS_EX_DLGMODALFRAME/*WS_EX_CLIENTEDGE*/;
	NWnd::Create(wc);

	return true;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Color Dialog Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NColorDialog::NColorDialog()
{
}

NColorDialog::~NColorDialog()
{
}

bool NColorDialog::Create(const char* name, NWnd* parent, const NColor& _color)
{
	//Store Some Infos
	m_Color = _color;

	//Win32 Color Dialog Box Structure
	memset( &m_Cc, 0,	  sizeof(CHOOSECOLOR));
	m_Cc.lStructSize		= sizeof(CHOOSECOLOR);
	m_Cc.Flags					= CC_ANYCOLOR|CC_FULLOPEN|CC_RGBINIT;
	m_Cc.lpCustColors		= m_acrCustClr;
	m_Cc.rgbResult			= m_Color.GetARGB();

  if (parent!=null)		m_Cc.hwndOwner = parent->m_W32HWnd;
	else								m_Cc.hwndOwner = null;

	return true;
}

udword	NColorDialog::DoModal()
{
	//Call W32 Fct
	udword Result = (udword)::ChooseColor(&m_Cc);

	//Store some infos
	m_Color	= NColor( m_Color.mA, (float)GetBValue(m_Cc.rgbResult)/255.0f, (float)GetGValue(m_Cc.rgbResult)/255.0f, (float)GetRValue(m_Cc.rgbResult)/255.0f);

	return Result;
}

void NColorDialog::SetColor(NColor& _color)
{
	m_Cc.rgbResult = _color.GetARGB();
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NGradientCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

int cmp(const void *elem1, const void *elem2)
{
	GradientElem *e1 = (GradientElem*)elem1;
	GradientElem *e2 = (GradientElem*)elem2;

	return e1->height >= e2->height;
}

COLORREF g_rgbBackground = RGB(0, 0, 255);
COLORREF g_rgbCustom[16] = {0};

void DoSelectColour(HWND hwnd)
{
    CHOOSECOLOR cc = {sizeof(CHOOSECOLOR)};

    cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR;
    cc.hwndOwner = hwnd;
    cc.rgbResult = g_rgbBackground;
    cc.lpCustColors = g_rgbCustom;

    if(ChooseColor(&cc))
    {
        g_rgbBackground = cc.rgbResult;
    }
}

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NGradientCtrl::NGradientCtrl()
{
	// misc dimension variables and bools
	start_client_x = 40;
	start_client_y = 90;
	client_Width = 430;
	client_Height = 40;


	m_bSelectedRect = false;
	m_bMouseLDown = false;

	// I need two gradient points at least
	m_clientLineX = 0;
	NColor start;
	start.mR = 0;
	start.mG = 0;
	start.mB = 0;
	start.mA = 255;
	float fStart = start_client_x;

	// point 1
	GradientElem elem;
	elem.height = fStart;
	elem.color[0] = start.mR;
	elem.color[1] = start.mG;
	elem.color[2] = start.mB;
	elem.color[3] = start.mA;
	m_gradientElems.AddItem( elem );

	NColor end;
	end.mR = 255;
	end.mG = 255;
	end.mB = 255;
	end.mA = 255;
	float fEnd = start_client_x + client_Width;

	// point 2
	GradientElem elem2;
	elem2.height = fEnd;
	elem2.color[0] = end.mR;
	elem2.color[1] = end.mG;
	elem2.color[2] = end.mB;
	elem2.color[3] = end.mA;
	m_gradientElems.AddItem( elem2 );


}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NGradientCtrl::~NGradientCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NGradientCtrl::Create(const char* name, const NRect& rect, NWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.Id						= 1;
	wc.Name					= const_cast<char*>(name);
	wc.Parent				= parent;
	wc.Rect					= rect;
	wc.W32ClassName	= "FXGen_WNDCLASS";
	wc.W32Style			= WS_CHILD | WS_CLIPCHILDREN;   //WS_CLIPSIBLINGS;
	wc.W32StyleEx		= WS_EX_ACCEPTFILES;
	NWnd::Create(wc);

	//Create Normal Font
	m_hfontNormal = ::CreateFont(
		10, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Lucida console");

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Window proc
//-----------------------------------------------------------------
LRESULT NGradientCtrl::WndProc( UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg==WM_ERASEBKGND)		return 1;

	//Else Call Base Class
	else return NWnd::WndProc(msg, wparam, lparam);

}

//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
void NGradientCtrl::Update()
{

	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}

//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
float NGradientCtrl::to_graph_space( float X )
{
	return 2*(X - start_client_x)/client_Width - 1.0;
}

//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
float NGradientCtrl::from_graph_space( float X )
{
	return client_Width*( X + 1.0 )/ 2 + start_client_x;
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.FillSolidRect(rc, RGB(115,115,115));
	dc.SetBkMode(TRANSPARENT);

	int NumElems = m_gradientElems.Count();

	for( int i = 0; i < NumElems; i++ )
	{

		NRect rct2;
		rct2.left		= (sdword) (m_gradientElems[i].height );
		rct2.top		= (sdword) (start_client_y);

		// is this the end? use the end
		float h = i==NumElems-1?
			start_client_x + client_Width:
			m_gradientElems[i+1].height;

		rct2.right	= (sdword) ((h) );
		rct2.bottom	= (sdword) (rct2.top  + client_Height);

		sdword r1 = m_gradientElems[i].color[0];//.mR;
		sdword g1 = m_gradientElems[i].color[1];//.mG;
		sdword b1 = m_gradientElems[i].color[2];//.mB;

		// is this the end? then use the current color
		sdword r2 = i == NumElems-1 ? m_gradientElems[i].color[0] : m_gradientElems[i+1].color[0] ;
		sdword g2 = i == NumElems-1 ? m_gradientElems[i].color[1] : m_gradientElems[i+1].color[1] ;
		sdword b2 = i == NumElems-1 ? m_gradientElems[i].color[2] : m_gradientElems[i+1].color[2] ;

		// render the current gradient
		dc.GradientHRect(rct2,
			RGB(r1, g1, b1),
			RGB(r2, g2, b2));

		// draw small blue rectangle
		NRect rc4;
		rc4.left		= (sdword) ( m_gradientElems[i].height );
		rc4.top		= (sdword) ( start_client_y + client_Height );
		rc4.right	= (sdword) ( rc4.left + 2.0);
		rc4.bottom	= (sdword) ( rc4.top  + 2.0 );

		dc.FillSolidRect( rc4, RGB(64,64,128) );

		// draw line through gradient
		NRect rc5;
		rc5.left	= (sdword) (m_gradientElems[i].height );
		rc5.top		= (sdword) ( start_client_y );
		rc5.right	= (sdword) ( rc5.left + 1 );
		rc5.bottom	= (sdword) ( rc5.top  + 10 );

		dc.FillSolidRect(rc5, RGB(0,0,0));

		// draw arrow for point
		dc.MoveTo( m_gradientElems[i].height,      start_client_y + client_Height	);
		dc.LineTo( m_gradientElems[i].height - 1,  start_client_y + client_Height + 1 );

		dc.MoveTo( m_gradientElems[i].height - 1,  start_client_y + client_Height + 1	);
		dc.LineTo( m_gradientElems[i].height + 1,  start_client_y + client_Height + 1);

		dc.MoveTo( m_gradientElems[i].height + 1,  start_client_y + client_Height + 1	);
		dc.LineTo( m_gradientElems[i].height,      start_client_y + client_Height );



		// draw a selection rectangle
		NRect rc7;
		rc7.left		= (sdword) (m_gradientElems[i].height - 2 );
		rc7.top		= (sdword) ( start_client_y + client_Height + 10 );
		rc7.right	= (sdword) ( rc7.left + 4 );
		rc7.bottom	= (sdword) ( rc7.top  + 4 );
		dc.FillSolidRect( rc7, RGB(0,0,0));
	}

	/*{
		NRect rectText1;
		rectText1.left		= (sdword) ( 430 );
		rectText1.top		= (sdword) ( y + Height + 10 );
		rectText1.right	= (sdword) ( rectText1.left + 84.0);
		rectText1.bottom	= (sdword) ( rectText1.top  + 32 );

		char buff[256];
		sprintf( buff, "%f", (float)1 );

		dc.DrawTextA(buff, rectText1, 0);

		NRect rectText2;
		rectText2.left		= (sdword) ( 30 );
		rectText2.top		= (sdword) ( y + Height + 10 );
		rectText2.right	= (sdword) ( rectText2.left + 84.0);
		rectText2.bottom	= (sdword) ( rectText2.top  + 32 );

		char buff2[256];
		sprintf( buff2, "%f", (float)(-1.0) );

		dc.DrawTextA(buff2, rectText2, 0);

	}
*/



	// clamp coordinates
	if( m_clientLineX < start_client_x ) m_clientLineX = start_client_x;
	if( m_clientLineX > start_client_x + client_Width ) m_clientLineX = start_client_x + client_Width;

	// draw current rectangle
	// note: where is this thing?
	NRect rc6;
	rc6.left		= (sdword) ( m_clientLineX );
	rc6.top		= (sdword) ( start_client_y );
	rc6.right	= (sdword) ( rc6.left + 1 );
	rc6.bottom	= (sdword) ( rc6.top  + 10 );
	dc.FillSolidRect( rc6, RGB(0,0,0));

	// draw current arrow
	dc.MoveTo( m_clientLineX,      start_client_y + client_Height	);
	dc.LineTo( m_clientLineX - 1,  start_client_y + client_Height + 1 );

	dc.MoveTo( m_clientLineX - 1,  start_client_y + client_Height + 1	);
	dc.LineTo( m_clientLineX + 1,  start_client_y + client_Height + 1);

	dc.MoveTo( m_clientLineX + 1,  start_client_y + client_Height + 1	);
	dc.LineTo( m_clientLineX,      start_client_y + client_Height );

	// render the selected rectangle in red
	if( m_bSelectedRect )
	{
		NRect rc8;
		rc8.left		= (sdword) (selectedElem->height - 2 );
		rc8.top		= (sdword) ( start_client_y + client_Height + 10 );
		rc8.right	= (sdword) ( rc8.left + 4 );
		rc8.bottom	= (sdword) ( rc8.top  + 4 );
		dc.FillSolidRect( rc8, RGB(255,0,0));

		// add text for the selected rectangle...
		NRect rectText;
		rectText.left		= (sdword) ( selectedElem->height );
		rectText.top		= (sdword) ( start_client_y + client_Height + 10 );
		rectText.right	= (sdword) ( rectText.left + 84.0);
		rectText.bottom	= (sdword) ( rectText.top  + 32 );

		char buff[256];
		sprintf( buff, "%f", to_graph_space(selectedElem->height) );

		dc.DrawTextA(buff, rectText, 0);
	}
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::SelectRectangles()
{
	int NumElems = m_gradientElems.Count();

	for( int i = 0; i < NumElems; i++ )
	{
		NRect rc7;
		rc7.left	= (sdword) ( m_gradientElems[i].height-2 );
		rc7.top		= (sdword) ( start_client_y + client_Height + 10 );
		rc7.right	= (sdword) ( rc7.left + 4 );
		rc7.bottom	= (sdword) ( rc7.top  + 4 );

		if( rc7.Contain( mousePos ) )
		{
			sel = rc7;
			selectedElem = &m_gradientElems[i];
			m_bSelectedRect = true;
			return;
		}
	}

	m_bSelectedRect = false;
	selectedElem = NULL;
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::OnLeftButtonDown(udword flags, NPoint pos)
{
	SetFocus();

	m_clientLineX = pos.x;

	mousePos = pos;

	SelectRectangles();

	m_bMouseLDown = true;
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::OnLeftButtonUp(udword flags, NPoint pos)
{
	m_bMouseLDown = false;
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::OnMouseMove(udword flags, NPoint pos )
{
	if( m_bSelectedRect && m_bMouseLDown )
	{
		if( selectedElem->height == start_client_x ||
			selectedElem->height == start_client_x + client_Width ) return;

		m_clientLineX = pos.x;
		if( m_clientLineX < start_client_x ) m_clientLineX = start_client_x;
		if( m_clientLineX > start_client_x + client_Width ) m_clientLineX = start_client_x + client_Width;

		selectedElem->height = m_clientLineX;

		for( int i = 0 ; i < m_gradientElems.Count(); i++ )
		{
			for( int j = 0; j < m_gradientElems.Count(); j++ )
			{
				if( i != j && m_gradientElems[i].height == m_gradientElems[j].height )
				{

					m_gradientElems[i].height -= 1;
					if( m_gradientElems[i].height < start_client_x )
					{
						m_gradientElems[i].height = start_client_x;
						m_gradientElems[j].height += 1;
					}
					if( m_gradientElems[i].height > start_client_x + client_Width )
					{
						m_gradientElems[i].height = start_client_x + client_Width;
						m_gradientElems[j].height -= 1;
					}
				}

			}
		}
		m_gradientElems.Sort(cmp);
	}
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::OnSize()
{
	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::AssignColorToSelection()
{
	DoSelectColour( this->m_W32HWnd );
	if( m_bSelectedRect )
	{
		selectedElem->color[2] = RGBA_GETRED( g_rgbBackground );
		selectedElem->color[1] = RGBA_GETGREEN( g_rgbBackground );
		selectedElem->color[0]= RGBA_GETBLUE( g_rgbBackground );
		selectedElem->color[3] = 255;
	}
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::OnAddPoint()
{
	DoSelectColour( this->m_W32HWnd );
	NColor color;

	// should this be BGR or RGB?
	color.mB = RGBA_GETRED( g_rgbBackground );
	color.mG = RGBA_GETGREEN( g_rgbBackground );
	color.mR = RGBA_GETBLUE( g_rgbBackground );
	color.mA = 255;

	if( m_clientLineX < start_client_x ) m_clientLineX = start_client_x;
	if( m_clientLineX > start_client_x + client_Width ) m_clientLineX = start_client_x + client_Width;
	AddGradientPoint( m_clientLineX, color );
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGradientCtrl::AddGradientPoint( float height, const NColor &color )
{
	float rHeight = height;

	GradientElem elem;
	elem.height = rHeight;
	elem.color[0] = color.mR;
	elem.color[1] = color.mG;
	elem.color[2] = color.mB;
	elem.color[3] = color.mA;
	m_gradientElems.AddItem( elem );
	m_gradientElems.Sort(cmp);
}

void NGradientCtrl::OnDeletePoint()
{
	for( int i = 0; i < m_gradientElems.Size(); i++ )
	{
		if( selectedElem == &m_gradientElems[i] )
		{
			float f1 =to_graph_space(m_gradientElems[i].height);

			if (!( f1== 1.0 || f1 == -1.0))
				m_gradientElems.RemoveItem(i);
			break;
		}
	}
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NCurveCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

int cmp_curve_point(const void *elem1, const void *elem2)
{
	NPoint *e1 = (NPoint*)elem1;
	NPoint *e2 = (NPoint*)elem2;

	return e1->x <= e2->x;
}


//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NGraphCtrl::NGraphCtrl()
{
	// misc dimension variables and bools
	start_x = 40;
	start_y = 60;
	Width = 400;
	Height = 200;
	end_x = start_x + Width;
	end_y = start_y + Height;

	x_min = -1.0f;
	x_max = 1.0f;
	y_min = -1.0f;
	y_max = 1.0f;

	graph_space.left = x_min;
	graph_space.right = x_max;
	graph_space.top = y_max;
	graph_space.bottom = y_min;

	m_bSelectedRect = false;
	m_bMouseLDown = false;

	m_currentLineX = 0;
	m_currentLineY = 0;

	bMovingPoint = false;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NGraphCtrl::~NGraphCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NGraphCtrl::Create(const char* name, const NRect& rect, NWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.Id						= 1;
	wc.Name					= const_cast<char*>(name);
	wc.Parent				= parent;
	wc.Rect					= rect;
	wc.W32ClassName	= "FXGen_WNDCLASS";
	wc.W32Style			= WS_CHILD | WS_CLIPCHILDREN;   //WS_CLIPSIBLINGS;
	wc.W32StyleEx		= WS_EX_ACCEPTFILES;
	NWnd::Create(wc);

	//Create Normal Font
	m_hfontNormal = ::CreateFont(
		10, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Lucida console");

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Window proc
//-----------------------------------------------------------------
LRESULT NGraphCtrl::WndProc( UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg==WM_ERASEBKGND)		return 1;

	//Else Call Base Class
	else return NWnd::WndProc(msg, wparam, lparam);

}

//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
void NGraphCtrl::Update()
{
	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}


NDPoint NGraphCtrl::to_graph_space( float X, float Y )
{

	// X = 15,
	// percentage = ( X - start_x ) / ( end_x - start_x )
	// range = x_max - x_min
	// val = range * percentage + x_min
	// val = 2 * 0.25 + (-1)

	// compute percentage of current space
	// calculate the range of the target space
	// compute value in range of target space
	// add the value to the min value of the target space

	NDPoint pt;

	float percentageX = ( X - start_x ) / ( end_x - start_x );
	float percentageY = ( Y - start_y ) / ( end_y - start_y );

	float rangeX = graph_space.right - graph_space.left;
	float rangeY = -(graph_space.top - graph_space.bottom );

	pt.x = rangeX * percentageX + graph_space.left;
	pt.y = rangeY * percentageY + graph_space.top;

	return pt;
}

NDPoint NGraphCtrl::from_graph_space( float X, float Y )
{
	// basically this is the inverse of
	// to_graph_space(...)

	NDPoint pt;

	float rangeX = graph_space.right - graph_space.left;
	float rangeY = -(graph_space.top - graph_space.bottom);

	pt.x = ( end_x - start_x )* (X - graph_space.left) / rangeX + start_x;
	pt.y = ( end_y - start_y )* (Y - graph_space.top) / rangeY + start_y;

	return pt;
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGraphCtrl::Paint(NGraphics &dc)
{
	NRect rc = GetClientRect();

	dc.SetBkMode(TRANSPARENT);

	int NumElems = m_curveElems.Count();

	for( int i = 0; i < NumElems; i++ )
	{
		NDPoint gyx;
		gyx = from_graph_space( m_curveElems[i].x, m_curveElems[i].y  );

		// draw small blue rectangle
		NRect rc4;
		rc4.left		= (sdword) ( gyx.x );
		rc4.top		= (sdword) ( gyx.y );
		rc4.right	= (sdword) ( rc4.left + 8.0);
		rc4.bottom	= (sdword) ( rc4.top  + 8.0 );

		dc.FillSolidRect( rc4, RGB(0,0,128) );
	}


	////////////////////////////////////
	//plot the bounding box

	dc.SetPen(2, RGB(0,0,0));

	dc.MoveTo( start_x,			start_y );				// top left
	dc.LineTo( start_x,			start_y + Height );	// bottom left
	dc.LineTo( start_x + Width, start_y + Height );	// bottom right
	dc.LineTo( start_x + Width, start_y );				// top right
	dc.LineTo( start_x,			start_y );				// top left


	dc.SetPen(1, RGB(255,0,0));
	//////////////////////////////////////////
	// PLOT the text extents here!
	{
		// X min, Y min
		NRect rectX1Y1;
		rectX1Y1.left		= (sdword) ( start_x - 30 );
		rectX1Y1.top		= (sdword) ( end_y + 5 - Height / 2);
		rectX1Y1.right	= (sdword) ( rectX1Y1.left + 160);
		rectX1Y1.bottom	= (sdword) ( rectX1Y1.top  + 32 );

		char xmin_ymin[256];
		sprintf( xmin_ymin, "%f", (float)graph_space.left );
		dc.DrawTextA(xmin_ymin, rectX1Y1, 0);

		// X max, Y min
		NRect rectX2Y1;
		rectX2Y1.left		= (sdword) ( end_x + 10 );
		rectX2Y1.top		= (sdword) ( end_y + 5 - Height / 2);
		rectX2Y1.right	= (sdword) ( rectX2Y1.left + 160);
		rectX2Y1.bottom	= (sdword) ( rectX2Y1.top  + 32 );

		char xmax_ymin[256];
		sprintf( xmax_ymin, "%f", (float)graph_space.right );
		dc.DrawTextA(xmax_ymin, rectX2Y1, 0);

		// X min, Y max
		NRect rectX1Y2;
		rectX1Y2.left		= (sdword) ( start_x + Width / 2);
		rectX1Y2.top		= (sdword) ( start_y - 20 );
		rectX1Y2.right	= (sdword) ( rectX1Y2.left + 160);
		rectX1Y2.bottom	= (sdword) ( rectX1Y2.top  + 32 );

		char xmin_ymax[256];
		sprintf( xmin_ymax, "%f", (float)graph_space.top );
		dc.DrawTextA(xmin_ymax, rectX1Y2, 0);

		// X max, Y max
		NRect rectX2Y2;
		rectX2Y2.left		= (sdword) (  start_x + Width / 2);
		rectX2Y2.top		= (sdword) ( end_y + 10 );
		rectX2Y2.right	= (sdword) ( rectX2Y2.left + 160);
		rectX2Y2.bottom	= (sdword) ( rectX2Y2.top  + 32 );

		char xmax_ymax[256];
		sprintf( xmax_ymax, "%f", (float)graph_space.bottom );
		dc.DrawTextA(xmax_ymax, rectX2Y2, 0);
	}


	////////////////////
	// Draw the zero point in red
	dc.SetPen(1, RGB(255,0,0));

	if( graph_space.left < 0 && graph_space.right > 0 )
	{
		float Y_AXIS_X = 0.0f;
		float Y_AXIS_Y1 = graph_space.top;
		float Y_AXIS_Y2 = graph_space.bottom;

		NDPoint zero_point1 = from_graph_space( Y_AXIS_X, Y_AXIS_Y1 );
		NDPoint zero_point2 = from_graph_space( Y_AXIS_X, Y_AXIS_Y2 );

		if( zero_point1.x > start_x && zero_point1.x < end_x )
		{
			dc.MoveTo( zero_point1.x,			zero_point1.y  );				// middle top
			dc.LineTo( zero_point2.x,			zero_point2.y);	// middle bottom
		}
	}

	if( graph_space.bottom < 0 && graph_space.top > 0 )
	{
		float X_AXIS_Y = 0.0f;
		float X_AXIS_X1 = graph_space.right;
		float X_AXIS_X2 = graph_space.left;

		NDPoint zero_point1 = from_graph_space( X_AXIS_X1, X_AXIS_Y );
		NDPoint zero_point2 = from_graph_space( X_AXIS_X2, X_AXIS_Y );

		if( zero_point1.y > start_y && zero_point1.y < end_y )
		{
			dc.MoveTo( zero_point1.x,			zero_point1.y  );		// middle left
			dc.LineTo( zero_point2.x,			zero_point2.y );			// middle right
		}
	}

	// draw current point axes
	dc.SetPen(1, RGB(0,0,255));
	dc.MoveTo( m_currentLineX,  m_currentLineY );
	dc.LineTo( start_x,			m_currentLineY );

	dc.MoveTo( m_currentLineX,  m_currentLineY );
	dc.LineTo( end_x,			m_currentLineY );

	dc.MoveTo( m_currentLineX,			m_currentLineY );
	dc.LineTo( m_currentLineX,			start_y );

	dc.MoveTo( m_currentLineX,			m_currentLineY );
	dc.LineTo( m_currentLineX,			end_y  );

	// clamp coordinates
	if( m_currentLineX < start_x ) m_currentLineX = start_x;
	if( m_currentLineX > start_x + Width ) m_currentLineX = start_x + Width;

	if( m_currentLineY < start_y ) m_currentLineY = start_y;
	if( m_currentLineY > start_y + Height ) m_currentLineY = start_y + Height;

	// draw the line for the arrow
	NRect rc6;
	rc6.left		= (sdword) ( m_currentLineX );
	rc6.top		= (sdword) ( start_y + 2*Height );
	rc6.right	= (sdword) ( rc6.left + 1 );
	rc6.bottom	= (sdword) ( rc6.top  + 10 );
	dc.FillSolidRect( rc6, RGB(0,0,0));


	// draw current arrow
	dc.MoveTo( m_currentLineX,      start_y + 2*Height	);
	dc.LineTo( m_currentLineX - 1,  start_y + 2*Height + 1 );

	dc.MoveTo( m_currentLineX - 1,  start_y + 2*Height + 1	);
	dc.LineTo( m_currentLineX + 1,  start_y + 2*Height + 1);

	dc.MoveTo( m_currentLineX + 1,  start_y + 2*Height + 1	);
	dc.LineTo( m_currentLineX,      start_y + 2*Height );

	// render the selected rectangle in red
	if( m_bSelectedRect )
	{

		NDPoint sel_elem_screen;

		sel_elem_screen = from_graph_space( selectedElem->x, selectedElem->y  );
		NRect rc8;
		rc8.left		= (sdword) (sel_elem_screen.x );
		rc8.top		= (sdword) ( sel_elem_screen.y );
		rc8.right	= (sdword) ( rc8.left + 8 );
		rc8.bottom	= (sdword) ( rc8.top  + 8 );
		dc.FillSolidRect( rc8, RGB(255,0,0));


		NRect rectText2;
		rectText2.left		= (sdword) ( start_x );
		rectText2.top		= (sdword) ( end_y + 70 );
		rectText2.right	= (sdword) ( rectText2.left + 260);
		rectText2.bottom	= (sdword) ( rectText2.top  + 32 );

		//NDPoint conv;
		//conv = to_graph_space( selectedElem->x, selectedElem->y );
		char buff[256];
		sprintf( buff, "Selected Point: %f, %f", selectedElem->x, selectedElem->y );

		dc.DrawTextA(buff, rectText2, 0);
	}
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGraphCtrl::SelectRectangles()
{
	int NumElems = m_curveElems.Count();

	for( int i = 0; i < NumElems; i++ )
	{
		NDPoint pPoint = from_graph_space(m_curveElems[i].x, m_curveElems[i].y);

		NRect rc7;
		rc7.left		= (sdword) (pPoint.x-2 );
		rc7.top		= (sdword) ( pPoint.y - 2 );
		rc7.right	= (sdword) ( rc7.left + 8 );
		rc7.bottom	= (sdword) ( rc7.top  + 8 );

		if( rc7.Contain( mousePos ) )
		{
			sel = rc7;
			selectedElem = &m_curveElems[i];
			m_bSelectedRect = true;
			return;
		}
	}

	m_bSelectedRect = false;
	selectedElem = NULL;
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGraphCtrl::OnLeftButtonDown(udword flags, NPoint pos)
{
	SetFocus();

	m_currentLineX = pos.x;
	m_currentLineY = pos.y;

	mousePos = pos;

	if( !bMovingPoint )
	{
		SelectRectangles();
	}

	m_bMouseLDown = true;
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGraphCtrl::OnLeftButtonUp(udword flags, NPoint pos)
{
	m_bMouseLDown = false;
	bMovingPoint = false;
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGraphCtrl::OnSize()
{
	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NGraphCtrl::OnAddPoint()
{
	if( m_currentLineX < start_x ) m_currentLineX = start_x;
	if( m_currentLineX > start_x + Width ) m_currentLineX = start_x + Width;
	if( m_currentLineY < start_y ) m_currentLineY = start_y;
	if( m_currentLineY > start_y + Height ) m_currentLineY = start_y + Height;

	AddCurvePoint( m_currentLineX, m_currentLineY );
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NCurveCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NCurveCtrl::NCurveCtrl() : NGraphCtrl()
{
	float fStart = start_x;
	float fEnd = start_x + Width;
	float fMid1 =   (fEnd - fStart) / 3;
	float fMid2 = 2*(fEnd - fStart) / 3;

	// point 1
	NDPoint elem;
	elem.x = fStart;
	elem.y = start_y + Height /2 ;
	NDPoint add_1 = to_graph_space(elem.x, elem.y);
	m_curveElems.AddItem( add_1 );

	// point 2
	NDPoint elem2;
	elem2.x = fMid1;
	elem2.y = start_y + Height /2 ;
	NDPoint add_2 = to_graph_space(elem2.x, elem2.y);
	m_curveElems.AddItem( add_2 );

	// point 3
	NDPoint elem3;
	elem3.x = fMid2;
	elem3.y = start_y + Height /2 ;
	NDPoint add_3 = to_graph_space(elem3.x, elem3.y);
	m_curveElems.AddItem( add_3 );

	// point 4
	NDPoint elem4;
	elem4.x = fEnd;
	elem4.y = start_y + Height /2;
	NDPoint add_4 = to_graph_space(elem4.x, elem4.y);
	m_curveElems.AddItem( add_4 );

}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NCurveCtrl::~NCurveCtrl()
{
}


//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NCurveCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.SetBkMode(TRANSPARENT);
	dc.FillSolidRect(rc, RGB(255,255,255));

	int NumElems = m_curveElems.Count();

	// using a P3DMathNaturalCubicSpline ...
	// from ngplant...
	P3DMathNaturalCubicSpline curveToPlot;

	for( int i = 0; i < NumElems; i++ )
	{
		// add the current control point to the curve ...
		//NDPoint gxy;
		//gxy = to_graph_space(m_curveElems[i].x, m_curveElems[i].y );
		curveToPlot.AddCP(m_curveElems[i].x, m_curveElems[i].y );
	}


	/////////////////////////////////////////
	// now plot the curve...

	dc.SetPen(2, RGB(0,255,0));
	dc.MoveTo( start_x,      start_y + Height/2	);

	for( int i = start_x; i < end_x; i++ )
	{
		// curve for graph-space 'x' in screen space
		NDPoint p1;
		p1 = to_graph_space(i , curveToPlot.GetValue(i) );
		p1 = from_graph_space(i , curveToPlot.GetValue(p1.x) );
		dc.MoveTo( i,      p1.y	);

		// curve for graph-space 'x' in screen space
		NDPoint p2;
		p2 = to_graph_space(i+1, curveToPlot.GetValue(i+1) );
		p2 = from_graph_space(i+1, curveToPlot.GetValue(p2.x) );

		if(	p2.y > start_y && p2.y < end_y ){
			dc.LineTo( i+1,   p2.y );
		}
	}

	NGraphCtrl::Paint(dc);

}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NCurveCtrl::OnMouseMove(udword flags, NPoint pos )
{
	if( m_bSelectedRect && m_bMouseLDown )
	{

		float moveX =pos.x;
		float moveY = pos.y;
		NDPoint pPoint1 = to_graph_space(moveX, moveY);

		if( m_currentLineX < start_x ) m_currentLineX = start_x;
		if( m_currentLineX > start_x + Width ) m_currentLineX = start_x + Width;

		if( m_currentLineY < start_y ) m_currentLineY = start_y;
		if( m_currentLineY > start_y + Height ) m_currentLineY = start_y + Height;

		bMovingPoint = true;
		bool bSwappedPoint = false;

		for( int i = 0 ; i < m_curveElems.Count(); i++ )
		{
			if( m_curveElems[i].x == pPoint1.x )
			{
				if( selectedElem != &m_curveElems[i] )
				{

					if( selectedElem->x > pPoint1.x )
						pPoint1.x +=1;
					if( selectedElem->x < pPoint1.x )
						pPoint1.x -=1;

					//NDPoint pPoint2 = from_graph_space(selectedElem->x, selectedElem->y);

					//moveX = pPoint2.x;
					pPoint1.x = selectedElem->x;
					//MessageBox(0,"","",0);
					//bMovingPoint = false;
					bSwappedPoint = true;
					break;
				}
			}
			/*for( int j = 0; j < m_curveElems.Count(); j++ )
			{
				if( i != j && m_curveElems[i].x == m_curveElems[j].x )
				{


					m_curveElems[i].x -= 1;
					if( m_curveElems[i].x < x )
					{
						m_curveElems[i].x = x;
						m_curveElems[j].x += 1;
					}
					if( m_curveElems[i].x > x + Width )
					{
						m_curveElems[i].x = x + Width;
						m_curveElems[j].x -= 1;
					}

					bMovingPoint = false;
					break;
				}
			}*/
		}

		if( bMovingPoint == true )
		{

			NDPoint pPoint2 = from_graph_space(pPoint1.x, pPoint1.y);
			m_currentLineX = pos.x;
			m_currentLineY = pos.y;
			NDPoint pPoint = to_graph_space(m_currentLineX, m_currentLineY);
			selectedElem->x = pPoint1.x;
			selectedElem->y = pPoint1.y;
		}

		if( bSwappedPoint )m_curveElems.Sort(cmp_curve_point);
	}
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NCurveCtrl::AddCurvePoint( float X, float Y )
{
	NDPoint pt = to_graph_space(X,Y);
	//pt.x = X;
	//pt.y = Y;

	m_curveElems.AddItem( pt );
	m_curveElems.Sort(cmp_curve_point);
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NCurveCtrl::OnDeletePoint()
{
	if( m_curveElems.Size() <= 4 )
		return;

	for( int i = 0; i < m_curveElems.Size(); i++ )
	{
		if( selectedElem == &m_curveElems[i] )
		{
			m_curveElems.RemoveItem(i);
			break;
		}
	}
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NTerraceCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
int cmp_curve_point2(const void *elem1, const void *elem2)
{
	NPoint *e1 = (NPoint*)elem1;
	NPoint *e2 = (NPoint*)elem2;

	return e1->x > e2->x;
}

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NTerraceCtrl::NTerraceCtrl() : NGraphCtrl()
{
	float fStart = start_x;
	float fEnd = start_x + Width/2;
	float fMid1 =   (fEnd - fStart) / 4;
	float fMid2 = 2*(fEnd - fStart) / 3;

	// point 1
	NDPoint elem;
	elem.x = fStart;
	elem.y = start_y + Height /2 ;
	AddCurvePoint(elem.x, elem.y);

	// point 4
	NDPoint elem4;
	elem4.x = fMid1;
	elem4.y = start_y + Height /2;
	AddCurvePoint(elem4.x, elem4.y);

	m_bInvertTerrace = false;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NTerraceCtrl::~NTerraceCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NTerraceCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.SetBkMode(TRANSPARENT);
	dc.FillSolidRect(rc, RGB(255,255,255));


	int NumElems = m_curveElems.Count();

	for( int i = 0; i < NumElems; i++ )
	{
		// we need to use the control point in client-rect space

		NDPoint gyx1, gyx2;
		gyx1 = from_graph_space( m_curveElems[i].x, m_curveElems[i].y  );

		if( i < NumElems )
		{
			gyx2 = from_graph_space( m_curveElems[i+1].x, m_curveElems[i+1].y  );

			/////////////////////////////////////////
			// now plot the curve...

			dc.SetPen(2, RGB(0,255,0));
			dc.MoveTo( start_x,      start_y + Height/2	);

			int p;

			float span = m_curveElems[i+1].x - m_curveElems[i].x;

			// we loop in client-rect space, because the client-rect space
			// is definately a set of integers, and is also the visible space ->
			for( int j = (int)gyx1.x; j < (int)gyx2.x; j++ )
			{
				// we need to convert j, the looping value
				// back to graph space,
				NDPoint input_1 = to_graph_space(j,y_max);

				// now the terrace value is computed, in graph space
				// in the same way as the libnoise function
				float alpha = ( input_1.x - m_curveElems[i].x ) / ( span );

				if( m_bInvertTerrace ) alpha = 1 - alpha;

				alpha *= alpha;

				float output_1 = lerp( alpha,
									   m_curveElems[i].x,
									   m_curveElems[i+1].x );

				// now the Y value of the Terrace output needs to be in
				// client-rect space for this to plot
				NDPoint P1 = from_graph_space( input_1.x, output_1 );


				dc.MoveTo( j,   P1.y 	);


				/////////////////////////////////////////////////


				// Get the j+1 value into graph space
				NDPoint input_2 = to_graph_space(j+1,y_max);

				// now the terrace value is computed, in graph space
				// in the same way as the libnoise function
				float alpha_plus_1 = ( input_2.x - m_curveElems[i].x ) / ( span );

				if( m_bInvertTerrace ) alpha_plus_1 = 1 - alpha_plus_1;

				alpha_plus_1 *= alpha_plus_1;

				float output_2 = lerp( alpha_plus_1,
									   m_curveElems[i].x,
									   m_curveElems[i+1].x );

				// now the Y value of the Terrace output needs to be in
				// client-rect space for this to plot
				NDPoint P2 = from_graph_space( input_2.x, output_2 );

				// only plot the line if it is within the client rect
				if((P2.y < end_y)
				&& (P2.y > start_y)
				&& (j+1  < end_x)
				&& (j+1  > start_x))
				{
					dc.LineTo( j+1,   P2.y );
				}
			}
		}
	}

	NGraphCtrl::Paint(dc);
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NTerraceCtrl::OnMouseMove(udword flags, NPoint pos )
{
	if( m_bSelectedRect && m_bMouseLDown )
	{

		float moveX =pos.x;
		float moveY = pos.y;
		NDPoint pPoint1 = to_graph_space(moveX, moveY);
		//if( selectedElem->x == x  ) return;

		//if( selectedElem->y == y  ) return;

		if( m_currentLineX < start_x ) m_currentLineX = start_x;
		if( m_currentLineX > start_x + Width ) m_currentLineX = start_x + Width;

		if( m_currentLineY < start_y ) m_currentLineY = start_y;
		if( m_currentLineY > start_y + Height ) m_currentLineY = start_y + Height;

		bMovingPoint = true;
		bool bSwappedPoint = false;

		for( int i = 0 ; i < m_curveElems.Count(); i++ )
		{
			if( m_curveElems[i].x == pPoint1.x )
			{
				if( selectedElem != &m_curveElems[i] )
				{

					float move_size = 1;
					NDPoint pPoint2 = to_graph_space(move_size, move_size);

					if( selectedElem->x >= pPoint1.x )
						pPoint1.x +=pPoint2.x;
					if( selectedElem->x <= pPoint1.x )
						pPoint1.x -=pPoint2.x;

					//NDPoint pPoint2 = from_graph_space(selectedElem->x, selectedElem->y);

					//moveX = pPoint2.x;
					pPoint1.x = selectedElem->x;
					//MessageBox(0,"","",0);
					//bMovingPoint = false;

					break;
				}
			}

		}

		if( bMovingPoint == true )
		{

			NDPoint pPoint2 = from_graph_space(pPoint1.x, pPoint1.y);
			m_currentLineX = pPoint2.x;
			m_currentLineY = pPoint2.x;
			NDPoint pPoint = to_graph_space(m_currentLineX, m_currentLineY);
			selectedElem->x = pPoint1.x;
			selectedElem->y = pPoint1.x;
			bSwappedPoint = true;
		}

		//if( bSwappedPoint )
		//	m_curveElems.Sort(cmp_curve_point2);
	}


}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NTerraceCtrl::AddCurvePoint( float X, float Y )
{
	NDPoint pt = to_graph_space(X,Y);
	pt.y = pt.x;
	//pt.y = Y;

	m_curveElems.AddItem( pt );
	//m_curveElems.Sort(cmp_curve_point2);
}

void NTerraceCtrl::OnDeletePoint()
{
	if( m_curveElems.Size() <= 2 )
		return;

	for( int i = 0; i < m_curveElems.Size(); i++ )
	{
		if( selectedElem == &m_curveElems[i] )
		{
			m_curveElems.RemoveItem(i);
			break;
		}
	}
}
