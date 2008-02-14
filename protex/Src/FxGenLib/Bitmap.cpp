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

#include "noiseutils.h"

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
	m_dwWidth=m_dwHeight=0;
	m_pbyPixels=null;
	//m_dwWidth=m_dwHeight= 256;
	//m_pbyPixels = (RGBA*)NMemAlloc(m_dwWidth*m_dwHeight*sizeof(RGBA));
}

//-----------------------------------------------------------------
//!	\brief	Destructor
//-----------------------------------------------------------------
NBitmap::~NBitmap()
{
	if (m_pbyPixels)			NMemFree(m_pbyPixels);
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
			RGBA* pbyNewPixels = (RGBA*)NMemAlloc(_w*_h*sizeof(RGBA));

			RGBA* pPxSrc	= m_pbyPixels;
			RGBA* pPxDst	= pbyNewPixels;

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
			m_pbyPixels = (RGBA*)NMemAlloc(_w*_h*sizeof(RGBA));
		}
		
		m_dwWidth	=_w;	m_dwHeight=_h;
	}

}


//-----------------------------------------------------------------
//!	\brief	Change bitmap size
//!	\param	_w	Width
//!	\param	_h	Height
//-----------------------------------------------------------------
void NBitmap::saveBMP(char *filename)
{
	

	int divide_by_128_w = m_dwWidth / 128;
	int tiles_per_row = divide_by_128_w;

	for( int X = 0; X < tiles_per_row; X++ )
	{
		for( int Y = 0; Y < tiles_per_row; Y++ )
		{

			RGBA* pdata = m_pbyPixels ;
			noise::utils::Image *image = new noise::utils::Image( 128, 128 );

			for( int i = 0; i < 128	; i++ )
			{
				for( int j = 0; j < 128; j++ )
				{
					// I = i+X*128;
					// J = j+Y*128
					// pdata index = (i*128+j)
					int index = ((128 * m_dwWidth * X) + (i-X)*m_dwWidth) + (Y*128 + (j-Y));
					image->SetValue( j, 127-i, 
						noise::utils::Color(pdata[index].r, 
											pdata[index].g, 
											pdata[index].b, 
											pdata[index].a));

					//int offset = 1+(X*divide_by_128_w + Y)*128;
					//*pdata++;

				}
			}

			noise::utils::WriterBMP writer;

			std::string fname(filename);
			char buff[256];
			sprintf(buff, "%d%d.bmp", X, Y);
			fname+=buff;
			writer.SetDestFilename (fname);
			writer.SetSourceImage(*image);
			
			writer.WriteDestFile();

			delete image;
		}
	}
}

