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
//                   Forward declarations
//-----------------------------------------------------------------
namespace vgvm
{
	namespace cairo
	{
		class CairoContext;
	}
	class Program;
}

//-----------------------------------------------------------------
//                   Prototypes
//-----------------------------------------------------------------
//NObject
//	NOperator
//		NFlatOp
//		NCloudOp
//		NGradientOp
//		NCellOp
//		NNoiseOp

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
	virtual const char*		GetName()			{ return "Flat"; }			//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
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
	virtual const char*		GetName()			{ return "Cloud"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

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
	virtual const char*		GetName()			{ return "Gradient"; }	//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }		//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

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
	virtual const char*		GetName()			{ return "Cell"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NNoiseOp
//!	\brief		Render noise
//-----------------------------------------------------------------
class FXGEN_API NNoiseOp : public NOperator
{
public:
	DECLARE_CLASS();

	NNoiseOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Noise"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);
};

//-----------------------------------------------------------------
//!	\class		NVectorOp
//!	\brief		Render vector graphics
//-----------------------------------------------------------------
class FXGEN_API NVectorOp : public NOperator
{
public:
	DECLARE_CLASS();

	NVectorOp();
	~NVectorOp();

	//Methods
	virtual	udword	GetColor()		{ return 0x009696c0;	}
	virtual const char*		GetName()			{ return "Vector"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	void			SetVectorData(ubyte* _data, udword _length);

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor);

private:
	vgvm::cairo::CairoContext*	m_pContext;
	vgvm::Program*			m_pProgram;
};
