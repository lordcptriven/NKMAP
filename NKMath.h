//-----------------------------------------------------------------------------
// File : NKMath.h
// Desc :
//-----------------------------------------------------------------------------

#ifndef _NKMATH_H_
#define _NKMATH_H_


#include "NKRTLib.h"


//-----------------------------------------------------------------------------
// Useful constants
//-----------------------------------------------------------------------------
// PI
const float	NK_PI = 4.0f * atanf( 1.0f );
const float	NK_TWOPI = 2.0f * NK_PI;
const float	NK_HALFPI = 0.5f * NK_PI;
const float	NK_QUATERPI = atanf( 1.0f );
const float	NK_THREEQUATERPI = NK_QUATERPI * 3.0f;
const float	NK_INVPI = 1.0f / NK_PI;
const float	NK_INVTWOPI = 1.0f / NK_TWOPI;
const float	NK_DEGTORAD = NK_PI / 180.0f;
const float	NK_RADTODEG = 180.0f / NK_PI;

// Epsilon
const float NK_EPSILON1 = 1e-1f;
const float NK_EPSILON2 = 1e-2f;
const float NK_EPSILON3 = 1e-3f;
const float NK_EPSILON4 = 1e-4f;
const float NK_EPSILON5 = 1e-5f;
const float NK_EPSILON6 = 1e-6f;

// D3DXVECTOR3
const D3DXVECTOR3	NKVEC_ZERO = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3	NKVEC_UNITX = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
const D3DXVECTOR3	NKVEC_UNITY = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
const D3DXVECTOR3	NKVEC_UNITZ = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );

// D3DXMATRIX
const D3DXMATRIX	NKMAT_ZERO = D3DXMATRIX( 0.0f, 0.0f, 0.0f, 0.0f,
											  0.0f, 0.0f, 0.0f, 0.0f,
											  0.0f, 0.0f, 0.0f, 0.0f,
											  0.0f, 0.0f, 0.0f, 0.0f );
const D3DXMATRIX	NKMAT_IDENTITY = D3DXMATRIX( 1.0f, 0.0f, 0.0f, 0.0f,
												  0.0f, 1.0f, 0.0f, 0.0f,
												  0.0f, 0.0f, 1.0f, 0.0f,
												  0.0f, 0.0f, 0.0f, 1.0f );

// D3DXQUATERNION
const D3DXQUATERNION	NKQUAT_ZERO = D3DXQUATERNION( 0.0f, 0.0f, 0.0f, 0.0f );
const D3DXQUATERNION	NKQUAT_IDENTITY = D3DXQUATERNION( 0.0f, 0.0f, 0.0f, 1.0f );


//-----------------------------------------------------------------------------
// Structs NKMATRIX3 (3x3)
//-----------------------------------------------------------------------------
typedef struct _NKMATRIX3
{
public:
	union {
		struct {
			float	m00, m01, m02;
			float	m10, m11, m12;
			float	m20, m21, m22;
        };
        float m[3][3];
    };
} NKMATRIX3;


//-----------------------------------------------------------------------------
// miscellaneous functions
//-----------------------------------------------------------------------------
float	NKMinf( float fA, float fB );											// Get a minimum value
float	NKMaxf( float fA, float fB );											// Get a maximum value


//-----------------------------------------------------------------------------
// Plane functions
//-----------------------------------------------------------------------------
float	NKPlanePointDistance( const D3DXPLANE& rPlane,							// Get a signed distance from a plane to a point
							const D3DXVECTOR3& rPoint );


//-----------------------------------------------------------------------------
// Vector functions
//-----------------------------------------------------------------------------
void	NKVectorMatrixMultiply( D3DXVECTOR3& rOut, const D3DXVECTOR3& rVec,		// Multiply a vector & a matrix
							const NKMATRIX3& rMat );					


//-----------------------------------------------------------------------------
// Matrix functions
//-----------------------------------------------------------------------------				
void	NKMatrixTranspose( NKMATRIX3& rMat );									// Transpose a matrix
void	NKMatrixFromYawPitch( D3DXMATRIX& rMat, float fYaw, float fPitch );		// Calculate a rotation matrix from yaw & pitch
						


//-----------------------------------------------------------------------------
// Quaternion functions
//-----------------------------------------------------------------------------
void	NKQuaternionFromYawPitch( D3DXQUATERNION& rQ,							// Caculate a rotation quaternion from yaw & pitch angle
							float fYaw, float fPitch);
void	NKQuaternionToMatrix( NKMATRIX3& rOut,									// Convert a unit quaternion into a rotation matrix
							float qx, float qy, float qz, float qw );
void	NKQuaternionToMatrix( NKMATRIX3& rOut, const D3DXQUATERNION& rQ );		// Convert a unit quaternion into a rotation matrix
void	NKQuaternionToTransposedMatrix( NKMATRIX3& rOut,						// Convert a unit quaternion into a transposed rotation matrix
							float qx, float qy, float qz, float qw );
void	NKQuaternionToTransposedMatrix( NKMATRIX3& rOut,
							const D3DXQUATERNION& rQ );


//-----------------------------------------------------------------------------
// Intersection functions
//-----------------------------------------------------------------------------
int		NKTestPointsInRect( float fMinX, float fMinZ, float fMaxX, float fMaxZ,	// Test if points are in a rectangle
						const D3DXVECTOR3* pPoints, int nNum );
int		NKTestRectInRect( float fMinX, float fMinZ, float fMaxX,				// Test if a rectangle is in a rectangle
						float fMaxZ, float fTestMinX, float fTestMinZ,
						float fTestMaxX, float fTestMaxZ );

bool	NKIntersectRaySphere( const D3DXVECTOR3& rOrigin,						// Test a intersection & find a intersection point
						const D3DXVECTOR3& rDir, const D3DXVECTOR3& rCenter,	// between a ray & a sphere
						float fRadius, float* fT = NULL );

int		NKTestPointsInAABB( const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax,	// Test if points are in AABB
						const D3DXVECTOR3* pPoints, int nNum );
bool	NKIntersectRayAABB( const D3DXVECTOR3& rOrigin,							// Test a intersection & find a intersection point
						const D3DXVECTOR3& rDir, const D3DXVECTOR3& rMin,		// between a ray & a AABB
						const D3DXVECTOR3& rMax, float* fT = NULL );
bool	NKIntersectRayAABB( const D3DXVECTOR3& rOrigin,
						const D3DXVECTOR3& rDir, float fMinX, float fMinY,
						float fMinZ, float fMaxX, float fMaxY, float fMaxZ,
						float* fT = NULL );

bool	NKIntersectRayOBB( const D3DXVECTOR3& rOrig,							// Test a intersection & find a intersection point
						const D3DXVECTOR3& rDir, const D3DXVECTOR3* pPoints, 	// between a ray & a OBB
						float* fT );

bool	NKIntersectRayTri( const D3DXVECTOR3& rOrig, const D3DXVECTOR3& rDir,	// Test a intersection & find a intersection point
						D3DXVECTOR3& rP0, D3DXVECTOR3& rP1, D3DXVECTOR3& rP2,	// between a ray & a triangle
						float* fT );


// Inline functions
#include "NKMath.inl"


#endif // _NKMATH_H_