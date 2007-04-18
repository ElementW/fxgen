//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DentoidOps.h
//! \brief	Operators contributed by Anders 'Dentoid' Stenberg
//!
//!	\author	Anders Stenberg (anders.stenberg@gmail.com)
//!	\date		12-04-2007
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
//		NCellOp
//		NInvertOp

//-----------------------------------------------------------------
//!	\class		NCellOp
//!	\brief		Render cells
//-----------------------------------------------------------------
class NCellOp : public NOperator
{
public:
	DECLARE_CLASS();

	NCellOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Cell"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};


//-----------------------------------------------------------------
//!	\class		NInvertOp
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class NInvertOp : public NOperator
{
public:
	DECLARE_CLASS();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Invert"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Filter"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};
