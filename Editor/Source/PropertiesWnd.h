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
#ifndef PROPERTIESWND_H
#define PROPERTIESWND_H


//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "PropertiesCtrl.h"
#include "EditorApp.h"

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
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);


protected:
	//m_dwWidth
	EVT_DECLARE_HANDLER(OnOPSelChanged);
	EVT_DECLARE_HANDLER(OnOPDeleting);
	EVT_DECLARE_HANDLER(OnPageSelected);

	// Messages Notify
	virtual void OnPropertiesChanged();

	//Datas
	NOperatorFx* m_pcurOp;	//!< current viewed operator FX
};

#endif //PROPERTIESWND_H
