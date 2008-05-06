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
#ifndef OPERATORSCTRL_H_INCLUDED
#define OPERATORSCTRL_H_INCLUDED

//-----------------------------------------------------------------
// 			Includes
//-----------------------------------------------------------------
#include <wx/wx.h>
#include <wx/control.h>

#include "..\Src\FxGenLib\Core.h"
#include "..\Src\FxGenLib\EngineOp.h"

//-----------------------------------------------------------------
// 			Externs
//-----------------------------------------------------------------
extern const wxChar wxOperatorsCtrlNameStr[];

//-----------------------------------------------------------------
// 			Defines
//-----------------------------------------------------------------
#define GB_GRIDUNIT					16						//16 pixels = 1 unitee
#define GB_GRIDCOLOR				RGB(100,100,100)
#define GB_BLOCCOLOR				RGB(200,200,200)
#define GB_BLOCSELCOLOR			RGB(200,200,240)
#define GB_MARKEDSHOWCOLOR	RGB(0,200,0)

WX_DEFINE_ARRAY(NOperator*, arrayOperators);

//-----------------------------------------------------------------
//!	\class		wxOperatorsCtrl
//!	\brief		GUI control for operators editing
//-----------------------------------------------------------------
class wxOperatorsCtrl : public wxControl
{
    DECLARE_DYNAMIC_CLASS(wxOperatorsCtrl)

public:
    wxOperatorsCtrl() {}

    wxOperatorsCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxOperatorsCtrlNameStr);

  //Methods
  void Reset();
	void DisplayOperatorsPage(NOperatorsPage* _popsPage);

	//Members Access
	NOperator*	GetMarkedShowOperator()		{ return m_popMarkedShow; }

  //Events
  DECLARE_EVENT_TABLE()
  void OnPaint(wxPaintEvent& event);
  void OnMouseMove(wxMouseEvent& event);
  void OnMiddleDown(wxMouseEvent& event);
  void OnMiddleUp(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftDClick(wxMouseEvent& event);

  //FxGen Events
  virtual void OnFxGenPageChanged( wxCommandEvent& event );

protected:
  //Methods
 	void DisplayOperator(wxDC* _pdc, NOperator* _pop);
  void GetOperatorRect(NOperator* _pop, wxRect& _rc);
  void Draw3dRect(wxDC* _pdc, const wxRect& _rc, const wxColor& _clrTopLeft, const wxColor& _clrBottomRight);

	bool IsMovedSelOperatorsCollide(sdword _dwoffsetX, sdword _dwoffsetY);
	bool IsOpRectCollide(wxRect& _rcItemTest, bool _bExcludeSel);

	NOperator*	GetOperatorAt(wxPoint& _pt, bool& _bResizeZone);

	//Selection Methods
	bool IsOperatorSelected(NOperator* _pcbloc);
	void SelectOperatorsIntoRect(wxRect& _rc);
	void SelectOperator(NOperator* _pop);

	//Cursor Methods
	void DisplayCursor(wxDC* _pdc);
  void SetCursorPos(sdword _dwX, sdword _dwY);

	// Datas
	float	m_fPosX, m_fPosY, m_fStartPosXPan, m_fStartPosYPan;
	bool		m_bPanning, m_bMovingBloc, m_bResizingBloc, m_bSelectingRect;
	wxPoint	m_ptStartPan, m_ptStartBloc;
	float	m_fScaleX, m_fScaleY;
	sdword	m_nStartBlocPosX, m_nStartBlocPosY, m_nStartBlocWidth;	//Moving-Resizing blocs
	udword	m_dwSeedValue;
	wxPoint	m_ptStartRect, m_ptEndRect;
  wxFont  m_fontOperator;

	//Datas Operators
	NOperatorsPage*			m_popsPage;						//!< Page used for editing

	//WX_DEFINE_ARRAY(NOperator*, m_carrayOpsSelected);
	//WX_DEFINE_ARRAY(NOperator*, m_carrayOpsClipboard);

  arrayOperators  m_carrayOpsSelected;
  arrayOperators  m_carrayOpsClipboard;

	NOperator	*m_popMarkedSelected, *m_popMarkedShow; //!< Operators selected and in clipboard
	float			m_fupdateTime;

	//Datas cursor
	sdword	m_dwCursorX, m_dwCursorY;

/*
	void	PlaceOperator(ID CLASSID);

	void	DeleteOperatorsSelected();

	void	Update(float _ftime);

	virtual void	OnMarkShowOperator(NOperator* pop);			//OVERLOAD
	virtual void	OnDeletingOperator(NOperator* pop)	{}	//OVERRIDE
	virtual void	OnDeletedOperator(NOperator* pop)		{}	//OVERRIDE


protected:
	// Internal methods
	NOperator*	AddOperator(sword x, sword y, sword w, ID CLASSID);
	void				DeleteOperator(NOperator* pop);

	void				UpdateCursor(NPoint& pt);

	// Clipboard Methods
	void CopyOperatorsSelectedToClipboard();
	void PasteOperatorsFromClipboard();
	void ClearClipboard();
*/
};

#endif // OPERATORSCTRL_H_INCLUDED
