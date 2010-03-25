//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		StandardsCtrl.h
//! \brief		Standard GUI controls
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		21-11-2007
//!
//!	\brief	This file applies the GNU GENERAL PUBLIC LICENSE
//!					Version 2, read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef STANDARDSCTRL_H
#define STANDARDSCTRL_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "GUI.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//class NGUIWnd
		class NTextCtrl;
		class NButtonCtrl;
		class NColorPickerCtrl;
		class NEditCtrl;
		class NSlideCtrl;

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NTextCtrl
//!	\brief		GUI control for text
//-----------------------------------------------------------------
class CORELIB_API NTextCtrl : public NGUIWnd
{
public:
	// Constructor-Destructor
	NTextCtrl();
	virtual ~NTextCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);

	// Methods
	void	Update();

protected:
	// Internal methods

	// Datas

protected:
	//Win32 Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnSize();
};



//-----------------------------------------------------------------
//!	\class		NButtonCtrl
//!	\brief		GUI control for button
//-----------------------------------------------------------------
#define NBUT_STYLE_PUSH		0
#define NBUT_STYLE_MENU		1

class CORELIB_API NButtonCtrl : public NGUIWnd
{
public:
	// Constructor-Destructor
	NButtonCtrl();
	virtual ~NButtonCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent, udword _dwStyle);

	// Methods
	void	Update();

	//Notification Messages
	FDelegate		OnClick;

protected:
	// Internal methods

	// Datas
	bool		m_bClicked;
	bool		m_bMouseOver;
	udword	m_dwStyle;

protected:
	//Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnSize();
	virtual void	OnMouseMove(NPoint pos );
	virtual void	OnMButtonDown(NPoint pos);
	virtual void	OnMButtonUp(NPoint pos);
	virtual void	OnLButtonUp(NPoint pos);
	virtual void	OnLButtonDown(NPoint pos);
	virtual void	OnRButtonDown(NPoint pos);
	virtual	void	OnKeyDown(udword dwchar);
	virtual	void	OnMouseLeave();

};


//-----------------------------------------------------------------
//!	\class		NColorPickerCtrl
//!	\brief		GUI control for color picker
//-----------------------------------------------------------------
class CORELIB_API NColorPickerCtrl : public NGUIWnd
{

public:
	// Constructor-Destructor
	NColorPickerCtrl();
	virtual ~NColorPickerCtrl();

	// Methods
	virtual	bool Create(NGUIWnd* parent);

	// Methods
	void	TrackPopup(NPoint _ptScreen, const NColor& _clr);

	//Members access
	NColor GetClickedColor()	{ return m_clr; }

	//Notification Messages
	FDelegate		OnColorClick;

protected:
	// Internal methods
	void GetHueAtPoint(NPoint& _pt, float& _fHue);
	void GetLSAtPoint(NPoint& _pt, float& _fL, float& _fS);
	void UpdateColorFromMousePt(NPoint& _pt);

	// Datas
	NColor	m_clr;
	bool		m_bEntered;
	NRect		m_rcHue, m_rcLS;
	float		m_fHue, m_fL, m_fS;
	bool		m_bPickedLS;
	bool		m_bPickedHue;

protected:
	//Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnMouseMove(NPoint pos );
	virtual void	OnLButtonUp(NPoint pos);
	virtual void	OnLButtonDown(NPoint pos);
	virtual	void	OnLButtonDblClk(NPoint point);
	virtual	void	OnKillFocus(NGUIWnd* pNewWnd);
	virtual	void	OnKeyUp(udword _dwchar);
};


//-----------------------------------------------------------------
//!	\class	NEditCtrl
//!	\brief	Edit Control Class Definition
//-----------------------------------------------------------------
class CORELIB_API NEditCtrl : public NGUIWnd
{
public:
	//Constructor-Destructor
	NEditCtrl();
	virtual		~NEditCtrl();

	//Windows Creation
	virtual	bool Create(const char* _pszname, const NRect& _rect, NGUIWnd* _parent);

	//Selection operations
	void			SetSel(udword _startchar, udword _endchar);
	void			GetSel(udword& _startchar, udword& _endChar) { _startchar=nmin(m_dwCursorPos, m_dwSelectionTail); _endChar=nmax(m_dwCursorPos, m_dwSelectionTail); }
	void			SelectAll();
	void			ReplaceSel(const char* _pszText);

	//Notification Messages
	FDelegate		OnEnter;
	FDelegate		OnEscape;

protected:
	//Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnLButtonUp(udword _flags, NPoint _pos);
	virtual void	OnLButtonDown(udword _flags, NPoint _pos);
	virtual	void	OnLButtonDblClk(udword _flags, NPoint _point);
	virtual	void	OnKillFocus(NGUIWnd* _pNewWnd);
	virtual	void	OnKeyDown(udword _dwchar);
	virtual	void	OnText(udword _unicode);
	//Datas
	udword m_dwCursorPos, m_dwSelectionTail;
};


//-----------------------------------------------------------------
//!	\class		NSlideCtrl
//!	\brief		GUI control for slider
//-----------------------------------------------------------------
class CORELIB_API NSlideCtrl : public NGUIWnd
{
public:
	// Constructor-Destructor
	NSlideCtrl();
	virtual ~NSlideCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);

	void	Update();

	void	SetPos(float _fPos);
	void	SetStep(float _fStep);
	void	SetRange(float _fMin, float _fMax);	//If _fMin<_fMax => Slide value ranged

protected:
	// Datas
	float m_fPos, m_fMin, m_fMax, m_fStep;

	//Delegates
	virtual	void	OnPaint();
	virtual void	OnSize();
};

#endif  //STANDARDSCTRL_H
