//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		GUI_Win32.h
//! \brief	Win32 specific implementation
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		23-06-2008
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
// Class Prototypes
//-----------------------------------------------------------------
	class NW32Application;
	class NW32FileDialog;

//-----------------------------------------------------------------
//!	\class	NW32Application
//!	\brief	Win32 Application Class Definition
//-----------------------------------------------------------------
class NW32Application
{
public:
	//Constructor-Destructor
	NW32Application();
	virtual	~NW32Application();

	//Methods
	virtual	bool				Init();			//!< Override to perform Windows instance initialization, such as creating your window objects.
	virtual	void				Run();			//!< Runs the default message loop. Override to customize the message loop.
	virtual	bool				Exit();			//!< Override to clean up when your application terminates.
	virtual void				Idle()	{}	//!< It is called by the framework when there is no message in the queue.
	virtual void				Update()	{}	//!< It is called by the framework in Run Loop

	virtual void AskExit();	//!< Called in order to exit application
	
	virtual	void GetCursorPos(NPoint& _pos);
	virtual	void SetCursorPos(const NPoint& _pos);
	virtual	udword MessageBox(char* _pszText, udword _dwStyle=NMB_OK);

protected:
	//Methods
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	udword W32KeyCodeToFxGen(udword _w32keycode);
	
	//Datas
	HDC m_hDC;
	HWND m_hW32Wnd;
//	NKey::Code m_keymap[NKey::Code.Count];
};


//-----------------------------------------------------------------
//!	\class	NW32FileDialog
//!	\brief	FileDialog Class Definition
//-----------------------------------------------------------------
/*class NW32FileDialog
{
public:
	//Platform Dependent Methods
	virtual bool Create(char* name, NWnd* parent, bool open=true, bool multiselect=false);
	virtual udword	DoModal();	//Displays the dialog box and allows the user to make a selection

protected:
	//W32 Datas
	OPENFILENAME	mOfn;								//Win32 OPENFILENAME struct
	bool					mOpenFileDialog;		//Open or Save Display
};
*/