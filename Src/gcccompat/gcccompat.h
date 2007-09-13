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
#include <windows.h>

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

#endif // __GNUC__
#endif // GCCCOMPAT_WAS_HERE
