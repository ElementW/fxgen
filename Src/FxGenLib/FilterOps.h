//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		FilterOps.h
//! \brief	Filter Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
//!
//!	\date		17-05-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
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
#include "EngineOp.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//NObject
//	NOperator
//		NBlurOp
//		NColorsOp
//		NLightOp
//		NNormalsOp
//		NInvertOp
//		NThresholdOp
//		NAlphaOp

//-----------------------------------------------------------------
//!	\class		NBlurOp
//!	\brief		Blur filtering
//-----------------------------------------------------------------
class FXGEN_API NBlurOp : public NOperator
{
public:
	DECLARE_CLASS();

	NBlurOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Blur"; }			//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NColorsOp
//!	\brief		Colors filtering
//-----------------------------------------------------------------
class FXGEN_API NColorsOp : public NOperator
{
public:
	DECLARE_CLASS();

	NColorsOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Colors"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};


//-----------------------------------------------------------------
//!	\class		NLightOp
//!	\brief		Lighting rendering
//-----------------------------------------------------------------
class FXGEN_API NLightOp : public NOperator
{
public:
	DECLARE_CLASS();

	NLightOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Light"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NNormalsOp
//!	\brief		Compute 2D Normals
//-----------------------------------------------------------------
class FXGEN_API NNormalsOp : public NOperator
{
public:
	DECLARE_CLASS();

	NNormalsOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Normals"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};


//-----------------------------------------------------------------
//!	\class		NInvertOp
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NInvertOp : public NOperator
{
public:
	DECLARE_CLASS();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Invert"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NThresholdOp
//!	\brief		Cuts all color information above or below a threshold
//-----------------------------------------------------------------
class FXGEN_API NThresholdOp : public NOperator
{
public:
	DECLARE_CLASS();

	NThresholdOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Threshold"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NAlphaOp
//!	\brief		Create alpha channel from RGB bitmap source
//-----------------------------------------------------------------
class FXGEN_API NAlphaOp : public NOperator
{
public:
	DECLARE_CLASS();

	NAlphaOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Alpha"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};
