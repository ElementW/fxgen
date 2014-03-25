//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		WinMain.h
//! \brief	Main entry for application
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
/*int WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Get Application
	NW32Application* pApp = (NW32Application*)GetApp();

	// Perform specific initializations
	if (!pApp->Init())		pApp->Exit();

	// Run Application
	pApp->Run();

	// Exit Application
	pApp->Exit();

	return 0;
}
*/
