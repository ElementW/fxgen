//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		AssetsWnd.h
//! \brief	Assets's windows
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		13-03-2010
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef ASSETSWND_H
#define ASSETSWND_H

//-----------------------------------------------------------------
//			Includes
//-----------------------------------------------------------------
#include "EditorApp.h"
#include "TreeNodeCtrl.h"

//-----------------------------------------------------------------
//			Prototypes
//-----------------------------------------------------------------
class NOperatorsPage;

//-----------------------------------------------------------------
//!	\class		NAssetsWnd
//!	\brief		Assets's windows
//-----------------------------------------------------------------
class NAssetsWnd :	public NTreeNodeCtrl
{
public:
	NAssetsWnd(void);
	virtual ~NAssetsWnd(void);

	//Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);


protected:
	//Methods

	// Messages Notify
	void OnTreeSelChange(NObject* _psender);

	//Events
	virtual void OnRButtonDown(NPoint pos);
	void OnMenuClick(NObject* _psender);

	//Methods
	void InitCtxMenu();

	//Datas
	NMenuCtrl		m_wndMenu;
};

#endif //ASSETSWND_H
