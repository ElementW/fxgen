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
#include "..\pch.h"
#include "SplitWnd.h"
#include "GUI.h"
#include "Font.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Variables
//-----------------------------------------------------------------
NbaseApplication*			g_pApp;

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
NbaseApplication*			GetApp()		{ return g_pApp;}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NbaseApplication class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NbaseApplication::NbaseApplication()
{
	g_pApp				= this;

	m_pMainWnd		= null;
	m_pFocusedWnd	= null;
	m_pCapturedWnd= null;
	m_pfont				= null;
	m_pOldWndUnderMouse = null;
	m_bMustDrawWindows = false;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NbaseApplication::~NbaseApplication()
{
	if (m_pMainWnd)	delete (m_pMainWnd);

}

//-----------------------------------------------------------------
//!	\brief	Main loop
//-----------------------------------------------------------------
void NbaseApplication::Run()
{
	if ( m_pMainWnd == NULL ) {
		//Haaa ! No main window ! Exit !!!
		Exit();
	}

}

//-----------------------------------------------------------------
//!	\brief	Exit application
//!	\return	true if success
//-----------------------------------------------------------------
bool NbaseApplication::Exit()
{
	if (m_pfont)
	{
		m_pfont->clean();
		delete m_pfont;
		m_pfont=null;
	}

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Ask exit
//-----------------------------------------------------------------
/*void NbaseApplication::AskExit()
{
	PostQuitMessage(0);
}*/

//-----------------------------------------------------------------
//!	\brief	Init OpenGL State
//-----------------------------------------------------------------
void NbaseApplication::InitGLState()
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
		m_pfont = new NGUIFont();
	else
		m_pfont->clean();

	m_pfont->init("fxfont.ttf", 8);

}

//-----------------------------------------------------------------
//!	\brief	Process Msgs
//-----------------------------------------------------------------
void NbaseApplication::ProcessMsgs_OnSize(udword _w, udword _h)
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

//-----------------------------------------------------------------
//!	\brief	Return window under mouse cursor
//-----------------------------------------------------------------
NWnd* NbaseApplication::GetWndUnderMouse(sdword _x, sdword _y)
{
	NWnd* pwnd = _GetPopupWndUnderMouse(_x, _y, m_pMainWnd);
	if (pwnd==null)
		pwnd=_GetWndUnderMouse(_x, _y, m_pMainWnd);

	return pwnd;
}

NWnd* NbaseApplication::_GetWndUnderMouse(sdword _x, sdword _y, NWnd* _pwndParent)
{
	NWnd* pwndFind=null;
	NPoint pt(_x, _y);
	for (udword i=0 ;i<_pwndParent->m_arrayChildren.Count(); i++)
	{
		NWnd* pwnd = (NWnd*)_pwndParent->m_arrayChildren[i];

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


NWnd* NbaseApplication::_GetPopupWndUnderMouse(sdword _x, sdword _y, NWnd* _pwndParent)
{
	NWnd* pwndFind=null;
	NPoint pt(_x, _y);
	for (udword i=0 ;i<_pwndParent->m_arrayChildren.Count(); i++)
	{
		NWnd* pwnd = (NWnd*)_pwndParent->m_arrayChildren[i];

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
void NbaseApplication::RedrawWindow(NWnd* _pwndFrom)
{
	m_bMustDrawWindows = true;
}

void NbaseApplication::_RedrawWindowChild(NWnd* _pwndFrom)
{

	NWnd* pWnd = _pwndFrom;
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
		NWnd* pChild = (NWnd*)pWnd->m_arrayChildren[i];
		if (pChild->IsWindowVisible() /*&& !(pChild->GetStyle()&NWS_POPUP)*/ )
			_RedrawWindowChild(pChild);
	}

}

void NbaseApplication::_RedrawWindowPopup(NWnd* _pwndFrom)
{

	NWnd* pWnd = _pwndFrom;
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
		NWnd* pChild = (NWnd*)pWnd->m_arrayChildren[i];
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
void NbaseApplication::SetFocus(NWnd* _pwnd)
{
	//Events kill focus
	if (m_pFocusedWnd)
		m_pFocusedWnd->OnKillFocus(_pwnd);

	m_pFocusedWnd = _pwnd;
}

//-----------------------------------------------------------------
//!	\brief	Return focused Window
//-----------------------------------------------------------------
NWnd* NbaseApplication::GetFocus()
{
	return m_pFocusedWnd;
}

//-----------------------------------------------------------------
//!	\brief	Change captured Window
//-----------------------------------------------------------------
void NbaseApplication::SetCapture(NWnd* _pwnd)
{
	m_pCapturedWnd = _pwnd;
}

//-----------------------------------------------------------------
//!	\brief	Release captured Window
//-----------------------------------------------------------------
void NbaseApplication::ReleaseCapture()
{
	m_pCapturedWnd=null;
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
	m_pwnd = pwnd;

	glPushMatrix();

	//udword h = SDL_GetVideoSurface()->h;*
	udword h=0; //###TOFIX###
	if (GetApp()->GetMainWnd())
	{
		NRect rcM = GetApp()->GetMainWnd()->GetClientRect();
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
NGraphics::~NGraphics()
{
	//glDisable(GL_SCISSOR_TEST);
	glPopMatrix();

}

void NGraphics::FillSolidRect(NRect& rc, NColor clr)
{
	NRect rcW = m_pwnd->GetWindowRect();
	
	NRect rc2 = rc;
	//rc2.Move(rcW.left, rcW.top);

	glColor4ub(clr.GetR(), clr.GetG(), clr.GetB(), clr.GetA());
	glRecti(rc2.left,rc2.top,rc2.right,rc2.bottom);
}

void NGraphics::Draw3dRect(NRect& rc, NColor clrTopLeft, NColor clrBottomRight)
{
	DrawLine(rc.left, rc.top, rc.right-1, rc.top+1, clrTopLeft, 1);
	DrawLine(rc.left, rc.top, rc.left+1, rc.bottom-1, clrTopLeft, 1);

	DrawLine(rc.right, rc.top, rc.right-1, rc.bottom, clrBottomRight, 1);
	DrawLine(rc.left, rc.bottom, rc.right, rc.bottom-1, clrBottomRight, 1);
}

void NGraphics::GradientVRect(NRect& rc, NColor clrStart, NColor clrEnd)
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

void NGraphics::RoundRect(ubyte _roundboxtype, NRect& _rc, float _rad, NColor _clr)
{
	//Code From Blender :-)
	float vec[7][2]= {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
	                  {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
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



void NGraphics::DrawLine(sdword _x1, sdword _y1, sdword _x2, sdword _y2, NColor _clr, udword _dwWidth)
{
	NRect rcW = m_pwnd->GetWindowRect();

	glColor4ub(_clr.GetR(), _clr.GetG(), _clr.GetB(), _clr.GetA());
	glLineWidth(_dwWidth);
	glBegin(GL_LINES);
	glVertex2i(_x1,_y1);
	glVertex2i(_x2,_y2);
	glEnd();
}

sdword NGraphics::DrawText(const char* _pszString, NRect& _rc, udword _nFormat, NColor _clr)
{
	sdword dwposX=0, dwposY=0;
	
	if (_nFormat&NDT_HCENTER)
	{
		sdword h = GetApp()->GetFont()->m_h-1;
		sdword l = strlen(_pszString);
		dwposX=(_rc.Width()/2) - ((l*h)/2);
	}
	if (_nFormat&NDT_VCENTER)
	{
		sdword h = GetApp()->GetFont()->m_h;
		dwposY=(_rc.Height()/2) - (h/2);
	}

	glColor4ub(_clr.GetR(), _clr.GetG(), _clr.GetB(), _clr.GetA());
	glPushMatrix();
	glLoadIdentity();
	glRotatef(180,1,0,0);
	glTranslatef(dwposX,-9-dwposY,0);
	GetApp()->GetFont()->print(_rc.left,_rc.top, _pszString);
	glPopMatrix();

	return 0;
}

void NGraphics::Polygon(NPoint* _ptsArray, udword _dwCount, NColor _clr)
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
	m_dwStyle			= 0;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NWnd::~NWnd()
{
	//###TODO### remove from children list...
	if (m_pParentWnd)
	{
		udword idx = m_pParentWnd->m_arrayChildren.Find(this);
		m_pParentWnd->m_arrayChildren.RemoveItem(idx);
	}
}

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NWnd::Create(NWNDCREATE &c)
{
	//Save Creation struct datas
	m_pParentWnd	= c.pwndParent;
	m_cstrText		= c.pszText;
	m_rcWnd				= c.rcRect;
	m_dwStyle			= c.dwStyle;

	//Add This control at parent's control list
	NWnd* parent = c.pwndParent;
	if (parent)		((NWControl*)parent)->m_arrayChildren.AddItem((NWnd*)this);

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
NRect NWnd::GetClientRect()
{
	NRect rect(0,0,m_rcWnd.Width(), m_rcWnd.Height());
	return rect;
}

//-----------------------------------------------------------------
// Name:	GetWindowRect()
// Desc:	...
//-----------------------------------------------------------------
NRect NWnd::GetWindowRect()
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
void NWnd::SetWindowRect(const NRect& r)
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

void NWnd::ScreenToClient(NPoint& _pt)
{
	_pt.x-= m_rcWnd.left;
	_pt.y-= m_rcWnd.top;
}

void NWnd::ClientToScreen(NPoint& _pt)
{
	_pt.x+= m_rcWnd.left;
	_pt.y+= m_rcWnd.top;
}

void NWnd::SetFocus()
{
	GetApp()->SetFocus(this);
}

NWnd* NWnd::GetFocus()
{
	return GetApp()->GetFocus();
}


void NWnd::SetCapture()
{
	GetApp()->SetCapture(this);
}

void NWnd::ReleaseCapture()
{
	GetApp()->ReleaseCapture();
}

void NWnd::SetText(char* _pszText)
{
	m_cstrText = _pszText;
}

NString NWnd::GetText()
{
	return m_cstrText;
}


void NWnd::RedrawWindow()
{
	GetApp()->RedrawWindow(this);
}

void NWnd::ShowWindow(bool _bVisible)
{
	if ((m_dwStyle&NWS_VISIBLE)!=_bVisible)
	{
		_bVisible?m_dwStyle|=NWS_VISIBLE:m_dwStyle&=~NWS_VISIBLE;
		GetApp()->RedrawWindow(null);
	}
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
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NFrmWnd::~NFrmWnd()
{
	if (mWorkspace)		delete mWorkspace;

}

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NFrmWnd::Create(char* name, const NRect& rect)
{
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= name;
	wc.pwndParent		= null;
	wc.rcRect				= rect;
	wc.dwStyle			= NWS_VISIBLE;
	NWnd::Create(wc);

	//Create workspace
	NRect r = GetClientRect();
	NSplitWnd* splitwnd = new NSplitWnd;
	splitwnd->Create("wks", r, this);

	mWorkspace = splitwnd;

	//OnSize();

	return true;
}

//-----------------------------------------------------------------
// Name:	OnSize()
// Desc:	...
//-----------------------------------------------------------------
void NFrmWnd::OnSize()
{
	NRect rc = GetClientRect();

	//GetApp()->ProcessMsgs_OnSize(r.Width(), r.Height());

	//Resize workspace
	if (mWorkspace)		mWorkspace->SetWindowRect(rc);

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

	return NWnd::Create(c);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					File Dialog Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Name:	GetFileNumber()
// Desc:	...
//-----------------------------------------------------------------
udword NbaseFileDialog::GetFileNumber()
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
NString NbaseFileDialog::GetPathName(udword Index) const
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

