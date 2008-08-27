#ifndef PCH_H
#define PCH_H

#ifdef WIN32

  // Modify the following defines if you have to target a platform prior to the ones specified below.
  // Refer to MSDN for the latest info on corresponding values for different platforms.
  #ifndef WINVER				// Allow use of features specific to Windows XP or later.
  #define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
  #endif

  #ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
  #define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
  #endif

  #ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
  #define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
  #endif

  #ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
  #define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
  #endif

  #define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
  // Windows Header Files:
  #include <windows.h>
  #include <windowsx.h>
  #include <commctrl.h>
  #include <commdlg.h>
  #include <richedit.h>

#endif

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "Core.h"

#include "GUI.h"
#include "SplitWnd.h"
#include "MenuCtrl.h"
#include "EventsMgr.h"
#include "EventsList.h"
#include "StandardsCtrl.h"

#include "EngineOp.h"

#include "MainOps.h"
#include "FilterOps.h"
#include "DistordOps.h"
#include "RenderOps.h"
#include "CombineOps.h"

//Win32
#ifdef WIN32
  #include "GUI_Win32.h"
  #define NApplication	NW32Application
  #define NFileDialog		NW32FileDialog
  #include "WinMain.h"

//Linux
#else
  #include "GUI_Linux.h"
  #define NApplication	NLinuxApplication
  #define NFileDialog		NLinuxFileDialog

#endif



#endif //PCH_H
