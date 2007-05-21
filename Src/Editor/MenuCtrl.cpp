//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MenuCtrl.cpp
//! \brief	GUI control for menu
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		24-04-2007
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
#include "MenuCtrl.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NGLRenderer Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NMenuCtrl::NMenuCtrl()
{
	m_hfontNormal = m_hfontBold = null;
	m_dwItemHighLightedIdx = -1;

}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NMenuCtrl::~NMenuCtrl()
{
	if (m_hfontNormal)	::DeleteObject(m_hfontNormal);
	if (m_hfontBold)		::DeleteObject(m_hfontBold);
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NMenuCtrl::Create(char* name, NWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.Id						= 1;
	wc.Name					= name;
	wc.Parent				= parent;
	wc.Rect					= NRect(0,0,0,0);
	wc.W32ClassName	= "FXGen_WNDCLASS";
	wc.W32Style			= WS_POPUP;
	wc.W32StyleEx		= WS_EX_ACCEPTFILES;
	NWnd::Create(wc);

	//Create Normal Font
	m_hfontNormal = ::CreateFont(
		10, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Lucida console");

	//Create Bold Font
	m_hfontBold = ::CreateFont(
		10, 0, 0, 0, FW_BOLD, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Lucida console");


	return true;
}

//-----------------------------------------------------------------
//!	\brief	Window proc
//-----------------------------------------------------------------
LRESULT NMenuCtrl::WndProc( UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg==WM_ERASEBKGND)		return 1;

	//Else Call Base Class
	else return NWnd::WndProc(msg, wparam, lparam);
}


//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
void NMenuCtrl::Update()
{
	//::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
	::RedrawWindow(m_W32HWnd, null, null, RDW_INVALIDATE | RDW_UPDATENOW /*| RDW_ERASE*/ );
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NMenuCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.SetBkMode(TRANSPARENT);

	dc.SetTextColor(RGB(0,0,0));

	/////////////////////////////////////////////////
	//Erase Background
	dc.FillSolidRect(rc, RGB(115,115,115));
	dc.Draw3dRect(rc, RGB(200,200,200), RGB(0,0,0));

	/////////////////////////////////////////////////
	//Display Menu Title
	NRect rcText;
	udword y = 0;
	if (cstrWindowName.Length())
	{
		dc.SetFont(m_hfontBold);

		rcText = rc;
		rcText.left+=1;
		rcText.right-=1;
		rcText.bottom = rc.top + ME_ITEMTEXTIDENT;

		dc.FillSolidRect(rcText, RGB(180,210,180));

		rcText.left+=2;
		dc.DrawText(cstrWindowName.Buffer() , rcText, DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

		y+=ME_ITEMHEIGHT;
	}

	/////////////////////////////////////////////////
	//Display Items
	dc.SetFont(m_hfontNormal);
	dc.SetPen(1, RGB(140,140,140));

	udword dwCount = m_carrayItems.Count();
	for (udword i=0; i<dwCount; i++)
	{
		NMEItemDesc* pitem = &m_carrayItems[i];
		
		//Item rect
		pitem->rcItem.left	= 0;
		pitem->rcItem.right = rc.Width();
		pitem->rcItem.top		= y;
		pitem->rcItem.bottom= y+ME_ITEMHEIGHT;

		// HighLight Item
		if (m_dwItemHighLightedIdx==i)
		{
			NRect rcHL;
			rcHL = pitem->rcItem;
			rcHL.left+=1;	rcHL.right-=1;
			dc.FillSolidRect(rcHL,	RGB(255,141,15));
		}

		//Display Text
		rcText = pitem->rcItem;
		rcText.left+=ME_ITEMTEXTIDENT;
		dc.DrawText(pitem->strName.Buffer(), rcText, DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

		//Items Separator
		dc.MoveTo(1, pitem->rcItem.top);
		dc.LineTo(pitem->rcItem.right-1, pitem->rcItem.top);

		//Next line
		y+=ME_ITEMHEIGHT;
	}

}


void NMenuCtrl::OnSize()
{
	//Redraw
	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}


void NMenuCtrl::OnLeftButtonDown(udword flags, NPoint _point)
{
	//TRACE("OnLeftButtonDown\n");
	SetFocus();
	ShowMenu(false);

	//Send selected item ID
	if (m_dwItemHighLightedIdx!=-1)
	{
		udword id = m_carrayItems[m_dwItemHighLightedIdx].dwID;
		::SendMessage(m_pParentWnd->m_W32HWnd, WM_COMMAND, id, 0);
	}

}

void NMenuCtrl::OnLeftButtonUp(udword flags, NPoint point)
{
}


void NMenuCtrl::OnLeftButtonDblClk(udword flags, NPoint point)
{
}


void NMenuCtrl::OnRightButtonDown(udword flags, NPoint point)
{
	//TrackPopupMenu(point);
	ShowMenu(false);
}

void NMenuCtrl::OnMouseMove(udword flags, NPoint point )
{
	udword idx = GetItemIdxUnderPt(point);
	if (idx!=m_dwItemHighLightedIdx)
	{
		m_dwItemHighLightedIdx = idx;
		Update();
	}
}


void NMenuCtrl::OnMButtonDown(udword flags, NPoint pos)
{
	SetFocus();
}

void NMenuCtrl::OnMButtonUp(udword flags, NPoint pos)
{
}


void NMenuCtrl::OnKeyUp(udword dwchar)
{
	if (dwchar==VK_ESCAPE)
	{
		ShowMenu(false);
	}

}

//-----------------------------------------------------------------
//!	\brief	Show menu
//!	\param	if true show menu else hide it
//-----------------------------------------------------------------
void NMenuCtrl::ShowMenu(bool _bShow)
{
	if (_bShow)
	{
		::ShowWindow(m_W32HWnd, SW_SHOW);
		SetFocus();
		SetCapture();
	} else {
		::ShowWindow(m_W32HWnd, SW_HIDE);
		ReleaseCapture();
	}

}

//-----------------------------------------------------------------
//!	\brief	Add an item
//-----------------------------------------------------------------
NMEItemDesc* NMenuCtrl::AddItem(char* _pszName, udword _id, NMEItemDesc* _pparent)
{
	NMEItemDesc		st;
	st.dwID				= _id;
	st.dwUserData	= 0;
	st.strName		= _pszName;
	st.bEnabled		= true;
	st.bChecked		= false;
	m_carrayItems.AddItem(st);

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Display menu at screen position
//!	\return	Pointer to selected Item Desc
//-----------------------------------------------------------------
NMEItemDesc* NMenuCtrl::TrackPopupMenu(NPoint _ptScreen)
{
	//Calc Menu rect
	NRect rc;
	CalcMenuSize(rc);
	rc.Move(_ptScreen);
	SetWindowRect(rc);

	//Display menu window
	ShowMenu(true);

	//Graphic update
	Update();

	//Hide menu window
	//::ShowWindow(m_W32HWnd, SW_HIDE);

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Calc Window Menu rectangle
//!	\param	_rcOut	Computed menu rectangle
//-----------------------------------------------------------------
void NMenuCtrl::CalcMenuSize(NRect& _rcOut)
{
	//Get Max Width
	udword dwCount = m_carrayItems.Count();
	udword dwMaxLen = cstrWindowName.Length();
	for (udword i=0; i<dwCount; i++)
	{
		NMEItemDesc* pitem = &m_carrayItems[i];
		if (pitem->strName.Length()>dwMaxLen)
			dwMaxLen = pitem->strName.Length();
	}

	//Return rect
	udword dwTitleH = 0;
	if (cstrWindowName.Length())	dwTitleH=ME_ITEMHEIGHT;

	_rcOut.left		= 0;
	_rcOut.top		= 0;
	_rcOut.right	= dwMaxLen * 10;
	_rcOut.bottom	= (dwCount * ME_ITEMHEIGHT) + 1 + dwTitleH;

}

//-----------------------------------------------------------------
//!	\brief	Return Item Indice under screen coordinate
//!	\param	_point	Point at screen
//!	\return	Item indice if found, else -1.
//-----------------------------------------------------------------
udword NMenuCtrl::GetItemIdxUnderPt(NPoint& _point)
{
	udword dwCount = m_carrayItems.Count();
	for (udword i=0; i<dwCount; i++)
	{
		NMEItemDesc* pitem = &m_carrayItems[i];
		if (pitem->rcItem.Contain(_point))
		{
			return i;
		}
	}
	return (udword)-1;
}