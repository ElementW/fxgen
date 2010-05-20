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
#ifndef MENUCTRL_H
#define MENUCTRL_H


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
class CORELIB_API NMenuCtrl : public NGUIWnd
{

public:
	// Constructor-Destructor
	NMenuCtrl();
	virtual ~NMenuCtrl();

	// Methods
	virtual	bool Create(char* name, NGUIWnd* parent);

	// Methods
	void	Update();

	void	TrackPopupMenu(NPoint _ptScreen, NMenuCtrl* _pParentMenu=null);

	void DeleteAllItems();

	NMEItemDesc* AddItem(const char* _pszName, udword _id, udword _dwStyle);

	udword				GetItemsCount();
	NMEItemDesc*	GetItemDesc(udword _idx);
	NMEItemDesc*	GetItemDescFromID(udword _id);

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
	static udword	m_dwReturnCmdID;	//!< Only one menu at once

protected:
	//Win32 Events
	virtual	void	OnPaint(N2DPainter* _ppainter);
	virtual void	OnSize();
	virtual void	OnMouseMove(NPoint pos );
	virtual void	OnMButtonDown(NPoint pos);
	virtual void	OnMButtonUp(NPoint pos);
	virtual void	OnLButtonUp(NPoint pos);
	virtual void	OnLButtonDown(NPoint pos);
	virtual void	OnRButtonDown(NPoint pos);
	virtual	void	OnKeyUp(udword dwchar);
	virtual	void	OnKillFocus(NGUIWnd* pNewWnd);
	//virtual	void	OnTimer(udword _dwTimerID);

};

#endif //MENUCTRL_H
