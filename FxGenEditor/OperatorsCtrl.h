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

  //Events
  DECLARE_EVENT_TABLE()
  void OnPaint(wxPaintEvent& event);
  void OnMouseMove(wxMouseEvent& event);
  void OnMiddleDown(wxMouseEvent& event);
  void OnMiddleUp(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);

  //FxGen Events
  virtual void OnFxGenPageChanged( wxCommandEvent& event );

protected:
  //Methods
 	void DisplayOperator(wxPaintDC* _pdc, NOperator* _pop);
  void GetOperatorRect(NOperator* _pop, wxRect& _rc);
  void Draw3dRect(wxPaintDC* _pdc, const wxRect& _rc, const wxColor& _clrTopLeft, const wxColor& _clrBottomRight);

	//Selection Methods
	bool IsOperatorSelected(NOperator* pcbloc);

	// Datas
	float		m_fPosX, m_fPosY, m_fStartPosXPan, m_fStartPosYPan;
	bool		m_bPanning, m_bMovingBloc, m_bResizingBloc, m_bSelectingRect;
	wxPoint	m_ptStartPan, m_ptStartBloc;
	float		m_fScaleX, m_fScaleY;
	sdword	m_nStartBlocPosX, m_nStartBlocPosY, m_nStartBlocWidth;	//Moving-Resizing blocs
	udword	m_dwSeedValue;
	wxPoint	m_ptStartRect, m_ptEndRect;
  wxFont m_fontOperator;

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

	void	SelectOperator(NOperator* pop);
	void	DeleteOperatorsSelected();

	void	Update();	//!< MAJ de l'affichage
	void	Update(float _ftime);

	virtual void	OnMarkShowOperator(NOperator* pop);			//OVERLOAD
	virtual void	OnDeletingOperator(NOperator* pop)	{}	//OVERRIDE
	virtual void	OnDeletedOperator(NOperator* pop)		{}	//OVERRIDE

	//Members Access
	NOperator*	GetMarkedShowOperator()		{ return m_popMarkedShow; }

protected:
	// Internal methods
	NOperator*	AddOperator(sword x, sword y, sword w, ID CLASSID);
	void				DeleteOperator(NOperator* pop);
	void				Reset();

	NOperator*	GetOperatorAt(NPoint& pt, bool& bResizeZone);
	void				UpdateCursor(NPoint& pt);
	void				SelectOperatorsIntoRect(NRect& rc);
	void				GetOperatorRect(NOperator* _pop, NRect& _rc);

	void				DisplayOperator(NGraphics* _pdc, NOperator* _pop);

	bool				IsMovedSelOperatorsCollide(sdword _dwoffsetX, sdword _dwoffsetY);
	bool				IsOpRectCollide(NRect _rcItemTest, bool _bExcludeSel);

	// Clipboard Methods
	void CopyOperatorsSelectedToClipboard();
	void PasteOperatorsFromClipboard();
	void ClearClipboard();

	//Selection Methods
	bool				IsOperatorSelected(NOperator* pcbloc);

	//Cursor Methods
	void				DisplayCursor(NGraphics* _pdc);
	void				SetCursorPos(sdword _dwX, sdword _dwY);


protected:
	//Win32 Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnSize();
	virtual void	OnMouseMove(udword flags, NPoint pos );
	virtual void	OnMButtonDown(udword flags, NPoint pos);
	virtual void	OnMButtonUp(udword flags, NPoint pos);
	virtual void	OnLeftButtonUp(udword flags, NPoint pos);
	virtual void	OnLeftButtonDown(udword flags, NPoint pos);
	virtual	void	OnLeftButtonDblClk(udword flags, NPoint point);
	virtual void	OnRightButtonDown(udword flags, NPoint pos);
	virtual	void	OnMouseWheel(udword flags, sword zDelta, NPoint pos);
	virtual	void	OnKeyDown(udword dwchar);
*/
};

#endif // OPERATORSCTRL_H_INCLUDED
