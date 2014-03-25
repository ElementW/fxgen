//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PropertiesWnd.cpp
//! \brief	Window for properties editing
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

//-----------------------------------------------------------------
//			Includes
//-----------------------------------------------------------------
#include "PropertiesWnd.h"
#include "OpGraphModel.h"

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
	EVT_UNREGISTERALL();
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NPropertiesWnd::Create(const char* name, const NRect& rect, NGUIWnd* parent)
{
	NPropertiesCtrl::Create(name, rect, parent);

	EVT_REGISTER(EVT_OPSELECTED, (EVENTFNC)&NPropertiesWnd::OnOPSelChanged);
	EVT_REGISTER(EVT_OPDELETING, (EVENTFNC)&NPropertiesWnd::OnOPDeleting);
	EVT_REGISTER(EVT_PAGESELECTED, (EVENTFNC)&NPropertiesWnd::OnPageSelected );

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Delegate for properties value changed
//-----------------------------------------------------------------
void NPropertiesWnd::OnPropertiesChanged()
{
	//Set Operator Invalid
	if (m_pcurOp)
		GetApp()->GetMainWnd()->EmitPropertiesChanged(m_pcurOp);

}

//-----------------------------------------------------------------
//!	\brief	Event for operator selection changed
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NPropertiesWnd, OnOPSelChanged)
{
	NOperatorNode* pnode = (NOperatorNode*)dwParam1;
	if (pnode!=null)		m_pcurOp = pnode->m_op;
	else								m_pcurOp = null;
	DisplayObjectProperties((NObject*)m_pcurOp);
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

//-----------------------------------------------------------------
//!	\brief	Event for page selection changed
//-----------------------------------------------------------------
EVT_IMPLEMENT_HANDLER(NPropertiesWnd, OnPageSelected)
{
	m_pcurOp = null;
	DisplayObjectProperties(null);
	Update();

	return 0;
}

