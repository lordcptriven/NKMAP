//-----------------------------------------------------------------------------
// File : NKCamera.h
// Desc :
//-----------------------------------------------------------------------------

#ifndef _NKCAMERA_H_
#define _NKCAMERA_H_


#include "NKRTLib.h"


//-----------------------------------------------------------------------------
// Viewing frustum culling flags
//-----------------------------------------------------------------------------
#define	VFCULL_OUTSIDE	0
#define	VFCULL_INSIDE	1
#define	VFCULL_OVERLAP	2


//-----------------------------------------------------------------------------
// Viewing frustum plane IDs
//-----------------------------------------------------------------------------
#define	VFPLANE_NEAR	0
#define	VFPLANE_FAR		1
#define VFPLANE_LEFT	2
#define	VFPLANE_RIGHT	3
#define	VFPLANE_TOP		4
#define	VFPLANE_BOTTOM	5


//-----------------------------------------------------------------------------
// Name : class CNKCamera
// Desc : 
//-----------------------------------------------------------------------------
class CNKCamera
{
protected:

	// [2007/11/1 theodoric] Ä«¸Þ¶ó Å¾ºä
	BOOL			m_bTopView;
	
	float			m_fFov, m_fAspect, m_fNear, m_fFar;					// Field of view(radian), aspect, distance of near & far plane
	float			m_fFrustumNear, m_fFrustumFar;						// Distance of near & far planes of a view frustum
	float			m_fPitch, m_fMinPitch, m_fMaxPitch;					// X-axis angle(degree) & limitation angle(degree)
	float			m_fYaw;												// Y-axis angle(degree) ( y-axis can rotate 360 degree )
	DWORD			m_dwWidth, m_dwHeight;								// a client width/height of a window
	D3DXQUATERNION	m_qOrientation;										// A camera orientation ( derived from pitch, yaw & roll )
	D3DXVECTOR3		m_vDirection;										// A camera direction
	D3DXVECTOR3		m_vLocation;										// A camera location
	bool			m_bRecalViewMatrix;									// Whether a view matrix is recalculated
	D3DXPLANE		m_aFrustumPlanes[6];								// Viewing frustum planes
	D3DXMATRIX		m_matView;											// View matrix
	D3DXMATRIX		m_matProj;											// Projection matrix
	D3DXMATRIX		m_matFrustumProj;									// Projection matrix for creating frustum planes
	D3DXMATRIX		m_matViewProj;										// View matrix * projection matrix
	
	// Variables for observing mode ( used in a game )
	D3DXVECTOR3		m_vTarget;											// A target position for tracking
	float			m_fDistance;										// A distance from a target to a camera
	float			m_fMinDistance, m_fMaxDistance;						// Min & max distances of a camera

	// A variable for flying mode ( used in a tool )
	D3DXVECTOR3		m_vMove;											// A moving offset of a camera

protected:
	void	UpdateFrustumPlanes();										// Update viewing frustum planes

public:
	CNKCamera();
	virtual ~CNKCamera();

	void	SetPitch( float fPitch );									// Set a pitch angle
	void	SetYaw( float fYaw );										// Set a yaw angle
	void	SetPitchLimits( float fMin, float fMax );					// Set a limitation angle of a pitch
	float	GetPitch() const;											// Get a pitch angle
	float	GetYaw() const;												// Get a yaw angle
	void	AddPitch( float fDelta );									// Add a pitch angle
	void	AddYaw( float fDelta );										// Add a yaw angle

	void	SetLocation( const D3DXVECTOR3& rLocation );				// Set a camera location
	void	SetLocation( float fx, float fy, float fz );
	const D3DXVECTOR3&	GetLocation() const;							// Get a camera location
	const D3DXVECTOR3&	GetDirection() const;							// Get a camera direction

	float	GetNear() const;											// Get a near distance of a projection matrix
	float	GetFar() const;												// Get a far distance of a projection matrix
	float	GetFrustumNear() const;										// Get a near distance of a view frustum
	float	GetFrustumFar() const;										// Get a far distance of a view frustum
	const D3DXMATRIX&	GetViewMatrix() const;							// Get a view matrix
	const D3DXMATRIX&	GetProjMatrix() const;							// Get a projection matrix
	void	UpdateProjMatrix( float fFov, DWORD dwWidth,				// Update a projection matrix	
								DWORD dwHeight, float fNear, float fFar,
								float fFrustumNear, float fFrustumFar );
	
	void	GetPickingRay( D3DXVECTOR3& rOrg, D3DXVECTOR3& rDir,		// Get a picking ray from a screen coordinate
							DWORD dwX, DWORD dwY ) const;
	
	int		CullPoints( const D3DXVECTOR3* pPoints, int nNum );			// Culling test between points & viewing frustum
	int		CullSphere( const D3DXVECTOR3& rCenter, float fRadius );	// Culling test between sphere & viewing frustum
	int		CullAABB( const D3DXVECTOR3& rMin,							// Culling test between AABB & viewing frustum
						const D3DXVECTOR3& rMax );
	int		CullAABB( float fMinX, float fMinY, float fMinZ,
						float fMaxX, float fMaxY, float fMaxZ );
	
	// Functions for observing mode
	void	SetTarget( const D3DXVECTOR3& rTarget );					// Set a target position
	void	SetTarget( float fx, float fy, float fz );
	void	SetDistance( float fDistance );								// Set a distance from a target to a camera
	void	SetMinMaxDistance( float fMin, float fMax );				// Set min & max distances of a camera
	const D3DXVECTOR3&	GetTarget() const;								// Get a target position
	float	GetDistance() const;										// Get a distance
	void	AddDistance( float fDelta );								// Add a distance
	void	UpdateViewMatrix();											// Update a view matrix from variables changed

	// A function for flying mode
	void	AddMove( const D3DXVECTOR3& rMove );						// Add a moving offset to a camera location
	void	AddMove( float fx, float fy, float fz );					// fx:sideward, fy:up-down, fz:front-back
	void	UpdateFlyViewMatrix();										// Update a view matrix ( flying mode )

	// [2007/11/1 theodoric] Ä«¸Þ¶ó Å¾ºä
	BOOL	IsTopView(){ return m_bTopView; }
	void	ToggleTopView(){ m_bTopView = !m_bTopView;}
	void	SetTopView(BOOL topView){m_bTopView = topView;}
	void	SetRecalViewMatrix(bool RecalViewMatrix){m_bRecalViewMatrix = RecalViewMatrix;}
};


// Inline functions
#include "NKCamera.inl"


#endif // _NKCAMERA_H_