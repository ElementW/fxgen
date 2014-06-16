//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		SDLApplication.h
//! \brief	Win32 specific implementation
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		16-06-2014
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "CoreLibPkg.h"

//-----------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------
struct SDL_Window;
struct SDL_KeyboardEvent;
//class SDL_Renderer;

//-----------------------------------------------------------------
//!	\class	NSDLApplication
//!	\brief	SDL Application Class Definition
//-----------------------------------------------------------------
class NSDLApplication
{
public:
	//Constructor-Destructor
	NSDLApplication();
	virtual	~NSDLApplication();

	//Methods
	virtual	bool				Init();			//!< Override to perform Windows instance initialization, such as creating your window objects.
	virtual	void				Run();			//!< Runs the default message loop. Override to customize the message loop.
	virtual	bool				Exit();			//!< Override to clean up when your application terminates.
	virtual void				Idle()	{}	//!< It is called by the framework when there is no message in the queue.
	virtual void				Update()	{}	//!< It is called by the framework in Run Loop
	virtual void				Present();

	virtual void AskExit();	//!< Called in order to exit application

	virtual	void GetCursorPos(NPoint& _pos);
	virtual	void SetCursorPos(const NPoint& _pos);
	virtual	udword MessageBox(char* _pszText, udword _dwStyle = NMB_OK);

protected:
	//Methods
	NKey::Code NSDLApplication::SDLKeyCodeToFxGen(SDL_KeyboardEvent _sdlkey);

	//Datas
	SDL_Window* m_pwnd;

	
	
};
