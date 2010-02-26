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
class FXGEN_API NFlatOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NFlatOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Flat"; }			//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

};

//-----------------------------------------------------------------
//!	\class		NCloudOp
//!	\brief		Render clouds
//-----------------------------------------------------------------
class FXGEN_API NCloudOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NCloudOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Cloud"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

};

//-----------------------------------------------------------------
//!	\class		NGradientOp
//!	\brief		Four corners gradient rendering
//-----------------------------------------------------------------
class FXGEN_API NGradientOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NGradientOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Gradient"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }		//!< Operator's Category

};

//-----------------------------------------------------------------
//!	\class		NCellOp
//!	\brief		Render cells
//-----------------------------------------------------------------
class FXGEN_API NCellOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NCellOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Cell"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

};

//-----------------------------------------------------------------
//!	\class		NNoiseOp
//!	\brief		Render noise
//-----------------------------------------------------------------
class FXGEN_API NNoiseOp : public NOperatorNode
{
public:
	FDECLARE_CLASS();

	NNoiseOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Noise"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

};

#endif //RENDEROPSUI_H
