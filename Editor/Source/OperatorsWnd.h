//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorsWnd.h
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
#include "CoreLibPkg.h"
#include "Operator.h"

//-----------------------------------------------------------------
// 			Defines
//-----------------------------------------------------------------
#define GB_GRIDUNIT					16						//16 pixels = 1 unitee
#define GB_GRIDCOLOR				RGBA(100,100,100,255)
#define GB_BLOCCOLOR				RGBA(200,200,200,255)
#define GB_BLOCSELCOLOR			RGBA(200,200,240,255)
#define GB_MARKEDSHOWCOLOR	RGBA(0,200,0,255)


//-----------------------------------------------------------------
//!	\class		NOperatorsWnd
//!	\brief		GUI control for operators editing
//-----------------------------------------------------------------
class NOperatorsWnd : public NGUIWnd
{
public:
	// Constructor-Destructor
	NOperatorsWnd();
	virtual ~NOperatorsWnd();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);

	void DisplayOperatorsPage(NOperatorsPage* popsPage);

	void	PlaceOperator(const char* _pszClassName);

	void	SelectOperator(NOperatorNode* pop);
	void	DeleteOperatorsSelected();

	void	Update();	//!< MAJ de l'affichage
	void	Update(float _ftime);

	virtual void OnMarkShowOperator(NOperatorNode* pop);
	virtual void OnDeletingOperator(NOperatorNode* pop);
	virtual void OnDeletedOperator(NOperatorNode* pop);

	//Members Access
	NOperatorNode*	GetMarkedShowOperator()		{ return m_popMarkedShow; }

protected:
	// Internal methods
	NOperatorNode*	AddOperator(sword x, sword y, sword w, const char* _pszClassName);
	void				DeleteOperator(NOperatorNode* pop);
	void				Reset();

	NOperatorNode*	GetOperatorAt(NPoint& pt, bool& bResizeZone);
	void				UpdateCursor(NPoint& pt);
	void				SelectOperatorsIntoRect(NRect& rc);
	void				GetOperatorRect(NOperatorNode* _pop, NRect& _rc);

	void				DisplayOperator(N2DPainter* _pdc, NOperatorNode* _pop);

	bool				IsMovedSelOperatorsCollide(sdword _dwoffsetX, sdword _dwoffsetY);
	bool				IsOpRectCollide(NRect _rcItemTest, bool _bExcludeSel);

	void				DisplayOperatorsMap(N2DPainter* _pdc);

	//Context menu
	void InitCtxMenu();
	void OnMenuItemClick(NObject* _psender);
	NMenuCtrl	m_wndMenu;

	// Clipboard Methods
	void CopyOperatorsSelectedToClipboard();
	void PasteOperatorsFromClipboard();
	void ClearClipboard();

	//Selection Methods
	bool				IsOperatorSelected(NOperatorNode* pcbloc);

	//Cursor Methods
	void				DisplayCursor(N2DPainter* _pdc);
	void				SetOpCursorPos(sdword _dwX, sdword _dwY);

	// Datas
	float		m_fPosX, m_fPosY, m_fStartPosXPan, m_fStartPosYPan;
	bool		m_bPanning, m_bMovingBloc, m_bResizingBloc, m_bSelectingRect;
	NPoint	m_ptStartPan, m_ptStartBloc;
	float		m_fScaleX, m_fScaleY;
	sdword	m_nStartBlocPosX, m_nStartBlocPosY, m_nStartBlocWidth;	//Moving-Resizing blocs
	udword	m_dwSeedValue;
	NPoint	m_ptStartRect, m_ptEndRect;
	bool		m_bAskedForMove;

	//Datas Operators
	NOperatorsPage*			m_popsPage;						//!< Page used for editing
	NArray<NOperatorNode*>	m_carrayOpsSelected;	//!< Operators selected
	NArray<NOperatorNode*>	m_carrayOpsClipboard;	//!< Operators in clipboard
	NOperatorNode	*m_popMarkedSelected, *m_popMarkedShow;
	float								m_fupdateTime;
	NRect	m_rcAllOperators;	//!< Operators zone

	//Datas cursor
	sdword	m_dwCursorX, m_dwCursorY;

	//m_dwWidth
	EVT_DECLARE_HANDLER(OnPageSelected);
	EVT_DECLARE_HANDLER(OnRendering);

protected:
	//Events
	virtual	void	OnPaint(N2DPainter* _ppainter);
	virtual void	OnSize();

	virtual void	OnMouseMove(NPoint pos );
	virtual void	OnMButtonDown(NPoint pos);
	virtual void	OnMButtonUp(NPoint pos);
	virtual void	OnLButtonUp(NPoint pos);
	virtual void	OnLButtonDown(NPoint pos);
	virtual	void	OnLButtonDblClk(NPoint point);
	virtual void	OnRButtonDown(NPoint pos);
	virtual	void	OnMouseWheel(NPoint pos, sdword zDelta);

	virtual	void	OnKeyUp(udword dwchar);

};

