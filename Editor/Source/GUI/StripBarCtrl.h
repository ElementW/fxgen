//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		StripBarCtrl.h
//! \brief	GUI control for controls container as toolbar
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		15-11-2007
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
#include "GUI.h"
#include "StandardsCtrl.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define	SB_BARHEIGHT	24
#define	SB_BORDER			2
#define	SB_INDENT			16

//-----------------------------------------------------------------
//! \struct NSBItemDesc
//! \brief	Item description
//-----------------------------------------------------------------
struct NSBItemDesc
{
	NWnd*	pwnd;
	udword dwWidth;
	//Datas RT
	NRect	rcItem;			//!< Windows Pos
};


//-----------------------------------------------------------------
//!	\class		NStripBarCtrl
//!	\brief		GUI control for controls container as toolbar
//-----------------------------------------------------------------
class  NStripBarCtrl : public NWControl
{
public:
	// Constructor-Destructor
	NStripBarCtrl();
	virtual ~NStripBarCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent);

	// Methods
	void	Update();

	NTextCtrl*		InsertItemText(const char* _pszname, udword _dwWidth);
	NButtonCtrl*	InsertItemButton(const char* _pszname, udword _dwWidth);

	//Notification Messages
//	FDelegate		OnSelChange;

protected:
	// Internal methods
	void RecalcLayout();

	// Datas
	GArray<NSBItemDesc>	m_carrayItems;

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
	virtual	void	OnKeyDown(udword dwchar);
};

