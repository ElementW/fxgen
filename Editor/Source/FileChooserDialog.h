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
#ifndef FILECHOOSER_H
#define FILECHOOSER_H

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class	NFileChooserDialog
//!	\brief	FileDialog Class Definition
//-----------------------------------------------------------------
/*
#ifdef WIN32
	class NFileChooserDialog
	{
	public:
    virtual  ~NFileChooserDialog();
		//Platform Dependent Methods
		virtual bool Create(char* name, NGUIWnd* parent, bool open=true, bool multiselect=false);
		virtual udword	DoModal();	//Displays the dialog box and allows the user to make a selection

		//Methods
		udword	GetFileNumber();
		NString	GetPathName(udword Index=0)		const;	//The path of the filename includes the file's title plus the entire directory path

	protected:
		//W32 Datas
		OPENFILENAME	m_stOfn;								//Win32 OPENFILENAME struct
		bool					m_stOpenFileDialog;		//Open or Save Display

		//Datas
		char		m_szFileName[_MAX_PATH];			//Contains full path name after filedialog return
		NString	m_strTransfilter;							//File filter (ex. "TLM Files (*.tlm)|*.tlm|All Files (*.*)|*.*||")
	};

#endif	//WIN32

#ifdef LINUX
	class NFileChooserDialog
	{
	public:
		//Platform Dependent Methods
		virtual bool Create(char* name, NGUIWnd* parent, bool open=true, bool multiselect=false);
		virtual udword	DoModal();	//Displays the dialog box and allows the user to make a selection

		//Methods
		udword	GetFileNumber();
		NString	GetPathName(udword Index=0)		const;	//The path of the filename includes the file's title plus the entire directory path

	protected:
		bool					m_stOpenFileDialog;		//Open or Save Display

		//Datas
		char		m_szFileName[_MAX_PATH];			//Contains full path name after filedialog return
		NString	m_strTransfilter;							//File filter (ex. "TLM Files (*.tlm)|*.tlm|All Files (*.*)|*.*||")
	};

#endif
*/
#endif //FILECHOOSER_H
