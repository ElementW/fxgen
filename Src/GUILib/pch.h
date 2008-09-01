#ifndef PCH_H
#define PCH_H


// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
//#include <tchar.h>  //###JN###linuxport
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

//FxGen API
#include "Core.h"

//OpenGL
  #define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
  // Windows Header Files:
  #include <windows.h>
#include <GL/gl.h>	//Header File For The OpenGL Library

#endif //PCH_H
