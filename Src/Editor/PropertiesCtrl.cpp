//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PrpertiesCtrl.cpp
//! \brief	GUI control for properties editing
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

//-----------------------------------------------------------------
//			Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "PropertiesCtrl.h"

#ifdef __GNUC__
#include <limits.h>
#endif

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NPropertiesCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NPropertiesCtrl::NPropertiesCtrl()
{
	m_hfontNormal = m_hfontBold = null;
	m_dwCurSelRow	= -1;
	m_bEditingRow	= false;
	m_bAddValue		= false;
}


//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NPropertiesCtrl::~NPropertiesCtrl()
{
	if (m_hfontNormal)	::DeleteObject(m_hfontNormal);
	if (m_hfontBold)		::DeleteObject(m_hfontBold);
}

//-----------------------------------------------------------------
//!	\brief	control Creation
//-----------------------------------------------------------------
bool NPropertiesCtrl::Create(const char* name, const NRect& rect, NWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.Id						= 1;
	wc.Name					= const_cast<char*>(name);
	wc.Parent				= parent;
	wc.Rect					= rect;
	wc.W32ClassName	= "FXGen_WNDCLASS";
	wc.W32Style			= WS_CHILD | WS_CLIPCHILDREN;   //WS_CLIPSIBLINGS;
	wc.W32StyleEx		= WS_EX_ACCEPTFILES;
	NWnd::Create(wc);

	//Create Normal Font
	m_hfontNormal = ::CreateFont(
		10, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Lucida console");

	//Create Bold Font
	m_hfontBold = ::CreateFont(
		12, 0, 0, 0, FW_BOLD, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Lucida console");


	return true;
}

//-----------------------------------------------------------------
//!	\brief	control messages
//-----------------------------------------------------------------
LRESULT NPropertiesCtrl::WndProc( UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg==WM_ERASEBKGND)		return 1;

	//Else Call Base Class
	else return NWnd::WndProc(msg, wparam, lparam);
}

//-----------------------------------------------------------------
//!	\brief	Update display
//-----------------------------------------------------------------
void NPropertiesCtrl::Update()
{
	//::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
	::RedrawWindow(m_W32HWnd, null, null, RDW_INVALIDATE | RDW_UPDATENOW /*| RDW_ERASE*/ );
}

//-----------------------------------------------------------------
//!	\brief	Display
//-----------------------------------------------------------------
void NPropertiesCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.SetBkMode(TRANSPARENT);

	/////////////////////////////////////////////////
	//Erase Background
	m_dwHeaderWidth = rc.Width()/2;
	dc.FillSolidRect(rc, RGB(115,115,115));

	/////////////////////////////////////////////////
	//Display Rows
	dc.SetPen(1, RGB(140,140,140));

	sdword dwMaxDepth=INT_MAX;
	sdword dwPosY=0;

	for (udword i=0; i<m_carrayRowsDesc.Count(); i++)
	{
		NRowDesc* prd = &m_carrayRowsDesc[i];

		/////////////////////////////////////////////
		//Group
		if (prd->pItem==null && prd->dwDepth<=dwMaxDepth)
		{
			dc.SetTextColor(RGB(255,255,255));
			dc.SetFont(m_hfontBold);
			if (i!=0)				dwPosY+=PC_ROWGROUPIDENT;

			NRect rcRow;
			rcRow.left  = rc.left;	rcRow.top			= dwPosY;
			rcRow.right = rc.right;	rcRow.bottom	= dwPosY+ PC_ROWSHEIGHT - 1;
			rcRow.left+=(prd->dwDepth*PC_ROWDEPTHIDENT);

			prd->rcItem = rcRow;

			dc.SetPen(1,RGB(0,0,0));
			dc.SetBrush(RGB(0,0,0));
			dc.RoundRect(rcRow, PC_ROWSHEIGHT,PC_ROWSHEIGHT);

			rcRow.top+=PC_ROWSHEIGHT/2;
			dc.FillSolidRect(rcRow, RGB(0,0,0));

			rcRow.top-=(PC_ROWSHEIGHT/2)-2;
			rcRow.left+=PC_ROWTEXTIDENT;
			dc.DrawText(prd->strName.Buffer(), rcRow, DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

			dwPosY+=PC_ROWSHEIGHT;

			dwMaxDepth=INT_MAX;

		/////////////////////////////////////////////
		//Variable
		} else if (prd->pItem!=null && prd->dwDepth<=dwMaxDepth) {

			dc.SetTextColor(RGB(0,0,0));
			dc.SetFont(m_hfontNormal);

			//Display Left Part
			NRect rcRow;
			rcRow.left = rc.left;		rcRow.top = dwPosY;
			rcRow.right = m_dwHeaderWidth;	rcRow.bottom = rcRow.top + PC_ROWSHEIGHT - 1;

			if (m_dwCurSelRow!=i)		dc.FillSolidRect(rcRow,	RGB(200,200,200));
			else										dc.FillSolidRect(rcRow,	RGB(200,230,200));

			rcRow.left+=(prd->dwDepth*PC_ROWDEPTHIDENT);
			dc.DrawText(prd->strName.Buffer(), rcRow, DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

			//Check if value can be animated
			bool bCanBeAnimate = prd->pItem->m_pvarBlocDesc->bCanBeAnimate;
			bool bAnimated		 = prd->pItem->m_pvarValue->pcCtrlObj!=null;

			//Display Right Part
			rcRow.left = rcRow.right;
			rcRow.right = rc.right;

			//Store Row Rect
			prd->rcItem = rcRow;

			//leave place for animate button
			if (bCanBeAnimate)
				rcRow.right-=PC_ROWSHEIGHT;

			//Display row
			dc.FillSolidRect(rcRow, RGB(255,255,255));
			prd->pItem->DrawItem(&dc, rcRow);

			//Display button if variable can be animated
			if (bCanBeAnimate)
			{
				NRect rcABut;
				rcABut.left=rcRow.right;		rcABut.top=rcRow.top;
				rcABut.right=rc.right;			rcABut.bottom=rcRow.bottom;
				dc.Draw3dRect(rcABut, RGB(255,255,255), RGB(0,0,0));
				if (!bAnimated)		dc.DrawText("A", rcABut, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				else							dc.DrawText("R", rcABut, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

				prd->rcItemAnim=rcABut;
			} else {
				prd->rcItemAnim.left=prd->rcItemAnim.right=0;
			}

			dwPosY+=PC_ROWSHEIGHT;
		}

		//depth
		if (!prd->bExpanded && dwMaxDepth==INT_MAX)
			dwMaxDepth = prd->dwDepth;

	}


}

//-----------------------------------------------------------------
//!	\brief	control resize message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnSize()
{
	//Redraw
	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}

//-----------------------------------------------------------------
//!	\brief	control Left button down message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnLeftButtonDown(udword flags, NPoint point)
{
	SetFocus();

	//TRACE("OnLeftButtonDown\n");
	udword idx = GetRowUnderPoint(point);

	//Row selection and AddValue
	if (idx!=-1 && !m_bAddValue)
	{
		//Row expand (if group only)
		if (!ExpandRow(idx))
		{
			SelectRow(idx);
			ClickRow(idx, point);

			bool bAnimButton = IsAnimButtonUnderPoint(point);

			//Value sliding
			if (m_dwCurSelRow!=-1 && !bAnimButton)
			{
				POINT pt; 	::GetCursorPos(&pt);
				m_ptCursor.x = pt.x;	m_ptCursor.y = pt.y;
				m_ptStartMouse = point;
				::SetCursorPos(m_ptCursor.x, m_ptCursor.y);
				SetCapture();
				::ShowCursor(FALSE);

				m_bAddValue = true;
			}

			if (bAnimButton)
			{
				AddRemoveAnimControlToRow(m_dwCurSelRow);
			}

		}


	}

	//Refresh
	Update();


}

//-----------------------------------------------------------------
//!	\brief	control right button up message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnLeftButtonUp(udword flags, NPoint point)
{
	if (m_bAddValue)
	{
		m_bAddValue=false;
		ReleaseCapture();
		::ShowCursor(TRUE);
	}

}

//-----------------------------------------------------------------
//!	\brief	control right button down message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnRightButtonDown(udword flags, NPoint point)
{
	SetFocus();
}


//-----------------------------------------------------------------
//!	\brief	control left button double click message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnLeftButtonDblClk(udword flags, NPoint point)
{
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

}


//-----------------------------------------------------------------
//!	\brief	control mouse mouve message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnMouseMove(udword flags, NPoint point )
{
	//On fait varier les valeurs de la ligne
	// en deplacant vers la gauche ou vers la droite la souris
	if (m_bAddValue)
	{
		sdword dwOffset =	point.x - m_ptStartMouse.x;
		//TRACE("dwOffset <%d> point<%d> ptStartMouse<%d> ptCursor<%d,%d>\n", dwOffset, point.x, m_ptStartMouse.x, m_ptCursor.x, m_ptCursor.y);

		if (dwOffset)
		{
			OffsetRowValue(dwOffset);
			::SetCursorPos(m_ptCursor.x, m_ptCursor.y);
			Update();
		}
	}

}

//-----------------------------------------------------------------
//!	\brief	control left button double click message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnMButtonDown(udword flags, NPoint pos)
{
	SetFocus();
}

//-----------------------------------------------------------------
//!	\brief	control middle button up message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnMButtonUp(udword flags, NPoint pos)
{
}

//-----------------------------------------------------------------
//!	\brief	control key down message
//-----------------------------------------------------------------
void NPropertiesCtrl::OnKeyDown(udword dwchar)
{
	GetApp()->GetMainWnd()->OnKeyDown(dwchar);
}

//-----------------------------------------------------------------
//!	\brief	Display properties
//-----------------------------------------------------------------
void NPropertiesCtrl::DisplayObjectProperties(NObject* _pobj)
{
	EndRowEditing(true);

	//Clear current display
	DeleteAllProperties();

	//Store
	m_pobj=_pobj;

	//Show object properties
	if (_pobj!=null)
	{
		_DisplayObjectProperties(_pobj,0);
		_DisplayAnimationObjectProperties(_pobj,0);
	}

}

//-----------------------------------------------------------------
//!	\brief	Display properties (recurse)
//-----------------------------------------------------------------
void NPropertiesCtrl::_DisplayObjectProperties(NObject* _pobj, udword _dwDepth)
{
	//Get First Object var bloc
	NVarsBloc* pbloc = _pobj->GetFirstVarsBloc();

	while (pbloc)
	{
		NVarsBlocDesc*	pblocdesc	= pbloc->GetBlocDesc();
		NVarValue*			pvalues		= pbloc->GetValues();

		//Add a Group for this bloc
		AddGroup(_pobj->GetName(), _dwDepth);	//##TOFIX### variable bloc name

		//Parse Vars for this bloc
		for (udword i=0; i<pbloc->Count(); i++)
		{
			//Add a row for each vars
			AddVarProperties(pbloc, pblocdesc+i, pvalues+i, i, _dwDepth+1);
		}

		//pblocdesc=pblocdesc->	//###TOFIX### Next Bloc
		pbloc=null;	//###TOFIX####
	}

}

//-----------------------------------------------------------------
//!	\brief	Display animation properties (recurse)
//-----------------------------------------------------------------
void NPropertiesCtrl::_DisplayAnimationObjectProperties(NObject* _pobj, udword _dwDepth)
{
	//Get First Object var bloc
	NVarsBloc* pbloc = _pobj->GetFirstVarsBloc();

	while (pbloc)
	{
		NVarsBlocDesc*	pblocdesc	= pbloc->GetBlocDesc();
		NVarValue*			pvalues		= pbloc->GetValues();

		//Parse Vars for this bloc
		for (udword i=0; i<pbloc->Count(); i++)
		{
			//Animation for this variable?
			NObject* pctrlObj = pvalues[i].pcCtrlObj;
			if (pctrlObj!=null)
			{
				NString str;
				str.Format("Animation %s", pblocdesc[i].pszName );
				pctrlObj->SetName(str.Buffer());	//##TOFIX### variable bloc name

				//Display properties for this controler
				_DisplayObjectProperties(pctrlObj, _dwDepth);
			}

		}

		//pblocdesc=pblocdesc->	//###TOFIX### Next Bloc
		pbloc=null;	//###TOFIX####
	}


}




//-----------------------------------------------------------------
//!	\brief	Add a group
//-----------------------------------------------------------------
udword NPropertiesCtrl::AddGroup(char* pszName, udword _dwDepth)
{
	NRowDesc				rd;
	rd.dwDepth			= _dwDepth;
	rd.strName			= pszName;
	rd.bExpanded		= true;
	rd.pItem				= null;	//!< null for a group
	return m_carrayRowsDesc.AddItem(rd);
}


//-----------------------------------------------------------------
//!	\brief	Add a var property
//-----------------------------------------------------------------
udword NPropertiesCtrl::AddVarProperties(NVarsBloc* _pvarBloc, NVarsBlocDesc* _pvarBlocDesc, NVarValue* _pvarValue, udword _dwvarIdx, udword _dwDepth)
{
	NRowDesc				rd;
	rd.strName			= _pvarBlocDesc->pszName;
	rd.bExpanded		= true;
	rd.dwDepth			= _dwDepth;

	//udword dwParentRowIdx =	GetRowIdxFromGroupIdx(_dwGroupIdx);
	//NRowDesc* pparentRow = GetRowAtIdx(dwParentRowIdx);

	//pparentRow?rd.dwDepth = pparentRow->dwDepth+1:0;

	rd.pItem = (NPropertyItem*)NRTClass::CreateByName(_pvarBlocDesc->pszCLASSGUI);
	if (rd.pItem)
	{
		rd.pItem->m_pParent				= this;

		rd.pItem->m_pvarBloc			= _pvarBloc;
		rd.pItem->m_pvarBlocDesc	= _pvarBlocDesc;
		rd.pItem->m_pvarValue			= _pvarValue;
		rd.pItem->m_dwvarIdx			= _dwvarIdx;
	}

	return m_carrayRowsDesc.AddItem(rd);
}

//-----------------------------------------------------------------
//!	\brief	Delete all properties
//-----------------------------------------------------------------
void NPropertiesCtrl::DeleteAllProperties()
{
	for (udword i=0; i<m_carrayRowsDesc.Count(); i++)
	{
		NRowDesc* prd = &m_carrayRowsDesc[i];
		if (prd->pItem)		delete prd->pItem;
	}

	m_carrayRowsDesc.Clear();
}

//-----------------------------------------------------------------
//!	\brief	Return row description from idx
//-----------------------------------------------------------------
NRowDesc* NPropertiesCtrl::GetRowAtIdx(udword idx)
{
	if (idx>=m_carrayRowsDesc.Count())
		return null;

	return &m_carrayRowsDesc[idx];
}

//-----------------------------------------------------------------
//!	\brief	Return row idx from a position
//-----------------------------------------------------------------
udword NPropertiesCtrl::GetRowUnderPoint(NPoint& pt)
{
	for (udword i=0; i<m_carrayRowsDesc.Count(); i++)
	{
		NRowDesc* prd = &m_carrayRowsDesc[i];
		if (prd->rcItem.Contain(pt))
			return i;
	}

	return -1;
}

//-----------------------------------------------------------------
//!	\brief	Add property's row Value
//-----------------------------------------------------------------
void NPropertiesCtrl::OffsetRowValue(sdword dwOffset)
{
	if (m_dwCurSelRow==-1 || !m_bAddValue)		return;

	if (m_carrayRowsDesc[m_dwCurSelRow].pItem->AddValue(dwOffset) && dwOffset)
	{
		//TRACE("Row Value %s\n", m_carrayRowsDesc[m_dwCurSelRow].pItem->GetValue());

		//Send Message
		OnPropertiesChanged();
	}

}

//-----------------------------------------------------------------
//!	\brief	Select a row from an index
//-----------------------------------------------------------------
bool NPropertiesCtrl::SelectRow(udword dwRowIdx)
{
	if (dwRowIdx>=m_carrayRowsDesc.Count() || dwRowIdx<0)
		return false;

	if (m_carrayRowsDesc[dwRowIdx].pItem==null)
		return false;

	EndRowEditing();
	m_dwCurSelRow = dwRowIdx;
	return true;
}

//-----------------------------------------------------------------
//!	\brief	Expand a row
//-----------------------------------------------------------------
bool NPropertiesCtrl::ExpandRow(udword _dwRowIdx)
{
	if (_dwRowIdx>=m_carrayRowsDesc.Count() || _dwRowIdx<0)
		return false;

	if (m_carrayRowsDesc[_dwRowIdx].pItem==null)
	{
		m_carrayRowsDesc[_dwRowIdx].bExpanded = !m_carrayRowsDesc[_dwRowIdx].bExpanded;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------
//!	\brief	Dispatch Click message to a row
//-----------------------------------------------------------------
void NPropertiesCtrl::ClickRow(udword dwRowIdx, NPoint& pt)
{
	if (dwRowIdx!=-1)
	{
		if (dwRowIdx>=m_carrayRowsDesc.Count())
			return;

		if (m_carrayRowsDesc[dwRowIdx].pItem!=null)
			m_carrayRowsDesc[dwRowIdx].pItem->Click(pt, m_carrayRowsDesc[dwRowIdx].rcItem);
	}

}

//-----------------------------------------------------------------
//!	\brief	Start Row editing
//-----------------------------------------------------------------
void NPropertiesCtrl::StartRowEditing(udword dwRowIdx)
{
	if (dwRowIdx==-1)		return;

	EndRowEditing(true);

	//Edit Cell
	m_bEditingRow=true;

	NRowDesc* pRow = &m_carrayRowsDesc[dwRowIdx];
	if (pRow->pItem->BeginEdit(pRow->rcItem))
	{
		EndRowEditing(true);	//BeginEdit = true => Fin de l'edition Demande

	} else {

		Update();
	}

}

//-----------------------------------------------------------------
//!	\brief	Stop Row editing
//-----------------------------------------------------------------
void NPropertiesCtrl::EndRowEditing(bool bSaveChanged/*=true*/)
{
	if (m_bEditingRow && m_dwCurSelRow!=-1)
	{
		NRowDesc* pRow = &m_carrayRowsDesc[m_dwCurSelRow];
		if (pRow->pItem->EndEdit(bSaveChanged))
		{
			Update();							//EndEdit = true => Changement valide
			OnPropertiesChanged();
		}

		m_bEditingRow=false;
	}

}

//-----------------------------------------------------------------
//!	\brief	Return true if an anim button is under a position
//-----------------------------------------------------------------
bool NPropertiesCtrl::IsAnimButtonUnderPoint(NPoint& _pt)
{
	for (udword i=0; i<m_carrayRowsDesc.Count(); i++)
	{
		NRowDesc* prd = &m_carrayRowsDesc[i];
		if (prd->rcItemAnim.Contain(_pt))
			return true;
	}
	return false;
}

//-----------------------------------------------------------------
//!	\brief	Add or remove an animation controler at a porperty's row
//-----------------------------------------------------------------
void NPropertiesCtrl::AddRemoveAnimControlToRow(udword _dwRowIdx)
{
	NRowDesc* prd = &m_carrayRowsDesc[_dwRowIdx];
	bool bCanBeAnimate = prd->pItem->m_pvarBlocDesc[prd->pItem->m_dwvarIdx].bCanBeAnimate;
	if (bCanBeAnimate && prd->pItem->m_pvarValue->pcCtrlObj==null)
	{
		prd->pItem->m_pvarValue->pcCtrlObj = new NController();
		DisplayObjectProperties(m_pobj);
	} else if (prd->pItem->m_pvarValue->pcCtrlObj) {
		delete prd->pItem->m_pvarValue->pcCtrlObj;
		prd->pItem->m_pvarValue->pcCtrlObj=null;
		DisplayObjectProperties(m_pobj);
	}

	//Send Message
	OnPropertiesChanged();

}


/*udword NPropertiesCtrl::GetRowIdxFromGroupIdx(udword _groupIdx)
{
	udword groupidx=-1;
	for (udword i=0; i<m_carrayRowsDesc.Count(); i++)
	{
		NRowDesc* prd = &m_carrayRowsDesc[i];
		if (prd->pItem==null)		groupidx++;
		if (groupidx==_groupIdx)	return i;
	}

	return -1;
}*/
