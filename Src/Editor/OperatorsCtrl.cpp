//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorsCtrl.cpp
//! \brief	GUI control for operators editing
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
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "OperatorsCtrl.h"
#include "Editor.h"


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NOperatorsCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NOperatorsCtrl::NOperatorsCtrl()
{
	m_fPosX=m_fPosY=0.0f;
	m_bPanning=m_bMovingBloc=m_bResizingBloc=m_bSelectingRect=false;
	m_fScaleX=m_fScaleY=1.0f;

	m_popMarkedShow			= null;
	m_popMarkedSelected = null;
	m_hfontNormal				= null;
	m_dwSeedValue				= 123345646;
	m_popsPage					= null;
	m_dwCursorX = m_dwCursorY = 0;

	m_fupdateTime = 0.0f;
}


//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NOperatorsCtrl::~NOperatorsCtrl()
{
	if (m_hfontNormal)	::DeleteObject(m_hfontNormal);
	ClearClipboard();
}

bool NOperatorsCtrl::Create(char* name, NRect& rect, NWnd* parent)
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

	//Create Normal Font
	m_hfontNormal = ::CreateFont(
		10, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Lucida console");

	return true;
}

LRESULT NOperatorsCtrl::WndProc( UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg==WM_ERASEBKGND)		return 1;

	//Else Call Base Class
	else return NWnd::WndProc(msg, wparam, lparam);
}


void NOperatorsCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics dc(this);
	dc.FillSolidRect(rc, RGB(115,115,115));
	dc.SetBkMode(TRANSPARENT);

	/////////////////////////////////////////////////
	//Draw Grid
	dc.SetPen(1, GB_GRIDCOLOR);

	float fStartX = fmod(m_fPosX, GB_GRIDUNIT) * m_fScaleX;
	float fStartY = fmod(m_fPosY, GB_GRIDUNIT) * m_fScaleY;
	float fMaxX = (float)rc.Width();
	float fMaxY = (float)rc.Height();
	float	fIncX	= GB_GRIDUNIT * m_fScaleX;
	float	fIncY	= GB_GRIDUNIT * m_fScaleY;

	if (m_fScaleX<1.0f)		fIncX*=2.0f;
	if (m_fScaleY<1.0f)		fIncY*=2.0f;

	for (float y=fStartY; y<fMaxY; y+=fIncY)
	{
		dc.MoveTo(rc.left,	(sdword)y);
		dc.LineTo(rc.right, (sdword)y);
	}
	for (float x=fStartX; x<fMaxX; x+=fIncX)
	{
		dc.MoveTo((sdword)x, rc.top);
		dc.LineTo((sdword)x, rc.bottom);
	}

	/////////////////////////////////////////////////
	//Draw Operators Bloc
	if (m_popsPage!=null)
	{
		dc.SetPen(1, RGB(0,0,0));
		dc.SetFont(m_hfontNormal);

		for (udword i=0; i<m_popsPage->GetOpsCount(); i++)
		{
			NOperator* pop = m_popsPage->GetOpFromIdx(i);
			DisplayOperator(&dc, pop);
		}

	} else {
		dc.SetFont(m_hfontNormal);
		dc.SetTextColor(RGB(200,255,200));
		dc.DrawText("Please select a page !", rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}

	/////////////////////////////////////////////////
	//Draw selecting rect
	if (m_bSelectingRect)
	{
		NRect rcZone(m_ptStartRect.x, m_ptStartRect.y, m_ptEndRect.x, m_ptEndRect.y);
		dc.Draw3dRect(rcZone, RGB(255,255,255), RGB(255,255,255));
	}

	/////////////////////////////////////////////////
	//Draw Cursor
	DisplayCursor(&dc);



}


void NOperatorsCtrl::DisplayOperator(NGraphics* _pdc, NOperator* _pop)
{
	//Operator's Zone
	NRect rcBloc;
	GetOperatorRect(_pop, rcBloc);

	//Shadow
	//NRect rcShadow(rcBloc);
	//rcShadow.Move(2,2);
	//_pdc->FillSolidRect(rcShadow, RGB(0,0,0) );

	//Background color
	_pdc->GradientVRect(rcBloc, RGB(220, 220, 220), _pop->GetColor());

	//Operator's Caption
	NRect rcCap(rcBloc);
	rcCap.bottom=rcCap.top+3;
	if (!_pop->m_bError)		_pdc->FillSolidRect(rcCap, _pop->GetColor());
	else										_pdc->FillSolidRect(rcCap, RGB(255,0,0));

	//Operator marked show
	NRect rcMark(rcBloc);
	rcMark.top=rcMark.top+(3*m_fScaleY);		rcMark.bottom=rcMark.top+(3*m_fScaleY);
	rcMark.left+=(3*m_fScaleX);							rcMark.right=rcMark.left+(8*m_fScaleX);

	if (m_popMarkedShow==_pop)		_pdc->FillSolidRect(rcMark, RGB(0,128,0));

	//Operator marked selected
	rcMark.Move(0,8);
	if (m_popMarkedSelected==_pop)	 _pdc->FillSolidRect(rcMark, RGB(128,0,0));

	//Operator's Contour on fct de la selection
	if (IsOperatorSelected(_pop))	_pdc->Draw3dRect(rcBloc,RGB(0,0,0),RGB(255,255,255));
	else													_pdc->Draw3dRect(rcBloc,RGB(255,255,255),RGB(0,0,0));

	//Operator's Name
	//_pdc->DrawText(_pop->GetRTClass()->m_pszClassName, rcBloc, DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS|DT_VCENTER);
	_pdc->DrawText(_pop->GetName(), rcBloc, DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS|DT_VCENTER);

	//Operator's Resize Zone
	NRect rcResize(rcBloc);
	rcResize.top+=2;
	rcResize.left=rcResize.right-6;
	_pdc->Draw3dRect(rcResize, RGB(220,220,220), RGB(0,0,0));

	//Operator Invalid or Processed
	if (_pop->m_bInvalided || _pop->m_fProcessedTime==m_fupdateTime)
	{
		DWORD dwSeed = _pop->m_fProcessedTime;
		udword y = rcResize.top + (dwSeed % rcResize.Height());
		_pdc->MoveTo(rcResize.left, y);
		_pdc->LineTo(rcResize.right, y);
	}

}


void NOperatorsCtrl::DisplayCursor(NGraphics* _pdc)
{
	if (m_bMovingBloc)
	{
		POINT pt;		::GetCursorPos(&pt);
		NPoint point(pt.x, pt.y);
		ScreenToClient(point);

		NPoint ptOffset = point - m_ptStartBloc;

		sdword dwOffsetX = (sdword)	((m_nStartBlocPosX  + ( ((float)ptOffset.x/m_fScaleX)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_wPosX);
		sdword dwOffsetY = (sdword)	((m_nStartBlocPosY  + ( ((float)ptOffset.y/m_fScaleY)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_wPosY);

		//TRACE("%d %d\n", dwOffsetX, dwOffsetY);

		for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		{
			NOperator* pop = m_carrayOpsSelected[i];

			sdword x = pop->m_wPosX+dwOffsetX;
			sdword y = pop->m_wPosY+dwOffsetY;

			NRect rc;
			rc.left		= (sdword) ((( x * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
			rc.top		= (sdword) ((( y * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
			rc.right	= (sdword) (rc.left + (pop->m_wWidth * GB_GRIDUNIT * m_fScaleX));
			rc.bottom	= (sdword) (rc.top  + (1 * GB_GRIDUNIT * m_fScaleY));
			_pdc->Draw3dRect(rc, RGB(128,64,64), RGB(0,0,128));
		}

	} else {

		NRect rc;
		rc.left		= (sdword) ((( m_dwCursorX * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
		rc.top		= (sdword) ((( m_dwCursorY * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
		rc.right	= (sdword) (rc.left + (1 * GB_GRIDUNIT * m_fScaleX));
		rc.bottom	= (sdword) (rc.top  + (1 * GB_GRIDUNIT * m_fScaleY));
		_pdc->Draw3dRect(rc, RGB(64,64,128), RGB(0,0,128));

	}


}

void NOperatorsCtrl::SetCursorPos(sdword _dwX, sdword _dwY)
{
	m_dwCursorX=_dwX;
	m_dwCursorY=_dwY;
}



void NOperatorsCtrl::OnSize()
{
	//Redraw
	::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}


void NOperatorsCtrl::OnLeftButtonDown(udword flags, NPoint point)
{
	SetFocus();

	//Operator Selected
	bool bResizeZone;
	NOperator* pstBloc = GetOperatorAt(point, bResizeZone);

	if (!(flags&MK_CONTROL))
	{
		if (!IsOperatorSelected(pstBloc) )
			SelectOperator(null);	//Clear Selection
	}

	SelectOperator(pstBloc);

	//Operator Move or resizing
	if (m_popMarkedSelected)
	{
		SetCapture();

		if (!bResizeZone)		m_bMovingBloc=true;
		else								m_bResizingBloc=true;

		m_ptStartBloc			= point;
		m_nStartBlocPosX	= m_popMarkedSelected->m_wPosX;
		m_nStartBlocPosY	= m_popMarkedSelected->m_wPosY;
		m_nStartBlocWidth	= m_popMarkedSelected->m_wWidth;

	//Start Selection Rect
	} else {

		SetCapture();
		m_bSelectingRect	= true;
		m_ptStartRect			= point;
		m_ptEndRect				= point;
	}

	//Move cursor
	if (!bResizeZone)
	{
		//Move cursor
		sdword x = (sdword) (((float)point.x/m_fScaleX)-m_fPosX) / GB_GRIDUNIT;
		sdword y = (sdword) (((float)point.y/m_fScaleY)-m_fPosY) / GB_GRIDUNIT;
		SetCursorPos(x,y);
	}

	Update();

}

void NOperatorsCtrl::OnLeftButtonUp(udword flags, NPoint point)
{
	//Stop operators moving
	if (m_bMovingBloc)
	{
		ReleaseCapture();
		m_bMovingBloc=false;

		NPoint ptOffset = point - m_ptStartBloc;

		sdword dwOffsetX = (sdword)	((m_nStartBlocPosX  + ( ((float)ptOffset.x/m_fScaleX)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_wPosX);
		sdword dwOffsetY = (sdword)	((m_nStartBlocPosY  + ( ((float)ptOffset.y/m_fScaleY)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_wPosY);

		if (dwOffsetX || dwOffsetY)
		{
			for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
			{
				NOperator* pop = m_carrayOpsSelected[i];
				m_popsPage->MoveOp(pop, sword(pop->m_wPosX+dwOffsetX), sword(pop->m_wPosY+ dwOffsetY));
			}
			Update();
		}
	}

	//Stop Operator Resizing
	if (m_bResizingBloc)
	{
		ReleaseCapture();
		m_bResizingBloc=false;

		for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		{
			NOperator* pop = m_carrayOpsSelected[i];
			m_popsPage->MoveOp(pop, pop->m_wPosX, pop->m_wPosY);	//###TOFIX###
		}

		Update();
	}

	//Stop Operator Rect Selection
	if (m_bSelectingRect)
	{
		ReleaseCapture();
		m_bSelectingRect=false;

		NRect rcZone(m_ptStartRect.x, m_ptStartRect.y, m_ptEndRect.x, m_ptEndRect.y);
		SelectOperatorsIntoRect(rcZone);

		Update();
	}

}

void NOperatorsCtrl::OnRightButtonDown(udword flags, NPoint point)
{
	SetFocus();
}

void NOperatorsCtrl::OnMouseMove(udword flags, NPoint point )
{
	//Cursor Aspect
	UpdateCursor(point);

	NRect rc = GetClientRect();

	if (m_bPanning)
	{
		NPoint ptOffset = point - m_ptStartPan;
		m_fPosX=m_fStartPosXPan + (float)ptOffset.x/m_fScaleX;
		m_fPosY=m_fStartPosYPan + (float)ptOffset.y/m_fScaleY;
		if (m_fPosX>0.0f)	m_fPosX=0.0f;
		if (m_fPosY>0.0f)	m_fPosY=0.0f;
		Update();
	}

	if (m_bMovingBloc)
	{
		Update();
	}

	if (m_bResizingBloc)
	{

		NPoint ptOffset = point - m_ptStartBloc;
		sdword dwOffsetW = (sdword) ((m_nStartBlocWidth + ((((float)ptOffset.x/m_fScaleX)) / GB_GRIDUNIT)) - m_popMarkedSelected->m_wWidth);

		for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		{
			NOperator* pop = m_carrayOpsSelected[i];
			pop->m_wWidth+= (sword)dwOffsetW;
			if (pop->m_wWidth<1)		pop->m_wWidth=1;
		}

		Update();
	}

	if (m_bSelectingRect)
	{
		m_ptEndRect = point;
		Update();
	}
}

void NOperatorsCtrl::OnMouseWheel(udword flags, sword zDelta, NPoint pos)
{
	ScreenToClient(pos);

	//Change reference (project screen to grid)
	float fx0 = pos.x;
	float fy0 = pos.y;
	fx0/=m_fScaleX;
	fy0/=m_fScaleY;
	fx0-=m_fPosX;
	fy0-=m_fPosY;

	//Change scale
	m_fScaleX+=(float)zDelta / WHEEL_DELTA;
	if (m_fScaleX<0.2f)	m_fScaleX=0.2f;
	if (m_fScaleX>2.0f)	m_fScaleX=2.0f;
	m_fScaleY=m_fScaleX;

	//Scale on mouse target point
	m_fPosX	= -fx0 + ((float)pos.x/m_fScaleX);
	m_fPosY	= -fy0 + ((float)pos.y/m_fScaleY);

	//Update
	Update();
}


void NOperatorsCtrl::OnMButtonDown(udword flags, NPoint pos)
{
	SetFocus();
	SetCapture();

	m_bPanning = true;
	m_ptStartPan = pos;
	m_fStartPosXPan= m_fPosX;
	m_fStartPosYPan= m_fPosY;
}

void NOperatorsCtrl::OnMButtonUp(udword flags, NPoint pos)
{
	ReleaseCapture();
	m_bPanning = false;
}

void NOperatorsCtrl::OnKeyDown(udword dwchar)
{
	//Delete selected operators
	if (dwchar==VK_DELETE)
	{
		DeleteOperatorsSelected();
		Update();

	//Mark operator as "show"
	}	else if (dwchar=='S')	{

		OnMarkShowOperator(m_popMarkedSelected);
		Update();

	//Copy selected operators to clipboard
	} else if (dwchar=='C') {

		CopyOperatorsSelectedToClipboard();

	} else if (dwchar=='V') {

		PasteOperatorsFromClipboard();

	}



}


void NOperatorsCtrl::UpdateCursor(NPoint& pt)
{
	bool bResizeZone;

	//Sous un op
	NOperator* pop = GetOperatorAt(pt, bResizeZone);
	if (pop)
	{
		//In Operator resizable zone
		if (bResizeZone)	::SetCursor(::LoadCursor(null, IDC_SIZEWE));
		else							::SetCursor(::LoadCursor(null, IDC_HAND));
	}

	//Panning
	if (m_bPanning)
		::SetCursor(::LoadCursor(null, IDC_SIZEALL));
}



void NOperatorsCtrl::DisplayOperatorsPage(NOperatorsPage* popsPage)
{
	Reset();

	m_popsPage = popsPage;
}



void NOperatorsCtrl::PlaceOperator(ID CLASSID)
{
	//Place New Operator
	NOperator* pop = AddOperator((sword)m_dwCursorX,(sword)m_dwCursorY,4,CLASSID);

	//clear selection
	SelectOperator(null);

	//Selection du op
	SelectOperator(pop);

}


NOperator* NOperatorsCtrl::AddOperator(sword x, sword y, sword w, ID CLASSID)
{
	if (m_popsPage==null)		return null;

	//Create operator
	NOperator* pop = (NOperator*)NRTClass::CreateByID(CLASSID);
	if (pop)
	{
		//Init
		pop->m_wPosX			= x;
		pop->m_wPosY			= y;
		pop->m_wWidth			= w;
		pop->m_byDepth		= 0;
		pop->m_byInputs		= 0;
		pop->m_bInvalided	= true;

		//Add operator
		m_popsPage->AddOp(pop);
	}
	return pop;
}


void NOperatorsCtrl::DeleteOperator(NOperator* pop)
{

	if (pop==m_popMarkedShow)				m_popMarkedShow=null;
	if (pop==m_popMarkedSelected)		m_popMarkedSelected=null;

	EVT_EXECUTE(EVT_OPDELETING, (DWORD)pop, 0);	//###TOFIX###

	OnDeletingOperator(pop);

	m_popsPage->DeleteOp(pop);

	OnDeletedOperator(pop);

}


void NOperatorsCtrl::Reset()
{
	m_carrayOpsSelected.Clear();

	m_fPosX=m_fPosY=0.0f;
	m_bPanning=m_bMovingBloc=m_bResizingBloc=false;
	m_popMarkedSelected=null;
	m_popMarkedShow=null;

	m_fScaleX=m_fScaleY=1.0f;
	m_popsPage = null;
}


void NOperatorsCtrl::DeleteOperatorsSelected()
{
	for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
	{
		DeleteOperator(m_carrayOpsSelected[i]);
	}

	m_carrayOpsSelected.Clear();
}


NOperator* NOperatorsCtrl::GetOperatorAt(NPoint& pt, bool& bResizeZone)
{
	bResizeZone = false;

	if (m_popsPage)
	{
		for (udword i=0; i<m_popsPage->GetOpsCount() ; i++)
		{
			NOperator* pop = m_popsPage->GetOpFromIdx(i);
			NRect rcItem;
			GetOperatorRect(pop, rcItem);
			if (rcItem.Contain(pt))
			{
				NRect rcResize(rcItem);
				rcResize.top+=4;
				rcResize.left=rcResize.right-8;
				if (rcResize.Contain(pt))		bResizeZone = true;

				return pop;
			}
		}

	}

	return null;
}


void NOperatorsCtrl::SelectOperator(NOperator* pop)
{
	if (pop==null)
	{
		m_carrayOpsSelected.Clear();

	} else {

		if (!IsOperatorSelected(pop))
			m_carrayOpsSelected.AddItem(pop);
	}

	//###TOMOVE###
	m_popMarkedSelected = pop;
	EVT_EXECUTE(EVT_OPSELECTED, (DWORD)pop, 0);
}

void NOperatorsCtrl::SelectOperatorsIntoRect(NRect& rc)
{
	if (!m_popsPage)	return;

	rc.Normalize();
	if (rc.IsEmpty())		return;

	m_carrayOpsSelected.Clear();

	for (udword i=0; i<m_popsPage->GetOpsCount(); i++)
	{
		NOperator* pop = m_popsPage->GetOpFromIdx(i);
		NRect rcItem;
		GetOperatorRect(pop, rcItem);
		if (!rc.IsIntersected(rcItem))
			m_carrayOpsSelected.AddItem(pop);
	}


}


bool NOperatorsCtrl::IsOperatorSelected(NOperator* pop)
{
	for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		if (m_carrayOpsSelected[i] == pop)
			return true;

	return false;
}


void NOperatorsCtrl::OnMarkShowOperator(NOperator* pop)
{
	m_popMarkedShow = pop;
}


void NOperatorsCtrl::Update()
{
	::RedrawWindow(m_W32HWnd, null, null, RDW_INVALIDATE | RDW_UPDATENOW );
}

void NOperatorsCtrl::Update(float _ftime)
{
	m_fupdateTime = _ftime;
	Update();
}

void NOperatorsCtrl::GetOperatorRect(NOperator* _pop, NRect& _rc)
{
	_rc.left		= (sdword) ((( _pop->m_wPosX * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
	_rc.top			= (sdword) ((( _pop->m_wPosY * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
	_rc.right		= (sdword) (_rc.left + (_pop->m_wWidth * GB_GRIDUNIT * m_fScaleX));
	_rc.bottom	= (sdword) (_rc.top  + (1 * GB_GRIDUNIT * m_fScaleY));
}


void NOperatorsCtrl::CopyOperatorsSelectedToClipboard()
{
	ClearClipboard();

	for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
	{
		//Duplicate operator
		NOperator* popOriginal = m_carrayOpsSelected[i];
		NOperator* popClone = (NOperator*)popOriginal->Duplicate();

		//Add to Array
		m_carrayOpsClipboard.AddItem( popClone );
	}

}

void NOperatorsCtrl::PasteOperatorsFromClipboard()
{
	if (m_carrayOpsClipboard.Count()==0)	return;

	//Calc offset
	sdword dwOffsetX = m_dwCursorX - (sdword)m_carrayOpsClipboard[0]->m_wPosX;
	sdword dwOffsetY = m_dwCursorY - (sdword)m_carrayOpsClipboard[0]->m_wPosY;

	//Paste operators
	for (udword i=0; i<m_carrayOpsClipboard.Count(); i++)
	{
		NOperator* pop = m_carrayOpsClipboard[i];

		//Init
		pop->m_wPosX			= sword(pop->m_wPosX+dwOffsetX);
		pop->m_wPosY			= sword(pop->m_wPosY+dwOffsetY);

		//Add operator
		m_popsPage->AddOp(pop);
	}

	m_carrayOpsClipboard.Clear();

	//Refresh
	Update();
}

void NOperatorsCtrl::ClearClipboard()
{
	for (udword i=0; i<m_carrayOpsClipboard.Count(); i++)
	{
		delete m_carrayOpsClipboard[i];
	}
	m_carrayOpsClipboard.Clear();
}