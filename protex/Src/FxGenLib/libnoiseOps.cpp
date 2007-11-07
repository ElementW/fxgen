#include "pch.h"

#include "libnoiseOps.h"
//#include "resource.h"
#include <windows.h>
#include <Commdlg.h>

// I need to use the NArray<T> object here
#include "../GUILib/gui.h"
//#include "gui.h"
//#include "PropertyItems.h"
//#include "JPEGLoader.h"
#include "../noise/src/noise.h"
#include "noiseutils.h"

using namespace noise;
using namespace noise::module;
using namespace noise::utils;

#ifdef GetUserName
#undef GetUserName
#endif

//NGradientDlg grad_dlg(IDD_ENTERNAME, 0);

//-----------------------------------------------------------------
//                   Generators
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							Perlin class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NPerlin,	NOperator);

static NVarsBlocDesc blocdescFlatOp2[] =
{
	VAR(eudword,	true, "Num Octaves",	"6",		"NUwordProp")	//0
	VAR(eudword,		true, "Seed",			"0",		"NUwordProp")	//1
	VAR(eudword,		true, "Type",			"0",		"NUwordProp")	//2
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
	VAR(efloat,		true, "Persistance",			"0.5",	"NFloatProp")	//4
	VAR(efloat,		true, "Lacunarity",			"2.0",	"NFloatProp")	//5
	VAR(eubyte,		true, "Quality",			"1",	"NUwordProp")	//6
};

NPerlin::NPerlin()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescFlatOp2);
}

udword NPerlin::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	int octaves = 6;
	int seed = 0;
	int quality = 1;
	float frequency = 1.0f;
	float persistance = 0.5f;
	float Lacunarity = 2.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)octaves);
	m_pcvarsBloc->GetValue(1, _ftime, (uword&)seed);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)persistance);
	m_pcvarsBloc->GetValue(5, _ftime, (float&)Lacunarity);
	m_pcvarsBloc->GetValue(6, _ftime, (uword&)quality);

	octaves = (int)nv_clamp( octaves, 1, 30.0 );
	frequency = nv_clamp( frequency, 0, 50.0 );
	quality = (int)nv_clamp( quality, 0, 2.0 );


	//noise::module::Perlin myModule;
	m_module.SetOctaveCount(octaves);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);
	m_module.SetPersistence (persistance);
	m_module.SetLacunarity (Lacunarity);
	m_module.SetNoiseQuality (noise::NoiseQuality(quality));

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NNopOp2 class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NBillow,	NOperator);

static NVarsBlocDesc blocdescBillow[] =
{
	VAR(eudword,	true, "Num Octaves",	"6",		"NUwordProp")	//0
	VAR(eudword,		true, "Seed",			"0",		"NUwordProp")	//1
	VAR(eudword,		true, "Type",			"0",		"NUwordProp")	//2
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
	VAR(efloat,		true, "Persistance",			"0.5",	"NFloatProp")	//4
	VAR(efloat,		true, "Lacunarity",			"2.0",	"NFloatProp")	//5
	VAR(eubyte,		true, "Quality",			"1",	"NUwordProp")	//6
};

NBillow::NBillow()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescBillow);
}

udword NBillow::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	int octaves = 6;
	int seed = 0;
	int quality = 1;
	float frequency = 1.0f;
	float persistance = 0.5f;
	float Lacunarity = 2.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)octaves);
	m_pcvarsBloc->GetValue(1, _ftime, (uword&)seed);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)persistance);
	m_pcvarsBloc->GetValue(5, _ftime, (float&)Lacunarity);
	m_pcvarsBloc->GetValue(6, _ftime, (uword&)quality);

	octaves = (int)nv_clamp( octaves, 1, 30.0 );
	frequency = nv_clamp( frequency, 0, 50.0 );
	quality = (int)nv_clamp( quality, 0, 2.0 );


	//noise::module::NBillow myModule;
	m_module.SetOctaveCount(octaves);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);
	m_module.SetPersistence (persistance);
	m_module.SetLacunarity (Lacunarity);
	m_module.SetNoiseQuality (noise::NoiseQuality(quality));

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRidgedMulti class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NRidgedMulti,	NOperator);

static NVarsBlocDesc blocdescNRidgedMulti[] =
{
	VAR(eudword,	true, "Num Octaves",	"1",		"NUwordProp")	//0
	VAR(eudword,		true, "Seed",			"1",		"NUwordProp")	//1
	VAR(eudword,		true, "Type",			"0",		"NUwordProp")	//2
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
	VAR(efloat,		true, "Lacunarity",			"2.0",	"NFloatProp")	//4
	VAR(eubyte,		true, "Quality",			"1",	"NUwordProp")	//5
};

NRidgedMulti::NRidgedMulti()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescNRidgedMulti);
}

udword NRidgedMulti::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	int octaves = 1;
	int seed = 1;
	int quality = 1;
	float frequency = 1.0f;
	float persistance = 1.0f;
	float Lacunarity = 2.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)octaves);
	m_pcvarsBloc->GetValue(1, _ftime, (uword&)seed);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)Lacunarity);
	m_pcvarsBloc->GetValue(5, _ftime, (uword&)quality);

	octaves = (int)nv_clamp( octaves, 1, 30.0 );
	frequency = nv_clamp( frequency, 0, 50.0 );
	quality = (int)nv_clamp( quality, 0, 2.0 );

	//noise::module::NBillow myModule;
	m_module.SetOctaveCount(octaves);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);

	m_module.SetLacunarity (Lacunarity);
	m_module.SetNoiseQuality (noise::NoiseQuality(quality));

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCheckerboard class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NCheckerboard,	NOperator);

NCheckerboard::NCheckerboard()
{
}

udword NCheckerboard::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NConst class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NConst,	NOperator);

static NVarsBlocDesc blocdescNConst[] =
{
	VAR(efloat,		true, "value",			"0.0",	"NFloatProp")	//4
};

NConst::NConst()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescNConst);
}

udword NConst::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float val = 0.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)val);

	m_module.SetConstValue(val);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCylinders class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NCylinders,	NOperator);

static NVarsBlocDesc blocdescNCylinders[] =
{
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
};

NCylinders::NCylinders()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescNCylinders);
}

udword NCylinders::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float frequency = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)frequency);

	frequency = nv_clamp( frequency, 0, 50.0 );

	m_module.SetFrequency (frequency /* 4.34375*/);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NSpheres class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NSpheres,	NOperator);

static NVarsBlocDesc blocdescNSpheres[] =
{
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
};

NSpheres::NSpheres()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(1, blocdescNSpheres);
}

udword NSpheres::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float frequency = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)frequency);

	frequency = nv_clamp( frequency, 0, 50.0 );

	m_module.SetFrequency (frequency /* 4.34375*/);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NVoronoi class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NVoronoi,	NOperator);

static NVarsBlocDesc blocdescNVoronoi[] =
{
	VAR(eubyte,		true, "Enable Distance",		"1",			"NUbyteProp")	//2
	VAR(efloat,		true, "Displacement",			"1.0",	"NFloatProp")	//3
	VAR(eudword,		true, "Seed",			"1",		"NUwordProp")	//1
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3

};

NVoronoi::NVoronoi()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(4, blocdescNVoronoi);
}

udword NVoronoi::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	ubyte bEnableDistance = true;
	float displacement = 1.0f;
	int seed = 1;
	float frequency = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (ubyte&)bEnableDistance);

	m_pcvarsBloc->GetValue(1, _ftime, (float&)displacement);
	m_pcvarsBloc->GetValue(2, _ftime, (uword&)seed);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);

	m_module.EnableDistance(bEnableDistance);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);
	m_module.SetDisplacement (displacement);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRidgedMulti class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NHybridMulti,	NOperator);

static NVarsBlocDesc blocdescNHybridMulti[] =
{
	VAR(eudword,	true, "Num Octaves",	"1",		"NUwordProp")	//0
	VAR(efloat,		true, "Gain",			"1.0",		"NFloatProp")	//1
	VAR(efloat,		true, "offset",			"0.0",		"NFloatProp")	//2
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
	VAR(efloat,		true, "Lacunarity",			"2.0",	"NFloatProp")	//4
	VAR(eubyte,		true, "Quality",			"1",	"NUwordProp")	//5
	VAR(eudword,		true, "Seed",			"1",	"NUwordProp")	//6
};

NHybridMulti::NHybridMulti()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescNHybridMulti);
}

udword NHybridMulti::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	int octaves = 1.0f;
	int seed = 1;
	float gain = 1.0f;
	float offset = 1.0f;
	int quality = 1;
	float frequency = 1.0f;
	float persistance = 1.0f;
	float Lacunarity = 2.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)octaves);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)gain);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)offset);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)Lacunarity);
	m_pcvarsBloc->GetValue(5, _ftime, (uword&)quality);
	m_pcvarsBloc->GetValue(6, _ftime, (udword&)seed);

	octaves = (int)nv_clamp( octaves, 1, 30.0 );
	frequency = nv_clamp( frequency, 0, 50.0 );
	quality = (int)nv_clamp( quality, 0, 2.0 );

	//noise::module::NBillow myModule;
	m_module.SetOctaveCount(octaves);
	m_module.SetGain(gain);
	m_module.SetOffset(offset);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);
	m_module.SetLacunarity (Lacunarity);

	m_module.SetNoiseQuality (noise::NoiseQuality(quality));

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRidgedMulti class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NHeteroTerrain,	NOperator);

static NVarsBlocDesc blocdescNHeteroTerrain[] =
{
	VAR(eudword,	true, "Num Octaves",	"1",		"NUwordProp")	//0
	VAR(efloat,		true, "Gain",			"1.0",		"NFloatProp")	//1
	VAR(efloat,		true, "offset",			"0.0",		"NFloatProp")	//2
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
	VAR(efloat,		true, "Lacunarity",			"2.0",	"NFloatProp")	//4
	VAR(eubyte,		true, "Quality",			"1",	"NUwordProp")	//5
		VAR(eudword,		true, "Seed",			"1",	"NUwordProp")	//6

};

NHeteroTerrain::NHeteroTerrain()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescNHeteroTerrain);
}

udword NHeteroTerrain::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	int octaves = 1.0f;
	int seed = 1;
	float gain = 1.0f;
	float offset = 1.0f;
	int quality = 1;
	float frequency = 1.0f;
	float persistance = 1.0f;
	float Lacunarity = 2.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)octaves);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)gain);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)offset);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)Lacunarity);
	m_pcvarsBloc->GetValue(5, _ftime, (uword&)quality);
	m_pcvarsBloc->GetValue(6, _ftime, (udword&)seed);

	octaves = (int)nv_clamp( octaves, 1, 30.0 );
	frequency = nv_clamp( frequency, 0, 50.0 );
	quality = (int)nv_clamp( quality, 0, 2.0 );

	//noise::module::NBillow myModule;
	m_module.SetOctaveCount(octaves);
	m_module.SetGain(gain);
	m_module.SetOffset(offset);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);
	m_module.SetLacunarity (Lacunarity);

	m_module.SetNoiseQuality (noise::NoiseQuality(quality));

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRidgedMulti class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NMultiFractal,	NOperator);

static NVarsBlocDesc blocdescNMultiFractal[] =
{
	VAR(eudword,	true, "Num Octaves",	"1",		"NUwordProp")	//0
	VAR(efloat,		true, "Gain",			"1.0",		"NFloatProp")	//1
	VAR(efloat,		true, "offset",			"0.0",		"NFloatProp")	//2
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
	VAR(efloat,		true, "Lacunarity",			"2.0",	"NFloatProp")	//4
	VAR(eubyte,		true, "Quality",			"1",	"NUwordProp")	//5
	VAR(eudword,		true, "Seed",			"1",	"NUwordProp")	//6
};

NMultiFractal::NMultiFractal()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescNMultiFractal);
}

udword NMultiFractal::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	int octaves = 1.0f;
	int seed = 1;
	float gain = 1.0f;
	float offset = 1.0f;
	int quality = 1;
	float frequency = 1.0f;
	float persistance = 1.0f;
	float Lacunarity = 2.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)octaves);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)gain);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)offset);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)Lacunarity);
	m_pcvarsBloc->GetValue(5, _ftime, (uword&)quality);
	m_pcvarsBloc->GetValue(6, _ftime, (udword&)seed);

	octaves = (int)nv_clamp( octaves, 1, 30.0 );
	frequency = nv_clamp( frequency, 0, 50.0 );
	quality = (int)nv_clamp( quality, 0, 2.0 );

	//noise::module::NBillow myModule;
	m_module.SetOctaveCount(octaves);
	m_module.SetGain(gain);
	m_module.SetOffset(offset);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);
	m_module.SetLacunarity (Lacunarity);

	m_module.SetNoiseQuality (noise::NoiseQuality(quality));

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRidgedMulti class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NFbm,	NOperator);

static NVarsBlocDesc blocdescNFbm[] =
{
	VAR(eudword,	true, "Num Octaves",	"1",		"NUwordProp")	//0
	VAR(efloat,		true, "Gain",			"1.0",		"NFloatProp")	//1
	VAR(efloat,		true, "offset",			"0.0",		"NFloatProp")	//2
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
	VAR(efloat,		true, "Lacunarity",			"2.0",	"NFloatProp")	//4
	VAR(eubyte,		true, "Quality",			"1",	"NUwordProp")	//5
	VAR(eudword,		true, "Seed",			"1",	"NUwordProp")	//6

};

NFbm::NFbm()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(7, blocdescNFbm);
}

udword NFbm::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	int octaves = 1.0f;
	int seed = 1;
	float gain = 1.0f;
	float offset = 1.0f;
	int quality = 1;
	float frequency = 1.0f;
	float persistance = 1.0f;
	float Lacunarity = 2.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)octaves);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)gain);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)offset);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)Lacunarity);
	m_pcvarsBloc->GetValue(5, _ftime, (uword&)quality);
	m_pcvarsBloc->GetValue(6, _ftime, (udword&)seed);

	octaves = (int)nv_clamp( octaves, 1, 30.0 );
	frequency = nv_clamp( frequency, 0, 50.0 );
	quality = (int)nv_clamp( quality, 0, 2.0 );

	//noise::module::NBillow myModule;
	m_module.SetOctaveCount(octaves);
	m_module.SetGain(gain);
	m_module.SetOffset(offset);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);
	m_module.SetLacunarity (Lacunarity);

	m_module.SetNoiseQuality (noise::NoiseQuality(quality));

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NTurbulenceGenerator class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NTurbulenceGenerator,	NOperator);

static NVarsBlocDesc blocdescNTurbulenceGenerator[] =
{
	VAR(efloat,	true, "Num Octaves",	"1",		"NFloatProp")	//0
	VAR(efloat,		true, "Gain",			"1.0",		"NFloatProp")	//1
	VAR(efloat,		true, "offset",			"0.0",		"NFloatProp")	//2
	VAR(efloat,		true, "Frequency",			"1.0",	"NFloatProp")	//3
	VAR(efloat,		true, "Lacunarity",			"2.0",	"NFloatProp")	//4
	VAR(eubyte,		true, "Quality",			"1",	"NUwordProp")	//5
	VAR(eudword,		true, "Seed",			"1",	"NUwordProp")	//6
	VAR(efloat,		true, "Exponent",			"1.0",	"NFloatProp")	//7

};

NTurbulenceGenerator::NTurbulenceGenerator()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(8, blocdescNTurbulenceGenerator);
}

udword NTurbulenceGenerator::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float octaves = 1.0f;
	int seed = 1;
	float gain = 1.0f;
	float offset = 1.0f;
	int quality = 1;
	float frequency = 1.0f;
	float persistance = 1.0f;
	float Lacunarity = 2.0f;
	float exponent = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)octaves);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)gain);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)offset);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)Lacunarity);
	m_pcvarsBloc->GetValue(5, _ftime, (uword&)quality);
	m_pcvarsBloc->GetValue(6, _ftime, (udword&)seed);
	m_pcvarsBloc->GetValue(7, _ftime, (float&)exponent);

	octaves = nv_clamp( octaves, 1, 30.0 );
	frequency = nv_clamp( frequency, 0, 50.0 );
	quality = (int)nv_clamp( quality, 0, 2.0 );

	//noise::module::NBillow myModule;
	m_module.SetOctaves(octaves);
	m_module.SetGain(gain);
	m_module.SetOffset(offset);
	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency /* 4.34375*/);
	m_module.SetLacunarity (Lacunarity);
	m_module.SetExponent(exponent);

	if( exponent != 1.0f )
	{
		m_module.ReCalcSpectralWeights();
	}


	m_module.SetNoiseQuality (noise::NoiseQuality(quality));

	return 0;
}

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
//-----------------------------------------------------------------
//
//							NAbs class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NAbs,	NOperator);

NAbs::NAbs()
{
}

udword NAbs::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	//module::ScaleBias smallSlime;
	m_module.SetSourceModule (0, *m);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NClamp class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NClamp,	NOperator);

static NVarsBlocDesc blocdescNClamp[] =
{
	VAR(efloat,		true, "min",			"0.0",	"NFloatProp")	//0
	VAR(efloat,		true, "max",			"1.0",	"NFloatProp")	//1

};

NClamp::NClamp()
{
	m_pcvarsBloc = AddVarsBloc(2, blocdescNClamp);
}

udword NClamp::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float cmin = 0.0f, cmax = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)cmin);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)cmax);

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	m_module.SetSourceModule (0, *m);
	m_module.SetBounds(cmin,cmax);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCurve class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NCurve,	NOperator);

static NVarsBlocDesc blocdescNCurve[] =
{
	VAR(erefobj2,		false, "CurveDialog",			"0",	"NCurveDialogProp")	//0

};

NCurve::NCurve()
{
	m_pcvarsBloc = AddVarsBloc(1, blocdescNCurve);
}

udword NCurve::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	m_module.ClearAllControlPoints();

	NArray<NDPoint> *pVarsObj = NULL;
	m_pcvarsBloc->GetValue(0, 0, (NObject*&)pVarsObj);

	if( pVarsObj != NULL )
	{
		NArray<NDPoint> *curveObj = ( NArray<NDPoint>* )pVarsObj;

		if( curveObj->Count() >= 4 )
		{
			for( int i = 0; i < curveObj->Count(); i++ )
			{
				float X1 = (*curveObj)[i].x;
				float Y1 = (*curveObj)[i].y;

				m_module.AddControlPoint( (double)X1, (double)Y1 );
			}
		}
	}

	void *ptr = NULL;
	m_pcvarsBloc->GetValue(0, 0, (NObject*&)ptr);

	m_module.SetSourceModule (0, *m);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NExponent class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NExponent,	NOperator);

static NVarsBlocDesc blocdescNExponent[] =
{
	VAR(efloat,		true, "Exponent",			"1.0",	"NFloatProp")	//0

};

NExponent::NExponent()
{
	m_pcvarsBloc = AddVarsBloc(1, blocdescNExponent);
}

udword NExponent::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float Exponent = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)Exponent);


	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	//module::ScaleBias smallSlime;
	m_module.SetExponent(Exponent);
	m_module.SetSourceModule (0, *m);


	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NInvert class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NInvert,	NOperator);

NInvert::NInvert()
{
}

udword NInvert::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	m_module.SetSourceModule (0, *m);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NScaleBias class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NScaleBias,	NOperator);

static NVarsBlocDesc blocdescNScaleBias[] =
{
	VAR(efloat,		true, "Scale",			"0.5",	"NFloatProp")	//0
	VAR(efloat,		true, "Bias",			"-0.5",	"NFloatProp")	//1

};

NScaleBias::NScaleBias()
{
	m_pcvarsBloc = AddVarsBloc(2, blocdescNScaleBias);
}

udword NScaleBias::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float scale = 0.5f, bias = -0.5f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)scale);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)bias);

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	//module::ScaleBias smallSlime;
	m_module.SetSourceModule (0, *m);
	m_module.SetScale (scale);
	m_module.SetBias (bias);

	return 0;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NTerrace class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NTerrace,	NOperator);

static NVarsBlocDesc blocdescNTerrace[] =
{
	VAR(erefobj2,		false, "TerraceDialog",			"0",	"NTerraceDialogProp")
};

NTerrace::NTerrace()
{
	m_pcvarsBloc = AddVarsBloc(1, blocdescNTerrace);
}

udword NTerrace::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	m_module.ClearAllControlPoints();

	NArray<NDPoint> *pVarsObj = NULL;
	m_pcvarsBloc->GetValue(0, 0, (NObject*&)pVarsObj);

	if( pVarsObj != NULL )
	{
		NArray<NDPoint> *curveObj = ( NArray<NDPoint>* )pVarsObj;

		if( curveObj->Count() >= 2 )
		{
			for( int i = 0; i < curveObj->Count(); i++ )
			{
				float X1 = (*curveObj)[i].x;
				m_module.AddControlPoint( (double)X1 );
			}
		}
	}

	void *ptr = NULL;
	m_pcvarsBloc->GetValue(0, 0, (NObject*&)ptr);


	m_module.SetSourceModule (0, *m);

	return 0;
}



//-----------------------------------------------------------------
//                   Transformers
//	NDisplace
//	NRotatePoint
//	NScalePoint
//	NTranslatePoint
//	NTurbulence
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NInvertOp2 class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NDisplace,	NOperator);

NDisplace::NDisplace()
{
}

udword NDisplace::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=4)		return (udword)-1;

	noise::module::Module *m0 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (0, *m0);
	_pOpsInts++;

	noise::module::Module *mX = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetXDisplaceModule( *mX);
	_pOpsInts++;

	noise::module::Module *mY = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetYDisplaceModule( *mY);
	_pOpsInts++;

	noise::module::Module *mZ = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetZDisplaceModule( *mZ);
	_pOpsInts++;

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NRotatePoint class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NRotatePoint,	NOperator);

static NVarsBlocDesc blocdescNRotatePoint[] =
{
	VAR(efloat,		true, "X angle",			"0.0",	"NFloatProp")	//0
	VAR(efloat,		true, "Y angle",			"0.0",	"NFloatProp")	//1
	VAR(efloat,		true, "Z angle",			"0.0",	"NFloatProp")	//2
};

NRotatePoint::NRotatePoint()
{
	m_pcvarsBloc = AddVarsBloc(4, blocdescNRotatePoint);
}

udword NRotatePoint::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float xangle = 0.0f, yangle = 0.0f, zangle = 0.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)xangle);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)yangle);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)zangle);

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	m_module.SetSourceModule (0, *m);

	m_module.SetAngles(xangle, yangle, zangle);

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NScalePoint class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NScalePoint,	NOperator);

static NVarsBlocDesc blocdescNScalePoint[] =
{
	VAR(efloat,		true, "X scale",			"1.0",	"NFloatProp")	//0
	VAR(efloat,		true, "Y scale",			"1.0",	"NFloatProp")	//1
	VAR(efloat,		true, "Z scale",			"1.0",	"NFloatProp")	//2
};

NScalePoint::NScalePoint()
{
	m_pcvarsBloc = AddVarsBloc(4, blocdescNScalePoint);
}

udword NScalePoint::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float sx = 1.0f, sy = 1.0f, sz = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)sx);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)sy);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)sz);

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	m_module.SetSourceModule (0, *m);

	m_module.SetScale(sx, sy, sz);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NTranslatePoint class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NTranslatePoint,	NOperator);

static NVarsBlocDesc blocdescNTranslatePoint[] =
{
	VAR(efloat,		true, "X trans",			"0.0",	"NFloatProp")	//0
	VAR(efloat,		true, "Y trans",			"0.0",	"NFloatProp")	//1
	VAR(efloat,		true, "Z trans",			"0.0",	"NFloatProp")	//2
};

NTranslatePoint::NTranslatePoint()
{
	m_pcvarsBloc = AddVarsBloc(4, blocdescNScalePoint);
}

udword NTranslatePoint::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	float x = 0.0f, y = 0.0f, z = 0.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)x);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)y);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)z);

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	m_module.SetSourceModule (0, *m);

	m_module.SetTranslation(x,y,z);


	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NTurbulence class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NTurbulence,	NOperator);

static NVarsBlocDesc blocdescNTurbulence[] =
{
	VAR(eudword,		true, "Seed",			"1",	"NUwordProp")	//0
	VAR(efloat,		true, "Frequency",			"8.0f",	"NFloatProp")	//1
	VAR(efloat,		true, "Power",			"1.0",	"NFloatProp")	//2
	VAR(eudword,		true, "Roughness",			"2.0",	"NUwordProp")	//3
};

NTurbulence::NTurbulence()
{
	m_pcvarsBloc = AddVarsBloc(4, blocdescNTurbulence);
}

udword NTurbulence::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if( !(*_pOpsInts) ) return (udword)-1;

	int seed = 1;
	float frequency = 8.0f;
	float power = 1.0 / 32.0;
	int roughness = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)seed);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)frequency);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)power);
	m_pcvarsBloc->GetValue(3, _ftime, (uword&)roughness);

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m != NULL  )
		m_module.SetSourceModule (0, *m);
	else
		return (udword)-1;

	m_module.SetSeed (seed);
	m_module.SetFrequency (frequency);
	m_module.SetPower (power);
	m_module.SetRoughness (roughness);

	return 0;
}


//-----------------------------------------------------------------
//                   Misc
//	NCache
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NCache class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NCache,	NOperator);

static NVarsBlocDesc blocdescNCacheOp[] =
{
	VAR(estring,	false, "Name", "",	"NStringProp")	//1
};

NCache::NCache()
{
	m_pcvarsBloc = AddVarsBloc(1, blocdescNCacheOp, 1);
}

char* NCache::GetUserName()
{
	char* pszname;
	m_pcvarsBloc->GetValue(0, 0, pszname);
	return pszname;
}

udword NCache::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	m_module.SetSourceModule (0, *m);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NLoadCache class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NLoadCache,	NOperator);

static NVarsBlocDesc blocdescNLoadCache[] =
{
	VAR(erefobj,	false, "Load",	"0", "NLoadLibnoiseCache")	//0
};

NLoadCache::NLoadCache()
{
	m_pcvarsBloc = AddVarsBloc(1, blocdescNLoadCache, 1);
}

char* NLoadCache::GetUserName()
{
		//Get Variables Values
	NOperator* popRef;
	m_pcvarsBloc->GetValue(0, 0, (NObject*&)popRef);

	//Process
	if (popRef)
		return popRef->GetUserName();

	return "";
}

udword NLoadCache::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	noise::module::Module *m = NULL;

	NOperator* popRef;
	m_pcvarsBloc->GetValue(0, _ftime, (NObject*&)popRef);

	if( !popRef )
		return (udword)-1;

	if( !strcmp(popRef->GetRTClass()->m_pszClassName, "NCache") )
	{
		NCache *cache = (NCache*)popRef;
		m = cache->GetLibnoiseModule();
	}
	else
	{
		return (udword)-1;
	}

	if( m == NULL )
		return (udword)-1;

	m_module.SetSourceModule (0, *m);

	return 0;
}

//-----------------------------------------------------------------
//                   Combiners
//	NAdd
//	NMax
//	NMin
//	NMulitply
//	NPower
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NAdd class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NAdd,	NOperator);

NAdd::NAdd()
{
}

udword NAdd::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	noise::module::Module *m1 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (0, *m1);
	_pOpsInts++;

	noise::module::Module *m2 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (1, *m2);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NAdd class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NMax,	NOperator);

NMax::NMax()
{
}

udword NMax::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	noise::module::Module *m1 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (0, *m1);
	_pOpsInts++;

	noise::module::Module *m2 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (1, *m2);

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NMin class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NMin,	NOperator);

NMin::NMin()
{
}

udword NMin::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	noise::module::Module *m1 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (0, *m1);
	_pOpsInts++;

	noise::module::Module *m2 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (1, *m2);

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NMultiply class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NMultiply,	NOperator);

NMultiply::NMultiply()
{
}

udword NMultiply::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	noise::module::Module *m1 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (0, *m1);
	_pOpsInts++;

	noise::module::Module *m2 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (1, *m2);


	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NMin class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NPower,	NOperator);

NPower::NPower()
{
}

udword NPower::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=2)		return (udword)-1;

	noise::module::Module *m1 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (0, *m1);
	_pOpsInts++;

	noise::module::Module *m2 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (1, *m2);

	return 0;
}

//-----------------------------------------------------------------
//                   Selector
//	NSelect
//	NBlend
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NInvertOp2 class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NSelect,	NOperator);

static NVarsBlocDesc blocdescNSelect[] =
{
	VAR(efloat,	true, "xmin",	"-0.375",		"NFloatProp")	//0
	VAR(efloat,		true, "xmax",			"0.375",		"NFloatProp")	//1
	VAR(efloat,		true, "edgeFalloff",			"0.5",		"NFloatProp")	//2
	VAR(estring,		true, "text entry",			"0.5",		"NStringProp")	//3
};

NSelect::NSelect()
{
	m_pcvarsBloc = AddVarsBloc(4, blocdescNSelect);
}

udword NSelect::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=3)		return 0;//(udword)-1;

	int seed = 1;
	float frequency = 8.0f;
	float power = 1.0 / 32.0;
	int roughness = 1.0f;
	float xmin=-0.375, xmax = 0.375;
	float edgeFalloff = 0.5;

	m_pcvarsBloc->GetValue(0, _ftime, (float&)xmin);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)xmax);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)edgeFalloff);

	noise::module::Module *m1 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (0, *m1);
	_pOpsInts++;
	noise::module::Module *m2 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetSourceModule (1, *m2);
	_pOpsInts++;
	noise::module::Module *m3 = (*_pOpsInts)->GetLibnoiseModule();
	m_module.SetControlModule (*m3);

	m_module.SetBounds (xmin, xmax);
	m_module.SetEdgeFalloff (edgeFalloff);

	return 0;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NBlend class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NBlend,	NOperator);

NBlend::NBlend()
{
}

udword NBlend::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	if (m_byInputs!=3)		return 0;//(udword)-1;


	noise::module::Module *m1 = (*_pOpsInts)->GetLibnoiseModule();
	if( m1 == NULL )
		return (udword)-1;
	m_module.SetSourceModule (0, *m1);


	if( (*_pOpsInts+1) )
	{
		noise::module::Module *m2 = (*_pOpsInts+1)->GetLibnoiseModule();
		if( m2 == NULL )
			return (udword)-1;
		m_module.SetSourceModule (1, *m2);
	}

	if( (*_pOpsInts+2) )
	{
		noise::module::Module *m3 = (*_pOpsInts+2)->GetLibnoiseModule();
		if( m3 == NULL )
			return (udword)-1;

		m_module.SetControlModule (*m3);
	}

	return 0;
}


class NModelPlane;
class NModelCylinder;
//class NModelLine;
class NModelSphere;


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NModelPlane class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NModelPlane, NOperator);

static NVarsBlocDesc blocdescNModelPlane[] =
{
	VAR(eudword,		true, "Width",			"512",	"NUwordProp")	//0
	VAR(eudword,		true, "Height",			"512",	"NUwordProp")	//1
	VAR(efloat,		true, "x1",			"-5.0",		"NFloatProp")	//2
	VAR(efloat,		true, "y1",			"-5.0",		"NFloatProp")	//3
	VAR(efloat,		true, "x2",			"5.0",	"NFloatProp")	//4
	VAR(efloat,		true, "y2",			"5.0",	"NFloatProp")	//5
};

NModelPlane::NModelPlane()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(6, blocdescNModelPlane);
}

udword NModelPlane::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	int sizeX = 1024, sizeY = 1024;
	float x1 = -5.0, y1 = -5.0;
	float x2 = 5.0, y2 = 5.0;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)sizeX);
	m_pcvarsBloc->GetValue(1, _ftime, (uword&)sizeY);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)x1);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)y1);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)x2);
	m_pcvarsBloc->GetValue(5, _ftime, (float&)y2);

	sizeX = (int)nv_clamp( sizeX, 32, 8192 );
	sizeY = (int)nv_clamp( sizeY, 32, 8192 );

	/*x1 = (int)nv_clamp( x1, -sizeX, 8192 );
	x2 = (int)nv_clamp( sizeY, 32, 8192 );
	y1 = (int)nv_clamp( sizeX, 32, 8192 );
	y2 = (int)nv_clamp( sizeY, 32, 8192 );*/

	m_heightMapBuilder.SetSourceModule (*m);
	m_heightMapBuilder.SetDestNoiseMap (m_heightMap);
	m_heightMapBuilder.SetDestSize (sizeX, sizeY);
	m_heightMapBuilder.SetBounds (x1, x2, y1, y2);
	m_heightMapBuilder.Build ();

	return 0;
}



//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NModelPlane class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NModelPlane2, NOperator);

static NVarsBlocDesc blocdescNModelPlane2[] =
{
	VAR(eudword,		true, "Width",			"512",	"NUwordProp")	//0
	VAR(eudword,		true, "Height",			"512",	"NUwordProp")	//1
	VAR(efloat,		true, "x1",			"-5.0",		"NFloatProp")	//2
	VAR(efloat,		true, "y1",			"-5.0",		"NFloatProp")	//3
	VAR(efloat,		true, "x2",			"5.0",	"NFloatProp")	//4
	VAR(efloat,		true, "y2",			"5.0",	"NFloatProp")	//5
	VAR(eudword,	false, "Tile",	"0", "NUwordProp")	//6
	VAR(efloat,		true, "Depth",			"0.0",		"NFloatProp")	//7
};

NModelPlane2::NModelPlane2()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(8, blocdescNModelPlane2);
}

udword NModelPlane2::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	int sizeX = 1024, sizeY = 1024;
	float x1 = -5.0, y1 = -5.0;
	float x2 = 5.0, y2 = 5.0;
	uword bTile = false;
	float depth = 0.0;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)sizeX);
	m_pcvarsBloc->GetValue(1, _ftime, (uword&)sizeY);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)x1);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)y1);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)x2);
	m_pcvarsBloc->GetValue(5, _ftime, (float&)y2);
	m_pcvarsBloc->GetValue(6, _ftime, (uword&)bTile);
	m_pcvarsBloc->GetValue(7, _ftime, (float&)depth);

	sizeX = (int)nv_clamp( sizeX, 32, 8192 );
	sizeY = (int)nv_clamp( sizeY, 32, 8192 );

	/*x1 = (int)nv_clamp( x1, -sizeX, 8192 );
	x2 = (int)nv_clamp( sizeY, 32, 8192 );
	y1 = (int)nv_clamp( sizeX, 32, 8192 );
	y2 = (int)nv_clamp( sizeY, 32, 8192 );*/

	m_heightMapBuilder.EnableSeamless(bTile);

	m_heightMapBuilder.SetDepth( depth );

	m_heightMapBuilder.SetSourceModule (*m);
	m_heightMapBuilder.SetDestNoiseMap (m_heightMap);
	m_heightMapBuilder.SetDestSize (sizeX, sizeY);
	m_heightMapBuilder.SetBounds (x1, x2, y1, y2);
	m_heightMapBuilder.Build ();

	return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NModelCylinder class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NModelCylinder, NOperator);

static NVarsBlocDesc blocdescNModelCylinder[] =
{
	VAR(eudword,		true, "Height",			"512",	"NUwordProp")	//1
	VAR(efloat,		true, "theta1",			"-180.0",		"NFloatProp")	//2
	VAR(efloat,		true, "theta2",			"180.0",		"NFloatProp")	//3
	VAR(efloat,		true, "h1",			"-1.0",	"NFloatProp")	//4
	VAR(efloat,		true, "h2",			"1.0",	"NFloatProp")	//5
};

NModelCylinder::NModelCylinder()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(5, blocdescNModelCylinder);
}

udword NModelCylinder::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	int height = 512;
	float theta1 = -180.0f;
	float theta2 = 180.0f;
	float h1 = -1.0f;
	float h2 = 1.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)height);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)theta1);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)theta2);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)h1);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)h2);

	height = (int)nv_clamp( height, 32, 8192 );
	theta1 = (int)nv_clamp( theta1, -180.0f, 0.0f );
	theta2 = (int)nv_clamp( theta2, 0.0f, 180.0f );
	h1 = (int)nv_clamp( h1, -1.0f, 0.0f );
	h2 = (int)nv_clamp( h2, 0.0f, 1.0f );


	m_heightMapBuilder.SetBounds (theta1, theta2, h1, h2); // degrees
	m_heightMapBuilder.SetDestSize (height * 2, height);
	m_heightMapBuilder.SetSourceModule (*m);
	m_heightMapBuilder.SetDestNoiseMap (m_heightMap);
	m_heightMapBuilder.Build ();

	return 0;
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NModelSphere class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NModelSphere, NOperator);

static NVarsBlocDesc blocdescNModelSphere[] =
{
	VAR(eudword,		true, "Height",			"512",	"NUwordProp")	//1
	VAR(efloat,		true, "phi1",			"-90.0",		"NFloatProp")	//2
	VAR(efloat,		true, "phi2",			"90.0",		"NFloatProp")	//3
	VAR(efloat,		true, "theta1",			"-180.0",	"NFloatProp")	//4
	VAR(efloat,		true, "theta2",			"180.0",	"NFloatProp")	//5
};

NModelSphere::NModelSphere()
{
	//Create variables bloc
	m_pcvarsBloc = AddVarsBloc(5, blocdescNModelSphere);
}

udword NModelSphere::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	noise::module::Module *m = (*_pOpsInts)->GetLibnoiseModule();

	if( m == NULL )
		return (udword)-1;

	int height = 512;
	float phi1 = -90.0f;
	float phi2 = 90.0f;
	float theta1 = -180.0f;
	float theta2 =  180.0f;

	m_pcvarsBloc->GetValue(0, _ftime, (uword&)height);
	m_pcvarsBloc->GetValue(1, _ftime, (float&)phi1);
	m_pcvarsBloc->GetValue(2, _ftime, (float&)phi2);
	m_pcvarsBloc->GetValue(3, _ftime, (float&)theta1);
	m_pcvarsBloc->GetValue(4, _ftime, (float&)theta2);

	height = (int)nv_clamp( height, 32, 8192 );
	phi1 = (int)nv_clamp( phi1, -90.0f, 0.0f );
	phi2 = (int)nv_clamp( phi2, 0.0f, 90.0f );
	theta1 = (int)nv_clamp( theta1, -180.0f, 0.0f );
	theta2 = (int)nv_clamp( theta2, 0.0f, 180.0f );


	m_heightMapBuilder.SetBounds (phi1, phi2, theta1, theta2); // degrees
	m_heightMapBuilder.SetDestSize (height * 2, height);
	m_heightMapBuilder.SetSourceModule (*m);
	m_heightMapBuilder.SetDestNoiseMap (m_heightMap);
	m_heightMapBuilder.Build ();

	return 0;
}




//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							NImageRenderer class implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
IMPLEMENT_CLASS(NImageRenderer, NOperator);



static NVarsBlocDesc blocdescNImageRenderer[] =
{
	VAR(erefobj2,		false, "Gradient File:",			"NULL",	"NGradientDialogProp")	//1
};

NImageRenderer::NImageRenderer()
{
	m_pcvarsBloc = AddVarsBloc(1, blocdescNImageRenderer);
}

udword NImageRenderer::Process(float _ftime, NOperator** _pOpsInts, float _fDetailFactor)
{
	//Only one Input
	if (m_byInputs!=1)		return (udword)-1;

	noise::utils::NoiseMap *nm = (*_pOpsInts)->GetNoiseMap();

	if( nm == NULL )
		return (udword)-1;

	noise::utils::Image image;
	m_renderer.SetSourceNoiseMap (*nm);
	m_renderer.SetDestImage (image);
	m_renderer.ClearGradient ();

	NArray<GradientElem> *pVarsObj = NULL;
	m_pcvarsBloc->GetValue(0, 0, (NObject*&)pVarsObj);

	if( pVarsObj != NULL )
	{
		NArray<GradientElem> *gradObj = ( NArray<GradientElem>* )pVarsObj;

		RGBA col[13];
		for( int i = 0; i < (*gradObj).Count(); i++ )
		{
			float height = (*gradObj)[i].height;

			m_renderer.AddGradientPoint (height,
				utils::Color(  (*gradObj)[i].color[0],
							   (*gradObj)[i].color[1],
							   (*gradObj)[i].color[2],
								255));
		}
	}
	else
	{
		// just a simple black and white gradient
		float h1 = -1.0f;
		m_renderer.AddGradientPoint (h1,
			utils::Color (  0,
							0,
							0,
							255));

		float h2 = 1.0f;
		m_renderer.AddGradientPoint (h2,
			utils::Color (  255,
							255,
							255,
							255));
	}

	// this pointer was created with 'new'
	// and it must be deleted
	// note: the property item that this is
	// created in will now be destroyed, hence
	// the lack of a dialog created with 'new'
	/*if( pVarsObj )
	{
		pVarsObj->Clear();
		///delete pVarsObj;
		////pVarsObj = NULL;
	}*/

	void *ptr = NULL;

	m_pcvarsBloc->GetValue(0, 0, (NObject*&)ptr);

	int value = 8421504;

	m_renderer.Render();

	//Bitmap instance
	NEngineOp::GetEngine()->GetBitmap(&m_pObj);

	NBitmap* pSrc = (NBitmap*)m_pObj;
	pSrc->SetSize( nm->GetWidth(), nm->GetHeight() );
	RGBA* pPxDst = pSrc->GetPixels();
	for (udword y=0; y<pSrc->GetHeight(); y++)
	{
		for (udword x=0; x<pSrc->GetWidth(); x++)
		{
			noise::utils::Color c1 = image.GetValue(x,pSrc->GetHeight()-y-1);
			(*pPxDst).r = c1.red;
			(*pPxDst).g = c1.green;
			(*pPxDst).b = c1.blue;
			(*pPxDst).a = c1.alpha;
			*pPxDst++;
		}
	}

	return 0;
}
