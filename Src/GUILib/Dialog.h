//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Dialog.h
//! \brief	Dialog Box
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
#pragma once

//-----------------------------------------------------------------
//                   Macros
//-----------------------------------------------------------------
#ifdef GUI_EXPORTS
	#define GUI_API __declspec(dllexport)
#else
	#define GUI_API __declspec(dllimport)
#endif

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Core.h"
#include "GUI.h"

//-----------------------------------------------------------------
//!	\class	NDialog
//!	\brief	Dialog Box
//-----------------------------------------------------------------
class GUI_API NDialog : public NWnd
{
public:
	NDialog(UINT _nResID, NWnd* _parent);
	NDialog(char* _pszResName, NWnd* _parent);

	virtual ~NDialog();

	//Creation Methods
	virtual HWND		DoModeless();
	virtual INT_PTR DoModal();

	//Helpers
	POINT Center();	//!< Center DialogBox

protected:
	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	//Methods
	void EndDialog(INT_PTR nResult);

	//Datas
	LPCTSTR					m_lpszResName;
	LPCDLGTEMPLATE	m_lpTemplate;
	bool						m_bIsModal;

	//Dialog Proc
	static INT_PTR CALLBACK StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
