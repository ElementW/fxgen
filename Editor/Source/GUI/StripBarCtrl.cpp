//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		StripBarCtrl.cpp
//! \brief	GUI control for controls container as toolbar
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		15-11-2007
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "..\pch.h"
#include "StripBarCtrl.h"

#ifdef __GNUC__
#include <limits.h>
#endif

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NStripBarCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NStripBarCtrl::NStripBarCtrl()
{
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NStripBarCtrl::~NStripBarCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NStripBarCtrl::Create(const char* name, const NRect& rect, NWnd* parent)
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
void NStripBarCtrl::Update()
{
	RedrawWindow();
	//OnPaint();
	//::RedrawWindow(m_W32HWnd, null, null, RDW_INVALIDATE | RDW_UPDATENOW /*| RDW_ERASE*/ );
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NStripBarCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics gfx(this);

	/////////////////////////////////////////////////
	//Erase Background
	gfx.FillSolidRect(rc, RGBA(115,115,115,255));
	gfx.RoundRect(0xC, rc, 10, RGBA(64,64,64,255));
	rc.Deflate(1,1);
	gfx.RoundRect(0xC, rc, 10, RGBA(195,195,195,255));

	//TRACE("%s %dx%d-%dx%d %d\n", m_cstrText, m_rcWnd.left, m_rcWnd.top, m_rcWnd.right, m_rcWnd.bottom, m_dwStyle);
}


void NStripBarCtrl::OnSize()
{
	//Redraw
	RecalcLayout();
	Update();
	//::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}


void NStripBarCtrl::OnLeftButtonDown(udword flags, NPoint _point)
{
	//TRACE("OnLeftButtonDown\n");
}

void NStripBarCtrl::OnLeftButtonUp(udword flags, NPoint point)
{
}


void NStripBarCtrl::OnLeftButtonDblClk(udword flags, NPoint point)
{
}


void NStripBarCtrl::OnRightButtonDown(udword flags, NPoint point)
{
}

void NStripBarCtrl::OnMouseMove(udword flags, NPoint point )
{
}


void NStripBarCtrl::OnMButtonDown(udword flags, NPoint pos)
{
}

void NStripBarCtrl::OnMButtonUp(udword flags, NPoint pos)
{
}


void NStripBarCtrl::OnKeyDown(udword dwchar)
{
}

//-----------------------------------------------------------------
//!	\brief	Insert a text Item
//-----------------------------------------------------------------
NTextCtrl* NStripBarCtrl::InsertItemText(const char* _pszname, udword _dwWidth)
{
	//Create a text
	NRect rc;
	NTextCtrl* pctrl = new NTextCtrl();
	pctrl->Create(_pszname, rc, this);

	//Insert new control
	NSBItemDesc st;
	st.pwnd			= pctrl;
	st.dwWidth	= _dwWidth;
	m_carrayItems.AddItem(st);

	//Recalc Layout
	RecalcLayout();

	return pctrl;
}



//-----------------------------------------------------------------
//!	\brief	Insert a button Item
//-----------------------------------------------------------------
NButtonCtrl* NStripBarCtrl::InsertItemButton(const char* _pszname, udword _dwWidth)
{
	//Create a button
	NRect rc;
	NButtonCtrl* pctrl = new NButtonCtrl();
	pctrl->Create(_pszname, rc, this, NBUT_STYLE_MENU);

	//Insert new control
	NSBItemDesc st;
	st.pwnd			= pctrl;
	st.dwWidth	= _dwWidth;
	m_carrayItems.AddItem(st);

	//Recalc Layout
	RecalcLayout();

	return pctrl;
}


//-----------------------------------------------------------------
//!	\brief	Recalc controls positions and size
//-----------------------------------------------------------------
void NStripBarCtrl::RecalcLayout()
{
	udword posx=SB_INDENT+SB_BORDER;
	for (udword i=0; i<m_carrayItems.Count(); i++)
	{
		NSBItemDesc* pst = &m_carrayItems[i];
		pst->rcItem.left=posx;
		pst->rcItem.right=posx+pst->dwWidth;
		pst->rcItem.top=SB_BORDER;
		pst->rcItem.bottom=SB_BARHEIGHT-(SB_BORDER);

		pst->pwnd->SetWindowRect(pst->rcItem);

		posx+=pst->dwWidth+SB_BORDER;
	}

}