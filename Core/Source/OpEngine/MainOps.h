//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MainOps.h
//! \brief	Main Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!	\date		12-02-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#ifndef MAINOPS_H
#define MAINOPS_H


//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "EngineOp.h"

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
class NBitmap;

//-----------------------------------------------------------------
//!	\class		NStoreOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class CORELIB_API NStoreOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NStoreOp();
	virtual ~NStoreOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x004080FF;	}
	virtual const char*		GetName()			{ return "Store";	}		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Misc"; }		//!< Operator's Category
	virtual const char*		GetUserName();											//!< Operator's User Name

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

};

//-----------------------------------------------------------------
//!	\class		NStoreResultOp
//!	\brief		Store final result operator
//-----------------------------------------------------------------
class CORELIB_API NStoreResultOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NStoreResultOp();
	virtual ~NStoreResultOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x000000FF;	}
	virtual const char*		GetName()			{ return "Store Result";	}	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Misc";	}					//!< Operator's Category
	virtual const char*		GetUserName();														//!< Operator's User Name

	//Bitmap Access
	NBitmap* GetBitmap();

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

};

//-----------------------------------------------------------------
//!	\class		NLoadOp
//!	\brief		Load a stored operator
//-----------------------------------------------------------------
class CORELIB_API NLoadOp : public NOperator
{
public:
	FDECLARE_CLASS();

	NLoadOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x00e36464;	}
	virtual const char*		GetName()			{ return "Load"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Misc"; }		//!< Operator's Category
	virtual const char*		GetUserName();											//!< Operator's User Name

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

};


//-----------------------------------------------------------------
//!	\class		NChannelAnimFX1Op
//!	\brief		Channel animation operator
//-----------------------------------------------------------------
class CORELIB_API NChannelAnimFX1Op : public NOperator
{
public:
	FDECLARE_CLASS();

	NChannelAnimFX1Op();

	//Methods
	virtual	udword	GetColor()		{ return 0x00e36464;	}
	virtual const char*		GetName()			{ return "Channel FX1"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Animation"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

};

#endif //MAINOPS_H
