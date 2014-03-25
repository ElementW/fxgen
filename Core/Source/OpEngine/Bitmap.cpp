//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Bitmap.cpp
//! \brief	bitmap resource type
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
#include "../../Include/CoreLib.h"
#include "Bitmap.h"


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							N2DBitmap class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(N2DBitmap, NResourceFx);

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
N2DBitmap::N2DBitmap() : NResourceFx()
{
	m_dwWidth=m_dwHeight=0;
	m_pbyPixels=null;
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
N2DBitmap::~N2DBitmap()
{
	if (m_pbyPixels)			NMemFree(m_pbyPixels);
}

//-----------------------------------------------------------------
//!	\brief	Set bitmap size
//!	\param	_w	Width
//!	\param	_h	Height
//-----------------------------------------------------------------
void N2DBitmap::SetSize(udword _w, udword _h)
{
	if (_w!=m_dwWidth || _h!=m_dwHeight)
	{
		//Bitmap resize
		if (m_pbyPixels)
		{
			NRGBA* pbyNewPixels = (NRGBA*)NMemAlloc(_w*_h*sizeof(NRGBA));

			NRGBA* pPxSrc	= m_pbyPixels;
			NRGBA* pPxDst	= pbyNewPixels;

			for (udword y=0; y<_h; y++)
			{
				for (udword x=0; x<_w; x++)
				{
					udword px = x * m_dwWidth/_w;
					udword py = y * m_dwHeight/_h;
					*pPxDst = pPxSrc[px + py*m_dwWidth];
					pPxDst++;
				}
			}

			NMemFree(m_pbyPixels);
			m_pbyPixels = pbyNewPixels;

		//Bitmap creation
		} else {
			m_pbyPixels = (NRGBA*)NMemAlloc(_w*_h*sizeof(NRGBA));
		}

		m_dwWidth	=_w;	m_dwHeight=_h;
	}

}

