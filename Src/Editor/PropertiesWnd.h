//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PropertiesWnd.h
//! \brief	Window for properties editing
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
//                   Includes
//-----------------------------------------------------------------
#include "PropertiesCtrl.h"
#include "Editor.h"

//-----------------------------------------------------------------
//!	\class		NPropertiesWnd
//!	\brief		Object's Properties windows viewer
//-----------------------------------------------------------------
class NPropertiesWnd :	public NPropertiesCtrl
{
public:
	NPropertiesWnd(void);
	virtual ~NPropertiesWnd(void);

	//Methods
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent);


protected:
	//Events
	EVT_DECLARE_HANDLER(OnOPSelChanged);
	EVT_DECLARE_HANDLER(OnOPDeleting);
	EVT_DECLARE_HANDLER(OnPageSelected);

	// Messages Notify
	virtual void OnPropertiesChanged();

	//Win32 Messages Dispatching

	//Datas
	NOperator* m_pcurOp;	//!< current viewed operator
};
