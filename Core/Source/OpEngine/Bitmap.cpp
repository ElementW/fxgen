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
#include "../../Include/CoreLib.h"
#include "Bitmap.h"


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							N2DBitmap class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
FIMPLEMENT_CLASS(N2DBitmap, NObject);

//-----------------------------------------------------------------
//!	\brief	Constructor
//-----------------------------------------------------------------
N2DBitmap::N2DBitmap() : NObject()
{
	m_dwWidth=m_dwHeight=0;
	m_pbyPixels=null;
	//m_dwWidth=m_dwHeight= 256;
	//m_pbyPixels = (NRGBA*)NMemAlloc(m_dwWidth*m_dwHeight*sizeof(NRGBA));
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

////-----------------------------------------------------------------
////!	\brief	Change bitmap size
////!	\param	_w	Width
////!	\param	_h	Height
////-----------------------------------------------------------------
//void N2DBitmap::saveBMP(char *filename)
//{
//	NRGBA* pdata = m_pbyPixels;
//
//	noise::utils::Image *image = new noise::utils::Image( m_dwWidth, m_dwHeight );
//
//	for( int i = 0; i < m_dwHeight; i++ )
//	{
//		for( int j = 0; j < m_dwWidth; j++ )
//		{
//			image->SetValue( j, i,
//				noise::utils::Color((*pdata).r,
//									(*pdata).g,
//									(*pdata).b,
//									(*pdata).a));
//
//			*pdata++;
//
//		}
//	}
//
//	noise::utils::WriterBMP writer;
//
//	std::string fname(filename);
//	writer.SetDestFilename (fname);
//	writer.SetSourceImage(*image);
//
//	writer.WriteDestFile();
//
//	delete image;
//}
//
