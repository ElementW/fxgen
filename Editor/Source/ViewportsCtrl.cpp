//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		NViewportsCtrl.cpp
//! \brief	GUI control for viewport
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
//                   Includes
//-----------------------------------------------------------------
#include "ViewportsCtrl.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NViewportsCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NViewportsCtrl::NViewportsCtrl()
{
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NViewportsCtrl::~NViewportsCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NViewportsCtrl::Create(const char* name, const NRect& rect, NGUIWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= const_cast<char*>(name);
	wc.pwndParent		= parent;
	wc.rcRect				= rect;
	wc.dwStyle			= NWS_VISIBLE;
	NGUIWnd::Create(wc);

	return true;
}


//-----------------------------------------------------------------
//!	\brief	Update control
//-----------------------------------------------------------------
void NViewportsCtrl::Update()
{
	RedrawWindow();
}

void NViewportsCtrl::OnSize()
{
	//Redraw
	Update();
}


