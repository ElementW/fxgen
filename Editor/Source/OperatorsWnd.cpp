//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorsWnd.cpp
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
#include "OperatorsWnd.h"
#include "EditorApp.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NOperatorsWnd Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NOperatorsWnd::NOperatorsWnd()
{
	m_fPosX=m_fPosY=0.0f;
	m_bPanning=m_bMovingBloc=m_bResizingBloc=m_bSelectingRect=false;
	m_fScaleX=m_fScaleY=1.0f;

	m_popMarkedShow			= null;
	m_popMarkedSelected = null;
	m_dwSeedValue				= 123345646;
	m_popsPage					= null;
	m_dwCursorX = m_dwCursorY = 0;

	m_fupdateTime = 0.0f;
	m_bAskedForMove = false;
	m_bMovingBloc=false;
}


//-----------------------------------------------------------------
//		Destructor
//-----------------------------------------------------------------
NOperatorsWnd::~NOperatorsWnd()
{
	ClearClipboard();
	EVT_UNREGISTERALL();
}

bool NOperatorsWnd::Create(const char* name, const NRect& rect, NGUIWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= (char*)name;
	wc.pwndParent		= parent;
	wc.rcRect				= rect;
	wc.dwStyle			= NWS_VISIBLE;
	NGUIWnd::Create(wc);

	//Init context Menu
	InitCtxMenu();

	//Register m_dwWidth
	EVT_REGISTER(EVT_PAGESELECTED, &NOperatorsWnd::OnPageSelected );
	EVT_REGISTER(EVT_RENDER, &NOperatorsWnd::OnRendering );

	return true;
}


void NOperatorsWnd::OnPaint()
{
	NRect rc = GetClientRect();

	N2DPainter dc(this);
	dc.FillSolidRect(rc, RGBA(115,115,115,255));

	/////////////////////////////////////////////////
	//Draw Grid
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
		dc.DrawLine(rc.left,	(sdword)y, rc.right, (sdword)y, GB_GRIDCOLOR, 1);
	}
	for (float x=fStartX; x<fMaxX; x+=fIncX)
	{
		dc.DrawLine((sdword)x, rc.top, (sdword)x, rc.bottom, GB_GRIDCOLOR, 1);
	}

	/////////////////////////////////////////////////
	//Draw Operators Bloc
	m_rcAllOperators.left=m_rcAllOperators.top=1000000000;	//##TOFIX###
	m_rcAllOperators.right=m_rcAllOperators.bottom=0;

	if (m_popsPage!=null)
	{
		for (udword i=0; i<m_popsPage->GetOpsCount(); i++)
		{
			NOperatorNode* pop = m_popsPage->GetOpFromIdx(i);
			DisplayOperator(&dc, pop);
		}

	} else {
    dc.DrawString("Please select a page !", rc, NDT_HCENTER|NDT_VCENTER|NDT_SINGLELINE, RGBA(0,0,0,255) );
	}

	/////////////////////////////////////////////////
	//Draw selecting rect
	if (m_bSelectingRect)
	{
		NRect rcZone(m_ptStartRect.x, m_ptStartRect.y, m_ptEndRect.x, m_ptEndRect.y);
		dc.FillSolidRect(rcZone, RGBA(200,55,55,128));
	}

	/////////////////////////////////////////////////
	//Draw Cursor
	DisplayCursor(&dc);

	/////////////////////////////////////////////////
	//Draw Global Operators View
	DisplayOperatorsMap(&dc);

}


void NOperatorsWnd::DisplayOperator(N2DPainter* _pdc, NOperatorNode* _pop)
{
	//Operator's Zone
	NRect rcBloc;
	GetOperatorRect(_pop, rcBloc);

	if (rcBloc.left<m_rcAllOperators.left)
		m_rcAllOperators.left=rcBloc.left;
	if (rcBloc.right>m_rcAllOperators.right)
		m_rcAllOperators.right=rcBloc.right;

	if (rcBloc.top<m_rcAllOperators.top)
		m_rcAllOperators.top=rcBloc.top;
	if (rcBloc.bottom>m_rcAllOperators.bottom)
		m_rcAllOperators.bottom=rcBloc.bottom;

	//Shadow
	NRect rcShadow(rcBloc);
	rcShadow.Move(2,2);
	_pdc->FillSolidRect(rcShadow, RGBA(0,0,0,128) );

	//Background color
	_pdc->GradientVRect(rcBloc, RGBA(220, 220, 220,255), _pop->GetColor());

	//Operator's Caption
	NRect rcCap(rcBloc);
	rcCap.bottom=rcCap.top+3;
	if (!_pop->m_bError)		_pdc->FillSolidRect(rcCap, _pop->GetColor());
	else										_pdc->FillSolidRect(rcCap, RGBA(255,0,0,255));

	//Operator marked show
	NRect rcMark(rcBloc);
	rcMark.top=rcMark.top+(3*m_fScaleY);		rcMark.bottom=rcMark.top+(3*m_fScaleY);
	rcMark.left+=(3*m_fScaleX);							rcMark.right=rcMark.left+(8*m_fScaleX);

	if (m_popMarkedShow==_pop)		_pdc->FillSolidRect(rcMark, RGBA(0,128,0,255));

	//Operator marked selected
	rcMark.Move(0,8);
	if (m_popMarkedSelected==_pop)	 _pdc->FillSolidRect(rcMark, RGBA(128,0,0,255));

	//Operator's Contour on fct de la selection
	if (IsOperatorSelected(_pop))	_pdc->Draw3dRect(rcBloc,RGBA(0,0,0,255),RGBA(255,255,255,255));
	else													_pdc->Draw3dRect(rcBloc,RGBA(255,255,255,255),RGBA(0,0,0,255));

	//Operator's Name
  const char* pszname = _pop->GetGUIName();
	if (pszname==null || (strlen(pszname)==0))		pszname=_pop->GetName();

  _pdc->DrawString(pszname, rcBloc, NDT_SINGLELINE|NDT_HCENTER|NDT_END_ELLIPSIS|NDT_VCENTER, RGBA(0,0,0,255) );

	//Operator's Resize Zone
	NRect rcResize(rcBloc);
	rcResize.top+=2;
	rcResize.left=rcResize.right-6;
	_pdc->Draw3dRect(rcResize, RGBA(255,220,220,220), RGBA(0,0,0,255));

	//Operator Invalid or Processed
	if (_pop->m_bInvalided || _pop->m_fProcessedTime==m_fupdateTime)
	{
		udword dwSeed = _pop->m_fProcessedTime;
		udword y = rcResize.top + (dwSeed % rcResize.Height());
		_pdc->DrawLine(rcResize.left, y, rcResize.right, y, RGBA(0,0,0,255), 1);
	}

}


void NOperatorsWnd::DisplayCursor(N2DPainter* _pdc)
{
	if (m_bMovingBloc && m_popMarkedSelected)
	{
		NPoint point;
		GetGUISubSystem()->GetCursorPos(point);
		ScreenToClient(point);

		NPoint ptOffset = point - m_ptStartBloc;

		sdword dwOffsetX = (sdword)	((m_nStartBlocPosX  + ( ((float)ptOffset.x/m_fScaleX)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_dwPosX);
		sdword dwOffsetY = (sdword)	((m_nStartBlocPosY  + ( ((float)ptOffset.y/m_fScaleY)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_dwPosY);

		//TRACE("%d %d\n", dwOffsetX, dwOffsetY);

		for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		{
			NOperatorNode* pop = m_carrayOpsSelected[i];

			//New position
			sdword x = pop->m_dwPosX+dwOffsetX;
			sdword y = pop->m_dwPosY+dwOffsetY;

			NRect rc;
			rc.left		= (sdword) ((( x * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
			rc.top		= (sdword) ((( y * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
			rc.right	= (sdword) (rc.left + (pop->m_dwWidth * GB_GRIDUNIT * m_fScaleX));
			rc.bottom	= (sdword) (rc.top  + (1 * GB_GRIDUNIT * m_fScaleY));

			//Background color
			_pdc->GradientVRect(rc, RGBA(255,255,190,190), pop->GetColor());

			//Operator's Caption
			NRect rcCap(rc);
			rcCap.bottom=rcCap.top+3;
			_pdc->FillSolidRect(rcCap, pop->GetColor());

			//Operator's Name
      const char* pszname = pop->GetGUIName();
			if (pszname==null || (strlen(pszname)==0))		pszname=pop->GetName();
      _pdc->DrawString(pszname, rc, NDT_SINGLELINE|NDT_HCENTER|NDT_END_ELLIPSIS|NDT_VCENTER, RGBA(0,0,0,255) );

		}

	} else {

		NRect rc;
		rc.left		= (sdword) ((( m_dwCursorX * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
		rc.top		= (sdword) ((( m_dwCursorY * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
		rc.right	= (sdword) (rc.left + (1 * GB_GRIDUNIT * m_fScaleX));
		rc.bottom	= (sdword) (rc.top  + (1 * GB_GRIDUNIT * m_fScaleY));
		_pdc->FillSolidRect(rc, RGBA(64,64,128,128));

	}


}


void NOperatorsWnd::DisplayOperatorsMap(N2DPainter* _pdc)
{
	if (m_popsPage==null)		return;

	udword w=100,h=64;

	//Zone Rect
	NRect rcClient = GetClientRect();
	if (m_rcAllOperators.Width()<rcClient.Width() &&
			m_rcAllOperators.Height()<rcClient.Height()		)
		return;

	NRect rc(rcClient);
	rc.right-=16;
	rc.top+=16;
	rc.left		= rc.right - w;
	rc.bottom = rc.top + h;
	_pdc->FillSolidRect(rc, RGBA(128,128,128,100));

	//Calc Min and Max operators zone
	float fScaleX, fScaleY;
	fScaleX = ((float)w/(float)m_rcAllOperators.Width()) * m_fScaleX;
	fScaleY = ((float)h/(float)m_rcAllOperators.Height())* m_fScaleY;

	//Display operators
	for (udword i=0; i<m_popsPage->GetOpsCount() ; i++)
	{
		NOperatorNode* pop = m_popsPage->GetOpFromIdx(i);

		NRect rcBlock;
		rcBlock.left		= (sdword) ((( pop->m_dwPosX * GB_GRIDUNIT) ) * fScaleX);
		rcBlock.top			= (sdword) ((( pop->m_dwPosY * GB_GRIDUNIT) ) * fScaleY);
		rcBlock.right		= (sdword) (rcBlock.left + (pop->m_dwWidth * GB_GRIDUNIT * fScaleX));
		rcBlock.bottom	= (sdword) (rcBlock.top  + (1 * GB_GRIDUNIT * fScaleY));
		rcBlock.Move(rc.left, rc.top);
		//Just Background color
		_pdc->GradientVRect(rcBlock, RGBA(255,255,190,190), pop->GetColor());

	}

	//user View rect
	NRect rcV;
	float fStartX = -m_fPosX * fScaleX;
	float fStartY = -m_fPosY * fScaleY;

	rcV.left=rc.left+fStartX;
	rcV.top=rc.top+fStartY;
	rcV.right =rcV.left+((float)(rcClient.Width()/m_fScaleX) * fScaleX);
	rcV.bottom=rcV.top +((float)(rcClient.Height()/m_fScaleY)* fScaleY);
	_pdc->Draw3dRect(rcV, RGBA(255,255,255,100), RGBA(255,255,255,100));

}


void NOperatorsWnd::SetOpCursorPos(sdword _dwX, sdword _dwY)
{
	m_dwCursorX=_dwX;
	m_dwCursorY=_dwY;
}



void NOperatorsWnd::OnSize()
{
	//Redraw
	Update();
}


void NOperatorsWnd::OnLButtonDown(NPoint point)
{
	SetFocus();

	//Operator Selected
	bool bResizeZone;
	NOperatorNode* pstBloc = GetOperatorAt(point, bResizeZone);

	if (!(GetGUISubSystem()->IsKeyDown(NKey::RControl)))
	{
		if (!IsOperatorSelected(pstBloc) )
			SelectOperator(null);	//Clear Selection
	}

	SelectOperator(pstBloc);

	//Operator Move or resizing
	if (m_popMarkedSelected)
	{
		//SetCapture();

		if (!bResizeZone)		m_bAskedForMove=true;
		else								m_bResizingBloc=true;

		m_ptStartBloc			= point;
		m_nStartBlocPosX	= m_popMarkedSelected->m_dwPosX;
		m_nStartBlocPosY	= m_popMarkedSelected->m_dwPosY;
		m_nStartBlocWidth	= m_popMarkedSelected->m_dwWidth;


	//Start Selection Rect
	} else {

		//SetCapture();
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
		SetOpCursorPos(x,y);
	}

	Update();

}

void NOperatorsWnd::OnLButtonUp(NPoint point)
{
	//Stop operators moving
	if (m_bAskedForMove)
	{
		m_bAskedForMove=false;
	}

	if (m_bMovingBloc && m_popMarkedSelected)
	{
		//ReleaseCapture();
		m_bMovingBloc=false;
		m_bAskedForMove=false;

		NPoint ptOffset = point - m_ptStartBloc;

		sdword dwOffsetX = (sdword)	((m_nStartBlocPosX  + ( ((float)ptOffset.x/m_fScaleX)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_dwPosX);
		sdword dwOffsetY = (sdword)	((m_nStartBlocPosY  + ( ((float)ptOffset.y/m_fScaleY)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_dwPosY);

		if (dwOffsetX || dwOffsetY)
		{
			if (!IsMovedSelOperatorsCollide(dwOffsetX, dwOffsetY))
			{
				for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
				{
					NOperatorNode* pop = m_carrayOpsSelected[i];
					m_popsPage->MoveOp(pop, sword(pop->m_dwPosX+dwOffsetX), sword(pop->m_dwPosY+ dwOffsetY));
				}
			}
			Update();
		}
	}

	//Stop Operator Resizing
	if (m_bResizingBloc)
	{
		//ReleaseCapture();
		m_bResizingBloc=false;

		for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		{
			NOperatorNode* pop = m_carrayOpsSelected[i];
			m_popsPage->MoveOp(pop, pop->m_dwPosX, pop->m_dwPosY);	//###TOFIX###
		}

		Update();
	}

	//Stop Operator Rect Selection
	if (m_bSelectingRect)
	{
		//ReleaseCapture();
		m_bSelectingRect=false;

		NRect rcZone(m_ptStartRect.x, m_ptStartRect.y, m_ptEndRect.x, m_ptEndRect.y);
		SelectOperatorsIntoRect(rcZone);

		Update();
	}

}

//-----------------------------------------------------------------
//!	\brief	Windows Message Right button down
//-----------------------------------------------------------------
void NOperatorsWnd::OnRButtonDown(NPoint point)
{
	SetFocus();
	ClientToScreen(point);
	m_wndMenu.TrackPopupMenu(point);
}

void NOperatorsWnd::OnLButtonDblClk(NPoint point)
{
	SetFocus();

	//Select operator
	bool bResizeZone;
	NOperatorNode* pstBloc = GetOperatorAt(point, bResizeZone);
	SelectOperator(pstBloc);

	//Mark operator as "show"
	OnMarkShowOperator(m_popMarkedSelected);
	Update();
}


void NOperatorsWnd::OnMouseMove(NPoint point )
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

	if (m_bAskedForMove)
	{
		NPoint ptOffset = point - m_ptStartBloc;
		if (abs(ptOffset.x)>4 || abs(ptOffset.y)>4)
		{
			m_bMovingBloc = true;
			m_bAskedForMove=false;
		}
	}

	if (m_bMovingBloc)
	{
		Update();
	}

	if (m_bResizingBloc)
	{

		NPoint ptOffset = point - m_ptStartBloc;
		sdword dwOffsetW = (sdword) ((m_nStartBlocWidth + ((((float)ptOffset.x/m_fScaleX)) / GB_GRIDUNIT)) - m_popMarkedSelected->m_dwWidth);

		for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		{
			NOperatorNode* pop = m_carrayOpsSelected[i];
			pop->m_dwWidth+= (sword)dwOffsetW;
			if (pop->m_dwWidth<1)		pop->m_dwWidth=1;
		}

		Update();
	}

	if (m_bSelectingRect)
	{
		m_ptEndRect = point;
		Update();
	}
}

void NOperatorsWnd::OnMouseWheel(NPoint pos, sdword zDelta)
{
	//ScreenToClient(pos);

	//Change reference (project screen to grid)
	float fx0 = pos.x;
	float fy0 = pos.y;
	fx0/=m_fScaleX;
	fy0/=m_fScaleY;
	fx0-=m_fPosX;
	fy0-=m_fPosY;

	//Change scale
	//m_fScaleX+=(float)zDelta / WHEEL_DELTA;
	if (zDelta>0)	m_fScaleX+=0.2f;
	if (zDelta<0)	m_fScaleX-=0.2f;

	if (m_fScaleX<0.2f)	m_fScaleX=0.2f;
	if (m_fScaleX>2.0f)	m_fScaleX=2.0f;
	m_fScaleY=m_fScaleX;

	//Scale on mouse target point
	m_fPosX	= -fx0 + ((float)pos.x/m_fScaleX);
	m_fPosY	= -fy0 + ((float)pos.y/m_fScaleY);

	//Update
	Update();
}


void NOperatorsWnd::OnMButtonDown(NPoint pos)
{
	SetFocus();
	SetCapture();

	m_bPanning = true;
	m_ptStartPan = pos;
	m_fStartPosXPan= m_fPosX;
	m_fStartPosYPan= m_fPosY;
}

void NOperatorsWnd::OnMButtonUp(NPoint pos)
{
	ReleaseCapture();
	m_bPanning = false;
}

void NOperatorsWnd::OnKeyUp(udword dwchar)
{
	//Delete selected operators
	if (dwchar==NKey::Delete)
	{
		DeleteOperatorsSelected();
		Update();

	//Mark operator as "show"
	}	else if (dwchar==NKey::S)	{

		OnMarkShowOperator(m_popMarkedSelected);
		Update();

	//Copy selected operators to clipboard
	} else if (dwchar==NKey::C) {

		CopyOperatorsSelectedToClipboard();

	} else if (dwchar==NKey::V) {

		PasteOperatorsFromClipboard();
	}

}


void NOperatorsWnd::UpdateCursor(NPoint& pt)
{
	bool bResizeZone;

	//Sous un op
	//###WIN32### TOFIX
/*	NOperatorNode* pop = GetOperatorAt(pt, bResizeZone);
	if (pop)
	{
		//In Operator resizable zone
		if (bResizeZone)	::SetCursor(::LoadCursor(null, IDC_SIZEWE));
		else							::SetCursor(::LoadCursor(null, IDC_HAND));
	}

	//Panning
	if (m_bPanning)
		::SetCursor(::LoadCursor(null, IDC_SIZEALL));*/

}



void NOperatorsWnd::DisplayOperatorsPage(NOperatorsPage* popsPage)
{
	Reset();

	m_popsPage = popsPage;
}



void NOperatorsWnd::PlaceOperator(const char* _pszClassName)
{
	udword dwWidth = 4;

	//Compute to add operator rect
	NRect rcItemAdd;
	rcItemAdd.left		= (sdword) ((( (m_dwCursorX) * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
	rcItemAdd.top			= (sdword) ((( (m_dwCursorY) * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
	rcItemAdd.right		= (sdword) (rcItemAdd.left + (dwWidth * GB_GRIDUNIT * m_fScaleX));
	rcItemAdd.bottom	= (sdword) (rcItemAdd.top  + (1 * GB_GRIDUNIT * m_fScaleY));
	rcItemAdd.Deflate(1,1);

	//If we doesn't have a collision
	if(!IsOpRectCollide(rcItemAdd, false))
	{
		//Place New Operator
		NOperatorNode* pop = AddOperator((sword)m_dwCursorX,(sword)m_dwCursorY,dwWidth,_pszClassName);

		//clear selection
		SelectOperator(null);

		//Selection du op
		SelectOperator(pop);
	}

}


NOperatorNode* NOperatorsWnd::AddOperator(sword x, sword y, sword w, const char* _pszClassName)
{
	if (m_popsPage==null)		return null;

	//Create operator
	NOperatorNode* pop = (NOperatorNode*)NRTClass::CreateByName(_pszClassName);
	if (pop)
	{
		//Init
		pop->m_dwPosX			= x;
		pop->m_dwPosY			= y;
		pop->m_dwWidth			= w;
		pop->m_bInvalided	= true;

		//Add operator
		m_popsPage->AddOp(pop);
	}
	return pop;
}


void NOperatorsWnd::DeleteOperator(NOperatorNode* pop)
{

	if (pop==m_popMarkedShow)				m_popMarkedShow=null;
	if (pop==m_popMarkedSelected)		m_popMarkedSelected=null;

	EVT_EXECUTE(EVT_OPDELETING, (udword)pop, 0);	//###TOFIX###

	OnDeletingOperator(pop);

	m_popsPage->DeleteOp(pop);

	OnDeletedOperator(pop);

}


void NOperatorsWnd::Reset()
{
	m_carrayOpsSelected.Clear();

	m_fPosX=m_fPosY=0.0f;
	m_bPanning=m_bMovingBloc=m_bResizingBloc=false;
	m_popMarkedSelected=null;
	m_popMarkedShow=null;

	m_fScaleX=m_fScaleY=1.0f;
	m_popsPage = null;
}


void NOperatorsWnd::DeleteOperatorsSelected()
{
	for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
	{
		DeleteOperator(m_carrayOpsSelected[i]);
	}

	m_carrayOpsSelected.Clear();
}


NOperatorNode* NOperatorsWnd::GetOperatorAt(NPoint& pt, bool& bResizeZone)
{
	bResizeZone = false;

	if (m_popsPage)
	{
		for (udword i=0; i<m_popsPage->GetOpsCount() ; i++)
		{
			NOperatorNode* pop = m_popsPage->GetOpFromIdx(i);
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


void NOperatorsWnd::SelectOperator(NOperatorNode* pop)
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
	EVT_EXECUTE(EVT_OPSELECTED, (udword)pop, 0);
}

void NOperatorsWnd::SelectOperatorsIntoRect(NRect& rc)
{
	if (!m_popsPage)	return;

	rc.Normalize();
	if (rc.IsEmpty())		return;

	m_carrayOpsSelected.Clear();

	for (udword i=0; i<m_popsPage->GetOpsCount(); i++)
	{
		NOperatorNode* pop = m_popsPage->GetOpFromIdx(i);
		NRect rcItem;
		GetOperatorRect(pop, rcItem);
		if (!rc.IsIntersected(rcItem))
			m_carrayOpsSelected.AddItem(pop);
	}


}


bool NOperatorsWnd::IsOperatorSelected(NOperatorNode* pop)
{
	for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		if (m_carrayOpsSelected[i] == pop)
			return true;

	return false;
}




void NOperatorsWnd::Update()
{
	RedrawWindow();
}

void NOperatorsWnd::Update(float _ftime)
{
	m_fupdateTime = _ftime;
//	Update();	//###GUI### to add later
}

void NOperatorsWnd::GetOperatorRect(NOperatorNode* _pop, NRect& _rc)
{
	_rc.left		= (sdword) ((( _pop->m_dwPosX * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
	_rc.top			= (sdword) ((( _pop->m_dwPosY * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
	_rc.right		= (sdword) (_rc.left + (_pop->m_dwWidth * GB_GRIDUNIT * m_fScaleX));
	_rc.bottom	= (sdword) (_rc.top  + (1 * GB_GRIDUNIT * m_fScaleY));
}


void NOperatorsWnd::CopyOperatorsSelectedToClipboard()
{
	ClearClipboard();

	for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
	{
		//Duplicate operator
		NOperatorNode* popOriginal = m_carrayOpsSelected[i];
		NOperatorNode* popClone = (NOperatorNode*)popOriginal->Duplicate();
		//Add to Array
		m_carrayOpsClipboard.AddItem( popClone );
	}

}

void NOperatorsWnd::PasteOperatorsFromClipboard()
{
	if (m_carrayOpsClipboard.Count()==0)	return;

	//Calc offset
	sdword dwOffsetX = m_dwCursorX - (sdword)m_carrayOpsClipboard[0]->m_dwPosX;
	sdword dwOffsetY = m_dwCursorY - (sdword)m_carrayOpsClipboard[0]->m_dwPosY;

	//Paste operators
	for (udword i=0; i<m_carrayOpsClipboard.Count(); i++)
	{
		NOperatorNode* pop = (NOperatorNode*)m_carrayOpsClipboard[i]->Duplicate();

		//Init
		pop->m_dwPosX			= sword(pop->m_dwPosX+dwOffsetX);
		pop->m_dwPosY			= sword(pop->m_dwPosY+dwOffsetY);

		//Add operator
		m_popsPage->AddOp(pop);
	}

	//m_carrayOpsClipboard.Clear();

	//Refresh
	Update();
}

void NOperatorsWnd::ClearClipboard()
{
	for (udword i=0; i<m_carrayOpsClipboard.Count(); i++)
	{
		NDELETE(m_carrayOpsClipboard[i], NOperatorNode);
	}
	m_carrayOpsClipboard.Clear();
}

bool NOperatorsWnd::IsMovedSelOperatorsCollide(sdword _dwoffsetX, sdword _dwoffsetY)
{

	for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
	{
		NOperatorNode* popSel = m_carrayOpsSelected[i];

		//Compute asked operator rect
		NRect rcItemSel;
		rcItemSel.left		= (sdword) ((( (popSel->m_dwPosX+_dwoffsetX) * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
		rcItemSel.top			= (sdword) ((( (popSel->m_dwPosY+_dwoffsetY) * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
		rcItemSel.right		= (sdword) (rcItemSel.left + (popSel->m_dwWidth * GB_GRIDUNIT * m_fScaleX));
		rcItemSel.bottom	= (sdword) (rcItemSel.top  + (1 * GB_GRIDUNIT * m_fScaleY));
		rcItemSel.Deflate(1,1);

		//Control if one collision with other operators rect
		if (IsOpRectCollide(rcItemSel, true))
			return true;		//One collision found !

	}

	return false;	//No collision found
}

bool NOperatorsWnd::IsOpRectCollide(NRect _rcItemTest, bool _bExcludeSel)
{
	if (m_popsPage)
	{
		for (udword i=0; i<m_popsPage->GetOpsCount() ; i++)
		{
			NOperatorNode* pop = m_popsPage->GetOpFromIdx(i);

			if (_bExcludeSel && m_carrayOpsSelected.Find(pop)!=-1)	continue;

			NRect rcItem;
			GetOperatorRect(pop, rcItem);
			if (!rcItem.IsIntersected(_rcItemTest))
				return true;	//One collision found !
		}
	}

	return false;	//No collision found
}



//-----------------------------------------------------------------
//!	\brief	Context menu creation
//-----------------------------------------------------------------
void NOperatorsWnd::InitCtxMenu()
{
	//Creation du menu
	m_wndMenu.Create("Operators:", this);
	m_wndMenu.OnItemClick=FDelegate(this, (TDelegate)&NOperatorsWnd::OnMenuItemClick);

	//Create Operators list sorted by category
	NRTClass* prtc = NRTClass::GetFirstClassDerivedFrom("NOperatorNode");
	while (prtc)
	{
		//Create operator in order to get operator name and categorie
		NOperatorNode* pop = (NOperatorNode*)prtc->m_pCreateCB();

		//Search if category already exist in menu
		NMenuCtrl* popMenu = null;
		udword count = m_wndMenu.GetItemsCount();
		udword j = 0;
		for (j = 0; j < count; j++)
		{
			NMEItemDesc* pitem = m_wndMenu.GetItemDesc(j);
			if (pitem->strName == NString(pop->GetCategory()))
			{
				popMenu = m_wndMenu.GetPopupMenu(j);
				break;
			}
		}

		//Create category if it doesn't exist
		if (popMenu==null)
		{
			popMenu = m_wndMenu.CreatePopupMenu(pop->GetCategory(), -1);
		}

		//Add new operator
		popMenu->AddItem(pop->GetName(), (udword)prtc, 0);	//###TOFIX### 64bits ptr

		//Delete operator
		NDELETE(pop, NOperatorNode);

		//Next RTC
		prtc = NRTClass::GetNextClassDerivedFrom("NOperatorNode", prtc);
	}

}


//-----------------------------------------------------------------
//!	\brief	Windows Message Command
//-----------------------------------------------------------------
void NOperatorsWnd::OnMenuItemClick(NObject* _psender)
{
	NMenuCtrl* pmenu = (NMenuCtrl*)_psender;
	NRTClass* rtclass = (NRTClass*)pmenu->GetClickedCmdID();
	PlaceOperator(rtclass->m_pszClassName);
	SetFocus();
	Update();
}

//-----------------------------------------------------------------
//!	\brief	An operator is marked show
//-----------------------------------------------------------------
void NOperatorsWnd::OnMarkShowOperator(NOperatorNode* pop)
{
	m_popMarkedShow = pop;

	NFxGenApp* papp = GetApp();
	NEditorGUI* pfrm = (NEditorGUI*)papp->GetMainWnd();
	pfrm->MarkShowOperator(pop);

}

//-----------------------------------------------------------------
//!	\brief	An operator is deleting
//-----------------------------------------------------------------
void NOperatorsWnd::OnDeletingOperator(NOperatorNode* pop)
{
	NFxGenApp* papp = (NFxGenApp*)GetGUISubSystem();
	NEditorGUI* pfrm = (NEditorGUI*)papp->GetMainWnd();
	pfrm->DeletingOperator(pop);
}

//-----------------------------------------------------------------
//!	\brief	An operator is deleted
//-----------------------------------------------------------------
void NOperatorsWnd::OnDeletedOperator(NOperatorNode* pop)
{
	NFxGenApp* papp = (NFxGenApp*)GetGUISubSystem();
	NEditorGUI* pfrm = (NEditorGUI*)papp->GetMainWnd();
	pfrm->DeletedOperator(pop);
}


//-----------------------------------------------------------------
//!	\brief	Event a page is selected
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NOperatorsWnd, OnPageSelected)
{
	NOperatorsPage* popsPage = (NOperatorsPage*)dwParam1;
	DisplayOperatorsPage(popsPage);
	Update();

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	Event rendering asked
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NOperatorsWnd, OnRendering)
{
	float* ftime = (float*)dwParam2;
	Update(*ftime);
	return 0;
}
