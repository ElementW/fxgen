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
	CopyMemory(mBuffer, string, len);
	mBuffer[len] = 0;
}


NString::NString(NString& string)
{
	udword len = string.Length();
	mBuffer = (char*)malloc(len+1);
	CopyMemory(mBuffer, string.Buffer(), len);
	mBuffer[len] = 0;
}

//-----------------------------------------------------------------
// Destructors
//-----------------------------------------------------------------
NString::~NString()
{
	if (mBuffer)	free(mBuffer);
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
	CopyMemory(str.mBuffer, mBuffer+first, count);

	return str;
}

NString& NString::Format(const char* str, ...)
{
	//format tps string
	char buf[256];
	va_list args;
	va_start(args, str);
	_vsnprintf_s(buf, sizeof(buf), sizeof(buf), str, args);
	va_end(args);

	//set formated string
	*this=buf;

	return *this;
}
