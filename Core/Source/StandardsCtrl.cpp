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
#include "../Include/CoreLib.h"
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
bool NTextCtrl::Create(const char* name, const NRect& rect, NGUIWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= (char*)name;
	wc.pwndParent		= parent;
	wc.rcRect				= rect;
	wc.dwStyle			= NWS_VISIBLE;
	NGUIWnd::Create(wc);

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

	N2DPainter dc(this);
  //dc.FillSolidRect(rc, GetGUISubSystem()->GetBarColor());
  dc.DrawString(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE|NDT_HCENTER, RGBA(0,0,0,255) );

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
bool NButtonCtrl::Create(const char* name, const NRect& rect, NGUIWnd* parent, udword _dwStyle)
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
	NGUIWnd::Create(wc);

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

	N2DPainter dc(this);

	///////////////////////////////////////////////////////////
	//Normal Button
	if (m_dwStyle==NBUT_STYLE_PUSH)
	{
		if (!m_bClicked)
		{
			dc.GradientVRect(rc, RGBA(255, 220, 220, 220), RGBA(120, 120, 120,255));

			if (m_bMouseOver)			dc.Draw3dRect(rc, RGBA(205,194,14,255), RGBA(205,194,14,255));
			else									dc.Draw3dRect(rc, RGBA(255,255,255,255), RGBA(0,0,0,255));

      dc.DrawString(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE|NDT_HCENTER, RGBA(0,0,0,255) );
		} else {
			dc.GradientVRect(rc, RGBA(120, 120, 120,255), RGBA(220, 220, 220,255));

			if (m_bMouseOver)			dc.Draw3dRect(rc, RGBA(205,194,14,255), RGBA(205,194,14,255));
			else									dc.Draw3dRect(rc, RGBA(0,0,0,255), RGBA(255,255,255,255));

			rc.Move(1,1);
      dc.DrawString(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE|NDT_HCENTER, RGBA(0,0,0,255) );
		}

	///////////////////////////////////////////////////////////
	//Menu Button
	} else if (m_dwStyle==NBUT_STYLE_MENU) {
		NColor clr;
    if (m_bMouseOver)			{ /*dc.RoundRect(0xF, rc, 0, RGBA(64,64,64,255));*/ clr=0xFFFFFFFF; }
		else									{ clr=0xFF000000; }
    dc.DrawString(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE|NDT_HCENTER, clr );
	}


}


void NButtonCtrl::OnSize()
{
	Update();
}


void NButtonCtrl::OnLButtonDown(NPoint _point)
{
	//TRACE("OnLButtonDown\n");
	m_bClicked = true;
	Update();
}

void NButtonCtrl::OnLButtonUp(NPoint point)
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

void NButtonCtrl::OnRButtonDown(NPoint point)
{
}

void NButtonCtrl::OnMouseMove(NPoint point )
{
	m_bMouseOver = true;
}


void NButtonCtrl::OnMButtonDown(NPoint pos)
{
}

void NButtonCtrl::OnMButtonUp(NPoint pos)
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
bool NColorPickerCtrl::Create(NGUIWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= "";
	wc.pwndParent		= parent;
	wc.rcRect				= NRect(0,0,0,0);
	wc.dwStyle			= NWS_POPUP;
	NGUIWnd::Create(wc);

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

	NRect rcApp = GetGUISubSystem()->GetMainWnd()->GetWindowRect();
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

	N2DPainter gfx(this);
  gfx.FillSolidRect(rc, GetGUISubSystem()->GetBarColor() );

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
  gfx.DrawString(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );
	cstr.Format("L: %f", m_fL); rcText.top+=10;
  gfx.DrawString(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );
	cstr.Format("S: %f", m_fS); rcText.top+=10;
  gfx.DrawString(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );


	rcText.top+=20;
	cstr.Format("R: %d", m_clr.GetR() );
  gfx.DrawString(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );
	cstr.Format("G: %d", m_clr.GetG()); rcText.top+=10;
  gfx.DrawString(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );
	cstr.Format("B: %d", m_clr.GetB()); rcText.top+=10;
  gfx.DrawString(cstr.Buffer() , rcText, NDT_SINGLELINE, RGBA(0,0,0,255) );

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



void NColorPickerCtrl::OnMouseMove(NPoint pos )
{
	UpdateColorFromMousePt(pos);
}

void NColorPickerCtrl::OnLButtonUp(NPoint pos)
{
	m_bPickedHue=false;
	m_bPickedLS=false;
	ReleaseCapture();
}

void NColorPickerCtrl::OnLButtonDown(NPoint pos)
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

void NColorPickerCtrl::OnLButtonDblClk(NPoint point)
{
}

void NColorPickerCtrl::OnKillFocus(NGUIWnd* pNewWnd)
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
	if (dwchar==NKey::Escape)
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
NEditCtrl::NEditCtrl() : NGUIWnd()
{
	m_dwCursorPos=m_dwSelectionTail=0;
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
bool NEditCtrl::Create(const char* _pszname, const NRect& _rect, NGUIWnd* _parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= "";
	wc.pwndParent		= _parent;
	wc.rcRect				= _rect;
	wc.dwStyle			= NWS_VISIBLE;
	NGUIWnd::Create(wc);

	return true;
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::SetSel(udword _startchar, udword _endchar)
{
	m_dwCursorPos = _endchar;
	m_dwSelectionTail = _startchar;
	if(m_dwCursorPos>m_cstrText.Length())
		m_dwCursorPos = m_cstrText.Length();
	if(m_dwSelectionTail>m_cstrText.Length())
		m_dwSelectionTail = m_cstrText.Length();
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::SelectAll()
{
	m_dwSelectionTail = 0; m_dwCursorPos = m_cstrText.Length();
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::ReplaceSel(const char* _pszText)
{
	if (m_dwSelectionTail != m_dwCursorPos)
	{
		udword start = 0, end = 0;
		GetSel(start, end);
		m_cstrText.RemoveAt(start, end-start);
		m_dwCursorPos = start;
		m_dwSelectionTail = start;
		if(_pszText != null)
		{
			m_cstrText.InsertAt(m_dwCursorPos, _pszText);
			m_dwSelectionTail += strlen(_pszText);
		}
	}
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	N2DPainter dc(this);
	dc.FillSolidRect(rc, RGBA(255,255,255,255));
  dc.DrawString(m_cstrText.Buffer(), rc, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE, RGBA(0,0,0,255) );
	//Cursor
	if( m_dwSelectionTail == m_dwCursorPos )
	{
		udword px = m_dwCursorPos*(GetGUISubSystem()->GetFont()->m_h-1);
		dc.DrawLine(px,0,px, rc.Height(), RGBA(255,141,15,255), 1);
	} else {
		udword pxs = nmin(m_dwCursorPos, m_dwSelectionTail)*(GetGUISubSystem()->GetFont()->m_h-1);
		udword pxe = nmax(m_dwCursorPos, m_dwSelectionTail)*(GetGUISubSystem()->GetFont()->m_h-1);
		NRect rc2(pxs, 0, pxe, rc.Height());
		dc.FillSolidRect(rc2, NColor(255,141,15,128));
	}

}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnLButtonUp(udword _flags, NPoint _pos)
{
}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnLButtonDown(udword _flags, NPoint _pos)
{
}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnLButtonDblClk(udword _flags, NPoint _point)
{
}
//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnKillFocus(NGUIWnd* _pNewWnd)
{
	OnEscape(this);
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnText(udword _unicode)
{
	if( _unicode >= 32 && _unicode < 128 )
	{
		NString str;
		str.Format("%c", (char)_unicode);
		if (m_dwSelectionTail != m_dwCursorPos)
		{
			ReplaceSel(str.Buffer());
			m_dwCursorPos++;
			m_dwSelectionTail = m_dwCursorPos;
		} else {
			m_cstrText.InsertAt(m_dwCursorPos, str.Buffer());
			m_dwCursorPos++;
			m_dwSelectionTail++;
		}
	}
}

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
void NEditCtrl::OnKeyDown(udword _dwchar)
{
	if (_dwchar==NKey::Delete) {
		if(m_dwSelectionTail == m_dwCursorPos)
		{
			if (m_dwCursorPos<m_cstrText.Length())
				m_cstrText.RemoveAt(m_dwCursorPos, 1);
		} else {
			ReplaceSel("");
		}
	} else if (_dwchar==NKey::Back)	{
		if(m_dwSelectionTail == m_dwCursorPos)
		{
			if (m_dwCursorPos>0)
			{
				m_dwCursorPos--;
				m_dwSelectionTail--;
				m_cstrText.RemoveAt(m_dwCursorPos, 1);
			}
		} else {
			ReplaceSel("");
		}
	} else if (_dwchar==NKey::Left) {
		if (m_dwCursorPos>0)
			m_dwCursorPos--;
		if (!GetGUISubSystem()->IsKeyDown(NKey::LShift) && !GetGUISubSystem()->IsKeyDown(NKey::RShift))
			m_dwSelectionTail = m_dwCursorPos;
	} else if (_dwchar==NKey::Right)	{
		if (m_dwCursorPos<m_cstrText.Length())
			m_dwCursorPos++;
		if (!GetGUISubSystem()->IsKeyDown(NKey::LShift) && !GetGUISubSystem()->IsKeyDown(NKey::RShift))
			m_dwSelectionTail = m_dwCursorPos;
	} else if (_dwchar==NKey::Home) {
		m_dwCursorPos = 0;
		if (!GetGUISubSystem()->IsKeyDown(NKey::LShift) && !GetGUISubSystem()->IsKeyDown(NKey::RShift))
			m_dwSelectionTail = m_dwCursorPos;
	} else if (_dwchar==NKey::End)	{
		m_dwCursorPos = m_cstrText.Length();
		if (!GetGUISubSystem()->IsKeyDown(NKey::LShift) && !GetGUISubSystem()->IsKeyDown(NKey::RShift))
			m_dwSelectionTail = m_dwCursorPos;
	} else if (_dwchar==NKey::Return)	{
		OnEnter(this);
	} else if (_dwchar==NKey::Escape)	{
		OnEscape(this);
	}
	RedrawWindow();
}
