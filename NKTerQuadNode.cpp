//-----------------------------------------------------------------------------
// File : NKTerQuadNode.cpp
// Desc :
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "MyNKApp.h"
#include "NKTerQuadNode.h"
#include "NKCamera.h"
#include "NKMath.h"
#include "config.h"


// Global variables
extern CMyNKApp*	g_pMyNKApp;


//-----------------------------------------------------------------------------
// Static member variables
//-----------------------------------------------------------------------------
int			CNKTerQuadNode::ms_nVBIdxCnt = 0;
CNKTerrain*	CNKTerQuadNode::ms_pTerrain = NULL;
float		CNKTerQuadNode::ms_fdt = 0.0f;


//-----------------------------------------------------------------------------
// Name : CNKTerQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKTerQuadNode::CNKTerQuadNode()
{
	m_pParent = NULL;
	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;
	m_nL1TexID = 0;
	m_nL2TexID = 0;
	m_nL3TexID = 0;
}


//-----------------------------------------------------------------------------
// Name : CNKTerQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKTerQuadNode::CNKTerQuadNode( CNKTerQuadNode* pParent, int nLevel, int nLeftX, int nLeftZ,
								int nRightX, int nRightZ )
{
	int		nCenterX, nCenterZ;
	float	fMinY, fMaxY, fTemp;
	int		nX, nZ;

	m_pParent = pParent;
	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;

	if( nLevel < 1 )			// Lowest level node
	{
		// Set a quad tree node pointer to access directly in map tool
		ms_pTerrain->SetQuadNodePointer( nLeftZ, nLeftX, this );

		// Calculate min & max heights
		fMinY = fMaxY = ms_pTerrain->GetHeights( nLeftZ, nLeftX );
		for( nZ = nLeftZ; nZ <= nRightZ; nZ++ )
		{
			for( nX = nLeftX; nX <= nRightX; nX++ )
			{
				fTemp = ms_pTerrain->GetHeights( nZ, nX );
				if( fMinY > fTemp ) fMinY = fTemp;
				if( fMaxY < fTemp ) fMaxY = fTemp;
			}
		}

		// Set a bounding box of lowest level node
		m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
		m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );

		// Set a texture ID
		if( ms_pTerrain->GetTextureCount() == 0 )
		{
			m_nL1TexID = TEXTURE_MAX_COUNT;
			m_nL2TexID = TEXTURE_MAX_COUNT;
			m_nL3TexID = TEXTURE_MAX_COUNT;
		}
		else
		{
			m_nL1TexID = 0;
			m_nL2TexID = TEXTURE_MAX_COUNT;
			m_nL3TexID = TEXTURE_MAX_COUNT;
		}

		return ;
	}
	
	// Prepare for child node
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft = new CNKTerQuadNode( this, nLevel, nLeftX, nLeftZ, nCenterX, nCenterZ );
	m_pLRight = new CNKTerQuadNode( this, nLevel, nCenterX, nLeftZ, nRightX, nCenterZ );
	m_pULeft = new CNKTerQuadNode( this, nLevel, nLeftX, nCenterZ, nCenterX, nRightZ );
	m_pURight = new CNKTerQuadNode( this, nLevel, nCenterX, nCenterZ, nRightX, nRightZ );

	// Get min & max heights of lower level node
	fMinY = m_pLLeft->m_vMin.y;
	fMaxY = m_pLLeft->m_vMax.y;

	// Calculate min & max heights of current level node
	if( fMinY > m_pLRight->m_vMin.y ) fMinY = m_pLRight->m_vMin.y;
	if( fMinY > m_pULeft->m_vMin.y ) fMinY = m_pULeft->m_vMin.y;
	if( fMinY > m_pURight->m_vMin.y ) fMinY = m_pURight->m_vMin.y;

	if( fMaxY < m_pLRight->m_vMax.y ) fMaxY = m_pLRight->m_vMax.y;
	if( fMaxY < m_pULeft->m_vMax.y ) fMaxY = m_pULeft->m_vMax.y;
	if( fMaxY < m_pURight->m_vMax.y ) fMaxY = m_pURight->m_vMax.y;

	// Set a bounding box of current level node
	m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
	m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );
}


//-----------------------------------------------------------------------------
// Name : CNKTerQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKTerQuadNode::CNKTerQuadNode( CNKTerQuadNode* pParent, int nLevel, int nLeftX, int nLeftZ,
								int nRightX, int nRightZ, unsigned char* pL1TexID,
								unsigned char* pL2TexID, unsigned char* pL3TexID )
{
	int		nCenterX, nCenterZ;
	float	fMinY, fMaxY, fTemp;
	int		nX, nZ;

	m_pParent = pParent;
	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;

	if( nLevel < 1 )			// Lowest level node
	{
		// Set a quad tree node pointer to access directly in map tool
		ms_pTerrain->SetQuadNodePointer( nLeftZ, nLeftX, this );

		// Calculate min & max heights
		fMinY = fMaxY = ms_pTerrain->GetHeights( nLeftZ, nLeftX );
		for( nZ = nLeftZ; nZ <= nRightZ; nZ++ )
		{
			for( nX = nLeftX; nX <= nRightX; nX++ )
			{
				fTemp = ms_pTerrain->GetHeights( nZ, nX );
				if( fMinY > fTemp ) fMinY = fTemp;
				if( fMaxY < fTemp ) fMaxY = fTemp;
			}
		}

		// Set a bounding box of lowest level node
		m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
		m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );

		// Set a texture ID
		m_nL1TexID = pL1TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];
		m_nL2TexID = pL2TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];
		m_nL3TexID = pL3TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];		

		return ;
	}
	
	// Prepare for child node
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft = new CNKTerQuadNode( this, nLevel, nLeftX, nLeftZ, nCenterX, nCenterZ, 
									pL1TexID, pL2TexID, pL3TexID );
	m_pLRight = new CNKTerQuadNode( this, nLevel, nCenterX, nLeftZ, nRightX, nCenterZ,
									pL1TexID, pL2TexID, pL3TexID );
	m_pULeft = new CNKTerQuadNode( this, nLevel, nLeftX, nCenterZ, nCenterX, nRightZ,
									pL1TexID, pL2TexID, pL3TexID );
	m_pURight = new CNKTerQuadNode( this, nLevel, nCenterX, nCenterZ, nRightX, nRightZ,
									pL1TexID, pL2TexID, pL3TexID );

	// Get min & max heights of lower level node
	fMinY = m_pLLeft->m_vMin.y;
	fMaxY = m_pLLeft->m_vMax.y;

	// Calculate min & max heights of current level node
	if( fMinY > m_pLRight->m_vMin.y ) fMinY = m_pLRight->m_vMin.y;
	if( fMinY > m_pULeft->m_vMin.y ) fMinY = m_pULeft->m_vMin.y;
	if( fMinY > m_pURight->m_vMin.y ) fMinY = m_pURight->m_vMin.y;

	if( fMaxY < m_pLRight->m_vMax.y ) fMaxY = m_pLRight->m_vMax.y;
	if( fMaxY < m_pULeft->m_vMax.y ) fMaxY = m_pULeft->m_vMax.y;
	if( fMaxY < m_pURight->m_vMax.y ) fMaxY = m_pURight->m_vMax.y;

	// Set a bounding box of current level node
	m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
	m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );
}


//-----------------------------------------------------------------------------
// Name : CNKTerQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKTerQuadNode::CNKTerQuadNode( int nLevel, int nLeftX, int nLeftZ,	int nRightX, int nRightZ,
								unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID )
{
	int		nCenterX, nCenterZ;
	float	fMinY, fMaxY, fTemp;
	int		nX, nZ;

	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;

	if( nLevel < 1 )			// Lowest level node
	{
		// Set a quad tree node pointer to access directly in map tool
		ms_pTerrain->SetQuadNodePointer( nLeftZ, nLeftX, this );

		// Calculate min & max heights
		fMinY = fMaxY = ms_pTerrain->GetHeights( nLeftZ, nLeftX );
		for( nZ = nLeftZ; nZ <= nRightZ; nZ++ )
		{
			for( nX = nLeftX; nX <= nRightX; nX++ )
			{
				fTemp = ms_pTerrain->GetHeights( nZ, nX );
				if( fMinY > fTemp ) fMinY = fTemp;
				if( fMaxY < fTemp ) fMaxY = fTemp;
			}
		}

		// Set a bounding box of lowest level node
		m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
		m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );

		// Set a texture ID
		m_nL1TexID = pL1TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];
		m_nL2TexID = pL2TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];
		m_nL3TexID = pL3TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];	

		return ;
	}
	
	// Prepare for child node
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft = new CNKTerQuadNode( nLevel, nLeftX, nLeftZ, nCenterX, nCenterZ, pL1TexID, pL2TexID, pL3TexID );
	m_pLRight = new CNKTerQuadNode( nLevel, nCenterX, nLeftZ, nRightX, nCenterZ, pL1TexID, pL2TexID, pL3TexID );
	m_pULeft = new CNKTerQuadNode( nLevel, nLeftX, nCenterZ, nCenterX, nRightZ, pL1TexID, pL2TexID, pL3TexID );
	m_pURight = new CNKTerQuadNode( nLevel, nCenterX, nCenterZ, nRightX, nRightZ, pL1TexID, pL2TexID, pL3TexID );

	// Get min & max heights of lower level node
	fMinY = m_pLLeft->m_vMin.y;
	fMaxY = m_pLLeft->m_vMax.y;

	// Calculate min & max heights of current level node
	if( fMinY > m_pLRight->m_vMin.y ) fMinY = m_pLRight->m_vMin.y;
	if( fMinY > m_pULeft->m_vMin.y ) fMinY = m_pULeft->m_vMin.y;
	if( fMinY > m_pURight->m_vMin.y ) fMinY = m_pURight->m_vMin.y;

	if( fMaxY < m_pLRight->m_vMax.y ) fMaxY = m_pLRight->m_vMax.y;
	if( fMaxY < m_pULeft->m_vMax.y ) fMaxY = m_pULeft->m_vMax.y;
	if( fMaxY < m_pURight->m_vMax.y ) fMaxY = m_pURight->m_vMax.y;

	// Set a bounding box of current level node
	m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
	m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );
}


//-----------------------------------------------------------------------------
// Name : ~CNKTerQuadNode()
// Desc : Destructor
//-----------------------------------------------------------------------------
CNKTerQuadNode::~CNKTerQuadNode()
{
	SAFE_DELETE( m_pLLeft );
	SAFE_DELETE( m_pLRight );
	SAFE_DELETE( m_pULeft );
	SAFE_DELETE( m_pURight );
}


//-----------------------------------------------------------------------------
// Name : InitVertexBuffer()
// Desc : 
//-----------------------------------------------------------------------------
void CNKTerQuadNode::InitVertexBuffer( int nLeftX, int nLeftZ, int nRightX, int nRightZ,
										int nOffsetX, int nOffsetZ, NKVERTEXT2* pVB )
{
	int	nCenterX, nCenterZ;
	int	nX, nZ;
	
	// Prepare for child node
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	if( m_pLLeft->m_pLLeft == NULL )		// If this node is lowest level node
	{
		// Vertex buffer data
		pVB[ms_nVBIdxCnt].x = pVB[ms_nVBIdxCnt+3].x = pVB[ms_nVBIdxCnt+6].x =
								nLeftX * ms_pTerrain->GetPatchSize();
		pVB[ms_nVBIdxCnt+1].x = pVB[ms_nVBIdxCnt+4].x = pVB[ms_nVBIdxCnt+7].x =
								nCenterX * ms_pTerrain->GetPatchSize();
		pVB[ms_nVBIdxCnt+2].x = pVB[ms_nVBIdxCnt+5].x = pVB[ms_nVBIdxCnt+8].x =
								nRightX * ms_pTerrain->GetPatchSize();

		pVB[ms_nVBIdxCnt].z = pVB[ms_nVBIdxCnt+1].z = pVB[ms_nVBIdxCnt+2].z =
								nLeftZ * ms_pTerrain->GetPatchSize();
		pVB[ms_nVBIdxCnt+3].z = pVB[ms_nVBIdxCnt+4].z = pVB[ms_nVBIdxCnt+5].z =
								nCenterZ * ms_pTerrain->GetPatchSize();
		pVB[ms_nVBIdxCnt+6].z = pVB[ms_nVBIdxCnt+7].z = pVB[ms_nVBIdxCnt+8].z =
								nRightZ * ms_pTerrain->GetPatchSize();
		
		pVB[ms_nVBIdxCnt].u1 = pVB[ms_nVBIdxCnt+3].u1 = pVB[ms_nVBIdxCnt+6].u1 = 0.0f;
		pVB[ms_nVBIdxCnt+1].u1 = pVB[ms_nVBIdxCnt+4].u1 = pVB[ms_nVBIdxCnt+7].u1 = 0.5f;
		pVB[ms_nVBIdxCnt+2].u1 = pVB[ms_nVBIdxCnt+5].u1 = pVB[ms_nVBIdxCnt+8].u1 = 1.0f;
		pVB[ms_nVBIdxCnt].v1 = pVB[ms_nVBIdxCnt+1].v1 = pVB[ms_nVBIdxCnt+2].v1 = 1.0f;
		pVB[ms_nVBIdxCnt+3].v1 = pVB[ms_nVBIdxCnt+4].v1 = pVB[ms_nVBIdxCnt+5].v1 = 0.5f;
		pVB[ms_nVBIdxCnt+6].v1 = pVB[ms_nVBIdxCnt+7].v1 = pVB[ms_nVBIdxCnt+8].v1 = 0.0f;

		int nWidth = 1 << ms_pTerrain->GetQuadTreeDepth();
		pVB[ms_nVBIdxCnt].u2 = pVB[ms_nVBIdxCnt+3].u2 = pVB[ms_nVBIdxCnt+6].u2 =
			((nLeftX + nWidth*nOffsetX) << 1) * ms_fdt;
		pVB[ms_nVBIdxCnt+1].u2 = pVB[ms_nVBIdxCnt+4].u2 = pVB[ms_nVBIdxCnt+7].u2 =
			((nCenterX + nWidth*nOffsetX) << 1) * ms_fdt;
		pVB[ms_nVBIdxCnt+2].u2 = pVB[ms_nVBIdxCnt+5].u2 = pVB[ms_nVBIdxCnt+8].u2 =
			((nRightX + nWidth*nOffsetX) << 1) * ms_fdt;
		pVB[ms_nVBIdxCnt].v2 = pVB[ms_nVBIdxCnt+1].v2 = pVB[ms_nVBIdxCnt+2].v2 =
			((nLeftZ + nWidth*nOffsetZ) << 1) * ms_fdt;
		pVB[ms_nVBIdxCnt+3].v2 = pVB[ms_nVBIdxCnt+4].v2 = pVB[ms_nVBIdxCnt+5].v2 =
			((nCenterZ + nWidth*nOffsetZ) << 1) * ms_fdt;
		pVB[ms_nVBIdxCnt+6].v2 = pVB[ms_nVBIdxCnt+7].v2 = pVB[ms_nVBIdxCnt+8].v2 =
			((nRightZ + nWidth*nOffsetZ ) << 1) * ms_fdt;

		int nCount = 0;
		for( nZ = nLeftZ; nZ <= nRightZ; nZ++ )
		{
			for( nX = nLeftX; nX <= nRightX; nX++)
			{
				pVB[ms_nVBIdxCnt + nCount].y = ms_pTerrain->GetHeights( nZ, nX );
				nCount++;
			}
		}

		// Vertex buffer index
		m_pLLeft->m_wVBIndex = ms_nVBIdxCnt;
		m_pLRight->m_wVBIndex = ms_nVBIdxCnt + 1;
		m_pULeft->m_wVBIndex = ms_nVBIdxCnt + 3;
		m_pURight->m_wVBIndex = ms_nVBIdxCnt + 4;
		ms_nVBIdxCnt += 9;

		return ;
	}

	// Recursive call
	m_pLLeft->InitVertexBuffer( nLeftX, nLeftZ, nCenterX, nCenterZ, nOffsetX, nOffsetZ, pVB );
	m_pLRight->InitVertexBuffer( nCenterX, nLeftZ, nRightX, nCenterZ, nOffsetX, nOffsetZ, pVB );
	m_pULeft->InitVertexBuffer( nLeftX, nCenterZ, nCenterX, nRightZ, nOffsetX, nOffsetZ, pVB );
	m_pURight->InitVertexBuffer( nCenterX, nCenterZ, nRightX, nRightZ, nOffsetX, nOffsetZ, pVB );
}


//-----------------------------------------------------------------------------
// Name : Render()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerQuadNode::Render( int nCullingState )
{
	if( nCullingState != VFCULL_INSIDE )
	{
		nCullingState = g_pMyNKApp->GetCamera()->CullAABB( m_vMin, m_vMax );
		if( nCullingState == VFCULL_OUTSIDE )
			return ;
	}

	// Render if this node is lowest level node
	if( m_pLLeft == NULL )
	{
		DWORD dwCount;

		dwCount = ms_pTerrain->m_vecIBL1[m_nL1TexID]->dwCount;
		ms_pTerrain->m_vecIBL1[m_nL1TexID]->pIB[dwCount++] = m_wVBIndex + 3;
		ms_pTerrain->m_vecIBL1[m_nL1TexID]->pIB[dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_vecIBL1[m_nL1TexID]->pIB[dwCount++] = m_wVBIndex;
		ms_pTerrain->m_vecIBL1[m_nL1TexID]->pIB[dwCount++] = m_wVBIndex;
		ms_pTerrain->m_vecIBL1[m_nL1TexID]->pIB[dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_vecIBL1[m_nL1TexID]->pIB[dwCount++] = m_wVBIndex + 1;
		ms_pTerrain->m_vecIBL1[m_nL1TexID]->dwCount = dwCount;

		dwCount = ms_pTerrain->m_vecIBL2[m_nL2TexID]->dwCount;
		ms_pTerrain->m_vecIBL2[m_nL2TexID]->pIB[dwCount++] = m_wVBIndex + 3;
		ms_pTerrain->m_vecIBL2[m_nL2TexID]->pIB[dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_vecIBL2[m_nL2TexID]->pIB[dwCount++] = m_wVBIndex;
		ms_pTerrain->m_vecIBL2[m_nL2TexID]->pIB[dwCount++] = m_wVBIndex;
		ms_pTerrain->m_vecIBL2[m_nL2TexID]->pIB[dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_vecIBL2[m_nL2TexID]->pIB[dwCount++] = m_wVBIndex + 1;
		ms_pTerrain->m_vecIBL2[m_nL2TexID]->dwCount = dwCount;

		dwCount = ms_pTerrain->m_vecIBL3[m_nL3TexID]->dwCount;
		ms_pTerrain->m_vecIBL3[m_nL3TexID]->pIB[dwCount++] = m_wVBIndex + 3;
		ms_pTerrain->m_vecIBL3[m_nL3TexID]->pIB[dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_vecIBL3[m_nL3TexID]->pIB[dwCount++] = m_wVBIndex;
		ms_pTerrain->m_vecIBL3[m_nL3TexID]->pIB[dwCount++] = m_wVBIndex;
		ms_pTerrain->m_vecIBL3[m_nL3TexID]->pIB[dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_vecIBL3[m_nL3TexID]->pIB[dwCount++] = m_wVBIndex + 1;
		ms_pTerrain->m_vecIBL3[m_nL3TexID]->dwCount = dwCount;
		
		return ;
	}

	// Recursive call
	m_pLLeft->Render( nCullingState );
	m_pLRight->Render( nCullingState );
	m_pULeft->Render( nCullingState );
	m_pURight->Render( nCullingState );
}


//-----------------------------------------------------------------------------
// Name : RenderGrid()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerQuadNode::RenderGrid( int nLevel, DWORD dwColor, int nLeftX, int nLeftZ,
									int nRightX, int nRightZ, int nCullingState )
{
	if( nCullingState != VFCULL_INSIDE )
	{
		nCullingState = g_pMyNKApp->GetCamera()->CullAABB( m_vMin, m_vMax );
		if( nCullingState == VFCULL_OUTSIDE )
			return ;
	}

	if( nLevel == 0 )
	{
		NKVERTEXL	aGrid[17];
		int			nCount = 0;
		int			nX, nZ;
		
		for( nX = nLeftX; nX < nRightX; nX++ )
		{
			aGrid[nCount].x = nX * ms_pTerrain->GetPatchSize();
			aGrid[nCount].y = ms_pTerrain->GetHeights( nLeftZ, nX ) + 0.5f;
			aGrid[nCount].z = nLeftZ * ms_pTerrain->GetPatchSize();
			aGrid[nCount++].c = dwColor;
		}
		for( nZ = nLeftZ; nZ < nRightZ; nZ++ )
		{
			aGrid[nCount].x = nRightX * ms_pTerrain->GetPatchSize();
			aGrid[nCount].y = ms_pTerrain->GetHeights( nZ, nRightX ) + 0.5f;
			aGrid[nCount].z = nZ * ms_pTerrain->GetPatchSize();
			aGrid[nCount++].c = dwColor;
		}
		for( nX = nRightX; nX > nLeftX; nX-- )
		{
			aGrid[nCount].x = nX * ms_pTerrain->GetPatchSize();
			aGrid[nCount].y = ms_pTerrain->GetHeights( nRightZ, nX ) + 0.5f;
			aGrid[nCount].z = nRightZ * ms_pTerrain->GetPatchSize();
			aGrid[nCount++].c = dwColor;
		}
		for( nZ = nRightZ; nZ >= nLeftZ; nZ-- )
		{
			aGrid[nCount].x = nLeftX * ms_pTerrain->GetPatchSize();
			aGrid[nCount].y = ms_pTerrain->GetHeights( nZ, nLeftX ) + 0.5f;
			aGrid[nCount].z = nZ * ms_pTerrain->GetPatchSize();
			aGrid[nCount++].c = dwColor;
		}

		ms_pTerrain->GetD3DDevice()->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, aGrid, nCount, 0 );

		return ;
	}

	// Prepare for child node
	int		nCenterX, nCenterZ;
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft->RenderGrid( nLevel, dwColor, nLeftX, nLeftZ, nCenterX, nCenterZ, nCullingState );
	m_pLRight->RenderGrid( nLevel, dwColor, nCenterX, nLeftZ, nRightX, nCenterZ, nCullingState );
	m_pULeft->RenderGrid( nLevel, dwColor, nLeftX, nCenterZ, nCenterX, nRightZ, nCullingState );
	m_pURight->RenderGrid( nLevel, dwColor, nCenterX, nCenterZ, nRightX, nRightZ, nCullingState );	
}


//-----------------------------------------------------------------------------
// Name : RenderLayerGrid()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerQuadNode::RenderLayerGrid( int nLevel, DWORD dwColor, int nLayer,
										int nLeftX, int nLeftZ, int nRightX, int nRightZ, int nCullingState )
{
	if( nCullingState != VFCULL_INSIDE )
	{
		nCullingState = g_pMyNKApp->GetCamera()->CullAABB( m_vMin, m_vMax );
		if( nCullingState == VFCULL_OUTSIDE )
			return ;
	}

	if( nLevel == 0 )		// Lowest level node
	{		
		if( ( nLayer == 1 && m_nL1TexID == TEXTURE_MAX_COUNT ) || ( nLayer == 2 && m_nL2TexID == TEXTURE_MAX_COUNT ) || 
			( nLayer == 4 && m_nL3TexID == TEXTURE_MAX_COUNT ) )
			return ;

		NKVERTEXL	aGrid[17];
		int			nCount = 0;
		int			nX, nZ;
		
		for( nX = nLeftX; nX < nRightX; nX++ )
		{
			aGrid[nCount].x = nX * ms_pTerrain->GetPatchSize();
			aGrid[nCount].y = ms_pTerrain->GetHeights( nLeftZ, nX ) + 0.5f;
			aGrid[nCount].z = nLeftZ * ms_pTerrain->GetPatchSize();
			aGrid[nCount++].c = dwColor;
		}
		for( nZ = nLeftZ; nZ < nRightZ; nZ++ )
		{
			aGrid[nCount].x = nRightX * ms_pTerrain->GetPatchSize();
			aGrid[nCount].y = ms_pTerrain->GetHeights( nZ, nRightX ) + 0.5f;
			aGrid[nCount].z = nZ * ms_pTerrain->GetPatchSize();
			aGrid[nCount++].c = dwColor;
		}
		for( nX = nRightX; nX > nLeftX; nX-- )
		{
			aGrid[nCount].x = nX * ms_pTerrain->GetPatchSize();
			aGrid[nCount].y = ms_pTerrain->GetHeights( nRightZ, nX ) + 0.5f;
			aGrid[nCount].z = nRightZ * ms_pTerrain->GetPatchSize();
			aGrid[nCount++].c = dwColor;
		}
		for( nZ = nRightZ; nZ >= nLeftZ; nZ-- )
		{
			aGrid[nCount].x = nLeftX * ms_pTerrain->GetPatchSize();
			aGrid[nCount].y = ms_pTerrain->GetHeights( nZ, nLeftX ) + 0.5f;
			aGrid[nCount].z = nZ * ms_pTerrain->GetPatchSize();
			aGrid[nCount++].c = dwColor;
		}

		ms_pTerrain->GetD3DDevice()->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, aGrid, nCount, 0 );
		
		return ;
	}

	// Prepare for child node
	int		nCenterX, nCenterZ;
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft->RenderLayerGrid( nLevel, dwColor, nLayer, nLeftX, nLeftZ, nCenterX, nCenterZ, nCullingState );
	m_pLRight->RenderLayerGrid( nLevel, dwColor, nLayer, nCenterX, nLeftZ, nRightX, nCenterZ, nCullingState );
	m_pULeft->RenderLayerGrid( nLevel, dwColor, nLayer, nLeftX, nCenterZ, nCenterX, nRightZ, nCullingState );
	m_pURight->RenderLayerGrid( nLevel, dwColor, nLayer, nCenterX, nCenterZ, nRightX, nRightZ, nCullingState );	
}


//-----------------------------------------------------------------------------
// Name : GetPickedPoint()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerQuadNode::GetPickedPoint( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir, float& rDist )
{
	if( !NKIntersectRayAABB( rOrigin, rDir, m_vMin, m_vMax ) )
		return ;

	// Test a intersection between a ray and a triangle if this node is lowest level node
	if( m_pLLeft == NULL )
	{
		D3DXVECTOR3	vP0, vP1, vP2;
		float		fDist2;
		int			nMinX, nMinZ, nMaxX, nMaxZ;		
		float		fPatchSize = ms_pTerrain->GetPatchSize();

		nMinX = m_vMin.x / fPatchSize;
		nMinZ = m_vMin.z / fPatchSize;
		nMaxX = nMinX + 1;
		nMaxZ = nMinZ + 1;

		// Upper left triangle ( vP0, vP1, vP2 )
		vP0 = D3DXVECTOR3( m_vMin.x, ms_pTerrain->GetHeights( nMaxZ, nMinX ), m_vMax.z );
		vP1 = D3DXVECTOR3( m_vMax.x, ms_pTerrain->GetHeights( nMaxZ, nMaxX ), m_vMax.z );
		vP2 = D3DXVECTOR3( m_vMin.x, ms_pTerrain->GetHeights( nMinZ, nMinX ), m_vMin.z );
		if( NKIntersectRayTri( rOrigin, rDir, vP0, vP1, vP2, &fDist2 ) )
		{
			if( fDist2 < rDist )
				rDist = fDist2;
		}

		// Lower right triangle ( vP2, vP1, vP0 )
		vP0 = D3DXVECTOR3( m_vMax.x, ms_pTerrain->GetHeights( nMinZ, nMaxX ), m_vMin.z );
		if( NKIntersectRayTri( rOrigin, rDir, vP2, vP1, vP0, &fDist2 ) )
		{
			if( fDist2 < rDist )
				rDist = fDist2;
		}

		return ;
	}

	// Recursive call
	m_pLLeft->GetPickedPoint( rOrigin, rDir, rDist );
	m_pLRight->GetPickedPoint( rOrigin, rDir, rDist );
	m_pULeft->GetPickedPoint( rOrigin, rDir, rDist );
	m_pURight->GetPickedPoint( rOrigin, rDir, rDist );
}


//-----------------------------------------------------------------------------
// Name : RecalcAllBoundingBox()
// Desc : 
//-----------------------------------------------------------------------------
void CNKTerQuadNode::RecalcAllBoundingBox( int nLeftX, int nLeftZ, int nRightX, int nRightZ )
{
	int		nCenterX, nCenterZ;
	float	fMinY, fMaxY, fTemp;
	int		nX, nZ;

	if( m_pLLeft == NULL )			// Lowest level node
	{
		// Calculate min & max heights
		fMinY = fMaxY = ms_pTerrain->GetHeights( nLeftZ, nLeftX );
		for( nZ = nLeftZ; nZ <= nRightZ; nZ++ )
		{
			for( nX = nLeftX; nX <= nRightX; nX++ )
			{
				fTemp = ms_pTerrain->GetHeights( nZ, nX );
				if( fMinY > fTemp ) fMinY = fTemp;
				if( fMaxY < fTemp ) fMaxY = fTemp;
			}
		}

		// Set a bounding box of lowest level node
		m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
		m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );

		return ;
	}
	
	// Prepare for child node
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft->RecalcAllBoundingBox( nLeftX, nLeftZ, nCenterX, nCenterZ );
	m_pLRight->RecalcAllBoundingBox( nCenterX, nLeftZ, nRightX, nCenterZ );
	m_pULeft->RecalcAllBoundingBox( nLeftX, nCenterZ, nCenterX, nRightZ );
	m_pURight->RecalcAllBoundingBox( nCenterX, nCenterZ, nRightX, nRightZ );

	// Get min & max heights of lower level node
	fMinY = m_pLLeft->m_vMin.y;
	fMaxY = m_pLLeft->m_vMax.y;

	// Calculate min & max heights of current level node
	if( fMinY > m_pLRight->m_vMin.y ) fMinY = m_pLRight->m_vMin.y;
	if( fMinY > m_pULeft->m_vMin.y ) fMinY = m_pULeft->m_vMin.y;
	if( fMinY > m_pURight->m_vMin.y ) fMinY = m_pURight->m_vMin.y;

	if( fMaxY < m_pLRight->m_vMax.y ) fMaxY = m_pLRight->m_vMax.y;
	if( fMaxY < m_pULeft->m_vMax.y ) fMaxY = m_pULeft->m_vMax.y;
	if( fMaxY < m_pURight->m_vMax.y ) fMaxY = m_pURight->m_vMax.y;

	// Set a bounding box of current level node
	m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
	m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );
}


//-----------------------------------------------------------------------------
// Name : RecalcUpperBoundingBox()
// Desc : 
//-----------------------------------------------------------------------------
void CNKTerQuadNode::RecalcUpperBoundingBox()
{
	// Recalaculate a bounding box
	if( m_pLLeft )
	{
		float	fMinY, fMaxY;

		// Get min & max heights of lower level node
		fMinY = m_pLLeft->m_vMin.y;
		fMaxY = m_pLLeft->m_vMax.y;

		// Calculate min & max heights of current level node
		if( fMinY > m_pLRight->m_vMin.y ) fMinY = m_pLRight->m_vMin.y;
		if( fMinY > m_pULeft->m_vMin.y ) fMinY = m_pULeft->m_vMin.y;
		if( fMinY > m_pURight->m_vMin.y ) fMinY = m_pURight->m_vMin.y;

		if( fMaxY < m_pLRight->m_vMax.y ) fMaxY = m_pLRight->m_vMax.y;
		if( fMaxY < m_pULeft->m_vMax.y ) fMaxY = m_pULeft->m_vMax.y;
		if( fMaxY < m_pURight->m_vMax.y ) fMaxY = m_pURight->m_vMax.y;

		// Set new min & max heights current level node
		m_vMin.y = fMinY;
		m_vMax.y = fMaxY;
	}

	if( m_pParent == NULL )
		return ;

	// Recursive call
	m_pParent->RecalcUpperBoundingBox();
}


//-----------------------------------------------------------------------------
// Name : RecalcBoundingBox()
// Desc : 
//-----------------------------------------------------------------------------
void CNKTerQuadNode::RecalcBoundingBox( int nZ, int nX )
{
	// Recalculate a bounding box of a current node
	float	fMinY, fMaxY, fTemp;
	fMinY = fMaxY = ms_pTerrain->GetHeights( nZ, nX );

	fTemp = ms_pTerrain->GetHeights( nZ, nX + 1 );
	if( fMinY > fTemp )	fMinY = fTemp;
	if( fMaxY < fTemp ) fMaxY = fTemp;
	fTemp = ms_pTerrain->GetHeights( nZ + 1, nX );
	if( fMinY > fTemp )	fMinY = fTemp;
	if( fMaxY < fTemp ) fMaxY = fTemp;
	fTemp = ms_pTerrain->GetHeights( nZ + 1, nX + 1 );
	if( fMinY > fTemp )	fMinY = fTemp;
	if( fMaxY < fTemp ) fMaxY = fTemp;

	m_vMin.y = fMinY;
	m_vMax.y = fMaxY;

	// Recalculate a bounding box of a upper node
	m_pParent->RecalcUpperBoundingBox();		
}


//-----------------------------------------------------------------------------
// Name : CountIndices()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerQuadNode::CountIndices()
{
	if( m_pLLeft == NULL )			// If this node is lowest level node
	{
		ms_pTerrain->m_vecIBL1[m_nL1TexID]->dwTotalCount += 6;
		ms_pTerrain->m_vecIBL2[m_nL2TexID]->dwTotalCount += 6;
		ms_pTerrain->m_vecIBL3[m_nL3TexID]->dwTotalCount += 6;

		return ;
	}

	// Recursive call
	m_pLLeft->CountIndices();
	m_pLRight->CountIndices();
	m_pULeft->CountIndices();
	m_pURight->CountIndices();
}


//-----------------------------------------------------------------------------
// Name : ModifyQuadTree()
// Desc : 
//-----------------------------------------------------------------------------
void CNKTerQuadNode::ModifyQuadTree( int nLeftX, int nLeftZ, int nRightX, int nRightZ, int nSZ, int nSX,
										int nEZ, int nEX, unsigned char* pL1TexID, unsigned char* pL2TexID,
										unsigned char* pL3TexID )
{
	int		nCenterX, nCenterZ;
	float	fMinY, fMaxY, fTemp;
	int		nX, nZ;

	if( m_pLLeft == NULL )			// Lowest level node
	{
		if( nLeftX >= nSX && nLeftX <= nEX && nLeftZ >= nSZ && nLeftZ <= nEZ )
		{
			// Calculate min & max heights
			fMinY = fMaxY = ms_pTerrain->GetHeights( nLeftZ, nLeftX );
			for( nZ = nLeftZ; nZ <= nRightZ; nZ++ )
			{
				for( nX = nLeftX; nX <= nRightX; nX++ )
				{
					fTemp = ms_pTerrain->GetHeights( nZ, nX );
					if( fMinY > fTemp ) fMinY = fTemp;
					if( fMaxY < fTemp ) fMaxY = fTemp;
				}
			}

			// Set a bounding box of lowest level node
			m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
			m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );

			// Set a texture ID
			m_nL1TexID = pL1TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];
			m_nL2TexID = pL2TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];
			m_nL3TexID = pL3TexID[nLeftX + nLeftZ * ( ms_pTerrain->GetHMWidth() - 1 )];
		}

		return ;
	}
	
	// Prepare for child node
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft->ModifyQuadTree( nLeftX, nLeftZ, nCenterX, nCenterZ, nSZ, nSX, nEZ, nEX,
								pL1TexID, pL2TexID, pL3TexID );
	m_pLRight->ModifyQuadTree( nCenterX, nLeftZ, nRightX, nCenterZ, nSZ, nSX, nEZ, nEX,
								pL1TexID, pL2TexID, pL3TexID );
	m_pULeft->ModifyQuadTree( nLeftX, nCenterZ, nCenterX, nRightZ, nSZ, nSX, nEZ, nEX,
								pL1TexID, pL2TexID, pL3TexID );
	m_pURight->ModifyQuadTree( nCenterX, nCenterZ, nRightX, nRightZ, nSZ, nSX, nEZ, nEX,
								pL1TexID, pL2TexID, pL3TexID );

	// Get min & max heights of lower level node
	fMinY = m_pLLeft->m_vMin.y;
	fMaxY = m_pLLeft->m_vMax.y;

	// Calculate min & max heights of current level node
	if( fMinY > m_pLRight->m_vMin.y ) fMinY = m_pLRight->m_vMin.y;
	if( fMinY > m_pULeft->m_vMin.y ) fMinY = m_pULeft->m_vMin.y;
	if( fMinY > m_pURight->m_vMin.y ) fMinY = m_pURight->m_vMin.y;

	if( fMaxY < m_pLRight->m_vMax.y ) fMaxY = m_pLRight->m_vMax.y;
	if( fMaxY < m_pULeft->m_vMax.y ) fMaxY = m_pULeft->m_vMax.y;
	if( fMaxY < m_pURight->m_vMax.y ) fMaxY = m_pURight->m_vMax.y;

	// Set a bounding box of current level node
	m_vMin = D3DXVECTOR3( nLeftX * ms_pTerrain->GetPatchSize(), fMinY, nLeftZ * ms_pTerrain->GetPatchSize() );
	m_vMax = D3DXVECTOR3( nRightX * ms_pTerrain->GetPatchSize(), fMaxY, nRightZ * ms_pTerrain->GetPatchSize() );
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