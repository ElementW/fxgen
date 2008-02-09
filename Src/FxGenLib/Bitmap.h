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

	//! Non-commutative +: argument is put on top existing color based on alpha
	RGBA operator+(const RGBA& v) const
	{
		RGBA retval;
		float alpha = v.a / 255.f;
		float beta = 1- alpha; //opt
		retval.a = v.a     +     a * beta;
		retval.r = v.r * alpha + r * beta;
		retval.g = v.g * alpha + g * beta;
		retval.b = v.b * alpha + b * beta;
		return retval;
	}

	//! Non-commutative +: argument is put on top existing color based on alpha
	void operator+=(const RGBA& v)
	{
		float alpha = v.a / 255.f;
		float beta = 1- alpha; //opt
		a = v.a     +     a * beta;
		r = v.r * alpha + r * beta;
		g = v.g * alpha + g * beta;
		b = v.b * alpha + b * beta;
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

//	virtual void					saveBMP(char *filename);

protected:
	//Datas
	RGBA*		m_pbyPixels;
	udword	m_dwWidth, m_dwHeight;
};
