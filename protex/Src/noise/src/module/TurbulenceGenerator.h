// ridgedmulti.h
//
// Copyright (C) 2003, 2004 Jason Bevins
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
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef NOISE_MODULE_TURBULENCE_GEN_H
#define NOISE_MODULE_TURBULENCE_GEN_H

#include "modulebase.h"

namespace noise
{

  namespace module
  {

    /// @addtogroup libnoise
    /// @{

    /// @addtogroup modules
    /// @{

    /// @addtogroup generatormodules
    /// @{

    /// Default frequency for the noise::module::RidgedMulti noise module.
    const double DEFAULT_TURBULENCE_GEN_FREQUENCY = 1.0;

    /// Default lacunarity for the noise::module::RidgedMulti noise module.
    const double DEFAULT_TURBULENCE_GEN_LACUNARITY = 2.0;

    /// Default number of octaves for the noise::module::RidgedMulti noise
    /// module.
    const int DEFAULT_TURBULENCE_GEN_OCTAVE_COUNT = 6;

    /// Default noise quality for the noise::module::RidgedMulti noise
    /// module.
    const noise::NoiseQuality DEFAULT_TURBULENCE_GEN_QUALITY = QUALITY_STD;

    /// Default noise seed for the noise::module::RidgedMulti noise module.
    const int DEFAULT_TURBULENCE_GEN_SEED = 0;

    /// Maximum number of octaves for the noise::module::RidgedMulti noise
    /// module.
    const int TURBULENCE_GEN_MAX_OCTAVE = 30;

    /// Noise module that outputs 3-dimensional Heterogeneous procedural terrain noise.
    ///
    /// @image html moduleridgedmulti.png
    ///
    /// This noise module, heavily based on the Perlin-noise module, generates
    /// Heterogeneous procedural terrain noise.  Heterogeneous procedural terrain noise is generated in
    /// much of the same way as Perlin noise, except the output of each octave
    /// is modified by an absolute-value function.  Modifying the octave
    /// values in this way produces ridge-like formations.
    ///
    /// Heterogeneous procedural terrain noise does not use a persistence value.  This is
    /// because the persistence values of the octaves are based on the values
    /// generated from from previous octaves, creating a feedback loop (or
    /// that's what it looks like after reading the code.)
    ///
    /// This noise module outputs Heterogeneous procedural terrain-noise values that
    /// usually range from -1.0 to +1.0, but there are no guarantees that all
    /// output values will exist within that range.
    ///
    /// @note For Heterogeneous procedural terrain noise generated with only one octave,
    /// the output value ranges from -1.0 to 0.0.
    ///
    /// Heterogeneous procedural terrain noise is often used to generate craggy mountainous
    /// terrain or marble-like textures.
    ///
    /// This noise module does not require any source modules.
    ///
    /// <b>Octaves</b>
    ///
    /// The number of octaves control the <i>amount of detail</i> of the
    /// Heterogeneous procedural terrain noise.  Adding more octaves increases the detail
    /// of the Heterogeneous procedural terrain noise, but with the drawback of increasing
    /// the calculation time.
    ///
    /// An application may specify the number of octaves that generate
    /// Heterogeneous procedural terrain noise by calling the SetOctaveCount() method.
    ///
    /// <b>Frequency</b>
    ///
    /// An application may specify the frequency of the first octave by
    /// calling the SetFrequency() method.
    ///
    /// <b>Lacunarity</b>
    ///
    /// The lacunarity specifies the frequency multipler between successive
    /// octaves.
    ///
    /// The effect of modifying the lacunarity is subtle; you may need to play
    /// with the lacunarity value to determine the effects.  For best results,
    /// set the lacunarity to a number between 1.5 and 3.5.
    ///
    /// <b>References &amp; Acknowledgments</b>
    ///
    /// <a href=http://www.texturingandmodeling.com/Musgrave.html>F.
    /// Kenton "Doc Mojo" Musgrave's texturing page</a> - This page contains
    /// links to source code that generates hybrid-multfractal noise, among
    /// other types of noise.  The source file <a
    /// href=http://www.texturingandmodeling.com/CODE/MUSGRAVE/CLOUD/fractal.c>
    /// fractal.c</a> contains the code I used in my Heterogeneous procedural terrain class
    /// (see the @a RidgedMultifractal() function.)  This code was written by F.
    /// Kenton Musgrave, the person who created
    /// <a href=http://www.pandromeda.com/>MojoWorld</a>.  He is also one of
    /// the authors in <i>Texturing and Modeling: A Procedural Approach</i>
    /// (Morgan Kaufmann, 2002. ISBN 1-55860-848-6.)
    class TurbulenceGenerator: public Module
    {

      public:

        /// Constructor.
        ///
        /// The default number of octaves is set to
        /// noise::module::DEFAULT_RIDGED_OCTAVE_COUNT.
        ///
        /// The default frequency is set to
        /// noise::module::DEFAULT_RIDGED_FREQUENCY.
        ///
        /// The default lacunarity is set to
        /// noise::module::DEFAULT_RIDGED_LACUNARITY.
        ///
        /// The default seed value is set to
        /// noise::module::DEFAULT_RIDGED_SEED.
        TurbulenceGenerator ();

        /// Returns the frequency of the first octave.
        ///
        /// @returns The frequency of the first octave.
        double GetFrequency () const
        {
          return m_frequency;
        }

        /// Returns the lacunarity of the Heterogeneous procedural terrain noise.
        ///
        /// @returns The lacunarity of the Heterogeneous procedural terrain noise.
        /// 
        /// The lacunarity is the frequency multiplier between successive
        /// octaves.
        double GetLacunarity () const
        {
          return m_lacunarity;
        }

        /// Returns the quality of the Heterogeneous procedural terrain noise.
        ///
        /// @returns The quality of the Heterogeneous procedural terrain noise.
        ///
        /// See noise::NoiseQuality for definitions of the various
        /// coherent-noise qualities.
        noise::NoiseQuality GetNoiseQuality () const
        {
          return m_noiseQuality;
        }

        /// Returns the number of octaves that generate the
        /// Heterogeneous procedural terrain noise.
        ///
        /// @returns The number of octaves that generate the
        /// Heterogeneous procedural terrain noise.
        ///
        /// The number of octaves controls the amount of detail in the
        /// Heterogeneous procedural terrain noise.
        int GetOctaveCount () const
        {
          return m_octaveCount;
        }

        /// Returns the seed value used by the Heterogeneous procedural terrain-noise
        /// function.
        ///
        /// @returns The seed value.
        int GetSeed () const
        {
          return m_seed;
        }

        virtual int GetSourceModuleCount () const
        {
          return 0;
        }

        virtual double GetValue (double x, double y, double z) const;

        /// Sets the frequency of the first octave.
        ///
        /// @param frequency The frequency of the first octave.
        void SetFrequency (double frequency)
        {
          m_frequency = frequency;
        }

        /// Sets the lacunarity of the Heterogeneous procedural terrain noise.
        ///
        /// @param lacunarity The lacunarity of the Heterogeneous procedural terrain noise.
        /// 
        /// The lacunarity is the frequency multiplier between successive
        /// octaves.
        ///
        /// For best results, set the lacunarity to a number between 1.5 and
        /// 3.5.
        void SetLacunarity (double lacunarity)
        {
          m_lacunarity = lacunarity;
          CalcSpectralWeights ();
        }

        /// Sets the quality of the Heterogeneous procedural terrain noise.
        ///
        /// @param noiseQuality The quality of the Heterogeneous procedural terrain noise.
        ///
        /// See noise::NoiseQuality for definitions of the various
        /// coherent-noise qualities.
        void SetNoiseQuality (noise::NoiseQuality noiseQuality)
        {
          m_noiseQuality = noiseQuality;
        }

        /// Sets the number of octaves that generate the Heterogeneous procedural terrain
        /// noise.
        ///
        /// @param octaveCount The number of octaves that generate the
        /// Heterogeneous procedural terrain noise.
        ///
        /// @pre The number of octaves ranges from 1 to
        /// noise::module::TURBULENCE_GEN_MAX_OCTAVE.
        ///
        /// @throw noise::ExceptionInvalidParam An invalid parameter was
        /// specified; see the preconditions for more information.
        ///
        /// The number of octaves controls the amount of detail in the
        /// ridged-multifractal noise.
        ///
        /// The larger the number of octaves, the more time required to
        /// calculate the Heterogeneous procedural terrain-noise value.
        void SetOctaveCount (int octaveCount)
        {
          if (octaveCount > TURBULENCE_GEN_MAX_OCTAVE) {
            throw noise::ExceptionInvalidParam ();
          }
          m_octaveCount = octaveCount;
        }

		void SetOctaves( double _octaves )
		{
			octaves = _octaves;
			SetOctaveCount( (int)octaves );
		}

        /// Sets the seed value used by the Heterogeneous procedural terrain-noise
        /// function.
        ///
        /// @param seed The seed value.
        void SetSeed (int seed)
        {
          m_seed = seed;
        }

		void SetGain(double Gain)
		{
			gain = Gain;
		}

		double GetGain(){return gain;}

		void SetOffset( double Offset ) {
			offset = Offset;
		}

		double GetOffset(){return offset;}


		void ReCalcSpectralWeights(){ CalcSpectralWeights(); }

		void SetExponent( double _exp ){ exponent = _exp; }
		double GetExponent(){ return exponent; }

      protected:

        /// Calculates the spectral weights for each octave.
        ///
        /// This method is called when the lacunarity changes.
        void CalcSpectralWeights ();

        /// Frequency of the first octave.
        double m_frequency;

        /// Frequency multiplier between successive octaves.
        double m_lacunarity;

        /// Quality of the Heterogeneous procedural terrain noise.
        noise::NoiseQuality m_noiseQuality;

        /// Total number of octaves that generate the Heterogeneous procedural terrain
        /// noise.
        int m_octaveCount;

		double octaves;

        /// Contains the spectral weights for each octave.
        double m_pSpectralWeights[TURBULENCE_GEN_MAX_OCTAVE];

		double gain;

		 double offset;

		double exponent;


        /// Seed value used by the hybrid-multfractal-noise function.
        int m_seed;

    };

    /// @}

    /// @}

    /// @}

  }

}

#endif
