//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Layout.cpp
//! \brief	GUI Layout
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		23-10-2009
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
#include "../Include/CoreLib.h"
#include "GUI.h"
#include "Layout.h"


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//					NGUILayout class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------
NGUILayout::NGUILayout() : NGUIWnd()
{
	mCurPaneId		= SPLITPANE_DEFAULT_ID;
	mLastMousePos	= NPoint(0,0);
	mSplitBox			= null;
}

//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
NGUILayout::~NGUILayout()
{
	//Free Panes Infos Dynamics structs
	for (sdword i=mPaneList.Count()-1; i>=0; i--)
	{
		if (mPaneList[i]->pstripBar)
			NDELETE(mPaneList[i]->pstripBar, NStripBarCtrl);
		if (mPaneList[i]->wnd)
			NDELETE(mPaneList[i]->wnd, NGUIWnd);

		NDELETE(mPaneList[i], NPANEINFO);
	}
}

bool NGUILayout::Create(const char* name, const NRect& rect, NGUIWnd* parent)
{
	//Call Base class
	NWNDCREATE			wc;
	wc.dwId					= 1;
	wc.pszText			= const_cast<char*>(name);
	wc.pwndParent		= parent;
	wc.rcRect				= rect;
	wc.dwStyle			= NWS_VISIBLE;
	NGUIWnd::Create(wc);

	//Create the first pane
	NPANEINFO* paneinfo = NNEW(NPANEINFO);
	paneinfo->id		= mCurPaneId++;
	paneinfo->wnd		= null;
	paneinfo->rect		= GetClientRect();
	paneinfo->rect.right+=SPLITBORDER_SIZE;
	paneinfo->rect.bottom+=SPLITBORDER_SIZE;
	paneinfo->pstripBar = NNEW(NStripBarCtrl);
	paneinfo->pstripBar->Create(name, paneinfo->rect, this);
	mPaneList.AddItem(paneinfo);

	//Setup some datas
	mFactorY = mFactorX = 1.0f;

	return true;
}

/*LRESULT NGUILayout::WndProc( UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg==WM_ERASEBKGND)		return 1;

	//Else Call Base Class
	else return NGUIWnd::WndProc(msg, wparam, lparam);
}*/


void NGUILayout::OnPaint()
{
	sdword i;

	NRect rc = GetClientRect();
	//TRACE("%s %dx%d-%dx%d %d\n", m_cstrText, m_rcWnd.left, m_rcWnd.top, m_rcWnd.right, m_rcWnd.bottom, m_dwStyle);

	N2DPainter gfx(this);
	gfx.FillSolidRect(rc, RGBA(90,90,90,255));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Move All the Panes and Draw Panes Caption
	for (i=mPaneList.Count()-1; i>=0; i--)
	{
		NPANEINFO* cpane = mPaneList[i];

		//Calc the Pane Rect
		NRect rect	= cpane->rect;
		rect.left		=(udword)((float)rect.left	* mFactorX);
		rect.right	=(udword)((float)rect.right	* mFactorX) - SPLITBORDER_SIZE;
		rect.top		=(udword)((float)rect.top		* mFactorY) + CAPTIONPANE_SIZE;
		rect.bottom	=(udword)((float)rect.bottom* mFactorY) - SPLITBORDER_SIZE;
		//::Rectangle(hdc, rect.left+SPLITBORDER_SIZE, rect.top+SPLITBORDER_SIZE, rect.right-SPLITBORDER_SIZE, rect.bottom-SPLITBORDER_SIZE);
		//gfx.FillSolidRect(rect, RGBA(255, 255,0,0));	//AARRGGBB

		//Move window	//###GUI###
		//if (cpane->wnd)		::MoveWindow(cpane->wnd->m_W32HWnd, rect.left, rect.top, rect.Width(), rect.Height(), true);
		if (cpane->wnd)			cpane->wnd->SetWindowRect(rect);

		//Display Caption
		rect.top-=CAPTIONPANE_SIZE;
		rect.bottom=rect.top+CAPTIONPANE_SIZE;
		//gfx.GradientVRect(rect, RGBA(160,153,147), RGBA(131,126,120) );
		//gfx.Draw3dRect(rect, RGBA(255,255,255,255), RGBA(255,0,0,0));

		//gfx.SetBrush(RGBA(80,80,80));
		//gfx.RoundRect(12, rect.left, rect.top, rect.right, rect.bottom, 16);

		/*if (cpane->strCaption.Length())
		{
			rect.left+=24;
			//gfx.SetTextColor(RGBA(240,240,240));
			gfx.DrawText(cpane->strCaption.Buffer(), rect, NDT_END_ELLIPSIS|NDT_VCENTER|NDT_SINGLELINE, RGBA(255,240,240,240));
		}*/

		//###GUI###
		if (cpane->pstripBar)			cpane->pstripBar->SetWindowRect(rect);

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Draw Splitter Boxes
/*
	if (mPaneList.Count() <= 1)								return;

	HDC		hdc_bx	= ::GetDC(m_W32HWnd);
	HBRUSH	brush	= ::CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
//	HBRUSH	brush	= ::CreateSolidBrush(RGBA(0,0,0));
	RECT	w32rect;

	for (i=mPaneList.Count()-1; i>=0; i--)
	{
		NPANEINFO* cpane = mPaneList[i];

		//Calc the splitterBox Rect
		NRect rect		= cpane->rect;
		rect.left		=(udword)((float)rect.left		* mFactorX);
		rect.right		=(udword)((float)rect.right	* mFactorX);
		rect.top		=(udword)((float)rect.top		* mFactorY);
		rect.bottom	=(udword)((float)rect.bottom	* mFactorY);

		//Draw SplitterBox
		w32rect.left	= rect.right - SPLITBORDER_SIZE;	w32rect.top		= rect.top;
		w32rect.right	= rect.right;						w32rect.bottom	= rect.bottom;
		//::FillRect(hdc_bx, &w32rect, brush);

		w32rect.left	= rect.left;						w32rect.top		= rect.bottom - SPLITBORDER_SIZE;
		w32rect.right	= rect.right;						w32rect.bottom	= rect.bottom;
		//::FillRect(hdc_bx, &w32rect, brush);
	}

	::DeleteObject (brush);
	ReleaseDC(m_W32HWnd, hdc_bx);
*/
//	EndPaint(m_W32HWnd, &ps);

}

//-----------------------------------------------------------------
//!	\brief	Split the frame in 2 Columns
//!	\param	paneid					pane identifier
//!	\param	dwPosInPercent	pos is the location where the split occurs in percent
//!	\return return the new right pane identifier
//-----------------------------------------------------------------
udword	NGUILayout::SplitColumn(udword paneid, sdword dwPosInPercent)
{
	//Get the pane identifier from the user position
	NPANEINFO* leftpane = GetPaneInfo(paneid);

	sdword pos = (dwPosInPercent * leftpane->rect.Width() / 100);

	//Split this pane in 2 Columns
	// |Col1			|Col2					|
	// | leftpane	| rightpane		|
	// |	old			|	   new			|

	//Create a new pane (rightpane)
	NPANEINFO* rightpane		= NNEW(NPANEINFO);
	rightpane->id						= ++mCurPaneId;
	rightpane->wnd					= null;
	rightpane->rect.left		= leftpane->rect.left + pos;
	rightpane->rect.top			= leftpane->rect.top;
	rightpane->rect.right		= leftpane->rect.right;
	rightpane->rect.bottom	= leftpane->rect.bottom;

	NRect rc(rightpane->rect);
	rightpane->pstripBar		= NNEW(NStripBarCtrl);
	rightpane->pstripBar->Create("", rc, this);

	//Resize the left pane
	leftpane->rect.right		= leftpane->rect.left + pos;

	//Add new pane in panelist
	mPaneList.AddItem(rightpane);

	return rightpane->id;
}


//-----------------------------------------------------------------
//!	\brief	Split the frame in 2 Rows
//!	\param	paneid					pane identifier
//!	\param	dwPosInPercent	pos is the location where the split occurs in percent
//!	\return return the new right pane identifier
//-----------------------------------------------------------------
udword	NGUILayout::SplitRow	(udword paneid, sdword dwPosInPercent)
{
	//Get the pane identifier from the user position
	NPANEINFO* toppane = GetPaneInfo(paneid);

	sdword pos= (dwPosInPercent * toppane->rect.Height() / 100);

	//Split this pane in 2 Rows
	// -------------
	// Row1
	//   toppane
	//     old
	// -------------
	// Row2
	//   bottompane
	//      new
	// -------------

	//Create a new pane (bottompane)
	NPANEINFO* bottompane		= NNEW(NPANEINFO);
	bottompane->id					= ++mCurPaneId;
	bottompane->wnd					= null;
	bottompane->rect.left		= toppane->rect.left;
	bottompane->rect.top		= toppane->rect.top + pos;
	bottompane->rect.right	= toppane->rect.right;
	bottompane->rect.bottom	= toppane->rect.bottom;

	NRect rc(bottompane->rect);
	bottompane->pstripBar		= NNEW(NStripBarCtrl);
	bottompane->pstripBar->Create("", rc, this);

	//Resize the top pane
	toppane->rect.bottom		= toppane->rect.top + pos;

	//Add new pane in panelist
	mPaneList.AddItem(bottompane);

	return bottompane->id;
}


//-----------------------------------------------------------------
//!	\brief	Destroy a pane (SPLITPANE_DEFAULT_ID can't be deleting)
//!	\param	id							pane identifier
//!	\param	sidetomerge			side to merge
//!	\return true if success
//-----------------------------------------------------------------
bool	NGUILayout::DestroyPane(udword id, udword sidetomerge)
{
	//Check Id
	if (id<=SPLITPANE_DEFAULT_ID)									return false;

	//Check Side
	if ( (sidetomerge!=SIDE_TOP ) && (sidetomerge!=SIDE_BOTTOM) &&
		 (sidetomerge!=SIDE_LEFT) && (sidetomerge!=SIDE_RIGHT ))	return false;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Get Pane Info
	sdword i;

	NPANEINFO* pane	= null;
	for (i=mPaneList.Count()-1; i>=0; i--) {
		pane = mPaneList[i];
		if (pane->id == id)											break;
	}

	if (!pane)														return false;

	//Array for Hit panes
	NArray<NPANEINFO*> hitlist;

	//Clear Panes Side Flag
	for (i=mPaneList.Count()-1; i>=0; i--) 	mPaneList[i]->side = 0;

	//Get Top Pane List
	GetHitPanes(pane, sidetomerge, hitlist);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Resize the nearest panes
	for (i=hitlist.Count()-1; i>=0; i--) {
		NPANEINFO* p = hitlist[i];
		switch(sidetomerge) {
			case SIDE_TOP:		p->rect.bottom	= pane->rect.bottom;		break;
			case SIDE_BOTTOM:	p->rect.top		= pane->rect.top;			break;
			case SIDE_RIGHT:	p->rect.left	= pane->rect.left;			break;
			case SIDE_LEFT:		p->rect.right	= pane->rect.right;			break;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Remove the pane from list then Destroy paneinfos
//	mPaneList.Remove(index-1);	####TOFIX####
	NDELETE(pane, NPANEINFO);

	return false;
}


//-----------------------------------------------------------------
// Name:	CreateSplitterBox()
// Desc:	Create the Splitters Boxes
//-----------------------------------------------------------------
NSPLITBOX*	NGUILayout::CreateSplitterBox(NPANEINFO* pane, udword side)
{
	//Array for Hit panes
	NArray<NPANEINFO*> hitlist;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Create Vertical Splitters Boxes

	if (side&SIDE_RIGHT || side&SIDE_LEFT) {

		//Clear Panes Side Flag
		for (sdword i=mPaneList.Count()-1; i>=0; i--)	mPaneList[i]->side = 0;

		//------- Create Splitters Boxes -------

		//Reset Array
		hitlist.Clear();

		//Get panes hitting curpane
		GetHitPanes(pane, side, hitlist);

		//if any, create a splitter box for this panes
		if (hitlist.Count()) {

			//Create a new split box
			NSPLITBOX* splitbox = NNEW(NSPLITBOX);
			splitbox->type = 1;
			//splitbox->move.Init(4,4);

			//Calc Rect
			splitbox->rect = NRect(pane->rect.right-SPLITBORDER_SIZE, pane->rect.top, pane->rect.right+SPLITBORDER_SIZE, pane->rect.bottom);

			//Union with other pane rect
//			uword idx;
//			NPANEINFO* p = (NPANEINFO*)hitlist.GetFirst(idx);
//			while (idx!=(uword)-1) {
			for (sdword i=hitlist.Count()-1; i>=0; i--) {
				NPANEINFO* p = hitlist[i];

				if (p->side&SIDE_RIGHT)
					splitbox->rect.Union( NRect(p->rect.right-SPLITBORDER_SIZE, p->rect.top, p->rect.right+SPLITBORDER_SIZE, p->rect.bottom) );

				else if (p->side&SIDE_LEFT)
					splitbox->rect.Union( NRect(p->rect.left-SPLITBORDER_SIZE, p->rect.top, p->rect.left+SPLITBORDER_SIZE, p->rect.bottom) );

				splitbox->move.AddItem((udword&)p);
				splitbox->move.AddItem((udword)p->side);

			}

			//Convert pane factors to screen factors
			splitbox->rect.left		=(udword)((float)splitbox->rect.left	* mFactorX);
			splitbox->rect.right	=(udword)((float)splitbox->rect.right	* mFactorX);
			splitbox->rect.top		=(udword)((float)splitbox->rect.top		* mFactorY);
			splitbox->rect.bottom	=(udword)((float)splitbox->rect.bottom	* mFactorY);

			return splitbox;
		}

	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Create Horizontal Splitters Boxes

	if (side&SIDE_TOP || side&SIDE_BOTTOM) {

		//Clear Panes Side Flag
		//uword index2;
		//NPANEINFO* curpane2 = (NPANEINFO*)mPaneList.GetFirst(index2);
		//while (index2!=(uword)-1) {	curpane2->side = 0;	curpane2 = (NPANEINFO*)mPaneList.GetNext(index2); }
		for (sdword i=mPaneList.Count()-1; i>=0; i--)	mPaneList[i]->side = 0;

		//-------- Create Splitters Boxes -------

		//Reset Array
		hitlist.Clear();

		//Get panes hitting curpane
		GetHitPanes(pane, side, hitlist);

		//if any, create a splitter box for this panes
		if (hitlist.Count()) {

			//Create a new split box
			NSPLITBOX* splitbox = NNEW(NSPLITBOX);
			splitbox->type = 0;
			//splitbox->move.Init(4,4);

			//Calc Rect
			splitbox->rect = NRect(pane->rect.left, pane->rect.bottom-SPLITBORDER_SIZE, pane->rect.right, pane->rect.bottom+SPLITBORDER_SIZE);

			//Union with other pane rect
//			uword idx;
//			NPANEINFO* p = (NPANEINFO*)hitlist.GetFirst(idx);
//			while (idx!=(uword)-1) {
			for (sdword i=hitlist.Count()-1; i>=0; i--) {
				NPANEINFO* p = hitlist[i];

				if (p->side&SIDE_BOTTOM)
					splitbox->rect.Union( NRect(p->rect.left, p->rect.bottom-SPLITBORDER_SIZE, p->rect.right, p->rect.bottom+SPLITBORDER_SIZE) );

				else if (p->side&SIDE_TOP)
					splitbox->rect.Union( NRect(p->rect.left, p->rect.top-SPLITBORDER_SIZE, p->rect.right, p->rect.top+SPLITBORDER_SIZE) );

				splitbox->move.AddItem((udword&)p);
				splitbox->move.AddItem((udword)p->side);
			}

			//Convert pane factors to screen factors
			splitbox->rect.left		=(udword)((float)splitbox->rect.left	* mFactorX);
			splitbox->rect.right	=(udword)((float)splitbox->rect.right	* mFactorX);
			splitbox->rect.top		=(udword)((float)splitbox->rect.top		* mFactorY);
			splitbox->rect.bottom	=(udword)((float)splitbox->rect.bottom	* mFactorY);

			return splitbox;
		}
	}

	return null;

}


//-----------------------------------------------------------------
// Name:	DestroySplitterBox()
// Desc:	Create the Splitters Boxes
//-----------------------------------------------------------------
void	NGUILayout::DestroySplitterBox(NSPLITBOX* splitbox)
{
	if (splitbox==null)														return;

	//###TO DO### Multi splitter Horizontal + Vertical
	NDELETE(splitbox, NSPLITBOX);

}


//-----------------------------------------------------------------
// Name:	GetHitPanes()
// Desc:	Rq. user must Clear the Panes Side Flag !!!!
//-----------------------------------------------------------------
void	NGUILayout::GetHitPanes(NPANEINFO* pane, udword side, NArray<NPANEINFO*> &array)
{
	if (pane==null)															return;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Get Split Box
	NRect SplitBox = pane->rect;
	switch(side) {
		case 1:		SplitBox.bottom = SplitBox.top		+ SPLITBORDER_SIZE;	SplitBox.Deflate(SPLITBORDER_SIZE,-SPLITBORDER_SIZE);	break;
		case 2:		SplitBox.top	= SplitBox.bottom	- SPLITBORDER_SIZE;	SplitBox.Deflate(SPLITBORDER_SIZE,-SPLITBORDER_SIZE);	break;
		case 4:		SplitBox.right	= SplitBox.left		+ SPLITBORDER_SIZE;	SplitBox.Deflate(-SPLITBORDER_SIZE,SPLITBORDER_SIZE);	break;
		case 8:		SplitBox.left	= SplitBox.right	- SPLITBORDER_SIZE;	SplitBox.Deflate(-SPLITBORDER_SIZE,SPLITBORDER_SIZE);	break;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Get Hitting Pane
//	uword index;
//	NPANEINFO* curpane = (NPANEINFO*)mPaneList.GetFirst(index);
//	while (index!=(uword)-1) {
	for (sdword i=mPaneList.Count()-1; i>=0; i--) {
		NPANEINFO* curpane = mPaneList[i];

		if (curpane!=pane){
		//Check if this pane is hitting the Selected pane
			//----- down -----
			if (side == SIDE_BOTTOM){
				NRect R = curpane->rect;
				R.bottom = R.top + SPLITBORDER_SIZE;
				if (!R.IsIntersected(SplitBox)){
					if (!(curpane->side&SIDE_TOP)) {
						array.AddItem(curpane);
						curpane->side|=SIDE_TOP;
						GetHitPanes(curpane, SIDE_TOP, array);
					}
				}
			}
			//----- up -----
			else if (side == SIDE_TOP){
				NRect R = curpane->rect;
				R.top = R.bottom - SPLITBORDER_SIZE;
				if (!R.IsIntersected(SplitBox)) {
					if (!(curpane->side&SIDE_BOTTOM)) {
						array.AddItem(curpane);
						curpane->side|=SIDE_BOTTOM;
						GetHitPanes(curpane, SIDE_BOTTOM, array);
					}
				}
			}

			//----- left -----
			else if (side == SIDE_LEFT){
				NRect R = curpane->rect;
				R.left = R.right - SPLITBORDER_SIZE;
				if (!R.IsIntersected(SplitBox))	{
					if (!(curpane->side&SIDE_RIGHT)) {
						array.AddItem(curpane);
						curpane->side|=SIDE_RIGHT;
						GetHitPanes(curpane, SIDE_RIGHT, array);
					}
				}
			}

			//----- right -----
			else if (side == SIDE_RIGHT){
				NRect R = curpane->rect;
				R.right = R.left + SPLITBORDER_SIZE;
				if (!R.IsIntersected(SplitBox)) {
					if (!(curpane->side&SIDE_LEFT)){
						array.AddItem(curpane);
						curpane->side|=SIDE_LEFT;
						GetHitPanes(curpane, SIDE_LEFT, array);
					}
				}
			}

		}//if (curpane!=pane)

	}//while (index!=(uword)-1)


}


//-----------------------------------------------------------------
// Name:	UpdateCursor()
// Desc:
//-----------------------------------------------------------------
udword	NGUILayout::UpdateCursor(NPoint& mpos)
{
	//Get pane under the mouse
	NPoint pt = mpos;
	pt.x = (udword)((float)pt.x / mFactorX);
	pt.y = (udword)((float)pt.y / mFactorY);
	NPANEINFO* pane = GetPaneInfo(pt);
	if (pane==null)												return 0;

	//Check Splitter Boxes
	udword Flag = 0;

	//Convert screen coord to pane coord
	NRect rect = pane->rect;
	rect.left	=(udword)((float)rect.left	* mFactorX);
	rect.right	=(udword)((float)rect.right * mFactorX);
	rect.top	=(udword)((float)rect.top	* mFactorY);
	rect.bottom	=(udword)((float)rect.bottom* mFactorY);


	//(left box)
	NRect SBox = rect;
	SBox.right = SBox.left + SPLITBORDER_SIZE;
	if (SBox.Contain(mpos))		Flag|=SIDE_LEFT;

	//(top box)
	SBox = rect;
	SBox.bottom = SBox.top + SPLITBORDER_SIZE;
	if (SBox.Contain(mpos))		Flag|=SIDE_TOP;

	//(right box)
	SBox = rect;
	SBox.left = SBox.right - SPLITBORDER_SIZE;
	if (SBox.Contain(mpos))		Flag|=SIDE_RIGHT;

	//(bottom box)
	SBox = rect;
	SBox.top = SBox.bottom - SPLITBORDER_SIZE;
	if (SBox.Contain(mpos))		Flag|=SIDE_BOTTOM;

	//Set Cursor Aspect
	//if		(Flag&SIDE_TOP || Flag&SIDE_BOTTOM)		::SetCursor(::LoadCursor(null, IDC_SIZENS));
	//else if	(Flag&SIDE_LEFT|| Flag&SIDE_RIGHT )		::SetCursor(::LoadCursor(null, IDC_SIZEWE));
	//else if (Flag!=0)								::SetCursor(::LoadCursor(null, IDC_SIZEALL));	//Cross

	return Flag;
}


//-----------------------------------------------------------------
// Name:	GetPaneInfo()
// Desc:	Return the pane at the client coordinates (x,y)
//-----------------------------------------------------------------
NPANEINFO*	NGUILayout::GetPaneInfo(NPoint& pos)
{
	//Research the matching pane under this position
//	uword index;
//	NPANEINFO* paneinfo = (NPANEINFO*)mPaneList.GetFirst(index);
//	while (index!=(uword)-1) {
	for (sdword i=mPaneList.Count()-1; i>=0; i--) {
		NPANEINFO* paneinfo = mPaneList[i];
		if (paneinfo->rect.Contain(pos))							return paneinfo;
	}

	return null;
}

//-----------------------------------------------------------------
// Name:	GetPaneInfo()
// Desc:	Return pane info from an identifier (return null if not found)
//-----------------------------------------------------------------
NPANEINFO*	NGUILayout::GetPaneInfo(udword id)
{
	//Research the matching pane identifier
//	uword index;
//	NPANEINFO* paneinfo = (NPANEINFO*)mPaneList.GetFirst(index);
//	while (index!=(uword)-1) {
	for (sdword i=mPaneList.Count()-1; i>=0; i--) {
		NPANEINFO* paneinfo = mPaneList[i];
		if (paneinfo->id == id)							return paneinfo;
	}

	return null;
}

//-----------------------------------------------------------------
// Name:	SetPaneWnd()
// Desc:	Attach a window to a pane
//-----------------------------------------------------------------
bool	NGUILayout::SetPaneWnd(char* _pszCaption, udword id, NGUIWnd* wnd)
{
	//Research the matching pane identifier
//	uword index;
//	NPANEINFO* paneinfo = (NPANEINFO*)mPaneList.GetFirst(index);
//	while (index!=(uword)-1) {
	for (sdword i=mPaneList.Count()-1; i>=0; i--) {
		NPANEINFO* paneinfo = mPaneList[i];

		if (paneinfo->id == id)
		{
			paneinfo->wnd = wnd;
			paneinfo->strCaption = _pszCaption;

			wnd->SetWindowRect(paneinfo->rect);
			//###GUI###
			//::RedrawWindow(m_W32HWnd, null, null, RDW_INVALIDATE | RDW_UPDATENOW);		//RedrawWindow(OnPaint)
			return true;
		}
	}
	return false;
}



//-----------------------------------------------------------------
// Name:	MoveSplit()
// Desc:
//-----------------------------------------------------------------
bool	NGUILayout::MoveSplit(NSPLITBOX* splitbox, NPoint &deltas)
{
	if (splitbox == null)									return false;
	if (deltas.x == 0 && deltas.y == 0)		return false;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Move Splitter

	//Move Horizontal Splitter
	if (splitbox->type==0)
	{
		splitbox->rect.top   += deltas.y;
		splitbox->rect.bottom+= deltas.y;
	}

	//Move Vertical Splitter
	if (splitbox->type==1)
	{
		splitbox->rect.left  += deltas.x;
		splitbox->rect.right += deltas.x;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Splitter Collision Test
	udword i = 0;
	NPANEINFO* pane = (NPANEINFO*)splitbox->move[i++];
	udword	   side = splitbox->move[i++];
	while(pane!=null)
	{
		NRect rect = pane->rect;
		rect.left	=(udword)((float)rect.left	* mFactorX);
		rect.right	=(udword)((float)rect.right * mFactorX);
		rect.top	=(udword)((float)rect.top	* mFactorY);
		rect.bottom	=(udword)((float)rect.bottom* mFactorY);

		//Horizontal Moving Test
		if (side&SIDE_BOTTOM)
		{
			sdword pos = rect.bottom;
			pos = splitbox->rect.top + SPLITBORDER_SIZE;
			if ((pos - rect.top) < SPLITMINPANE_SIZE)					return false;

		} else if (side&SIDE_TOP) {
			sdword pos = rect.top;
			pos = splitbox->rect.top + SPLITBORDER_SIZE;
			if ((rect.bottom - pos) < SPLITMINPANE_SIZE)				return false;
		}

		//Vertical Moving Test
		if (side&SIDE_RIGHT) {
			sdword pos = rect.right;
			pos = splitbox->rect.left + SPLITBORDER_SIZE;
			if ((pos - rect.left) < SPLITMINPANE_SIZE)				return false;

		} else if (side&SIDE_LEFT) {
			sdword pos = rect.left;
			pos = splitbox->rect.left + SPLITBORDER_SIZE;
			if ((rect.right - pos) < SPLITMINPANE_SIZE)				return false;
		}

		//Next Pane
//		pane = (NPANEINFO*)splitbox->move.GetNext(idxm);
//		side = splitbox->move.GetNext(idxm);
		pane = (NPANEINFO*)splitbox->move[i++];
		side = splitbox->move[i++];
	}



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Recal Layout

	//Convert splitbox factors to pane factors
	NRect s = splitbox->rect;
	s.left	=(udword)((float)s.left		/ mFactorX);
	s.right	=(udword)((float)s.right	/ mFactorX);
	s.top	=(udword)((float)s.top		/ mFactorY);
	s.bottom=(udword)((float)s.bottom	/ mFactorY);

	//Windows Repositionning
	i = 0;
	pane = (NPANEINFO*)splitbox->move[i++];
	side = splitbox->move[i++];
	while(pane!=null)
	{
		if (side&SIDE_RIGHT)			pane->rect.right = s.left + SPLITBORDER_SIZE;
		else if (side&SIDE_LEFT)	pane->rect.left  = s.left + SPLITBORDER_SIZE;

		if (side&SIDE_BOTTOM)			pane->rect.bottom = s.top + SPLITBORDER_SIZE;
		else if (side&SIDE_TOP)		pane->rect.top    = s.top + SPLITBORDER_SIZE;

		if (pane->wnd)
		{
			NRect rect	= pane->rect;
			rect.left		=(udword)((float)rect.left	* mFactorX);
			rect.right	=(udword)((float)rect.right * mFactorX);
			rect.top		=(udword)((float)rect.top		* mFactorY) + CAPTIONPANE_SIZE;
			rect.bottom	=(udword)((float)rect.bottom* mFactorY);
			//###GUI###
			//::DeferWindowPos(hDWP, pane->wnd->m_W32HWnd, null, rect.left, rect.top, rect.Width() ,rect.Height() , SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED|SWP_NOACTIVATE);
		}

		//Next Pane
		pane = (NPANEINFO*)splitbox->move[i++];
		side = splitbox->move[i++];
	}

	return false;
}


//-----------------------------------------------------------------
// Name:	OnMouseMove()
// Desc:	...
//-----------------------------------------------------------------
void NGUILayout::OnMouseMove(NPoint pos)
{
	//Set Cursor Aspect
	udword side = UpdateCursor(pos);

/*
	//Left Button Change
//	if (_struct->mFlags&MFLG_L_CHANGED) {
	if (flags&MK_LBUTTON) {

		//bool drag = (_struct->mButtons&MBTN_LEFT_DOWN)!=0;
		bool drag = (flags&MK_LBUTTON)!=0;

		if (drag) {
			::SetCapture(m_W32HWnd);
			mLastMousePos	= pos;
			//get the Pane under the mouse
			NPoint pt = pos;
			pt.x = (udword)((float)pt.x / mFactorX);
			pt.y = (udword)((float)pt.y / mFactorY);
			NPANEINFO* pane	 = GetPaneInfo(pt);
			mSplitBox = CreateSplitterBox(pane, side);
		} else {
			::ReleaseCapture();
			DestroySplitterBox(mSplitBox);
			mSplitBox = null;
		}

	}
*/

	//Check Left Mouse Button Down
//	if (_struct->mFlags&MFLG_POS_CHANGED ) {

		if (mSplitBox) {

			//Calc Mouse Deltas
			NPoint deltas = pos - mLastMousePos;
			mLastMousePos = pos;

			//Move the splitters
			MoveSplit(mSplitBox, deltas);
			RedrawWindow();
		}//if (drag)

//	}


}


//-----------------------------------------------------------------
// Name:	OnLButtonDown()
// Desc:	...
//-----------------------------------------------------------------
void NGUILayout::OnLButtonDown(NPoint pos)
{
/*
	POINT  w32pt;
	::GetCursorPos(&w32pt);
	::ScreenToClient(m_W32HWnd, &w32pt);

	NPoint pos;
	pos.x	= w32pt.x;
	pos.y	= w32pt.y;*/

	udword side = UpdateCursor(pos);

	//Left Button Change
//	if (_struct->mFlags&MFLG_L_CHANGED) {
//	if (flags&MK_LBUTTON) {

		//bool drag = (_struct->mButtons&MBTN_LEFT_DOWN)!=0;
//		bool drag = (flags&MK_LBUTTON)!=0;

//		if (drag) {
			//###GUI###
			SetCapture();
			mLastMousePos = pos;
			//get the Pane under the mouse
			NPoint pt = mLastMousePos;
			pt.x = (udword)((float)pt.x / mFactorX);
			pt.y = (udword)((float)pt.y / mFactorY);
			NPANEINFO* pane	 = GetPaneInfo(pt);
			mSplitBox = CreateSplitterBox(pane, side);
/*		} else {
			::ReleaseCapture();
			DestroySplitterBox(mSplitBox);
			mSplitBox = null;
		}

	}*/

}

//-----------------------------------------------------------------
// Name:	OnLButtonUp()
// Desc:	...
//-----------------------------------------------------------------
void NGUILayout::OnLButtonUp(NPoint pos)
{
	ReleaseCapture();
	DestroySplitterBox(mSplitBox);
	mSplitBox = null;
}


//-----------------------------------------------------------------
// Name:	OnSize()
// Desc:	...
//-----------------------------------------------------------------
void NGUILayout::OnSize()
{
	//Get New Client Rect
	NRect SplitWnd = GetClientRect();
	NPoint S;

	//Get Old Client Rect ###TO FIX###
//	uword index;
//	NPANEINFO* paneinfo = (NPANEINFO*)mPaneList.GetFirst(index);
//	while (index!=(uword)-1) {
	for (sdword i=mPaneList.Count()-1; i>=0; i--)
	{
		NPANEINFO* paneinfo = mPaneList[i];
		if (paneinfo->rect.right  > S.x )	S.x = paneinfo->rect.right;
		if (paneinfo->rect.bottom > S.y )	S.y = paneinfo->rect.bottom;
	}

	//Calc x and y factors
	SplitWnd.right+=SPLITBORDER_SIZE;
	SplitWnd.bottom+=SPLITBORDER_SIZE;
	mFactorX = (float)SplitWnd.Width()	 / (float)S.x;
	mFactorY = (float)SplitWnd.Height()  / (float)S.y;

	//Redraw
	//RedrawWindow();
	//OnPaint();
	//###GUI###
	//::RedrawWindow(m_W32HWnd, null, null, RDW_INTERNALPAINT);
}
