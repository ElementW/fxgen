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
//	NOperator
//		NRotoZoomOp
//		NDistortOp
//		NWarpOp
//		NLookupOp


//-----------------------------------------------------------------
//!	\class		NRotoZoomOp
//!	\brief		RotoZoom filtering
//-----------------------------------------------------------------
class FXGEN_API NRotoZoomOp : public NOperator
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
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NDistordOp
//!	\brief		Distort filtering
//-----------------------------------------------------------------
class FXGEN_API NDistortOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NDistortOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Distort"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

};

//-----------------------------------------------------------------
//!	\class		NVortexOp
//!	\brief		Twirls the input
//-----------------------------------------------------------------
class FXGEN_API NVortexOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NVortexOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Vortex"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

};

//-----------------------------------------------------------------
//!	\class		NLookupOp
//!	\brief		Use one input as lookup coordinates in another
//-----------------------------------------------------------------
class FXGEN_API NLookupOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NLookupOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Lookup"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

};

//-----------------------------------------------------------------
//!	\class		NCraterOp
//!	\brief		Render noise
//-----------------------------------------------------------------
class NCraterOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NCraterOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Crater"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};


/*//-----------------------------------------------------------------
//!	\class		NCraterSphereOp
//!	\brief		Render noise
//-----------------------------------------------------------------
class NCraterSphereOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NCraterSphereOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "CraterSphere"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Distord"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};
*/

#endif //DISTORDOPS_H
