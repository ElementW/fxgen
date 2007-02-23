//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Bitmap.cpp
//! \brief	bitmap resource type
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
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
#include "pch.h"
#include "Bitmap.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NBitmap class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NBitmap, NObject);

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NBitmap::NBitmap() : NObject()
{
	m_dwWidth=m_dwHeight= 256;
	m_pbyPixels = (RGBA*)NMemAlloc(m_dwWidth*m_dwHeight*sizeof(RGBA));
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NBitmap::~NBitmap()
{
	if (m_pbyPixels)			NMemFree(m_pbyPixels);
}

//-----------------------------------------------------------------
//!	\brief	Change bitmap size
//!	\param	_w	Width
//!	\param	_h	Height
//-----------------------------------------------------------------
void NBitmap::SetSize(udword _w, udword _h)
{
	m_dwWidth	=_w;
	m_dwHeight=_h;
	m_pbyPixels = (RGBA*)NMemRealloc(m_pbyPixels, m_dwWidth*m_dwHeight*sizeof(RGBA));
}
