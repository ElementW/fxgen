/***************************************************************************

 Copyright (C) 2006  Sergey Prokhorchuk

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

***************************************************************************/


/* Taken from ngPlant */

#ifndef __P3DMATHSPLINE_H__
#define __P3DMATHSPLINE_H__

#define P3DMATH_NATURAL_CUBIC_SPLINE_CP_MAX_COUNT (32)

class P3DMathNaturalCubicSpline
 {
  public           :

                   P3DMathNaturalCubicSpline    ();
                   P3DMathNaturalCubicSpline    (const P3DMathNaturalCubicSpline
                                                                    &src);

  void             operator =                   (const P3DMathNaturalCubicSpline
                                                                    &src);

  unsigned int     GetCPCount                   () const;
  float            GetCPX                       (unsigned int        cp) const;
  float            GetCPY                       (unsigned int        cp) const;

  float            GetValue                     (float               x) const;
  float            GetTangent                   (float               x) const;

  void             AddCP                        (float               x,
                                                 float               y);

  void             UpdateCP                     (float               x,
                                                 float               y,
                                                 unsigned int        cp);

  void             DelCP                        (unsigned int        cp);

  void             SetConstant                  (float               value);
  void             SetLinear                    (float               ax,
                                                 float               ay,
                                                 float               bx,
                                                 float               by);

  void             CopyFrom                     (const P3DMathNaturalCubicSpline
                                                                    &src);

  private          :

  void             RecalcY2                     ();


  unsigned int     cp_count;
  float            cp_x[P3DMATH_NATURAL_CUBIC_SPLINE_CP_MAX_COUNT];
  float            cp_y[P3DMATH_NATURAL_CUBIC_SPLINE_CP_MAX_COUNT];
  float            cp_y2[P3DMATH_NATURAL_CUBIC_SPLINE_CP_MAX_COUNT];
 };

#endif

