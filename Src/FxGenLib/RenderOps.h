//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		RenderOps.h
//! \brief	Render Operators
//!
//!	\author	Johann Nadalutti (fxgen@free.fr)
//!					Anders Stenberg (anders.stenberg@gmail.com)
//!
//!	\date		07-05-2007
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
//		NFlatOp
//		NCloudOp
//		NGradientOp
//		NCellOp

//-----------------------------------------------------------------
//!	\class		NFlatOp
//!	\brief		Flat rendering
//-----------------------------------------------------------------
class FXGEN_API NFlatOp : public NOperator
{
public:
	DECLARE_CLASS();

	NFlatOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Flat"; }			//!< Operator's Name
	virtual char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);
};

//-----------------------------------------------------------------
//!	\class		NCloudOp
//!	\brief		Render clouds
//-----------------------------------------------------------------
class FXGEN_API NCloudOp : public NOperator
{
public:
	DECLARE_CLASS();

	NCloudOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Cloud"; }		//!< Operator's Name
	virtual char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

	//Methods
	void	Cloud(ubyte byoctaves, float fampMod);
	void	AddRandom(float fampMod);
	void	ImageResize(udword w, udword h);
	void	BoxBlur(udword bw, udword bh);
	void	Normalize();

	//Datas
	udword m_dwWidth, m_dwHeight;	//!< Temporary image size
	sdword* m_pbyPxTps1, *m_pbyPxTps2;	//!< the two Temporary buffers
	sdword* m_pbyPxTps;	//!< pointeur for memfree

};

//-----------------------------------------------------------------
//!	\class		NGradientOp
//!	\brief		Four corners gradient rendering
//-----------------------------------------------------------------
class FXGEN_API NGradientOp : public NOperator
{
public:
	DECLARE_CLASS();

	NGradientOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual char*		GetName()			{ return "Gradient"; }	//!< Operator's Name
	virtual char*		GetCategory()	{ return "Render"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts);

};

//-----------------------------------------------------------------
//!	\class		NCellOp
//!	\brief		Render cells
//-----------------------------------------------------------------
class FXGEN_API NCellOp : public NOperator
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

