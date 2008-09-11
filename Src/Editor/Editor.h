//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Editor.cpp
//! \brief	FxGen Editor application
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
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
#ifndef EDITOR_H
#define EDITOR_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "MainFrm.h"

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

//-----------------------------------------------------------------
//!	\class		NFXGenApp
//!	\brief		Application class declaration
//-----------------------------------------------------------------
class NFxGenApp
{
public:
	//Constructor-Destructor
					NFxGenApp();
	virtual	~NFxGenApp();

	//Methods
	bool Init();
	void Run();
	bool Exit();
	void Update();

	NMainFrm* GetMainWnd()		{ return (NMainFrm*)GetGUISubSystem()->GetMainWnd(); }

	udword MessageBox(char* _pszText, udword _dwStyle);

protected:
	//Datas
	sf::Window m_appWnd;
	float m_fOldTime;
};


//-----------------------------------------------------------------
//                   Externs
//-----------------------------------------------------------------
extern NEventsMgr*	g_pceventsMgr;
extern NFxGenApp*		GetApp();
#endif //EDITOR_H
