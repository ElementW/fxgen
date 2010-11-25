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
#ifndef FILTEROPS_H
#define FILTEROPS_H


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
class CORELIB_API NBlurOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NColorsOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NLightOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NLightOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Light"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};


//-----------------------------------------------------------------
//!	\class		NNormalsOp
//!	\brief		Compute 2D Normals
//-----------------------------------------------------------------
class CORELIB_API NNormalsOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NAbnormalsOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NSlopeMagnitudeOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NInvertOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NThresholdOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NAlphaOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NSegmentOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NDilateOp : public NOperator
{
public:
	FDECLARE_CLASS();

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
class CORELIB_API NAlphaMaskOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NAlphaMaskOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "AlphaMask"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

#endif //FILTEROPS_H