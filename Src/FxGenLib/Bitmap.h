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
#ifndef BITMAP_H
#define BITMAP_H

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
	RGBA() {}
	RGBA(ubyte R, ubyte G, ubyte B, ubyte A): r(R), g(G), b(B), a(A) {}
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
		retval.a = ubyte(retval.a * c);
		return retval;
	}

	void operator*=(float c)
	{
		a = ubyte(a * c);
	}

	//! Non-commutative +: argument is put on top existing color based on alpha
	RGBA operator+(const RGBA& v) const
	{
		RGBA retval;
		float alpha = v.a / 255.f;
		float beta = 1- alpha; //opt
		retval.a = ubyte(v.a     +     a * beta);
		retval.r = ubyte(v.r * alpha + r * beta);
		retval.g = ubyte(v.g * alpha + g * beta);
		retval.b = ubyte(v.b * alpha + b * beta);
		return retval;
	}

	//! Non-commutative +: argument is put on top existing color based on alpha
	void operator+=(const RGBA& v)
	{
		float alpha = v.a / 255.f;
		float beta = 1- alpha; //opt
		a = ubyte(v.a     +     a * beta);
		r = ubyte(v.r * alpha + r * beta);
		g = ubyte(v.g * alpha + g * beta);
		b = ubyte(v.b * alpha + b * beta);
	}
};


//-----------------------------------------------------------------
//!	\struct	RGBAI
//!	\brief	32Bits composants
//-----------------------------------------------------------------
struct RGBAI
{
	RGBAI(){}
	RGBAI(sdword R, sdword G, sdword B, sdword A): r(R), g(G), b(B), a(A) {}
	sdword r,g,b,a;
};

//-----------------------------------------------------------------
//!	\class	NBitmap
//!	\brief	Bitmap resource type
//-----------------------------------------------------------------
class FXGEN_API NBitmap : public NObject
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

#endif //BITMAP_H

