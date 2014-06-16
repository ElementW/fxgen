//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Editor.cpp
//! \brief	FxGen Editor application
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		01-09-2008
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef EDITORAPP_H
#define EDITORAPP_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "CoreLibPkg.h"

#include "EditorGUI.h"
#include "EventsList.h"

#include "GUI_Win32.h"
#include "SDLApplication.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#ifdef __GNUC__
#define CAPTION		"FxGen Editor v0.7 alpha (gcc) - "
#else
#define CAPTION		"FxGen Editor v0.7 alpha - "
#endif
#define WIDTH			800
#define HEIGHT		600

#define PLATFORMAPP_CLASS	NSDLApplication
//#define PLATFORMAPP_CLASS	NW32Application

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NEventsMgr;

//-----------------------------------------------------------------
//!	\class		NFXGenApp
//!	\brief		Application class declaration
//-----------------------------------------------------------------
class NFxGenApp : PLATFORMAPP_CLASS
{
public:
	//Constructor-Destructor
					NFxGenApp();
	virtual	~NFxGenApp();

	//Methods
	virtual bool Init();
	virtual void Run();
	virtual bool Exit();
	virtual void Update();

	NEditorGUI* GetMainWnd()		{ return (NEditorGUI*)GetGUISubSystem()->GetMainWnd(); }

	udword MessageBox(char* _pszText, udword _dwStyle);

protected:
	//Datas
	float m_fOldTime;
};


//-----------------------------------------------------------------
//                   Externs
//-----------------------------------------------------------------
extern NEventsMgr*	g_pceventsMgr;
extern NFxGenApp*		GetApp();

#endif //EDITORAPP_H
