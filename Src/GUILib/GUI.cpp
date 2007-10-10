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

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define WM_MOUSEWHEEL                   0x020A

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


sdword NGraphics::DrawText(char* szString, NRect& rc, udword nFormat)
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

bool NWnd::IsWindowVisible()
{
	return ::IsWindowVisible(m_W32HWnd)!=0;
}

//-----------------------------------------------------------------
// Name:	Windows Proc()
// Desc:	...
//-----------------------------------------------------------------
LRESULT CALLBACK NWnd::StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//Get NWnd Class pointer
	NWnd* nwnd = (NWnd*)::GetWindowLong(hwnd, GWL_USERDATA);
	if (nwnd==null)											return (::DefWindowProc(hwnd, msg, wparam, lparam));

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
