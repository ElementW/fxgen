//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		FileChooserDialog.h
//! \brief	
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		29-08-2008
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
#include "FileChooserDialog.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					Win32 File Chooser Dialog Class Implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//-----------------------------------------------------------------
// Name:	Create()
// Desc:	...
//-----------------------------------------------------------------
bool NFileChooserDialog::Create(char* name, NWnd* parent, bool open, bool multiselect)
{
	//Open or Save Display
	m_stOpenFileDialog = open;

	//Win32 Open Dialog Box Structure
	NMemFill( &m_stOfn, 0, sizeof(OPENFILENAME));

	m_stOfn.lStructSize		= sizeof(OPENFILENAME);
	m_stOfn.hInstance			= GetModuleHandle (null);
	m_stOfn.hwndOwner = null;

	//To Do...
	NString Filename	= "";
	NString DefExt		= "";
	NString Filter		= "All Files (*.*)|*.*||";
	NString InitialDir	= "";

	//Filters
		//Translate string filter into OPENFILENAME format (lots of \0)
	sdword idx;
	if (Filter.Length() )
	{
		m_strTransfilter = Filter;
		while ((idx = m_strTransfilter.ReverseFind('|')) != -1)		m_strTransfilter.Buffer()[idx]=0;		//Change '|' with '\0'
	}
	//Fill Struct
	m_stOfn.lpstrFilter				= (LPCTSTR)m_strTransfilter.Buffer();
	m_stOfn.lpstrCustomFilter	= NULL;
	m_stOfn.nMaxCustFilter			= 0;
	m_stOfn.nFilterIndex				= 0;

	//Default File Name edit control (return)
	// setup initial file name
	ZeroMemory(m_szFileName, sizeof(m_szFileName));
	if (Filename.Length())	lstrcpyn((LPTSTR)m_szFileName, (char*)Filename.Buffer(), sizeof(m_szFileName));
	//Fill Struct
	m_stOfn.lpstrFile			= (LPTSTR)m_szFileName;
	m_stOfn.nMaxFile				= sizeof(m_szFileName);

	//Selected File Name (return)
	m_stOfn.lpstrFileTitle		= NULL;
	m_stOfn.nMaxFileTitle		= 0;

	//Default File Directory
   m_stOfn.lpstrInitialDir	= null;	//(LPCTSTR)InitialDir.Buffer();

	//Dialog Title Name
	m_stOfn.lpstrTitle			= name;

	//Offset
	m_stOfn.nFileOffset		= 0;
  m_stOfn.nFileExtension	= 0;

	//Default extension
	m_stOfn.lpstrDefExt		= null;	//(LPCTSTR)DefExt.Buffer();

	//Flags
  m_stOfn.Flags					= OFN_EXPLORER;
	if (multiselect)		m_stOfn.Flags|=OFN_ALLOWMULTISELECT;

	//...
  m_stOfn.lCustData			= 0;
	m_stOfn.lpfnHook				= NULL;
  m_stOfn.lpTemplateName	= NULL;

	return true;
}


//-----------------------------------------------------------------
// Name:	DoModal()
// Desc:	...
//-----------------------------------------------------------------
udword NFileChooserDialog::DoModal()
{
	udword Result;

	if	(m_stOpenFileDialog)			{

		//If the Win32 method returns 0, it means that the opening failed, so DoModal has to returns 0 too
		if (::GetOpenFileName(&m_stOfn))		{
			//if the Win32 method returns 1, the opening worked well, an then, DoModal returns the number of opened files
			return GetFileNumber();
		}	else	return 0;
	}
	else	Result = ::GetSaveFileName(&m_stOfn);	//Save Dialog

	return Result;
}

//-----------------------------------------------------------------
// Name:	GetFileNumber()
// Desc:	...
//-----------------------------------------------------------------
udword NFileChooserDialog::GetFileNumber()
{
	udword	ret = 1, i = 0;

	while (i < _MAX_PATH)		{
		while (m_szFileName[i])		i++;

		if (!m_szFileName[i+1])		return ret == 1 ? ret : ret - 1;
		else						ret++;

		i++;
	}

	return ret;
}


//-----------------------------------------------------------------
// Name:	GetPathName()
// Desc:	...
//-----------------------------------------------------------------
NString NFileChooserDialog::GetPathName(udword Index) const
{
	//Get File Path
	NString path = m_szFileName;

	//If User Select only one file
	if (m_szFileName[path.Length()+1]==0 || m_szFileName[path.Length()+2]==0)	return path;

	//Get FileName
	udword i=0, lpos=0;

	//If More than 1 file selected
	while (m_szFileName[i] | m_szFileName[i+1] ) {
		if (m_szFileName[i] == 0)	{
			if (lpos==Index)									return path+"\\"+(m_szFileName+i+1);
			lpos++;
		}
		i++;
	}

	//Return an Empty String if no pathname found at the given index
	return NString("");
}



#endif	//WIN32