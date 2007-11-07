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
#include "pch.h"
#include "p3dmathspline.h"

/*NOTE: math is based on "Numerical Recipes in C" */

                   P3DMathNaturalCubicSpline::P3DMathNaturalCubicSpline
                                                ()
 {
  cp_count = 0;
 }

                   P3DMathNaturalCubicSpline::P3DMathNaturalCubicSpline
                                                (const P3DMathNaturalCubicSpline
                                                                    &src)
 {
  CopyFrom(src);
 }

void               P3DMathNaturalCubicSpline::operator =
                                                (const P3DMathNaturalCubicSpline
                                                                    &src)
 {
  if (&src != this)
   {
    CopyFrom(src);
   }
 }

void               P3DMathNaturalCubicSpline::CopyFrom
                                                (const P3DMathNaturalCubicSpline
                                                                    &src)
 {
  cp_count = src.cp_count;

  for (unsigned int i = 0; i < cp_count; i++)
   {
    cp_x[i]  = src.cp_x[i];
    cp_y[i]  = src.cp_y[i];
    cp_y2[i] = src.cp_y2[i];
   }
 }

unsigned int       P3DMathNaturalCubicSpline::GetCPCount
                                                () const
 {
  return(cp_count);
 }

float              P3DMathNaturalCubicSpline::GetCPX
                                                (unsigned int        cp) const
 {
  return(cp_x[cp]);
 }

float              P3DMathNaturalCubicSpline::GetCPY
                                                (unsigned int        cp) const
 {
  return(cp_y[cp]);
 }

float              P3DMathNaturalCubicSpline::GetValue
                                                (float               x) const
 {
  if (cp_count == 0)
   {
    return(0.0f);
   }
  else if (cp_count == 1)
   {
    return(cp_y[0]);
   }
  else
   {
    unsigned int base;

    base = 0;

    while ((base < cp_count) && (cp_x[base] < x))
     {
      base++;
     }

    if (base == 0)
     {
      return(cp_y[0]);
     }
    else if (base == cp_count)
     {
      return(cp_y[cp_count - 1]);
     }

    float h,a,b;

    h = cp_x[base] - cp_x[base - 1];
    a = (cp_x[base] - x) / h;
    b = (x - cp_x[base - 1]) / h;

    return(a * cp_y[base - 1] + b * cp_y[base] +
           ((a * a * a - a) * cp_y2[base - 1] + (b * b * b - b) * cp_y2[base]) * h * h / 6.0f);
   }
 }

float              P3DMathNaturalCubicSpline::GetTangent
                                                (float               x) const
 {
  if (cp_count == 0)
   {
    return(0.0f);
   }
  else if (cp_count == 1)
   {
    return(cp_y[0]);
   }
  else
   {
    unsigned int base;

    base = 0;

    while ((base < cp_count) && (cp_x[base] < x))
     {
      base++;
     }

    if (base == 0)
     {
      base = 1;
     }
    else if (base == cp_count)
     {
      base = cp_count - 1;
     }

    float h,a,b;

    h = cp_x[base] - cp_x[base - 1];
    a = (cp_x[base] - x) / h;
    b = (x - cp_x[base - 1]) / h;

    return((cp_y[base] - cp_y[base - 1]) / h -
           ((((3.0f * a * a) - 1.0f) * h * cp_y2[base - 1]) / 6.0f) +
           ((((3.0f * b * b) - 1.0f) * h * cp_y2[base]) / 6.0f));
   }
 }

void               P3DMathNaturalCubicSpline::RecalcY2
                                                ()
 {
  float                                          u[P3DMATH_NATURAL_CUBIC_SPLINE_CP_MAX_COUNT - 1];
  float                                          sig;
  float                                          p;
  float                                          qn,un;

  cp_y2[0] = 0.0f;
  u[0]     = 0.0f;

  for (unsigned int i = 1; i < (cp_count - 1); i++)
   {
    sig = (cp_x[i] - cp_x[i - 1]) / (cp_x[i + 1] - cp_x[i - 1]);
    p   = sig * cp_y2[i - 1] + 2.0f;
    cp_y2[i] = (sig - 1.0f) / p;

    u[i] = (cp_y[i + 1] - cp_y[i]) / (cp_x[i + 1] - cp_x[i]) -
           (cp_y[i] - cp_y[i - 1]) / (cp_x[i] - cp_x[i - 1]);

    u[i] = (6.0 * u[i] / (cp_x[i + 1] - cp_x[i - 1]) - sig * u[i - 1]) / p;
   }

  qn = un = 0.0f;

  cp_y2[cp_count - 1] = (un - qn * u[cp_count - 2]) / (qn * cp_y2[cp_count - 2] + 1.0f);

  for (int k = (cp_count - 2); k >= 0; k--)
   {
    cp_y2[k] = cp_y2[k] * cp_y2[k + 1] + u[k];
   }
 }

void               P3DMathNaturalCubicSpline::AddCP
                                                (float               x,
                                                 float               y)
 {
  unsigned int                                   cp;

  if (cp_count < P3DMATH_NATURAL_CUBIC_SPLINE_CP_MAX_COUNT)
   {
    cp = 0;

    while ((cp < cp_count) && (cp_x[cp] < x))
     {
      cp++;
     }

    if (cp < cp_count)
     {
      for (int i = cp_count; i > ((int)cp); i--)
       {
        cp_x[i] = cp_x[i - 1]; cp_y[i] = cp_y[i - 1];
       }
     }

    cp_x[cp] = x; cp_y[cp] = y;

    cp_count++;

    if (cp_count > 1)
     {
      RecalcY2();
     }
    else
     {
      cp_y2[0] = 0.0f;
      cp_y2[1] = 0.0f;
     }
   }
 }

void               P3DMathNaturalCubicSpline::DelCP
                                                (unsigned int        cp)
 {
  if (cp < cp_count)
   {
    for (unsigned int i = (cp + 1); i < cp_count; i++)
     {
      cp_x[i - 1] = cp_x[i]; cp_y[i - 1] = cp_y[i];
     }

    cp_count--;

    if (cp_count > 1)
     {
      RecalcY2();
     }
   }
 }

void               P3DMathNaturalCubicSpline::UpdateCP
                                                (float               x,
                                                 float               y,
                                                 unsigned int        cp)
 {
  if (cp < cp_count)
   {
    cp_x[cp] = x; cp_y[cp] = y;

    if (cp_count > 1)
     {
      RecalcY2();
     }
   }
 }

void               P3DMathNaturalCubicSpline::SetConstant
                                                (float               value)
 {
  SetLinear(0.0f,value,1.0f,value);
 }

void               P3DMathNaturalCubicSpline::SetLinear
                                                (float               ax,
                                                 float               ay,
                                                 float               bx,
                                                 float               by)
 {
  cp_count = 2;

  cp_x[0] = ax;
  cp_y[0] = ay;
  cp_x[1] = bx;
  cp_y[1] = by;

  cp_y2[0] = 0.0f;
  cp_y2[1] = 0.0f;
 }

