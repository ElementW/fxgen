//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorsCtrl.h
//! \brief	GUI control for operators editing
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		02-04-2008
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    		The original version of this library can be located at:
//!    		http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 			Includes
//-----------------------------------------------------------------
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "EventsList.h"

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "EventsList.h"
#include "OperatorsCtrl.h"

//#include "..\Src\FxGenLib\Core.h"
//#include "..\Src\FxGenLib\EngineOp.h"

//-----------------------------------------------------------------
// 			Events
//-----------------------------------------------------------------
DEFINE_EVENT_TYPE(wxEVT_FXGEN_OPERATORSCTRL)

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					wxOperatorsCtrl class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
const wxChar wxOperatorsCtrlNameStr[] = wxT("OperatorsCtrl");

BEGIN_EVENT_TABLE(wxOperatorsCtrl, wxControl)
  EVT_PAINT(wxOperatorsCtrl::OnPaint)
  EVT_MOTION(wxOperatorsCtrl::OnMouseMove)
  EVT_MIDDLE_DOWN(wxOperatorsCtrl::OnMiddleDown)
  EVT_MIDDLE_UP(wxOperatorsCtrl::OnMiddleUp)
  EVT_LEFT_DOWN(wxOperatorsCtrl::OnLeftDown)
  EVT_LEFT_UP(wxOperatorsCtrl::OnLeftUp)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxOperatorsCtrl, wxControl)

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
wxOperatorsCtrl::wxOperatorsCtrl(wxWindow *parent, wxWindowID id,
               const wxPoint& pos, const wxSize& size, long style,
               const wxValidator &validator,  const wxString& name)
    : wxControl(parent, id, pos, size, style, validator, name)
{
	m_fPosX=m_fPosY=0.0f;
	m_bPanning=m_bMovingBloc=m_bResizingBloc=m_bSelectingRect=false;
	m_fScaleX=m_fScaleY=1.0f;

	m_popMarkedShow			= null;
	m_popMarkedSelected = null;
	//m_hfontNormal				= null;
	m_dwSeedValue				= 123345646;
	m_popsPage					= null;
	m_dwCursorX = m_dwCursorY = 0;

	m_fupdateTime = 0.0f;

  //Windows style
  SetBackgroundStyle(wxBG_STYLE_CUSTOM ); //Added for double buffer

  //Font
  m_fontOperator.SetFamily(wxFONTFAMILY_ROMAN);

  //Connectors
  parent->Connect(wsID_FXGEN_PAGE_CHANGED, wxEVT_FXGEN_PROJECT, wxCommandEventHandler( wxOperatorsCtrl::OnFxGenPageChanged ),0,this);
}

//-----------------------------------------------------------------
//!	\brief	Event Painting
//-----------------------------------------------------------------
void wxOperatorsCtrl::OnPaint(wxPaintEvent& event)
{
  wxAutoBufferedPaintDC dc(this); //Double buffered

	/////////////////////////////////////////////////
  //Draw Background
  int w, h;
  GetSize(&w,&h);

  wxRect rcOP;
  rcOP.x = 0;
  rcOP.y = 0;
  rcOP.width = w;
  rcOP.height = h;

  wxColor col1(160,160,160);
  wxColor col2(200,200,200);
  dc.GradientFillLinear(rcOP, col1, col2, wxNORTH);

  //dc.DrawRoundedRectangle(128,128,256,64,8);


	/////////////////////////////////////////////////
	//Draw Grid
  wxPen penGrid;
  penGrid.SetColour( GB_GRIDCOLOR );
  dc.SetPen(penGrid);

	float fStartX = fmod(m_fPosX, GB_GRIDUNIT) * m_fScaleX;
	float fStartY = fmod(m_fPosY, GB_GRIDUNIT) * m_fScaleY;
	float fMaxX = (float)w;
	float fMaxY = (float)h;
	float	fIncX	= GB_GRIDUNIT * m_fScaleX;
	float	fIncY	= GB_GRIDUNIT * m_fScaleY;

	if (m_fScaleX<1.0f)		fIncX*=2.0f;
	if (m_fScaleY<1.0f)		fIncY*=2.0f;

  //Horizontal
	for (float y=fStartY; y<fMaxY; y+=fIncY)
		dc.DrawLine(0,	(sdword)y, w, (sdword)y);

  //Vertical
	for (float x=fStartX; x<fMaxX; x+=fIncX)
		dc.DrawLine((sdword)x, 0, (sdword)x, h);

	/////////////////////////////////////////////////
	//Draw Operators Bloc
	if (m_popsPage!=null)
	{
		//dc.SetPen(1, RGB(0,0,0));
		//dc.SetFont(m_hfontNormal);

    dc.SetFont(m_fontOperator);

		for (udword i=0; i<m_popsPage->GetOpsCount(); i++)
		{
			NOperator* pop = m_popsPage->GetOpFromIdx(i);
			DisplayOperator(&dc, pop);
		}

	} else {
		//dc.SetFont(m_hfontNormal);
		//dc.SetTextColor(RGB(200,255,200));
		//dc.DrawText("Please select a page !", rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}

	/////////////////////////////////////////////////
	//Draw selecting rect
	if (m_bSelectingRect)
	{
		wxRect rcSel(m_ptStartRect.x, m_ptStartRect.y, m_ptEndRect.x-m_ptStartRect.x, m_ptEndRect.y-m_ptStartRect.y);

    //dc.SetPen(*wxTRANSPARENT_PEN);
    //wxBrush brSel;
    //brSel.SetColour(wxColor(100,100,128,0));  //!!!Alpha channel don't supported !!! :-(
    //dc.SetBrush(brSel);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.DrawRectangle(rcSel);
		//dc.Draw3dRect(rcZone, RGB(255,255,255), RGB(255,255,255));
	}

	/////////////////////////////////////////////////
	//Draw Cursor
	DisplayCursor(&dc);


}



//-----------------------------------------------------------------
//!	\brief	Event MouseMove
//-----------------------------------------------------------------
void wxOperatorsCtrl::OnMouseMove(wxMouseEvent& event)
{
  wxPoint pos = event.GetPosition();

  int w, h;
  GetSize(&w,&h);

	//Cursor Aspect
	//UpdateCursor(point);

	if (m_bPanning)
	{
		wxPoint ptOffset = pos - m_ptStartPan;
		m_fPosX=m_fStartPosXPan + (float)ptOffset.x/m_fScaleX;
		m_fPosY=m_fStartPosYPan + (float)ptOffset.y/m_fScaleY;
		if (m_fPosX>0.0f)	m_fPosX=0.0f;
		if (m_fPosY>0.0f)	m_fPosY=0.0f;
		Refresh();
	}

	if (m_bMovingBloc)
	{
		Refresh();
	}

	if (m_bResizingBloc)
	{

		wxPoint ptOffset = pos - m_ptStartBloc;
		sdword dwOffsetW = (sdword) ((m_nStartBlocWidth + ((((float)ptOffset.x/m_fScaleX)) / GB_GRIDUNIT)) - m_popMarkedSelected->m_wWidth);

		for (udword i=0; i<m_carrayOpsSelected.GetCount(); i++)
		{
			NOperator* pop = m_carrayOpsSelected[i];
			pop->m_wWidth+= (sword)dwOffsetW;
			if (pop->m_wWidth<1)		pop->m_wWidth=1;
		}

		Refresh();
	}

	if (m_bSelectingRect)
	{
		m_ptEndRect = pos;
		Refresh();
	}

}

//-----------------------------------------------------------------
//!	\brief	Event Mouse Middle Down
//-----------------------------------------------------------------
void wxOperatorsCtrl::OnMiddleDown(wxMouseEvent& event)
{
  wxPoint pos = event.GetPosition();

	SetFocus();
	CaptureMouse();

	m_bPanning = true;
	m_ptStartPan = pos;
	m_fStartPosXPan= m_fPosX;
	m_fStartPosYPan= m_fPosY;
}

//-----------------------------------------------------------------
//!	\brief	Event Mouse Middle Up
//-----------------------------------------------------------------
void wxOperatorsCtrl::OnMiddleUp(wxMouseEvent& event)
{
	ReleaseMouse();
	m_bPanning = false;
}

//-----------------------------------------------------------------
//!	\brief	Event Mouse Left Up
//-----------------------------------------------------------------
void wxOperatorsCtrl::OnLeftUp(wxMouseEvent& event)
{
  wxPoint pos = event.GetPosition();

	//Stop operators moving
	if (m_bMovingBloc)
	{
		ReleaseMouse();
		m_bMovingBloc=false;

		wxPoint ptOffset = pos - m_ptStartBloc;

		sdword dwOffsetX = (sdword)	((m_nStartBlocPosX  + ( ((float)ptOffset.x/m_fScaleX)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_wPosX);
		sdword dwOffsetY = (sdword)	((m_nStartBlocPosY  + ( ((float)ptOffset.y/m_fScaleY)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_wPosY);

		if (dwOffsetX || dwOffsetY)
		{
			if (!IsMovedSelOperatorsCollide(dwOffsetX, dwOffsetY))
			{
				for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
				{
					NOperator* pop = m_carrayOpsSelected[i];
					m_popsPage->MoveOp(pop, sword(pop->m_wPosX+dwOffsetX), sword(pop->m_wPosY+ dwOffsetY));
				}
			}
			Refresh();
		}
	}

	//Stop Operator Resizing
	if (m_bResizingBloc)
	{
		ReleaseMouse();
		m_bResizingBloc=false;

		for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		{
			NOperator* pop = m_carrayOpsSelected[i];
			m_popsPage->MoveOp(pop, pop->m_wPosX, pop->m_wPosY);	//###TOFIX###
		}

		Refresh();
	}

	//Stop Operator Rect Selection
	if (m_bSelectingRect)
	{
		ReleaseMouse();
		m_bSelectingRect=false;

		wxRect rcZone(m_ptStartRect.x, m_ptStartRect.y, m_ptEndRect.x-m_ptStartRect.x, m_ptEndRect.y-m_ptStartRect.y);
		SelectOperatorsIntoRect(rcZone);

		Refresh();
	}

}

//-----------------------------------------------------------------
//!	\brief	Event Mouse Left Down
//-----------------------------------------------------------------
void wxOperatorsCtrl::OnLeftDown(wxMouseEvent& event)
{
  wxPoint pos = event.GetPosition();
	SetFocus();

	//Operator Selected
	bool bResizeZone;
	NOperator* pstBloc = GetOperatorAt(pos, bResizeZone);

	if (!event.ControlDown())
	{
		if (!IsOperatorSelected(pstBloc) )
			SelectOperator(null);	//Clear Selection
	}

	SelectOperator(pstBloc);

	//Operator Move or resizing
	if (m_popMarkedSelected)
	{
		CaptureMouse();

		if (!bResizeZone)		m_bMovingBloc=true;
		else								m_bResizingBloc=true;

		m_ptStartBloc			= pos;
		m_nStartBlocPosX	= m_popMarkedSelected->m_wPosX;
		m_nStartBlocPosY	= m_popMarkedSelected->m_wPosY;
		m_nStartBlocWidth	= m_popMarkedSelected->m_wWidth;

	//Start Selection Rect
	} else {

		CaptureMouse();
		m_bSelectingRect	= true;
		m_ptStartRect			= pos;
		m_ptEndRect				= pos;
	}

	//Move cursor
	if (!bResizeZone)
	{
		//Move cursor
		sdword x = (sdword) (((float)pos.x/m_fScaleX)-m_fPosX) / GB_GRIDUNIT;
		sdword y = (sdword) (((float)pos.y/m_fScaleY)-m_fPosY) / GB_GRIDUNIT;
		SetCursorPos(x,y);
	}

	Refresh();

}

//-----------------------------------------------------------------
//!	\brief	Display an operators page
//-----------------------------------------------------------------
void wxOperatorsCtrl::DisplayOperatorsPage(NOperatorsPage* _popsPage)
{
	Reset();
	m_popsPage = _popsPage;
}

//-----------------------------------------------------------------
//!	\brief	Reset
//-----------------------------------------------------------------
void wxOperatorsCtrl::Reset()
{
	m_carrayOpsSelected.Clear();

	m_fPosX=m_fPosY=0.0f;
	m_bPanning=m_bMovingBloc=m_bResizingBloc=false;
	m_popMarkedSelected=null;
	m_popMarkedShow=null;

	m_fScaleX=m_fScaleY=1.0f;
	m_popsPage = null;
}


//-----------------------------------------------------------------
//!	\brief  Display an operator
//-----------------------------------------------------------------
void wxOperatorsCtrl::DisplayOperator(wxDC* _pdc, NOperator* _pop)
{
  _pdc->SetPen(*wxTRANSPARENT_PEN);

	//Operator's Zone
	wxRect rcBloc;
	GetOperatorRect(_pop, rcBloc);

	//Background color
	_pdc->GradientFillLinear(rcBloc, wxColor(220,220,220), wxColor(_pop->GetColor()), wxNORTH);

	//Operator's Caption
	wxRect rcCap(rcBloc);
	rcCap.height=3;
  wxBrush brCap;
	if (!_pop->m_bError)		brCap.SetColour(wxColor(_pop->GetColor()));
	else										brCap.SetColour(255,0,0);
	_pdc->SetBrush(brCap);
  _pdc->DrawRectangle(rcCap);

	//Operator marked show
	wxRect rcMark(rcBloc);
	rcMark.x+=(3*m_fScaleX);		rcMark.width=(8*m_fScaleX);
	rcMark.y+=(3*m_fScaleY);		rcMark.height=(3*m_fScaleY);

	if (m_popMarkedShow==_pop)
	{
	  brCap.SetColour(0,255,0);
 	  _pdc->SetPen(*wxBLACK_PEN);
	  _pdc->DrawRectangle(rcMark);
	}

	//Operator marked selected
	rcMark.Offset(0,8);
	if (m_popMarkedSelected==_pop)
	{
	  brCap.SetColour(128,0,0);
	  _pdc->SetBrush(brCap);
	  _pdc->DrawRectangle(rcMark);
	}

	//Operator's Contour on fct de la selection
	if (IsOperatorSelected(_pop))	Draw3dRect(_pdc, rcBloc, wxColor(0,0,0),      wxColor(255,255,255));
	else													Draw3dRect(_pdc, rcBloc, wxColor(255,255,255),wxColor(0,0,0));

	//Operator's Name
	const char* pszname = _pop->GetUserName();
	if (pszname==null || (strlen(pszname)==0))		pszname=_pop->GetName();
	_pdc->DrawLabel(wxString::FromAscii(pszname), rcBloc, wxALIGN_CENTER);

	//Operator's Resize Zone
	wxRect rcResize(rcBloc);
	rcResize.y+=2;
	rcResize.x=rcResize.GetRight()-6;
	rcResize.width=6;
	rcResize.height-=2;
	Draw3dRect(_pdc, rcResize, wxColor(220,220,220), wxColor(0,0,0));

/*
	//Operator Invalid or Processed
	if (_pop->m_bInvalided || _pop->m_fProcessedTime==m_fupdateTime)
	{
		DWORD dwSeed = _pop->m_fProcessedTime;
		udword y = rcResize.top + (dwSeed % rcResize.Height());
		_pdc->DrawLine(rcResize.left, y, rcResize.right, y);
	}
*/
}

//-----------------------------------------------------------------
//!	\brief  Display cursor
//-----------------------------------------------------------------
void wxOperatorsCtrl::DisplayCursor(wxDC* _pdc)
{
	if (m_bMovingBloc)
	{
		//POINT pt;		::GetCursorPos(&pt);
		//NPoint point(pt.x, pt.y);
		//ScreenToClient(point);
    wxPoint point = wxGetMousePosition();
    point = ScreenToClient(point);

		wxPoint ptOffset = point - m_ptStartBloc;

		sdword dwOffsetX = (sdword)	((m_nStartBlocPosX  + ( ((float)ptOffset.x/m_fScaleX)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_wPosX);
		sdword dwOffsetY = (sdword)	((m_nStartBlocPosY  + ( ((float)ptOffset.y/m_fScaleY)  / GB_GRIDUNIT)) - m_popMarkedSelected->m_wPosY);

		//TRACE("%d %d\n", dwOffsetX, dwOffsetY);

		for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
		{
			NOperator* pop = m_carrayOpsSelected[i];

			sdword x = pop->m_wPosX+dwOffsetX;
			sdword y = pop->m_wPosY+dwOffsetY;

			wxRect rc;
			rc.x		= (sdword) ((( x * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
			rc.y		= (sdword) ((( y * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
			rc.width	= (sdword) (pop->m_wWidth * GB_GRIDUNIT * m_fScaleX);
			rc.height	= (sdword) (1 * GB_GRIDUNIT * m_fScaleY);
			Draw3dRect(_pdc, rc, wxColor(128,64,64), wxColor(0,0,128));
		}

	} else {

		wxRect rc;
		rc.x		= (sdword) ((( m_dwCursorX * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
		rc.y		= (sdword) ((( m_dwCursorY * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
		rc.width	= (sdword) (1 * GB_GRIDUNIT * m_fScaleX);
		rc.height	= (sdword) (1 * GB_GRIDUNIT * m_fScaleY);
		Draw3dRect(_pdc, rc, wxColor(64,64,128), wxColor(0,0,128));

	}


}

//-----------------------------------------------------------------
//!	\brief  Change cursor position
//-----------------------------------------------------------------
void wxOperatorsCtrl::SetCursorPos(sdword _dwX, sdword _dwY)
{
	m_dwCursorX=_dwX;
	m_dwCursorY=_dwY;
}



//-----------------------------------------------------------------
//!	\brief  Return operator display rect
//-----------------------------------------------------------------
void wxOperatorsCtrl::GetOperatorRect(NOperator* _pop, wxRect& _rc)
{
	_rc.x		  = (sdword) ((( _pop->m_wPosX * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
	_rc.y			= (sdword) ((( _pop->m_wPosY * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
	_rc.width	= (sdword) ((_pop->m_wWidth * GB_GRIDUNIT * m_fScaleX));
	_rc.height= (sdword) ((1 * GB_GRIDUNIT * m_fScaleY));
}


//-----------------------------------------------------------------
//!	\brief	FxGen event
//-----------------------------------------------------------------
void wxOperatorsCtrl::OnFxGenPageChanged( wxCommandEvent& event )
{
  //wxMessageBox(_("wxOperatorsCtrl OnFxGenPageChanged"), _("FxGen"));

  NOperatorsPage* ppage = (NOperatorsPage*)event.GetClientData();
  DisplayOperatorsPage(ppage);
  Refresh();
}

//-----------------------------------------------------------------
//!	\brief	Display 3D Rectangle
//-----------------------------------------------------------------
void wxOperatorsCtrl::Draw3dRect(wxDC* _pdc, const wxRect& _rc, const wxColor& _clrTopLeft, const wxColor& _clrBottomRight)
{
  wxPen pen;
  pen.SetColour( _clrTopLeft );
  _pdc->SetPen(pen);
	_pdc->DrawLine(_rc.GetX(), _rc.GetY(), _rc.GetRight(), _rc.GetY() );
	_pdc->DrawLine(_rc.GetX(), _rc.GetY(), _rc.GetX(), _rc.GetBottom() );

  pen.SetColour( _clrBottomRight );
  _pdc->SetPen(pen);
	_pdc->DrawLine(_rc.GetX(), _rc.GetBottom(), _rc.GetRight()+1, _rc.GetBottom() );
	_pdc->DrawLine(_rc.GetRight(), _rc.GetY(), _rc.GetRight(), _rc.GetBottom() );

}

//-----------------------------------------------------------------
//!	\brief  Check if selected and moved operators collides with others
//-----------------------------------------------------------------
bool wxOperatorsCtrl::IsMovedSelOperatorsCollide(sdword _dwoffsetX, sdword _dwoffsetY)
{

	for (udword i=0; i<m_carrayOpsSelected.Count(); i++)
	{
		NOperator* popSel = m_carrayOpsSelected[i];

		//Compute asked operator rect
		wxRect rcItemSel;
		rcItemSel.x		  = (sdword) ((( (popSel->m_wPosX+_dwoffsetX) * GB_GRIDUNIT) + m_fPosX) * m_fScaleX);
		rcItemSel.y			= (sdword) ((( (popSel->m_wPosY+_dwoffsetY) * GB_GRIDUNIT) + m_fPosY) * m_fScaleY);
		rcItemSel.width	= (sdword) (popSel->m_wWidth * GB_GRIDUNIT * m_fScaleX);
		rcItemSel.height= (sdword) (1 * GB_GRIDUNIT * m_fScaleY);
		rcItemSel.Deflate(1,1);

		//Control if one collision with other operators rect
		if (IsOpRectCollide(rcItemSel, true))
			return true;		//One collision found !

	}

	return false;	//No collision found
}

//-----------------------------------------------------------------
//!	\brief  Check if an operator collides with others
//-----------------------------------------------------------------
bool wxOperatorsCtrl::IsOpRectCollide(wxRect& _rcItemTest, bool _bExcludeSel)
{
	for (udword i=0; i<m_popsPage->GetOpsCount() ; i++)
	{
		NOperator* pop = m_popsPage->GetOpFromIdx(i);

		if (_bExcludeSel && m_carrayOpsSelected.Index(pop)!=wxNOT_FOUND)	continue;

		wxRect rcItem;
		GetOperatorRect(pop, rcItem);
		if (rcItem.Intersects(_rcItemTest))
			return true;	//One collision found !
	}

	return false;	//No collision found
}

//-----------------------------------------------------------------
//!	\brief  Select an operator
//-----------------------------------------------------------------
void wxOperatorsCtrl::SelectOperator(NOperator* _pop)
{
	if (_pop==null)
	{
		m_carrayOpsSelected.Clear();

	} else {

		if (!IsOperatorSelected(_pop))
			m_carrayOpsSelected.Add(_pop);
	}

	m_popMarkedSelected = _pop;

	//Event
  wxCommandEvent  fxevent( wxEVT_FXGEN_OPERATORSCTRL, wsID_FXGEN_OPSEL_CHANGED);
  fxevent.SetClientData(_pop);
  fxevent.SetEventObject( this );
  wxPostEvent( GetEventHandler(), fxevent );

}

//-----------------------------------------------------------------
//!	\brief  Select operators into a given rect
//-----------------------------------------------------------------
void wxOperatorsCtrl::SelectOperatorsIntoRect(wxRect& _rc)
{
	if (!m_popsPage)	return;

	//_rc.Normalize();
	if (_rc.IsEmpty())		return;

	m_carrayOpsSelected.Clear();

	for (udword i=0; i<m_popsPage->GetOpsCount(); i++)
	{
		NOperator* pop = m_popsPage->GetOpFromIdx(i);
		wxRect rcItem;
		GetOperatorRect(pop, rcItem);
		if (_rc.Intersects(rcItem))
			m_carrayOpsSelected.Add(pop);
	}


}

//-----------------------------------------------------------------
//!	\brief  Return true if operator is selected
//-----------------------------------------------------------------
bool wxOperatorsCtrl::IsOperatorSelected(NOperator* _pop)
{
	for (udword i=0; i<m_carrayOpsSelected.GetCount(); i++)
		if (m_carrayOpsSelected[i] == _pop)
			return true;

	return false;
}

//-----------------------------------------------------------------
//!	\brief  Return an operator under a point
//-----------------------------------------------------------------
NOperator* wxOperatorsCtrl::GetOperatorAt(wxPoint& _pt, bool& _bResizeZone)
{
	_bResizeZone = false;

	if (m_popsPage)
	{
		for (udword i=0; i<m_popsPage->GetOpsCount() ; i++)
		{
			NOperator* pop = m_popsPage->GetOpFromIdx(i);
			wxRect rcItem;
			GetOperatorRect(pop, rcItem);
			if (rcItem.Contains(_pt))
			{
				wxRect rcResize(rcItem);
				rcResize.y+=4;
				//rcResize.left=rcResize.right-8;
				rcResize.x=rcResize.GetRight()-8;
				if (rcResize.Contains(_pt))		_bResizeZone = true;

				return pop;
			}
		}

	}

	return null;
}
