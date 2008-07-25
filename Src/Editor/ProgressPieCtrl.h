//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ProgressPieCtrl.h
//! \brief	Progress Pie GUI controls
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		27-05-2008
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
#include "GUI.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//class NWControl
		class NProgressPieCtrl;

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//!	\class		NProgressPieCtrl
//!	\brief		GUI control for color picker
//-----------------------------------------------------------------
class NProgressPieCtrl : public NWControl
{

public:
	// Constructor-Destructor
	NProgressPieCtrl();
	virtual ~NProgressPieCtrl();

	// Methods
	virtual	bool Create(NWnd* parent);

	// Methods
	void	TrackPopup(NPoint _ptScreen, const NColor& _clr);

protected:
	// Internal methods
	float DiffCap(float _fCap1, float _fCap2);

	// Datas
	NColor	m_clr;
	float		m_fPos;

protected:
	//Messages Dispatching
	virtual	void	OnPaint();
};

