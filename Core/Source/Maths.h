//-----------------------------------------------------------------
//-----------------------------------------------------------------
//! \file		Maths.h
//! \brief	mathematics
//!
//!	\author	Johann Nadalutti (fxgen@free.fr) (based on nvidia sdk)
//!				Sebastian Olter (qduaty@gmail.com)
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
#ifndef MATHS_H
#define MATHS_H

//-----------------------------------------------------------------
//                   Defines
//-----------------------------------------------------------------
#define nv_zero			      float(0)
#define nv_zero_5         float(0.5)
#define nv_one			      float(1.0)
#define nv_two			      float(2)
#define nv_half_pi        float(3.14159265358979323846264338327950288419716939937510582 * 0.5)
#define nv_pi						  float(3.14159265358979323846264338327950288419716939937510582)
#define nv_two_pi					float(3.14159265358979323846264338327950288419716939937510582 * 2.0)
#define nv_oo_pi					nv_one / nv_pi
#define nv_oo_two_pi	    nv_one / nv_two_pi
#define nv_oo_255   	    nv_one / float(255)
#define nv_oo_128   	    nv_one / float(128)
#define nv_to_rad         nv_pi / float(180)
#define nv_to_deg         float(180) / nv_pi
#define nv_eps		        float(10e-6)
#define nv_double_eps	    float(10e-6) * nv_two
#define nv_big_eps        float(10e-6)
#define nv_small_eps      float(10e-2)

//-----------------------------------------------------------------
// prototypes
//-----------------------------------------------------------------
struct vec2;
struct vec3;
struct vec4;
struct quat;
struct mat4;

//--------------------------------
// CRT
//--------------------------------
//double __stdcall sin(double d);
/*double __stdcall sqrt(double f);
double __stdcall cos(double d);
double __stdcall tan(double a);
double __stdcall atan(double a);
double __stdcall atan2(double d, double b);
double __stdcall asin(double d);
double __stdcall acos(double d);
double __stdcall pow(double d, double e);
long _declspec () _ftol ();*/

//-----------------------------------------------------------------
//! \struct vec3
//! \brief	vector 3 dimensions
//-----------------------------------------------------------------
struct CORE_EXPORT vec3
{
	vec3() { }
	vec3(float X, float Y, float Z):x(X),y(Y),z(Z) { }

	bool operator==(const vec3 & u) const
	{
			return (u.x == x && u.y == y && u.z == z) ? true : false;
	}

	bool operator!=( const vec3& rhs ) const
	{
		return !(*this == rhs );
	}

	float normalize();
	float sq_norm() const { return x * x + y * y + z * z; }
	float norm() const { float Norm = sq_norm(); return Norm ? sqrtf(Norm) : 0; }

	//! Angle between (1,0,0) and the horizontal cast of this vector.
	float azimuth()
	{
		if(x == 0)
		{
			if(y > 0)
				return nv_pi / 2;
			else
				return 1.5f * nv_pi;
		}
		else if(x < 0)
			return atan(y / x) + nv_pi;
		else if(y < 0)
			return atan(y / x) + 2 * nv_pi;
		else
			return atan(y / x);
	}

	//! Angle between this vector and its horizontal cast [-pi/2, pi/2].
	float elevation()
	{
		float n = x * x + y * y;
		if(n == 0)
		{
			if(z > 0)
				return nv_pi / 2;
			else
				return 1.5f * nv_pi;
		}
		else // n > 0
		{
			n = sqrtf(n);
			if(z >= 0)
				return atan(z / n);
			else
				return atan(z / n) + 2 * nv_pi;
		}
	}


	float & operator[](int i)
	{
		return vec_array[i];
	}

	const float operator[](int i) const
	{
		return vec_array[i];
	}

	union {
		struct {
			float x,y,z;        // standard names for components
		};
		struct {
			float s,t,r;        // standard names for components
		};
		float vec_array[3];     // array access
	};

};

//-----------------------------------------------------------------
//! \struct vec4
//! \brief	vector 4 dimensions
//-----------------------------------------------------------------
struct CORE_EXPORT vec4
{
	vec4() { }
	vec4(float X, float Y, float Z, float W):x(X),y(Y),z(Z),w(W) { }

	union {
		struct {
			float x,y,z,w;        // standard names for components
		};
		struct {
			float x1,y1,x2,y2;        // standard names for components
		};
		float vec_array[4];     // array access
	};

};

//-----------------------------------------------------------------
//! \struct vec2
//! \brief	vector 2 dimensions
//-----------------------------------------------------------------
struct CORE_EXPORT vec2
{
	vec2() { }
	vec2(float X, float Y):x(X),y(Y) { }

	union {
		struct {
			float x,y;        // standard names for components
		};
		struct {
			float x1,y1;        // standard names for components
		};
		float vec_array[2];     // array access
	};

};


//-----------------------------------------------------------------
//! \struct mat4
//! \brief	matrix 4x4
//-----------------------------------------------------------------
struct CORE_EXPORT mat4
{
  mat4()	{ }


  const vec3 operator[](int i) const
  {
		vec3 v;
		v.x = mat_array[i];	v.y=mat_array[i + 3]; v.z=mat_array[i + 6];
    return v;
  }

  const float& operator()(const int& i, const int& j) const
  {
		return mat_array[ j * 4 + i ];
  }

  float& operator()(const int& i, const int& j)
  {
		return  mat_array[ j * 4 + i ];
  }

  quat & get_rot(quat & q) const;

  void set_rot(const quat & q);
	void set_rot(const vec3 & a);		// Angles Euler X,Y,Z en radian
	void set_scale(const vec3& s);
  void set_translation(const vec3 & t);
  vec3 & get_translation(vec3 & t) const;

  union {
		struct {
			float a00, a10, a20, a30;   // standard names for components
			float a01, a11, a21, a31;   // standard names for components
			float a02, a12, a22, a32;   // standard names for components
			float a03, a13, a23, a33;   // standard names for components
		};
		struct {
			float _11, _12, _13, _14;   // standard names for components
			float _21, _22, _23, _24;   // standard names for components
			float _31, _32, _33, _34;   // standard names for components
			float _41, _42, _43, _44;   // standard names for components
		};
		union {
			struct {
				float b00, b10, b20, p; // standard names for components
				float b01, b11, b21, q; // standard names for components
				float b02, b12, b22, r; // standard names for components
				float x, y, z, w;       // standard names for components
			};
		};
		float mat_array[16];     // array access
  };
};


//-----------------------------------------------------------------
//! \struct quat
//! \brief	quaternion
//-----------------------------------------------------------------
struct CORE_EXPORT quat
{
public:
	quat();
	quat(float x, float y, float z, float w);
	quat(float phi, vec3 axis)
	{
		w = cosf(phi / 2);
		float s = sinf(phi / 2);
		axis.normalize();
		x = s * axis.x;
		y = s * axis.y;
		z = s * axis.z;
	}

	quat Inverse();
	void Normalize();

	static const quat Identity;
	float& operator[](int i) { return comp[i]; }
	const float operator[](int i) const { return comp[i]; }
	union {
		struct {
			float x, y, z, w;
		};
		float comp[4];
	};

	quat operator*(float coeff)
	{
		quat result;

		result.w = w * coeff;
		result.x = x * coeff;
		result.y = y * coeff;
		result.z = z * coeff;

		return result;
	}

	void operator*=(float coeff)
	{
		w = w * coeff;
		x = x * coeff;
		y = y * coeff;
		z = z * coeff;
	}

	quat operator*(const quat& other)
	{
		quat result;

		result.w = w * other.w - x * other.x - y * other.y - z * other.z;
		result.x = w * other.x + x * other.w + z * other.y - y * other.z;
		result.y = w * other.y + y * other.w + x * other.z - z * other.x;
		result.z = w * other.z + z * other.w + y * other.x - x * other.y;

		return result;
	}

	quat operator+(const quat& other)
	{
		quat result;

		result.x = x + other.x;
		result.y = y + other.y;
		result.z = z + other.z;
		result.w = w + other.w;

		return result;
	}

	void operator+=(const quat& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
	}
};


//-----------------------------------------------------------------
//
//								constant algebraic values
//
//-----------------------------------------------------------------
/*const float array16_id[] =        { nv_one, nv_zero, nv_zero, nv_zero,
                                    nv_zero, nv_one, nv_zero, nv_zero,
                                    nv_zero, nv_zero, nv_one, nv_zero,
                                    nv_zero, nv_zero, nv_zero, nv_one};*/

/*const float array16_null[] =      { nv_zero, nv_zero, nv_zero, nv_zero,
                                    nv_zero, nv_zero, nv_zero, nv_zero,
                                    nv_zero, nv_zero, nv_zero, nv_zero,
                                    nv_zero, nv_zero, nv_zero, nv_zero};*/

/*const float array16_scale_bias[] = {nv_zero_5, nv_zero,   nv_zero,   nv_zero,
	                                  nv_zero,   nv_zero_5, nv_zero,   nv_zero,
		                                nv_zero,   nv_zero,   nv_zero_5, nv_zero,
			                              nv_zero_5, nv_zero_5, nv_zero_5, nv_one};*/


//-----------------------------------------------------------------
//
//											Fonctions
//
//-----------------------------------------------------------------

/*const vec3      vec3_one(nv_one,nv_one,nv_one);
const vec3      vec3_null(nv_zero,nv_zero,nv_zero);
const vec3      vec3_x(nv_one,nv_zero,nv_zero);
const vec3      vec3_y(nv_zero,nv_one,nv_zero);
const vec3      vec3_z(nv_zero,nv_zero,nv_one);
const vec3      vec3_neg_x(-nv_one,nv_zero,nv_zero);
const vec3      vec3_neg_y(nv_zero,-nv_one,nv_zero);
const vec3      vec3_neg_z(nv_zero,nv_zero,-nv_one);
const quat      quat_id(nv_zero,nv_zero,nv_zero,nv_one);
const mat4      mat4_id(array16_id);
const mat4      mat4_null(array16_null);
const mat4      mat4_scale_bias(array16_scale_bias);*/

///////////////////////////////////////////////////////////
//	Vector

// normalizes a vector and return a reference of itself
extern CORE_EXPORT vec3 & normalize(vec3 & u);

// Computes the squared magnitude
inline float nv_sq_norm(const vec3 & n)
{ return n.x * n.x + n.y * n.y + n.z * n.z; }

// Computes the magnitude
inline float nv_norm(const vec3 & n)
{ return sqrtf(nv_sq_norm(n)); }

// linear interpolation
inline float lerp(float t, float a, float b)
{ return a * (nv_one - t) + t * b; }

inline vec3 & lerp(vec3 & w, const float & t, const vec3 & u, const vec3 & v)
{ w.x = lerp(t, u.x, v.x); w.y = lerp(t, u.y, v.y); w.z = lerp(t, u.z, v.z); return w; }

// computes the cross product ( v cross w) and stores the result in u
// i.e.     u = v cross w
extern CORE_EXPORT vec3 & cross(vec3 & u, const vec3 & v, const vec3 & w);

// computes the dot product ( v dot w) and stores the result in u
// i.e.     u = v dot w
extern CORE_EXPORT float & dot(float & u, const vec3 & v, const vec3 & w);

// compute the reflected vector R of L w.r.t N - vectors need to be
// normalized
//
//                R     N     L
//                  _       _
//                 |\   ^   /|
//                   \  |  /
//                    \ | /
//                     \|/
//                      +
extern CORE_EXPORT vec3 & reflect(vec3 & r, const vec3 & n, const vec3 & l);

// Computes u = v * lambda + u
extern CORE_EXPORT vec3 & madd(vec3 & u, const vec3 & v, const float & lambda);
// Computes u = v * lambda
extern CORE_EXPORT vec3 & mult(vec3 & u, const vec3 & v, const float & lambda);
// Computes u = v * w
extern CORE_EXPORT vec3 & mult(vec3 & u, const vec3 & v, const vec3 & w);
// Computes u = v + w
extern CORE_EXPORT vec3 & add(vec3 & u, const vec3 & v, const vec3 & w);
// Computes u = v - w
extern CORE_EXPORT vec3 & sub(vec3 & u, const vec3 & v, const vec3 & w);

// Computes u = u * s
extern CORE_EXPORT vec3 & scale(vec3 & u, const float s);

// Computes u = M(4x4) * v and devides by w
extern CORE_EXPORT vec3 & mult_pos(vec3 & u, const mat4 & M, const vec3 & v);
// Computes u = M(4x4) * v
extern CORE_EXPORT vec3 & mult_dir(vec3 & u, const mat4 & M, const vec3 & v);



///////////////////////////////////////////////////////////
//	Matrix

// Computes A += B
extern CORE_EXPORT mat4 & add(mat4 & A, const mat4 & B);

// Computes C = A * B
extern CORE_EXPORT mat4 & mult(mat4 & C, const mat4 & A, const mat4 & B);

// Computes B = Transpose(A)
//       T
//  B = A
extern CORE_EXPORT mat4 & transpose(mat4 & B, const mat4 & A);
extern CORE_EXPORT mat4 & transpose(mat4 & B);

// Computes B = inverse(A)
//       -1
//  B = A
extern CORE_EXPORT mat4 & invert(mat4 & B, const mat4 & A);

// Computes B = inverse(A)
//                                       T  T
//                   (R t)             (R -R t)
// assuming that A = (0 1) so that B = (0    1)
//  B = A
extern CORE_EXPORT mat4 & invert_rot_trans(mat4 & B, const mat4 & A);

extern CORE_EXPORT mat4 & look_at(mat4 & M, const vec3 & eye, const vec3 & center, const vec3 & up);
extern CORE_EXPORT mat4 & frustum(mat4 & M, const float l, const float r, const float b,
               const float t, const float n, const float f);

extern CORE_EXPORT mat4 & perspective(mat4 & M, const float fovy, const float aspect, const float n, const float f);

extern CORE_EXPORT mat4	& ortho2D(mat4 & M, const float left, const float right, const float bottom, const float top);

extern CORE_EXPORT bool	unproject(vec3& out, vec3& v, mat4& proj, mat4& view, mat4& world, int w, int h);

///////////////////////////////////////////////////////////
// quaternion
extern CORE_EXPORT quat & normalize(quat & p);
extern CORE_EXPORT quat & conj(quat & p);
extern CORE_EXPORT quat & conj(quat & p, const quat & q);
extern CORE_EXPORT quat & add_quats(quat & p, const quat & q1, const quat & q2);
extern CORE_EXPORT quat & axis_to_quat(quat & q, const vec3 & a, const float phi);
extern CORE_EXPORT quat & mat_2_quat(quat &q,const mat4 &M);

///////////////////////////////////////////////////////////
// Utilities


// spline interpolation
float spline(float t, float* a, float* b, float* c, float* d);

// utilities
inline float nv_min(const float & lambda, const float & n)
{ return (lambda < n ) ? lambda : n; }

inline float nv_max(const float & lambda, const float & n)
{ return (lambda > n ) ? lambda : n; }

inline float nv_clamp(float u, const float min, const float max)
{ u = (u < min) ? min : u; u = (u > max) ? max : u; return u; }

extern CORE_EXPORT float nv_random();

// fast cosine functions
extern CORE_EXPORT float fast_cos(const float x);
extern CORE_EXPORT float ffast_cos(const float x);


// At the assembly level the recommended workaround for the second FIST bug is the same for the first;
// inserting the FRNDINT instruction immediately preceding the FIST instruction.
/*__forceinline void FloatToInt(int *int_pointer, float f)
{
#ifdef __GNUC__
	*int_pointer = int(f + .5);
#else
	__asm  fld  f
  __asm  mov  edx,int_pointer
  __asm  FRNDINT
  __asm  fistp dword ptr [edx];
#endif
}*/

extern CORE_EXPORT void SetSeedValue(unsigned int dwSeedValue);
extern CORE_EXPORT unsigned int  myRandom(int=0);

#endif //MATHS_H
