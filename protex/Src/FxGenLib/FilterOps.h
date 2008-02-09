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
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Blur"; }			//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }		//!< Operator's Category

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
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Colors"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

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
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Light"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};


//-----------------------------------------------------------------
//!	\class		NLightOp2
//!	\brief		Lighting rendering
//-----------------------------------------------------------------
class FXGEN_API NLightOp2 : public NOperator
{
public:
	DECLARE_CLASS();

	NLightOp2();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Light2"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,  float _fDetailFactor);
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
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Normals"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NAbnormalsOp
//!	\brief		Rotate normal vectors
//-----------------------------------------------------------------
class FXGEN_API NAbnormalsOp : public NOperator
{
public:
	DECLARE_CLASS();

	NAbnormalsOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Abnormals"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NNormalsOp
//!	\brief		Compute 2D Normals
//-----------------------------------------------------------------
class FXGEN_API NSlopeMagnitudeOp : public NOperator
{
public:
	DECLARE_CLASS();

	NSlopeMagnitudeOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Slope Magnitude"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

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
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Invert"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

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
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Threshold"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

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
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Alpha"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NSegmentOp
//!	\brief		Unifies color on joint segments
//-----------------------------------------------------------------
class FXGEN_API NSegmentOp : public NOperator
{
public:
	DECLARE_CLASS();

	NSegmentOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Segment"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};


//-----------------------------------------------------------------
//!	\class		NDilate
//!	\brief		Expands colored areas
//-----------------------------------------------------------------
class FXGEN_API NDilateOp : public NOperator
{
public:
	DECLARE_CLASS();

	NDilateOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Dilate"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};


//-----------------------------------------------------------------
//!	\class		NAlphaMaskOp
//!	\brief		Create monochrome image from alpha channel
//-----------------------------------------------------------------
class FXGEN_API NAlphaMaskOp : public NOperator
{
public:
	DECLARE_CLASS();

	NAlphaMaskOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "AlphaMask"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};