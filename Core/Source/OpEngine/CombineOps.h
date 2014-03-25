//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		CombineOps.h
//! \brief	Combine Operators
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
#ifndef COMBINEOPS_H
#define COMBINEOPS_H

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "EngineOp.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//NObject
//	NOperatorFx
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
class CORELIB_API NNopOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	//Methods
	virtual	udword	GetColor()					{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Nop"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Misc"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);
	
};


//-----------------------------------------------------------------
//!	\class		NRectOp
//!	\brief		Rectangle rendering
//-----------------------------------------------------------------
class CORELIB_API NRectOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NRectOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Rect"; }			//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);
};


//-----------------------------------------------------------------
//!	\class		NPixelsOp
//!	\brief		Pixels rendering
//-----------------------------------------------------------------
class CORELIB_API NPixelsOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NPixelsOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Pixels"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);
};

//-----------------------------------------------------------------
//!	\class		NAddOp
//!	\brief		Add bitmaps
//-----------------------------------------------------------------
class CORELIB_API NAddOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NAddOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Add"; }			//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);
};

//-----------------------------------------------------------------
//!	\class		NGlowOp
//!	\brief		Glow rendering
//-----------------------------------------------------------------
class CORELIB_API NGlowOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NGlowOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Glow"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category


	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);

};

//-----------------------------------------------------------------
//!	\class		NCrackOp
//!	\brief		Compute cracked bitmap
//-----------------------------------------------------------------
class CORELIB_API NCrackOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NCrackOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Crack"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);

	//Methods
	float myfRandom();

};


//-----------------------------------------------------------------
//!	\class		NLerpOp
//!	\brief		Cuts all color information above or below a threshold
//-----------------------------------------------------------------
class CORELIB_API NLerpOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NLerpOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Lerp"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combine"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);
};

#endif //COMBINEOPS_H
