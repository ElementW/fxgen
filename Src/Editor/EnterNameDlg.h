//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		EnterNameDlg.h
//! \brief	DialogBox with a text box
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
#pragma once

//-----------------------------------------------------------------
//!	\class		NEnterNameDlg
//!	\brief		DialogBox with a text box
//-----------------------------------------------------------------
class NEnterNameDlg : public NDialog
{
public:
	NEnterNameDlg(UINT _nResID, NWnd* _parent);

	//Methods
	void	SetName(char* _pszName)		{ m_cstrName = _pszName;			}
	char* GetName()									{ return m_cstrName.Buffer(); }

protected:
	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	//Datas
	NEditCtrl m_editCtrl;
	NString	m_cstrName;
};
