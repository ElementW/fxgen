#ifndef FXGENLIB_H
#define FXGENLIB_H

#include <stddef.h>

#ifdef FG_USE_NAMESPACE
 #define BEGIN_FG_NAMESPACE namespace FxGenLib {
 #define END_FG_NAMESPACE }
#else
 #define BEGIN_FG_NAMESPACE
 #define END_FG_NAMESPACE
#endif

BEGIN_FG_NAMESPACE

//-----------------------------------------------------------------
// FxGenLib version
//-----------------------------------------------------------------
#define FXGENLIB_VERSION        10000
#define FXGENLIB_VERSION_MAJOR  1
#define FXGENLIB_VERSION_MINOR  0
#define FXGENLIB_VERSION_BUILD  0
#define FXGENLIB_VERSION_STRING "1.0.0"

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

#ifdef FXGEN_EXPORT
	#define FXGEN_API __declspec(dllexport)
#elif defined FXGEN_EXPORT_DLL_LIBRARY_IMPORT
	#define FXGEN_API __declspec(dllimport)
#else // STATIC_BUILD
	#define FXGEN_API
#endif // STATIC_BUILD

//-----------------------------------------------------------------
// Class Prototypes
//-----------------------------------------------------------------
	class NErrors;
	class NEngineOp;


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

#ifdef _DEBUG
	#define TRACE	gDebugLog
	void FXGEN_API gDebugLog(const char* fmt, ... );
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
class FXGEN_API NErrors
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
extern FXGEN_API NErrors* gGetErrors();

//-----------------------------------------------------------------
//	Includes
//-----------------------------------------------------------------
//	#include "MemoryMgr.h"
//	#include "Maths.h"
//	#include "Bitmap.h"

//-----------------------------------------------------------------
//	Functions
//-----------------------------------------------------------------
FXGEN_API NEngineOp* fgCreateOpEngine();


END_FG_NAMESPACE

#endif
