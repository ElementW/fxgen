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

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "../Include/CoreLib.h"
#include "MenuCtrl.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define IDTIMER_HIDE			1000
#define DELAY_TIMER_HIDE	500

//-----------------------------------------------------------------
//                   Static Variables
//-----------------------------------------------------------------
udword NMenuCtrl::m_dwReturnCmdID = 0;

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
	m_dwItemHighLightedIdx = -1;
	m_pcurPopupMenu = null;
	m_pcurParentMenu = null;
	//m_dwTimerHide = 0;
	m_bEntered = false;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NMenuCtrl::~NMenuCtrl()
{
	DeleteAllItems();
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NMenuCtrl::Create(char* name, NGUIWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= name;
	wc.pwndParent		= parent;
	wc.rcRect				= NRect(0,0,0,0);
	wc.dwStyle			= NWS_POPUP;
	NGUIWnd::Create(wc);

	return true;
}



//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
void NMenuCtrl::Update()
{
	GetGUISubSystem()->RedrawWindow(null);	//All Windows

}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NMenuCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	N2DPainter dc(this);

  ubyte byAlpha = 128;

	//NRect rcS = GetWindowRect();
	//TRACE("Display: Menu %d %d - %d %d\n", rcS.left, rcS.top, rcS.right, rcS.bottom );

	/////////////////////////////////////////////////
	//Erase Background
  dc.FillSolidRect(rc, GetGUISubSystem()->GetMenuColor());

	/////////////////////////////////////////////////
	//Display Menu Title
	NRect rcText;
	udword y = 0;
	if (m_cstrText.Length())
	{
		rcText = rc;
		rcText.left+=1;
		rcText.right-=1;
		rcText.bottom = rc.top + ME_ITEMTEXTIDENT;

    dc.FillSolidRect(rcText, GetGUISubSystem()->GetMenuColor());

		rcText.left+=2;
    dc.DrawString(m_cstrText.Buffer() , rcText, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE, RGBA(0,0,0,byAlpha) );

		y+=ME_ITEMHEIGHT;
	}

	/////////////////////////////////////////////////
	//Display Items
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
      dc.FillSolidRect(rcHL,	RGBA(0,128,255,byAlpha));
		}

		//Display Text
		rcText = pitem->rcItem;
		rcText.left+=ME_ITEMTEXTIDENT;
    dc.DrawString(pitem->strName.Buffer(), rcText, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE, RGBA(0,0,0,255) );

		//TRACE("Display: Menu Item %s\n", pitem->strName.Buffer());

		//Check Box
		if (pitem->dwStyle&ME_ITEMSTYLE_CHECKBOX)
		{
			if (pitem->bChecked)
			{
				NRect rcCheck;
				rcCheck = pitem->rcItem;
				rcCheck.top+=4;		rcCheck.bottom-=4;
				rcCheck.left+=4;	rcCheck.right=rcCheck.left+8;

				dc.RoundRect(0xF, rcCheck, 8, RGBA(0,0,0,255));
				//dc.RoundRect(rcCheck, 8,8);
			}
		}

		//Items Separator
		dc.DrawLine(1, pitem->rcItem.top, pitem->rcItem.right-1, pitem->rcItem.top, RGBA(140,140,140,byAlpha), 1);

		//Next line
		y+=ME_ITEMHEIGHT;
	}

}


void NMenuCtrl::OnSize()
{
}


void NMenuCtrl::OnLButtonDown(NPoint _point)
{
	//TRACE("OnLButtonDown\n");
	//SetFocus();

	if (m_dwItemHighLightedIdx!=-1)
	{
		NMEItemDesc* pitem = &m_carrayItems[m_dwItemHighLightedIdx];

		//Toggle CheckBox
		if (pitem->dwStyle&ME_ITEMSTYLE_CHECKBOX)
			pitem->bChecked=!pitem->bChecked;

		//Notify selected item ID
		if (pitem->ppopUpMenu==null)
		{
			m_dwReturnCmdID = pitem->dwID;
			ShowMenu(false, m_pcurParentMenu);
			OnItemClick(this);
			//Send To Parent Menu too
			if (m_pcurParentMenu)	m_pcurParentMenu->OnItemClick(this);
		}
	}

}

void NMenuCtrl::OnLButtonUp(NPoint point)
{
}

void NMenuCtrl::OnRButtonDown(NPoint point)
{
	//TrackPopupMenu(point);
	//ShowMenu(false);
}

void NMenuCtrl::OnMouseMove(NPoint point )
{
	udword idx = GetItemIdxUnderPt(point);
	TRACE("HighLighted idx %d\n", idx);

	if (idx!=m_dwItemHighLightedIdx)
	{
		m_bEntered = true;
		//Change highLighted item
		m_dwItemHighLightedIdx = idx;

		if (m_dwItemHighLightedIdx!=-1)
		{
			NMEItemDesc* pitem = &m_carrayItems[m_dwItemHighLightedIdx];


			//Open Popup Menu
			if (pitem->ppopUpMenu)
			{
				//TRACE("Opening Popup idx %d\n", m_dwItemHighLightedIdx);

				//Hide old popup
				if (m_pcurPopupMenu)
					m_pcurPopupMenu->ShowMenu(false, null);

				//Show new popup Menu
				NPoint pT(pitem->rcItem.right, pitem->rcItem.top);
				NPoint pTF(pT);
				ClientToScreen(pTF);

				//Clipping...
				NRect rcPopup;
				pitem->ppopUpMenu->CalcMenuSize(rcPopup);

				NRect rcApp = GetGUISubSystem()->GetMainWnd()->GetWindowRect();
				if (pTF.x + rcPopup.Width() > rcApp.right)
				{
					pT.x=pitem->rcItem.left-rcPopup.Width();
					pTF=pT;
					ClientToScreen(pTF);
				}
				if (pTF.y + rcPopup.Height() > rcApp.bottom)
				{
					pTF.y=rcApp.Height()-rcPopup.Height();
				}

				//TRACE("%d %d\n", pTF.x, pTF.y);

				//Display Popup
				pitem->ppopUpMenu->TrackPopupMenu(pTF, this);

				m_pcurPopupMenu = pitem->ppopUpMenu;
			}
		}

		//Repaint
		Update();
	}

}


void NMenuCtrl::OnMButtonDown(NPoint pos)
{
	//SetFocus();
}

void NMenuCtrl::OnMButtonUp(NPoint pos)
{
}


void NMenuCtrl::OnKeyUp(udword dwchar)
{
	if (dwchar==NKey::Escape)
		ShowMenu(false);
}

//-----------------------------------------------------------------
//!	\brief	Show menu
//!	\param	if true show menu else hide it
//-----------------------------------------------------------------
void NMenuCtrl::ShowMenu(bool _bShow, NMenuCtrl* _pParentMenu/*=null*/)
{
	TRACE("NMenuCtrl::ShowMenu %d\n", _bShow);

	if (_bShow)
	{
		m_bEntered = false;
		m_pcurParentMenu = _pParentMenu;
		//::ShowWindow(m_W32HWnd, SW_SHOWNOACTIVATE);

		//if (_pParentMenu==null)			SetFocus();

		//if (m_dwTimerHide)	KillTimer(m_W32HWnd, m_dwTimerHide);
		//m_dwTimerHide = ::SetTimer(m_W32HWnd, IDTIMER_HIDE, DELAY_TIMER_HIDE, NULL);
		//SetCapture();
		ShowWindow(true);
	} else {

		if (_pParentMenu)
			_pParentMenu->ShowMenu(false);
		//if (m_dwTimerHide)
		//{
			//KillTimer(m_W32HWnd, m_dwTimerHide);
			//m_dwTimerHide = 0;
		//}

		//Hide popup
		if (m_pcurPopupMenu)
			m_pcurPopupMenu->ShowMenu(false, NULL);

    m_dwItemHighLightedIdx = 0xFFFFFFFF;
		ShowWindow(false);
		//ReleaseCapture();
	}

}

//-----------------------------------------------------------------
//!	\brief	Add an item
//-----------------------------------------------------------------
NMEItemDesc* NMenuCtrl::AddItem(const char* _pszName, udword _id, udword _dwStyle)
{
	NMEItemDesc		st;
	st.dwID				= _id;
	st.dwUserData	= 0;
	st.strName		= _pszName;
	st.bEnabled		= true;
	st.bChecked		= false;
	st.dwStyle		=_dwStyle;
	st.ppopUpMenu	= null;
	m_carrayItems.AddItem(st);

	return null;
}

//-----------------------------------------------------------------
//!	\brief	Create a popup item
//-----------------------------------------------------------------
NMenuCtrl* NMenuCtrl::CreatePopupMenu(const char* _pszName, udword _idx)
{
	NMenuCtrl* ppopup =NNEW(NMenuCtrl);

	if (_idx<m_carrayItems.Count())
	{
		m_carrayItems[_idx].ppopUpMenu = ppopup;
		m_carrayItems[_idx].ppopUpMenu->Create("", this);
		m_carrayItems[_idx].strName = _pszName;

	//Append
	} else {

		NMEItemDesc		st;
		st.dwID				= m_carrayItems.Count()+1;
		st.dwUserData	= 0;
		st.strName		= _pszName;
		st.bEnabled		= true;
		st.bChecked		= false;
		st.dwStyle		= 0;
		st.ppopUpMenu	= ppopup;
		m_carrayItems.AddItem(st);

		st.ppopUpMenu->Create("", this);
	}

	return ppopup;
}


//-----------------------------------------------------------------
//!	\brief	Display menu at screen position
//!	\return	Pointer to selected Item Desc
//-----------------------------------------------------------------
void NMenuCtrl::TrackPopupMenu(NPoint _ptScreen, NMenuCtrl* _pParentMenu/*=null*/)
{
	TRACE("NMenuCtrl::TrackPopupMenu\n");

	m_dwReturnCmdID = 0;

	//Calc Menu rect
	NRect rc;
	CalcMenuSize(rc);

	//Move Menu
	//if (m_pParentWnd)
	//	m_pParentWnd->ClientToScreen(_ptScreen);

	// Clipping
	NRect rcApp = GetGUISubSystem()->GetMainWnd()->GetWindowRect();
	if(_ptScreen.x + rc.Width() > rcApp.right)
		_ptScreen.x -= _ptScreen.x + rc.Width() - rcApp.right;
	if(_ptScreen.y + rc.Height() > rcApp.bottom)
		_ptScreen.y -= _ptScreen.y + rc.Height() - rcApp.bottom;

	if (m_pParentWnd)
		m_pParentWnd->ScreenToClient(_ptScreen);

	rc.Move(_ptScreen);
	SetWindowRect(rc);

	//TRACE("TrackPopupMenu %d,%d-%d,%d\n", rc.left, rc.top, rc.right, rc.bottom);
	//TRACE("Count %d\n", m_carrayItems.Count());

	//TRACE("_pParentMenu %d\n", _pParentMenu);
	if (_pParentMenu==null)			SetFocus();

	//Display menu window
	ShowMenu(true, _pParentMenu);

	//Graphic update
	//Update();

}

//-----------------------------------------------------------------
//!	\brief	Delete all menu items
//-----------------------------------------------------------------
void NMenuCtrl::DeleteAllItems()
{
	udword dwCount = m_carrayItems.Count();
	for (udword i=0; i<dwCount; i++)
	{
		NMEItemDesc* pitem = &m_carrayItems[i];
		if (pitem->ppopUpMenu)
			NDELETE(pitem->ppopUpMenu, NMenuCtrl);
	}

	m_carrayItems.Clear();
}

//-----------------------------------------------------------------
//!	\brief	Calc Window Menu rectangle
//!	\param	_rcOut	Computed menu rectangle
//-----------------------------------------------------------------
void NMenuCtrl::CalcMenuSize(NRect& _rcOut)
{
	//Get Max Width
	udword dwCount = m_carrayItems.Count();
	udword dwMaxLen = m_cstrText.Length();
	for (udword i=0; i<dwCount; i++)
	{
		NMEItemDesc* pitem = &m_carrayItems[i];
		if (pitem->strName.Length()>dwMaxLen)
			dwMaxLen = pitem->strName.Length();
	}

	//Return rect
	udword dwTitleH = 0;
	if (m_cstrText.Length())	dwTitleH=ME_ITEMHEIGHT;

	_rcOut.left		= 0;
	_rcOut.top		= 0;
	_rcOut.right	= dwMaxLen * 12;
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

udword NMenuCtrl::GetItemsCount()
{
	return m_carrayItems.Count();
}

NMEItemDesc* NMenuCtrl::GetItemDesc(udword _idx)
{
	if (_idx<m_carrayItems.Count())
	{
		return &m_carrayItems[_idx];
	}

	return null;
}

NMEItemDesc* NMenuCtrl::GetItemDescFromID(udword _id)
{
	for (udword i=0; i<m_carrayItems.Count(); i++)
	{
		NMEItemDesc* pitem = &m_carrayItems[i];
		if (pitem->dwID==_id)
		{
			return &m_carrayItems[i];
		}
	}

	return null;
}

NMenuCtrl* NMenuCtrl::GetPopupMenu(udword _idx)
{
	if (_idx<m_carrayItems.Count())
	{
		return m_carrayItems[_idx].ppopUpMenu;
	}

	return null;
}

void NMenuCtrl::OnKillFocus(NGUIWnd* pNewWnd)
{
	//TRACE("NMenuCtrl::OnKillFocus\n");
	if ((pNewWnd==NULL || pNewWnd!=m_pcurPopupMenu))
	{
		if (m_pcurParentMenu)
			m_pcurParentMenu->ShowMenu(false, NULL);
		else
			ShowMenu(false, NULL);
	}
}
