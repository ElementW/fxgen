//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		TreeNodeCtrl.h
//! \brief	GUI control for tree nodes editing
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
//!	\date		12-02-2007
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
#include "CoreLibPkg.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define TN_ITEMHEIGHT			16
#define TN_ITEMTEXTIDENT	16
#define TN_ITEMIDENT			16

//-----------------------------------------------------------------
//! \struct NTNItemDesc
//! \brief	Item description
//-----------------------------------------------------------------
struct NTNItemDesc
{
	sdword					dwDepth;		//!< Tree Depth

	//Datas RT
	NObject*				pcobj;			//!< Object Associed
	NRect						rcItem;			//!< Windows Pos
	NRect						rcItemExpandSel;
	bool						bExpanded;	//!< Is Branch Expanded
	udword					dwLastUpdate;
};


//-----------------------------------------------------------------
//!	\class		NTreeNodeCtrl
//!	\brief		GUI control for tree nodes editing
//-----------------------------------------------------------------
class NTreeNodeCtrl : public NGUIWnd
{

public:
	// Constructor-Destructor
	NTreeNodeCtrl();
	virtual ~NTreeNodeCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NGUIWnd* parent);

	// Methods
	void	Clear();
	void	DisplayTreeNode(NTreeNode* _proot);
	void	Update();

	NObject*	GetSelectedItemObj();

	udword		GetSelectedItemIdx() { return m_dwCurItemIdx; }
	NObject*	GetParent(NObject* _pobjSon);

	//Notification Messages
	FDelegate		OnSelChange;

protected:
	// Internal methods
	udword GetItemIdxFromObject(NObject* _pcobj);
	void _DisplayTreeNode(NTreeNode* _pParent, udword _dwCurDepth, udword _dwCurPos);
	void InsertItem(NObject* _pcobj, udword _dwCurDepth, udword _dwCurPos);
	udword GetExpandItemIdxUnderPt(NPoint& _point);
	udword GetItemIdxUnderPt(NPoint& _point);
	void ExpandItemFromIdx(udword _idx, bool _bExpand=true);
	void SelectItemFromIdx(udword _idx);
	void DeleteItemFromIdx(udword _idx);
	udword GetSortedIdxFromIdx(udword _idx);

	bool IsParentItem(udword _nSortedIdx);
	bool IsItemExpanded(udword _idx);

	// Datas
	NPoint	m_ptStartMouse, m_ptCursor;

	NTreeNode* m_prootNode;

	NArray<NTNItemDesc>	m_carrayItems;
	NArray<udword>			m_carrayItemsSortedPos;

	udword	m_dwCurItemIdx;	//Idx in m_carrayItems
	udword	m_dwLastUpdate;

protected:
	//Win32 Events
	virtual	void	OnPaint(N2DPainter* _ppainter);
	virtual void	OnSize();
	virtual void	OnMouseMove(NPoint pos );
	virtual void	OnMButtonDown(NPoint pos);
	virtual void	OnMButtonUp(NPoint pos);
	virtual void	OnLButtonUp(NPoint pos);
	virtual void	OnLButtonDown(NPoint pos);
	virtual	void	OnLButtonDblClk(NPoint point);
	virtual void	OnRButtonDown(NPoint pos);
	virtual	void	OnKeyDown(udword dwchar);
};

