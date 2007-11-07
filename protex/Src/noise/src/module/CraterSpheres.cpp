// CraterSpheres.cpp copyright CyberWiz
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

#include "../mathconsts.h"
#include "CraterSpheres.h"

#include "../noisegen.h"
#include "VecMath.h"

using namespace noise::module;

extern const int X_NOISE_GEN ;
extern const int Y_NOISE_GEN ;
extern const int Z_NOISE_GEN ;
extern const int SEED_NOISE_GEN ;
extern const int SHIFT_NOISE_GEN ;


#define alpha   (35.25/180.0*PI)      /* sphere segment cutoff angle */
#define crater_depth 0.85  /* 0.9 */

extern double c_density;          /* surface crater density */
static double a1 = crater_depth, b, d;


Crater::Crater(double rmin, double rmax, double depthmin, double depthmax, int seed)
{
	GetRandomVector(x, y, z);

	radius = GetRandomNumber(rmin,rmax);
	depth = GetRandomNumber(depthmin,depthmax);
	scale = GetRandomNumber(0.025,1.25);
	wall_thickness = GetRandomNumber(10.0,50.0);

	double radius_percentage = radius / (rmax-rmin);
	//wall_thickness *= radius_percentage;

	double depth_percentage  = depth / (depthmax-depthmin);

	// relate Squeeze to radius and depth 
	Squeeze = 0.8;// *radius_percentage;//
}

CraterSpheres::CraterSpheres ():
  Module (GetSourceModuleCount ()),
  m_displacement   (DEFAULT_CRATER_DISPLACEMENT),
  m_frequency      (DEFAULT_CRATER_FREQUENCY   ),
  m_seed           (DEFAULT_CRATER_SEED        ),
  m_enableDistance (false)
{
	for( int i = 0; i < MAX_NUM_CRATERS; i++ )
	{
		if( m_craters[i] )
		{
			m_craters[i] = NULL;
		}
	}

	b = sin(alpha);
    d = a1 - cos(alpha);

	craterHeight = 0.85;
}

CraterSpheres::~CraterSpheres ()
{
	for( int i = 0; i < m_numCraters; i++ )
	{
		if( m_craters[i] )
		{
			delete m_craters[i];
			m_craters[i] = NULL;
		}
	}
}


void CraterSpheres::CalcDistribution ()
{
	srand(m_seed);
	for(int i = 0; i < m_numCraters; i++ )
	{
		m_craters[i] = new Crater(m_rmin, m_rmax, m_depthmin, m_depthmax, m_seed);
	}
}



/* function to determine the z-elevation dependent on normalized squared
   radial coordinate */
// thanks to J. Beale (gForge) and Heiko Eissfeldt for Crater.c
double Crater_profile(double nsq_rad, double depth, double thickness)
{
   double radialcoord;
   radialcoord = sqrt(fabs(nsq_rad));

   if (radialcoord > b) {
     /* outer region gauss distribution */
     return d*exp(-thickness*(radialcoord-b)*(radialcoord-b));
   } else {
     /* inner region sphere segment */
     return depth - sqrt(1.0 - nsq_rad);
   }
}




/* function to control the weight of the crater profile as opposed
   to the underlying terrain elevation */
		/* check if outside */

// thanks to J. Beale (gForge) and Heiko Eissfeldt for Crater.c
double Dissolve(double nsq_rad)
{
  if (nsq_rad > 0.6*0.6) {//30
      return 1.0-0.9*exp(-30.0*(sqrt(nsq_rad) - 0.6)*(sqrt(nsq_rad) - 0.6));
  } else {		//25
      return 0.1*exp(-25.0*(sqrt(nsq_rad) - 0.6)*(sqrt(nsq_rad) - 0.6));
  }
}




double CraterSpheres::GetValue (double x, double y, double z) const
{
	if( m_pSourceModule[0] == NULL ) return 0.0;

	double value = m_pSourceModule[0]->GetValue (x, y, z);

	double val2 = value;

	Vec3 vecIn(x,y,z);

	int num_influenced = 0;

	for( int i = 0; i < m_numCraters; i++ )
	{
		Vec3 center(	m_craters[i]->x,
								m_craters[i]->y,
								m_craters[i]->z);

		Vec3 toPt(  vecIn.x - m_craters[i]->x,
							vecIn.y - m_craters[i]->y,
							vecIn.z - m_craters[i]->z);

		double sq_radius = toPt.x*toPt.x + toPt.y*toPt.y + toPt.z*toPt.z;
		double inv_len = 1 / sqrtf(sq_radius);
		
		double nsq_rad = sq_radius/ (m_craters[i]->radius * m_craters[i]->radius);
		
		if (sq_radius < m_craters[i]->radius*m_craters[i]->radius) 
		{
			Vec3 sampl0 = normalize(toPt) * m_craters[i]->radius / 2;
			Vec3 sampl1 = normalize(center-toPt) * m_craters[i]->radius / 2;
			Vec3 sampl2 = normalize(toPt.Cross( toPt, center )) * m_craters[i]->radius / 2;
			Vec3 sampl3 = normalize(sampl1.Cross( sampl1, center )) * m_craters[i]->radius / 2;


			double averageVal = value;
			averageVal+=m_pSourceModule[0]->GetValue (sampl0.x, sampl0.y, sampl0.z) ;
			averageVal+=m_pSourceModule[0]->GetValue (sampl1.x, sampl1.y, sampl1.z) ;
			averageVal+=m_pSourceModule[0]->GetValue (sampl2.x, sampl2.y, sampl2.z) ;
			averageVal+=m_pSourceModule[0]->GetValue (sampl3.x, sampl3.y, sampl3.z) ;


			averageVal /= 5;

			double the_depth;


			the_depth = m_craters[i]->depth;
			

			double depth_percentage = the_depth / (LIM_MAX - LIM_MIN);

			// this is a global value
			d = the_depth - cos(alpha);

			double shift = m_craters[i]->scale*Crater_profile(nsq_rad,  
															  the_depth, 
															  m_craters[i]->wall_thickness*depth_percentage);

			double weight = Dissolve(nsq_rad);

			double shape = (shift  + val2)*weight;

			if( shape > LIM_MAX )
				shape = LIM_MAX;//(val2 - the_depth) + shift;
			if( shape < LIM_MIN )
				shape = LIM_MIN;
			
			// this is different to the original function
			if( weight > 0.0 && weight < 1.0 )
				val2 = shape + abs((value-averageVal)/m_craters[i]->Squeeze)*(1.0-weight); 	
			else
				val2 = shape + abs((value-averageVal)/m_craters[i]->Squeeze);
		}
	}

	// clamp the result
	if (val2 < LIM_MIN) {
		return LIM_MIN;
	} else if (val2 > LIM_MAX) {
		return LIM_MAX;
	} else {
		return val2;
	}

}
