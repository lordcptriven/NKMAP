//-----------------------------------------------------------------------------
// File : NKCamera.cpp
// Desc :
//-----------------------------------------------------------------------------


#include "stdafx.h"
#include "NKMath.h"
#include "NKCamera.h"
#include "MyNKApp.h"

extern CMyNKApp*		g_pMyNKApp;

//-----------------------------------------------------------------------------
// Name : CNKCamera()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKCamera::CNKCamera()
{
	m_fPitch = m_fYaw = 0.0f;
	m_fMinPitch = -90;
	m_fMaxPitch = 90;
	m_vLocation = NKVEC_ZERO;
	m_vDirection = NKVEC_ZERO;
	m_vMove = NKVEC_ZERO;
	m_vTarget = NKVEC_ZERO;
	m_fDistance = 250.0f;
	m_fMinDistance = 35.0f;
//	m_fMaxDistance = 5000.0f;
	m_fMaxDistance = 500000.0f; // ÁÜ ±æÀÌ ´Ã¸².(±×·¡ÇÈÆÀ ¿äÃ»)
	m_matView = NKMAT_IDENTITY;
	m_matProj = NKMAT_IDENTITY;
	m_matFrustumProj = NKMAT_IDENTITY;
	m_matViewProj = NKMAT_IDENTITY;
	m_bRecalViewMatrix = true;
//	UpdateProjMatrix( NK_QUATERPI, 800, 600, 30.0f, 10000.0f, 30.0f, 2500.0f );
	UpdateProjMatrix( NK_QUATERPI, 800, 600, 30.0f, 10000.0f, 30.0f, 2500.0f );

	// [2007/11/1 theodoric] Ä«¸Þ¶ó Å¾ºä
	m_bTopView = FALSE;
}


//-----------------------------------------------------------------------------
// Name : SetPitch()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::SetPitch( float fPitch )
{
	if( fPitch >= m_fMinPitch && fPitch <= m_fMaxPitch )
	{
		m_fPitch = fPitch;
		m_bRecalViewMatrix = true;
	}
}


//-----------------------------------------------------------------------------
// Name : SetYaw()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::SetYaw( float fYaw )
{
	if( fYaw >= -360.0f && fYaw <= 360.0f )
	{
		m_fYaw = fYaw;
		m_bRecalViewMatrix = true;
	}
}


//-----------------------------------------------------------------------------
// Name : SetPitchLimits()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::SetPitchLimits( float fMin, float fMax )
{
	if( fMin < fMax && fMin >= -360.0f &&	fMin <= 360.0f &&
		fMax >= -360.0f && fMax <= 360.0f )
	{
		m_fMinPitch = fMin;
		m_fMaxPitch = fMax;
	}
}


//-----------------------------------------------------------------------------
// Name : AddPitch()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::AddPitch( float fDelta )
{
	m_fPitch += fDelta;
	if( m_fPitch < m_fMinPitch )
	{
		m_fPitch = m_fMinPitch;
	}
	else if (m_fPitch > m_fMaxPitch )
	{
		m_fPitch = m_fMaxPitch;
	}

	m_bRecalViewMatrix = true;
}


//-----------------------------------------------------------------------------
// Name : AddYaw()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::AddYaw( float fDelta )
{
	m_fYaw += fDelta;
	m_fYaw = (float)fmod( m_fYaw, 360.0f );
	m_bRecalViewMatrix = true;
}


//-----------------------------------------------------------------------------
// Name : SetDistance()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::SetDistance( float fDistance )
{
	if( m_fDistance >= m_fMinDistance && m_fDistance <= m_fMaxDistance )
	{
		m_fDistance = fDistance;
		m_bRecalViewMatrix = true;
	}
}


//-----------------------------------------------------------------------------
// Name : SetMinMaxDistance()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::SetMinMaxDistance( float fMin, float fMax )
{
	if( fMin < fMax && fMin > 0.0f )
	{
		m_fMinDistance = fMin;
		m_fMaxDistance = fMax;

		if( m_fDistance < m_fMinDistance )
		{
			m_fDistance = m_fMinDistance;
			m_bRecalViewMatrix = true;
		}
		else if( m_fDistance > m_fMaxDistance )
		{
			m_fDistance = m_fMaxDistance;
			m_bRecalViewMatrix = true;
		}
	}
}


//-----------------------------------------------------------------------------
// Name : AddDistance()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::AddDistance( float fDelta )
{
	m_fDistance += fDelta;

	if( m_fDistance < m_fMinDistance )
	{
		m_fDistance = m_fMinDistance;
	}
	else if( m_fDistance > m_fMaxDistance )
	{
		m_fDistance = m_fMaxDistance;
	}

	m_bRecalViewMatrix = true;
}


//-----------------------------------------------------------------------------
// Name : SetProjMatrix()
// Desc : Aspect = Width / Height
//        Need a width & a height because of calculating picking ray
//-----------------------------------------------------------------------------
void CNKCamera::UpdateProjMatrix( float fFov, DWORD dwWidth, DWORD dwHeight, float fNear, float fFar,
									float fFrustumNear, float fFrustumFar )
{
	m_fFov = fFov;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_fAspect = (float)dwHeight / (float)dwWidth;
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFrustumNear = fFrustumNear;
	m_fFrustumFar = fFrustumFar;

	D3DXMatrixPerspectiveFovLH( &m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar );
	
	D3DXMatrixPerspectiveFovLH( &m_matFrustumProj, m_fFov, m_fAspect, m_fFrustumNear, m_fFrustumFar );
	UpdateFrustumPlanes();
}


//-----------------------------------------------------------------------------
// Name : UpdateFrustumPlanes()
// Desc : By Gribb and Hartmann -  fast extraction of viewing frustum planes
//            from the world-view-projection matrix
//                (http://www2.ravensoft.com/users/ggribb/plane extraction.pdf)
//-----------------------------------------------------------------------------
void CNKCamera::UpdateFrustumPlanes()
{
	// Multiply a view matrix and a projection matrix of a frustum
	D3DXMatrixMultiply( &m_matViewProj, &m_matView, &m_matFrustumProj );

	// Left plane
	m_aFrustumPlanes[VFPLANE_LEFT].a = m_matViewProj.m03 + m_matViewProj.m00;
	m_aFrustumPlanes[VFPLANE_LEFT].b = m_matViewProj.m13 + m_matViewProj.m10;
	m_aFrustumPlanes[VFPLANE_LEFT].c = m_matViewProj.m23 + m_matViewProj.m20;
	m_aFrustumPlanes[VFPLANE_LEFT].d = m_matViewProj.m33 + m_matViewProj.m30;

	// Right plane
	m_aFrustumPlanes[VFPLANE_RIGHT].a = m_matViewProj.m03 - m_matViewProj.m00;
	m_aFrustumPlanes[VFPLANE_RIGHT].b = m_matViewProj.m13 - m_matViewProj.m10;
	m_aFrustumPlanes[VFPLANE_RIGHT].c = m_matViewProj.m23 - m_matViewProj.m20;
	m_aFrustumPlanes[VFPLANE_RIGHT].d = m_matViewProj.m33 - m_matViewProj.m30;

	// Top plane
	m_aFrustumPlanes[VFPLANE_TOP].a = m_matViewProj.m03 - m_matViewProj.m01;
	m_aFrustumPlanes[VFPLANE_TOP].b = m_matViewProj.m13 - m_matViewProj.m11;
	m_aFrustumPlanes[VFPLANE_TOP].c = m_matViewProj.m23 - m_matViewProj.m21;
	m_aFrustumPlanes[VFPLANE_TOP].d = m_matViewProj.m33 - m_matViewProj.m31;

	// Bottom plane
	m_aFrustumPlanes[VFPLANE_BOTTOM].a = m_matViewProj.m03 + m_matViewProj.m01;
	m_aFrustumPlanes[VFPLANE_BOTTOM].b = m_matViewProj.m13 + m_matViewProj.m11;
	m_aFrustumPlanes[VFPLANE_BOTTOM].c = m_matViewProj.m23 + m_matViewProj.m21;
	m_aFrustumPlanes[VFPLANE_BOTTOM].d = m_matViewProj.m33 + m_matViewProj.m31;

	// Far plane
	m_aFrustumPlanes[VFPLANE_FAR].a = m_matViewProj.m03 - m_matViewProj.m02;
	m_aFrustumPlanes[VFPLANE_FAR].b = m_matViewProj.m13 - m_matViewProj.m12;
	m_aFrustumPlanes[VFPLANE_FAR].c = m_matViewProj.m23 - m_matViewProj.m22;
	m_aFrustumPlanes[VFPLANE_FAR].d = m_matViewProj.m33 - m_matViewProj.m32;

	// Near plane
	m_aFrustumPlanes[VFPLANE_NEAR].a = m_matViewProj.m02;
	m_aFrustumPlanes[VFPLANE_NEAR].b = m_matViewProj.m12;
	m_aFrustumPlanes[VFPLANE_NEAR].c = m_matViewProj.m22;
	m_aFrustumPlanes[VFPLANE_NEAR].d = m_matViewProj.m32;

	// Normalize planes
	D3DXPlaneNormalize( &m_aFrustumPlanes[0], &m_aFrustumPlanes[0] );
	D3DXPlaneNormalize( &m_aFrustumPlanes[1], &m_aFrustumPlanes[1] );
	D3DXPlaneNormalize( &m_aFrustumPlanes[2], &m_aFrustumPlanes[2] );
	D3DXPlaneNormalize( &m_aFrustumPlanes[3], &m_aFrustumPlanes[3] );
	D3DXPlaneNormalize( &m_aFrustumPlanes[4], &m_aFrustumPlanes[4] );
	D3DXPlaneNormalize( &m_aFrustumPlanes[5], &m_aFrustumPlanes[5] );
}


//-----------------------------------------------------------------------------
// Name : UpdateViewMatrix()
// Desc : Observing mode
//-----------------------------------------------------------------------------
void CNKCamera::UpdateViewMatrix()
{
	if( m_bRecalViewMatrix )
	{
		// [2007/11/1 theodoric] Ä«¸Þ¶ó Å¾ºä
		if( m_bTopView )
		{
			m_matView.m00 = 1.0f;		m_matView.m01 = 0.0f;			m_matView.m02 = 0.0f;
			m_matView.m10 = 0.0f;		m_matView.m11 = 0.0f;			m_matView.m12 = -1.0f;
			m_matView.m20 = 0.0f;		m_matView.m21 = 1.0f;			m_matView.m22 = 0.0f;
			m_matView.m30 = m_matView.m31 = - ((g_pMyNKApp->GetTerrain()->GetHMWidth() - 1)/2*50);
			switch(g_pMyNKApp->GetTerrain()->GetHMWidth() - 1)
			{
			case 32:	m_matView.m32 = 1970;	break;
			case 64:	m_matView.m32 = 3900;	break;
			case 128:	m_matView.m32 = 7850;	break;
			case 256:	m_matView.m32 = 15690;	break;
			}			
			
			// Update viewing frustum planes
			UpdateFrustumPlanes();

			m_bRecalViewMatrix = false;
		}
		else
		{
			NKMATRIX3	matTemp;
			D3DXVECTOR3	vTrans;

			// Get a rotation quaternion from yaw & pitch angles
			NKQuaternionFromYawPitch( m_qOrientation, m_fYaw * NK_DEGTORAD, m_fPitch * NK_DEGTORAD );
			
			// Convert a quaternion into a transposed rotation matrix
			NKQuaternionToTransposedMatrix( matTemp, m_qOrientation );

			// Get a camera location
			m_vDirection.x = matTemp.m02;	m_vDirection.y = matTemp.m12;	m_vDirection.z = matTemp.m22;
			D3DXVec3Add( &m_vLocation, &m_vTarget, &( m_vDirection*(-m_fDistance) ) );
			
			// Set a view matrix		
			NKVectorMatrixMultiply( vTrans, -m_vLocation, matTemp );		
			m_matView.m00 = matTemp.m00;	m_matView.m01 = matTemp.m01;	m_matView.m02 = matTemp.m02;
			m_matView.m10 = matTemp.m10;	m_matView.m11 = matTemp.m11;	m_matView.m12 = matTemp.m12;
			m_matView.m20 = matTemp.m20;	m_matView.m21 = matTemp.m21;	m_matView.m22 = matTemp.m22;
			m_matView.m30 = vTrans.x;		m_matView.m31 = vTrans.y;		m_matView.m32 = vTrans.z;
			
			// Update viewing frustum planes
			UpdateFrustumPlanes();

			m_bRecalViewMatrix = false;
		}
	}
}


//-----------------------------------------------------------------------------
// Name : UpdateFlyViewMatrix()
// Desc : Flying mode
//-----------------------------------------------------------------------------
void CNKCamera::UpdateFlyViewMatrix()
{
	if( m_bRecalViewMatrix )
	{
		NKMATRIX3	matTemp;
		D3DXVECTOR3	vTrans, vUp, vAt;

		// Get a rotation quaternion from yaw & pitch angles
		NKQuaternionFromYawPitch( m_qOrientation, m_fYaw * NK_DEGTORAD, m_fPitch * NK_DEGTORAD );
		
		// Convert a quaternion into a transposed rotation matrix
		NKQuaternionToMatrix( matTemp, m_qOrientation );
						
		// Get a camera location
		NKVectorMatrixMultiply( vTrans, m_vMove, matTemp );
		D3DXVec3Add( &m_vLocation, &m_vLocation, &vTrans );
		m_vMove.x = 0.0f;	m_vMove.y = 0.0f;	m_vMove.z = 0.0f;

		// Get a camera axes
		m_vDirection.x = matTemp.m20;	m_vDirection.y = matTemp.m21;	m_vDirection.z = matTemp.m22;
		vUp.x = matTemp.m10;			vUp.y = matTemp.m11;			vUp.z = matTemp.m12;
		vAt = m_vLocation + m_vDirection;

		// Set a view matrix
		D3DXMatrixLookAtLH( &m_matView, &m_vLocation, &vAt, &vUp );

		// Update viewing frustum planes
		UpdateFrustumPlanes();

		m_bRecalViewMatrix = false;
	}
}


//-----------------------------------------------------------------------------
// Name : GetPickRay()
// Desc :
//-----------------------------------------------------------------------------
void CNKCamera::GetPickingRay( D3DXVECTOR3& rOrg, D3DXVECTOR3& rDir, DWORD dwX, DWORD dwY ) const
{
	D3DXVECTOR3	vTemp;
	D3DXMATRIX	matTemp;

	vTemp.x = ( ( ( 2.0f * dwX ) / m_dwWidth ) - 1 ) / m_matProj.m00;
	vTemp.y = -( ( ( 2.0f * dwY ) / m_dwHeight ) - 1 ) / m_matProj.m11;
	vTemp.z = 1.0f;

	D3DXMatrixInverse( &matTemp, NULL, &m_matView );

	rDir.x = vTemp.x*matTemp.m00 + vTemp.y*matTemp.m10 + vTemp.z*matTemp.m20;
	rDir.y = vTemp.x*matTemp.m01 + vTemp.y*matTemp.m11 + vTemp.z*matTemp.m21;
	rDir.z = vTemp.x*matTemp.m02 + vTemp.y*matTemp.m12 + vTemp.z*matTemp.m22;
	rOrg.x = matTemp.m30;	rOrg.y = matTemp.m31;	rOrg.z = matTemp.m32;
}


//-----------------------------------------------------------------------------
// Name : CullPoints()
// Desc :
//-----------------------------------------------------------------------------
int CNKCamera::CullPoints( const D3DXVECTOR3* pPoints, int nNum )
{
	int nOut, nOverlap = 0;

	for(int i = 0; i < 6; i++ )
	{
		nOut = 0;
		for(int j = 0; j < nNum; j++ )
		{
			if( NKPlanePointDistance( m_aFrustumPlanes[i], pPoints[j] ) < NK_EPSILON4 )
				nOut++;
		}

		if( nOut == nNum )
			return VFCULL_OUTSIDE;

		nOverlap += nOut;
	}

	if( nOverlap > 0 )
		return VFCULL_OVERLAP;

	return VFCULL_INSIDE;
}


//-----------------------------------------------------------------------------
// Name : CullSphere()
// Desc :
//-----------------------------------------------------------------------------
int CNKCamera::CullSphere( const D3DXVECTOR3& rCenter, float fRadius )
{
	float fDistance;
	bool bOverlap = false;

	for( int i = 0; i < 6; i++ )
	{
		fDistance = NKPlanePointDistance( m_aFrustumPlanes[i], rCenter );
		
		if( fDistance < -fRadius )
			return VFCULL_OUTSIDE;
		
		if( fDistance < fRadius )
			bOverlap = true;
	}

	if( bOverlap )
		return VFCULL_OVERLAP;

	return VFCULL_INSIDE;
}


//-----------------------------------------------------------------------------
// Name : CullAABB()
// Desc :
//-----------------------------------------------------------------------------
int CNKCamera::CullAABB( const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax )
{
	D3DXVECTOR3	vMinExtream, vMaxExtream;
	bool bOverlap = false;

	for( int i = 0; i < 6; i++ )
	{
		if( m_aFrustumPlanes[i].a < NK_EPSILON4 ) {
			vMinExtream.x = rMin.x;	vMaxExtream.x = rMax.x;
		} else {
			vMinExtream.x = rMax.x;	vMaxExtream.x = rMin.x;
		}

		if( m_aFrustumPlanes[i].b < NK_EPSILON4 ) {
			vMinExtream.y = rMin.y;	vMaxExtream.y = rMax.y;
		} else {
			vMinExtream.y = rMax.y;	vMaxExtream.y = rMin.y;
		}

		if( m_aFrustumPlanes[i].c < NK_EPSILON4 ) {
			vMinExtream.z = rMin.z;	vMaxExtream.z = rMax.z;
		} else {
			vMinExtream.z = rMax.z;	vMaxExtream.z = rMin.z;
		}

		if( NKPlanePointDistance( m_aFrustumPlanes[i], vMinExtream ) < NK_EPSILON4 )
			return VFCULL_OUTSIDE;

		if( NKPlanePointDistance( m_aFrustumPlanes[i], vMaxExtream ) < NK_EPSILON4 )
			bOverlap = true;
	}

	if( bOverlap )
		return VFCULL_OVERLAP;

	return VFCULL_INSIDE;
}
