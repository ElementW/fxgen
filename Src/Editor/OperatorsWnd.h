//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		OperatorsWnd.h
//! \brief	Windows for operators editing
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
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent);
	virtual void OnMarkShowOperator(NOperator* pop);

	virtual void OnDeletingOperator(NOperator* pop);
	virtual void OnDeletedOperator(NOperator* pop);

	//Methods Debug
	void Dump();

protected:
	//Methods
	void InitCtxMenu();

//	NRTClass* GetFirstClassBySuperClass(char* _pszSuperClassName);
//	NRTClass* GetNextClassBySuperClass(char* _pszSuperClassName, NRTClass* _prtclass);

	//Events
	EVT_DECLARE_HANDLER(OnPageSelected);
	EVT_DECLARE_HANDLER(OnRendering);

	//Win32 Messages Dispatching
	virtual void OnRightButtonDown(udword flags, NPoint pos);
	virtual	void OnCommand(udword id);

	//Datas
	//HMENU			m_hMenu;
	NMenuCtrl	m_wndMenu;

};
