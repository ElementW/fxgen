//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EnterNameDlg.cpp
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

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "EnterNameDlg.h"


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//										NEnterNameDlg Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//		Constructor
//-----------------------------------------------------------------
NEnterNameDlg::NEnterNameDlg(UINT nResID, NWnd* _parent) : NDialog(nResID, _parent)
{
}

//-----------------------------------------------------------------
//!	\brief	Init Dialogbox
//!	\return	True if success
//-----------------------------------------------------------------
BOOL NEnterNameDlg::OnInitDialog()
{
	NDialog::OnInitDialog();

	//Create Edit control
	m_editCtrl.Create(m_cstrName.Buffer(), NRect(10,10,10+256,10+18), this, false);
	m_editCtrl.SetFocus();

	return FALSE;	//FALSE => Focus on control
}

//-----------------------------------------------------------------
//!	\brief	User click on OK button
//-----------------------------------------------------------------
void NEnterNameDlg::OnOK()
{
	NDialog::OnOK();

	//Change label
	m_cstrName = m_editCtrl.GetText();
}

//-----------------------------------------------------------------
//!	\brief	User click on cancel button
//-----------------------------------------------------------------
void NEnterNameDlg::OnCancel()
{
	NDialog::OnCancel();
}
