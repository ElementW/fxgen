
#ifndef _3DMATH_H
#define _3DMATH_H

#include "windows.h"
#include <stdlib.h>
#include "math.h"
#include "float.h"

//#define _USED3D
#ifdef _USED3D
#include <d3d9.h>
#include <d3dx9.h>
#endif

// TODO: put all of this inside a namespace,
// so that I can look up functions using MATH::
// namepace MATH { 

#ifndef __MATH_MACROS_AND_CONSTANTS
#define __MATH_MACROS_AND_CONSTANTS

//-----------------------------------------------------------------------------
// Globals and Macros 
//-----------------------------------------------------------------------------
#define LERP(m,x0,x1)       ((x0) + (m)*((x1)-(x0)))      
#define POW(x)              ( x*x )
#define RMAG(v)             ( POW(v.x) + POW(v.y) + POW(v.z) )
#define PMAG(v)             ( POW(v->x) + POW(v->y) + POW(v->z) )
#define PDOT( v1, v2 )      ( v1->x*v2->x + v1->y*v2->y + v1->z*v2->z )
#define RDOT( v1, v2 )      ( (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z) )
#define rnd()               (((float)rand()-(float)rand())/(2L*RAND_MAX))
#define RND()               (((float)rand())/RAND_MAX)


//#define epsilon FLT_EPSILON     
const float epsilon = (const float)1e-6;

/* needs a damping */

const float infinity = FLT_MAX/10;

//inline float fmax( float a, float b) { return (a>b?a:b); }  

static inline float DegToRad(float a) { return a*0.01745329252f; };
static inline float RadToDeg(float a) { return a*57.29577951f; };

#define fmax( a, b) (a>b?a:b)
#define flabs( f )  (f>=0.0f?f:-f)

#define IsZero(f) (flabs(f)<epsilon)
#define IsPositive(f) (f>epsilon)
#define IsNegative(f) (f<-epsilon)


//#define DegToRad(a)  

#ifndef PI
#ifdef _USED3D
#define PI  D3DX_PI
#endif
#ifndef _USED3D
#define PI  3.14159265359 // TODO : Get a pi
#endif
#endif

#endif // __MATH_MACROS_AND_CONSTANTS



#ifndef __VECTOR2
#define __VECTOR2

class Vec2
{
public:

	Vec2( float X, float Y )
	{
		x = X;
		y = Y;
	}
	float x,y;
};

#endif


#ifndef __VECTOR3
#define __VECTOR3

//-----------------------------------------------------------------------------
// Vec3 
//-----------------------------------------------------------------------------
 
class Vec3
#ifdef _USED3D
: public D3DXVECTOR3
#endif
{
public:
	

	//-----------------------------------------------------------------------------
	// Constructors/Destructor
	//-----------------------------------------------------------------------------
	
	Vec3();
	
	Vec3
	( 
		float X, 
		float Y, 
		float Z 
	);
	
	Vec3( Vec3 &rhs );
	
	Vec3( Vec3 *ptr );
	
	virtual ~Vec3();


	//-----------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------
	
	bool operator==( const Vec3 &rhs );

	bool operator!=( const Vec3 &rhs );

	void operator+=( const Vec3 &rhs );
	
	void operator-=( const Vec3 &rhs );

	void operator+=( const float P );

	void operator-=( const float P );
	
	void operator*=( const float P );
	
	void operator/=( const float P );

#ifdef _USED3D
	
	void operator+=( const D3DXVECTOR3 &rhs );

	void operator-=( const D3DXVECTOR3 &rhs );

#endif


	//-----------------------------------------------------------------------------
	// Inline Operators
	//-----------------------------------------------------------------------------
	
	inline Vec3 operator + ( const Vec3& v ) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}
	
	inline Vec3 operator - ( const Vec3& v ) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}
	
	inline Vec3 operator * ( float f ) const
	{
		return Vec3(x * f, y * f, z * f);
	}
	
	inline Vec3 operator / ( float f ) const
	{
	    float fInv = 1.0f / f;
		return Vec3(x * fInv, y * fInv, z * fInv);
	}
	
	Vec3 operator = ( Vec3 &v ) 
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return(*this);
	}

	float& operator [] ( int i )
	{
		if( i == 0 ) return x;
		if( i == 1 ) return y;
		if( i == 2 ) return z;
	}

#ifdef _USED3D	

	inline Vec3 operator + ( const D3DXVECTOR3& v ) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	inline Vec3 operator - ( const D3DXVECTOR3& v ) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	void operator = ( D3DXVECTOR3 &v ) 
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

#endif


	//-----------------------------------------------------------------------------
	// Methods
	//-----------------------------------------------------------------------------

	Vec3 XAxis(){ return Vec3( 1.0f, 0.0f, 0.0f ); }

	Vec3 YAxis(){ return Vec3( 0.0f, 1.0f, 0.0f ); }

	Vec3 ZAxis(){ return Vec3( 0.0f, 0.0f, 1.0f ); }
	
	
	void Set( float _x, float _y, float _z )
	{
		x = _x;  y = _y;  z = _z;
	}

	inline float Dot( const Vec3 &rhs )
	{
		return x*rhs.x + 
			   y*rhs.y +
			   z*rhs.z;
	}
	
	Vec3 CrossProduct( Vec3 &out, const Vec3 &rhs );
	Vec3 Cross( const Vec3 &v1, const Vec3 &v2 );
	
	inline float Magnitude();
	
	void Normalize();

#ifdef _USED3D

	inline float Dot( const D3DXVECTOR3 &rhs );

	D3DXVECTOR3  CrossProduct( D3DXVECTOR3 &out, const D3DXVECTOR3 &rhs );

#endif
#ifndef _USED3D


	//-----------------------------------------------------------------------------
	// Data Members
	//-----------------------------------------------------------------------------

	float x, y, z;

#endif

};


//-----------------------------------------------------------------------------
// Vec3 support functions - these form the bulk of the class 
// functionality since they provide compatibility with the D3DXVECTOR3
//-----------------------------------------------------------------------------

void Add( Vec3 &vout, const Vec3 &v1, const Vec3 &v2 );
float Dot( const Vec3 &v1, const Vec3 &v2 );
void Cross( Vec3 &out, const Vec3 &v1, const Vec3 &v2 );
void NormalizeInto( Vec3 &vout, const Vec3 &vin );
float Magnitude( const Vec3& v );
float Distance( Vec3 &v1, Vec3 &v2 );

inline Vec3 normalize( Vec3 v ){ v.Normalize(); return v; }

//Vec3 cross( Vec3 &v1, Vec3 &v2 );//{ return v1.Cross(v1, v2); } 


inline float AngleBetweenVectors( Vec3 v1, Vec3 v2 )
{
	return (float)acos(Dot(v1,v2)/(Magnitude(v1)*Magnitude(v2)));
}

inline Vec3& operator+( Vec3 &v, Vec3 &w )
{
	v.x += w.x;
	v.y += w.y;
	v.z += w.z;
	return v;
}

inline Vec3& operator+( Vec3 &v, float w )
{
	v.x += w;
	v.y += w;
	v.z += w;
	return v;
}

inline Vec3& operator*( Vec3 &v, Vec3 &w )
{
	v.x *= w.x;
	v.y *= w.y;
	v.z *= w.z;
	return v;
}



#ifdef _USED3D

void Add( Vec3 &vout, const D3DXVECTOR3 &v1, const D3DXVECTOR3 &v2 );
float Dot( const Vec3 &v1, const D3DXVECTOR3 &v2 );
float Dot( const D3DXVECTOR3 &v1, const Vec3 &v2 );
void Cross( Vec3 &out, const Vec3 &v1, const D3DXVECTOR3 &v2 );
void Cross( Vec3 &out, const D3DXVECTOR3 &v1, const Vec3 &v2 );
void Cross( Vec3 &out, const D3DXVECTOR3 &v1, const D3DXVECTOR3 &v2 );
void NormalizeInto( Vec3 &vout, const D3DXVECTOR3 &vin );

#endif


class Vec4
#ifdef _USED3D
: public D3DXVECTOR4
#endif
{
public:
	Vec4()
	{
		x = y = z = 0.0F;
	}
	Vec4( float X, float Y, float Z, float W )
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
	Vec4( Vec4 &rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}

	void operator+=( const Vec4 &rhs );
	
	void operator-=( const Vec4 &rhs );

	void operator+=( const float P );

	void operator-=( const float P );
	
	void operator*=( const float P );
	
	void operator/=( const float P );

#ifdef _USED3D
	
	void operator+=( const D3DXVECTOR4 &rhs );

	void operator-=( const D3DXVECTOR4 &rhs );

#endif

	inline float Dot( const Vec4 &rhs )
	{
		return ( x*rhs.x +y*rhs.y +z*rhs.z +w*rhs.w );
	}
	
	Vec4 CrossProduct( Vec4 &out, const Vec4 &rhs );
	
	inline float Magnitude()
	{
		return sqrt( this->Dot( *this ));
	}
	
	void Normalize()
	{
		float mag_recip = 1/Magnitude();
		x *= mag_recip;
		y *= mag_recip;
		z *= mag_recip;
	}

#ifdef _USED3D

	inline float Dot( const D3DXVECTOR4 &rhs )
	{
		return ( x*rhs.x +y*rhs.y +z*rhs.z +w*rhs.w );
	}

	D3DXVECTOR4 CrossProduct( D3DXVECTOR4 &out, const D3DXVECTOR4 &rhs );

#endif

#ifndef _USED3D

	float x, y, z, w;

#endif

};


#endif  // __VECTOR

#ifndef __RAY
#define __RAY

struct Ray
{
	Vec3 orig;
	Vec3 dir;

	enum OUPT
	{
		MISS,
		HIT,
		INPRIM
	};
	int IntersectSphere( Vec3 &center, float radius, float &dist )
	{
		Vec3 v = orig - center;
		float b = -v.Dot( dir );
		float det = (b * b) - v.Dot( v ) + radius*radius;
		int retval = (int)MISS;
		if (det > 0)
		{
			det = sqrt( det );
			float i1 = b - det;
			float i2 = b + det;
			if (i2 > 0)
			{
				if (i1 < 0) 
				{
					if (i2 < dist) 
					{
						dist = i2;
						retval = (int)INPRIM;
					}
				}
				else
				{
					if (i1 < dist)
					{
						dist = i1;
						retval = (int)HIT;
					}
				}
			}
		}
		return retval;
	}
};

#endif //__RAY




#ifndef __MATRIX44
#define __MATRIX44
//-----------------------------------------------------------------------------
// Mat44
//-----------------------------------------------------------------------------


class Mat44 
#ifdef _USED3D
: public D3DXMATRIXA16
#endif
{
public:


	//-----------------------------------------------------------------------------
	// Constructors/Destructor
	//-----------------------------------------------------------------------------

	Mat44();
	
	Mat44
	( 
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44 
	);
	
	Mat44( Mat44 &rhs );
	
	Mat44( Mat44 *ptr );
	
	virtual ~Mat44();


	//-----------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------
	
	void operator+=( Mat44 &rhs );
	
	void operator-=( Mat44 &rhs );
	
	void operator*=( Mat44 &rhs );
	
	void operator/=( float F );
	
	void operator=(Mat44 &rhs);

	float& operator()( int i, int j ){ return m[i][j]; }
	
#ifdef _USED3D

	void operator-=( D3DXMATRIXA16 &rhs );

	void operator*=( D3DXMATRIXA16 &rhs );

	void operator=(D3DXMATRIXA16 &rhs);

	void operator+=( D3DXMATRIXA16 &rhs );

#endif


	//-----------------------------------------------------------------------------
	// Methods
	//-----------------------------------------------------------------------------

	void MultiplyVector( Vec3 &out, const Vec3 &v );

	void Set( Vec3 R, Vec3 U, Vec3 L, Vec3 P );
	
#ifdef _USED3D

	void MultiplyVector( Vec3 &out, const D3DXVECTOR3 &v );

#endif
#ifndef _USED3D

	//-----------------------------------------------------------------------------
	// Data Members
	//-----------------------------------------------------------------------------

	union
	{
		struct
		{
			float   _11,  _12,  _13,  _14,
					_21,  _22,  _23,  _24,
					_31,  _32,  _33,  _34,
					_41,  _42,  _43,  _44;
		};
		float m[4][4]; 
		float mm[16];
	};

#endif

};


//-----------------------------------------------------------------------------
// Mat44 support functions - these form the bulk of the class 
// functionality and they provide compatibility with the D3DXMATRIXA16
//-----------------------------------------------------------------------------

void Identity( Mat44 &mat );
void VectorMatrixMultiply( Vec3& vDest, Vec3& vSrc, Mat44& mat);
void InvertFast( Mat44 &out, const Mat44 &m1 );
void InvertFull( Mat44 &out, Mat44 &m1 );
void Adjoint( Mat44 &out, Mat44 &m1 );
float Determinant(Mat44 &m);
inline float Minor( Mat44& m, const int r0, const int r1, const int r2, 
	                             const int c0, const int c1, const int c2 );
void Transpose( Mat44 &out, const Mat44 &m );
void TransposeAdd( Mat44 &out, const Mat44 &m1, const Mat44 &m2 );
void MatCpy( Mat44 &out, Mat44 &in );
void MatMult( Mat44 &out, const Mat44 &m1, const Mat44 &m2 );
Mat44 TranslateMatrix44(float x, float y, float z);
void RotationMatrixX( Mat44 &out, float angle );
void RotationMatrixY( Mat44 &out, float angle );
void RotationMatrixZ( Mat44 &out, float angle );
void RotationMatrixAxis( Mat44 &out, Vec3& axis, float angle );
Mat44 LookAtMatrix44( Vec3 &camPos, Vec3 &camUp, Vec3 &target );
Mat44 PerspectiveMatrix44( float fovY, float aspect, float n, float f );
Mat44 FrustumMatrix44( float l, float r, float b, float t, float n, float f ); 
Mat44 OrthoMatrix44( float l, float r, float b, float t, float n, float f ); 
Mat44 OrthoNormalMatrix44( const Vec3 &xdir, const Vec3 &ydir, const Vec3 &zdir );
void ScaleMatrix44( Mat44& out, float sx, float sy, float sz );

#ifdef _USED3D

void MatMult( Mat44 &out, const Mat44 &m1, const D3DXMATRIXA16 &m2 );
void MatMult( Mat44 &out, const D3DXMATRIXA16 &m1, const Mat44 &m2 );
void MatMult( Mat44 &out, const D3DXMATRIXA16 &m1, const D3DXMATRIXA16 &m2 );

#endif


#ifdef _USED3D
#define Vec3Dot( a, b ) 				D3DXVec3Dot( a, b )
#define Vec3Normalize( a, b ) 				D3DXVec3Normalize( a, b )
#define Vec3Cross( a, b, c ) 				D3DXVec3Cross( a, b, c )
#define MatrixTranslation( a, b, c, d )		D3DXMatrixTranslation( a, b, c, d )
#define IdentityMatrix( a )					D3DXMatrixIdentity( a )
#define MatrixRotationX( a, b )				D3DXMatrixRotationX( a, b )
#define MatrixRotationY( a, b )				D3DXMatrixRotationY( a, b )
#define MatrixRotationZ( a, b )				D3DXMatrixRotationZ( a, b )
#define MatrixRotationAxis( a, b, c )		D3DXMatrixRotationAxis( a, b, c )
#define MatrixMultiply( a, b, c )			D3DXMatrixMultiply( a, b, c )

#endif

//-----------------------------------------------------------------------------
// Mat44 global operators - these provide compatibility 
// with the D3DXVECTOR3
//-----------------------------------------------------------------------------

inline Mat44
operator* (const Mat44& a, const Mat44& b)
{
    Mat44 ret;
	MatMult(ret,a,b);
    return ret;
}

#ifdef _USED3D

inline Mat44
operator* (const Mat44& a, const D3DXMATRIXA16& b)
{
    Mat44 ret;
	MatMult(ret,a,b);
    return ret;
}

inline Mat44
operator* (const D3DXMATRIXA16& a, const Mat44& b)
{
    Mat44 ret;
	MatMult(ret,a,b);
    return ret;
}

#endif


#endif // __MATRIX44

#ifndef __QUATERNION
#define __QUATERNION

//-----------------------------------------------------------------------------
// CQuaternion - TODO:- develop this class
//-----------------------------------------------------------------------------
class Quaternion
{
public:


	//-----------------------------------------------------------------------------
	// Constructors/Destructors
	//-----------------------------------------------------------------------------
	
	Quaternion();
	~Quaternion();

#ifndef _USED3D

	float x, y, z, w;

#endif

};




//-----------------------------------------------------------------------------
// Name: QuaternionFromRotation()
// Desc: Converts a normalized axis and angle to a unit quaternion.
//-----------------------------------------------------------------------------
VOID QuaternionFromRotation( float& x, float& y, float& z, float& w,
                                     Vec3& v, float fTheta );

VOID RotationFromQuaternion( Vec3& v, float& fTheta,
                                     float x, float y, float z, float w );

//-----------------------------------------------------------------------------
// Name: QuaternionFromAngles()
// Desc: Converts euler angles to a unit quaternion.
//-----------------------------------------------------------------------------
VOID QuaternionFromAngles( float& x, float& y, float& z, float& w,
                                   float fYaw, float fPitch, float fRoll );

//-----------------------------------------------------------------------------
// Name: MatrixFromQuaternion()
// Desc: Converts a unit quaternion into a rotation matrix.
//-----------------------------------------------------------------------------
VOID MatrixFromQuaternion( Mat44& mat, float x, float y, float z,
                                   float w );

//-----------------------------------------------------------------------------
// Name: QuaternionFromMatrix()
// Desc: Converts a rotation matrix into a unit quaternion.
//-----------------------------------------------------------------------------
VOID QuaternionFromMatrix( float& x, float& y, float& z, float& w,
                                   Mat44& mat );

VOID QuaternionMultiply( float& Qx, float& Qy, float& Qz, float& Qw,
                         float Ax, float Ay, float Az, float Aw,
                         float Bx, float By, float Bz, float Bw );
VOID QuaternionSlerp
( 
	float& Qx, float& Qy, float& Qz, float& Qw,
    float  Ax, float  Ay, float  Az, float  Aw,
    float  Bx, float  By, float  Bz, float  Bw,
    float fAlpha 
);




#endif // __QUATERNION

#endif // _3DMATH_H

