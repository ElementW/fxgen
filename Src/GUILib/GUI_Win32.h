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
#ifndef GUIW32_H
#define GUIW32_H

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------
#ifndef STATIC_BUILD
#ifdef GUI_EXPORTS
	#define GUI_API __declspec(dllexport)
#else
	#define GUI_API __declspec(dllimport)
#endif
#else // STATIC_BUILD
#define FXGEN_API
#endif // STATIC_BUILD


//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "GUI.h"

//#include "Types.h"
//#include "Templates.h"

//-----------------------------------------------------------------
// Class Prototypes
//-----------------------------------------------------------------
class NbaseApplication;
	class NW32Application;

class NbaseFileDialog;
	class NW32FileDialog;

//-----------------------------------------------------------------
//!	\class	NW32Application
//!	\brief	Win32 Application Class Definition
//-----------------------------------------------------------------
	class GUI_API NW32Application : public NbaseApplication
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
	virtual void CaptureMouse();
	virtual void ReleaseMouse();
	virtual	udword MessageBox(char* _pszText, udword _dwStyle=NMB_OK);

protected:
	//Methods
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	udword W32KeyCodeToFxGen(udword _w32keycode);

	//Datas
	HDC m_hDC;
	HWND m_hW32Wnd;
	NKey m_keymap[NK_LAST];
};


//-----------------------------------------------------------------
//!	\class	NW32FileDialog
//!	\brief	FileDialog Class Definition
//-----------------------------------------------------------------
class GUI_API NW32FileDialog : public NbaseFileDialog
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


	//W32 Datas
	//CHOOSECOLOR		m_Cc;			//W32 Color Dialog Struct
	//NColor			m_acrCustClr[16]; // array of custom colors

#endif //GUIW32_H
