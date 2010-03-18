//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RenderOpsUI.h
//! \brief	Render Operators Node for user interface
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!
//!	\date		26-02-2010
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef RENDEROPSUI_H
#define RENDEROPSUI_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "Operator.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//NObject
//	NOperatorNode
//		NFlatOp
//		NCloudOp
//		NGradientOp
//		NCellOp
//		NNoiseOp

//-----------------------------------------------------------------
//!	\class		NFlatOp
//!	\brief		Flat rendering
//-----------------------------------------------------------------
/*struct SFlatParams
{
	ubyte wpow;
	ubyte hpow;
	NRGBA color;
};*/

class  NFlatOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NFlatOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()				{ return "Flat"; }			//!< Operator's Name
	virtual const char*		GetCategory()		{ return "Render"; }	//!< Operator's Category

	//Params
	//SFlatParams m_params;
	ubyte wpow;
	ubyte hpow;
	NRGBA color;

};

//-----------------------------------------------------------------
//!	\class		NCloudOp
//!	\brief		Render clouds
//-----------------------------------------------------------------
class  NCloudOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NCloudOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Cloud"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Params
	//SCloudParams	m_params;
};

//-----------------------------------------------------------------
//!	\class		NGradientOp
//!	\brief		Four corners gradient rendering
//-----------------------------------------------------------------
class  NGradientOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NGradientOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Gradient"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }		//!< Operator's Category

	//Params
	//SGradientParams	m_params;
};

//-----------------------------------------------------------------
//!	\class		NCellOp
//!	\brief		Render cells
//-----------------------------------------------------------------
class  NCellOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NCellOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Cell"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Params
	//SCellParams	m_params;
};

//-----------------------------------------------------------------
//!	\class		NNoiseOp
//!	\brief		Render noise
//-----------------------------------------------------------------
class  NNoiseOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NNoiseOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Noise"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Params
	//SNoiseParams	m_params;
};

#endif //RENDEROPSUI_H
