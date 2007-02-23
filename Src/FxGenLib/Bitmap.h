//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Bitmap.h
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
#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "Core.h"

//-----------------------------------------------------------------
//!	\struct	RGBA
//!	\brief	8Bits composants
//-----------------------------------------------------------------
struct RGBA
{
	union
	{
		struct { ubyte r, g, b, a;	};
		ubyte		col_array[4];
		udword	dwCol;
		struct { ubyte x, y, z, w;	};
	};

};


//-----------------------------------------------------------------
//!	\struct	RGBI
//!	\brief	32Bits composants
//-----------------------------------------------------------------
struct RGBI
{
	sdword r,g,b;
};

//-----------------------------------------------------------------
//!	\class	NBitmap
//!	\brief	Bitmap resource type
//-----------------------------------------------------------------
class NBitmap : public NObject
{
public:
	//Constructor-Destructor
					NBitmap();
	virtual	~NBitmap();

	DECLARE_CLASS();

	//Methods
	void SetSize(udword _w, udword _h);

	//Members Access
	udword				GetWidth()			{ return m_dwWidth;			}
	udword				GetHeight()			{ return m_dwHeight;		}
	RGBA*					GetPixels()			{ return m_pbyPixels;		}

protected:
	//Datas
	RGBA*		m_pbyPixels;
	udword	m_dwWidth, m_dwHeight;
};
