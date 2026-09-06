//-----------------------------------------------------------------------------
// File : NKMath.cpp
// Desc :
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "NKMath.h"


//-----------------------------------------------------------------------------
// Name : NKVectorMatrixMultiply()
// Desc : Multiply a vector(1x3)  and a matrix(3x3)
//-----------------------------------------------------------------------------
void NKVectorMatrixMultiply( D3DXVECTOR3& rOut, const D3DXVECTOR3& rVec, const NKMATRIX3& rMat )
{
	float x = rVec.x;
	float y = rVec.y;
	float z = rVec.z;

	rOut.x = x*rMat.m00 + y*rMat.m10 + z*rMat.m20;
	rOut.y = x*rMat.m01 + y*rMat.m11 + z*rMat.m21;
	rOut.z = x*rMat.m02 + y*rMat.m12 + z*rMat.m22;
}


//-----------------------------------------------------------------------------
// Name : NKQuaternionRotationYawPitch()
// Desc : Caculate a rotation quaternion form yaw & pitch angle
//        a roll angle is 0 degree ( z-axis is fixed )
//-----------------------------------------------------------------------------
void NKQuaternionFromYawPitch( D3DXQUATERNION& rQ, float fYaw, float fPitch)
{
	float fSinY = sinf( 0.5f*fYaw );
	float fCosY = cosf( 0.5f*fYaw );
	float fSinP = sinf( 0.5f*fPitch );
	float fCosP = cosf( 0.5f*fPitch );
	
	rQ.x = fSinP*fCosY;
	rQ.y = fCosP*fSinY;
	rQ.z = -( fSinP*fSinY );
	rQ.w = fCosP*fCosY;
}


//-----------------------------------------------------------------------------
// Name : NKQuaternionToMatrix()
// Desc : Convert a unit quaternion into a rotation matrix
//-----------------------------------------------------------------------------
void NKQuaternionToMatrix( NKMATRIX3& rOut, float qx, float qy, float qz, float qw )
{
	float f2x = 2.0f * qx;				// 2x
	float f2y = 2.0f * qy;				// 2y
	float f2z = 2.0f * qz;				// 2z

	float fVal1 = f2y * qx;				// 2xy
	float fVal2 = f2z * qw;				// 2wz
	rOut.m01 = fVal1 + fVal2;			// 2xy + 2wz
	rOut.m10 = fVal1 - fVal2;			// 2xy - 2wz	

	fVal1 = f2z * qx;					// 2xz
	fVal2 = f2y * qw;					// 2wy
	rOut.m02 = fVal1 - fVal2;			// 2xz - 2wy
	rOut.m20 = fVal1 + fVal2;			// 2xz + 2wy	

	fVal1 = f2z * qy;					// 2yz
	fVal2 = f2x * qw;					// 2wx
	rOut.m12 = fVal1 + fVal2;			// 2yz + 2wx
	rOut.m21 = fVal1 - fVal2;			// 2yz - 2wx	

	fVal1 = f2x * qx;					// 2xx
	fVal2 = f2y * qy;					// 2yy
	f2z = f2z * qz;						// 2zz
	rOut.m00 = 1.0f - (fVal2 + f2z);	// 1 - (2yy + 2zz)
	rOut.m11 = 1.0f - (fVal1 + f2z);	// 1 - (2xx + 2zz)
	rOut.m22 = 1.0f - (fVal1 + fVal2);	// 1 - (2xx + 2yy)
}


//-----------------------------------------------------------------------------
// Name : NKQuaternionToMatrix()
// Desc : Convert a unit quaternion into a transposed rotation matrix
//-----------------------------------------------------------------------------
void NKQuaternionToTransposedMatrix( NKMATRIX3& rOut, float qx, float qy, float qz, float qw )
{
	float f2x = 2.0f * qx;				// 2x
	float f2y = 2.0f * qy;				// 2y
	float f2z = 2.0f * qz;				// 2z

	float fVal1 = f2y * qx;				// 2xy
	float fVal2 = f2z * qw;				// 2wz
	rOut.m01 = fVal1 - fVal2;			// 2xy - 2wz
	rOut.m10 = fVal1 + fVal2;			// 2xy + 2wz	

	fVal1 = f2z * qx;					// 2xz
	fVal2 = f2y * qw;					// 2wy
	rOut.m02 = fVal1 + fVal2;			// 2xz + 2wy
	rOut.m20 = fVal1 - fVal2;			// 2xz - 2wy	

	fVal1 = f2z * qy;					// 2yz
	fVal2 = f2x * qw;					// 2wx
	rOut.m12 = fVal1 - fVal2;			// 2yz - 2wx
	rOut.m21 = fVal1 + fVal2;			// 2yz + 2wx	

	fVal1 = f2x * qx;					// 2xx
	fVal2 = f2y * qy;					// 2yy
	f2z = f2z * qz;						// 2zz
	rOut.m00 = 1.0f - (fVal2 + f2z);	// 1 - (2yy + 2zz)
	rOut.m11 = 1.0f - (fVal1 + f2z);	// 1 - (2xx + 2zz)
	rOut.m22 = 1.0f - (fVal1 + fVal2);	// 1 - (2xx + 2yy)
}


//-----------------------------------------------------------------------------
// Name : NKTestPointsInRect()
// Desc : Test if points are in a rectangle
//-----------------------------------------------------------------------------
int NKTestPointsInRect( float fMinX, float fMinZ, float fMaxX, float fMaxZ,
						const D3DXVECTOR3* pPoints, int nNum )
{
	int nOut = 0;
	int	nIn = 0;

	for( int i = 0; i < nNum; i++ )
	{
		if( pPoints[i].x >= fMinX && pPoints[i].x <= fMaxX &&
			pPoints[i].z >= fMinZ && pPoints[i].z <= fMaxZ )
			nIn++;
		else
			nOut++;			
	}

	if( nIn == nNum )			// Inside
		return 2;
	else if( nOut == nNum )		// Outside
		return 0;
	
	return 1;					// Overlap
}


//-----------------------------------------------------------------------------
// Name : NKTestRectInRect()
// Desc : Test if a rectangle is in a rectangle
//-----------------------------------------------------------------------------
int NKTestRectInRect( float fMinX, float fMinZ, float fMaxX, float fMaxZ,
							float fTestMinX, float fTestMinZ, float fTestMaxX, float fTestMaxZ )
{
	if( fTestMinX > fMaxX || fTestMinZ > fMaxZ || fTestMaxX < fMinX || fTestMaxZ < fMinZ )
		return 0;

	if( fTestMinX > fMinX && fTestMinZ > fMinZ && fTestMaxX < fMaxX && fTestMaxZ < fMaxZ )
		return 2;	

	return 1;
}


//-----------------------------------------------------------------------------
// Name : NKTestPointsInAABB()
// Desc : Test if points are in AABB
//-----------------------------------------------------------------------------
int NKTestPointsInAABB( const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax,
						const D3DXVECTOR3* pPoints,	int nNum )
{
	int nOut = 0;
	int	nIn = 0;

	for(int i = 0; i < nNum; i++ )
	{
		if( pPoints[i].x >= rMin.x && pPoints[i].x <= rMax.x &&
			pPoints[i].y >= rMin.y && pPoints[i].y <= rMax.y &&
			pPoints[i].z >= rMin.z && pPoints[i].z <= rMax.z )
			nIn++;
		else
			nOut++;			
	}

	if( nIn == nNum )			// Inside
		return 2;
	else if( nOut == nNum )		// Outside
		return 0;
	
	return 1;					// Overlap
}


//-----------------------------------------------------------------------------
// Name : NKIntersectRayAABB()
// Desc : Test a intersection between a ray & a AABB
//        http://www.codercorner.com/RayAABB.cpp
//-----------------------------------------------------------------------------
bool NKIntersectRayAABB( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir,
							const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax, float* fT )
{
	bool		bInside = true;
	float		fMaxT[3];
	D3DXVECTOR3	vCoord;
		
	fMaxT[0] = fMaxT[1] = fMaxT[2] = -1.0f;

	// Find a candidate plane
	if( rOrigin.x < rMin.x )
	{
		bInside = false;	vCoord.x = rMin.x;
		if( rDir.x != 0 ) fMaxT[0] = ( rMin.x - rOrigin.x ) / rDir.x;
	} else if( rOrigin.x > rMax.x )
	{
		bInside = false;	vCoord.x = rMax.x;
		if( rDir.x != 0 ) fMaxT[0] = ( rMax.x - rOrigin.x ) / rDir.x;
	}

	if( rOrigin.y < rMin.y )
	{
		bInside = false;	vCoord.y = rMin.y;
		if( rDir.y != 0 ) fMaxT[1] = ( rMin.y - rOrigin.y ) / rDir.y;
	} else if( rOrigin.y > rMax.y )
	{
		bInside = false;	vCoord.y = rMax.y;
		if( rDir.y != 0 ) fMaxT[1] = ( rMax.y - rOrigin.y ) / rDir.y;
	}

	if( rOrigin.z < rMin.z )
	{
		bInside = false;	vCoord.z = rMin.z;
		if( rDir.z != 0 ) fMaxT[2] = ( rMin.z - rOrigin.z ) / rDir.z;
	} else if( rOrigin.z > rMax.z )
	{
		bInside = false;	vCoord.z = rMax.z;
		if( rDir.z != 0 ) fMaxT[2] = ( rMax.z - rOrigin.z ) / rDir.z;
	}

	// If origin is inside AABB
	if( bInside )
		return true;

	// Get a largest value among MaxT
	int	nWhichPlane = 0;
	if( fMaxT[1] > fMaxT[nWhichPlane] ) nWhichPlane = 1;
	if( fMaxT[2] > fMaxT[nWhichPlane] ) nWhichPlane = 2;

	if( fMaxT[nWhichPlane] < 0 )
		return false;

	if( nWhichPlane != 0 )
	{
		vCoord.x = rOrigin.x + fMaxT[nWhichPlane] * rDir.x;
		if( vCoord.x < rMin.x - NK_EPSILON5 || vCoord.x > rMax.x + NK_EPSILON5 )
			return false;
	}

	if( nWhichPlane != 1 )
	{
		vCoord.y = rOrigin.y + fMaxT[nWhichPlane] * rDir.y;
		if( vCoord.y < rMin.y - NK_EPSILON5 || vCoord.y > rMax.y + NK_EPSILON5 )
			return false;
	}

	if( nWhichPlane != 2 )
	{
		vCoord.z = rOrigin.z + fMaxT[nWhichPlane] * rDir.z;
		if( vCoord.z < rMin.z - NK_EPSILON5 || vCoord.z > rMax.z + NK_EPSILON5 )
			return false;
	}

	if( fT )
		*fT = fMaxT[nWhichPlane];

	return true;
}


//-----------------------------------------------------------------------------
// Name : NKIntersectRayOBB()
// Desc : Test a intersection & find a intersection point
//        between a ray & a OBB
//-----------------------------------------------------------------------------
bool NKIntersectRayOBB( const D3DXVECTOR3& rOrig, const D3DXVECTOR3& rDir,
						const D3DXVECTOR3* pPoints, float* fT )
{
	D3DXVECTOR3	vP0, vP1, vP2;

	// Front
	vP0 = pPoints[4];
	vP1 = pPoints[7];
	vP2 = pPoints[0];
	if( NKIntersectRayTri( rOrig, rDir, vP0, vP1, vP2, fT ) )
		return true;

	vP0 = pPoints[3];
	if( NKIntersectRayTri( rOrig, rDir, vP2, vP1, vP0, fT ) )
		return true;

	// Left, back, right
	for( int i = 0; i < 3; i++ )
	{
		vP0 = pPoints[i + 5];
		vP1 = pPoints[i + 4];
		vP2 = pPoints[i + 1];
		if( NKIntersectRayTri( rOrig, rDir, vP0, vP1, vP2, fT ) )
			return true;

		vP0 = pPoints[i];
		if( NKIntersectRayTri( rOrig, rDir, vP2, vP1, vP0, fT ) )
			return true;
	}

	// Up
	vP0 = pPoints[5];
	vP1 = pPoints[6];
	vP2 = pPoints[4];
	if( NKIntersectRayTri( rOrig, rDir, vP0, vP1, vP2, fT ) )
		return true;

	vP0 = pPoints[7];
	if( NKIntersectRayTri( rOrig, rDir, vP2, vP1, vP0, fT ) )
		return true;

	// Down
	vP0 = pPoints[0];
	vP1 = pPoints[3];
	vP2 = pPoints[1];
	if( NKIntersectRayTri( rOrig, rDir, vP0, vP1, vP2, fT ) )
		return true;

	vP0 = pPoints[2];
	if( NKIntersectRayTri( rOrig, rDir, vP2, vP1, vP0, fT ) )
		return true;
	

	return false;
}


//-----------------------------------------------------------------------------
// Name : NKIntersectTriangle()
// Desc : Test a intersection & find a intersection point
//        between a ray & a triangle 
//-----------------------------------------------------------------------------
bool NKIntersectRayTri( const D3DXVECTOR3& rOrig, const D3DXVECTOR3& rDir, D3DXVECTOR3& rP0,
						D3DXVECTOR3& rP1, D3DXVECTOR3& rP2, float* fT )
{
	float	fU, fV;
	// Find vectors for two edges sharing vert0
	D3DXVECTOR3	vEdge1, vEdge2;
	D3DXVec3Subtract( &vEdge1, &rP1, &rP0 );
	D3DXVec3Subtract( &vEdge2, &rP2, &rP0 );

	// Begin calculating determinant - also used to calculate U parameter
	D3DXVECTOR3	vP;
	D3DXVec3Cross( &vP, &rDir, &vEdge2 );

	// If determinant is near zero, ray lies in plane of a triangle
	float	fDet = D3DXVec3Dot( &vEdge1, &vP );

	D3DXVECTOR3	vT;
	if( fDet > 0 )
	{
		D3DXVec3Subtract( &vT, &rOrig, &rP0 );
	}
	else
	{
		D3DXVec3Subtract( &vT, &rP0, &rOrig );
		fDet = -fDet;
	}

	if( fDet < NK_EPSILON4 )
		return false;

	// Calculate U parameter and test bounds
	fU = D3DXVec3Dot( &vT, &vP );
	if( fU < 0.0f || fU > fDet )
		return false;

	// Prepare to test V parameter
	D3DXVECTOR3	vQ;
	D3DXVec3Cross( &vQ, &vT, &vEdge1 );

	// Calculate V parameter and test bounds
	fV = D3DXVec3Dot( &rDir, &vQ );
	if( fV < 0.0f || fU + fV > fDet )
		return false;

	// Calculate t, a ray intersects a triangle
	*fT = D3DXVec3Dot( &vEdge2, &vQ ) / fDet;

	return true;
}


//-----------------------------------------------------------------------------
// Name : NKIntersectRaySphere()
// Desc :
//-----------------------------------------------------------------------------
bool NKIntersectRaySphere( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir,
							const D3DXVECTOR3& rCenter, float fRadius, float* fT )
{
	D3DXVECTOR3	vDiff;
	float		fA, fB, fC;
	D3DXVec3Subtract( &vDiff, &rOrigin, &rCenter );
	fA = D3DXVec3LengthSq( &rDir );
	fB = D3DXVec3Dot( &vDiff, &rDir );
	fC = D3DXVec3LengthSq( &vDiff ) - fRadius * fRadius;

	float	fNearT;
	float	fDiscr = fB * fB - fA * fC;
	if( fDiscr < 0.0f )
		return false;
	else if( fDiscr > 0.0f )
	{
		fNearT = ( -fB - sqrtf( fDiscr ) ) / fA;
		if( fNearT < 0.0f )
			return false;
	}
	else
	{
		fNearT = -fB / fA;
		if( fNearT < 0.0f )
			return false;
	}

	if( fT )
		*fT = fNearT;
	return true;
}


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------