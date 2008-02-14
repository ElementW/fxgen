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

//! also thanks to libnoise by Jason Bevins
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------

#include "EngineOp.h"
#include "../noise/src/noise.h"
#include "noiseutils.h"

#ifdef GetUserName
#undef GetUserName
#endif

//-----------------------------------------------------------------
//                   Generators
//	NPerlin
//	NBillow
//	NRidgedMulti
//	NCheckerboard
//	NConst
//	NCylinders
//	NSpheres
//	NVoronoi
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NNopOp2
//!	\brief		Perlin noise operator
//-----------------------------------------------------------------
class FXGEN_API NPerlin : public NOperator
{
public:
	DECLARE_CLASS();

	NPerlin();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Perlin"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,float _fDetailFactor = 1.0);

	noise::module::Perlin m_module;
};

//-----------------------------------------------------------------
//!	\class		NPerlinOctave
//!	\brief		Perlin noise operator
//-----------------------------------------------------------------
class FXGEN_API NPerlinOctave : public NOperator
{
public:
	DECLARE_CLASS();

	NPerlinOctave();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_PerlinOctave"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,float _fDetailFactor = 1.0);

	noise::module::PerlinOctave m_module;
};

//-----------------------------------------------------------------
//!	\class		NBillow
//!	\brief		Billow operator
//-----------------------------------------------------------------
class FXGEN_API NBillow : public NOperator
{
public:
	DECLARE_CLASS();

	NBillow();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Billow"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,float _fDetailFactor = 1.0);

	noise::module::Billow m_module;
};

//-----------------------------------------------------------------
//!	\class		NRidgedMulti
//!	\brief		NRidgedMulti operator
//-----------------------------------------------------------------
class FXGEN_API NRidgedMulti : public NOperator
{
public:
	DECLARE_CLASS();

	NRidgedMulti();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_RidgedMulti"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,float _fDetailFactor = 1.0);

	noise::module::RidgedMulti m_module;
};

//-----------------------------------------------------------------
//!	\class		NCheckerboard
//!	\brief		Checkerboard operator
//-----------------------------------------------------------------
class FXGEN_API NCheckerboard : public NOperator
{
public:
	DECLARE_CLASS();

	NCheckerboard();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Checkerboard"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,float _fDetailFactor = 1.0);

	noise::module::Checkerboard m_module;
};

//-----------------------------------------------------------------
//!	\class		NConst
//!	\brief		Const operator
//-----------------------------------------------------------------
class FXGEN_API NConst : public NOperator
{
public:
	DECLARE_CLASS();

	NConst();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Const"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,float _fDetailFactor = 1.0);

	noise::module::Const m_module;
};

//-----------------------------------------------------------------
//!	\class		NCylinders
//!	\brief		Cylinders operator
//-----------------------------------------------------------------
class FXGEN_API NCylinders : public NOperator
{
public:
	DECLARE_CLASS();

	NCylinders();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Cylinders"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,float _fDetailFactor = 1.0);

	noise::module::Cylinders m_module;
};


//-----------------------------------------------------------------
//!	\class		NSpheres
//!	\brief		Spheres operator
//-----------------------------------------------------------------
class FXGEN_API NSpheres : public NOperator
{
public:
	DECLARE_CLASS();

	NSpheres();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Spheres"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts,float _fDetailFactor = 1.0);

	noise::module::Spheres m_module;
};


//-----------------------------------------------------------------
//!	\class		NVoronoi
//!	\brief		Voronoi operator
//-----------------------------------------------------------------
class FXGEN_API NVoronoi : public NOperator
{
public:
	DECLARE_CLASS();

	NVoronoi();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Voronoi"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Voronoi m_module;
};


//-----------------------------------------------------------------
//!	\class		NHybridMulti
//!	\brief		NHybridMulti operator
//-----------------------------------------------------------------
class FXGEN_API NHybridMulti : public NOperator
{
public:
	DECLARE_CLASS();

	NHybridMulti();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_HybridMulti"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::HybridMulti m_module;
};

//-----------------------------------------------------------------
//!	\class		NHeteroTerrain
//!	\brief		NHeteroTerrain operator
//-----------------------------------------------------------------
class FXGEN_API NHeteroTerrain : public NOperator
{
public:
	DECLARE_CLASS();

	NHeteroTerrain();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_HeteroTerrain"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::HeteroTerrain m_module;
};

//-----------------------------------------------------------------
//!	\class		NMultiFractal
//!	\brief		NMultiFractal operator
//-----------------------------------------------------------------
class FXGEN_API NMultiFractal : public NOperator
{
public:
	DECLARE_CLASS();

	NMultiFractal();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_MultiFractal"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::MultiFractal m_module;
};

//-----------------------------------------------------------------
//!	\class		NFbm
//!	\brief		NFbm operator
//-----------------------------------------------------------------
class FXGEN_API NFbm : public NOperator
{
public:
	DECLARE_CLASS();

	NFbm();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Fbm"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Fbm m_module;
};

//-----------------------------------------------------------------
//!	\class		NTurbulenceGenerator
//!	\brief		NTurbulenceGenerator operator
//-----------------------------------------------------------------
class FXGEN_API NTurbulenceGenerator : public NOperator
{
public:
	DECLARE_CLASS();

	NTurbulenceGenerator();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_TurbulenceGen"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Generator"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::TurbulenceGenerator m_module;
};

//-----------------------------------------------------------------
//                   Modifiers
//	NAbs
//	NClamp
//	NCurve
//	NExponent
//	NInvert
//	NScaleBias
//	NTerrace
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NAbs
//!	\brief		Abs operator
//-----------------------------------------------------------------
class FXGEN_API NAbs : public NOperator
{
public:
	DECLARE_CLASS();

	NAbs();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Abs"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Modifier"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Abs m_module;
};

//-----------------------------------------------------------------
//!	\class		NClamp
//!	\brief		Clamp operator
//-----------------------------------------------------------------
class FXGEN_API NClamp : public NOperator
{
public:
	DECLARE_CLASS();

	NClamp();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Clamp"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Modifier"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Clamp m_module;
};

//-----------------------------------------------------------------
//!	\class		NCurve
//!	\brief		Curve operator
//-----------------------------------------------------------------
class FXGEN_API NCurve : public NOperator
{
public:
	DECLARE_CLASS();

	NCurve();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Curve"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Modifier"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Curve m_module;
};

//-----------------------------------------------------------------
//!	\class		NExponent
//!	\brief		Exponent operator
//-----------------------------------------------------------------
class FXGEN_API NExponent : public NOperator
{
public:
	DECLARE_CLASS();

	NExponent();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Exponent"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Modifier"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Exponent m_module;
};


//-----------------------------------------------------------------
//!	\class		NInvert
//!	\brief		Invert operator
//-----------------------------------------------------------------
class FXGEN_API NInvert : public NOperator
{
public:
	DECLARE_CLASS();

	NInvert();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Invert"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Modifier"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Invert m_module;
};

//-----------------------------------------------------------------
//!	\class		NScaleBias
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NScaleBias : public NOperator
{
public:
	DECLARE_CLASS();

	NScaleBias();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_ScaleBias"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Modifier"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::ScaleBias m_module;
};


//-----------------------------------------------------------------
//!	\class		NTerrace
//!	\brief		Terrace operator
//-----------------------------------------------------------------
class FXGEN_API NTerrace : public NOperator
{
public:
	DECLARE_CLASS();

	NTerrace();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Terrace"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Modifier"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Terrace m_module;
};

//-----------------------------------------------------------------
//!	\class		NCraterSpheres
//!	\brief		Terrace operator
//-----------------------------------------------------------------
class FXGEN_API NCraterSpheres : public NOperator
{
public:
	DECLARE_CLASS();

	NCraterSpheres();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_CraterSpheres"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Modifier"; }	//!< Operator's Category


	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::CraterSpheres m_module;
};


//-----------------------------------------------------------------
//                   Selector
//	NSelect
//	NBlend
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//!	\class		NSelect
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NSelect : public NOperator
{
public:
	DECLARE_CLASS();

	NSelect();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Select"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Selector"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Select m_module;
};

//-----------------------------------------------------------------
//!	\class		NSelect
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NBlend : public NOperator
{
public:
	DECLARE_CLASS();

	NBlend();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Blend"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Selector"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Blend m_module;
};

//-----------------------------------------------------------------
//                   Combiners
//	NAdd
//	NMax
//	NMin
//	NMulitply
//	NPower
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NAdd
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NAdd : public NOperator
{
public:
	DECLARE_CLASS();

	NAdd();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Add"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combiners"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Add m_module;
};

//-----------------------------------------------------------------
//!	\class		NAdd
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NMax : public NOperator
{
public:
	DECLARE_CLASS();

	NMax();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Max"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combiners"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Max m_module;
};

//-----------------------------------------------------------------
//!	\class		NMin
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NMin : public NOperator
{
public:
	DECLARE_CLASS();

	NMin();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Min"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combiners"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Min m_module;
};


//-----------------------------------------------------------------
//!	\class		NMulitply
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NMultiply : public NOperator
{
public:
	DECLARE_CLASS();

	NMultiply();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Multiply"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combiners"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Multiply m_module;
};

//-----------------------------------------------------------------
//!	\class		NMulitply
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NPower : public NOperator
{
public:
	DECLARE_CLASS();

	NPower();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Power"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Combiners"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Power m_module;
};

//-----------------------------------------------------------------
//                   Transformers
//	NDisplace
//	NRotatePoint
//	NScalePoint
//	NTranslatePoint
//	NTurbulence
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NDisplace
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NDisplace: public NOperator
{
public:
	DECLARE_CLASS();

	NDisplace();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Displace"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Transformer"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Displace m_module;
};

//-----------------------------------------------------------------
//!	\class		NRotatePoint
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NRotatePoint: public NOperator
{
public:
	DECLARE_CLASS();

	NRotatePoint();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_RotatePoint"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Transformer"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::RotatePoint m_module;
};

//-----------------------------------------------------------------
//!	\class		NScalePoint
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NScalePoint: public NOperator
{
public:
	DECLARE_CLASS();

	NScalePoint();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_ScalePoint"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Transformer"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::ScalePoint m_module;
};

//-----------------------------------------------------------------
//!	\class		NTranslatePoint
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NTranslatePoint: public NOperator
{
public:
	DECLARE_CLASS();

	NTranslatePoint();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_TranslatePoint"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Transformer"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::TranslatePoint m_module;
};

//-----------------------------------------------------------------
//!	\class		NTurbulence
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NTurbulence: public NOperator
{
public:
	DECLARE_CLASS();

	NTurbulence();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Turbulence"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Transformer"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Turbulence m_module;
};


//-----------------------------------------------------------------
//                   Libnoise Misc
//	NCache
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NCache
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NCache: public NOperator
{
public:
	DECLARE_CLASS();

	NCache();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Cache"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Libnoise Misc"; }	//!< Operator's Category

	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	char* GetUserName();

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);


	noise::module::Cache m_module;
};


//-----------------------------------------------------------------
//!	\class		NCache
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NLoadCache: public NOperator
{
public:
	DECLARE_CLASS();

	NLoadCache();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_LoadCache"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Libnoise Misc"; }	//!< Operator's Category
	noise::module::Module* GetLibnoiseModule() { return &m_module; }
	char* GetUserName();

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::module::Cache m_module;
};

class NModelPlane;
class NModelCylinder;
//class NModelLine;
class NModelSphere;

//-----------------------------------------------------------------
//                   Models
//	NModelPlane
//	NModelCylinder
//	NModelSphere
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NModelPlane
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NModelPlane : public NOperator
{
public:
	DECLARE_CLASS();

	NModelPlane();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Plane"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Models"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);
	noise::utils::NoiseMap* GetNoiseMap(){ return &m_heightMap; }

	noise::utils::NoiseMap				m_heightMap;
	noise::utils::NoiseMapBuilderPlane	m_heightMapBuilder;
};

//-----------------------------------------------------------------
//!	\class		NModelPlane
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NModelPlane2 : public NOperator
{
public:
	DECLARE_CLASS();

	NModelPlane2();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Plane2"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Models"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);
	noise::utils::NoiseMap* GetNoiseMap(){ return &m_heightMap; }

	noise::utils::NoiseMap						m_heightMap;
	noise::utils::NoiseMapBuilderDepthPlane		m_heightMapBuilder;
};

//-----------------------------------------------------------------
//!	\class		NModelCylinder
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NModelCylinder : public NOperator
{
public:
	DECLARE_CLASS();

	NModelCylinder();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Cylinder"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Models"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);
	noise::utils::NoiseMap* GetNoiseMap(){ return &m_heightMap; }

	noise::utils::NoiseMap					m_heightMap;
	noise::utils::NoiseMapBuilderCylinder	m_heightMapBuilder;
};

/*
//-----------------------------------------------------------------
//!	\class		NModelLine
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class NModelLine : public NOperator
{
public:
	DECLARE_CLASS();

	NModelLine();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_NoisePlane"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Models"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);
	noise::utils::NoiseMapBuilder* GetMapBuilder() { return &heightMapBuilder; }

	//noise::utils::N heightMapBuilder;
};
*/

//-----------------------------------------------------------------
//!	\class		NModelSphere
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NModelSphere : public NOperator
{
public:
	DECLARE_CLASS();

	NModelSphere();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_Sphere"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Models"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);
	noise::utils::NoiseMap* GetNoiseMap(){ return &m_heightMap; }


	noise::utils::NoiseMap				m_heightMap;
	noise::utils::NoiseMapBuilderSphere m_heightMapBuilder;

};


//-----------------------------------------------------------------
//!	\class		NModelSphere
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NModelConcentricDepthSphere : public NOperator
{
public:
	DECLARE_CLASS();

	NModelConcentricDepthSphere();

	//Methods
	virtual	udword	GetColor()		{ return 0x00008600;	}
	virtual const char*		GetName()			{ return "L_DepthSphere"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Models"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);
	noise::utils::NoiseMap* GetNoiseMap(){ return &m_heightMap; }


	noise::utils::NoiseMap				m_heightMap;
	noise::utils::NoiseMapConcentricDepthSphere m_heightMapBuilder;

};

//-----------------------------------------------------------------
//                   Outputs
//	NImageRenderer
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//!	\class		NImageRenderer
//!	\brief		Inverts an image
//-----------------------------------------------------------------
class FXGEN_API NImageRenderer : public NOperator
{
public:
	DECLARE_CLASS();

	NImageRenderer();

	//Methods
	virtual	udword	GetColor()		{ return 0x000000FF;	}
	virtual const char*		GetName()			{ return "L_ImageRenderer"; }		//!< Operator's Name
	virtual const char*		GetCategory()	{ return "Render"; }	//!< Operator's Category

	//Processing methods
	virtual udword Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor = 1.0);

	noise::utils::RendererImage m_renderer;
};



