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
#include "../Include/FxGenLib.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
struct SResource;

//-----------------------------------------------------------------
//!	\struct	NRGBA
//!	\brief	8Bits composants
//-----------------------------------------------------------------
struct NRGBA
{
	NRGBA() {}
	NRGBA(ubyte R, ubyte G, ubyte B, ubyte A): r(R), g(G), b(B), a(A) {}
	union
	{
		struct { ubyte r, g, b, a;	};
		ubyte		col_array[4];
		udword	dwCol;
		struct { ubyte x, y, z, w;	};
	};

	float grey()
	{
		return (r + g + b) / 3.f;
	}

	NRGBA operator*(float c) const
	{
		NRGBA retval(*this);
		retval.a = ubyte(retval.a * c);
		return retval;
	}

	void operator*=(float c)
	{
		a = ubyte(a * c);
	}

	//! Non-commutative +: argument is put on top existing color based on alpha
	NRGBA operator+(const NRGBA& v) const
	{
		NRGBA retval;
		float alpha = v.a / 255.f;
		float beta = 1- alpha; //opt
		retval.a = ubyte(v.a     +     a * beta);
		retval.r = ubyte(v.r * alpha + r * beta);
		retval.g = ubyte(v.g * alpha + g * beta);
		retval.b = ubyte(v.b * alpha + b * beta);
		return retval;
	}

	//! Non-commutative +: argument is put on top existing color based on alpha
	void operator+=(const NRGBA& v)
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
//!	\struct	NRGBAI
//!	\brief	32Bits composants
//-----------------------------------------------------------------
struct NRGBAI
{
	NRGBAI(){}
	NRGBAI(sdword R, sdword G, sdword B, sdword A): r(R), g(G), b(B), a(A) {}
	sdword r,g,b,a;
};

//-----------------------------------------------------------------
//										Functions
//-----------------------------------------------------------------
void Res_SetBmpSize(SResource* _pres, udword _w, udword _h);
void Res_CopyBmp(SResource* _pres1, SResource* _pres2);

//-----------------------------------------------------------------
//!	\class	NBitmap
//!	\brief	Bitmap resource type
//-----------------------------------------------------------------
/*class FXGEN_API NBitmap //: public NRessource
{
public:
	//Constructor-Destructor
					NBitmap();
	virtual	~NBitmap();
	//Methods
	void SetSize(udword _w, udword _h);

	//Members Access
	udword		GetWidth()			{ return m_dwWidth;			}
	udword		GetHeight()			{ return m_dwHeight;		}
	NRGBA*		GetPixels()			{ return m_pbyPixels;		}

//	virtual void					saveBMP(char *filename);

protected:
	//Datas
	NRGBA*		m_pbyPixels;
	udword	m_dwWidth, m_dwHeight;
};
*/

#endif //BITMAP_H

