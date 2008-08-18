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
#ifndef GUI_H
#define GUI_H

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


#ifndef _MAX_PATH
#define _MAX_PATH 32768
#endif

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Types.h"
#include "Templates.h"
#include "Keyboard.h"

#include <GL/gl.h>	//Header File For The OpenGL Library


//-----------------------------------------------------------------
// Class Prototypes
//-----------------------------------------------------------------
class NbaseApplication;
class NGraphics;
class NGUIFont;

	class NDialog;
	class NbaseFileDialog;

	class NCmdTarget;
		class NWnd;
			class NFrmWnd;
			class NSplitWnd;
			class NWControl;
				class NTabCtrl;

//-----------------------------------------------------------------
//			Defines Window Styles
//-----------------------------------------------------------------
#define	NWS_VISIBLE	1
#define	NWS_POPUP		2

//-----------------------------------------------------------------
//			Defines MessagBox Styles and Return
//-----------------------------------------------------------------
#define	NMB_OK			1
#define	NMB_YESNO		2

#define	NIDOK			1
#define	NIDYES		2
#define	NIDNO			3

//-----------------------------------------------------------------
//			Defines DrawText Format
//-----------------------------------------------------------------
#define NDT_HCENTER					1
#define NDT_VCENTER					2
#define NDT_END_ELLIPSIS		4
#define NDT_SINGLELINE			8

//-----------------------------------------------------------------
//! \struct NWNDCREATE
//! \brief	Window Description
//-----------------------------------------------------------------
struct NWNDCREATE
{
	udword		dwStyle;
	char*			pszText;					//This window text
	udword		dwId;							//This window identifier
	NWnd*			pwndParent;				//The parent window pointer if any else null
	NRect			rcRect;						//Window rect (pos relative to parent if any else to desktop)
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
typedef void(NObject::*TDelegate)(NObject* _psender);

// Simple bindings to an object and a member function of that object.
struct GUI_API FDelegate
{
	NObject* TargetObject;
	void (NObject::*TargetInvoke)(NObject* _psender);

	FDelegate( NObject* InTargetObject=null, TDelegate InTargetInvoke=null )
	: TargetObject( InTargetObject )
	, TargetInvoke( InTargetInvoke )
	{}

	virtual void operator()(NObject* _psender) { if( TargetObject ) (TargetObject->*TargetInvoke)(_psender); }
};


//-----------------------------------------------------------------
//!	\class	NbaseApplication
//!	\brief	Base Application Class Definition
//-----------------------------------------------------------------
class GUI_API NbaseApplication
{
public:
	//Constructor-Destructor
	NbaseApplication();
	virtual						~NbaseApplication();

	//Platform Dependent Methods
	virtual	bool	Init() { return false; }		//!< Override to perform Windows instance initialization, such as creating your window objects.
	virtual	void	Run();				//!< OverLoad Runs the default message loop. Override to customize the message loop.
	virtual	bool	Exit();				//!< OverLoad to clean up when your application terminates.
	virtual void	Idle()		{}	//!< OverRide It is called by the framework when there is no message in the queue.
	virtual void	Update()	{}	//!< OverRide It is called by the framework in Run Loop

	virtual void	AskExit() {} 	//!< Called in order to exit application

	virtual	void GetCursorPos(NPoint& _pos) {}; //Override
	virtual	void SetCursorPos(const NPoint& _pos) {}; //Override
	virtual	udword MessageBox(char* _pszText, udword _dwStyle=NMB_OK) { return 0; }

	//GUI
	void	RedrawWindow(NWnd* _pwndFrom);

	void	SetFocus(NWnd* _pwnd);
	NWnd* GetFocus();

	void	SetCapture(NWnd* _pwnd);
	void	ReleaseCapture();

	//Members access
	NWnd*	GetMainWnd()			{ return m_pMainWnd;	}
	NGUIFont* GetFont()			{ return m_pfont;			}


//protected:
	//Methods
	void ProcessMsgs_OnSize(udword _w, udword _h);	//!< Process Msgs (mouse, resize, paint ...)
	NWnd* GetWndUnderMouse(sdword _x, sdword _y);
	NWnd* _GetWndUnderMouse(sdword _x, sdword _y, NWnd* _pwndParent);
	NWnd* _GetPopupWndUnderMouse(sdword _x, sdword _y, NWnd* _pwndParent);
	void InitGLState();
	void	_RedrawWindowChild(NWnd* _pwndFrom);
	void	_RedrawWindowPopup(NWnd* _pwndFrom);
	//Datas
	NWnd*			m_pMainWnd;
	NString		m_strCmdLine;
	NGUIFont*	m_pfont;

	NWnd*			m_pFocusedWnd;
	NWnd*			m_pCapturedWnd;
	NWnd*			m_pOldWndUnderMouse;

	bool	m_bMustDrawWindows;
};

extern	GUI_API	NbaseApplication*			GetApp();


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

	//Rect
	void FillSolidRect(NRect& rc, NColor clr);
	void Draw3dRect(NRect& rc, NColor clrTopLeft, NColor clrBottomRight);
	void GradientVRect(NRect& rc, NColor clrStart, NColor clrEnd);
	void RoundRect(ubyte _roundboxtype, NRect& _rc, float _rad, NColor _clr);

	//Lines
	void DrawLine(sdword _x1, sdword _y1, sdword _x2, sdword _y2, NColor _clr, udword _dwWidth);

	//Text
	sdword DrawText(const char* _pszString, NRect& _rc, udword _nFormat, NColor _clr);

	//Polygones
	void  Polygon(NPoint* _ptsArray, udword _dwCount, NColor _clr);

	//Bitmap
	//HBITMAP CreateBitmap(udword _w, udword _h, NRGBA* _pbits);
	//void Blit(HBITMAP _hbmp, udword _srcW, udword _srcH, udword _dstW, udword _dstY, udword _dstW, udword _dstH);

protected:
	NWnd* m_pwnd;
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
	NRect	GetClientRect();	//!<
	NRect	GetWindowRect();	//!< Get Rect relative to parent
	void	ScreenToClient(NPoint& _pt);
	void	ClientToScreen(NPoint& _pt);

	void	SetWindowRect(const NRect& r);	//!< Set Rect relative to parent

	NRect	GetScreenRect() { return m_rcWnd; }	//!< Get Rect in relative to main window (screen space)

	void	SetFocus();
	NWnd*	GetFocus();

	void	SetCapture();
	void	ReleaseCapture();

	void SetText(char* _pszText);
	NString GetText();

	void ShowWindow(bool _bVisible);
	bool IsWindowVisible()				{ return m_dwStyle&NWS_VISIBLE;}

	void RedrawWindow();

	NWnd* GetParent()	{ return m_pParentWnd; }

	udword GetStyle()	{ return m_dwStyle; }

	udword GetDepth()	{ return m_dwDepth; }

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
	virtual	void	OnKeyUp(udword dwchar)		{}
	virtual	void	OnKeyDown(udword dwchar)		{}
	virtual	void	OnKillFocus(NWnd* pNewWnd)		{}
	virtual	void	OnTimer(udword _dwTimerID)		{}
	virtual	void	OnMouseLeave() {}

protected:
	//Datas
	NWnd*		m_pParentWnd;
	NString	m_cstrText;
	NRect		m_rcWnd;	//Window Rect in Screen coords.
	udword	m_dwDepth;
	udword	m_dwStyle;

	NObjectArray	m_arrayChildren;

	//Friends
	friend class NbaseApplication;
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
	virtual	bool Create(char* name, const NRect& rect);

	//Methods
	void ReplaceWorkspace(NWnd* wnd);

	//Members access
	NWnd*				GetWorkspace()		{ return mWorkspace;		}

	//Messages
	virtual void	OnSize();

protected:
	//Datas
	NWnd*				mWorkspace;
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
//!	\class	NTabCtrl
//!	\brief	Tab Control Class Definition
//-----------------------------------------------------------------
/*
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
	//virtual udword	CatchControlNotify(udword code);
};
*/

//-----------------------------------------------------------------
//!	\class	NbaseFileDialog
//!	\brief	FileDialog Class Definition
//-----------------------------------------------------------------
class GUI_API NbaseFileDialog
{
public:
	//Constructor/Destructor
					NbaseFileDialog()		{}
	virtual	~NbaseFileDialog()		{}

	//Platform Dependent Methods
	virtual bool Create(char* name, NWnd* parent, bool open=true, bool multiselect=false)	= 0;
	virtual udword	DoModal() = 0;												//Displays the dialog box and allows the user to make a selection

	//Methods
	udword	GetFileNumber();
	NString	GetPathName(udword Index=0)		const;	//The path of the filename includes the file's title plus the entire directory path

protected:
	//Methods

	//Datas
	char		mFileName[_MAX_PATH];			//Contains full path name after filedialog return
	NString	mTransfilter;							//File filter (ex. "TLM Files (*.tlm)|*.tlm|All Files (*.*)|*.*||")
};

#endif //GUI_H
