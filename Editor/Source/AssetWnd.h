//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		AssetWnd.h
//! \brief	Asset's windows
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		23-11-2010
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
#include "Editor.h"
#include "TreeNodeCtrl.h"

//-----------------------------------------------------------------
//			Prototypes
//-----------------------------------------------------------------
class NAssetModel;

//-----------------------------------------------------------------
//!	\class		NAssetWnd
//!	\brief		Asset's windows
//-----------------------------------------------------------------
class NAssetWnd :	public NTreeNodeCtrl
{
public:
	NAssetWnd(void);
	virtual ~NAssetWnd(void);

	//Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);

	void DisplayAssetModel(NAssetModel* _popsAsset);
	void SelectFirstGraph();

protected:
	//Methods
	void AddFolder();
	void AddGraph();
	void Delete();
	void Rename();

	NTreeNode*			GetSelectedFolder();
	NOpGraphModel* GetSelectedGraph();

	NObject* _FindNodeFromClassName(NTreeNode* _pParent, char* _pszClassName);

	// Messages Notify
	void OnTreeSelChange(NObject* _psender);

	//Messages Dispatching
	virtual void OnRButtonDown(NPoint pos);
	void OnMenuClick(NObject* _psender);

	//Methods
	void InitCtxMenu();

	//Datas
	NAssetModel*	m_popsAsset;
	NMenuCtrl			m_wndMenu;
};
