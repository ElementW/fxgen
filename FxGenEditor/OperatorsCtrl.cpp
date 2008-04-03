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
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 			Includes
//-----------------------------------------------------------------
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
  wxPaintDC dc(this);
	//dc.FillSolidRect(rc, RGB(115,115,115));

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
		//NRect rcZone(m_ptStartRect.x, m_ptStartRect.y, m_ptEndRect.x, m_ptEndRect.y);
		//dc.Draw3dRect(rcZone, RGB(255,255,255), RGB(255,255,255));
	}

	/////////////////////////////////////////////////
	//Draw Cursor
	//DisplayCursor(&dc);


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
}

//-----------------------------------------------------------------
//!	\brief	Event Mouse Left Down
//-----------------------------------------------------------------
void wxOperatorsCtrl::OnLeftDown(wxMouseEvent& event)
{
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
void wxOperatorsCtrl::DisplayOperator(wxPaintDC* _pdc, NOperator* _pop)
{
	//Operator's Zone
	wxRect rcBloc;
	GetOperatorRect(_pop, rcBloc);

	//Background color
	//_pdc->GradientVRect(rcBloc, RGB(220, 220, 220), _pop->GetColor());
	_pdc->GradientFillLinear(rcBloc, wxColor(220,220,220), wxColor(_pop->GetColor()), wxNORTH);

	//Operator's Caption
	wxRect rcCap(rcBloc);
	rcCap.width=3;
  wxBrush brCap;
	if (!_pop->m_bError)		brCap.SetColour(wxColor(_pop->GetColor()));
	else										brCap.SetColour(wxColor(255,0,0));
	_pdc->SetBrush(brCap);
  _pdc->DrawRectangle(rcCap);

	//Operator marked show
/*	NRect rcMark(rcBloc);
	rcMark.top=rcMark.top+(3*m_fScaleY);		rcMark.bottom=rcMark.top+(3*m_fScaleY);
	rcMark.left+=(3*m_fScaleX);							rcMark.right=rcMark.left+(8*m_fScaleX);

	if (m_popMarkedShow==_pop)		_pdc->FillSolidRect(rcMark, RGB(0,128,0));

	//Operator marked selected
	rcMark.Move(0,8);
	if (m_popMarkedSelected==_pop)	 _pdc->FillSolidRect(rcMark, RGB(128,0,0));
*/
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
//!	\brief
//-----------------------------------------------------------------
/*void wxOperatorsCtrl::DisplayCursor(NGraphics* _pdc)
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


}*/

//-----------------------------------------------------------------
//!	\brief
//-----------------------------------------------------------------
/*void wxOperatorsCtrl::SetCursorPos(sdword _dwX, sdword _dwY)
{
	m_dwCursorX=_dwX;
	m_dwCursorY=_dwY;
}*/


//-----------------------------------------------------------------
//!	\brief  Return true if operator is selected
//-----------------------------------------------------------------
bool wxOperatorsCtrl::IsOperatorSelected(NOperator* pop)
{
	for (udword i=0; i<m_carrayOpsSelected.GetCount(); i++)
		if (m_carrayOpsSelected[i] == pop)
			return true;

	return false;
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
//!	\brief	Affichage d'un rectangle 3D
//-----------------------------------------------------------------
void wxOperatorsCtrl::Draw3dRect(wxPaintDC* _pdc, const wxRect& _rc, const wxColor& _clrTopLeft, const wxColor& _clrBottomRight)
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
