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

#define IDC_PRESS						103
#define IDC_SELECT						104
#define IDC_OPEN						105
#define IDC_SAVE						106
#define IDD_GRADIENT					107
#define IDD_ADDGRADIENT_POINT                   108
#define IDD_CURVE						111
#define IDD_SETCURVEEXTENTS				112
#define IDD_ADDPOINTCONFIRM				114
#define IDD_ADDGRADIENTPOINTCONFIRM		116
#define IDC_DELETE						117
#define IDC_SETPOS						118
#define IDC_CREATEPOINTS				119
#define IDD_TERRACE						121
#define IDD_NUMPOINT					122


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
	if( m_pParentWnd  )
		nResult = ::DialogBoxParam(GetModuleHandle (null), m_lpszResName, m_pParentWnd->m_W32HWnd, NDialog::StaticDialogProc, (LPARAM)this);
	else
		nResult = ::DialogBoxParam(GetModuleHandle (null), m_lpszResName, 0, NDialog::StaticDialogProc, (LPARAM)this);

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


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NCurveExtentsDlg Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NCurveExtentsDlg::NCurveExtentsDlg(UINT nResID, NWnd* _parent) : NDialog(nResID, _parent)
{
	m_nResID = nResID;
	m_parent = _parent;
}

//-----------------------------------------------------------------
//!	\brief	Init Dialogbox
//!	\return	True if success
//-----------------------------------------------------------------
BOOL NCurveExtentsDlg::OnInitDialog()
{
	NDialog::OnInitDialog();

	char str1[256];
	char str2[256];
	char str3[256];
	char str4[256];

	sprintf( str1, "%f", vars[0] );
	sprintf( str2, "%f", vars[1] );
	sprintf( str3, "%f", vars[2] );
	sprintf( str4, "%f", vars[3] );

	m_editCtrl1.Create(str1, NRect(50,20,50+50,20+20), this, false);
	m_editCtrl2.Create(str2, NRect(50,50,50+50,50+20), this, false);
	m_editCtrl3.Create(str3, NRect(50,80,50+50,80+20), this, false);
	m_editCtrl4.Create(str4, NRect(50,110,50+50,110+20), this, false);

	m_editCtrl1.SetFocus();

	return TRUE;
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NCurveExtentsDlg::OnOK()
{
	NDialog::OnOK();
	vars[0] = (float)atof(m_editCtrl1.GetText().Buffer());
	vars[1] = (float)atof(m_editCtrl2.GetText().Buffer());
	vars[2] = (float)atof(m_editCtrl3.GetText().Buffer());
	vars[3] = (float)atof(m_editCtrl4.GetText().Buffer());
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NCurveExtentsDlg::OnCancel()
{
	NDialog::OnCancel();
}


BOOL NCurveExtentsDlg::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NAddPointConfirmDlg Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NAddPointConfirmDlg::NAddPointConfirmDlg(UINT nResID, NWnd* _parent) : NDialog(nResID, _parent)
{
	m_nResID = nResID;
	m_parent = _parent;
	vars[0] = 0.0;
	vars[1] = 0.0;
}

//-----------------------------------------------------------------
//!	\brief	Init Dialogbox
//!	\return	True if success
//-----------------------------------------------------------------
BOOL NAddPointConfirmDlg::OnInitDialog()
{
	NDialog::OnInitDialog();

	char str1[256];
	char str2[256];

	sprintf( str1, "%f", vars[0] );
	sprintf( str2, "%f", vars[1] );

	m_editCtrl1.Create(str1, NRect(50,20,50+50,20+20), this, false);
	m_editCtrl2.Create(str2, NRect(50,50,50+50,50+20), this, false);

	m_editCtrl1.SetFocus();

	return TRUE;
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NAddPointConfirmDlg::OnOK()
{
	NDialog::OnOK();
	vars[0] = (float)atof(m_editCtrl1.GetText().Buffer());
	vars[1] = (float)atof(m_editCtrl2.GetText().Buffer());
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NAddPointConfirmDlg::OnCancel()
{
	NDialog::OnCancel();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NAddPointConfirmDlg::SetValues( float X, float Y )
{
	vars[0] = X;
	vars[1] = Y;
}


BOOL NAddPointConfirmDlg::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NAddGradientPointConfirmDlg Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NAddGradientPointConfirmDlg::NAddGradientPointConfirmDlg(UINT nResID, NWnd* _parent) : NDialog(nResID, _parent)
{
	m_nResID = nResID;
	m_parent = _parent;
	vars = 0.0;

}

//-----------------------------------------------------------------
//!	\brief	Init Dialogbox
//!	\return	True if success
//-----------------------------------------------------------------
BOOL NAddGradientPointConfirmDlg::OnInitDialog()
{
	NDialog::OnInitDialog();

	char str1[256];
	sprintf( str1, "%f", vars );
	m_editCtrl1.Create(str1, NRect(50,20,50+50,20+20), this, false);
	m_editCtrl1.SetFocus();

	return TRUE;
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NAddGradientPointConfirmDlg::OnOK()
{
	NDialog::OnOK();
	vars = (float)atof(m_editCtrl1.GetText().Buffer());

}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NAddGradientPointConfirmDlg::OnCancel()
{
	NDialog::OnCancel();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NAddGradientPointConfirmDlg::SetValues( float X )
{
	vars = X;
}


BOOL NAddGradientPointConfirmDlg::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NNumPointsDlg Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NNumPointsDlg::NNumPointsDlg(UINT nResID, NWnd* _parent) : NDialog(nResID, _parent)
{
	m_nResID = nResID;
	m_parent = _parent;
	vars = 2;

}

//-----------------------------------------------------------------
//!	\brief	Init Dialogbox
//!	\return	True if success
//-----------------------------------------------------------------
BOOL NNumPointsDlg::OnInitDialog()
{
	NDialog::OnInitDialog();

	char str1[256];
	sprintf( str1, "%d", vars );
	m_editCtrl1.Create(str1, NRect(70,20,70+50,20+20), this, false);
	m_editCtrl1.SetFocus();

	return TRUE;
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NNumPointsDlg::OnOK()
{
	NDialog::OnOK();
	vars = atoi(m_editCtrl1.GetText().Buffer());

}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NNumPointsDlg::OnCancel()
{
	NDialog::OnCancel();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NNumPointsDlg::SetValues( int X )
{
	vars = X;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NGradientDlg Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NGradientDlg::NGradientDlg(UINT nResID, NWnd* _parent) : NDialog(nResID, _parent)
{
	m_nResID = nResID;
	m_parent = _parent;
}

//-----------------------------------------------------------------
//!	\brief	Init Dialogbox
//!	\return	True if success
//-----------------------------------------------------------------
BOOL NGradientDlg::OnInitDialog()
{
	NDialog::OnInitDialog();

	//Create Edit control
	m_gradientCtrl.Create( m_cstrName.Buffer(), NRect(0,110,512,110+512), this );

	SetGradData();

	return TRUE;
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NGradientDlg::OnOK()
{
	NDialog::OnOK();

	m_elems.Clear();

	SetGradData();

	m_cstrName = m_editCtrl.GetText();
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NGradientDlg::SetGradData()
{
	for( int i = 0; i < m_gradientCtrl.m_gradientElems.Count(); i++ )
	{
		float currClientHeight = m_gradientCtrl.m_gradientElems[i].height;

		float f1 = m_gradientCtrl.to_graph_space( currClientHeight );

		GradientElem e2;
		e2.color[0] = m_gradientCtrl.m_gradientElems[i].color[0];
		e2.color[1] = m_gradientCtrl.m_gradientElems[i].color[1];
		e2.color[2] = m_gradientCtrl.m_gradientElems[i].color[2];
		e2.color[3] = m_gradientCtrl.m_gradientElems[i].color[3];
		e2.height = f1;
		m_elems.AddItem( e2 );
	}
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NGradientDlg::OnCancel()
{
	NDialog::OnCancel();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NGradientDlg::OnAddPoint()
{
	NAddGradientPointConfirmDlg add_point_dlg(IDD_ADDGRADIENTPOINTCONFIRM,0);

	float currClientHeight = m_gradientCtrl.GetCurrentLineX();

	add_point_dlg.SetValues(m_gradientCtrl.to_graph_space(currClientHeight));

	if( add_point_dlg.DoModal() )
	{
		m_gradientCtrl.m_clientLineX = (int)m_gradientCtrl.from_graph_space(add_point_dlg.vars);
		m_gradientCtrl.OnAddPoint();
	}
	/*else
	{
		m_gradientCtrl.OnAddPoint();
	}*/

	char buff[256];

	sprintf( buff, "LinePos: %d", m_gradientCtrl.GetCurrentLineX() );
}

//-----------------------------------------------------------------
//!	\brief	User click on Save button
//-----------------------------------------------------------------
void NGradientDlg::OnSave()
{
	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Saving Gradient...", (NWnd*)this, false);
	if (dlg.DoModal())
	{
		NString str = dlg.GetPathName();

		FILE *stream = fopen(str.Buffer(), "wb" );

		int val = m_gradientCtrl.m_gradientElems.Count();

		fwrite( (void*) &val, sizeof(int), 1, stream );

		for( int i = 0; i < m_gradientCtrl.m_gradientElems.Count(); i++ )
		{
			fwrite( (void*) &m_gradientCtrl.m_gradientElems[i].color[0], sizeof(float), 1, stream );
			fwrite( (void*) &m_gradientCtrl.m_gradientElems[i].color[1], sizeof(float), 1, stream );
			fwrite( (void*) &m_gradientCtrl.m_gradientElems[i].color[2], sizeof(float), 1, stream );
			fwrite( (void*) &m_gradientCtrl.m_gradientElems[i].color[3], sizeof(float), 1, stream );
			fwrite( (void*) &m_gradientCtrl.m_gradientElems[i].height, sizeof(float), 1, stream );
		}

		fclose(stream);
	}
}

//-----------------------------------------------------------------
//!	\brief	User click on open button
//-----------------------------------------------------------------
void NGradientDlg::OnOpen()
{
	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Opening Gradient...", (NWnd*)this, true);
	if (dlg.DoModal())
	{
		NString str = dlg.GetPathName();

		FILE *stream = fopen(str.Buffer(), "rb" );

		m_gradientCtrl.m_gradientElems.Clear();

		int numElems = 0;
		fread( (void*)&numElems, sizeof(int), 1, stream );

		for( int i = 0; i < numElems; i++ )
		{
			GradientElem e;

			fread( (void*) &e.color[0], sizeof(float), 1, stream );
			fread( (void*) &e.color[1], sizeof(float), 1, stream );
			fread( (void*) &e.color[2], sizeof(float), 1, stream );
			fread( (void*) &e.color[3], sizeof(float), 1, stream );
			fread( (void*) &e.height, sizeof(float), 1, stream );

			m_gradientCtrl.AddGradientPoint( e.height, NColor(255,e.color[0],e.color[1],e.color[2]) );
		}

		///	The following is a correction, since the gradient dialog has changed size
		float minX=4000, maxX=-4000;
		for( int i = 0; i < numElems; i++ )
		{
			if( m_gradientCtrl.m_gradientElems[i].height < minX )
				minX = m_gradientCtrl.m_gradientElems[i].height;

			if( m_gradientCtrl.m_gradientElems[i].height > maxX )
				maxX = m_gradientCtrl.m_gradientElems[i].height;
		}

		float range = maxX - minX;
		for( int i = 0; i < numElems; i++ )
		{
			float XX = m_gradientCtrl.m_gradientElems[i].height;
			float percentageX = ( XX - minX ) / ( maxX - minX );
			float nX = 2 * percentageX - 1;

			m_gradientCtrl.m_gradientElems[i].height = m_gradientCtrl.from_graph_space( nX );
		}

		fclose(stream);
	}
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NGradientDlg::OnSetColor()
{
	m_gradientCtrl.AssignColorToSelection();
}


//-----------------------------------------------------------------
//!	\brief	User click on delete point button
//-----------------------------------------------------------------
void NGradientDlg::OnDeletePoint()
{
	m_gradientCtrl.OnDeletePoint();
}

//-----------------------------------------------------------------
//!	\brief Set Point Pos
//-----------------------------------------------------------------
void NGradientDlg::OnSetPointPos()
{
	NAddGradientPointConfirmDlg add_point_dlg(IDD_ADDGRADIENTPOINTCONFIRM,0);

	float fVal = m_gradientCtrl.to_graph_space( m_gradientCtrl.selectedElem->height );

	add_point_dlg.SetValues(fVal);

	if( add_point_dlg.DoModal() )
	{
		if( fVal != 1.0 && fVal != -1.0 )
			m_gradientCtrl.selectedElem->height = (int)( m_gradientCtrl.from_graph_space(add_point_dlg.vars ));
		else
			MessageBox(0,"Cannot Modify The End Points","Error",0);
	}
}

BOOL NGradientDlg::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_gradientCtrl.Update();
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
				case IDC_PRESS:	OnAddPoint();	return TRUE;
				case IDC_SELECT: OnSetColor(); return TRUE;
				case IDC_SAVE: OnSave(); return TRUE;
				case IDC_OPEN: OnOpen(); return TRUE;
				case IDC_DELETE: OnDeletePoint(); return TRUE;
				case IDC_SETPOS: OnSetPointPos(); return TRUE;
			}
		break;
	}

	return FALSE;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NCurveDlg Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
NCurveExtentsDlg curve_extents(IDD_SETCURVEEXTENTS,0);
//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NCurveDlg::NCurveDlg(UINT nResID, NWnd* _parent) : NDialog(nResID, _parent)
{
	m_nResID = nResID;
	m_parent = _parent;


	//m_editCtrl2.Create(m_cstrName.Buffer(), NRect(20+70,20,20+70+50,20+20), this, false);
}

//-----------------------------------------------------------------
//!	\brief	Init Dialogbox
//!	\return	True if success
//-----------------------------------------------------------------
BOOL NCurveDlg::OnInitDialog()
{
	NDialog::OnInitDialog();


	m_curveCtrl.Create( m_cstrName.Buffer(), NRect(0,110,512,110+512), this );


	return TRUE;
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NCurveDlg::OnOK()
{
	NDialog::OnOK();

//	if( m_elems )
	{
		for( int i = 0; i < m_curveCtrl.m_curveElems.Count(); i++ )
		{
			m_curveElemArray.AddItem( m_curveCtrl.m_curveElems[i] );
		}
	}
	assert( m_curveCtrl.m_curveElems.Count() > 0 );

	//Change label
	m_cstrName = m_editCtrl1.GetText();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NCurveDlg::OnCancel()
{
	NDialog::OnCancel();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NCurveDlg::OnAddPoint()
{


	NAddPointConfirmDlg add_point_dlg(IDD_ADDPOINTCONFIRM,0);


	NDPoint pt = m_curveCtrl.to_graph_space( m_curveCtrl.m_currentLineX, m_curveCtrl.m_currentLineY );


	add_point_dlg.SetValues(pt.x, pt.y);

	if( add_point_dlg.DoModal() )
	{
		NDPoint pt2 = m_curveCtrl.from_graph_space( add_point_dlg.vars[0], add_point_dlg.vars[1] );
		m_curveCtrl.m_currentLineX = pt2.x;
		m_curveCtrl.m_currentLineY = pt2.y;
		m_curveCtrl.OnAddPoint();
	}
	/*else
	{
		m_curveCtrl.OnAddPoint();
	}*/
}

//-----------------------------------------------------------------
//!	\brief	User click on Save button
//-----------------------------------------------------------------
void NCurveDlg::OnSave()
{
	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Saving Curve...", (NWnd*)this, false);
	if (dlg.DoModal())
	{
		NString str = dlg.GetPathName();

		FILE *stream = fopen(str.Buffer(), "wb" );

		int val = m_curveCtrl.m_curveElems.Count();

		fwrite( (void*) &val, sizeof(int), 1, stream );

		for( int i = 0; i < m_curveCtrl.m_curveElems.Count(); i++ )
		{
			float x = (float)m_curveCtrl.m_curveElems[i].x;
			float y = (float)m_curveCtrl.m_curveElems[i].y;

			fwrite( (void*) (float*)&x, sizeof(float), 1, stream );
			fwrite( (void*) (float*)&y, sizeof(float), 1, stream );
		}

		fclose(stream);
	}
}

//-----------------------------------------------------------------
//!	\brief	User click on open button
//-----------------------------------------------------------------
void NCurveDlg::OnOpen()
{
	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Opening Curve...", (NWnd*)this, true);
	
	if (dlg.DoModal())
	{
		NString str = dlg.GetPathName();

		FILE *stream = fopen(str.Buffer(), "rb" );

		m_curveCtrl.m_curveElems.Clear();

		int numElems = 0;
		fread( (void*)&numElems, sizeof(int), 1, stream );

		for( int i = 0; i < numElems; i++ )
		{
			NDPoint p;

			float x,y;

			fread( (void*) &x, sizeof(float), 1, stream );
			fread( (void*) &y, sizeof(float), 1, stream );

			NDPoint p2 = m_curveCtrl.from_graph_space( x, y );

			m_curveCtrl.AddCurvePoint( p2.x, p2.y );
		}

		fclose(stream);
	}
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NCurveDlg::OnSetColor()
{

	//NCurveExtentsDlg (IDD_SETCURVEEXTENTS,0);
	//curve_extents.SetName( "Curve Extents...");

	curve_extents.SetMinX( m_curveCtrl.graph_space.left );
	curve_extents.SetMaxX( m_curveCtrl.graph_space.right );
	curve_extents.SetMinY( m_curveCtrl.graph_space.bottom );
	curve_extents.SetMaxY( m_curveCtrl.graph_space.top );

	if( curve_extents.DoModal() )
	{
		m_curveCtrl.SetExtents( curve_extents.vars[0],
			curve_extents.vars[1],
			curve_extents.vars[2],
			curve_extents.vars[3]);
	}
	else
	{
		m_curveCtrl.SetExtents( m_curveCtrl.graph_space.left,
								m_curveCtrl.graph_space.right,
								m_curveCtrl.graph_space.bottom,
								m_curveCtrl.graph_space.top );
	}
}

//-----------------------------------------------------------------
//!	\brief	User click on delete point button
//-----------------------------------------------------------------
void NCurveDlg::OnDeletePoint()
{
	m_curveCtrl.OnDeletePoint();
}

//-----------------------------------------------------------------
//!	\brief Set Point Pos
//-----------------------------------------------------------------
void NCurveDlg::OnSetPointPos()
{
	NAddPointConfirmDlg add_point_dlg(IDD_ADDPOINTCONFIRM,0);

	NDPoint pt = m_curveCtrl.to_graph_space( m_curveCtrl.selectedElem->x, m_curveCtrl.selectedElem->y );


	add_point_dlg.SetValues(pt.x, pt.y);

	if( add_point_dlg.DoModal() )
	{
		NDPoint pt2 = m_curveCtrl.from_graph_space( add_point_dlg.vars[0], add_point_dlg.vars[1] );
		m_curveCtrl.selectedElem->x = add_point_dlg.vars[0];
		m_curveCtrl.selectedElem->y = add_point_dlg.vars[1];
	}
}



BOOL NCurveDlg::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_curveCtrl.Update();

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
				case IDC_PRESS:	OnAddPoint();	return TRUE;
				case IDC_SELECT: OnSetColor(); return TRUE;
				case IDC_SAVE: OnSave(); return TRUE;
				case IDC_OPEN: OnOpen(); return TRUE;
				case IDC_DELETE: OnDeletePoint(); return TRUE;

				case IDC_SETPOS: OnSetPointPos(); return TRUE;
			}
		break;

	}

	return FALSE;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NTerraceDlg Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NTerraceDlg::NTerraceDlg(UINT nResID, NWnd* _parent) : NDialog(nResID, _parent)
{
	m_nResID = nResID;
	m_parent = _parent;


	//m_editCtrl2.Create(m_cstrName.Buffer(), NRect(20+70,20,20+70+50,20+20), this, false);
}

//-----------------------------------------------------------------
//!	\brief	Init Dialogbox
//!	\return	True if success
//-----------------------------------------------------------------
BOOL NTerraceDlg::OnInitDialog()
{
	NDialog::OnInitDialog();


	m_terraceCtrl.Create( m_cstrName.Buffer(), NRect(0,110,512,110+512), this );


	return TRUE;
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NTerraceDlg::OnOK()
{
	NDialog::OnOK();

//	if( m_elems )
	{
		for( int i = 0; i < m_terraceCtrl.m_curveElems.Count(); i++ )
		{
			m_curveElemArray.AddItem( m_terraceCtrl.m_curveElems[i] );
		}
	}
	assert( m_terraceCtrl.m_curveElems.Count() > 0 );

	//Change label
	m_cstrName = m_editCtrl1.GetText();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NTerraceDlg::OnCancel()
{
	NDialog::OnCancel();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NTerraceDlg::OnAddPoint()
{


	NAddPointConfirmDlg add_point_dlg(IDD_ADDPOINTCONFIRM,0);


	NDPoint pt = m_terraceCtrl.to_graph_space( m_terraceCtrl.m_currentLineX, m_terraceCtrl.m_currentLineY );


	add_point_dlg.SetValues(pt.x, pt.y);

	if( add_point_dlg.DoModal() )
	{
		NDPoint pt2 = m_terraceCtrl.from_graph_space( add_point_dlg.vars[0], add_point_dlg.vars[1] );
		m_terraceCtrl.m_currentLineX = pt2.x;
		m_terraceCtrl.m_currentLineY = pt2.y;
		m_terraceCtrl.OnAddPoint();
	}

}

//-----------------------------------------------------------------
//!	\brief	User click on Save button
//-----------------------------------------------------------------
void NTerraceDlg::OnSave()
{
	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Saving Curve...", (NWnd*)this, false);
	if (dlg.DoModal())
	{
		NString str = dlg.GetPathName();

		FILE *stream = fopen(str.Buffer(), "wb" );

		int val = m_terraceCtrl.m_curveElems.Count();

		fwrite( (void*) &val, sizeof(int), 1, stream );

		for( int i = 0; i < m_terraceCtrl.m_curveElems.Count(); i++ )
		{
			float x = (float)m_terraceCtrl.m_curveElems[i].x;
			float y = (float)m_terraceCtrl.m_curveElems[i].y;

			fwrite( (void*) (float*)&x, sizeof(float), 1, stream );
			fwrite( (void*) (float*)&y, sizeof(float), 1, stream );
		}

		fclose(stream);
	}
}

//-----------------------------------------------------------------
//!	\brief	User click on open button
//-----------------------------------------------------------------
void NTerraceDlg::OnOpen()
{
	//Save File Dialog
	NFileDialog dlg;
	dlg.Create("Opening Curve...", (NWnd*)this, true);
	if (dlg.DoModal())
	{
		NString str = dlg.GetPathName();

		FILE *stream = fopen(str.Buffer(), "rb" );

		m_terraceCtrl.m_curveElems.Clear();

		int numElems = 0;
		fread( (void*)&numElems, sizeof(int), 1, stream );

		for( int i = 0; i < numElems; i++ )
		{
			NDPoint p;

			float x,y;

			fread( (void*) &x, sizeof(float), 1, stream );
			fread( (void*) &y, sizeof(float), 1, stream );

			NDPoint p2 = m_terraceCtrl.from_graph_space( x, y );

			m_terraceCtrl.AddCurvePoint( p2.x, p2.y );
		}

		fclose(stream);
	}
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NTerraceDlg::OnSetColor()
{

//NCurveExtentsDlg (IDD_SETCURVEEXTENTS,0);
	//curve_extents.SetName( "Curve Extents...");

	curve_extents.SetMinX( m_terraceCtrl.graph_space.left );
	curve_extents.SetMaxX( m_terraceCtrl.graph_space.right );
	curve_extents.SetMinY( m_terraceCtrl.graph_space.bottom );
	curve_extents.SetMaxY( m_terraceCtrl.graph_space.top );

	if( curve_extents.DoModal() )
	{
		m_terraceCtrl.SetExtents( curve_extents.vars[0],
			curve_extents.vars[1],
			curve_extents.vars[2],
			curve_extents.vars[3]);
	}
	else
	{
		m_terraceCtrl.SetExtents( m_terraceCtrl.graph_space.left,
								m_terraceCtrl.graph_space.right,
								m_terraceCtrl.graph_space.bottom,
								m_terraceCtrl.graph_space.top );
	}
}


//-----------------------------------------------------------------
//!	\brief	User click on delete point button
//-----------------------------------------------------------------
void NTerraceDlg::OnDeletePoint()
{
	m_terraceCtrl.OnDeletePoint();
}

//-----------------------------------------------------------------
//!	\brief Set Point Pos
//-----------------------------------------------------------------
void NTerraceDlg::OnSetPointPos()
{
	NAddPointConfirmDlg add_point_dlg(IDD_ADDPOINTCONFIRM,0);

	NDPoint pt = m_terraceCtrl.to_graph_space( m_terraceCtrl.selectedElem->x, m_terraceCtrl.selectedElem->y );


	add_point_dlg.SetValues(pt.x, pt.y);

	if( add_point_dlg.DoModal() )
	{
		NDPoint pt2 = m_terraceCtrl.from_graph_space( add_point_dlg.vars[0], add_point_dlg.vars[1] );
		m_terraceCtrl.selectedElem->x = add_point_dlg.vars[0];
		m_terraceCtrl.selectedElem->y = add_point_dlg.vars[1];
	}
}

//-----------------------------------------------------------------
//!	\brief Set Point Pos
//-----------------------------------------------------------------
void NTerraceDlg::OnCreatePoints()
{
	m_terraceCtrl.m_curveElems.Clear();

	int controlPointCount = 2;

	NNumPointsDlg add_point_dlg(IDD_NUMPOINT,0);

	if( add_point_dlg.DoModal() )
	{
		controlPointCount = add_point_dlg.vars;
	}

	double terraceStep = 2.0 / ((double)controlPointCount - 1.0);
	double curValue = -1.0;
	for (int i = 0; i < (int)controlPointCount; i++) {
		NDPoint pt = m_terraceCtrl.from_graph_space(curValue,curValue);
		m_terraceCtrl.AddCurvePoint(pt.x,pt.y);
		curValue += terraceStep;
	}
}

//-----------------------------------------------------------------
//!	\brief Set Point Pos
//-----------------------------------------------------------------
BOOL NTerraceDlg::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_terraceCtrl.Update();

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
				case IDC_PRESS:	OnAddPoint();	return TRUE;
				case IDC_SELECT: OnSetColor(); return TRUE;
				case IDC_SAVE: OnSave(); return TRUE;
				case IDC_OPEN: OnOpen(); return TRUE;
				case IDC_DELETE: OnDeletePoint(); return TRUE;
				case IDC_SETPOS: OnSetPointPos(); return TRUE;
				case IDC_CREATEPOINTS: OnCreatePoints(); return TRUE;
			}
		break;

	}

	return FALSE;
}
