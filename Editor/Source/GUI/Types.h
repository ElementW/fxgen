//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Types.h
//! \brief	GUI Types
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
#ifndef TYPES_H
#define TYPES_H

//-----------------------------------------------------------------
//!	\class	NString
//!	\brief	String class
//-----------------------------------------------------------------
class  NString
{
public:
	//Constructors-Destructor
			NString();
			NString(const char* string);
			NString(const NString& string);
			~NString();

	//Methods
			udword			Length		()	const 				{ return mBuffer?(udword)strlen(mBuffer):0;	}		//Return lenght without null caracter.
			void				SetLength	(udword length);													//lenght without null caracter.
			udword			Find		(char c, udword index);
			udword			Find		(const char* str, udword index);
			udword			ReverseFind	(char c);
			NString			Mid			(udword first, udword count=-1);

			NString			ExtractLine	(udword& i);
			udword			ExtractWord(udword i, NString& strWord);
			udword			SearchWord	(udword  i);

			udword			ExtractToken(udword i, NString& strToken, char* pszSeparators);

			NString&		Format		(const char* str, ...);

			void				InsertAt(udword _idx, const char* _str);
			void				RemoveAt(udword _idx, udword _count);

	//Members access
			char*				Buffer()	const 	{ return mBuffer;			}

	//Operators
			NString& operator=	(const NString& string)	{
									if (mBuffer)	free(mBuffer);
									udword len = string.Length();
									mBuffer = (char*)malloc(len+1);
									NMemCopy(mBuffer, string.Buffer(), len);
									mBuffer[len] = 0;
									return *this;
								}

			NString& operator=	(char* string)	{
									if (mBuffer)	free(mBuffer);
									udword len = (udword)strlen(string);
									mBuffer = (char*)malloc(len+1);
									NMemCopy(mBuffer, string, len);
									mBuffer[len] = 0;
									return *this;
								}

			NString& operator+	(const char* string) {
									if (mBuffer) {
										udword len1 = Length();
										udword len2 = (udword)strlen(string);
										mBuffer = (char*)realloc(mBuffer, len1 + len2 + 1);
										NMemCopy(mBuffer+len1, string, len2);
										mBuffer[len1+len2] = 0;
										return *this;
									} else {
										udword len = (udword)strlen(string);
										mBuffer = (char*)malloc(len+1);
										NMemCopy(mBuffer, string, len);
										mBuffer[len] = 0;
										return *this;
									}

								}


			NString& operator+=	(const char* string)	{ return *this + string; }

protected:
	//Datas
			char*				mBuffer;
};


//-----------------------------------------------------------------
//!	\class	NPoint
//!	\brief	Point class
//-----------------------------------------------------------------
class  NPoint
{
public:
	//Constructor-Destructor
								NPoint()											{ x = y = 0;		}
								NPoint(sdword X, sdword Y)							{ x = X; y = Y;	}

	//Arithmetic operators
				NPoint			operator+(NPoint& P)		{ return NPoint(x + P.x, y + P.y);																}
				NPoint			operator-(NPoint& P)		{ return NPoint(x - P.x, y - P.y);																}
				NPoint			operator-()  const			{ return NPoint(-x, -y);																			}
				NPoint			operator*(sdword m)			{ return NPoint(x * m, y * m);																	}
				NPoint			operator*(float m)			{ return NPoint((sdword) (x * m), (sdword) (y * m));												}
				NPoint			operator/(sdword f)			{ return NPoint((sdword) ((float) x / (float) f), (sdword) ((float) y / (float) f));				}
				NPoint			operator/(float f)			{ return NPoint((sdword) ((float) x / f), (sdword) ((float) y / f));								}
				NPoint&			operator+=(NPoint& P)		{ x += P.x;				y += P.y;		return *this;											}
				NPoint&			operator-=(NPoint& P)		{ x -= P.x;				y -= P.y;		return *this;											}
				NPoint&			operator*=(sdword m)		{ x *= m;	y *= m;		return *this;															}
				NPoint&			operator*=(float m)			{ x = (sdword) ((float) x * m);	 y = (sdword) ((float) y * m);	return *this;					}
				NPoint&			operator/=(sdword f)		{ x = (sdword) ((float) x / (float) f);		y = (sdword) ((float) y / (float) f);	return *this;	}
				NPoint&			operator/=(float f)			{ x = (sdword) ((float) x / f);				y = (sdword) ((float) y / f);			return *this;	}
				sdword			operator|(NPoint& P)		{ return x * P.x + y * P.y;																			}
				sdword			operator*(NPoint& P)		{ return x * P.y - y * P.x;																			}

	//Comparison operators
				bool			operator==(NPoint& P)			{ return ( x == P.x && y == P.y );																	}
				bool			operator!=(NPoint& P)			{ return ( x != P.x || y != P.y );																	}


	//Datas
	sdword x,y;
};


//-----------------------------------------------------------------
//!	\class	NRect
//!	\brief	Point class
//-----------------------------------------------------------------
class  NRect
{
public:
	//Constructors
	NRect()																					{ left = top = right = bottom = 0;					}
	NRect(const NRect& r)																	{ left = r.left; top = r.top; right = r.right; bottom = r.bottom;	}
	NRect(sdword l, sdword t, sdword r, sdword b)		{ left = l; top = t; right = r; bottom = b;	}

	//Methods
	sdword Width() const 							{ return right-left;	}
	sdword Height() const 							{ return bottom-top;	}

	NPoint CenterPoint()						{ return NPoint(left + Width()/2, top + Height()/2);								}
	NRect& Center(NRect& P)					{ Move( (P.Width() - Width()) / 2 + P.left - left, (P.Height() - Height()) / 2 + P.top - top);	return *this;	}

	bool Contain(NPoint& P)					{ return ((P.x >= left) && (P.x <= right) && (P.y >= top) && (P.y <= bottom));		}
	bool Contain(NRect& R)					{ return ( (left <= R.left) && (right >= R.right) && (top <= R.top) && (bottom >= R.bottom) );	}

	NRect& Inflate(sdword x, sdword y)												{ left -= x; right += x; top -= y; bottom += y;  return *this;						}
	NRect& Inflate(sdword l, sdword t, sdword r, sdword b)		{ left -= l; right += r; top -= t; bottom += b;	 return *this;						}
	inline NRect& Inflate(NRect& R);

	NRect& Deflate(sdword x, sdword y)												{ left += x; right -= x; top += y; bottom -= y;  return *this;						}
	NRect& Deflate(sdword l, sdword t, sdword r, sdword b)		{ left += l; right -= r; top += t; bottom -= b;	 return *this;						}
	inline	NRect& Deflate(NRect& R);

	inline	NRect& Normalize();
	inline	bool IsNormalized()		{ return !(top > bottom || left > right);	}

	NRect& Move(sdword x, sdword y)			{ left += x; right += x; top += y; bottom += y;	return *this;						}
	NRect& Move(NPoint& P)							{ left += P.x; right += P.x; top += P.y; bottom += P.y;	return *this;				}

	inline	NRect&	Intersect(NRect& R);
	inline	bool		IsIntersected(NRect& R);
	inline	NRect&	Union(const NRect& R);

	bool IsEmpty()		{ return (left==right || top==bottom); }

	//Comparison operators
	inline	bool	operator==(NRect& R);
	inline	bool	operator!=(NRect& R);

	//Datas
	sdword left,top,right,bottom;
};



//-----------------------------------------------------------------
//!	\class	NColor
//!	\brief	Color class (0xAARRGGBB)
//-----------------------------------------------------------------
#ifdef RGBA
	#undef RGBA
#endif
#define RGBA(r,g,b,a)          ((udword)((a<<24)|(b<<16)|(g<<8)|r ))

class  NColor
{
public:
	//Constructors
	NColor()							{	mR=mG=mB=mA=0.0f;							}

	NColor(udword col)		{	mA=((float)((col&0xff000000)>>24))/255.0f;
													mB=((float)((col&0xff0000)>>16))/255.0f;
													mG=((float)((col&0xff00)>>8))/255.0f;
													mR=((float)(col&0xff))/255.0f;}

	//NColor(float a, float r, float g, float b)		{	mA=a; mR=r; mG=g; mB=b; }
	NColor(ubyte r, ubyte g, ubyte b, ubyte a)		{	mA=(float)a/255.0f; mR=(float)r/255.0f; mG=(float)g/255.0f; mB=(float)b/255.0f; }

	//Methods
	ubyte		GetR()				{	return (ubyte)(mR*255.0f);					}
	ubyte		GetG()				{	return (ubyte)(mG*255.0f);					}
	ubyte		GetB()				{	return (ubyte)(mB*255.0f);					}
	ubyte		GetA()				{	return (ubyte)(mA*255.0f);					}

	//udword	GetARGB()			{	return (udword)((GetA()<<24)|(GetR()<<16)|(GetG()<<8)|GetB() );	}
	udword	GetRGBA()			{	return (udword)((GetA()<<24)|(GetB()<<16)|(GetG()<<8)|GetR() );	}

	NColor&				ColorLerp(const NColor *pC1, const NColor *pC2, float s) {
							mR = pC1->mR + s * (pC2->mR - pC1->mR);
							mG = pC1->mG + s * (pC2->mG - pC1->mG);
							mB = pC1->mB + s * (pC2->mB - pC1->mB);
							mA = pC1->mA + s * (pC2->mA - pC1->mA);
							return *this; }


	void ToHLS(float &_h, float &_l, float &_s);	//!< Just return conversion
	void SetFromHLS(float _h, float _l, float _s);	//!< Set New Color from HLS
	float ToRGB1(float rm1, float rm2, float rh);

	//Datas
		union {
			float		c[4];
			struct {	float	mR,mG,mB,mA;	};

		};
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Inlines from the NString class
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//NString inlines methods
inline	bool	operator== (const NString& str1, const NString& str2)	{ return strcmp(str1.Buffer(), str2.Buffer())==0;	}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Inlines from the NRect class
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

bool NRect::operator==(NRect& R)
{
	return (left == R.left && right == R.right && top == R.top && bottom == R.bottom);
}

bool NRect::operator!=(NRect& R)
{
	return (left != R.left || right != R.right || top != R.top || bottom != R.bottom);
}


NRect& NRect::Inflate(NRect& R)
{
	left	-= R.left;
	right	+= R.right;
	top		-= R.top;
	bottom	+= R.bottom;

	return *this;
}

NRect& NRect::Deflate(NRect& R)
{
	left	+= R.left;
	right	-= R.right;
	top		+= R.top;
	bottom	-= R.bottom;

	return *this;
}

NRect& NRect::Normalize()
{
	sdword temp;
	if (top  > bottom)		{	temp = top;	top = bottom;	bottom = temp;	}
	if (left > right )		{	temp = left;	left = right;	right = temp;	}

	return *this;
}

NRect& NRect::Intersect(NRect& R)
{
	if (left   < R.left  )	left	= R.left;
	if (top    < R.top   )	top	= R.top;
	if (right  > R.right )	right	= R.right;
	if (bottom > R.bottom)	bottom	= R.bottom;

	return *this;
}

bool NRect::IsIntersected(NRect& R)
{
	NRect	tempRect = *this;
	tempRect.Intersect(R);

	return !tempRect.IsNormalized();
}

NRect& NRect::Union(const NRect& R)
{
	if (left   > R.left  )	left	= R.left;
	if (top    > R.top   )	top	= R.top;
	if (right  < R.right )	right	= R.right;
	if (bottom < R.bottom)	bottom	= R.bottom;

	return *this;
}

#endif //TYPES_H
