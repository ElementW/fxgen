//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		VectorPropertyItems.h
//! \brief	Base class for the VectorData Property Item
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	        Anders Stenberg (anders.stenberg@gmail.com)
//!	\date		31-05-2007
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
#include "PropertyItems.h"
#include "GUI.h"

//-----------------------------------------------------------------
//									Prototypes
//-----------------------------------------------------------------
class NOperator;

class NPropertyItem;
	class NVectorDataProp;


//-----------------------------------------------------------------
//!	\class	NVectorDataProp
//! \brief	Vector Data Property Item
//-----------------------------------------------------------------
class NVectorDataProp : public NPropertyItem
{
public:
	DECLARE_CLASS();

	virtual	void		DrawItem(NGraphics* pdc, NRect& rcItem);
	virtual bool		BeginEdit	(NRect& rcItem);
	virtual bool		EndEdit		(bool bSaveChanged=true);
};
