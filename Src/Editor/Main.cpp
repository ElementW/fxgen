//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Main.h
//! \brief	Main entry for application
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		31-08-2008
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
#include "pch.h"
//#include <iostream>

#include "Core.h"
#include "GUI.h"
#include "Editor.h"

//-----------------------------------------------------------------
//!\func	Export WinMain to force linkage to this module
//-----------------------------------------------------------------
int main()
{
	// Application
	NFxGenApp* pApp = new NFxGenApp();

	// Perform specific initializations
	if (!pApp->Init())		pApp->Exit();

	// Run Application
	pApp->Run();

	// Exit Application
	pApp->Exit();

	delete pApp;

	return EXIT_SUCCESS;
}