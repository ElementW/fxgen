//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		LayoutDock.h
//! \brief	Docking Wnd Manager
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		11-03-2010
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
//                   Includes
//-----------------------------------------------------------------
#include "CoreLibPkg.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//! \enum		ETypeSplit
//! \brief	Splits types
//-----------------------------------------------------------------
enum ETypeSplit
{
	eAucun = 0,
	eHorizontal,
	eVertical
};

//-----------------------------------------------------------------
//! \struct SLayoutTabItem
//! \brief	Layout Tab description
//-----------------------------------------------------------------
struct SLayoutTabItem
{
	NGUIWnd*	pwnd;
	NRect			rc;
};

//-----------------------------------------------------------------
//! \struct SLayoutItem
//! \brief	Layout description
//-----------------------------------------------------------------
struct SLayoutItem
{
	udword			dwSplitAt;			//!< Split en pourcentage
	udword			dwIdxToSplit;
	ETypeSplit	eTypeSplit;
	udword			dwIdxWndSelected;

	GArray<SLayoutTabItem>	arrayTabs;	//!< tabs list for this layout

	//RunTime
	NRect rc;			//!< Layout's rect

};

//-----------------------------------------------------------------
//!	\class		NLayoutDock
//!	\brief		Docking Wnd manager
//-----------------------------------------------------------------
class NLayoutDock : public NGUIWnd
{

public:
	// Constructor-Destructor
	NLayoutDock();
	virtual ~NLayoutDock();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);

	// Methods
	void	Update();

	//Notification Messages
	//FDelegate		OnSelChange;

	udword SplitH(udword _dwID, udword _dwPercent);	//!< Horizontal split
	udword SplitV(udword _dwID, udword _dwPercent);	//!< Vertical split

	void AddWndToLayout(udword _dwID, NGUIWnd* _pwnd);	//!< rajoute une fenetre dans un layout

protected:
	// Internal methods
	SLayoutItem*		GetLayoutAt(NPoint point);
	udword GetLayoutTabAt(SLayoutItem* _pLayout, NPoint _point);

	// Datas
	NPoint	m_ptStartMouse, m_ptCursor;

	//Methods
	void RecalLayout();
	void _RecalLayout(SLayoutItem* _pitem);
	void DeleteAllLayouts();
	void SelectTab(SLayoutItem* _playout, udword _idx);

	//Datas
	GArray<SLayoutItem*>	m_arrayItems;
	udword dwTabHeight;

protected:
	//Win32 Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnSize();
	virtual void	OnMouseMove(NPoint pos );
	virtual void	OnMButtonDown(NPoint pos);
	virtual void	OnMButtonUp(NPoint pos);
	virtual void	OnLButtonUp(NPoint pos);
	virtual void	OnLButtonDown(NPoint pos);
	virtual	void	OnLButtonDblClk(NPoint point);
	virtual void	OnRButtonDown(NPoint pos);
	virtual	void	OnKeyDown(udword dwchar);
};

