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


//-----------------------------------------------------------------
//!	\class		NEnterNameDlg
//!	\brief		DialogBox with a text box
//-----------------------------------------------------------------
class GUI_API NCurveExtentsDlg : public NDialog
{
public:
	NCurveExtentsDlg(UINT _nResID, NWnd* _parent);

	//Methods
	void	SetName(char* _pszName)		{ m_cstrName = _pszName;			}
	virtual const char* GetName()									{ return m_cstrName.Buffer(); }

	float *GetExtents() { return vars; }
	float GetMinX(){ return vars[0]; }
	float GetMaxX(){ return vars[1]; }
	float GetMinY(){ return vars[2]; }
	float GetMaxY(){ return vars[3]; }

	void SetMinX(float v){ vars[0] = v; }
	void SetMaxX(float v){ vars[1] = v; }
	void SetMinY(float v){ vars[2] = v; }
	void SetMaxY(float v){ vars[3] = v; }

	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	void OnAddPoint();
	void OnSetColor();

	void OnSave();
	void OnOpen();

public:
	//Datas
	NEditCtrl m_editCtrl1;
	NEditCtrl m_editCtrl2;
	NEditCtrl m_editCtrl3;
	NEditCtrl m_editCtrl4;

	float vars[4];

	NString	m_cstrName;
	UINT m_nResID;
	NWnd* m_parent;

		//Datas
	LPCTSTR					m_lpszResName;
	LPCDLGTEMPLATE	m_lpTemplate;
	bool						m_bIsModal;
};

//-----------------------------------------------------------------
//!	\class		NEnterNameDlg
//!	\brief		DialogBox with a text box
//-----------------------------------------------------------------
class GUI_API NAddPointConfirmDlg : public NDialog
{
public:
	NAddPointConfirmDlg(UINT _nResID, NWnd* _parent);

	//Methods
	void	SetName(char* _pszName)		{ m_cstrName = _pszName;			}
	virtual const char* GetName()									{ return m_cstrName.Buffer(); }

	float *GetVars() { return vars; }
	float GetValX(){ return vars[0]; }
	float GetValY(){ return vars[1]; }


	void SetValues( float X, float Y );


	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	void OnAddPoint();
	void OnSetColor();

	void OnSave();
	void OnOpen();

public:
	//Datas
	NEditCtrl m_editCtrl1;
	NEditCtrl m_editCtrl2;

	float vars[2];

	NString	m_cstrName;
	UINT m_nResID;
	NWnd* m_parent;

		//Datas
	LPCTSTR					m_lpszResName;
	LPCDLGTEMPLATE	m_lpTemplate;
	bool						m_bIsModal;
};


//-----------------------------------------------------------------
//!	\class		NEnterNameDlg
//!	\brief		DialogBox with a text box
//-----------------------------------------------------------------
class GUI_API NAddGradientPointConfirmDlg : public NDialog
{
public:
	NAddGradientPointConfirmDlg(UINT _nResID, NWnd* _parent);

	//Methods
	void	SetName(char* _pszName)		{ m_cstrName = _pszName;			}
	virtual const char* GetName()									{ return m_cstrName.Buffer(); }





	void SetValues( float X );


	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	void OnAddPoint();
	void OnSetColor();

	void OnSave();
	void OnOpen();

public:
	//Datas
	NEditCtrl m_editCtrl1;
	NEditCtrl m_editCtrl2;

	float vars;

	NString	m_cstrName;
	UINT m_nResID;
	NWnd* m_parent;

		//Datas
	LPCTSTR					m_lpszResName;
	LPCDLGTEMPLATE	m_lpTemplate;
	bool						m_bIsModal;
};


//-----------------------------------------------------------------
//!	\class		NEnterNameDlg
//!	\brief		DialogBox with a text box
//-----------------------------------------------------------------
class GUI_API NNumPointsDlg : public NDialog
{
public:
	NNumPointsDlg(UINT _nResID, NWnd* _parent);

	//Methods
	void	SetName(char* _pszName)		{ m_cstrName = _pszName;			}
	virtual const char* GetName()									{ return m_cstrName.Buffer(); }

	void SetValues( int X );

	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

protected:

	void OnAddPoint();
	void OnSetColor();

	void OnSave();
	void OnOpen();

public:
	//Datas
	NEditCtrl m_editCtrl1;

	int vars;

	NString	m_cstrName;
	UINT m_nResID;
	NWnd* m_parent;

		//Datas
	LPCTSTR					m_lpszResName;
	LPCDLGTEMPLATE	m_lpTemplate;
	bool						m_bIsModal;
};



//-----------------------------------------------------------------
//!	\class		NEnterNameDlg
//!	\brief		DialogBox with a text box
//-----------------------------------------------------------------
class GUI_API NGradientDlg : public NDialog
{
public:
	NGradientDlg(UINT _nResID, NWnd* _parent);

	//Methods
	void	SetName(char* _pszName)		{ m_cstrName = _pszName;			}
	virtual const char* GetName()									{ return m_cstrName.Buffer(); }


	



	void GetData( NArray<GradientElem> *elems )
	{
		for( int i = 0; i < m_elems.Count(); i++ )
			(*elems).AddItem( m_elems[i] );
	}

	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	void OnAddPoint();
	void OnSetColor();

	void OnSave();
	void OnOpen();

	void OnDeletePoint();
	void OnSetPointPos();

	void SetGradData();


public:
	//Datas
	NEditCtrl m_editCtrl;
	NTabCtrl m_tabCtrl;

	NArray<GradientElem> m_elems;
	NGradientCtrl m_gradientCtrl;


	NString	m_cstrName;
	UINT m_nResID;
	NWnd* m_parent;

		//Datas
	LPCTSTR					m_lpszResName;
	LPCDLGTEMPLATE	m_lpTemplate;
	bool						m_bIsModal;
};



//-----------------------------------------------------------------
//!	\class		NEnterNameDlg
//!	\brief		DialogBox with a text box
//-----------------------------------------------------------------
class GUI_API NCurveDlg : public NDialog
{
public:
	NCurveDlg(UINT _nResID, NWnd* _parent);

	//Methods
	void	SetName(char* _pszName)		{ m_cstrName = _pszName;			}
	virtual const char* GetName()									{ return m_cstrName.Buffer(); }



	void SetData( NArray<NDPoint> *elems )
	{ 
		
		for( int i = 0; i < (*elems).Count(); i++ )
			m_curveElemArray.AddItem( (*elems)[i] );
	}

	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	void OnAddPoint();
	void OnSetColor();

	void OnSave();
	void OnOpen();

	void OnDeletePoint();
	void OnSetPointPos();

public:
	//Datas
	NEditCtrl m_editCtrl1;
	NEditCtrl m_editCtrl2;
	NEditCtrl m_editCtrl3;
	NEditCtrl m_editCtrl4;
	NTabCtrl m_tabCtrl;
	NArray<NDPoint> m_curveElemArray;
	NCurveCtrl m_curveCtrl;
	
	NString	m_cstrName;
	UINT m_nResID;
	NWnd* m_parent;

		//Datas
	LPCTSTR					m_lpszResName;
	LPCDLGTEMPLATE	m_lpTemplate;
	bool						m_bIsModal;
};





//-----------------------------------------------------------------
//!	\class		NEnterNameDlg
//!	\brief		DialogBox with a text box
//-----------------------------------------------------------------
class GUI_API NTerraceDlg : public NDialog
{
public:
	NTerraceDlg(UINT _nResID, NWnd* _parent);

	//Methods
	void	SetName(char* _pszName)		{ m_cstrName = _pszName;			}
	virtual const char* GetName()									{ return m_cstrName.Buffer(); }



	void SetData( NArray<NDPoint> *elems )
	{ 
		
		for( int i = 0; i < (*elems).Count(); i++ )
			m_curveElemArray.AddItem( (*elems)[i] );
	}

	//Messages
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	void OnAddPoint();
	void OnSetColor();

	void OnSave();
	void OnOpen();

	void OnDeletePoint();
	void OnSetPointPos();

	void OnCreatePoints();

public:
	//Datas
	NEditCtrl m_editCtrl1;
	NEditCtrl m_editCtrl2;
	NEditCtrl m_editCtrl3;
	NEditCtrl m_editCtrl4;
	NTabCtrl m_tabCtrl;
	NArray<NDPoint> m_curveElemArray;
	NTerraceCtrl m_terraceCtrl;
	
	NString	m_cstrName;
	UINT m_nResID;
	NWnd* m_parent;

		//Datas
	LPCTSTR					m_lpszResName;
	LPCDLGTEMPLATE	m_lpTemplate;
	bool						m_bIsModal;
};




