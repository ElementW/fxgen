//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		PropertyItems.h
//! \brief	Base classes for Properties Items
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
//									Includes
//-----------------------------------------------------------------
#include "Core.h"
#include "GUI.h"

//-----------------------------------------------------------------
//									Prototypes
//-----------------------------------------------------------------
class NOperator;

class NPropertyItem;
	class NUbyteProp;
	class NUwordProp;
	class NUdwordProp;
	class NFloatProp;
	class NColorProp;
	class NUbyteComboProp;
	class NFileBrowserProp;
	class NStringProp;
	class NUseStoredOpsProp;

//-----------------------------------------------------------------
//!	\class	NPropertyItem
//! \brief	Base class for Properties Items
//-----------------------------------------------------------------
class NPropertyItem : public NObject
{
public:
	NPropertyItem();
	virtual ~NPropertyItem();

	//Display
	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem)	= 0;
	virtual bool		BeginEdit	(NRect& rcItem)										{ return false;	}
	virtual bool		EndEdit		(bool bSaveChanged=true)					{ return false;	}
	virtual	bool		AddValue(sdword dwDelta)										{ return false; }
	virtual void		Click(NPoint& pt, NRect& rcItem)	{}

	//Members Access
	char* GetValue()	{ return m_strValue.Buffer(); }

protected:
	NString			m_strValue;			//!< Value in string form
	NWControl*	m_pwNGraphicstrl;			//!< Control used for item edition
public:
	NVarsBloc*			m_pvarBloc;			//!< Variable bloc
	NVarsBlocDesc*	m_pvarBlocDesc;	//!< Variable Description
	NVarValue*			m_pvarValue;		//!< Variable Value Ptr	//###TOREMOVE### see m_dwvarIdx
	udword					m_dwvarIdx;			//!< Variable indice in bloc

	NWnd*			m_pParent;				//!< Parent windows (ie CPropertiesCtrl)
};

//-----------------------------------------------------------------
//!	\class	NUbyteProp
//! \brief	UByte Property Item
//-----------------------------------------------------------------
class NUbyteProp : public NPropertyItem
{
public:
	DECLARE_CLASS();

	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
	virtual	bool		AddValue(sdword dwDelta);
};

//-----------------------------------------------------------------
//!	\class	NUwordProp
//! \brief	Uword Property Item
//-----------------------------------------------------------------
class NUwordProp : public NPropertyItem
{
public:
	DECLARE_CLASS();

	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
	virtual	bool		AddValue(sdword dwDelta);
};

//-----------------------------------------------------------------
//!	\class	NUdwordProp
//! \brief	Udword Property Item
//-----------------------------------------------------------------
class NUdwordProp : public NPropertyItem
{
public:
	//###TODO###
};


//-----------------------------------------------------------------
//!	\class	NFloatProp
//! \brief	Float Property Item
//-----------------------------------------------------------------
class NFloatProp : public NPropertyItem
{
public:
	DECLARE_CLASS();
	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
	virtual	bool		AddValue(sdword dwDelta);
};

//-----------------------------------------------------------------
//!	\class	NColorProp
//! \brief	Color Property Item
//-----------------------------------------------------------------
class NColorProp : public NPropertyItem
{
public:
	DECLARE_CLASS();

	NColorProp()		{ m_dwRGBEditingIdx = 0; }
	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
	virtual	bool		AddValue(sdword dwDelta);
	virtual void		Click(NPoint& pt, NRect& rcItem);
protected:
	udword m_dwRGBEditingIdx;
};

//-----------------------------------------------------------------
//!	\class	NUbyteComboProp
//! \brief	UByte Property Item
//-----------------------------------------------------------------
class NUbyteComboProp : public NPropertyItem
{
public:
	DECLARE_CLASS();

	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);

protected:
	//HMENU m_hMenu;
	NMenuCtrl	m_wndMenu;
	NArray<NString>	m_carrayStringsList;
};

//-----------------------------------------------------------------
//!	\class	NFileBrowserProp
//! \brief	File browser Property Item
//-----------------------------------------------------------------
class NFileBrowserProp : public NPropertyItem
{
public:
	DECLARE_CLASS();

	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
};

//-----------------------------------------------------------------
//!	\class	NUseStoredOpsProp
//! \brief	File browser Property Item
//-----------------------------------------------------------------
class NUseStoredOpsProp : public NPropertyItem
{
public:
	DECLARE_CLASS();
	
	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	//virtual void		Click(NPoint& pt, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);

	void BuildMenu(NTreeNode* _pnode);
	//HMENU m_hMenu;
	NMenuCtrl	m_wndMenu;
};


//-----------------------------------------------------------------
//!	\class	NStringProp
//! \brief	String Property Item
//-----------------------------------------------------------------
class NStringProp : public NPropertyItem
{
public:
	DECLARE_CLASS();

	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
};
