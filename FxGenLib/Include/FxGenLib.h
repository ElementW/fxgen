#ifndef CORELIB_H
#define CORELIB_H

// Standart Includes
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <cstdarg>

#ifdef WIN32
#	include <io.h>
#else
#	include <dirent.h>
#endif

#ifdef __GNUC__
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include "windows.h"
#endif

// OpenGL
#include <GL/gl.h>	/* Header File For The OpenGL Library */

// FreeType Headers
/*#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/fttrigon.h"*/

//#ifndef __GNUC__
//#define NOMINMAX
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <windows.h>
//#endif


/*#ifdef CORE_USE_NAMESPACE
 #define BEGIN_CORE_NAMESPACE namespace CoreLib {
 #define END_CORE_NAMESPACE }
#else
 #define BEGIN_CORE_NAMESPACE
 #define END_CORE_NAMESPACE
#endif

BEGIN_CORE_NAMESPACE
*/

//-----------------------------------------------------------------
// Types
//-----------------------------------------------------------------

  #define null				0

	//type definitions
	typedef unsigned char			ubyte;	// Unsigned 8 bit value
	typedef signed char				sbyte;	// Signed 8 bit value
	typedef unsigned short		uword;	// Unsigned 16 bit value
	typedef signed short			sword;	// Signed 16 bit value
#ifndef _WIN32
	typedef int32_t				    sdword;	// Signed 32 bit value
	typedef u_int32_t			    udword;	// Unsigned 32 bit value
#else
	typedef unsigned long			udword;	// Unsigned 32 bit value
	typedef signed long				sdword;	// Signed 32 bit value
#endif

//-----------------------------------------------------------------
// Macros
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Class Prototypes
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define		MAX_NAMELEN					256
#define		MAX_PATHLEN					256

//#ifndef max
#define nmax(a,b)            (((a) > (b)) ? (a) : (b))
//#endif

//#ifndef min
#define nmin(a,b)            (((a) < (b)) ? (a) : (b))
//#endif

//-----------------------------------------------------------------
// Macros
//-----------------------------------------------------------------
#ifdef CORELIB_EXPORT
	#define CORELIB_API __declspec(dllexport)
#elif defined CORELIB_EXPORT_DLL_LIBRARY_IMPORT
	#define CORELIB_API __declspec(dllimport)
#else // STATIC_BUILD
	#define CORELIB_API
#endif // STATIC_BUILD

#ifdef _DEBUG
	#define TRACE	gDebugLog
	void CORELIB_API gDebugLog(const char* fmt, ... );
#else
	#define TRACE
#endif

//Error ###TODO### Error management
				//TRACE("Code:%d Msg:%s\n", errcode, msg);
//#define ERR		gGetErrors()->AddError

//-----------------------------------------------------------------
//!	\class	NErrors Core.h
//!	\brief	Errors manager
//-----------------------------------------------------------------
class CORELIB_API NErrors
{
public:
	NErrors();
	~NErrors();

	void	AddError(udword _code, const char* _fmt, ... );
	char* GetErrors();	//!< Errors are clear after this call

protected:
	udword m_dwStringSize;
	udword m_dwStringPos;
	char* m_pszErrors;
};
extern CORELIB_API NErrors* gGetErrors();

//-----------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------
  #include "../Source/MemoryMgr.h"
  #include "../Source/Maths.h"
  //#include "../Source/EventsMgr.h"
  //#include "../Source/GUI.h"
  //#include "../Source/Templates.h"
  //#include "../Source/Types.h"
//	#include "Bitmap.h"

//-----------------------------------------------------------------
//	Functions
//-----------------------------------------------------------------

//END_CORE_NAMESPACE

#endif
