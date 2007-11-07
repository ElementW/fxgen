// TurbulenceGenerator.cpp
//
// Copyright (C) 2007 cyber_wiz
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "TurbulenceGenerator.h"

using namespace noise::module;

TurbulenceGenerator::TurbulenceGenerator ():
	Module (GetSourceModuleCount ()),
	m_octaveCount  (DEFAULT_TURBULENCE_GEN_OCTAVE_COUNT),
	m_frequency    (DEFAULT_TURBULENCE_GEN_FREQUENCY   ),
	m_lacunarity   (DEFAULT_TURBULENCE_GEN_LACUNARITY  ),
	m_noiseQuality (DEFAULT_TURBULENCE_GEN_QUALITY     ),
	m_seed         (DEFAULT_TURBULENCE_GEN_SEED)
{
	offset = 1.0;
	gain = 2.0;
	CalcSpectralWeights ();
}

// Calculates the spectral weights for each octave.
void TurbulenceGenerator::CalcSpectralWeights ()
{
	// This exponent parameter should be user-defined; it may be exposed in a
	// future version of libnoise.
	double h = 1.0;

	double frequency = 1.0;
	for (int i = 0; i < TURBULENCE_GEN_MAX_OCTAVE; i++) {
		// Compute weight for each frequency.
		m_pSpectralWeights[i] = pow (frequency, -h);
		frequency *= m_lacunarity;
	}
}

// Multifractal code originally written by F. Kenton "Doc Mojo" Musgrave,
// 1998.  Modified by cyber_wiz for use with libnoise.
double TurbulenceGenerator::GetValue (double x, double y, double z) const
{
	double frequency, remainder;
	int i;
	double signal = 0.0;
	double value  = 0.0;
	double weight = 1.0;

	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

    double nx, ny, nz;

	value = 1.0f;

	for (int curOctave = 0; curOctave < m_octaveCount; curOctave++) {

		// Make sure that these floating-point values have the same range as a 32-
		// bit integer so that we can pass them to the coherent-noise functions.
		nx = MakeInt32Range (x);
		ny = MakeInt32Range (y);
		nz = MakeInt32Range (z);

		// Get the coherent-noise value.
		int seed = (m_seed + curOctave) & 0x7fffffff;
		value *= GradientCoherentNoise3D (nx, ny, nz, seed, m_noiseQuality);
		value *= m_pSpectralWeights[curOctave];
		value += 1.0f;
		
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;
  }

	return( value );

//  return (value * 1.25) - 1.0;
}
