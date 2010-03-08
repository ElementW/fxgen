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
#ifndef _MAX_PATH
#define _MAX_PATH 32768
#endif

//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "Types.h"
#include "Templates.h"


//-----------------------------------------------------------------
// Class Prototypes
//-----------------------------------------------------------------
class NGUISubSystem;
class N2DPainter;
class NGUIFont;

class NCmdTarget;
	class NGUIWnd;
		class NFrmWnd;
		class NGUIWnd;

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
//			Defines Mouse Buttons Type
//-----------------------------------------------------------------
#define	NM_LBUTTON	1
#define	NM_MBUTTON	2
#define	NM_RBUTTON	4


//-----------------------------------------------------------------
//! \struct NWNDCREATE
//! \brief	Window Description
//-----------------------------------------------------------------
struct NWNDCREATE
{
	udword		dwStyle;
	char*			pszText;					//This window text
	udword		dwId;							//This window identifier
	NGUIWnd*			pwndParent;				//The parent window pointer if any else null
	NRect			rcRect;						//Window rect (pos relative to parent if any else to desktop)
};

namespace NKey
{
	enum Code
	{
			A = 'a',
			B = 'b',
			C = 'c',
			D = 'd',
			E = 'e',
			F = 'f',
			G = 'g',
			H = 'h',
			I = 'i',
			J = 'j',
			K = 'k',
			L = 'l',
			M = 'm',
			N = 'n',
			O = 'o',
			P = 'p',
			Q = 'q',
			R = 'r',
			S = 's',
			T = 't',
			U = 'u',
			V = 'v',
			W = 'w',
			X = 'x',
			Y = 'y',
			Z = 'z',
			Num0 = '0',
			Num1 = '1',
			Num2 = '2',
			Num3 = '3',
			Num4 = '4',
			Num5 = '5',
			Num6 = '6',
			Num7 = '7',
			Num8 = '8',
			Num9 = '9',
			Escape = 256,
			LControl,
			LShift,
			LAlt,
			LSystem,      ///< OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
			RControl,
			RShift,
			RAlt,
			RSystem,      ///< OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
			Menu,
			LBracket,     ///< [
			RBracket,     ///< ]
			SemiColon,    ///< ;
			Comma,        ///< ,
			Period,       ///< .
			Quote,        ///< '
			Slash,        ///< /
			BackSlash,
			Tilde,        ///< ~
			Equal,        ///< =
			Dash,         ///< -
			Space,
			Return,
			Back,
			Tab,
			PageUp,
			PageDown,
			End,
			Home,
			Insert,
			Delete,
			Add,          ///< +
			Subtract,     ///< -
			Multiply,     ///< *
			Divide,       ///< /
			Left,         ///< Left arrow
			Right,        ///< Right arrow
			Up,           ///< Up arrow
			Down,         ///< Down arrow
			Numpad0,
			Numpad1,
			Numpad2,
			Numpad3,
			Numpad4,
			Numpad5,
			Numpad6,
			Numpad7,
			Numpad8,
			Numpad9,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			Pause,

			Count // For internal use
	};
}
//-----------------------------------------------------------------
//!	\class	NCmdTarget
//!	\brief	Command Target
//-----------------------------------------------------------------
class CORELIB_API NCmdTarget : public NObject
{
public:
	virtual void Unused() {}
};

// Delegate function pointers
typedef void(NObject::*TDelegate)(NObject* _psender);

// Simple bindings to an object and a member function of that object.
struct  CORELIB_API FDelegate
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
//!	\class	NGUISubSystem
//!	\brief	GUI SubSystem Class Definition
//-----------------------------------------------------------------
class CORELIB_API NGUISubSystem
{
public:
	//Constructor-Destructor
	NGUISubSystem();
	virtual ~NGUISubSystem();

	//Platform Dependent Methods
	bool Init();
	bool ShutDown();
	void Update();

	void GetCursorPos(NPoint& _pos) { _pos=m_ptCursor;			}
	bool IsKeyDown(NKey::Code _key) { return m_keyDown[_key]; }

	//GUI
	void	RedrawWindow(NGUIWnd* _pwndFrom);

	void	SetFocus(NGUIWnd* _pwnd);
	NGUIWnd* GetFocus();

	void	SetCapture(NGUIWnd* _pwnd);
	void	ReleaseCapture();

	void SetMainWnd(NGUIWnd* _mainfrm)	{ m_pMainWnd = _mainfrm; }

	//Members access
	NGUIWnd*	GetMainWnd()	{ return m_pMainWnd;	}
	NGUIFont* GetFont()			{ return m_pfont;			}

	//Methods
	void ProcessMsgs_OnSize(udword _w, udword _h);

	void ProcessMsgs_MouseButtonDown(NPoint _ptScreen, udword _dwButton);
	void ProcessMsgs_MouseButtonUp(NPoint _ptScreen, udword _dwButton);
	void ProcessMsgs_MouseButtonDblClick(NPoint _ptScreen, udword _dwButton);
	void ProcessMsgs_MouseWheel(NPoint _ptScreen, sdword _dwDelta);
	void ProcessMsgs_MouseMove(NPoint _ptScreen);

	void ProcessMsgs_KeyDown(NKey::Code key);
	void ProcessMsgs_KeyUp(NKey::Code key);

	void ProcessMsgs_Text(udword _unicode);

	void NotifyWindowDeletion(NGUIWnd* _pWnd);

  //Colors
  NColor GetBarColor()    { return 0xFF58717A; }	//0xFF686868
  NColor GetFaceColor()   { return 0xFF727272; }
  NColor GetDarkColor()   { return 0xFF474747; }
  NColor GetLightColor()  { return 0xFF8b8b8b; }
  NColor GetMenuColor()   { return 0x80c8c8c8; }

protected:

	NGUIWnd* GetWndUnderMouse(sdword _x, sdword _y);
	NGUIWnd* _GetWndUnderMouse(sdword _x, sdword _y, NGUIWnd* _pwndParent);
	NGUIWnd* _GetPopupWndUnderMouse(sdword _x, sdword _y, NGUIWnd* _pwndParent);
	void	_RedrawWindowChild(NGUIWnd* _pwndFrom);
	void	_RedrawWindowPopup(NGUIWnd* _pwndFrom);

	void InitGLState();

	//Datas
	NGUIWnd*			m_pMainWnd;
	NGUIFont*	m_pfont;

	NGUIWnd*			m_pFocusedWnd;
	NGUIWnd*			m_pCapturedWnd;
	NGUIWnd*			m_pOldWndUnderMouse;
	bool			m_keyDown[NKey::Count];
	bool			m_bMustDrawWindows;
	NPoint		m_ptCursor;
};

extern CORELIB_API	NGUISubSystem*	GetGUISubSystem();


//-----------------------------------------------------------------
//!	\class	N2DPainter
//!	\brief	Graphics 2D
//-----------------------------------------------------------------
class CORELIB_API N2DPainter
{
public:
	//Constructor-Destructor
	N2DPainter(NGUIWnd* pwnd);
	~N2DPainter();

	//Rect
	void FillSolidRect(NRect& rc, NColor _clr);
	void Draw3dRect(NRect& rc, NColor clrTopLeft, NColor clrBottomRight);
	void GradientVRect(NRect& rc, NColor clrStart, NColor clrEnd);
	void RoundRect(ubyte _roundboxtype, NRect& _rc, float _rad, NColor _clr);

	//Lines
	void DrawLine(sdword _x1, sdword _y1, sdword _x2, sdword _y2, NColor _clr, udword _dwWidth);

	//Text
  sdword DrawString(const char* _pszString, NRect& _rc, udword _nFormat, NColor _clr);

	//Polygones
	void  Polygon(NPoint* _ptsArray, udword _dwCount, NColor _clr);

	//Bitmap
	//HBITMAP CreateBitmap(udword _w, udword _h, NRGBA* _pbits);
	//void Blit(HBITMAP _hbmp, udword _srcW, udword _srcH, udword _dstW, udword _dstY, udword _dstW, udword _dstH);

  //TODO
  //Draw button
  //Draw Edit
  //....

protected:
	NGUIWnd* m_pwnd;
};

//-----------------------------------------------------------------
//!	\class	NGUIWnd
//!	\brief	Windows Class Definition
//-----------------------------------------------------------------
class CORELIB_API NGUIWnd : public NCmdTarget
{
public:
	//Constructor-Destructor
					NGUIWnd();
	virtual	~NGUIWnd();

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
	NGUIWnd*	GetFocus();

	void	SetCapture();
	void	ReleaseCapture();

	void SetText(char* _pszText);
	NString GetText();

	void ShowWindow(bool _bVisible);
	bool IsWindowVisible()				{ return m_dwStyle&NWS_VISIBLE;}

	void RedrawWindow();

	NGUIWnd* GetParent()	{ return m_pParentWnd; }

	udword GetStyle()	{ return m_dwStyle; }

	udword GetDepth()	{ return m_dwDepth; }

	//Messages
	virtual	void	OnPaint()		{}
	virtual	void	OnSize()		{}

	virtual void	OnMouseMove(NPoint pos )				{}
	virtual	void	OnLButtonDown(NPoint pos )			{}
	virtual	void	OnRButtonDown(NPoint pos )			{}
	virtual	void	OnMButtonDown(NPoint pos )			{}
	virtual	void	OnLButtonUp(NPoint pos )				{}
	virtual	void	OnRButtonUp(NPoint pos )				{}
	virtual	void	OnMButtonUp(NPoint pos )				{}
	virtual	void	OnLButtonDblClk(NPoint pos)			{}
	virtual	void	OnMouseWheel(NPoint pos, sdword zDelta)			{}

	virtual	void	OnCommand(udword id)				{}
	virtual	void	OnKeyUp(udword dwchar)			{}
	virtual	void	OnKeyDown(udword dwchar)		{}
	virtual	void	OnKillFocus(NGUIWnd* pNewWnd)	{}
	virtual	void	OnTimer(udword _dwTimerID)	{}
	virtual	void	OnMouseLeave() {}
	virtual	void	OnText(udword _unicode)		{}

protected:
	//Datas
	NGUIWnd*		m_pParentWnd;
	NString	m_cstrText;
	NRect		m_rcWnd;	//Window Rect in Screen coords.
	udword	m_dwDepth;
	udword	m_dwStyle;

	NObjectArray	m_arrayChildren;

	//Friends
	friend class NGUISubSystem;
};


#endif //GUI_H
