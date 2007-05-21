//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MenuCtrl.h
//! \brief	GUI control for menu
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		24-04-2007
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

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define ME_ITEMHEIGHT			16
#define ME_ITEMTEXTIDENT	16
#define ME_ITEMIDENT			16

//-----------------------------------------------------------------
//! \struct NMEItemDesc
//! \brief	Item description
//-----------------------------------------------------------------
struct NMEItemDesc
{
	udword	dwID;
	udword	dwUserData;
	NString strName;
	bool		bChecked;
	bool		bEnabled;
	
	NArray<NMEItemDesc>	ChildItems;

	//Datas RT
	NRect						rcItem;			//!< Windows Pos
};


//-----------------------------------------------------------------
//!	\class		NMenuCtrl
//!	\brief		GUI control for menu
//-----------------------------------------------------------------
class NMenuCtrl : public NWControl
{

public:
	// Constructor-Destructor
	NMenuCtrl();
	virtual ~NMenuCtrl();

	// Methods
	virtual	bool Create(char* name, NWnd* parent);

	// Methods
	void	Update();

	NMEItemDesc*	TrackPopupMenu(NPoint _ptScreen);

	NMEItemDesc* AddItem(char* _pszName, udword _id, NMEItemDesc* _pparent);



	//Notification Messages
//	FDelegate		OnSelChange;

protected:
	// Internal methods
	void CalcMenuSize(NRect& _rcOut);
	udword GetItemIdxUnderPt(NPoint& _point);

	void ShowMenu(bool _bShow=true);

	// Datas
	HFONT		m_hfontNormal, m_hfontBold;

	NArray<NMEItemDesc>	m_carrayItems;
	udword	m_dwItemHighLightedIdx;	//!< Idx from m_carrayItems


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
	virtual	void	OnKeyUp(udword dwchar);

	//Windows Proc
	virtual	LRESULT	WndProc( UINT msg, WPARAM wparam, LPARAM lparam);
};

