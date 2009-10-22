//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PropertiesCtrl.h
//! \brief	GUI control for properties editing
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
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
#include "PropertyItems.h"

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define PC_ROWSHEIGHT					16
#define PC_ROWTEXTIDENT				16
#define PC_ROWGROUPIDENT			8
#define PC_ROWDEPTHIDENT			8
#define PC_ROWGROUPENDHEIGHT	4

//-----------------------------------------------------------------
//! \struct NRowDesc
//! \brief	Row description
//-----------------------------------------------------------------
struct NRowDesc
{
	NString					strName;		//!< Row's name
	sdword					dwDepth;		//!< Tree Depth

	//Datas RT
	NRect						rcItem;			//!< Windows Pos
	NRect						rcItemAnim;	//!< Windows Pos
	bool						bExpanded;	//!< Is Branch Expanded
	NPropertyItem*	pItem;			//!< Item for Editing
};

//-----------------------------------------------------------------
//!	\class		NPropertiesCtrl
//!	\brief		Object's Properties control viewer
//-----------------------------------------------------------------
class NPropertiesCtrl : public NWControl
{
public:
	// Constructor-Destructor
					NPropertiesCtrl();
	virtual ~NPropertiesCtrl();

	// Methods
	virtual	bool Create(const char* name, const NRect& rect, NWnd* parent);

	void	Update();	//!< Update Display
	void	DisplayObjectProperties(NObject* pobj);		//!< Display Object Properties

	void 	SaveRowEditing();
	void 	EndRowEditing(bool bSaveChanged=true);

	// Messages Notify
	virtual void OnPropertiesChanged()	{};

protected:
	// Internal methods
	void			_DisplayObjectProperties(NObject* _pobj, udword _dwDepth);	//!< Display Object Properties (called by DisplayObjectProperties)
	void			_DisplayAnimationObjectProperties(NObject* _pobj, udword _dwDepth);	//!< Display Object Properties (called by DisplayObjectProperties)


	udword		AddGroup(const char* pszName, udword _dwDepth);
	udword		AddVarProperties(NVarsBloc* _pvarBloc, NVarsBlocDesc* _pvarBlocDesc, NVarValue* _pvarValue, udword _dwvarIdx, udword _dwDepth);

	void			DeleteAllProperties();	//!< Reset
	NRowDesc* GetRowAtIdx(udword idx);
	udword		GetRowUnderPoint(NPoint& pt);
	void 			OffsetRowValue(sdword dwOffset);
	bool 			SelectRow(udword dwRowIdx);
	bool			ExpandRow(udword _dwRowIdx);
	void 			ClickRow(udword dwRowIdx, NPoint& pt);
	void 			StartRowEditing(udword dwRowIdx);
	bool			IsAnimButtonUnderPoint(NPoint& pt);
	void			AddRemoveAnimControlToRow(udword _dwRowIdx);
	udword		GetRowIdxFromGroupIdx(udword _groupIdx);


	// Datas
	NObject*	m_pobj;		//!< Object Edited

	udword		m_dwHeaderWidth;
	udword		m_dwCurSelRow;
	bool			m_bEditingRow;
	bool			m_bAddValue;
	NPoint		m_ptStartMouse, m_ptCursor;

	GArray<NRowDesc>	m_carrayRowsDesc;

protected:
	//Win32 Messages Dispatching
	virtual	void	OnPaint();
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


