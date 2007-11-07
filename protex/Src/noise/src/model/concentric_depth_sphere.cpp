
#include "../latlon.h"
#include "concentric_depth_sphere.h"

using namespace noise;
using namespace noise::model;

ConcentricDepthSphere::ConcentricDepthSphere ():
  m_pModule (NULL)
{
}

ConcentricDepthSphere::ConcentricDepthSphere (const module::Module& module):
  m_pModule (&module)
{
}

double ConcentricDepthSphere::GetValue (double lat, double lon) const
{
  //assert (m_pModule != NULL);

  if( m_pModule == NULL ) return 0.0;

  double x, y, z;
  LatLonToXYZ (lat, lon, x, y, z);
  x*= m_radius;
  y*= m_radius;
  z*= m_radius;
  return m_pModule->GetValue (x, y, z);
}
