//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RectangularArray.h
//! \brief	Simple wrapper for pixel arrays used internally by operators.
//!
//!	\author	Sebastian Olter (qduaty@gmail.com)
//!	\date		31-10-2007
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
#include <math.h>

//! Simple wrapper for pixel arrays, which allows safe operation as an option.
//! It can be a bit slower than pointer arithmetics.
//! RectangularArray r(RGBA*, width);
//! RGBA col = r(x,y);
//! \param bilc Means the T stores linear combination weight (such as RGBA in alpha channel)

template < typename T, bool bilc = true > struct RectangularArray
{
	//! Empty ctor for allocation or global scope arrays.
	RectangularArray()
			: width(0), height(0), data(NULL), need_delete(false)
	{}

	//! Ctor with given array.
	//! \param w Mandatory width to find out how to deal with y
	//! \param h Optional height which enables safe array operation. Set width to 0 to disable safety.
	RectangularArray(T* array, size_t w, size_t h = 0)
			: width(w), height(h), data(array), need_delete(false)
	{}

	//! Ctor with heap allocation.
	RectangularArray(size_t w, size_t h)
			: width(w), height(h), data(new T[w*h]), need_delete(true)
	{}

	//! dtor
	~RectangularArray()
	{
		if (need_delete)
			delete[] data;
	}

	//! Get a pixel value from given position.
	template<typename size_type>inline T& operator()(size_type x, size_type y)
	{
		if (height)
			return *(data + (static_cast<size_t>(x) % width) + (static_cast<size_t>(y) % height) * width);
		else
			return *(data + static_cast<size_t>(x) + static_cast<size_t>(y) * width);
	}

	//! Get an approximate pixel value from an arbitrary position.
	//! This references 4 physical pixels.
	inline T get(double x, double y) const
	{
		double cy2 = (x - floor(x)) * (y - floor(y));
		double cy1 = (y - floor(y)) * (ceil(x) - x);
		double cx2 = (x - floor(x)) * (ceil(y) - y);
		double cx1 = 1 - (cx2 + cy1 + cy2);
		size_t ox = x, oy = y;
		T *x1, *y1, *x2, *y2;

		if (height)
		{
			size_t len = width * height; //opt
			x1 = data + ((ox + oy * width) % len);
			y1 = data + ((ox + (oy + 1) * width) % len);
			x2 = data + ((ox + 1 + oy * width) % len);
			y2 = data + ((ox + 1 + (oy + 1) * width) % len);
		}
		else
		{
			x1 = data + ox + (oy * width);
			y1 = data + ox + (oy + 1) * width;
			x2 = data + ox + 1 + oy * width;
			y2 = data + ox + 1 + (oy + 1) * width;
		}

		if (bilc)
			return *x1 + *x2*(cx2 - cx1) + *y1*(cy1 - cx2) + *y2*(cy2 - cy1 + cx2);
		else
			return *x1*cx1 + *x2*cx2 + *y1*cy1 + *y2*cy2;
	}

	//! Set a pixel value at an arbitrary position.
	//! This references 4 physical pixels.
	inline void set(double x, double y, const T& value)
	{
		double cy2 = (x - floor(x)) * (y - floor(y));
		double cy1 = (y - floor(y)) * (ceil(x) - x);
		double cx2 = (x - floor(x)) * (ceil(y) - y);
		double cx1 = 1 - (cx2 + cy1 + cy2);
		size_t ox = (size_t)x, oy = (size_t)y, oxn, oyn;
		T *x1, *y1, *x2, *y2;

		if (height)
		{
			oxn = (ox + 1) % width;
			ox %= width;
			oyn = ((oy + 1) % height) * width;
			oy = (oy % height) * width;
		}
		else
		{
			oxn = ox + 1;
			oy *= width;
			oyn = oy + width;
		}

		x1 = data + ox + oy;
		y1 = data + ox + oyn;
		x2 = data + oxn + oy;
		y2 = data + oxn + oyn;

		if (!bilc)
		{
			*x1 *= (1 - cx1);
			*x2 *= (1 - cx2);
			*y1 *= (1 - cy1);
			*y2 *= (1 - cy2);
		}

		*x1 += value * cx1;

		*y1 += value * cy1;
		*x2 += value * cx2;
		*y2 += value * cy2;
	}

	//! Get a pixel value from a linear position.
	template<typename size_type>inline T& operator[](size_type idx)
	{
		return data[max(0,min(width*height-1,idx))];
	}

	//! Obtain the number of pixels in array (array length) or zero if unsafe operation.
	inline size_t size()
	{
		return width * height;
	}

	//! Should they be public?
	size_t width, height;
	T* data;

private:
	bool need_delete;
};

typedef RectangularArray<RGBA> RGBAArray;

template <typename T> T mod(T num)
{
	return max(num, -num);
}
