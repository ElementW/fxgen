//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorsCtrl.h
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
#pragma once

//-----------------------------------------------------------------
//			Includes
//-----------------------------------------------------------------
#include "GUI.h"
#include "EngineOp.h"

//-----------------------------------------------------------------
// 			Defines
//-----------------------------------------------------------------
#define GB_GRIDUNIT					16						//16 pixels = 1 unitee
#define GB_GRIDCOLOR				RGB(100,100,100)
#define GB_BLOCCOLOR				RGB(200,200,200)
#define GB_BLOCSELCOLOR			RGB(200,200,240)
#define GB_MARKEDSHOWCOLOR	RGB(0,200,0)


//-----------------------------------------------------------------
//!	\class		NOperatorsCtrl
//!	\brief		GUI control for operators editing
//-----------------------------------------------------------------
class NOperatorsCtrl : public NWControl
{
public:
	// Constructor-Destructor
	NOperatorsCtrl();
	virtual ~NOperatorsCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent);

	void DisplayOperatorsPage(NOperatorsPage* popsPage);

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

	// Datas
	float		m_fPosX, m_fPosY, m_fStartPosXPan, m_fStartPosYPan;
	bool		m_bPanning, m_bMovingBloc, m_bResizingBloc, m_bSelectingRect;
	NPoint	m_ptStartPan, m_ptStartBloc;
	float		m_fScaleX, m_fScaleY;
	sdword	m_nStartBlocPosX, m_nStartBlocPosY, m_nStartBlocWidth;	//Moving-Resizing blocs
	HFONT		m_hfontNormal;
	udword	m_dwSeedValue;
	NPoint	m_ptStartRect, m_ptEndRect;

	//Datas Operators
	NOperatorsPage*			m_popsPage;						//!< Page used for editing
	NArray<NOperator*>	m_carrayOpsSelected;	//!< Operators selected
	NArray<NOperator*>	m_carrayOpsClipboard;	//!< Operators in clipboard
	NOperator	*m_popMarkedSelected, *m_popMarkedShow;
	float								m_fupdateTime;

	//Datas cursor
	sdword	m_dwCursorX, m_dwCursorY;

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

	//Windows Proc
	virtual	LRESULT	WndProc( UINT msg, WPARAM wparam, LPARAM lparam);
};

