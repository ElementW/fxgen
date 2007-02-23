//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		NViewportsCtrl.cpp
//! \brief	GUI control for viewport
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
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
#include "pch.h"
#include "ViewportsCtrl.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NViewportsCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NViewportsCtrl::NViewportsCtrl()
{
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NViewportsCtrl::~NViewportsCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NViewportsCtrl::Create(char* name, NRect& rect, NWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.Id						= 1;
	wc.Name					= name;
	wc.Parent				= parent;
	wc.Rect					= rect;
	wc.W32ClassName	= "NEXUS_WNGraphicsLASS";
	wc.W32Style			= WS_CHILD | WS_CLIPCHILDREN;   //WS_CLIPSIBLINGS;
	wc.W32StyleEx		= WS_EX_ACCEPTFILES;
	NWnd::Create(wc);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Window proc
//-----------------------------------------------------------------
LRESULT NViewportsCtrl::WndProc( UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg==WM_ERASEBKGND)		return 1;

	//Else Call Base Class
	else return NWnd::WndProc(msg, wparam, lparam);
}

//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
void NViewportsCtrl::Update()
{
	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NViewportsCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.FillSolidRect(rc, RGB(0,0,0));

}


void NViewportsCtrl::OnSize()
{
	//Redraw
	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}


