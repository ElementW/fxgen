//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Dialog.cpp
//! \brief	Dialog
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

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Dialog.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NDialog class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------
NDialog::NDialog(char* _lpszResName, NWnd* _parent)
	: m_lpszResName(_lpszResName), m_lpTemplate(NULL), m_bIsModal(TRUE)
{
	m_pParentWnd = _parent;
}

NDialog::NDialog(UINT nResID, NWnd* _parent)
	: m_lpszResName(MAKEINTRESOURCE (nResID)), m_lpTemplate(NULL), m_bIsModal(TRUE)
{
	m_pParentWnd = _parent;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NDialog::~NDialog()
{
	if (m_W32HWnd != NULL)
		::DestroyWindow(m_W32HWnd);
}


INT_PTR NDialog::DoModal()
{
	//Create a modal dialog
	//A modal dialog box must be closed by the user before the application continues
	if (IsWindow(m_W32HWnd))		return 0;

	m_bIsModal=TRUE;

	//Create hook
	//SetHook()
	//HHOOK hHook = ::SetWindowsHookEx(WH_CBT, NDialog::StaticCBTProc, 0, ::GetCurrentThreadId());

	//Create a modal dialog
	INT_PTR nResult;
	nResult = ::DialogBoxParam(GetModuleHandle (null), m_lpszResName, m_pParentWnd->m_W32HWnd, NDialog::StaticDialogProc, (LPARAM)this);

	//RemoveHook();

	m_W32HWnd = NULL;

	return nResult;
}


HWND NDialog::DoModeless()
{
	//Modeless dialog
	if (IsWindow(m_W32HWnd))	return 0;

	m_bIsModal=FALSE;

	//Create a modeless dialog
	HWND hwnd;
	hwnd = ::CreateDialogParam(GetModuleHandle(null), m_lpszResName, m_pParentWnd->m_W32HWnd, NDialog::StaticDialogProc, (LPARAM)this);

	m_W32HWnd = hwnd;

	return hwnd;
}

BOOL NDialog::OnInitDialog()
{
	//Center the dialog
	//POINT CenterPt = Center(hWnd);
	//::SetWindowPos(hWnd, HWND_TOP, CenterPt.x, CenterPt.y, 0, 0,  SWP_NOSIZE);

	//##TODO### subclass controls

	return TRUE;
}

void NDialog::OnOK()
{
	//Override to customize OnOK behaviour
	EndDialog(IDOK);
}

void NDialog::OnCancel()
{
	//Override to customize OnCancel behaviour
	EndDialog(IDCANCEL);
}

void NDialog::EndDialog(INT_PTR nResult)
{
	if (::IsWindow(m_W32HWnd))
	{
		if (m_bIsModal)
			::EndDialog(m_W32HWnd, nResult);
		else
			::DestroyWindow(m_W32HWnd);
	}
	m_W32HWnd = NULL;
}

POINT NDialog::Center()
{
	//Determine the position of the top left corner which would
	// center the dialog on its parent window.
	RECT rcDialog;
	RECT rcParent;
	RECT rcDesktop;
	POINT CenterPos;

	//Get screen dimensions excluding task bar
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);
	int iWidth = rcDesktop.right;
	int iHeight = rcDesktop.bottom;

	//Get the dialog dimensions
	::GetWindowRect(m_W32HWnd, &rcDialog);

	//Get the parent window dimensions (parent could be the desktop)
	HWND hParent = ::GetParent(m_W32HWnd);
	if (hParent != NULL) ::GetWindowRect(hParent, &rcParent);
	else rcParent = rcDesktop;

	//Calculate point to center the dialog on the parent window
	int x = rcParent.left + ((rcParent.right  - rcParent.left) - (rcDialog.right - rcDialog.left))/2;
	int y = rcParent.top  + ((rcParent.bottom - rcParent.top ) - (rcDialog.bottom - rcDialog.top))/2;

	//Keep the dialog wholly on the desktop
	if (x < 0) x = 0;
	if (y < 0) y = 0;

	if (x > iWidth - (rcDialog.right - rcDialog.left))
		x = iWidth - (rcDialog.right - rcDialog.left);

	if (y > iHeight - (rcDialog.bottom - rcDialog.top))
		y = iHeight - (rcDialog.bottom - rcDialog.top);

	CenterPos.x = x;
	CenterPos.y = y;
	return CenterPos;
}


INT_PTR CALLBACK NDialog::StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg==WM_INITDIALOG)
	{
		//Set user data in param for retrieve this class pointer
		::SetWindowLongPtr (hWnd, GWL_USERDATA, (LONG)lParam);

		//Control->WindowDefWndProc = (WNDPROC)GetWindowLongX( Control->hWnd, GWL_WNDPROC );
		//SetWindowLong( Control->hWnd, GWL_WNDPROC, (LONG)WWindow::StaticWndProc );

		//return OnInitDialog();
	}

	//Get NWnd Class pointer
	NDialog *w = (NDialog *) (LONG_PTR)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
	//if (w==null)	return ::DefDlgProc(hWnd, uMsg, wParam, lParam);

	if (w!=null)	return w->DialogProc(hWnd, uMsg, wParam, lParam);

	return FALSE;
}


BOOL NDialog::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			m_W32HWnd = hwnd;
			return OnInitDialog();
		}

		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDOK:			OnOK();			return TRUE;
				case IDCANCEL:	OnCancel();	return TRUE;
			}
		break;

	}

	return FALSE;
}
