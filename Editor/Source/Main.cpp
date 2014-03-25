//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Main.h
//! \brief	Main entry for application
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
#include <stdio.h>

#include "EditorApp.h"

//NDebugMemoryMgr gMemMgr;

//-----------------------------------------------------------------
//!\func	Export WinMain to force linkage to this module
//-----------------------------------------------------------------
int main(int argc, char *argv[])
{

	//Serialization
/*	NFileStream *stream = new NFileStream();
	stream->Open("c:\\temp\\test.ar", true);

	NArchive ar(stream);
	ar.PrepareSave();
	ar.PutClass(&class1);
	ar.FinalizeSave();

	stream->Close();


	//Deserialization
	NFileStream *streamr = new NFileStream();
	streamr->Open("c:\\temp\\test.ar", false);

	NArchive arr(streamr);
	arr.Read();
	NObject* pobj = arr.GetClass();
	

	//###TEST### Fin
	*/

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
