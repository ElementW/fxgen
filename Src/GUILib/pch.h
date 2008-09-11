#ifndef PCH_H
#define PCH_H


// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>  //###JN###linuxport
#include <math.h>
//#include <assert.h>
//#include <stdarg.h>
#include <string.h>

//FxGen API
//#include "MemoryMgr.h"
#include "Core.h"

//For Win32 OpenGL
#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
  #include <windows.h>
  #include <GL/gl.h>	//Header File For The OpenGL Library
#endif


#endif //PCH_H
