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

	RGBA operator*(float c) const
	{
		RGBA retval(*this);
		retval.a *= c;
		return retval;
	}

	void operator*=(float c)
	{
		a *= c;
	}

	RGBA operator+(const RGBA& v) const
	{
		RGBA retval;
		float alpha = v.a / 255.f;
		retval.a = v.a     +     a * (1 - alpha);
		retval.r = v.r * alpha + r * (1 - alpha);
		retval.g = v.g * alpha + g * (1 - alpha);
		retval.b = v.b * alpha + b * (1 - alpha);
		return retval;
	}

	void operator+=(const RGBA& v)
	{
		float alpha = v.a / 255.f;
		a = v.a     +     a * (1 - alpha);
		r = v.r * alpha + r * (1 - alpha);
		g = v.g * alpha + g * (1 - alpha);
		b = v.b * alpha + b * (1 - alpha);
	}
};


//-----------------------------------------------------------------
//!	\struct	RGBAI
//!	\brief	32Bits composants
//-----------------------------------------------------------------
struct RGBAI
{
	sdword r,g,b,a;
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
