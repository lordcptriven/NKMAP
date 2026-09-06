//-----------------------------------------------------------------------------
// File : NKMath.inl
// Desc :
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name : NKMinf()
// Desc :
//-----------------------------------------------------------------------------
inline float NKMinf( float fA, float fB )
{
	return fA > fB ? fB : fA;
}


//-----------------------------------------------------------------------------
// Name : NKMaxf()
// Desc :
//-----------------------------------------------------------------------------
inline float NKMaxf( float fA, float fB )
{
	return fA > fB ? fA : fB;
}


//-----------------------------------------------------------------------------
// Name : NKPlanePointDistance()
// Desc :
//-----------------------------------------------------------------------------
inline float NKPlanePointDistance( const D3DXPLANE& rPlane, const D3DXVECTOR3& rPoint )
{
	return rPlane.a*rPoint.x + rPlane.b*rPoint.y + rPlane.c*rPoint.z + rPlane.d;
}


//-----------------------------------------------------------------------------
// Name : NKQuaternionToMatrix()
// Desc : Convert a unit quaternion into a rotation matrix
//-----------------------------------------------------------------------------
inline void NKQuaternionToMatrix( NKMATRIX3& rOut, const D3DXQUATERNION& rQ )
{
	NKQuaternionToMatrix( rOut, rQ.x, rQ.y, rQ.z, rQ.w );
}


//-----------------------------------------------------------------------------
// Name : NKQuaternionToTransposedMatrix()
// Desc : Convert a unit quaternion into a transposed rotation matrix
//-----------------------------------------------------------------------------
inline void NKQuaternionToTransposedMatrix( NKMATRIX3& rOut, const D3DXQUATERNION& rQ )
{
	NKQuaternionToTransposedMatrix( rOut, rQ.x, rQ.y, rQ.z, rQ.w );
}


//-----------------------------------------------------------------------------
// Name : NKMatrixTranspose()
// Desc : Transpose a matrix(3x3)
//-----------------------------------------------------------------------------
inline void NKMatrixTranspose( NKMATRIX3& rMat )
{
	float fTemp;
	fTemp = rMat.m01;	rMat.m01 = rMat.m10;	rMat.m10 = fTemp;
	fTemp = rMat.m02;	rMat.m02 = rMat.m20;	rMat.m20 = fTemp;
	fTemp = rMat.m12;	rMat.m12 = rMat.m21;	rMat.m12 = fTemp;
}


//-----------------------------------------------------------------------------
// Name : NKIntersectRayAABB()
// Desc : Test a intersection between a ray & a AABB
//-----------------------------------------------------------------------------
inline bool NKIntersectRayAABB( const D3DXVECTOR3& rOrigin,	const D3DXVECTOR3& rDir, float fMinX, float fMinY,
								float fMinZ, float fMaxX, float fMaxY, float fMaxZ, float* fT )
{
	return NKIntersectRayAABB( rOrigin, rDir, D3DXVECTOR3( fMinX, fMinY, fMinZ ),
								D3DXVECTOR3( fMaxX, fMaxY, fMaxZ ), fT );
}


//-----------------------------------------------------------------------------
// Name : NKMatrixFromYawPitch()
// Desc :
//-----------------------------------------------------------------------------
inline void NKMatrixFromYawPitch( D3DXMATRIX& rMat, float fYaw, float fPitch )
{
	D3DXQUATERNION	rQ;
	NKQuaternionFromYawPitch( rQ, fYaw, fPitch );
	D3DXMatrixRotationQuaternion( &rMat, &rQ );
}


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------

