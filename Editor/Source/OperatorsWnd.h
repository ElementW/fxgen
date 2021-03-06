//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorsWnd.h
//! \brief	Windows for operators editing
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
#include "OperatorsCtrl.h"
#include "Editor.h"

//-----------------------------------------------------------------
//!	\class		NOperatorsWnd
//!	\brief		Windows for operators editing
//-----------------------------------------------------------------
class NOperatorsWnd :	public NOperatorsCtrl
{
public:
	NOperatorsWnd(void);
	virtual ~NOperatorsWnd(void);

	//Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);
	virtual void OnMarkShowOperator(NOperatorNode* pop);

	virtual void OnDeletingOperator(NOperatorNode* pop);
	virtual void OnDeletedOperator(NOperatorNode* pop);

	//Methods Debug
	void Dump();

protected:
	//Methods
	void InitCtxMenu();

	// Messages Notify
	void OnMenuItemClick(NObject* _psender);

	//Events
	EVT_DECLARE_HANDLER(OnPageSelected);
	EVT_DECLARE_HANDLER(OnRendering);

	//Messages Dispatching
	virtual void OnRButtonDown(NPoint pos);

	//Datas
	NMenuCtrl	m_wndMenu;

};
