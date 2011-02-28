//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		DistordOps.h
//! \brief	Distord Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	        Anders Stenberg (anders.stenberg@gmail.com)
//!         Andrew Caudwell (acaudwell@gmail.com)
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
#ifndef DISTORDOPS_H
#define DISTORDOPS_H


//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "EngineOp.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//NObject
//	NOperatorFx
//		NRotoZoomOp
//		NDistortOp
//		NWarpOp
//		NLookupOp


//-----------------------------------------------------------------
//!	\class		NRotoZoomOp
//!	\brief		RotoZoom filtering
//-----------------------------------------------------------------
class CORELIB_API NRotoZoomOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NRotoZoomOp();
	virtual ~NRotoZoomOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Roto Zoom"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }		//!< Operator's Category


	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);
};

//-----------------------------------------------------------------
//!	\class		NDistordOp
//!	\brief		Distort filtering
//-----------------------------------------------------------------
class CORELIB_API NDistortOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NDistortOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Distort"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);

};

//-----------------------------------------------------------------
//!	\class		NVortexOp
//!	\brief		Twirls the input
//-----------------------------------------------------------------
class CORELIB_API NVortexOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NVortexOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Vortex"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);

};

//-----------------------------------------------------------------
//!	\class		NLookupOp
//!	\brief		Use one input as lookup coordinates in another
//-----------------------------------------------------------------
class CORELIB_API NLookupOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NLookupOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Lookup"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);

};

//-----------------------------------------------------------------
//!	\class		NCraterOp
//!	\brief		Render noise
//-----------------------------------------------------------------
class NCraterOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NCraterOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Crater"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);
};


/*//-----------------------------------------------------------------
//!	\class		NCraterSphereOp
//!	\brief		Render noise
//-----------------------------------------------------------------
class NCraterSphereOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NCraterSphereOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "CraterSphere"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperatorFx** _pOpsInts);
};
*/

#endif //DISTORDOPS_H
