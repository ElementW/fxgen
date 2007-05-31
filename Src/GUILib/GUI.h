//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		GUI.h
//! \brief	Serializer for datas loading and saving
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
#include "Types.h"
#include "Templates.h"

//-----------------------------------------------------------------
// Class Prototypes
//-----------------------------------------------------------------
class NApplication;
class NGraphics;

	class NDialog;
	class NFileDialog;
	class NMenuBar;
	class NStatusBar;

	class NCmdTarget;
		class NWnd;
			class NFrmWnd;
			class NSplitWnd;
			class NWControl;
				class NEditCtrl;
				class NTabCtrl;

//-----------------------------------------------------------------
//! \struct NWNDCREATE
//! \brief	Window Description
//-----------------------------------------------------------------
struct NWNDCREATE
{
	char*			Name;								//The window text name
	udword		Id;									//This window identifier
	NWnd*			Parent;							//The parent window pointer if any else null
	NRect			Rect;								//Window rect (pos relative to parent if any else to desktop)
	udword		W32Style;						//Windows style
	udword		W32StyleEx;					//Windows extended style
	char*			W32ClassName;				//Win32 CreateEx class name
};


//-----------------------------------------------------------------
//!	\class	NCmdTarget
//!	\brief	Command Target
//-----------------------------------------------------------------
class  GUI_API NCmdTarget : public NObject
{
public:
	virtual void Unused() {}
};

// Delegate function pointers
typedef void(NCmdTarget::*TDelegate)();

// Simple bindings to an object and a member function of that object.
struct GUI_API FDelegate
{
	NCmdTarget* TargetObject;
	void (NCmdTarget::*TargetInvoke)();

	FDelegate( NCmdTarget* InTargetObject=null, TDelegate InTargetInvoke=null )
	: TargetObject( InTargetObject )
	, TargetInvoke( InTargetInvoke )
	{}
	virtual void operator()() { if( TargetObject ) (TargetObject->*TargetInvoke)(); }
};


//-----------------------------------------------------------------
//!	\class	NApplication
//!	\brief	Application Class Definition
//-----------------------------------------------------------------
class GUI_API NApplication
{
public:
	//Constructor-Destructor
	NApplication();
	virtual						~NApplication();

	//Methods
	virtual	bool				Init();			//!< Override to perform Windows instance initialization, such as creating your window objects.
	virtual	void				Run();			//!< Runs the default message loop. Override to customize the message loop.
	virtual	bool				Exit();			//!< Override to clean up when your application terminates.
	virtual void				Idle()	{}	//!< It is called by the framework when there is no message in the queue.

	void AskExit();	//!< Called in order to exit application

	//Members access
	NWnd*	GetMainWnd()			{ return m_pMainWnd; }

	//Helpers
	udword MessageBox(char* _pszText, uword dwType=MB_OK);

protected:
	//Datas
	NWnd*				m_pMainWnd;
	HMODULE			m_hRichEditMod;
};

extern	GUI_API	NApplication*			GetApp();


//-----------------------------------------------------------------
//!	\class	NGraphics
//!	\brief	Graphics 2D
//-----------------------------------------------------------------
class GUI_API NGraphics
{
public:
	//Constructor-Destructor
	NGraphics(NWnd* pwnd);
	~NGraphics();

	//Attributes
	void SetPen(sdword width, COLORREF clr);
	void SetBrush(COLORREF clr);

	//Rect
	void FillSolidRect(NRect& rc, COLORREF clr);
	void Draw3dRect(NRect& rc, COLORREF clrTopLeft, COLORREF clrBottomRight);
	void RoundRect(NRect& rc, sdword ellipseW, sdword ellipseH);
	void GradientVRect(NRect& rc, COLORREF clrStart, COLORREF clrEnd);
	void RoundRect(int _roundboxtype, float _minx, float _miny, float _maxx, float _maxy, float _rad);

	//Lines
	void MoveTo(sdword x, sdword y);
	void LineTo(sdword x, sdword y);

	//Text
	void SetFont(HFONT hfont);
	void SetBkMode(udword dwMode);
	void SetTextColor(COLORREF clr);
	void SetTextBkColor(COLORREF clr);
	sdword DrawText(char* szString, NRect& rc, udword nFormat);

	//Polygones
	void  Polygon(NPoint* ptsArray, udword dwCount);

	//Bitmap
	//HBITMAP CreateBitmap(udword _w, udword _h, RGBA* _pbits);
	//void Blit(HBITMAP _hbmp, udword _srcW, udword _srcH, udword _dstW, udword _dstY, udword _dstW, udword _dstH);

protected:
	HDC		m_hDC, m_hMemDC;
	HWND	m_hWnd;
	HPEN	m_hPen;
	HBRUSH	m_hBrush;
	NRect	m_rc;
	HBITMAP	m_hBmp;
};


//-----------------------------------------------------------------
//!	\class	NWnd
//!	\brief	Windows Class Definition
//-----------------------------------------------------------------
class GUI_API NWnd : public NCmdTarget
{
public:
	//Constructor-Destructor
					NWnd();
	virtual	~NWnd();

	//Windows Creation
	virtual	bool Create(NWNDCREATE& c);

	//Methods
	NRect	GetClientRect();
	NRect	GetWindowRect();
	void	ScreenToClient(NPoint& _pt);
	void	ClientToScreen(NPoint& _pt);

	void	SetWindowRect(NRect& r);
	void	SetFocus();
	void	SetCapture();
	void	ReleaseCapture();

	void SetWindowText(char* _pszText);

	//Datas
	HWND m_W32HWnd;

protected:
	//Messages
	virtual	void	OnPaint()		{}
	virtual	void	OnSize()		{}
	virtual void	OnMouseMove(udword flags, NPoint pos )				{}
	virtual	void	OnLeftButtonDown(udword flags, NPoint pos )		{}
	virtual	void	OnRightButtonDown(udword flags, NPoint pos )	{}
	virtual	void	OnMButtonDown(udword flags, NPoint pos )			{}
	virtual	void	OnLeftButtonUp(udword flags, NPoint pos )			{}
	virtual	void	OnRightButtonUp(udword flags, NPoint pos )		{}
	virtual	void	OnMButtonUp(udword flags, NPoint pos )				{}
	virtual	void	OnLeftButtonDblClk(udword flags, NPoint point){}
	virtual	void	OnMouseWheel(udword flags, sword zDelta, NPoint point)			{}
	virtual	void	OnCommand(udword id)			{}
	virtual	void	OnKeyDown(udword dwchar)	{}
	virtual	void	OnKeyUp(udword dwchar)		{}
	virtual	void	OnKillFocus()							{}

	virtual	udword	CatchControlCommand(uword notifycode, uword id)	{ return 0; }
	virtual udword	CatchControlNotify(udword code)									{ return 0; }

	//Datas
	WNDPROC	m_OldWndProc;
	NWnd*		m_pParentWnd;
	NArray<NWControl*>	mControlsList;
	NString	cstrWindowName;

	//Windows Proc
	static	LRESULT CALLBACK StaticWndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam);
	virtual	LRESULT	WndProc( UINT msg, WPARAM wparam, LPARAM lparam);

	//Friends
	friend class NApplication;
};


//-----------------------------------------------------------------
//!	\class	NFrmWnd
//!	\brief	Frame Windows Class Definition
//-----------------------------------------------------------------
class  GUI_API NFrmWnd : public NWnd
{
public:
	//Constructor-Destructor
					NFrmWnd();
	virtual	~NFrmWnd();

	//Creation
	virtual	bool Create(char* name, NRect& rect);

	//Methods
	void ReplaceWorkspace(NWnd* wnd);

	//Members access
	NWnd*				GetWorkspace()		{ return mWorkspace;		}
	NMenuBar*		GetMenuBar()			{ return mMenuBar;			}
	NStatusBar*	GetStatusBar()		{ return mStatusBar;		}

protected:
	//Win32 Messages Dispatching
	virtual void	OnSize();

	//Datas
	NWnd*				mWorkspace;
	NMenuBar*		mMenuBar;
	NStatusBar*	mStatusBar;
};


//-----------------------------------------------------------------
//!	\class	NWControl
//!	\brief	Control Class Definition
//-----------------------------------------------------------------
class  GUI_API NWControl : public NWnd
{
public:
	//Constructor-Destructor
	NWControl() : NWnd()		{}
	virtual	~NWControl()		{}

	//Windows Creation
	virtual	bool	Create(NWNDCREATE& c);
};


//-----------------------------------------------------------------
//!	\class	NEditCtrl
//!	\brief	Edit Control Class Definition
//-----------------------------------------------------------------
class  GUI_API NEditCtrl : public NWControl
{
public:
	//Constructor-Destructor
	NEditCtrl() : NWControl()		{}
	virtual		~NEditCtrl()			{}

	//Windows Creation
	virtual	bool	Create(char* name, NRect& rect, NWnd* parent, bool bMultiLine);

	//General
	void			SetText(char* text);
	NString		GetText();

	//Caret
	void			GetCaretPos(udword& cx, udword& cy);

	//Line operations
	udword		GetLineCount();
	NString		GetLine(udword line);
	udword		GetLineIdx(udword line);

	//Selection operations
	void			SetSel(udword startchar, udword eNGraphicshar);
	void			GetSel(udword& startchar, udword& eNGraphicshar);
	void			SelectLine(udword line);
	void			ReplaceSel(char* str);
	void			HideSelection(bool hide);

	//Formatting operations
	void			SetSelCharFormat(NColor& col, bool bold, bool italic, bool underline);

protected:
	virtual	udword	CatchControlCommand(uword notifycode, uword id);
	virtual udword	CatchControlNotify(udword code);
};


//-----------------------------------------------------------------
//!	\class	NTabCtrl
//!	\brief	Tab Control Class Definition
//-----------------------------------------------------------------
class GUI_API NTabCtrl : public NWControl
{
public:
	//Constructor-Destructor
					NTabCtrl() : NWControl()	{}
	virtual	~NTabCtrl()								{}

	//Windows Creation
	virtual	bool Create(char* name, NRect& rect, NWnd* parent);

	//Methods
	void		InsertItem(udword pos, char* name, NWnd* wnd);	//pos from 0 to ...
	void		SelectItem(udword idx);	//idx from 0 to ...
	udword	GetSelectedItem();

	//Notification Messages
	FDelegate		OnSelChange;

protected:
	//Win32 messages
	virtual	void		OnSize();
	virtual udword	CatchControlNotify(udword code);
};


//-----------------------------------------------------------------
//!	\class	NMenuBar
//!	\brief	Menu Bar Class Definition
//-----------------------------------------------------------------
class GUI_API NMenuBar
{
public:
	//Constructor-Destructor
						NMenuBar();
	virtual		~NMenuBar();

	//Menu Creation
	virtual	bool Create(NWnd* owner);

	//Methods
	void		InsertItem		(udword parent, udword pos, char* name, udword id);
	udword	InsertPopupItem	(udword parent, udword pos, char* name);

	void		InsertFromResource(char* name);

	void		RemoveItem(udword pos);	//##TOADD## add parent
protected:
	//Methods
	void		_InsertFromResource(HMENU submenu, char* name, HMENU parent);

	//Datas
	HMENU		mW32Menu;
	NWnd*		mOwner;
};


//-----------------------------------------------------------------
//!	\class	NStatusBar
//!	\brief	Status Bar Class Definition
//-----------------------------------------------------------------
class GUI_API NStatusBar : public NWnd
{
public:
	//Constructor-Destructor
								NStatusBar();
	virtual						~NStatusBar();

	//Menu Creation
	virtual	bool				Create(NWnd* parent);

	//Methods
			udword				GetHeight()				{ return 24; }
};


//-----------------------------------------------------------------
//!	\class	NFileDialog
//!	\brief	FileDialog Class Definition
//-----------------------------------------------------------------
class GUI_API NFileDialog
{
public:
	//Constructor/Destructor
					NFileDialog()		{}
	virtual	~NFileDialog()		{}

	//Initialization
	virtual bool Create(char* name, NWnd* parent, bool open=true, bool multiselect=false);

	//Methods
	virtual udword	DoModal();														//Displays the dialog box and allows the user to make a selection
	NString					GetPathName(udword Index=0)		const;	//The path of the filename includes the file's title plus the entire directory path

protected:
	//Methods
	udword GetFileNumber();

	//Datas
	char		mFileName[_MAX_PATH];			//Contains full path name after filedialog return
	NString	mTransfilter;							//File filter (ex. "TLM Files (*.tlm)|*.tlm|All Files (*.*)|*.*||")

	//W32 Datas
	OPENFILENAME	mOfn;								//Win32 OPENFILENAME struct
	bool					mOpenFileDialog;		//Open or Save Display
};


//-----------------------------------------------------------------
//!	\class	NColorDialog
//!	\brief	ColorDialog Class Definition
//-----------------------------------------------------------------
class GUI_API NColorDialog
{
public:
	//Constructor/Destructor
					NColorDialog();
	virtual	~NColorDialog();

	//Initialization
	virtual bool Create(char* name, NWnd* parent, NColor& _color);

	//Operations
	virtual udword DoModal();		//Displays the dialog box and allows the user to make a selection
																		//If the user clicks the OK button of the dialog box, the return value is nonzero
	NColor	GetColor()								{ return m_Color;}
	void		SetColor(NColor& _color);

protected:
	//Datas
	NColor	m_Color;	//!< Current Color

	//W32 Datas
	CHOOSECOLOR		m_Cc;			//W32 Color Dialog Struct
	COLORREF			m_acrCustClr[16]; // array of custom colors
};
