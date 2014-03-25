//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		NViewportsCtrl.h
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
#include "CoreLibPkg.h"

//-----------------------------------------------------------------
//!	\class		NViewportsCtrl
//!	\brief		GUI control for viewport
//-----------------------------------------------------------------
class NViewportsCtrl : public NGUIWnd
{

public:
	// Constructor-Destructor
	NViewportsCtrl();
	virtual ~NViewportsCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);

	void	Update();

protected:
	//Messages Dispatching
	//virtual	void	OnPaint();
	virtual void	OnSize();
};

