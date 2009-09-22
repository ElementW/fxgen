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
#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "GUI.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//class NWControl
		class NTextCtrl;
		class NButtonCtrl;
		class NColorPickerCtrl;
		class NEditCtrl;

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NTextCtrl
//!	\brief		GUI control for text
//-----------------------------------------------------------------
class  NTextCtrl : public NWControl
{
public:
	// Constructor-Destructor
	NTextCtrl();
	virtual ~NTextCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent);

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

class  NButtonCtrl : public NWControl
{
public:
	// Constructor-Destructor
	NButtonCtrl();
	virtual ~NButtonCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent, udword _dwStyle);

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
	virtual void	OnMouseMove(udword flags, NPoint pos );
	virtual void	OnMButtonDown(udword flags, NPoint pos);
	virtual void	OnMButtonUp(udword flags, NPoint pos);
	virtual void	OnLeftButtonUp(udword flags, NPoint pos);
	virtual void	OnLeftButtonDown(udword flags, NPoint pos);
	virtual	void	OnLeftButtonDblClk(udword flags, NPoint point);
	virtual void	OnRightButtonDown(udword flags, NPoint pos);
	virtual	void	OnKeyDown(udword dwchar);
	virtual	void	OnMouseLeave();

};


//-----------------------------------------------------------------
//!	\class		NColorPickerCtrl
//!	\brief		GUI control for color picker
//-----------------------------------------------------------------
class  NColorPickerCtrl : public NWControl
{

public:
	// Constructor-Destructor
	NColorPickerCtrl();
	virtual ~NColorPickerCtrl();

	// Methods
	virtual	bool Create(NWnd* parent);

	// Methods
	void	TrackPopup(NPoint _ptScreen, const NColor& _clr);

	//Members access
	NColor GetClickedColor()	{ return m_clr; }

	//Notification Messages
	FDelegate		OnColorClick;

protected:
	// Internal methods
	bool GetHueAtPoint(NPoint& _pt, float& _fHue);
	bool GetLSAtPoint(NPoint& _pt, float& _fL, float& _fS);
	void UpdateColorFromMousePt(NPoint& _pt);

	// Datas
	NColor	m_clr;
	bool		m_bEntered;
	NRect		m_rcHue, m_rcLS;
	float		m_fHue, m_fL, m_fS;
	bool		m_bPicked;

protected:
	//Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnMouseMove(udword flags, NPoint pos );
	virtual void	OnLeftButtonUp(udword flags, NPoint pos);
	virtual void	OnLeftButtonDown(udword flags, NPoint pos);
	virtual	void	OnLeftButtonDblClk(udword flags, NPoint point);
	virtual	void	OnKillFocus(NWnd* pNewWnd);
	virtual	void	OnKeyUp(udword _dwchar);
};


//-----------------------------------------------------------------
//!	\class	NEditCtrl
//!	\brief	Edit Control Class Definition
//-----------------------------------------------------------------
class   NEditCtrl : public NWControl
{
public:
	//Constructor-Destructor
	NEditCtrl();
	virtual		~NEditCtrl();

	//Windows Creation
	virtual	bool Create(const char* _pszname, const NRect& _rect, NWnd* _parent, bool _bMultiLine);

	//General
	//void			SetText(const char* text);
	//NString		GetText();

	//Caret
	void	GetCaretPos(udword& _cx, udword& _cy)		{ _cx=m_dwCurX; _cy=m_dwCurY; }

	//Line operations
	udword		GetLineCount()	{ return m_dwLinesCount; }
	NString		GetLine(udword _line);
	//udword		GetLineIdx(udword _line);

	//Selection operations
	void			SetSel(udword _startchar, udword _endchar);
	void			GetSel(udword& _startchar, udword& _endChar) { _startchar=m_dwStartSel; _endChar=m_dwEndSel; }
	void			SelectLine(udword _line);
	void			ReplaceSel(const char* _pszText);
	//void			HideSelection(bool _hide);

protected:
	//Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnLeftButtonUp(udword _flags, NPoint _pos);
	virtual void	OnLeftButtonDown(udword _flags, NPoint _pos);
	virtual	void	OnLeftButtonDblClk(udword _flags, NPoint _point);
	virtual	void	OnKillFocus(NWnd* _pNewWnd);
	virtual	void	OnKeyDown(udword _dwchar);
	//Datas
	udword m_dwLinesCount;
	udword m_dwCurX, m_dwCurY;
	udword m_dwStartSel, m_dwEndSel;
};