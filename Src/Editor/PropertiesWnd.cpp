//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PropertiesWnd.cpp
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

//-----------------------------------------------------------------
//			Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "PropertiesWnd.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NPropertiesWnd Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NPropertiesWnd::NPropertiesWnd(void)
{
	m_pcurOp=null;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NPropertiesWnd::~NPropertiesWnd(void)
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NPropertiesWnd::Create(const char* name, const NRect& rect, NWnd* parent)
{
	NPropertiesCtrl::Create(name, rect, parent);

	EVT_REGISTER(EVT_OPSELECTED, (EVENTFNC)&NPropertiesWnd::OnOPSelChanged);
	EVT_REGISTER(EVT_OPDELETING, (EVENTFNC)&NPropertiesWnd::OnOPDeleting);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Delegate for properties value changed
//-----------------------------------------------------------------
void NPropertiesWnd::OnPropertiesChanged()
{
	//Set Operator Invalid
	if (m_pcurOp)
		NEngineOp::GetEngine()->InvalidateOp(m_pcurOp);

}

//-----------------------------------------------------------------
//!	\brief	Event for operator selection changed
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NPropertiesWnd, OnOPSelChanged)
{
	m_pcurOp = (NOperator*)dwParam1;
	DisplayObjectProperties(m_pcurOp);
	Update();

	return 0;
}

//-----------------------------------------------------------------
//!	\brief	Event for operator deleting
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NPropertiesWnd, OnOPDeleting)
{
	m_pcurOp = null;
	DisplayObjectProperties(null);
	Update();

	return 0;
}
