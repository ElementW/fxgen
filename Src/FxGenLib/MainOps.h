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
#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "EngineOp.h"

//-----------------------------------------------------------------
//!	\class		NStoreOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class NStoreOp : public NOperator
{
public:
	DECLARE_CLASS();

	NStoreOp();
	virtual ~NStoreOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x004080FF;	}
	virtual char*		GetName();

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};

//-----------------------------------------------------------------
//!	\class		NStoreResultOp
//!	\brief		Store final result operator
//-----------------------------------------------------------------
class NStoreResultOp : public NOperator
{
public:
	DECLARE_CLASS();

	NStoreResultOp();
	virtual ~NStoreResultOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x000000FF;	}
	virtual char*		GetName();

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};

//-----------------------------------------------------------------
//!	\class		NLoadOp
//!	\brief		Load a stored operator
//-----------------------------------------------------------------
class NLoadOp : public NOperator
{
public:
	DECLARE_CLASS();

	NLoadOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x00e36464;	}
	virtual char*		GetName();

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};


//-----------------------------------------------------------------
//!	\class		NChannelAnimFX1Op
//!	\brief		Channel animation operator
//-----------------------------------------------------------------
class NChannelAnimFX1Op : public NOperator
{
public:
	DECLARE_CLASS();

	NChannelAnimFX1Op();

	//Methods
	virtual	udword	GetColor()	{ return 0x00e36464;	}
	virtual char*		GetName()		{ return "Channel FX1"; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};
