//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Maths.cpp
//! \brief	mathematics
//!
//!	\author	Johann Nadalutti (fxgen@free.fr) (based on nvidia sdk)
//!	\date		12-02-2007
//!
//!	\brief	This file applies the GNU LESSER GENERAL PUBLIC LICENSE
//!					Version 2.1 , read file COPYING.
//!
//!    			The original version of this library can be located at:
//!    			http://sourceforge.net/projects/fxgen/
//!
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//                   Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "../Include/FxGenLib.h"
#include "Maths.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							vec3 struct implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
vec3 & cross(vec3 & u, const vec3 & v, const vec3 & w)
{
    u.x = v.y*w.z - v.z*w.y;
    u.y = v.z*w.x - v.x*w.z;
    u.z = v.x*w.y - v.y*w.x;
    return u;
}

float & dot(float& u, const vec3& v, const vec3& w)
{
    u = v.x*w.x + v.y*w.y + v.z*w.z;
    return u;
}


vec3 & reflect(vec3& r, const vec3& n, const vec3& l)
{
    float n_dot_l;
    n_dot_l = nv_two * dot(n_dot_l,n,l);
    mult(r,l,-nv_one);
    madd(r,n,n_dot_l);
    return r;
}

vec3 & madd(vec3 & u, const vec3& v, const float& lambda)
{
    u.x += v.x*lambda;
    u.y += v.y*lambda;
    u.z += v.z*lambda;
    return u;
}

vec3 & mult(vec3 & u, const vec3& v, const float& lambda)
{
    u.x = v.x*lambda;
    u.y = v.y*lambda;
    u.z = v.z*lambda;
    return u;
}

vec3 & mult(vec3 & u, const vec3& v, const vec3& w)
{
    u.x = v.x*w.x;
    u.y = v.y*w.y;
    u.z = v.z*w.z;
    return u;
}

vec3 & sub(vec3 & u, const vec3& v, const vec3& w)
{
    u.x = v.x - w.x;
    u.y = v.y - w.y;
    u.z = v.z - w.z;
    return u;
}

vec3 & add(vec3 & u, const vec3& v, const vec3& w)
{
    u.x = v.x + w.x;
    u.y = v.y + w.y;
    u.z = v.z + w.z;
    return u;
}

float vec3::normalize()
{
	float norm = sqrtf(x * x + y * y + z * z);
	if (norm > nv_eps)
		norm = nv_one / norm;
	else
		norm = nv_zero;
	x *= norm;
	y *= norm;
	z *= norm;
	return norm;
}

vec3 & scale(vec3& u, const float s)
{
    u.x *= s;
    u.y *= s;
    u.z *= s;
    return u;
}


vec3 & mult_pos(vec3& u, const mat4& M, const vec3& v)
{
    float oow = nv_one / (v.x * M.a30 + v.y * M.a31 + v.z * M.a32 + M.a33);
    u.x = (M.a00 * v.x + M.a01 * v.y + M.a02 * v.z + M.a03) * oow;
    u.y = (M.a10 * v.x + M.a11 * v.y + M.a12 * v.z + M.a13) * oow;
    u.z = (M.a20 * v.x + M.a21 * v.y + M.a22 * v.z + M.a23) * oow;
    return u;
}

vec3 & mult_dir(vec3& u, const mat4& M, const vec3& v)
{
    u.x = M.a00 * v.x + M.a01 * v.y + M.a02 * v.z + M.a03 * 1.0f;
		u.y = M.a10 * v.x + M.a11 * v.y + M.a12 * v.z + M.a13 * 1.0f;
		u.z = M.a20 * v.x + M.a21 * v.y + M.a22 * v.z + M.a23 * 1.0f;
    return u;
}

vec3 & normalize(vec3& u)
{
	float norm = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
	if (norm > nv_eps)
		norm = nv_one / norm;
	else
		norm = nv_zero;
	return scale(u,norm);
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							mat4 struct implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------

mat4 & add(mat4& A, const mat4& B)
{
    A.a00 += B.a00;
    A.a10 += B.a10;
    A.a20 += B.a20;
    A.a30 += B.a30;
    A.a01 += B.a01;
    A.a11 += B.a11;
    A.a21 += B.a21;
    A.a31 += B.a31;
    A.a02 += B.a02;
    A.a12 += B.a12;
    A.a22 += B.a22;
    A.a32 += B.a32;
    A.a03 += B.a03;
    A.a13 += B.a13;
    A.a23 += B.a23;
    A.a33 += B.a33;
    return A;
}


// C = A * B

// C.a00 C.a01 C.a02 C.a03   A.a00 A.a01 A.a02 A.a03   B.a00 B.a01 B.a02 B.a03
//
// C.a10 C.a11 C.a12 C.a13   A.a10 A.a11 A.a12 A.a13   B.a10 B.a11 B.a12 B.a13
//
// C.a20 C.a21 C.a22 C.a23   A.a20 A.a21 A.a22 A.a23   B.a20 B.a21 B.a22 B.a23
//
// C.a30 C.a31 C.a32 C.a33 = A.a30 A.a31 A.a32 A.a33 * B.a30 B.a31 B.a32 B.a33

mat4 & mult(mat4& C, const mat4& A, const mat4& B)
{
    C.a00 = A.a00 * B.a00 + A.a01 * B.a10 + A.a02 * B.a20 + A.a03 * B.a30;
    C.a10 = A.a10 * B.a00 + A.a11 * B.a10 + A.a12 * B.a20 + A.a13 * B.a30;
    C.a20 = A.a20 * B.a00 + A.a21 * B.a10 + A.a22 * B.a20 + A.a23 * B.a30;
    C.a30 = A.a30 * B.a00 + A.a31 * B.a10 + A.a32 * B.a20 + A.a33 * B.a30;
    C.a01 = A.a00 * B.a01 + A.a01 * B.a11 + A.a02 * B.a21 + A.a03 * B.a31;
    C.a11 = A.a10 * B.a01 + A.a11 * B.a11 + A.a12 * B.a21 + A.a13 * B.a31;
    C.a21 = A.a20 * B.a01 + A.a21 * B.a11 + A.a22 * B.a21 + A.a23 * B.a31;
    C.a31 = A.a30 * B.a01 + A.a31 * B.a11 + A.a32 * B.a21 + A.a33 * B.a31;
    C.a02 = A.a00 * B.a02 + A.a01 * B.a12 + A.a02 * B.a22 + A.a03 * B.a32;
    C.a12 = A.a10 * B.a02 + A.a11 * B.a12 + A.a12 * B.a22 + A.a13 * B.a32;
    C.a22 = A.a20 * B.a02 + A.a21 * B.a12 + A.a22 * B.a22 + A.a23 * B.a32;
    C.a32 = A.a30 * B.a02 + A.a31 * B.a12 + A.a32 * B.a22 + A.a33 * B.a32;
    C.a03 = A.a00 * B.a03 + A.a01 * B.a13 + A.a02 * B.a23 + A.a03 * B.a33;
    C.a13 = A.a10 * B.a03 + A.a11 * B.a13 + A.a12 * B.a23 + A.a13 * B.a33;
    C.a23 = A.a20 * B.a03 + A.a21 * B.a13 + A.a22 * B.a23 + A.a23 * B.a33;
    C.a33 = A.a30 * B.a03 + A.a31 * B.a13 + A.a32 * B.a23 + A.a33 * B.a33;
    return C;
}

mat4 & transpose(mat4& A)
{
    float tmp;
    tmp = A.a01;
    A.a01 = A.a10;
    A.a10 = tmp;

    tmp = A.a02;
    A.a02 = A.a20;
    A.a20 = tmp;

    tmp = A.a03;
    A.a03 = A.a30;
    A.a30 = tmp;

    tmp = A.a12;
    A.a12 = A.a21;
    A.a21 = tmp;

    tmp = A.a13;
    A.a13 = A.a31;
    A.a31 = tmp;

    tmp = A.a23;
    A.a23 = A.a32;
    A.a32 = tmp;
    return A;
}

mat4 & transpose(mat4& B, const mat4& A)
{
    B.a00 = A.a00;
    B.a01 = A.a10;
    B.a02 = A.a20;
    B.a03 = A.a30;
    B.a10 = A.a01;
    B.a11 = A.a11;
    B.a12 = A.a21;
    B.a13 = A.a31;
    B.a20 = A.a02;
    B.a21 = A.a12;
    B.a22 = A.a22;
    B.a23 = A.a32;
    B.a30 = A.a03;
    B.a31 = A.a13;
    B.a32 = A.a23;
    B.a33 = A.a33;
    return B;
}


/*
    calculate the determinent of a 2x2 matrix in the from

    | a1 a2 |
    | b1 b2 |

*/
float det2x2(float a1, float a2, float b1, float b2)
{
    return a1 * b2 - b1 * a2;
}

/*
    calculate the determinent of a 3x3 matrix in the from

    | a1 a2 a3 |
    | b1 b2 b3 |
    | c1 c2 c3 |

*/
float det3x3(float a1, float a2, float a3,
                         float b1, float b2, float b3,
                         float c1, float c2, float c3)
{
    return a1 * det2x2(b2, b3, c2, c3) - b1 * det2x2(a2, a3, c2, c3) + c1 * det2x2(a2, a3, b2, b3);
}

mat4 & invert(mat4& B, const mat4& A)
{
    float det,oodet;

    B.a00 =  det3x3(A.a11, A.a21, A.a31, A.a12, A.a22, A.a32, A.a13, A.a23, A.a33);
    B.a10 = -det3x3(A.a10, A.a20, A.a30, A.a12, A.a22, A.a32, A.a13, A.a23, A.a33);
    B.a20 =  det3x3(A.a10, A.a20, A.a30, A.a11, A.a21, A.a31, A.a13, A.a23, A.a33);
    B.a30 = -det3x3(A.a10, A.a20, A.a30, A.a11, A.a21, A.a31, A.a12, A.a22, A.a32);

    B.a01 = -det3x3(A.a01, A.a21, A.a31, A.a02, A.a22, A.a32, A.a03, A.a23, A.a33);
    B.a11 =  det3x3(A.a00, A.a20, A.a30, A.a02, A.a22, A.a32, A.a03, A.a23, A.a33);
    B.a21 = -det3x3(A.a00, A.a20, A.a30, A.a01, A.a21, A.a31, A.a03, A.a23, A.a33);
    B.a31 =  det3x3(A.a00, A.a20, A.a30, A.a01, A.a21, A.a31, A.a02, A.a22, A.a32);

    B.a02 =  det3x3(A.a01, A.a11, A.a31, A.a02, A.a12, A.a32, A.a03, A.a13, A.a33);
    B.a12 = -det3x3(A.a00, A.a10, A.a30, A.a02, A.a12, A.a32, A.a03, A.a13, A.a33);
    B.a22 =  det3x3(A.a00, A.a10, A.a30, A.a01, A.a11, A.a31, A.a03, A.a13, A.a33);
    B.a32 = -det3x3(A.a00, A.a10, A.a30, A.a01, A.a11, A.a31, A.a02, A.a12, A.a32);

    B.a03 = -det3x3(A.a01, A.a11, A.a21, A.a02, A.a12, A.a22, A.a03, A.a13, A.a23);
    B.a13 =  det3x3(A.a00, A.a10, A.a20, A.a02, A.a12, A.a22, A.a03, A.a13, A.a23);
    B.a23 = -det3x3(A.a00, A.a10, A.a20, A.a01, A.a11, A.a21, A.a03, A.a13, A.a23);
    B.a33 =  det3x3(A.a00, A.a10, A.a20, A.a01, A.a11, A.a21, A.a02, A.a12, A.a22);

    det = (A.a00 * B.a00) + (A.a01 * B.a10) + (A.a02 * B.a20) + (A.a03 * B.a30);

    oodet = nv_one / det;

    B.a00 *= oodet;
    B.a10 *= oodet;
    B.a20 *= oodet;
    B.a30 *= oodet;

    B.a01 *= oodet;
    B.a11 *= oodet;
    B.a21 *= oodet;
    B.a31 *= oodet;

    B.a02 *= oodet;
    B.a12 *= oodet;
    B.a22 *= oodet;
    B.a32 *= oodet;

    B.a03 *= oodet;
    B.a13 *= oodet;
    B.a23 *= oodet;
    B.a33 *= oodet;

    return B;
}

mat4 & invert_rot_trans(mat4& B, const mat4& A)
{
    B.a00 = A.a00;
    B.a10 = A.a01;
    B.a20 = A.a02;
    B.a30 = A.a30;
    B.a01 = A.a10;
    B.a11 = A.a11;
    B.a21 = A.a12;
    B.a31 = A.a31;
    B.a02 = A.a20;
    B.a12 = A.a21;
    B.a22 = A.a22;
    B.a32 = A.a32;
    B.a03 = - (A.a00 * A.a03 + A.a10 * A.a13 + A.a20 * A.a23);
    B.a13 = - (A.a01 * A.a03 + A.a11 * A.a13 + A.a21 * A.a23);
    B.a23 = - (A.a02 * A.a03 + A.a12 * A.a13 + A.a22 * A.a23);
    B.a33 = A.a33;
    return B;
}

void mat4::set_rot(const quat& q)
{
	float x2 = q.x * q.x * nv_two;
	float y2 = q.y * q.y * nv_two;
	float z2 = q.z * q.z * nv_two;

	a00 = nv_one - y2 - z2;
	a11 = nv_one - x2 - z2;
	a22 = nv_one - x2 - y2;

	float wz = q.w*q.z, xy = q.x*q.y;
	a01 = nv_two * (xy + wz);
	a10 = nv_two * (xy - wz);

	float wx = q.w*q.x, yz = q.y*q.z;
	a12 = nv_two * (yz + wx);
	a21 = nv_two * (yz - wx);

	float wy = q.w*q.y, xz = q.x*q.z;
	a02 = nv_two * (xz - wy);
	a20 = nv_two * (xz + wy);
}

// Angles Euler X,Y,Z en Radian
void mat4::set_rot(const vec3 & a)
{
	float sx=sin(a.x);
	float sy=sin(a.y);
	float sz=sin(a.z);
	float cx=cos(a.x);
	float cy=cos(a.y);
	float cz=cos(a.z);

	a00=cy*cz;
	a01=cy*sz;
	a02=-sy;
	a10=sx*sy*cz-cx*sz;
	a11=sx*sy*sz+cx*cz;
	a12=sx*cy;
	a20=cx*sy*cz+sx*sz;
	a21=cx*sy*sz-sx*cz;
	a22=cx*cy;

}

// v is normalized
// theta in radians
/*void mat4::set_rot(const float& theta, const vec3& v)
{
float ct = float(cos(theta));
float st = float(sin(theta));

float xx = v.x * v.x;
float yy = v.y * v.y;
float zz = v.z * v.z;
float xy = v.x * v.y;
float xz = v.x * v.z;
float yz = v.y * v.z;

a00 = xx + ct*(1-xx);
a01 = xy + ct*(-xy) + st*-v.z;
a02 = xz + ct*(-xz) + st*v.y;

a10 = xy + ct*(-xy) + st*v.z;
a11 = yy + ct*(1-yy);
a12 = yz + ct*(-yz) + st*-v.x;

a20 = xz + ct*(-xz) + st*-v.y;
a21 = yz + ct*(-yz) + st*v.x;
a22 = zz + ct*(1-zz);
}*/

/*void mat4::set_rot(const vec3& u, const vec3& v)
{
float phi;
float h;
float lambda;
vec3 w;

cross(w,u,v);
dot(phi,u,v);
dot(lambda,w,w);
if (lambda > nv_eps)
h = (nv_one - phi) / lambda;
else
h = lambda;

float hxy = w.x * w.y * h;
float hxz = w.x * w.z * h;
float hyz = w.y * w.z * h;

a00 = phi + w.x * w.x * h;
a01 = hxy - w.z;
a02 = hxz + w.y;

a10 = hxy + w.z;
a11 = phi + w.y * w.y * h;
a12 = hyz - w.x;

a20 = hxz - w.y;
a21 = hyz + w.x;
a22 = phi + w.z * w.z * h;
}*/


void mat4::set_translation(const vec3& t)
{
	a03 = t.x;
	a13 = t.y;
	a23 = t.z;
}

void mat4::set_scale(const vec3& s)
{
	a00 = s.x;
	a11	= s.y;
	a22 = s.z;
}


vec3 & mat4::get_translation(vec3& t) const
{
	t.x = a03;
	t.y = a13;
	t.z = a23;
	return t;
}


quat & mat4::get_rot(quat& q) const
{
	float tr = a00 + a11 + a22;

	// check the diagonal
	if ( tr > nv_zero)
	{
		// Diagonal is positive.
		float s = sqrtf(tr + nv_one);
		q.w = s * nv_zero_5;
		s = nv_zero_5 / s;
		q.x = (a12 - a21) * s;
		q.y = (a20 - a02) * s;
		q.z = (a01 - a10) * s;
	} // have positive trace
	else
	{
		// Diagonal is negative.
		int nxt[3] = {1, 2, 0};

		// Find the largest diagonal.
		int i = 0;
		if ( a11 > a00 )
			i = 1;
		if ( a22 > (*this)[i][i] )
			i = 2;
		int j = nxt[i];
		int k = nxt[j];

		float s = sqrtf(( (*this)[i][i] - ( (*this)[j][j] + (*this)[k][k] )) + nv_one );

		q[i] = s * nv_zero_5;
		s = nv_zero_5 / s;

		q.w  = ((*this)[j][k] - (*this)[k][j]) * s;
		q[j] = ((*this)[i][j] + (*this)[j][i]) * s;
		q[k] = ((*this)[i][k] + (*this)[k][i]) * s;
	} // have negative or zero trace
	return q;
}



mat4 & look_at(mat4& M, const vec3& eye, const vec3& center, const vec3& up)
{
    vec3 x, y, z;

    // make rotation matrix

    // Z vector
    z.x = eye.x - center.x;
    z.y = eye.y - center.y;
    z.z = eye.z - center.z;
    normalize(z);

    // Y vector
    y.x = up.x;
    y.y = up.y;
    y.z = up.z;

    // X vector = Y cross Z
    cross(x,y,z);

    // Recompute Y = Z cross X
    cross(y,z,x);

    // cross product gives area of parallelogram, which is < 1.0 for
    // non-perpendicular unit-length vectors; so normalize x, y here
    normalize(x);
    normalize(y);

    M.a00 = x.x; M.a01 = x.y; M.a02 = x.z; M.a03 = -x.x * eye.x - x.y * eye.y - x.z*eye.z;
    M.a10 = y.x; M.a11 = y.y; M.a12 = y.z; M.a13 = -y.x * eye.x - y.y * eye.y - y.z*eye.z;
    M.a20 = z.x; M.a21 = z.y; M.a22 = z.z; M.a23 = -z.x * eye.x - z.y * eye.y - z.z*eye.z;
    M.a30 = nv_zero; M.a31 = nv_zero; M.a32 = nv_zero; M.a33 = nv_one;
    return M;
}

mat4 & frustum(mat4& M, const float l, const float r, const float b,
               const float t, const float n, const float f)
{
    M.a00 = (nv_two*n) / (r-l);
    M.a10 = 0.0;
    M.a20 = 0.0;
    M.a30 = 0.0;

    M.a01 = 0.0;
    M.a11 = (nv_two*n) / (t-b);
    M.a21 = 0.0;
    M.a31 = 0.0;

    M.a02 = (r+l) / (r-l);
    M.a12 = (t+b) / (t-b);
    M.a22 = -(f+n) / (f-n);
    M.a32 = -nv_one;

    M.a03 = 0.0;
    M.a13 = 0.0;
    M.a23 = -(nv_two*f*n) / (f-n);
    M.a33 = 0.0;
    return M;
}

mat4 & perspective(mat4& M, const float fovy, const float aspect, const float n, const float f)
{
    float xmin, xmax, ymin, ymax;

    ymax = n * tanf(fovy * nv_zero_5);
    ymin = -ymax;

    xmin = ymin * aspect;
    xmax = ymax * aspect;

    return frustum(M, xmin, xmax, ymin, ymax, n, f);
}


mat4 & ortho2D(mat4 & M, const float left, const float right, const float bottom, const float top)
{
	float f	=  1.0f;	//far
	float n	= -1.0f;	//near

	M.a00 = 2.0f/(right-left);
	M.a10	= 0.0f;
	M.a20	= 0.0f;
	M.a30	= 0.0f;

	M.a01 = 0.0f;
	M.a11	= 2.0f/(top-bottom);
	M.a21	= 0.0f;
	M.a31	= 0.0f;

	M.a02	= 0.0f;
	M.a12	= 0.0f;
	M.a22	= -2.0f/(f-n);
	M.a32	= 0.0f;

	M.a03	= - ((right+left) / (right-left));
	M.a13	= - ((top+bottom) / (top-bottom));
	M.a23	= - ((f+n) / (f-n));
	M.a33	= 1.0f;

	return M;
}

bool unproject(vec3& out, vec3& v, mat4& proj, mat4& view, mat4& world, int w, int h)
{
	vec3 a;
	a.x = ((2.0f*v.x)/w)-1.0f;
	a.y = ((2.0f*v.y)/h)-1.0f;
	a.z = (2.0f*v.z)-1.0f;

	mat4 m, inv;
	mult(m, proj, view);
	mult(m, m, world);
	//m = proj*view*world;
	invert(inv, m);

	//mult_pos(out, inv, a);

	out.x = inv.a00 * a.x + inv.a01 * a.y + inv.a02 * a.z + inv.a03 * 1.0f;
	out.y = inv.a10 * a.x + inv.a11 * a.y + inv.a12 * a.z + inv.a13 * 1.0f;
	out.z = inv.a20 * a.x + inv.a21 * a.y + inv.a22 * a.z + inv.a23 * 1.0f;
float ow = inv.a30 * a.x + inv.a31 * a.y + inv.a32 * a.z + inv.a33 * 1.0f;
	if (ow<10e-3)		return false;

	out.x = out.x/ow;
	out.y = out.y/ow;
	out.z = out.z/ow;

	return true;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							quat struct implementation
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------


const quat quat::Identity(0, 0, 0, 1);

quat::quat() : x(0), y(0), z(0), w(1){}

quat::quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}


quat quat::Inverse()
{
	return quat(- x, - y, - z, w);
}

void quat::Normalize()
{
	float len = sqrtf(x * x + y * y + z * z + w * w);
	if (len > 0) {
		float invLen = 1 / len;
		x *= invLen;
		y *= invLen;
		z *= invLen;
		w *= invLen;
	}
}

quat & mat_2_quat(quat& q,const mat4& M)
{
    float tr = M.a00 + M.a11 + M.a22;

    // check the diagonal
    if ( tr > nv_zero)
    {
        // Diagonal is positive.
        float s = sqrtf(tr + nv_one);
        q.w = s * nv_zero_5;
        s = nv_zero_5 / s;
        q.x = (M.a21 - M.a12) * s;
        q.y = (M.a02 - M.a20) * s;
        q.z = (M.a10 - M.a01) * s;
    } // have positive trace
    else
    {
        // Diagonal is negative.
        int nxt[3] = {1, 2, 0};

        // Find the largest diagonal.
        int i = 0;
        if ( M.a11 > M.a00 )
            i = 1;
        if ( M.a22 > M[i][i] )
            i = 2;
        int j = nxt[i];
        int k = nxt[j];

        float s = sqrtf( M[i][i] - ( M[j][j] + M[k][k] )) + nv_one;

        q[i] = s * nv_zero_5;
        s = nv_zero_5 / s;

        q.w  = (M[k][j] - M[j][k]) * s;
        q[j] = (M[j][i] + M[i][j]) * s;
        q[k] = (M[k][i] + M[i][k]) * s;
    } // have negative or zero trace
    return q;
}

quat & normalize(quat & p)
{
	float norm = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z + p.w * p.w);
	if (norm > nv_eps)
		norm = nv_one / norm;
	else
		norm = nv_zero;
	p.x *= norm;
	p.y *= norm;
	p.z *= norm;
	p.w *= norm;
	return p;
}

/*
    Given an axis and angle, compute quaternion.
 */
quat & axis_to_quat(quat& q, const vec3& a, const float phi)
{
	vec3 tmp;//(a.x, a.y, a.z);
	tmp.x=a.x; tmp.y=a.y; tmp.z=a.z;

	normalize(tmp);
	float s = sinf(phi/nv_two);
	q.x = s * tmp.x;
	q.y = s * tmp.y;
	q.z = s * tmp.z;
	q.w = cosf(phi/nv_two);
	return q;
}

quat & conj(quat & p)
{
	p.x = -p.x;
	p.y = -p.y;
	p.z = -p.z;
	return p;
}

 quat & conj(quat& p, const quat& q)
{
    p.x = -q.x;
    p.y = -q.y;
    p.z = -q.z;
    p.w = q.w;
    return p;
}

 quat & add_quats(quat& p, const quat& q1, const quat& q2)
{
    quat t1, t2;

    t1 = q1;
    t1.x *= q2.w;
    t1.y *= q2.w;
    t1.z *= q2.w;

    t2 = q2;
    t2.x *= q1.w;
    t2.y *= q1.w;
    t2.z *= q1.w;

    p.x = (q2.y * q1.z) - (q2.z * q1.y) + t1.x + t2.x;
    p.y = (q2.z * q1.x) - (q2.x * q1.z) + t1.y + t2.y;
    p.z = (q2.x * q1.y) - (q2.y * q1.x) + t1.z + t2.z;
    p.w = q1.w * q2.w - (q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);

    return p;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//
//							Functions
//
//-----------------------------------------------------------------
//-----------------------------------------------------------------
const int HALF_RAND = (RAND_MAX / 2);

 float nv_random()
{
	return ((float)(rand() - HALF_RAND) / (float)HALF_RAND);
}


float ffast_cos(const float x)
{
    // assert:  0 <= fT <= PI/2
    // maximum absolute error = 1.1880e-03
    // speedup = 2.14

    float x_sqr = x*x;
    float res = float(3.705e-02);
    res *= x_sqr;
    res -= float(4.967e-01);
    res *= x_sqr;
    res += nv_one;
    return res;
}


float fast_cos(const float x)
{
    // assert:  0 <= fT <= PI/2
    // maximum absolute error = 2.3082e-09
    // speedup = 1.47

    float x_sqr = x*x;
    float res = float(-2.605e-07);
    res *= x_sqr;
    res += float(2.47609e-05);
    res *= x_sqr;
    res -= float(1.3888397e-03);
    res *= x_sqr;
    res += float(4.16666418e-02);
    res *= x_sqr;
    res -= float(4.999999963e-01);
    res *= x_sqr;
    res += nv_one;
    return res;
}


float spline(float t, float* a, float* b, float* c, float* d)
{
  float sp_t,sp_t2,sp_t3;
  float k1,k2,k3,k4;

  sp_t=(t-a[0])/(b[0]-c[0]);
  sp_t2=sp_t*sp_t;
  sp_t3=sp_t2*sp_t;

  k1=(((-1.0f)/(6.0f)) * sp_t3 + (1.0f/2.0f) * sp_t2 - (1.0f/2.0f)*sp_t + (1.0f/6.0f));
  k2=(((1.0f)/(2.0f)) * sp_t3 - sp_t2 + (2.0f/3.0f));
  k3=(((-1.0f)/(2.0f)) * sp_t3 + (1.0f/2.0f) * sp_t2 + (1.0f/2.0f) * sp_t + (1.0f/6.0f));
  k4=(((1.0f)/(6.0f)) * sp_t3);

  return (a[1]*k1+b[1]*k2+c[1]*k3+d[1]*k4);
}


unsigned int gdwSeedValue;
void SetSeedValue(unsigned int dwSeedValue)
{
	gdwSeedValue = dwSeedValue;
}


unsigned int myRandom(int mode)
{
	gdwSeedValue*=0x15a4e35;

	switch(mode)
	{
		case 1:
			gdwSeedValue *= 19;
			gdwSeedValue += gdwSeedValue % 17;
			return gdwSeedValue>>16;
		break;
		case 0:	default:
			return gdwSeedValue>>16;
		break;
	}
}
