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
#define __forceinline inline

#ifndef errno_t
#define errno_t int
#endif
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef min
template<typename T1, typename T2>inline T1 min(T1 t1, T2 t2) { return t1 < t2 ? t1 : t2; }
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

#include <stdarg.h>

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


#ifndef _WIN32
#define __cdecl
#define __declspec(x)
typedef u_int16_t DWORD;
typedef DWORD COLORREF;
#else
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>
#include <richedit.h>
#endif

#endif

#endif // GCCCOMPAT_WAS_HERE
