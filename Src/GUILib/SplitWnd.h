//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Splitwnd.h
//! \brief	Split window
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
#pragma	once

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
#include "Core.h"
#include "GUI.h"

//-----------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------
class NWnd;
	class NSplitWnd;

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define		SPLITPANE_DEFAULT_ID	1
#define		SPLITBORDER_SIZE			4
#define		SPLITMINPANE_SIZE			32
#define		CAPTIONPANE_SIZE			18

#define		SIDE_TOP				0x01
#define		SIDE_BOTTOM			0x02
#define		SIDE_LEFT				0x04
#define		SIDE_RIGHT			0x08

//-----------------------------------------------------------------
//! \struct NPANEINFO
//! \brief	Pane Informations
//-----------------------------------------------------------------
struct  NPANEINFO
{
	NPANEINFO()			{ wnd = null; }
	udword			id;										//!< Pane identifier
	NRect				rect;									//!< Pane Rectangle
	NWnd*				wnd;									//!< nwnd attached to this pane
	udword			side;									//!< Reserved
	NString			strCaption;						//!< Pane Caption
};

//-----------------------------------------------------------------
//! \struct NSPLITBOX
//! \brief	Splitter Box Informations
//-----------------------------------------------------------------
struct  NSPLITBOX
{
	udword					type;							//!< (0 = horizontal, 1 = vertical)
	NRect						rect;
	NArray<udword>	move;							//!< (plane*, side)
};


//-----------------------------------------------------------------
//!	\class	NSplitWnd
//!	\brief	Splitter window Class Definition
//-----------------------------------------------------------------
class	GUI_API NSplitWnd : public NWnd
{
public:
	//Constructor, destructor
					NSplitWnd();
	virtual	~NSplitWnd();

	//Windows Creation
	virtual	bool	Create(char* name, NRect& rect, NWnd* parent);

	//Methods
	udword	SplitColumn	(udword paneid, sdword dwPosInPercent);			//split the frame in 2 Columns	(return the new right pane identifier)  pos is the location where the split occurs.
	udword	SplitRow		(udword paneid, sdword dwPosInPercent);			//split the frame in 2 Rows		(return the new bottom pane identifier) pos is the location where the split occurs.

	NPANEINFO*	GetPaneInfo	(NPoint& pos);							//Return pane info at the client coordinates (x,y)
	NPANEINFO*	GetPaneInfo	(udword id);							//Return pane info from an identifier (return null if not found)

	bool	SetPaneWnd	(char* _pszCaption, udword id, NWnd* wnd);					//attach a window to a pane
	bool	DestroyPane (udword id, udword sidetomerge);		//Destroy a pane (SPLITPANE_DEFAULT_ID can't be destroying)

protected:

	//Helpers
	udword			UpdateCursor(NPoint& mpos);
	NSPLITBOX*	CreateSplitterBox(NPANEINFO* pane, udword side);
	void				DestroySplitterBox(NSPLITBOX* splitbox);
	void				GetHitPanes(NPANEINFO* pane, udword side, NArray<NPANEINFO*> &array);
	bool				MoveSplit(NSPLITBOX* splitbox, NPoint &deltas);

	//Win32 Messages Dispatching
	virtual	void	OnPaint();
	virtual void	OnSize();
	virtual void	OnMouseMove(udword flags, NPoint pos);
	virtual void	OnLeftButtonUp(udword flags, NPoint pos);
	virtual void	OnLeftButtonDown(udword flags, NPoint pos);

	//Datas
	udword							mCurPaneId;
	NArray<NPANEINFO*>	mPaneList;
	float								mFactorX, mFactorY;
	NSPLITBOX*					mSplitBox;
	NPoint							mLastMousePos;
	HFONT								m_hfontNormal;

	//Windows Proc
	virtual	LRESULT	WndProc( UINT msg, WPARAM wparam, LPARAM lparam);
};
