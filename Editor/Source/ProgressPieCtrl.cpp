//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		ProgressPieCtrl.cpp
//! \brief	Progress Pie GUI controls
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		25-07-2008
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
#include "pch.h"
#include "ProgressPieCtrl.h"
//#include "Font.h"

#ifdef __GNUC__
#include <limits.h>
#endif



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NProgressPieCtrl Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//										Constructor
//-----------------------------------------------------------------
NProgressPieCtrl::NProgressPieCtrl()
{
	m_fPos=0.0f;
}

//-----------------------------------------------------------------
//										Destructor
//-----------------------------------------------------------------
NProgressPieCtrl::~NProgressPieCtrl()
{
}

//-----------------------------------------------------------------
//!	\brief	Control creation
//-----------------------------------------------------------------
bool NProgressPieCtrl::Create(NWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= "";
	wc.pwndParent		= parent;
	wc.rcRect				= NRect(0,0,0,0);
	wc.dwStyle			= NWS_POPUP;
	NWControl::Create(wc);

	return true;
}

//-----------------------------------------------------------------
//!	\brief	Display menu at screen position
//!	\return	Pointer to selected Item Desc
//-----------------------------------------------------------------
void NProgressPieCtrl::TrackPopup(NPoint _ptScreen, const NColor& _clr)
{
	TRACE("NProgressPieCtrl::TrackPopup\n");

	NRect rc(0,0,256,256);

	if (m_pParentWnd)
		m_pParentWnd->ScreenToClient(_ptScreen);

	rc.Move(_ptScreen);
	SetWindowRect(rc);

	SetFocus();
	
	ShowWindow(true);

	//Graphic update
	RedrawWindow();
}


//-----------------------------------------------------------------
//!	\brief	Paint
//-----------------------------------------------------------------
void NProgressPieCtrl::OnPaint()
{
	NRect rc = GetClientRect();

	NGraphics gfx(this);
	gfx.FillSolidRect(rc, RGBA(255,255,255,190));

		//Centre de l'ecran
		NPoint pt;
		pt = rc.CenterPoint();

		//Affichage du Cercle
		int		nbSegments	= 16;
		float fRayon1			= (min(rc.Width(), rc.Height()) / 4);
		float fRayon2			= (min(rc.Width(), rc.Height()) / 10);
		float incA				= (float) ((2.0f*nv_pi) / (float)nbSegments );
		float a						= 0;

		float aa	= m_fPos*nv_pi/180.0f;
		float e		= (180/(nbSegments-1))*nv_pi/180;
	
		for (int i=0; i<nbSegments; i++, a+=incA)
		{
			float fdist = DiffCap(a, aa);
			int d = fdist*255/(nv_pi);

			NPoint pts[4];
			pts[0].x = (cos(a+e) * fRayon1) + pt.x;
			pts[0].y = (sin(a+e) * fRayon1) + pt.y;
			pts[1].x = (cos(a-e) * fRayon1) + pt.x;
			pts[1].y = (sin(a-e) * fRayon1) + pt.y;

			pts[2].x = (cos(a-e) * fRayon2) + pt.x;
			pts[2].y = (sin(a-e) * fRayon2) + pt.y;
			pts[3].x = (cos(a+e) * fRayon2) + pt.x;
			pts[3].y = (sin(a+e) * fRayon2) + pt.y;

			gfx.Polygon(pts, 4, NColor(RGBA(d/2, d, d/3, 255)));

		}

}

//-----------------------------------------------------------------
//!	\brief	DiffCap
//-----------------------------------------------------------------
float NProgressPieCtrl::DiffCap(float _fCap1, float _fCap2)
{
	float fret;

	//Calcul de la difference
	fret =  _fCap1 - _fCap2;
	if(fret > nv_pi)		fret -= 2*nv_pi;
	if(fret < -nv_pi)		fret += 2*nv_pi;
	return abs(fret);
}