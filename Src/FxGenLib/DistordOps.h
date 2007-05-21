//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DistordOps.h
//! \brief	Distord Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
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
//		NRotoZoomOp
//		NDistortOp


//-----------------------------------------------------------------
//!	\class		NRotoZoomOp
//!	\brief		RotoZoom filtering
//-----------------------------------------------------------------
class NRotoZoomOp : public NOperator
{
public:
	DECLARE_CLASS();

	NRotoZoomOp();
	virtual ~NRotoZoomOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Roto Zoom"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Distord"; }		//!< Operator's Category


	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NDistordOp
//!	\brief		Distort filtering
//-----------------------------------------------------------------
class NDistortOp : public NOperator
{
public:
	DECLARE_CLASS();

	NDistortOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Distort"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};
