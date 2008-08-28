//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		StandardsCtrl.cpp
//! \brief		Standard GUI controls
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		21-11-2007
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
#include "StandardsCtrl.h"
#include "Font.h"

#ifdef __GNUC__
#include <limits.h>
#endif

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NTextCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NTextCtrl::NTextCtrl()
{
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NTextCtrl::~NTextCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NTextCtrl::Create(const char* name, const NRect& rect, NWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= (char*)name;
	wc.pwndParent		= parent;
	wc.rcRect				= rect;
	wc.dwStyle			= NWS_VISIBLE;
	NWControl::Create(wc);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
void NTextCtrl::Update()
{
	RedrawWindow();
	//OnPaint();
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NTextCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.FillSolidRect(rc, RGBA(192,192,192,255));
	dc.DrawText(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE|NDT_HCENTER, RGBA(0,0,0,255) );
}

//-----------------------------------------------------------------
//!	\brief	Sizing
//-----------------------------------------------------------------
void NTextCtrl::OnSize()
{
	//Redraw
	Update();
	//###GUI###
	//::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NButtonCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NButtonCtrl::NButtonCtrl()
{
	m_bClicked		= false;
	m_bMouseOver	= false;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NButtonCtrl::~NButtonCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NButtonCtrl::Create(const char* name, const NRect& rect, NWnd* parent, udword _dwStyle)
{
	//Bouton style
	m_dwStyle = _dwStyle;

	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= const_cast<char*>(name);
	wc.pwndParent		= parent;
	wc.rcRect				= rect;
	wc.dwStyle			= NWS_VISIBLE;
	NWControl::Create(wc);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
void NButtonCtrl::Update()
{
	RedrawWindow();
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NButtonCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);

	///////////////////////////////////////////////////////////
	//Normal Button
	if (m_dwStyle==NBUT_STYLE_PUSH)
	{
		if (!m_bClicked)
		{
			dc.GradientVRect(rc, RGBA(255, 220, 220, 220), RGBA(120, 120, 120,255));

			if (m_bMouseOver)			dc.Draw3dRect(rc, RGBA(205,194,14,255), RGBA(205,194,14,255));
			else									dc.Draw3dRect(rc, RGBA(255,255,255,255), RGBA(0,0,0,255));

			dc.DrawText(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE|NDT_HCENTER, RGBA(0,0,0,255) );
		} else {
			dc.GradientVRect(rc, RGBA(120, 120, 120,255), RGBA(220, 220, 220,255));

			if (m_bMouseOver)			dc.Draw3dRect(rc, RGBA(205,194,14,255), RGBA(205,194,14,255));
			else									dc.Draw3dRect(rc, RGBA(0,0,0,255), RGBA(255,255,255,255));

			rc.Move(1,1);
			dc.DrawText(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE|NDT_HCENTER, RGBA(0,0,0,255) );
		}

	///////////////////////////////////////////////////////////
	//Menu Button
	} else if (m_dwStyle==NBUT_STYLE_MENU) {
		NColor clr;
		if (m_bMouseOver)			{ dc.RoundRect(0xF, rc, 0, RGBA(64,64,64,255)); clr=0xFFFFFFFF; }
		else									{ clr=0xFF000000; }
		dc.DrawText(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE|NDT_HCENTER, clr );
	}


}


void NButtonCtrl::OnSize()
{
	Update();
}


void NButtonCtrl::OnLeftButtonDown(udword flags, NPoint _point)
{
	//TRACE("OnLeftButtonDown\n");
	m_bClicked = true;
	Update();
}

void NButtonCtrl::OnLeftButtonUp(udword flags, NPoint point)
{
	if (m_bClicked)
		OnClick(this);

	m_bClicked = false;
	Update();
}

void NButtonCtrl::OnMouseLeave()
{
	m_bClicked		= false;
	m_bMouseOver	= false;
	Update();
}


void NButtonCtrl::OnLeftButtonDblClk(udword flags, NPoint point)
{
}


void NButtonCtrl::OnRightButtonDown(udword flags, NPoint point)
{
}

void NButtonCtrl::OnMouseMove(udword flags, NPoint point )
{
	m_bMouseOver = true;
}


void NButtonCtrl::OnMButtonDown(udword flags, NPoint pos)
{
}

void NButtonCtrl::OnMButtonUp(udword flags, NPoint pos)
{
}


void NButtonCtrl::OnKeyDown(udword dwchar)
{
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NColorPickerCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NColorPickerCtrl::NColorPickerCtrl()
{
	m_fHue=0.0f;
	m_fL=0.0f;
	m_fS=0.0f;
	m_bPickedLS=false;
	m_bPickedHue=false;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NColorPickerCtrl::~NColorPickerCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NColorPickerCtrl::Create(NWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= "";
	wc.pwndParent		= parent;
	wc.rcRect				= NRect(0,0,0,0);
	wc.dwStyle			= NWS_POPUP;
	NWControl::Create(wc);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Display menu at screen position
//!	\return	Pointer to selected Item Desc
//-----------------------------------------------------------------
void NColorPickerCtrl::TrackPopup(NPoint _ptScreen, const NColor& _clr)
{
	TRACE("NColorPickerCtrl::TrackPopup\n");

	m_clr = _clr;
	m_clr.ToHLS(m_fHue, m_fL, m_fS);

	NRect rcApp = GetApp()->GetMainWnd()->GetWindowRect();
	if (_ptScreen.x + 160 > rcApp.right)
	{
		_ptScreen.x=rcApp.right-160;
	}

	//Calc ColorPicker rect
	NRect rc(0,0,160,128);

	if (m_pParentWnd)
		m_pParentWnd->ScreenToClient(_ptScreen);

	rc.Move(_ptScreen);
	SetWindowRect(rc);

	SetFocus();

	ShowWindow(true);

	//Graphic update
	RedrawWindow();

}


//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NColorPickerCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics gfx(this);
	gfx.FillSolidRect(rc, RGBA(255,255,255,190));

	//Hue Rect
	m_rcHue.left=0;
	m_rcHue.top=110;
	m_rcHue.right=rc.Width();
	m_rcHue.bottom=m_rcHue.top+16;

	//Display Hue
	udword x, y;
	udword px,py;
	NColor clr(0xFF000000);
	for (x=m_rcHue.left, px=0; x<m_rcHue.right; x++, px++)
	{
		float h=px*360.0f/(float)m_rcHue.Width();
		clr.SetFromHLS(h,0.5f,0.5f);
		gfx.DrawLine(x,m_rcHue.top,x,m_rcHue.bottom, clr,1);
	}

	//LS Rect
	m_rcLS.left=0;
	m_rcLS.top=4;
	m_rcLS.right=m_rcLS.left+100;
	m_rcLS.bottom=m_rcLS.top+100;

	//Display Color (L,S 0.0 > 1.0)
	px=0;
	float l, s;
	for (x=m_rcLS.left; x<m_rcLS.right; x++, px++)
	{
		py=0;
		s=px*1.0f/100.0f;
		for (y=m_rcLS.top; y<m_rcLS.bottom; y++, py++)
		{
			l=py*1.0f/100.0f;
			clr.SetFromHLS(m_fHue, l, s);
			gfx.DrawLine(x,y,x+1,y+1, clr, 1);

		}
	}

	//Display Values
	NRect rcText(rc);
	rcText.left = m_rcLS.right;

	NString cstr;

	cstr.Format("H: %f", m_fHue);
	gfx.DrawText(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );
	cstr.Format("L: %f", m_fL); rcText.top+=10;
	gfx.DrawText(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );
	cstr.Format("S: %f", m_fS); rcText.top+=10;
	gfx.DrawText(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );


	rcText.top+=20;
	cstr.Format("R: %d", m_clr.GetR() );
	gfx.DrawText(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );
	cstr.Format("G: %d", m_clr.GetG()); rcText.top+=10;
	gfx.DrawText(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );
	cstr.Format("B: %d", m_clr.GetB()); rcText.top+=10;
	gfx.DrawText(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );

	//Hue Cursors v
	//						^
	NPoint pts[3];
	px=m_rcHue.left + (m_fHue*(float)m_rcHue.Width()/360.0f);
	py=m_rcHue.top;
	pts[0].x = px-4;	pts[0].y = py;
	pts[1].x = px+4;	pts[1].y = py;
	pts[2].x = px;		pts[2].y = py+4;
	gfx.Polygon(pts, 3, RGBA(0,0,0,255));

	py=m_rcHue.bottom;
	pts[0].y = py;
	pts[1].y = py;
	pts[2].y = py-4;
	gfx.Polygon(pts, 3, RGBA(0,0,0,255));

	//LS Cursors
	NRect rcC;
	px=m_rcLS.left + (m_fS*100.0f);
	py=m_rcLS.top  + (m_fL*100.0f);
	gfx.DrawLine(m_rcLS.left, py, m_rcLS.right, py, NColor(0,0,0,100), 1);
	gfx.DrawLine(px, m_rcLS.top, px, m_rcLS.bottom, NColor(0,0,0,100), 1);

}



void NColorPickerCtrl::OnMouseMove(udword flags, NPoint pos )
{
	UpdateColorFromMousePt(pos);
}

void NColorPickerCtrl::OnLeftButtonUp(udword flags, NPoint pos)
{
	m_bPickedHue=false;
	m_bPickedLS=false;
	ReleaseCapture();
}

void NColorPickerCtrl::OnLeftButtonDown(udword flags, NPoint pos)
{
	if( m_rcHue.Contain(pos) )
	{
		m_bPickedHue=true;
		SetCapture();
		UpdateColorFromMousePt(pos);
	} else if( m_rcLS.Contain(pos) )
	{
		m_bPickedLS=true;
		SetCapture();
		UpdateColorFromMousePt(pos);
	}
}

void NColorPickerCtrl::OnLeftButtonDblClk(udword flags, NPoint point)
{
}

void NColorPickerCtrl::OnKillFocus(NWnd* pNewWnd)
{
	TRACE("NColorPickerCtrl::OnKillFocus\n");
	ShowWindow(false);
}

void NColorPickerCtrl::GetHueAtPoint(NPoint& _pt, float& _fHue)
{
	_fHue =(float)(_pt.x - m_rcHue.left)*360.0f/(float)m_rcHue.Width();
	if(_fHue<0) _fHue = 0;
	if(_fHue>360) _fHue = 360;
}

void NColorPickerCtrl::GetLSAtPoint(NPoint& _pt, float& _fL, float& _fS)
{
	_fL = (float)(_pt.y - m_rcLS.top) / m_rcLS.Height();
	_fS = (float)(_pt.x - m_rcLS.left) / m_rcLS.Width();
	if(_fL<0) _fL = 0;
	if(_fL>1) _fL = 1;
	if(_fS<0) _fS = 0;
	if(_fS>1) _fS = 1;
}

void NColorPickerCtrl::UpdateColorFromMousePt(NPoint& _pt)
{
	//Hue Changed
	if (m_bPickedHue)
	{
		GetHueAtPoint(_pt, m_fHue);
		m_clr.SetFromHLS(m_fHue, m_fL, m_fS);
		RedrawWindow();
		OnColorClick(this);
	} else if (m_bPickedLS) //LS changed
	{
		GetLSAtPoint(_pt, m_fL, m_fS);
		m_clr.SetFromHLS(m_fHue, m_fL, m_fS);
		RedrawWindow();
		OnColorClick(this);
	}

}

void NColorPickerCtrl::OnKeyUp(udword dwchar)
{
	if (dwchar==NK_ESCAPE)
			ShowWindow(false);
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NEditCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NEditCtrl::NEditCtrl() : NWControl()
{
	m_dwCurX=m_dwCurY=0;
	m_dwLinesCount=1;
	m_dwStartSel=m_dwEndSel=0;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NEditCtrl::~NEditCtrl()
{

}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NEditCtrl::Create(const char* _pszname, const NRect& _rect, NWnd* _parent, bool _bMultiLine)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= "";
	wc.pwndParent		= _parent;
	wc.rcRect				= _rect;
	wc.dwStyle			= NWS_VISIBLE;
	NWControl::Create(wc);

	return true;
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
NString	 NEditCtrl::GetLine(udword _line)
{
	return "";
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::SetSel(udword _startchar, udword _endchar)
{
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::SelectLine(udword _line)
{
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::ReplaceSel(const char* _pszText)
{
}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.FillSolidRect(rc, RGBA(255,255,255,255));
	dc.DrawText(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE, RGBA(0,0,0,255) );
	//Cursor
	udword px = m_dwCurX*(GetApp()->GetFont()->m_h-1); //###TOFIX###
	px+=2;
	dc.DrawLine(px,0,px, rc.Height(), RGBA(255,141,15,255), 1);

}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnLeftButtonUp(udword _flags, NPoint _pos)
{
}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnLeftButtonDown(udword _flags, NPoint _pos)
{
}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnLeftButtonDblClk(udword _flags, NPoint _point)
{
}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnKillFocus(NWnd* _pNewWnd)
{

}


//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnKeyDown(udword _dwchar)
{
	if (_dwchar>=NK_SPACE && _dwchar<=NK_z)
	{
		NString str;
		str.Format("%c", (char)_dwchar);
		m_cstrText.InsertAt(m_dwCurX, str.Buffer());
		m_dwCurX++;
	} else if (_dwchar==NK_DELETE) {
		m_cstrText.RemoveAt(m_dwCurX, 1);
		m_dwCurX--;
	} else if (_dwchar==NK_BACKSPACE)	{
		if (m_dwCurX>0)
		{
			m_dwCurX--;
			m_cstrText.RemoveAt(m_dwCurX, 1);
		}
	} else if (_dwchar==NK_LEFT) {
		if (m_dwCurX>0)
			m_dwCurX--;
	} else if (_dwchar==NK_RIGHT)	{
		if (m_dwCurX<m_cstrText.Length())
			m_dwCurX++;
	}
	RedrawWindow();
}

