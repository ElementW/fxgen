//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		LayoutDock.cpp
//! \brief	Docking Wnd Manager
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		11-03-2010
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "LayoutDock.h"

#ifdef __GNUC__
#include <limits.h>
#endif

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define LD_SPLITBOX		4

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NLayoutDock Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NLayoutDock::NLayoutDock()
{
	SLayoutItem* pst			= new SLayoutItem();
	pst->dwIdxToSplit			= -1;
	pst->eTypeSplit				= eAucun;
	pst->dwIdxWndSelected	= -1;
	m_arrayItems.AddItem(pst);

	m_dwTabHeight = 18;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NLayoutDock::~NLayoutDock()
{
	DeleteAllLayouts();
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NLayoutDock::Create(const char* name, const NRect& rect, NGUIWnd* parent)
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
void NLayoutDock::Update()
{
	RedrawWindow();
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NLayoutDock::OnPaint(N2DPainter* _ppainter)
{
	NRect rc = GetClientRect();

	/////////////////////////////////////////////////
	//Erase Background
	//_ppainter->FillSolidRect(rc, RGBA(115,115,115,255));

	/////////////////////////////////////////////////
	//Display Tabs
	NRect lrc;
	for (udword i=0; i<m_arrayItems.Count(); i++)
	{
		SLayoutItem* pLayout = m_arrayItems[i];

		//Childs Wnd
		udword nCount = pLayout->arrayTabs.Count();
		if (nCount==0)	continue;

		lrc = pLayout->rc;
		lrc.bottom=lrc.top+m_dwTabHeight;

		//if (pLayout->dwIdxWndSelected!=-1)
		//Display tabs
		for (uword j=0; j<pLayout->arrayTabs.Count(); j++)
		{
			NGUIWnd* pwnd = pLayout->arrayTabs[j].pwnd;
			udword len = pwnd->GetText().Length();
			lrc.right=lrc.left+(len*8);	//###TOFIX### text len

			_ppainter->GradientVRect(lrc, GetGUISubSystem()->GetFaceColor(), GetGUISubSystem()->GetDarkColor());
			if (pLayout->dwIdxWndSelected==j)
				_ppainter->FillSolidRect(lrc, NColor(255,255,255,128));

			_ppainter->DrawLine(lrc.left, lrc.top, lrc.right, lrc.top, GetGUISubSystem()->GetDarkColor(), 1);
			_ppainter->DrawLine(lrc.left, lrc.top, lrc.left, lrc.bottom, GetGUISubSystem()->GetDarkColor(), 1);
			_ppainter->DrawLine(lrc.right, lrc.top, lrc.right, lrc.bottom, GetGUISubSystem()->GetDarkColor(), 1);

			_ppainter->DrawString(pwnd->GetText().Buffer(), lrc, NDT_END_ELLIPSIS|NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE, NColor(0,0,0,255));
			pLayout->arrayTabs[j].rc = lrc;

			lrc.left+=lrc.Width();

		}
	}

}


void NLayoutDock::OnSize()
{
	RecalLayout();

	//Redraw
	Update();
}


void NLayoutDock::OnLButtonDown(NPoint _point)
{
	//TRACE("OnLButtonDown\n");
	SetFocus();

	//MAJ
	Update();

}

void NLayoutDock::OnLButtonUp(NPoint _point)
{
	SLayoutItem* playout = GetLayoutAt(_point);
	if (playout)
	{
		udword idx = GetLayoutTabAt(playout, _point);
		if (idx!=0xFFFFFFFF)
		{
			SelectTab(playout, idx);
			RecalLayout();
			Update();
		}
	}

}


void NLayoutDock::OnLButtonDblClk(NPoint point)
{
}


void NLayoutDock::OnRButtonDown(NPoint point)
{
	SetFocus();
}

void NLayoutDock::OnMouseMove(NPoint point )
{
}


void NLayoutDock::OnMButtonDown(NPoint pos)
{
	SetFocus();
}

void NLayoutDock::OnMButtonUp(NPoint pos)
{
}


void NLayoutDock::OnKeyDown(udword dwchar)
{
}


void NLayoutDock::RecalLayout()
{
	NRect rc = GetClientRect();

	udword x = rc.left;
	udword y = rc.top;
	udword w =rc.Width();
	udword h =rc.Height();

	udword i, j;

	//Reset des rectangles
	for (i=0; i<m_arrayItems.Count(); i++)
		m_arrayItems[i]->rc.SetEmpty();

	//Init du layout principal
	SLayoutItem* pLayout	= m_arrayItems[0];
	pLayout->rc.left			= x;
	pLayout->rc.top				= y;
	pLayout->rc.right			= x+w;
	pLayout->rc.bottom		= y+h;

	//Recalcul de la dimension des layouts
	for (i=0; i<m_arrayItems.Count(); i++)
	{
		pLayout = m_arrayItems[i];

		//Recuperation du layout a splitter
		if (pLayout->dwIdxToSplit==-1)
			continue;

		SLayoutItem* pLayoutToSplit = m_arrayItems[pLayout->dwIdxToSplit];

		//Partition horizontal
		//------
		// pane1
		//------
		// pane2
		//------
		if (pLayout->eTypeSplit==eHorizontal)
		{
			//On partitionne la fenetre parent
			udword dwSplitAt = pLayout->dwSplitAt * pLayoutToSplit->rc.Height() / 100;

			//On recalcule la taille du layout courant
			pLayout->rc.left		= pLayoutToSplit->rc.left;
			pLayout->rc.top			=	pLayoutToSplit->rc.top + dwSplitAt + LD_SPLITBOX;
			pLayout->rc.right		= pLayoutToSplit->rc.right;
			pLayout->rc.bottom	= pLayoutToSplit->rc.bottom - LD_SPLITBOX;

			//MAJ du parent
			pLayoutToSplit->rc.bottom = pLayoutToSplit->rc.top + dwSplitAt;

		//Partition verticale
		//      |
		// pane1|pane2
		//      |
		} else if (pLayout->eTypeSplit==eVertical) {

			//On partitionne la fenetre parent
			udword dwSplitAt = pLayout->dwSplitAt * pLayoutToSplit->rc.Width() / 100;

			//On recalcule la taille du layout courant
			pLayout->rc.left		= pLayoutToSplit->rc.left + dwSplitAt + LD_SPLITBOX;
			pLayout->rc.top			=	pLayoutToSplit->rc.top;
			pLayout->rc.right		= pLayoutToSplit->rc.right;
			pLayout->rc.bottom	= pLayoutToSplit->rc.bottom;

			//MAJ du parent
			pLayoutToSplit->rc.right = pLayoutToSplit->rc.left + dwSplitAt;

		}

	}

	//Recalcul de la dimension des fenetres dans les layouts
	NRect lrc;
	for (i=0; i<m_arrayItems.Count(); i++)
	{
		pLayout = m_arrayItems[i];

		//Childs Wnd
		udword nCount = pLayout->arrayTabs.Count();
		if (nCount==0)	continue;

		lrc = pLayout->rc;
		lrc.top+=m_dwTabHeight;

		if (pLayout->dwIdxWndSelected!=-1)
		{
			NGUIWnd* pwnd = pLayout->arrayTabs[pLayout->dwIdxWndSelected].pwnd;
			pwnd->ShowWindow(true);
			pwnd->SetWindowRect(lrc);
		}

		for (j=0; j<pLayout->arrayTabs.Count(); j++)
		{
			NGUIWnd* pwnd = pLayout->arrayTabs[j].pwnd;
			if (pLayout->dwIdxWndSelected!=j)
				pwnd->ShowWindow(false);
		}


	}

}

udword NLayoutDock::SplitH(udword _dwID, udword _dwPercent)
{
	SLayoutItem*		pst = new SLayoutItem();
	pst->dwIdxToSplit		= _dwID;
	pst->dwSplitAt			= _dwPercent;
	pst->eTypeSplit			=	eHorizontal;
	pst->dwIdxWndSelected=-1;
	return m_arrayItems.AddItem(pst
);
}

udword NLayoutDock::SplitV(udword _dwID, udword _dwPercent)
{
	SLayoutItem*		pst = new SLayoutItem();
	pst->dwIdxToSplit		= _dwID;
	pst->dwSplitAt			= _dwPercent;
	pst->eTypeSplit			=	eVertical;
	pst->dwIdxWndSelected=-1;
	return m_arrayItems.AddItem(pst);
}

void NLayoutDock::DeleteAllLayouts()
{
	udword i;
	for (i=0; i<m_arrayItems.Count(); i++)
	{
		delete m_arrayItems[i];
	}
	m_arrayItems.Clear();
}

void NLayoutDock::AddWndToLayout(udword _dwID, NGUIWnd* _pwnd)
{
	SLayoutItem* pLayoutToSplit = m_arrayItems[_dwID];

	SLayoutTabItem st;
	st.rc.SetEmpty();
	st.pwnd = _pwnd;
	pLayoutToSplit->arrayTabs.AddItem(st);

	pLayoutToSplit->dwIdxWndSelected = 0;
}

SLayoutItem* NLayoutDock::GetLayoutAt(NPoint point)
{
	for (udword i=0; i<m_arrayItems.Count(); i++)
	{
		SLayoutItem* pLayout = m_arrayItems[i];
		if (pLayout->rc.Contain(point))
			return pLayout;
	}	
	return null;
}

udword NLayoutDock::GetLayoutTabAt(SLayoutItem* _pLayout, NPoint _point)
{
	for (udword j=0; j<_pLayout->arrayTabs.Count(); j++)
	{
		if (_pLayout->arrayTabs[j].rc.Contain(_point))
			return j;
	}

	return 0xFFFFFFFF;
}

void NLayoutDock::SelectTab(SLayoutItem* _playout, udword _idx)
{
	_playout->dwIdxWndSelected = _idx;
}