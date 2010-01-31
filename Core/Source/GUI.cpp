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
#include "../Include/CoreLib.h"
#include "GUI.h"
#include "Font.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Variables
//-----------------------------------------------------------------
NGUISubSystem*			g_pGUI;

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
NGUISubSystem*			GetGUISubSystem()		{ return g_pGUI;}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NGUISubSystem class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NGUISubSystem::NGUISubSystem()
{
	g_pGUI				= this;

	m_pMainWnd		= null;
	m_pFocusedWnd	= null;
	m_pCapturedWnd= null;
	m_pfont				= null;
	m_pOldWndUnderMouse = null;
	m_bMustDrawWindows = false;
	m_ptCursor.x=m_ptCursor.y=0;
	NMemFill(m_keyDown, 0, sizeof(m_keyDown));
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NGUISubSystem::~NGUISubSystem()
{
	//if (m_pMainWnd)	NDELETE(m_pMainWnd, NGUIWnd);
}

//-----------------------------------------------------------------
//!	\brief	Init GUI
//!	\return	true if success
//-----------------------------------------------------------------
bool NGUISubSystem::Init()
{
	InitGLState();
	return true;
}

//-----------------------------------------------------------------
//!	\brief	Shutdown GUI
//!	\return	true if success
//-----------------------------------------------------------------
bool NGUISubSystem::ShutDown()
{
	if (m_pfont)
	{
		m_pfont->clean();
		NDELETE(m_pfont, NGUIFont);
		m_pfont=null;
	}
  m_pMainWnd		= null;
  m_pFocusedWnd	= null;
  m_pCapturedWnd= null;

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Update GUI
//-----------------------------------------------------------------
void NGUISubSystem::Update()
{
	if (m_bMustDrawWindows)
	{
		_RedrawWindowChild(null);
		_RedrawWindowPopup(null);

		glFlush();
		m_bMustDrawWindows=false;
	}

}


//-----------------------------------------------------------------
//!	\brief	Init OpenGL State
//-----------------------------------------------------------------
void NGUISubSystem::InitGLState()
{
	//OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//Font rasterizer
	if (m_pfont==null)
	{
		m_pfont = NNEW(NGUIFont);
	} else {
		m_pfont->clean();
	}
	m_pfont->init("fxfont.ttf", 8);

}

//-----------------------------------------------------------------
//!	\brief	Process Msgs
//-----------------------------------------------------------------
void NGUISubSystem::ProcessMsgs_OnSize(udword _w, udword _h)
{
	NRect rc(0,0,_w,_h);

	glViewport(rc.left, rc.top, rc.Width() , rc.Height()); // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);
	mat4 matProj;
	ortho2D(matProj, 0, rc.Width(), rc.Height(), 0);
	glLoadMatrixf((GLfloat*)&matProj.mat_array);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Send Resize Message
	if (m_pMainWnd)
		m_pMainWnd->SetWindowRect(rc);

}

void NGUISubSystem::ProcessMsgs_MouseButtonDown(NPoint _ptScreen, udword _dwButtons)
{
	NGUIWnd* pwnd=null;
	if (m_pCapturedWnd)		pwnd=m_pCapturedWnd;
	else									pwnd = GetWndUnderMouse(_ptScreen.x, _ptScreen.y);

	if (pwnd)
	{
		pwnd->ScreenToClient(_ptScreen);

		if (_dwButtons==NM_LBUTTON)			pwnd->OnLButtonDown(_ptScreen);
		else if (_dwButtons==NM_MBUTTON)	pwnd->OnMButtonDown(_ptScreen);
		else if (_dwButtons==NM_RBUTTON)	pwnd->OnRButtonDown(_ptScreen);
	}

}

void NGUISubSystem::ProcessMsgs_MouseButtonUp(NPoint _ptScreen, udword _dwButtons)
{
	NGUIWnd* pwnd=null;
	if (m_pCapturedWnd)		pwnd=m_pCapturedWnd;
	else									pwnd = GetWndUnderMouse(_ptScreen.x, _ptScreen.y);

	if (pwnd)
	{
		pwnd->ScreenToClient(_ptScreen);

		if (_dwButtons==NM_LBUTTON)			pwnd->OnLButtonUp(_ptScreen);
		else if (_dwButtons==NM_MBUTTON)	pwnd->OnMButtonUp(_ptScreen);
		else if (_dwButtons==NM_RBUTTON)	pwnd->OnRButtonUp(_ptScreen);
	}

}

void NGUISubSystem::ProcessMsgs_MouseButtonDblClick(NPoint _ptScreen, udword _dwButtons)
{
	NGUIWnd* pwnd=null;
	if (m_pCapturedWnd)		pwnd=m_pCapturedWnd;
	else									pwnd = GetWndUnderMouse(_ptScreen.x, _ptScreen.y);

	if (pwnd)
	{
		pwnd->ScreenToClient(_ptScreen);

		if (_dwButtons==NM_LBUTTON)			pwnd->OnLButtonDblClk(_ptScreen);
		//else if (_dwButtons==NM_MBUTTON)	pwnd->OnMButtonUp(_ptScreen);
		//else if (_dwButtons==NR_RBUTTON)	pwnd->OnRButtonUp(_ptScreen);
	}

}

void NGUISubSystem::ProcessMsgs_MouseWheel(NPoint _ptScreen, sdword _dwDelta)
{
	NGUIWnd* pwnd=null;
	if (m_pCapturedWnd)	pwnd = m_pCapturedWnd;
	else								pwnd = GetWndUnderMouse(_ptScreen.x, _ptScreen.y);

	if (pwnd)
	{
		pwnd->ScreenToClient(_ptScreen);
		pwnd->OnMouseWheel(_ptScreen, _dwDelta);
	}

}


void NGUISubSystem::ProcessMsgs_MouseMove(NPoint _ptScreen)
{
	m_ptCursor = _ptScreen;

	NGUIWnd* pwnd=null;
	if (m_pCapturedWnd)		pwnd = m_pCapturedWnd;
	else									pwnd = GetWndUnderMouse(_ptScreen.x, _ptScreen.y);
	if (pwnd)
	{
		pwnd->ScreenToClient(_ptScreen);
		pwnd->OnMouseMove(_ptScreen);
	}
	if (m_pOldWndUnderMouse && pwnd!=m_pOldWndUnderMouse)
		m_pOldWndUnderMouse->OnMouseLeave();

	m_pOldWndUnderMouse = pwnd;
}

void NGUISubSystem::ProcessMsgs_Text(udword _unicode)
{
	NGUIWnd* pwnd=GetFocus();
	if (pwnd)	pwnd->OnText(_unicode);
}

void NGUISubSystem::ProcessMsgs_KeyDown(NKey::Code _key)
{
	m_keyDown[_key]	= true;
	NGUIWnd* pwnd=GetFocus();
	if (pwnd)	pwnd->OnKeyDown(_key);
}

void NGUISubSystem::ProcessMsgs_KeyUp(NKey::Code _key)
{
	m_keyDown[_key]	= false;
	NGUIWnd* pwnd=GetFocus();
	if (pwnd)	pwnd->OnKeyUp(_key);
}

void NGUISubSystem::NotifyWindowDeletion(NGUIWnd* _pWnd)
{
	if (m_pFocusedWnd==_pWnd)
		m_pFocusedWnd = null;

	if (m_pOldWndUnderMouse==_pWnd)
		m_pOldWndUnderMouse = null;
}

//-----------------------------------------------------------------
//!	\brief	Return window under mouse cursor
//-----------------------------------------------------------------
NGUIWnd* NGUISubSystem::GetWndUnderMouse(sdword _x, sdword _y)
{
	NGUIWnd* pwnd = _GetPopupWndUnderMouse(_x, _y, m_pMainWnd);
	if (pwnd==null)
		pwnd=_GetWndUnderMouse(_x, _y, m_pMainWnd);

	return pwnd;
}

NGUIWnd* NGUISubSystem::_GetWndUnderMouse(sdword _x, sdword _y, NGUIWnd* _pwndParent)
{
	NGUIWnd* pwndFind=null;
	NPoint pt(_x, _y);
	for (udword i=0 ;i<_pwndParent->m_arrayChildren.Count(); i++)
	{
		NGUIWnd* pwnd = (NGUIWnd*)_pwndParent->m_arrayChildren[i];

		pwndFind = _GetWndUnderMouse(_x,_y,pwnd);
		if (pwndFind)
			break;

		NRect rc = pwnd->GetScreenRect();
		if (pwnd->IsWindowVisible() && rc.Contain(pt))
		{
			pwndFind = pwnd;
			break;
		}

	}

	return pwndFind;
}


NGUIWnd* NGUISubSystem::_GetPopupWndUnderMouse(sdword _x, sdword _y, NGUIWnd* _pwndParent)
{
	NGUIWnd* pwndFind=null;
	NPoint pt(_x, _y);
	for (udword i=0 ;i<_pwndParent->m_arrayChildren.Count(); i++)
	{
		NGUIWnd* pwnd = (NGUIWnd*)_pwndParent->m_arrayChildren[i];

		pwndFind = _GetPopupWndUnderMouse(_x,_y,pwnd);
		if (pwndFind)
			break;

		NRect rc = pwnd->GetScreenRect();
		if (pwnd->IsWindowVisible() && (pwnd->GetStyle()&NWS_POPUP) && rc.Contain(pt))
		{
			pwndFind = pwnd;
			break;
		}

	}

	return pwndFind;
}


//-----------------------------------------------------------------
//!	\brief	Refresh Window Display with children
//-----------------------------------------------------------------
void NGUISubSystem::RedrawWindow(NGUIWnd* _pwndFrom)
{
	m_bMustDrawWindows = true;
}

void NGUISubSystem::_RedrawWindowChild(NGUIWnd* _pwndFrom)
{

	NGUIWnd* pWnd = _pwndFrom;
	if (pWnd==null)
		pWnd = m_pMainWnd;

	//Display Main Window
	if (pWnd->IsWindowVisible())
	{
		//TRACE("Paint Wnd -> %s\n", pWnd->GetText() );
		pWnd->OnPaint();
	}

	//Send to Children without popup style
	for (udword i=0 ;i<pWnd->m_arrayChildren.Count(); i++)
	{
		NGUIWnd* pChild = (NGUIWnd*)pWnd->m_arrayChildren[i];
		if (pChild->IsWindowVisible() /*&& !(pChild->GetStyle()&NWS_POPUP)*/ )
			_RedrawWindowChild(pChild);
	}

}

void NGUISubSystem::_RedrawWindowPopup(NGUIWnd* _pwndFrom)
{

	NGUIWnd* pWnd = _pwndFrom;
	if (pWnd==null)
		pWnd = m_pMainWnd;

	//Display Main Window
	if (pWnd->IsWindowVisible() && (pWnd->GetStyle()&NWS_POPUP))
	{
		//TRACE("Paint Wnd -> %s\n", pWnd->GetText() );
		pWnd->OnPaint();
	}

	//Send to Children with popup style
	for (udword i=0 ;i<pWnd->m_arrayChildren.Count(); i++)
	{
		NGUIWnd* pChild = (NGUIWnd*)pWnd->m_arrayChildren[i];
		if (pChild->IsWindowVisible() /*&& (pChild->GetStyle()&NWS_POPUP)*/)
		{
			//TRACE("Paint Popup -> %s\n", pChild->GetText() );
			_RedrawWindowPopup(pChild);
		}
	}

}

//-----------------------------------------------------------------
//!	\brief	Change focused Window
//-----------------------------------------------------------------
void NGUISubSystem::SetFocus(NGUIWnd* _pwnd)
{
	//Events kill focus
	if (m_pFocusedWnd)
		m_pFocusedWnd->OnKillFocus(_pwnd);

	m_pFocusedWnd = _pwnd;
}

//-----------------------------------------------------------------
//!	\brief	Return focused Window
//-----------------------------------------------------------------
NGUIWnd* NGUISubSystem::GetFocus()
{
	return m_pFocusedWnd;
}

//-----------------------------------------------------------------
//!	\brief	Change captured Window
//-----------------------------------------------------------------
void NGUISubSystem::SetCapture(NGUIWnd* _pwnd)
{
	m_pCapturedWnd = _pwnd;
	//CaptureMouse();
}

//-----------------------------------------------------------------
//!	\brief	Release captured Window
//-----------------------------------------------------------------
void NGUISubSystem::ReleaseCapture()
{
	//ReleaseMouse();
	m_pCapturedWnd=null;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					N2DPainter class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
N2DPainter::N2DPainter(NGUIWnd* pwnd)
{
	m_pwnd = pwnd;

	glPushMatrix();

	//udword h = SDL_GetVideoSurface()->h;*
	udword h=0; //###TOFIX###
	if (GetGUISubSystem()->GetMainWnd())
	{
		NRect rcM = GetGUISubSystem()->GetMainWnd()->GetClientRect();
		h=rcM.Height();
	}

	NRect rc = pwnd->GetScreenRect();
	//TRACE("%s %dx%d-%dx%d %d\n", m_pwnd->GetText(), rc.left, rc.top, rc.right, rc.bottom);
	//TRACE("h %d\n", h);

	glViewport(rc.left, h - (rc.top+rc.Height()), rc.Width() , rc.Height()); // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();

	mat4 matProj;
	ortho2D(matProj, 0, rc.Width(), rc.Height(), 0);
	glLoadMatrixf((GLfloat*)&matProj.mat_array);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Background
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
N2DPainter::~N2DPainter()
{
	//glDisable(GL_SCISSOR_TEST);
	glPopMatrix();

}

void N2DPainter::FillSolidRect(NRect& rc, NColor _clr)
{
	NRect rcW = m_pwnd->GetWindowRect();

	NRect rc2 = rc;
	//rc2.Move(rcW.left, rcW.top);

	glColor4ub(_clr.GetR(), _clr.GetG(), _clr.GetB(), _clr.GetA());
	glRecti(rc2.left,rc2.top,rc2.right,rc2.bottom);
}

void N2DPainter::Draw3dRect(NRect& rc, NColor clrTopLeft, NColor clrBottomRight)
{
	DrawLine(rc.left, rc.top, rc.right-1, rc.top+1, clrTopLeft, 1);
	DrawLine(rc.left, rc.top, rc.left+1, rc.bottom-1, clrTopLeft, 1);

	DrawLine(rc.right, rc.top, rc.right-1, rc.bottom, clrBottomRight, 1);
	DrawLine(rc.left, rc.bottom, rc.right, rc.bottom-1, clrBottomRight, 1);
}

void N2DPainter::GradientVRect(NRect& rc, NColor clrStart, NColor clrEnd)
{

	glBegin(GL_QUADS);
	glColor4ub(clrStart.GetR(), clrStart.GetG(), clrStart.GetB(), clrStart.GetA());
	glVertex2i(rc.left,rc.top);
	glColor4ub(clrEnd.GetR(), clrEnd.GetG(), clrEnd.GetB(), clrEnd.GetA());
	glVertex2i(rc.left,rc.bottom);
	glColor4ub(clrEnd.GetR(), clrEnd.GetG(), clrEnd.GetB(), clrEnd.GetA());
	glVertex2i(rc.right,rc.bottom);
	glColor4ub(clrStart.GetR(), clrStart.GetG(), clrStart.GetB(), clrStart.GetA());
	glVertex2i(rc.right,rc.top);
	glEnd();
}

void N2DPainter::RoundRect(ubyte _roundboxtype, NRect& _rc, float _rad, NColor _clr)
{
	//Code From Blender :-)
	float vec[7][2]= {{0.195f, 0.02f}, {0.383f, 0.067f}, {0.55f, 0.169f}, {0.707f, 0.293f},
	                  {0.831f, 0.45f}, {0.924f, 0.617f}, {0.98f, 0.805f}};
	int a;
	int i=0;
	NPoint pts[256];

	float _minx, _miny, _maxx, _maxy;
	_minx=_rc.left; _miny=_rc.top, _maxx=_rc.right, _maxy=_rc.bottom;

	// mult
	for(a=0; a<7; a++) {
		vec[a][0]*= _rad; vec[a][1]*= _rad;
	}

	// start with corner right-bottom
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

	// corner right-top
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

	// corner left-top
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

	// corner left-bottom
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

	Polygon(pts, i, _clr);

}



void N2DPainter::DrawLine(sdword _x1, sdword _y1, sdword _x2, sdword _y2, NColor _clr, udword _dwWidth)
{
	NRect rcW = m_pwnd->GetWindowRect();

	glColor4ub(_clr.GetR(), _clr.GetG(), _clr.GetB(), _clr.GetA());
	glLineWidth(_dwWidth);
	glBegin(GL_LINES);
	glVertex2i(_x1,_y1);
	glVertex2i(_x2,_y2);
	glEnd();
}

sdword N2DPainter::DrawString(const char* _pszString, NRect& _rc, udword _nFormat, NColor _clr)
{
	sdword dwposX=0, dwposY=0;

	if (_nFormat&NDT_HCENTER)
	{
		sdword h = GetGUISubSystem()->GetFont()->m_h-1;
		sdword l = strlen(_pszString);
		dwposX=(_rc.Width()/2) - ((l*h)/2);
	}
	if (_nFormat&NDT_VCENTER)
	{
		sdword h = GetGUISubSystem()->GetFont()->m_h;
		dwposY=(_rc.Height()/2) - (h/2);
	}

	glColor4ub(_clr.GetR(), _clr.GetG(), _clr.GetB(), _clr.GetA());
	glPushMatrix();
	glLoadIdentity();
	glRotatef(180,1,0,0);
	glTranslatef(dwposX,-9-dwposY,0);
	GetGUISubSystem()->GetFont()->print(_rc.left,_rc.top, _pszString);
	glPopMatrix();

	return 0;
}

void N2DPainter::Polygon(NPoint* _ptsArray, udword _dwCount, NColor _clr)
{
	glColor4ub(_clr.GetR(), _clr.GetG(), _clr.GetB(), _clr.GetA());
	glBegin(GL_POLYGON);
	for (udword i=0; i<_dwCount; i++)
		glVertex2i(_ptsArray[i].x, _ptsArray[i].y);
	glEnd();
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NGUIWnd class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NGUIWnd::NGUIWnd()
{
	m_pParentWnd	= null;
	m_dwStyle			= 0;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NGUIWnd::~NGUIWnd()
{
	GetGUISubSystem()->NotifyWindowDeletion(this);

	//Remove this windows from parent's children array...
	if (m_pParentWnd)
	{
		udword idx = m_pParentWnd->m_arrayChildren.Find(this);
		if (idx!=(udword)-1)
			m_pParentWnd->m_arrayChildren.RemoveItem(idx);
	}
}

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NGUIWnd::Create(NWNDCREATE &c)
{
	//Save Creation struct datas
	m_pParentWnd	= c.pwndParent;
	m_cstrText		= c.pszText;
	m_rcWnd				= c.rcRect;
	m_dwStyle			= c.dwStyle;

	//Add This Wnd at parent's child list
	NGUIWnd* parent = c.pwndParent;
	if (parent)		((NGUIWnd*)parent)->m_arrayChildren.AddItem((NGUIWnd*)this);

	if (m_pParentWnd)
	{
		m_rcWnd.Move(m_pParentWnd->m_rcWnd.left, m_pParentWnd->m_rcWnd.top);
	}

	return true;
}

//-----------------------------------------------------------------
// Name:	GetClientRect()
// Desc:	...
//-----------------------------------------------------------------
NRect NGUIWnd::GetClientRect()
{
	NRect rect(0,0,m_rcWnd.Width(), m_rcWnd.Height());
	return rect;
}

//-----------------------------------------------------------------
// Name:	GetWindowRect()
// Desc:	...
//-----------------------------------------------------------------
NRect NGUIWnd::GetWindowRect()
{
	NRect rc(m_rcWnd);
	//Convert Screen to window Rect
	// so Substract parent screen pos
	if (m_pParentWnd)
	{
		rc.Move(-m_pParentWnd->m_rcWnd.left, -m_pParentWnd->m_rcWnd.top);
	}

	return rc;
}


//-----------------------------------------------------------------
// Name:	SetWindowRect()
// Desc:	Rect relative to parent
//-----------------------------------------------------------------
void NGUIWnd::SetWindowRect(const NRect& r)
{
	//Convert in Screen Rect
	// so Add parent screen pos
	m_rcWnd = r;
	if (m_pParentWnd)
	{
		m_rcWnd.Move(m_pParentWnd->m_rcWnd.left, m_pParentWnd->m_rcWnd.top);
	}

	OnSize();
	RedrawWindow();
}

void NGUIWnd::ScreenToClient(NPoint& _pt)
{
	_pt.x-= m_rcWnd.left;
	_pt.y-= m_rcWnd.top;
}

void NGUIWnd::ClientToScreen(NPoint& _pt)
{
	_pt.x+= m_rcWnd.left;
	_pt.y+= m_rcWnd.top;
}

void NGUIWnd::SetFocus()
{
	GetGUISubSystem()->SetFocus(this);
}

NGUIWnd* NGUIWnd::GetFocus()
{
	return GetGUISubSystem()->GetFocus();
}


void NGUIWnd::SetCapture()
{
	GetGUISubSystem()->SetCapture(this);
}

void NGUIWnd::ReleaseCapture()
{
	GetGUISubSystem()->ReleaseCapture();
}

void NGUIWnd::SetText(char* _pszText)
{
	m_cstrText = _pszText;
}

NString NGUIWnd::GetText()
{
	return m_cstrText;
}


void NGUIWnd::RedrawWindow()
{
	GetGUISubSystem()->RedrawWindow(this);
}

void NGUIWnd::ShowWindow(bool _bVisible)
{
	if ((m_dwStyle&NWS_VISIBLE)!=_bVisible)
	{
		_bVisible?m_dwStyle|=NWS_VISIBLE:m_dwStyle&=~NWS_VISIBLE;
		GetGUISubSystem()->RedrawWindow(null);
	}
}
