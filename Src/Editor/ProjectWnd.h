//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ProjectWnd.h
//! \brief	Project's windows
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
#include "Editor.h"
#include "TreeNodeCtrl.h"

//-----------------------------------------------------------------
//!	\class		NProjectWnd
//!	\brief		Project's windows
//-----------------------------------------------------------------
class NProjectWnd :	public NTreeNodeCtrl
{
public:
	NProjectWnd(void);
	virtual ~NProjectWnd(void);

	//Methods
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent);

	void DisplayOperatorsProject(NEngineOp* _popsProject);
	void SelectFirstPage();

protected:
	//Methods
	void AddGroup();
	void AddPage();
	void Delete();
	void Rename();

	NTreeNode*			GetSelectedGroup();
	NOperatorsPage* GetSelectedPage();

//	NObject* _FindNodeFromClassName(NTreeNode* _pParent, char* _pszClassName);

	// Messages Notify
	void OnTreeSelChange();

	//Win32 Messages Dispatching
	virtual void OnRightButtonDown(udword flags, NPoint pos);
	virtual	void OnCommand(udword id);

	//Methods
	void InitCtxMenu();

	//Datas
	NEngineOp*	m_popsProject;
	HMENU				m_hMenu;
	NMenuCtrl		m_wndMenu;
};
