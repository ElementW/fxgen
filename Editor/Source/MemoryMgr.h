//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MemoryMgr.h
//! \brief	Memory manager for FxGen
//!
//!	\author	Johann Nadalutti (fxgen@free.fr) (based on nvidia sdk)
//!	\date		03-09-2008
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef MEMORY_H
#define MEMORY_H


//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Core.h"
#include <new>

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------
#ifndef STATIC_BUILD
#ifdef FXGEN_EXPORTS
	#define FXGEN_API __declspec(dllexport)
#else
	#define FXGEN_API __declspec(dllimport)
#endif
#else // STATIC_BUILD
#define FXGEN_API
#endif // STATIC_BUILD


#ifdef _DEBUG

	//-----------------------------------------------------------------
	//!	\class		NDebugMemoryMgr
	//!	\brief		Memory manager for debug
	//-----------------------------------------------------------------
	class FXGEN_API NDebugMemoryMgr
	{
	public:
		//Constructor-Destructor
		NDebugMemoryMgr()	{
    		mUsedMem = 0;	mFirstBlk=mLastBlk=null; mPeakMem = 0;
    		}
		virtual	~NDebugMemoryMgr()	{	DumpAllocs();	}

		//Methods
		void* Malloc(udword size, const char* file, udword line);
		void  Free(void* mem);
		//void* Realloc(void* mem, udword size, const char* file, udword line);
		void  DumpAllocs();


	protected:
		//Struct
		struct MemBlk
		{
			udword			size;
			const char*	file;
			udword			line;
			MemBlk     *pPrevBlk, *pNextBlk;
		};

		//Datas
		MemBlk    *mFirstBlk, *mLastBlk;
		udword	  mUsedMem;
		udword	  mPeakMem;
	};


	extern FXGEN_API NDebugMemoryMgr gMemMgr;

	//#define NEW(x)        new(((asALLOCFUNCDEBUG_t)(userAlloc))(sizeof(x), __FILE__, __LINE__)) x
	//#define DELETE(ptr,x) {void *tmp = ptr; (ptr)->~x(); userFree(tmp);}

	#define NNEW(x)						new(gMemMgr.Malloc(sizeof(x),__FILE__,__LINE__)) x
	#define NDELETE(ptr,x)		{ void *tmp = ptr; (ptr)->~x(); gMemMgr.Free(tmp); }

	#define NNEWARRAY(x,cnt)  (x*)gMemMgr.Malloc(sizeof(x)*cnt,__FILE__,__LINE__)
	#define NDELETEARRAY(ptr) gMemMgr.Free(ptr)

//-----------------------------------------------------------------
//	Memory System without Debug
//-----------------------------------------------------------------
#else

	#define NNEW(x)						new x
	#define NDELETE(ptr,x)		delete ptr

	#define NNEWARRAY(x,cnt)  new x[cnt]
	#define NDELETEARRAY(ptr) delete[] ptr

#endif


//-----------------------------------------------------------------
//	Memory Copy, Fill
//-----------------------------------------------------------------
#define NMemFill(_ptr, _val, _len)  memset(_ptr, _val, _len)
#define NMemCopy(_ptrD, _ptrS, _len)  memcpy(_ptrD, _ptrS, _len)
#define NMemMove(_ptrD, _ptrS, _len)  memmove(_ptrD, _ptrS, _len)

#endif //MEMORY_H
