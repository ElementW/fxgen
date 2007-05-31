//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		CombineOps.h
//! \brief	Combine Operators
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
//		NNopOp
//		NRectOp
//		NPixelsOp
//		NAddOp
//		NGlowOp
//		NCrackOp
//		NLerpOp

//-----------------------------------------------------------------
//!	\class		NNopOp
//!	\brief		Nop operator
//-----------------------------------------------------------------
class FXGEN_API NNopOp : public NOperator
{
public:
	DECLARE_CLASS();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Nop"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Misc"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};


//-----------------------------------------------------------------
//!	\class		NRectOp
//!	\brief		Rectangle rendering
//-----------------------------------------------------------------
class FXGEN_API NRectOp : public NOperator
{
public:
	DECLARE_CLASS();

	NRectOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Rect"; }			//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};


//-----------------------------------------------------------------
//!	\class		NPixelsOp
//!	\brief		Pixels rendering
//-----------------------------------------------------------------
class FXGEN_API NPixelsOp : public NOperator
{
public:
	DECLARE_CLASS();

	NPixelsOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Pixels"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NAddOp
//!	\brief		Add bitmaps
//-----------------------------------------------------------------
class FXGEN_API NAddOp : public NOperator
{
public:
	DECLARE_CLASS();

	NAddOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Add"; }			//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NGlowOp
//!	\brief		Glow rendering
//-----------------------------------------------------------------
class FXGEN_API NGlowOp : public NOperator
{
public:
	DECLARE_CLASS();

	NGlowOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Glow"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category


	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};

//-----------------------------------------------------------------
//!	\class		NCrackOp
//!	\brief		Compute cracked bitmap
//-----------------------------------------------------------------
class FXGEN_API NCrackOp : public NOperator
{
public:
	DECLARE_CLASS();

	NCrackOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Crack"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

	//Methods
	float myfRandom();

};


//-----------------------------------------------------------------
//!	\class		NLerpOp
//!	\brief		Cuts all color information above or below a threshold
//-----------------------------------------------------------------
class FXGEN_API NLerpOp : public NOperator
{
public:
	DECLARE_CLASS();

	NLerpOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Lerp"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};
