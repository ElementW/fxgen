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
//                   Macros
//-----------------------------------------------------------------
#ifndef STATIC_BUILD
#ifdef FXGEN_EXPORTS
	#define FXGEN_API __declspec(dllexport)
#else
	#define FXGEN_API __declspec(dllimport)
#endif
#else // STATIC_BUILD
#define FXGEN_API
#endif // STATIC_BUILD

//-----------------------------------------------------------------
//!	\class		NStoreOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NStoreOp : public NOperator
{
public:
	DECLARE_CLASS();

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
class FXGEN_API NStoreResultOp : public NOperator
{
public:
	DECLARE_CLASS();

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
class FXGEN_API NLoadOp : public NOperator
{
public:
	DECLARE_CLASS();

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
class FXGEN_API NChannelAnimFX1Op : public NOperator
{
public:
	DECLARE_CLASS();

	NChannelAnimFX1Op();

	//Methods
	virtual	udword	GetColor()		{ return 0x00e36464;	}
	virtual const char*		GetName()			{ return "Channel FX1"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Animation"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

};


//-----------------------------------------------------------------
//!	\class		NStoreOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NFloatVarOp : public NOperator
{
public:
	DECLARE_CLASS();

	NFloatVarOp();
	virtual ~NFloatVarOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "Float Var";	}		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();		
	virtual float		GetUserVal();
	virtual void		AddFloatVar(float *v){ if( indx < 256 )val[indx++] = v; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	float *val[256];
	int indx;
	float value;

};

//-----------------------------------------------------------------
//!	\class		NIntVarOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NUWordVarOp : public NOperator
{
public:
	DECLARE_CLASS();

	NUWordVarOp();
	virtual ~NUWordVarOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "UDWord Var";	}		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();		
	virtual udword		GetUserVal();
	virtual void		AddUWordVar(udword *v){ if( indx < 256 )val[indx++] = v; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	udword *val[256];
	int indx;
	udword value;

};

//-----------------------------------------------------------------
//!	\class		NLoadOp
//!	\brief		Load a stored operator
//-----------------------------------------------------------------
class FXGEN_API NLoadVarOp : public NOperator
{
public:
	DECLARE_CLASS();

	NLoadVarOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "Load Var"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();	
	virtual float		GetUserVal();
	virtual void		AddFloatVar(float *v){ if( indx < 256 )val[indx++] = v; }


	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	float *val[256];
	int indx;
	float value;

};


//-----------------------------------------------------------------
//!	\class		NVarsSumOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NVarsSumOp : public NOperator
{
public:
	DECLARE_CLASS();

	NVarsSumOp();
	virtual ~NVarsSumOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "VARS SUM";	}		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();		
	virtual float		GetUserVal(){return value;}
	virtual void		AddFloatVar(float *v){ if( indx < 256 )val[indx++] = v; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	float *val[256];
	int indx;
	float value;
	float val_ptr;
};

//-----------------------------------------------------------------
//!	\class		NVarsSumOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NVarsSubtractOp : public NOperator
{
public:
	DECLARE_CLASS();

	NVarsSubtractOp();
	virtual ~NVarsSubtractOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "VARS SUB";	}		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();		
	virtual float		GetUserVal(){return value;}
	virtual void		AddFloatVar(float *v){ if( indx < 256 )val[indx++] = v; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	float *val[256];
	int indx;
	float value;
	float val_ptr;
};


//-----------------------------------------------------------------
//!	\class		NVarsSumOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NVarsDivideOp : public NOperator
{
public:
	DECLARE_CLASS();

	NVarsDivideOp();
	virtual ~NVarsDivideOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "VARS DIVIDE";	}		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();		
	virtual float		GetUserVal(){return value;}
	virtual void		AddFloatVar(float *v){ if( indx < 256 )val[indx++] = v; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	float *val[256];
	int indx;
	float value;
	float val_ptr;
};

//-----------------------------------------------------------------
//!	\class		NVarsSumOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NVarsMultiplyOp : public NOperator
{
public:
	DECLARE_CLASS();

	NVarsMultiplyOp();
	virtual ~NVarsMultiplyOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "VARS MULTIPLY";	}		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();		
	virtual float		GetUserVal(){return value;}
	virtual void		AddFloatVar(float *v){ if( indx < 256 )val[indx++] = v; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	float *val[256];
	int indx;
	float value;
	float val_ptr;
};

//-----------------------------------------------------------------
//!	\class		NVarsSumOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NVarsPowerOp : public NOperator
{
public:
	DECLARE_CLASS();

	NVarsPowerOp();
	virtual ~NVarsPowerOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "VARS POWER";	}		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();		
	virtual float		GetUserVal(){return value;}
	virtual void		AddFloatVar(float *v){ if( indx < 256 )val[indx++] = v; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	float *val[256];
	int indx;
	float value;
	float val_ptr;
};

//-----------------------------------------------------------------
//!	\class		NVarsSumOp
//!	\brief		Store operator
//-----------------------------------------------------------------
class FXGEN_API NVarsLogOp : public NOperator
{
public:
	DECLARE_CLASS();

	NVarsLogOp();
	virtual ~NVarsLogOp();

	//Methods
	virtual	COLORREF	GetColor()		{ return 0x00AFE0F5;	}
	virtual char*		GetName()			{ return "VARS LOG";	}		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Variables"; }		//!< Operator's Category
	virtual char*		GetUserName();		
	virtual float		GetUserVal(){return value;}
	virtual void		AddFloatVar(float *v){ if( indx < 256 )val[indx++] = v; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

	float *val[256];
	int indx;
	float value;
	float val_ptr;
};


