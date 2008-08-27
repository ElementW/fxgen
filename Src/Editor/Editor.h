//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Editor.cpp
//! \brief	FxGen Editor application
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
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
#define CAPTION		"FxGen Editor v0.6 alpha (gcc) - "
#else
#define CAPTION		"FxGen Editor v0.6 alpha - "
#endif
#define WIDTH			800
#define HEIGHT		600

//-----------------------------------------------------------------
//!	\class		NFXGenApp
//!	\brief		Application class declaration
//-----------------------------------------------------------------
class NFxGenApp : public NApplication
{
public:
	//Constructor-Destructor
					NFxGenApp();
	virtual	~NFxGenApp();

	//Methods
	virtual	bool Init();		//!< Override to perform Windows instance initialization, such as creating your window objects.
	virtual	bool Exit();		//!< Override to clean up when your application terminates.
	virtual void Idle();		//!< It is called by the framework when there is no message in the queue.
	virtual void Update();	//!< It is called by the framework in Run Loop

protected:
	//Datas
	float m_fOldTime;
};


//-----------------------------------------------------------------
//                   Externs
//-----------------------------------------------------------------
extern NEventsMgr*	g_pceventsMgr;

#endif //EDITOR_H