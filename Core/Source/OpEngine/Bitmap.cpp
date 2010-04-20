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
//#include "../../Include/CoreLib.h"
#include "Bitmap.h"
#include "EngineOp.h"
#include "../Types.h"

//-----------------------------------------------------------------
//!	\func	Bitmap_SetSize
//!	\brief	Change Bitmap Size
//-----------------------------------------------------------------
void Res_SetBmpSize(SResource* _pres, udword _w, udword _h)
{
	if (_w!=_pres->dwWidth || _h!=_pres->dwHeight)
	{
		//Bitmap resize
		if (_pres->pbyPixels!=null)
		{
			NRGBA* pbyNewPixels = (NRGBA*)NNEWARRAY(NRGBA, _w*_h);

			NRGBA* pPxSrc	= _pres->pbyPixels;
			NRGBA* pPxDst	= pbyNewPixels;

			for (udword y=0; y<_h; y++)
			{
				for (udword x=0; x<_w; x++)
				{
					udword px = x * _pres->dwWidth/_w;
					udword py = y * _pres->dwHeight/_h;
					*pPxDst = pPxSrc[px + py*_pres->dwHeight];
					pPxDst++;
				}
			}

			NDELETEARRAY(_pres->pbyPixels);
			_pres->pbyPixels = pbyNewPixels;

		//Bitmap creation
		} else {
			_pres->pbyPixels = (NRGBA*)NNEWARRAY(NRGBA, _w*_h);
		}

		_pres->dwWidth = _w;
		_pres->dwHeight= _h;
	}

}

void Res_CopyBmp(SResource* _pres1, SResource* _pres2)
{
	udword w = _pres1->dwWidth;
	udword h = _pres1->dwHeight;
	if (_pres2->pbyPixels && (_pres2->dwWidth!=w || _pres2->dwHeight!=h))
	{
		NDELETEARRAY(_pres2->pbyPixels);
		_pres2->pbyPixels = (NRGBA*)NNEWARRAY(NRGBA, w*h);
	} else if (_pres2->pbyPixels==null) {
		_pres2->pbyPixels = (NRGBA*)NNEWARRAY(NRGBA, w*h);
	}
	_pres2->dwWidth=w;
	_pres2->dwHeight=h;
	NMemCopy(_pres2->pbyPixels, _pres1->pbyPixels, _pres1->dwWidth*_pres1->dwHeight*sizeof(NRGBA*));
}



/*
//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
NBitmap::NBitmap() : NObject()
{
	m_dwWidth=m_dwHeight=0;
	m_pbyPixels=null;
	//m_dwWidth=m_dwHeight= 256;
	//m_pbyPixels = (NRGBA*)NMemAlloc(m_dwWidth*m_dwHeight*sizeof(NRGBA));
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NBitmap::~NBitmap()
{
	if (m_pbyPixels)			NDELETEARRAY(m_pbyPixels);
}

//-----------------------------------------------------------------
//!	\brief	Set bitmap size
//!	\param	_w	Width
//!	\param	_h	Height
//-----------------------------------------------------------------
void NBitmap::SetSize(udword _w, udword _h)
{
	if (_w!=m_dwWidth || _h!=m_dwHeight)
	{
		//Bitmap resize
		if (m_pbyPixels)
		{
			NRGBA* pbyNewPixels = (NRGBA*)NNEWARRAY(NRGBA, _w*_h);

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

			NDELETEARRAY(m_pbyPixels);
			m_pbyPixels = pbyNewPixels;

		//Bitmap creation
		} else {
			m_pbyPixels = (NRGBA*)NNEWARRAY(NRGBA, _w*_h);
		}

		m_dwWidth	=_w;	m_dwHeight=_h;
	}

}
*/

