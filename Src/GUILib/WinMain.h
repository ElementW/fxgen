//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		WinMain.h
//! \brief	Main entry for application
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
#include "Core.h"
#include "GUI.h"

//-----------------------------------------------------------------
//!\func	Export WinMain to force linkage to this module
//-----------------------------------------------------------------
int WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Get Application
	NW32Application* pApp = (NW32Application*)GetApp();

	// One and only file can be loaded at startup and may be enquoted
/*	if(lpCmdLine[0] == '"')
		lpCmdLine++;
	char *tp = strchr(lpCmdLine, '"');
	if(tp != NULL)
		*tp = '\0';
	pApp->lpCmdLine = lpCmdLine;*/


	// Perform specific initializations
	if (!pApp->Init())		pApp->Exit();

	// Run Application
	pApp->Run();

	// Exit Application
	pApp->Exit();

	return 0;
}
