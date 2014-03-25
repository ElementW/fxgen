//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		MainOps.h
//! \brief	Main Operators
//!
//!	\author	Johann Nadalutti (jnadalutti@gmail.com)
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
class N2DBitmap;

//-----------------------------------------------------------------
//!	\class		NStoreOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class CORELIB_API NStoreOp : public NOperatorFx
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
	virtual udword Process(float _ftime, SEngineState& _state);

};

//-----------------------------------------------------------------
//!	\class		NOutputOp
//!	\brief		Store final result operator
//-----------------------------------------------------------------
class CORELIB_API NOutputOp : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NOutputOp();
	virtual ~NOutputOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x000000FF;	}
	virtual const char*		GetName()			{ return "Store Result";	}	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Misc";	}					//!< Operator's Category
	virtual const char*		GetUserName();														//!< Operator's User Name

	//Bitmap Access
	N2DBitmap* GetBitmap();

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);

};

//-----------------------------------------------------------------
//!	\class		NLoadOp
//!	\brief		Load a stored operator
//-----------------------------------------------------------------
class CORELIB_API NLoadOp : public NOperatorFx
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
	virtual udword Process(float _ftime, SEngineState& _state);

};


//-----------------------------------------------------------------
//!	\class		NChannelAnimFX1Op
//!	\brief		Channel animation operator
//-----------------------------------------------------------------
class CORELIB_API NChannelAnimFX1Op : public NOperatorFx
{
public:
	FDECLARE_CLASS();

	NChannelAnimFX1Op();

	//Methods
	virtual	udword	GetColor()		{ return 0x00e36464;	}
	virtual const char*		GetName()			{ return "Channel FX1"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Animation"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, SEngineState& _state);

};

#endif //MAINOPS_H
