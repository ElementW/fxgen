//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Types.cpp
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

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Types.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NString class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------
NString::NString()
{
	mBuffer = null;
}

NString::NString(const char* string)
{
	udword len = (udword)strlen(string);
	mBuffer = (char*)malloc(len+1);
	memcpy(mBuffer, string, len);
	mBuffer[len] = 0;
}


NString::NString(const NString& string)
{
	udword len = string.Length();
	mBuffer = (char*)malloc(len+1);
	memcpy(mBuffer, string.Buffer(), len);
	mBuffer[len] = 0;
}

//-----------------------------------------------------------------
// Destructors
//-----------------------------------------------------------------
NString::~NString()
{
	if (mBuffer)
	{
		free(mBuffer);
		mBuffer=null;
	}
}

void NString::SetLength(udword length)
{
	if (length==0) {
		if (mBuffer) {	free(mBuffer); mBuffer = null; }
		return;
	}

	mBuffer = (char*)realloc(mBuffer, length + 1);
	mBuffer[length] = 0;

}


NString NString::ExtractLine(udword& i)
{
	//Checking
	udword l = Length();
	if (!l || l<=i) {
		i = (udword)-1;
		return NString();
	}

	NString str;
	udword u = Find("\r\n", i);
	if (u == (udword)-1) {
		str = Mid(i, -1);
		i = -1;
	} else {
		str = Mid(i, u+2-i);
		i = (u+2==l) ? -1 : (u +2);
	}
	return str;
}


udword NString::ExtractWord(udword i, NString& strWord)
{
	// Make sure there's a word here
	i = SearchWord(i);
	if (i == (udword)-1)					return -1;

	udword f=i;
	udword l = Length();
	for (i; i<l; i++)
	{
		sbyte c = mBuffer[i];
		if ( !(c>='a' && c<='z') && !(c>='A' && c<='Z'))
		{
			strWord = Mid(f, i-f);
			return f;
		}
	}

	strWord = Mid(f);
	return f;
}

udword NString::ExtractToken(udword i, NString& strToken, char* pszSeparators)
{
	udword f=i;
	udword l = Length();
	sbyte c;

	udword sepCount = strlen(pszSeparators);
	if (sepCount==0)	return (udword)-1;

	udword j=0;
	while (j<sepCount)
	{
		while (mBuffer[i++]==pszSeparators[j] && i<l);
		j++;
		i--;
	}

	if (i>=l)	return (udword)-1;

	for (i; i<l; i++)
	{
		c = mBuffer[i];
		j=0;
		while (j<sepCount)
		{
			if (c==pszSeparators[j])
			{
				strToken = Mid(f, i-f);
				return f;
			}
			j++;
		}
	}

	strToken = Mid(f);
	return f;
}

udword NString::SearchWord(udword i)
{
	udword l = Length();

	if (!l)				return (udword)-1;										//No string, no words...
	if (l<=i)			return (udword)-1;										//Nothing at that index

	for (i; i<l; i++) {
		sbyte c = mBuffer[i];
		if ( (c>='a' && c<='z') || (c>='A' && c<='Z'))	return i;
	}

	//No words found
	return (udword)-1;
}

udword NString::Find(char c, udword index)
{
	udword l = Length();

	if (!l)				return (udword)-1;										//No string, no words...
	if (l<=index)		return (udword)-1;										//Nothing at that index

	udword	i=index;
	while (mBuffer[i]) {
		if (mBuffer[i] == c)		return i;
		i++;
	}
	return -1;
}

udword NString::ReverseFind(char c)
{
	udword	i=Length()-1;
	while (mBuffer[i]) {
		if (mBuffer[i] == c)		return i;
		i--;
	}
	return -1;
}

udword NString::Find(const char* str, udword index)
{
	udword l  = Length();
	udword sl = (udword)strlen(str);

	if (!l)				return (udword)-1;										//No string, no words...
	if (l<=index)		return (udword)-1;										//Nothing at that index
	if (!sl)			return (udword)-1;										//There's nothing to look for, it's an error

	for (index; index<l; index++)
	{
		udword i = index;

		udword si=0;
		for (; si<sl && i<l; si++)
		{
			char c = mBuffer[i++];
			if (c != str[(sdword)si])		break;
		}

		//Check if we've found the string
		if (si==sl)			return index;
	}

	//Not found
	return (udword)-1;
}

NString NString::Mid(udword first, udword count)
{
	if (count==-1)						count = Length()-first;
	if (!count)							return NString();

	if (count < 0)						count = 0;
	if ((first+count) > Length())		count = Length()-first;

	NString				str;
	str.mBuffer			= (char*)malloc(count+1);
	str.mBuffer[count]	= 0;
	memcpy(str.mBuffer, mBuffer+first, count);

	return str;
}

NString& NString::Format(const char* str, ...)
{
	//format tps string
	char buf[256];
	va_list args;
	va_start(args, str);
	vsnprintf(buf, sizeof(buf), str, args);
	va_end(args);

	//set formated string
	*this=buf;

	return *this;
}

void NString::InsertAt(udword _idx, const char* _str)
{
	if (mBuffer)
	{
		udword len1 = Length();
		udword len2 = (udword)strlen(_str);
		mBuffer = (char*)realloc(mBuffer, len1 + len2 + 1);
		memcpy(mBuffer+_idx+len2, mBuffer+_idx, len1-_idx);	//Move right part
		memcpy(mBuffer+_idx, _str, len2);
		mBuffer[len1+len2]=0;
	}

}

void NString::RemoveAt(udword _idx, udword _count)
{
	udword len1 = Length();
	if (mBuffer && _idx<len1)
	{
		memcpy(mBuffer+_idx, mBuffer+_idx+_count, len1-_count);
		mBuffer[len1-_count]=0;
	}

}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NColor class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

void NColor::ToHLS(float &_h, float &_l, float &_s)
{

  float minval = min(mR*255.0f, min(mG*255.0f, mB*255.0f));
  float maxval = max(mR*255.0f, max(mG*255.0f, mB*255.0f));
  float mdiff  = float(maxval) - float(minval);
  float msum   = float(maxval) + float(minval);

  _l = msum / 510.0f;

  if (maxval == minval)
  {
    _s = 0.0f;
    _h = 0.0f;
  }
  else
  {
    float rnorm = (maxval - mR*255.0f ) / mdiff;
    float gnorm = (maxval - mG*255.0f ) / mdiff;
    float bnorm = (maxval - mB*255.0f ) / mdiff;

    _s = (_l <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));

    if (mR*255.0f == maxval) _h = 60.0f * (6.0f + bnorm - gnorm);
    if (mG*255.0f == maxval) _h = 60.0f * (2.0f + rnorm - bnorm);
    if (mB*255.0f == maxval) _h = 60.0f * (4.0f + gnorm - rnorm);
    if (_h > 360.0f)	_h = _h - 360.0f;
  }

}

void NColor::SetFromHLS(float _h, float _l, float _s)
{
	//mA=1.0f;
  if (_s == 0.0)
  {
    mR=mG=mB=_l;
  }
  else
  {
    float rm1, rm2;

    if (_l <= 0.5f) rm2 = _l + _l * _s;
    else            rm2 = _l + _s - _l * _s;
    rm1 = 2.0f * _l - rm2;
    mR = ToRGB1(rm1, rm2, _h + 120.0f);
    mG = ToRGB1(rm1, rm2, _h);
    mB = ToRGB1(rm1, rm2, _h - 120.0f);
  }

}

float NColor::ToRGB1(float rm1, float rm2, float rh)
{
  if      (rh > 360.0f) rh -= 360.0f;
  else if (rh <   0.0f) rh += 360.0f;

  if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;
  else if (rh < 180.0f) rm1 = rm2;
  else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;

  return rm1;
}

