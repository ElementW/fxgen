//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		TreeNodeCtrl.cpp
//! \brief	GUI control for tree nodes editing
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
//#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "TreeNodeCtrl.h"

#ifdef __GNUC__
#include <limits.h>
#endif

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
NTreeNodeCtrl::NTreeNodeCtrl()
{
	m_dwCurItemIdx = -1;
	m_prootNode = null;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NTreeNodeCtrl::~NTreeNodeCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NTreeNodeCtrl::Create(const char* name, const NRect& rect, NGUIWnd* parent)
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
void NTreeNodeCtrl::Update()
{
	RedrawWindow();
}

//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NTreeNodeCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	N2DPainter dc(this);

	/////////////////////////////////////////////////
	//Erase Background
	dc.FillSolidRect(rc, RGBA(115,115,115,255));

	/////////////////////////////////////////////////
	//Calc Items rect
	sdword dwMaxDepth=INT_MAX;
	sdword dwPosY=0;
	for (udword i=0; i<m_carrayItemsSortedPos.Count(); i++)
	{
		udword idx = m_carrayItemsSortedPos[i];
		NTNItemDesc& item = m_carrayItems[idx];

		//Item visible
		if (item.dwDepth<=dwMaxDepth)
		{
			//Item rect
			NRect rcItem;
			rcItem.left		= rc.left;		rcItem.top		= dwPosY;
			rcItem.right	= rc.right;		rcItem.bottom = rcItem.top+TN_ITEMHEIGHT;

			//Item Rects
			item.rcItem = rcItem;
			item.rcItemExpandSel = rcItem;
			item.rcItemExpandSel.left-=TN_ITEMIDENT;
			item.rcItemExpandSel.right=item.rcItemExpandSel.left+TN_ITEMIDENT;

			dwPosY+=TN_ITEMHEIGHT;
			dwMaxDepth=INT_MAX;

		//Item invisible
		} else {
			item.rcItem.top=item.rcItem.bottom=0;	//Set Invisible
		}

		//depth
		if (!item.bExpanded && dwMaxDepth==INT_MAX)
			dwMaxDepth = item.dwDepth;

	}


	/////////////////////////////////////////////////
	//Display Items
	for (udword i=0; i<m_carrayItemsSortedPos.Count(); i++)
	{
		udword idx = m_carrayItemsSortedPos[i];
		NTNItemDesc& item = m_carrayItems[idx];

		if (item.rcItem.IsEmpty())
			continue;

		//dc.SetPen(1, RGBA(64,64,64));

		//Row Background
		if (m_dwCurItemIdx==idx)	dc.FillSolidRect(item.rcItem,	RGBA(160,159,167,255));
		//else if (i%2)						dc.FillSolidRect(item.rcItem,	RGBA(107,107,107));
		//else										dc.FillSolidRect(item.rcItem,	RGBA(113,113,113));

		//Display Text
		NRect rcText = item.rcItem;
		rcText.left+=((item.dwDepth+1)*TN_ITEMIDENT);
    dc.DrawString(item.pcobj->GetName(), rcText, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE, RGBA(0,0,0,255) );

		//Symbol (+) or (-)
		if (IsParentItem(i))
		{
			item.rcItemExpandSel=item.rcItem;
			item.rcItemExpandSel.right=rcText.left;

			NRect rcSymb = item.rcItemExpandSel;
			rcSymb.left = rcSymb.right-TN_ITEMIDENT;
			rcSymb.Deflate(4,4);

			if (item.bExpanded)
			{
				//(-) v
				NPoint pts[3];
				pts[0].x = rcSymb.left;		pts[0].y = rcSymb.top;
				pts[1].x = rcSymb.right;	pts[1].y = rcSymb.top;
				pts[2].x = rcSymb.left+rcSymb.Width()/2;		pts[2].y = rcSymb.bottom;
				dc.Polygon(pts, 3, RGBA(0,0,0,255));

			} else {
				//(+) >
				NPoint pts[3];
				pts[0].x = rcSymb.left;		pts[0].y = rcSymb.top;
				pts[1].x = rcSymb.right;	pts[1].y = rcSymb.top+rcSymb.Height()/2;
				pts[2].x = rcSymb.left;		pts[2].y = rcSymb.bottom;
				dc.Polygon(pts, 3, RGBA(0,0,0,255));

			}

		} else {
			item.rcItemExpandSel.top=item.rcItemExpandSel.bottom=0;
		}

	}


}


void NTreeNodeCtrl::OnSize()
{
	//Redraw
	Update();
	//::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}


void NTreeNodeCtrl::OnLButtonDown(NPoint _point)
{
	//TRACE("OnLButtonDown\n");

	SetFocus();

	//expand/collapse Item
	udword dwrowIdx = GetExpandItemIdxUnderPt(_point);
	if (dwrowIdx!=-1)
	{
		ExpandItemFromIdx(dwrowIdx, !IsItemExpanded(dwrowIdx) );
	}

	//Select Item
	dwrowIdx = GetItemIdxUnderPt(_point);
	if (dwrowIdx!=-1)
	{
		SelectItemFromIdx(dwrowIdx);
	}

	//MAJ
	Update();

}

void NTreeNodeCtrl::OnLButtonUp(NPoint point)
{
	/*
	if (m_bAddValue)
	{
		m_bAddValue=false;
		ReleaseCapture();
		::ShowCursor(TRUE);
	}
	*/

}


void NTreeNodeCtrl::OnLButtonDblClk(NPoint point)
{
	/*
	//Row selection
	int nidx = GetRowUnderPoint(point);
	SelectRow(nidx);

	//Row Click
	ClickRow(nidx, point);

	//Start Edit Cell
	if (!m_bEditingRow && m_dwCurSelRow!=-1)
	{
		//Edit Cell
		StartRowEditing(m_dwCurSelRow);
	}
	*/
}


void NTreeNodeCtrl::OnRButtonDown(NPoint point)
{
	SetFocus();
}

void NTreeNodeCtrl::OnMouseMove(NPoint point )
{
}


void NTreeNodeCtrl::OnMButtonDown(NPoint pos)
{
	SetFocus();
}

void NTreeNodeCtrl::OnMButtonUp(NPoint pos)
{
}


void NTreeNodeCtrl::OnKeyDown(udword dwchar)
{
	//GetGUISubSystem()->GetMainWnd()->OnKeyDown(dwchar);
}

void NTreeNodeCtrl::Clear()
{
	m_carrayItemsSortedPos.Clear();
	m_carrayItems.Clear();
	m_prootNode=null;
	m_dwCurItemIdx=-1;
}

void NTreeNodeCtrl::DisplayTreeNode(NTreeNode* _proot)
{
	if (m_prootNode != _proot)
		Clear();

	//Updating
	m_prootNode = _proot;
	m_carrayItemsSortedPos.Clear();
	m_dwLastUpdate = (udword)clock();

	if (m_prootNode!=null)
		_DisplayTreeNode(m_prootNode, 0, 0);

	//Deleting unused items
	for (udword i=0; i<m_carrayItems.Count(); i++)
	{
		if (m_carrayItems[i].dwLastUpdate!=m_dwLastUpdate)
		{
			DeleteItemFromIdx(i);
			i--;
		}
	}

}


void NTreeNodeCtrl::_DisplayTreeNode(NTreeNode* _pParent, udword _dwCurDepth, udword _dwCurPos)
{
	//Sons
	NTreeNode* pnode = _pParent->GetSon();
	while (pnode)
	{
		//Parent
		InsertItem(pnode, _dwCurDepth, _dwCurPos);
		_dwCurPos++;

		//Objects array
		NObjectArray& array = pnode->GetObjsArray();
		for (udword i=0; i<array.Count(); i++)
		{
			NObject* pobj = array[i];
			InsertItem(pobj, _dwCurDepth+1, _dwCurPos);
			_dwCurPos++;
		}

		//Son Nodes
		_DisplayTreeNode(pnode, _dwCurDepth+1, _dwCurPos);

		//Next brothers nodes
		pnode = pnode->GetBrother();
	}

}

void NTreeNodeCtrl::InsertItem(NObject* _pcobj, udword _dwCurDepth, udword _dwCurPos)
{
	udword idx = GetItemIdxFromObject(_pcobj);

	//Adding new node
	if (idx==-1)
	{
		NTNItemDesc			st;
		st.bExpanded		= false;
		st.dwDepth			= _dwCurDepth;
		st.pcobj				= _pcobj;
		st.dwLastUpdate	= m_dwLastUpdate;
		idx = m_carrayItems.AddItem(st);
		m_carrayItemsSortedPos.SetAtGrow(_dwCurPos, idx);

	//Update node position and depth
	} else {
		m_carrayItems[idx].dwDepth			= _dwCurDepth;
		m_carrayItems[idx].dwLastUpdate	= m_dwLastUpdate;
		m_carrayItemsSortedPos.SetAtGrow(_dwCurPos, idx);
	}

}

udword NTreeNodeCtrl::GetItemIdxFromObject(NObject* _pcobj)
{
	for (udword i=0; i<m_carrayItems.Count(); i++)
	{
		NTNItemDesc& item = m_carrayItems[i];
		if (item.pcobj == _pcobj)
			return i;
	}

	return -1;
}


udword NTreeNodeCtrl::GetExpandItemIdxUnderPt(NPoint& _point)
{
	for (udword i=0; i<m_carrayItems.Count(); i++)
	{
		NTNItemDesc& item = m_carrayItems[i];
		if (item.rcItemExpandSel.Contain( _point ))
			return i;
	}
	return -1;
}

udword NTreeNodeCtrl::GetItemIdxUnderPt(NPoint& _point)
{
	for (udword i=0; i<m_carrayItems.Count(); i++)
	{
		NTNItemDesc& item = m_carrayItems[i];
		if (item.rcItem.Contain( _point ))
			return i;
	}
	return -1;
}


void NTreeNodeCtrl::ExpandItemFromIdx(udword _idx, bool _bExpand)
{
	if (_idx<m_carrayItems.Count())
	{
		NTNItemDesc& item = m_carrayItems[_idx];
		item.bExpanded=_bExpand;
	}
}

bool NTreeNodeCtrl::IsItemExpanded(udword _idx)
{
	if (_idx<m_carrayItems.Count())
	{
		NTNItemDesc& item = m_carrayItems[_idx];
		return item.bExpanded;
	}
	return false;
}


void NTreeNodeCtrl::SelectItemFromIdx(udword _idx)
{
	m_dwCurItemIdx=-1;
	if (_idx<m_carrayItems.Count())
	{
		m_dwCurItemIdx=_idx;
		OnSelChange(this);
	}
}

bool NTreeNodeCtrl::IsParentItem(udword _nSortedIdx)
{
	if (_nSortedIdx+1< m_carrayItemsSortedPos.Count())
	{
		udword dwDepth0 = m_carrayItems[ m_carrayItemsSortedPos[_nSortedIdx+0] ].dwDepth;
		udword dwDepth1 = m_carrayItems[ m_carrayItemsSortedPos[_nSortedIdx+1] ].dwDepth;
		if (dwDepth0 <  dwDepth1)
			return true;
	}

	return false;
}

NObject* NTreeNodeCtrl::GetSelectedItemObj()
{
	if (m_dwCurItemIdx!=-1)
		return m_carrayItems[m_dwCurItemIdx].pcobj;

	return null;
}

void NTreeNodeCtrl::DeleteItemFromIdx(udword _idx)
{
	if (_idx<m_carrayItems.Count())
	{
		m_carrayItems.RemoveItem(_idx);

		for (udword i=0; i<m_carrayItemsSortedPos.Count(); i++)
		{
			if (m_carrayItemsSortedPos[i]>_idx)
				m_carrayItemsSortedPos[i]=m_carrayItemsSortedPos[i]-1;
		}

	}

}

udword NTreeNodeCtrl::GetSortedIdxFromIdx(udword _idx)
{
	for (udword i=0; i<m_carrayItemsSortedPos.Count(); i++)
	{
		udword idx = m_carrayItemsSortedPos[i];
		if (idx==_idx)	return i;
	}
	return -1;
}

NObject* NTreeNodeCtrl::GetParent(NObject* _pobjSon)
{
	udword idx = GetItemIdxFromObject(_pobjSon);
	if (idx!=-1)
	{
		sdword sortedIdx = (sdword)GetSortedIdxFromIdx(idx);

		while (sortedIdx>=1 )
		{
			udword dwDepth0 = m_carrayItems[ m_carrayItemsSortedPos[sortedIdx-1] ].dwDepth;
			udword dwDepth1 = m_carrayItems[ m_carrayItemsSortedPos[sortedIdx+0] ].dwDepth;
			if (dwDepth0 <  dwDepth1)
			{
				return m_carrayItems[ m_carrayItemsSortedPos[sortedIdx-1] ].pcobj;
			}

			sortedIdx--;
		}

	}

	return null;
}
