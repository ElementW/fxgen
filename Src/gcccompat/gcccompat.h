//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		gcccompat.h
//! \brief	GCC compatibility functions for FXGen
//!
//!	\author	Sebastian Olter (qduaty@gmail.com)
//!	\date		11-09-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

#ifndef GCCCOMPAT_WAS_HERE
#define GCCCOMPAT_WAS_HERE

#ifdef __GNUC__

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <algorithm>
using std::min;
using std::max;
#define __int64 long long int
#define __cdecl
#define __declspec(x)
typedef unsigned DWORD;
typedef DWORD COLORREF;
#define CopyMemory memcpy
#define GetTickCount clock
inline void ZeroMemory(void* dest, size_t size) { memset(dest, 0, size); }
#define TCHAR char
#define wsprintf sprintf
inline void MessageBox(void*,const char*,const char*,int){}
#define MB_OK 0
#define MB_ICONERROR 0
#define MB_APPLMODAL 0
#define MB_TOPMOST 0
#include <cmath>
#define sqrtf sqrt
#endif

#ifndef errno_t
#define errno_t int
#endif

inline errno_t strncpy_s(
   char *strDest,
   size_t numberOfElements,
   const char *strSource,
   size_t count
)
{
	strncpy(strDest, strSource, min(numberOfElements, count));
	return 0;
}

inline errno_t strcpy_s(
   char *strDestination,
   size_t numberOfElements,
   const char *strSource
)
{
	strncpy(strDestination, strSource, min(numberOfElements, strlen(strSource)));
	return 0;
}

template <size_t size>
errno_t strcpy_s(
   char *strDestination,
   size_t numberOfElements,
   const char (&strSource)[size]
)
{
	strncpy(strDestination, strSource, min(size, numberOfElements));
	return 0;
}

inline int _vsnprintf_s(
   char *buffer,
   size_t sizeOfBuffer,
   size_t count,
   const char *format,
   va_list argptr
)
{
	return vsnprintf(buffer, min(sizeOfBuffer, count), format, argptr);
}

#define __forceinline inline

#define FIGOFAGO 0
#if FIGOFAGO
#include <string>
using std::string;
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using std::min;
using std::max;

int MessageBox(string text, int type = MB_OK)
{
	return ::MessageBox(NULL, text.c_str(), "", type);
}
#endif

#else // not __GNUC__
template<class T> T log2(T t) {return static_cast<T>(log(1. * t) / log(2.)); }
#endif // __GNUC__
#endif // GCCCOMPAT_WAS_HERE
