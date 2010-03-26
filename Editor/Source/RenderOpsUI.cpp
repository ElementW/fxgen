//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RenderOpsUI.h
//! \brief	Render Operators Node for user interface
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!
//!	\date		26-02-2010
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "RenderOpsUI.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NFlatOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NFlatOp, NOperatorNode)
	NEWFIELD(eInteger, "width", NFlatOp, wpow, 0, 255, 1, "8", "NUbyteProp"),
	NEWFIELD(eInteger, "height", NFlatOp, hpow, 0, 255, 1, "8", "NUbyteProp"),
	NEWFIELD(eRgba,			"color", NFlatOp, color, 1, 255, 1, "0,0,0,255", "NColorProp"),
FIMPLEMENT_CLASS_END()

NFlatOp::NFlatOp()
{
	wpow=hpow=64;
	color=NRGBA(255,0,0,255);
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCloudOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NCloudOp, NOperatorNode)
FIMPLEMENT_CLASS_END()

NCloudOp::NCloudOp()
{
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NGradientOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NGradientOp, NOperatorNode)
FIMPLEMENT_CLASS_END()

NGradientOp::NGradientOp()
{
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCellOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NCellOp, NOperatorNode)
FIMPLEMENT_CLASS_END()

NCellOp::NCellOp()
{
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NNoiseOp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(NNoiseOp, NOperatorNode)
FIMPLEMENT_CLASS_END()

NNoiseOp::NNoiseOp()
{
}

