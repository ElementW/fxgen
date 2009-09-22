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
//                   Macros
//-----------------------------------------------------------------

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

//Items Styles
#define ME_ITEMSTYLE_CHECKBOX		1

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NMenuCtrl;

//-----------------------------------------------------------------
//! \struct NMEItemDesc
//! \brief	Item description
//-----------------------------------------------------------------
struct NMEItemDesc
{
	udword	dwID;
	udword	dwUserData;
	NString strName;
	udword	dwStyle;	//!< See ME_ITEMSTYLE_CHECKBOX...

	//Datas RT
	NRect		rcItem;			//!< Windows Pos
	bool		bChecked;
	bool		bEnabled;
	NMenuCtrl*	ppopUpMenu;
};


//-----------------------------------------------------------------
//!	\class		NMenuCtrl
//!	\brief		GUI control for menu
//-----------------------------------------------------------------
class  NMenuCtrl : public NWControl
{

public:
	// Constructor-Destructor
	NMenuCtrl();
	virtual ~NMenuCtrl();

	// Methods
	virtual	bool Create(char* name, NWnd* parent);

	// Methods
	void	Update();

	void	TrackPopupMenu(NPoint _ptScreen, NMenuCtrl* _pParentMenu=null);

	void DeleteAllItems();

	NMEItemDesc* AddItem(const char* _pszName, udword _id, udword _dwStyle);

	udword				GetItemsCount();
	NMEItemDesc*	GetItemDesc(udword _idx);

	NMenuCtrl* CreatePopupMenu(const char* _pszName, udword _idx);
	NMenuCtrl* GetPopupMenu(udword _idx);

	//Members access
	udword GetClickedCmdID()	{ return m_dwReturnCmdID; }

	//Notification Messages
	FDelegate		OnItemClick;

protected:
	// Internal methods
	void CalcMenuSize(NRect& _rcOut);
	udword GetItemIdxUnderPt(NPoint& _point);

	void ShowMenu(bool _bShow=true, NMenuCtrl* _pParentMenu=null);

	// Datas
	NArray<NMEItemDesc>	m_carrayItems;

	udword	m_dwItemHighLightedIdx;	//!< Idx from m_carrayItems
	bool		m_bEntered;

	NMenuCtrl* m_pcurPopupMenu, *m_pcurParentMenu;

	// Static Datas
	static udword	m_dwReturnCmdID;

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
	virtual	void	OnKillFocus(NWnd* pNewWnd);
	//virtual	void	OnTimer(udword _dwTimerID);

};


