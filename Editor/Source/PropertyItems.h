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
#include "CoreLibPkg.h"
/*#include "Core.h"
#include "GUI.h"
#include "StandardsCtrl.h"*/

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
	virtual void		Init() {}
	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem)	= 0;
	virtual bool		BeginEdit	(NRect& rcItem)										{ return false;	}
	virtual bool		EndEdit		(bool bSaveChanged=true)					{ return false;	}
	virtual	bool		AddValue(sdword dwDelta)										{ return false; }
	virtual void		Click(NPoint& pt, NRect& rcItem)	{}

	//Members Access
	char* GetValue()	{ return m_strValue.Buffer(); }

protected:
	NString			m_strValue;			//!< Value in string form
	NGUIWnd*	m_pwNGraphicstrl;			//!< Control used for item edition
public:
	NVarsBloc*			m_pvarBloc;			//!< Variable bloc
	NVarsBlocDesc*	m_pvarBlocDesc;	//!< Variable Description
	NVarValue*			m_pvarValue;		//!< Variable Value Ptr	//###TOREMOVE### see m_dwvarIdx
	udword					m_dwvarIdx;			//!< Variable indice in bloc

	NGUIWnd*			m_pParent;				//!< Parent windows (ie CPropertiesCtrl)
};

//-----------------------------------------------------------------
//!	\class	NUbyteProp
//! \brief	UByte Property Item
//-----------------------------------------------------------------
class NUbyteProp : public NPropertyItem
{
public:
	FDECLARE_CLASS();

	virtual void		Init();
	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem);

protected:
	NSlideCtrl m_slider;
	void OnValueChanged(NObject* _psender);
};


//-----------------------------------------------------------------
//!	\class	NUwordProp
//! \brief	Uword Property Item
//-----------------------------------------------------------------
class NUwordProp : public NPropertyItem
{
public:
	FDECLARE_CLASS();

	virtual void		Init();
	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem);

protected:
	NSlideCtrl m_slider;
	void OnValueChanged(NObject* _psender);
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
	FDECLARE_CLASS();

	virtual void		Init();
	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem);

protected:
	NSlideCtrl m_slider;
	void OnValueChanged(NObject* _psender);
};


//-----------------------------------------------------------------
//!	\class	NUFloatProp
//! \brief	Unsigned Float Property Item
//-----------------------------------------------------------------
/*class NUFloatProp : public NPropertyItem
{
public:
	FDECLARE_CLASS();
	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
	virtual	bool		AddValue(sdword dwDelta);
};*/

//-----------------------------------------------------------------
//!	\class	NColorProp
//! \brief	Color Property Item
//-----------------------------------------------------------------
class NColorProp : public NPropertyItem
{
public:
	FDECLARE_CLASS();

	virtual void		Init();
	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem);

protected:
	NColorButtonCtrl m_button;
	void OnValueChanged(NObject* _psender);
};

//-----------------------------------------------------------------
//!	\class	NUbyteComboProp
//! \brief	UByte Property Item
//-----------------------------------------------------------------
class NUbyteComboProp : public NPropertyItem
{
public:
	FDECLARE_CLASS();

	virtual void	Init();
	virtual	void	DrawItem(N2DPainter* pdc, NRect& rcItem);

protected:
	NMenuButtonCtrl m_button;
	NArray<NString>	m_carrayStringsList;

	void OnValueChanged(NObject* _psender);
};

//-----------------------------------------------------------------
//!	\class	NFileBrowserProp
//! \brief	File browser Property Item
//-----------------------------------------------------------------
/*class NFileBrowserProp : public NPropertyItem
{
public:
	FDECLARE_CLASS();

	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
};*/

//-----------------------------------------------------------------
//!	\class	NUseStoredOpsProp
//! \brief	File browser Property Item
//-----------------------------------------------------------------
class NUseStoredOpsProp : public NPropertyItem
{
public:
	FDECLARE_CLASS();

	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem);
	//virtual void		Click(NPoint& pt, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);

protected:
	//Messages Notify
	void OnMenuClick(NObject* _psender);
	//Methods
	void BuildMenu(NTreeNode* _pnode);
	//Datas
	NMenuCtrl	m_wndMenu;
};


//-----------------------------------------------------------------
//!	\class	NStringProp
//! \brief	String Property Item
//-----------------------------------------------------------------
/*class NStringProp : public NPropertyItem
{
public:
	FDECLARE_CLASS();

	virtual	void		DrawItem(N2DPainter* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
};
*/
