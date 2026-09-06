//-----------------------------------------------------------------------------
// File : NKTerrain.cpp
// Desc :
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "NKRTLib.h"
#include "NKTerrain.h"
#include "NKTerQuadNode.h"
#include "NKCamera.h"
#include "D3DUtil.h"
#include "NKMath.h"
#include "MyNKApp.h"
#include "Config.h"


// Global variables
extern CMyNKApp*	g_pMyNKApp;
extern CNKObjQuadNode*	g_pObjTree;


//-----------------------------------------------------------------------------
// Name : CNKTerrain()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKTerrain::CNKTerrain()
{
	m_pD3D = NULL;
	m_pd3dDevice = NULL;
	CNKTerQuadNode::ms_pTerrain = this;
	m_szTerName = NULL;
	m_pHeights = NULL;
	m_pTempHeights = NULL;
	m_pAttributeMap = NULL;	
	m_nAttributeMapWidth = 0;
	m_dwVBSize = 0;
	m_nPatchSize = 50;
	m_nHMWidth = 0;
	m_nHeightScale = 2;	
	m_nViewLayers = 7;
	m_nTimeID = 0;
	m_vecTerTex.reserve( TEXTURE_MAX_COUNT );

	m_pAlphaMapL2 = NULL;
	m_pAlphaMapL3 = NULL;
	int i = 0;
	for( i = 0; i < 6; i++ )
	{
		m_pLightMapsR[i] = NULL;
		m_pLightMapsG[i] = NULL;
		m_pLightMapsB[i] = NULL;
	}

	for( i = 0; i < 4; i++ )
	{
		m_aRootNode[i] = NULL;
		m_aVB[i] = NULL;
		m_aClrMapTexL2[i] = NULL;
		m_aClrMapTexL3[i] = NULL;
	}

	m_vecIBL1.reserve( TEXTURE_MAX_COUNT+1 );
	m_vecIBL2.reserve( TEXTURE_MAX_COUNT+1 );
	m_vecIBL3.reserve( TEXTURE_MAX_COUNT+1 );
	for( i = 0; i < TEXTURE_MAX_COUNT+1; i++ )
	{
		m_vecIBL1.push_back( NULL );
		m_vecIBL2.push_back( NULL );
		m_vecIBL3.push_back( NULL );
	}

	m_dwTriangles = 0;
	m_pNormals = NULL;

	m_aDirectLightDir[0] = D3DXVECTOR3( 0.0f, -0.743241f, 0.669023f );
	m_aDirectLightDir[1] = D3DXVECTOR3( 0.615358f, -0.675493f, 0.406255f );
	m_aDirectLightDir[2] = D3DXVECTOR3( 0.88732f, -0.460501f, 0.0245227f );
	m_aDirectLightDir[3] = D3DXVECTOR3( 0.0f, -0.57631f, -0.817225f );
	m_aDirectLightDir[4] = D3DXVECTOR3( -0.88732f, -0.460501f, 0.0245227f );
	m_aDirectLightDir[5] = D3DXVECTOR3( -0.615358f, -0.675493f, 0.406255f );
	for( i = 0; i < 6; i++ )
		D3DXVec3Normalize( &m_aDirectLightDir[i], &m_aDirectLightDir[i] );

	m_aDirectLightR[0] = 1.0f;	m_aDirectLightG[0] = 1.0f;	m_aDirectLightB[0] = 1.0f;
	m_aDirectLightR[1] = 0.92f;	m_aDirectLightG[1] = 0.92f;	m_aDirectLightB[1] = 0.92f;
	m_aDirectLightR[2] = 0.92f;	m_aDirectLightG[2] = 0.92f;	m_aDirectLightB[2] = 0.92f;
	m_aDirectLightR[3] = 0.62f;	m_aDirectLightG[3] = 0.62f;	m_aDirectLightB[3] = 0.62f;
	m_aDirectLightR[4] = 0.92f;	m_aDirectLightG[4] = 0.92f;	m_aDirectLightB[4] = 0.92f;
	m_aDirectLightR[5] = 0.92f;	m_aDirectLightG[5] = 0.92f;	m_aDirectLightB[5] = 0.92f;
	m_aAmbientLightR[0] = 0.35f;	m_aAmbientLightG[0] = 0.35f;	m_aAmbientLightB[0] = 0.35f;
	m_aAmbientLightR[1] = 0.31f;	m_aAmbientLightG[1] = 0.31f;	m_aAmbientLightB[1] = 0.31f;
	m_aAmbientLightR[2] = 0.31f;	m_aAmbientLightG[2] = 0.31f;	m_aAmbientLightB[2] = 0.31f;
	m_aAmbientLightR[3] = 0.24f;	m_aAmbientLightG[3] = 0.24f;	m_aAmbientLightB[3] = 0.24f;
	m_aAmbientLightR[4] = 0.31f;	m_aAmbientLightG[4] = 0.31f;	m_aAmbientLightB[4] = 0.31f;
	m_aAmbientLightR[5] = 0.31f;	m_aAmbientLightG[5] = 0.31f;	m_aAmbientLightB[5] = 0.31f;
}


//-----------------------------------------------------------------------------
// Name : ~CNKTerrain()
// Desc : Destructor
//-----------------------------------------------------------------------------
CNKTerrain::~CNKTerrain()
{
	Destroy();

	m_vecIBL1.erase( m_vecIBL1.begin(), m_vecIBL1.end() );
	m_vecIBL2.erase( m_vecIBL2.begin(), m_vecIBL2.end() );
	m_vecIBL3.erase( m_vecIBL3.begin(), m_vecIBL3.end() );
}


//-----------------------------------------------------------------------------
// Name : SetTerrainName()
// Desc : 
//-----------------------------------------------------------------------------
void CNKTerrain::SetTerrainName( const TCHAR* szTerrainName )
{
	m_szTerName = new TCHAR[_tcslen( szTerrainName ) + 1];
	_tcscpy( m_szTerName, szTerrainName );
	m_szTerName[_tcslen( szTerrainName )] = _T('\0');
}


//-----------------------------------------------------------------------------
// Name : Init()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKTerrain::Init( int nWHSegment, int nPatchSize, int nHeightScale, 
						const TCHAR* szHeightMap )
{
	// Progress dialog
	g_pMyNKApp->SetProgressTitle( _T(" Create Terrain ") );
	g_pMyNKApp->SetProgressContent( _T("Load a height map.\n")
									_T("Create a quad tree and a vertex buffer of a terrain.\n") );
	g_pMyNKApp->SetProgressRange( 0, 100 );
	g_pMyNKApp->SetProgressPos( 0 );

	m_nHMWidth = nWHSegment + 1;
	m_nPatchSize = nPatchSize;
	m_nHeightScale = nHeightScale;

	nWHSegment = nWHSegment == 256 ? 128 : nWHSegment;
	m_nVBWidth = nWHSegment * 3 / 2;								// W/H size of vertex buffer is less than 256
	m_dwVBSize = m_nVBWidth * m_nVBWidth;

	// Get a quad tree depth
	m_nQuadTreeDepth = 0;
	while( nWHSegment >>= 1 )
		m_nQuadTreeDepth++;

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 3 );

	// Add index buffer(15) without a texture
	TerIB*	pIB;
	pIB = new TerIB;
	m_vecIBL1[TEXTURE_MAX_COUNT] = pIB;
	pIB->pIB = new WORD[128*128*6];
	pIB = new TerIB;
	m_vecIBL2[TEXTURE_MAX_COUNT] = pIB;
	pIB->pIB = new WORD[128*128*6];
	pIB = new TerIB;
	m_vecIBL3[TEXTURE_MAX_COUNT] = pIB;
	pIB->pIB = new WORD[128*128*6];

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 5 );

	// Load height map
	LoadHeightMap( szHeightMap );

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 15 );	

	// Initialize quad tree
	if( !InitQuadTree() )
	{
		Destroy();
		return false;
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 55 );
	
	// Initialize vertex buffer
	if( !InitVertexBuffer() )
	{
		Destroy();
		return false;
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 95 );

	// Initialize a color map
	int	nWidth = ( m_nHMWidth -1 ) * 2;
	for( int nZ = 0; nZ < nWidth; nZ++ )
	{
		for( int nX = 0; nX < nWidth; nX++ )
		{
			m_aAlphaMapL2[nZ][nX] = 0;
			m_aAlphaMapL3[nZ][nX] = 0;
			m_aAlphaL2[nZ][nX] = 0.0f;
			m_aAlphaL3[nZ][nX] = 0.0f;
			m_aShadowR[0][nZ][nX] = 1.0f;	m_aShadowG[0][nZ][nX] = 1.0f;	m_aShadowB[0][nZ][nX] = 1.0f;
			m_aShadowR[1][nZ][nX] = 1.0f;	m_aShadowG[1][nZ][nX] = 1.0f;	m_aShadowB[1][nZ][nX] = 1.0f;
			m_aShadowR[2][nZ][nX] = 1.0f;	m_aShadowG[2][nZ][nX] = 1.0f;	m_aShadowB[2][nZ][nX] = 1.0f;
			m_aShadowR[3][nZ][nX] = 1.0f;	m_aShadowG[3][nZ][nX] = 1.0f;	m_aShadowB[3][nZ][nX] = 1.0f;
			m_aShadowR[4][nZ][nX] = 1.0f;	m_aShadowG[4][nZ][nX] = 1.0f;	m_aShadowB[4][nZ][nX] = 1.0f;
			m_aShadowR[5][nZ][nX] = 1.0f;	m_aShadowG[5][nZ][nX] = 1.0f;	m_aShadowB[5][nZ][nX] = 1.0f;
		}
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 100 );

	// Make normals
	MakeNormals();

	// Make rgb maps
	MakeAllRGBMaps();

	// Create color map tesxtures
	if( FAILED( CreateClrMapTexture() ) )
	{
		Destroy();
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : Destroy()
// Desc : 
//-----------------------------------------------------------------------------
void CNKTerrain::Destroy()
{
	InvalidateDeviceObjects();

	SAFE_DELETE_ARRAY( m_szTerName );
	SAFE_DELETE_ARRAY( m_pHeights );
	SAFE_DELETE_ARRAY( m_pTempHeights );
	SAFE_DELETE_ARRAY( m_pNormals );
	SAFE_DELETE_ARRAY( m_pAttributeMap );
	m_nAttributeMapWidth = 0;

	SAFE_DELETE_ARRAY( m_pAlphaMapL2 );
	SAFE_DELETE_ARRAY( m_pAlphaMapL3 );

	int i = 0;
	for( i = 0; i < 6; i++ )
	{
		SAFE_DELETE_ARRAY( m_pLightMapsR[i] );
		SAFE_DELETE_ARRAY( m_pLightMapsG[i] );
		SAFE_DELETE_ARRAY( m_pLightMapsB[i] );
	}

	for( i = 0; i < 4; i++ )
		SAFE_DELETE( m_aRootNode[i] );

	for( i = 0; i < m_vecIBL1.size(); i++ )
	{
		if( m_vecIBL1[i] )
		{
			SAFE_DELETE_ARRAY( m_vecIBL1[i]->pIB );
			SAFE_DELETE( m_vecIBL1[i] );
		}
		if( m_vecIBL2[i] )
		{
			SAFE_DELETE_ARRAY( m_vecIBL2[i]->pIB );
			SAFE_DELETE( m_vecIBL2[i] );
		}
		if( m_vecIBL3[i] )
		{
			SAFE_DELETE_ARRAY( m_vecIBL3[i]->pIB );
			SAFE_DELETE( m_vecIBL3[i] );
		}
	}	

	for( i = 0; i < m_vecTerTex.size(); i++ )
		SAFE_DELETE( m_vecTerTex[i] );
	m_vecTerTex.erase( m_vecTerTex.begin(), m_vecTerTex.end() );
}


//-----------------------------------------------------------------------------
// Name : RestoreDeviceObjects()
// Desc : 
//-----------------------------------------------------------------------------
HRESULT CNKTerrain::RestoreDeviceObjects( LPDIRECT3D7 pD3D, LPDIRECT3DDEVICE7 pd3dDevice )
{
	m_pD3D = pD3D;
	m_pd3dDevice = pd3dDevice;

	// Restore terrain vertex buffer
	if( InitVertexBuffer() == false )
		return E_FAIL;
	
	// Restore terrain textures
	for( int i = 0; i < m_vecTerTex.size(); i++ )
		m_vecTerTex[i]->RestoreAllTextures( m_pd3dDevice );

	// Create color map texture
	if( FAILED( CreateClrMapTexture() ) )
		return E_FAIL;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : InvalidateDeviceObjects()
// Desc : 
//-----------------------------------------------------------------------------
HRESULT CNKTerrain::InvalidateDeviceObjects()
{
	// Release terrain vertex buffer & color map textures
	int i = 0;
	for( i = 0; i < 4; i++ )
	{
		SAFE_RELEASE( m_aVB[i] );
		SAFE_RELEASE( m_aClrMapTexL2[i] );
		SAFE_RELEASE( m_aClrMapTexL3[i] );
	}

	// Release terrain textures
	for( i = 0; i < m_vecTerTex.size(); i++ )
		m_vecTerTex[i]->InvalidateAllTextures();
	
	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : LoadHeightMap()
// Desc : 
//-----------------------------------------------------------------------------
void CNKTerrain::LoadHeightMap( const TCHAR* szFilename )
{
	FILE*	fp;

	SAFE_DELETE_ARRAY( m_pHeights );
	SAFE_DELETE_ARRAY( m_pTempHeights );

	unsigned char*	pTempHeights = new unsigned char[m_nHMWidth * m_nHMWidth];
	m_pHeights = new unsigned char[m_nHMWidth * m_nHMWidth];
	m_pTempHeights = new float[m_nHMWidth * m_nHMWidth];
	
	m_nMinHeight = 255;
	m_nMaxHeight = 255;

	if( szFilename[0] == _T('\0') )									// If heightmap name is not specified
	{
		memset( m_pHeights, 0, m_nHMWidth * m_nHMWidth * sizeof( unsigned char) );
		//memset( m_pTempHeights, 0, m_nHMWidth * m_nHMWidth );
		// [10/15/2007 Theodoric] 디버깅 모드에서 맵의 높이가 마구 쓷는 버그 수정 : 메모리 초기화 에러 ㅡㅡ;;
		memset( m_pTempHeights, 0, m_nHMWidth * m_nHMWidth * sizeof(float) );
		m_nMinHeight = 0;
		m_nMaxHeight = 0;
	}
	else 
	{
		TCHAR	szPath[256];
		_tcscpy( szPath, _T("map\\") );
		_tcscat( szPath, m_szTerName );
		_tcscat( szPath, _T("\\") );
		_tcscat( szPath, szFilename );

		if( ( fp = fopen( szPath, "rb") ) != NULL )
		{
			int nCount = 0;
			nCount = fread( pTempHeights, sizeof(unsigned char), m_nHMWidth * m_nHMWidth, fp );
			fclose( fp );
	
			if( nCount != m_nHMWidth * m_nHMWidth )						// If file reading failed
			{
				TCHAR	szTemp[512];
				_tcscpy( szTemp, _T("'") );
				_tcscat( szTemp, szFilename );
				_tcscat( szTemp, _T("' 파일의 크기가 지형 크기보다 작습니다.\n\n0으로 초기화됩니다") );
				MessageBox( NULL, szTemp, _T("NKMapTool"), MB_ICONWARNING | MB_OK );

				memset( m_pHeights, 0, m_nHMWidth * m_nHMWidth );
				memset( m_pTempHeights, 0, m_nHMWidth * m_nHMWidth );
				m_nMinHeight = 0;
				m_nMaxHeight = 0;
			}
		}
		else															// If file opening failed
		{
			TCHAR	szTemp[512];
			_tcscpy( szTemp, _T("'") );
			_tcscat( szTemp, szFilename );
			_tcscat( szTemp, _T("' 파일을 열 수 없습니다. \n\n0으로 초기화됩니다") );
			MessageBox( NULL, szTemp, _T("NKMapTool"), MB_ICONWARNING | MB_OK );

			memset( m_pHeights, 0, m_nHMWidth * m_nHMWidth );
			memset( m_pTempHeights, 0, m_nHMWidth * m_nHMWidth );
			m_nMinHeight = 0;
			m_nMaxHeight = 0;
		}
	}

	if( m_nMinHeight == 255 && m_nMaxHeight == 255 )	
	{
		int	i, nX, nZ;
		// Vertical inverse
		for( nZ = 0; nZ < m_nHMWidth; nZ++ )
			memcpy( &m_pHeights[nZ * m_nHMWidth], &pTempHeights[( m_nHMWidth - nZ - 1 ) * m_nHMWidth],
					sizeof(unsigned char) * m_nHMWidth );

		// Get min & max heights		
		m_nMinHeight = 255;
		m_nMaxHeight = 0;

		for( i = 0, nZ = 0; nZ < m_nHMWidth; nZ++ )
		{
			for( nX = 0; nX < m_nHMWidth; nX++ )
			{
				if( m_pHeights[i] < m_nMinHeight )
					m_nMinHeight = m_pHeights[i];
				if( m_pHeights[i] > m_nMaxHeight )
					m_nMaxHeight = m_pHeights[i];

				m_pTempHeights[i] = (float)m_pHeights[i];
				i++;
			}
		}
	}

	SAFE_DELETE_ARRAY( pTempHeights );
}


//-----------------------------------------------------------------------------
// Name : InitQuadTree()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKTerrain::InitQuadTree( unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID )
{
	int nWidth = 1 << m_nQuadTreeDepth;
	int nRightX = nWidth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;
	
	if( pL1TexID )
	{
		// Lower left vertex buffer
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			m_aRootNode[0] = new CNKTerQuadNode( m_nQuadTreeDepth, 0, 0, nRightX, nRightZ,
													pL1TexID, pL2TexID, pL3TexID );
		}
		else
		{
			m_aRootNode[0] = new CNKTerQuadNode( NULL, m_nQuadTreeDepth, 0, 0, nRightX, nRightZ,
													pL1TexID, pL2TexID, pL3TexID );
		}
	}
	else
	{
		// Lower left vertex buffer
		m_aRootNode[0] = new CNKTerQuadNode( NULL, m_nQuadTreeDepth, 0, 0, nRightX, nRightZ );
	}

	nCenterX = nRightX;
	nCenterZ = nRightZ;
	nRightX <<= 1;
	nRightZ <<= 1;

	if( m_nHMWidth == 257 )				
	{
		if( pL1TexID )
		{
			if( g_pMyNKApp->IsLoadedGameMap() )
			{
				// Lower right vertex buffer
				m_aRootNode[1] = new CNKTerQuadNode( m_nQuadTreeDepth, nCenterX, 0, nRightX, nCenterZ,
														pL1TexID, pL2TexID, pL3TexID );

				// Upper left vertex buffer
				m_aRootNode[2] = new CNKTerQuadNode( m_nQuadTreeDepth, 0, nCenterZ, nCenterX, nRightZ,
														pL1TexID, pL2TexID, pL3TexID );

				// Upper right vertex buffer
				m_aRootNode[3] = new CNKTerQuadNode( m_nQuadTreeDepth, nCenterX, nCenterZ, nRightX, nRightZ,
														pL1TexID, pL2TexID, pL3TexID );
			}
			else
			{
				// Lower right vertex buffer
				m_aRootNode[1] = new CNKTerQuadNode( NULL, m_nQuadTreeDepth, nCenterX, 0, nRightX, nCenterZ,
														pL1TexID, pL2TexID, pL3TexID );

				// Upper left vertex buffer
				m_aRootNode[2] = new CNKTerQuadNode( NULL, m_nQuadTreeDepth, 0, nCenterZ, nCenterX, nRightZ,
														pL1TexID, pL2TexID, pL3TexID );

				// Upper right vertex buffer
				m_aRootNode[3] = new CNKTerQuadNode( NULL, m_nQuadTreeDepth, nCenterX, nCenterZ, nRightX, nRightZ,
														pL1TexID, pL2TexID, pL3TexID );
			}
		}
		else
		{
			// Lower right vertex buffer
			m_aRootNode[1] = new CNKTerQuadNode( NULL, m_nQuadTreeDepth, nCenterX, 0, nRightX, nCenterZ );

			// Upper left vertex buffer
			m_aRootNode[2] = new CNKTerQuadNode( NULL, m_nQuadTreeDepth, 0, nCenterZ, nCenterX, nRightZ );

			// Upper right vertex buffer
			m_aRootNode[3] = new CNKTerQuadNode( NULL, m_nQuadTreeDepth, nCenterX, nCenterZ, nRightX, nRightZ );
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : InitVertexBuffer()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKTerrain::InitVertexBuffer()
{
	if( m_szTerName == NULL )
		return true;

	int nWidth = 1 << m_nQuadTreeDepth;
	int nRightX = nWidth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;

	D3DVERTEXBUFFERDESC	VBDesc;
	VBDesc.dwSize = sizeof( VBDesc );
	if( g_pMyNKApp->IsLoadedGameMap() )
		VBDesc.dwCaps = D3DVBCAPS_WRITEONLY;
	else
		VBDesc.dwCaps = D3DVBCAPS_SYSTEMMEMORY;
	VBDesc.dwFVF = NKVERTEXT2::FVF;
	VBDesc.dwNumVertices = m_nVBWidth * m_nVBWidth;

	NKVERTEXT2*	pVB = new NKVERTEXT2[m_nVBWidth * m_nVBWidth];

	CNKTerQuadNode::ms_fdt = 1.0f / (float)(nWidth * 2);
	
	// Lower left vertex buffer
	CNKTerQuadNode::ms_nVBIdxCnt = 0;
	m_aRootNode[0]->InitVertexBuffer( 0, 0, nRightX, nRightZ, 0, 0, pVB );
	if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[0], 0 ) ) )
	{
		SAFE_DELETE_ARRAY( pVB );
		return false;
	}

	NKVERTEXT2*	pVert;
	if( FAILED( m_aVB[0]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
								(LPVOID*)(&pVert), NULL ) ) )
	{
		SAFE_DELETE_ARRAY( pVB );
		return false;
	}
	memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
	m_aVB[0]->Unlock();

	nCenterX = nRightX;		nCenterZ = nRightZ;
	nRightX <<= 1;			nRightZ <<= 1;

	if( m_nHMWidth == 257 )
	{
		// Lower right vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[1]->InitVertexBuffer( nCenterX, 0, nRightX, nCenterZ, -1, 0, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[1], 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return false;
		}

		if( FAILED( m_aVB[1]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return false;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[1]->Unlock();

		// Upper left vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[2]->InitVertexBuffer( 0, nCenterZ, nCenterX, nRightZ, 0, -1, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[2], 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return false;
		}

		if( FAILED( m_aVB[2]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return false;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[2]->Unlock();

		// Upper right vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[3]->InitVertexBuffer( nCenterX, nCenterZ, nRightX, nRightZ, -1, -1, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[3], 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return false;
		}

		if( FAILED( m_aVB[3]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return false;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[3]->Unlock();
	}

	SAFE_DELETE_ARRAY( pVB );

	return true;
}


//-----------------------------------------------------------------------------
// Name : Render()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::Render()
{
	m_dwTriangles = 0;

	// Get texture stage state
	DWORD	dwTexCoord0, dwTexCoord1;
	DWORD	dwColorOp0, dwColorArg01, dwColorArg02, dwAlphaOp0, dwAlphaArg01;
	DWORD	dwColorOp1, dwColorArg11, dwColorArg12, dwAlphaOp1, dwAlphaArg11, dwAddress1;
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, &dwTexCoord0 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, &dwTexCoord1 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &dwColorOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColorArg01 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG2, &dwColorArg02 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLOROP, &dwColorOp1 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLORARG1, &dwColorArg11 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLORARG2, &dwColorArg12 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwAlphaOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg01 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_ALPHAOP, &dwAlphaOp1 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_ALPHAARG1, &dwAlphaArg11 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_ADDRESS, &dwAddress1 );

	// Get render state ( alpha blending )
	DWORD	dwSrcBlend, dwDestBlend;
	m_pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcBlend );
	m_pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestBlend );

	// Set texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP );

	// Set render state ( alpha blending )
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// Lower left
	// Clear index buffer count
	int i = 0;
	for( i = 0; i < m_vecTerTex.size(); i++ )
	{
		m_vecIBL1[i]->dwCount = 0;
		m_vecIBL2[i]->dwCount = 0;
		m_vecIBL3[i]->dwCount = 0;
	}
	if( g_pMyNKApp->IsLoadedGameMap() )
	{
		m_vecIBL1[m_vecTerTex.size()]->dwCount = 0;
		m_vecIBL2[m_vecTerTex.size()]->dwCount = 0;
		m_vecIBL3[m_vecTerTex.size()]->dwCount = 0;
	}
	else
	{
		m_vecIBL1[TEXTURE_MAX_COUNT]->dwCount = 0;
		m_vecIBL2[TEXTURE_MAX_COUNT]->dwCount = 0;
		m_vecIBL3[TEXTURE_MAX_COUNT]->dwCount = 0;
	}

	// Collect triangles rendered
	m_aRootNode[0]->Render( VFCULL_OVERLAP );

	// Set texture ( stage 1 )
	m_pd3dDevice->SetTexture( 1, m_aClrMapTexL2[0] );

	if( m_nViewLayers & 1 )
	{
		// Set texture stage state for non-blending
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		
		// Render triangles
		for( i = 0; i < m_vecTerTex.size(); i++ )
		{
			if( m_vecIBL1[i]->dwCount > 0 )
			{			
				m_pd3dDevice->SetTexture( 0, m_vecTerTex[i]->GetSurfaceByIdx( 0 ) );
				m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[0], 0, m_dwVBSize,
							m_vecIBL1[i]->pIB, m_vecIBL1[i]->dwCount, 0 );
				m_dwTriangles += m_vecIBL1[i]->dwCount/3;
			}
		}
	}

	// Set texture stage state for blending
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	// Set render state ( enable alpha blending )
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

	if( m_nViewLayers & 2 )
	{
		// Render triangles
		for( i = 0; i < m_vecTerTex.size(); i++ )
		{
			if( m_vecIBL2[i]->dwCount > 0 )
			{
				m_pd3dDevice->SetTexture( 0, m_vecTerTex[i]->GetSurfaceByIdx( 0 ) );				
				m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[0], 0, m_dwVBSize,
							m_vecIBL2[i]->pIB, m_vecIBL2[i]->dwCount, 0 );
				m_dwTriangles += m_vecIBL2[i]->dwCount/3;
			}
		}	
	}

	if( m_nViewLayers & 4 )
	{
		// Set texture ( stage 1 )
		m_pd3dDevice->SetTexture( 1, m_aClrMapTexL3[0] );		

		// Render triangles
		for( i = 0; i < m_vecTerTex.size(); i++ )
		{
			if( m_vecIBL3[i]->dwCount > 0 )
			{
				m_pd3dDevice->SetTexture( 0, m_vecTerTex[i]->GetSurfaceByIdx( 0 ) );				
				m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[0], 0, m_dwVBSize,
							m_vecIBL3[i]->pIB, m_vecIBL3[i]->dwCount, 0 );
				m_dwTriangles += m_vecIBL3[i]->dwCount/3;
			}
		}	
	}

	// Set render state ( disable alpha blending )
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

	// Lower right, upper left, upper right
	if( m_nHMWidth == 257 )
	{
		for( int nIndex = 1; nIndex < 4; nIndex++ )
		{
			// Clear index buffer count
			for( i = 0; i < m_vecTerTex.size(); i++ )
			{
				m_vecIBL1[i]->dwCount = 0;
				m_vecIBL2[i]->dwCount = 0;
				m_vecIBL3[i]->dwCount = 0;
			}
			if( g_pMyNKApp->IsLoadedGameMap() )
			{
				m_vecIBL1[m_vecTerTex.size()]->dwCount = 0;
				m_vecIBL2[m_vecTerTex.size()]->dwCount = 0;
				m_vecIBL3[m_vecTerTex.size()]->dwCount = 0;
			}
			else
			{
				m_vecIBL1[TEXTURE_MAX_COUNT]->dwCount = 0;
				m_vecIBL2[TEXTURE_MAX_COUNT]->dwCount = 0;
				m_vecIBL3[TEXTURE_MAX_COUNT]->dwCount = 0;
			}

			// Collect triangles rendered
			m_aRootNode[nIndex]->Render( VFCULL_OVERLAP );

			// Set texture ( stage 1 )
			m_pd3dDevice->SetTexture( 1, m_aClrMapTexL2[nIndex] );

			if( m_nViewLayers & 1 )
			{
				// Set texture stage state for non-blending
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
				m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
				
				// Render triangles
				for( i = 0; i < m_vecTerTex.size(); i++ )
				{
					if( m_vecIBL1[i]->dwCount > 0 )
					{			
						m_pd3dDevice->SetTexture( 0, m_vecTerTex[i]->GetSurfaceByIdx( 0 ) );
						m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[nIndex], 0, m_dwVBSize,
									m_vecIBL1[i]->pIB, m_vecIBL1[i]->dwCount, 0 );
						m_dwTriangles += m_vecIBL1[i]->dwCount/3;
					}
				}
			}

			// Set texture stage state for blending
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );

			// Set render state ( enable alpha blending )
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

			if( m_nViewLayers & 2 )
			{
				// Render triangles
				for( i = 0; i < m_vecTerTex.size(); i++ )
				{
					if( m_vecIBL2[i]->dwCount > 0 )
					{
						m_pd3dDevice->SetTexture( 0, m_vecTerTex[i]->GetSurfaceByIdx( 0 ) );				
						m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[nIndex], 0, m_dwVBSize,
									m_vecIBL2[i]->pIB, m_vecIBL2[i]->dwCount, 0 );
						m_dwTriangles += m_vecIBL2[i]->dwCount/3;
					}
				}	
			}

			if( m_nViewLayers & 4 )
			{
				// Set texture ( stage 1 )
				m_pd3dDevice->SetTexture( 1, m_aClrMapTexL3[nIndex] );

				// Render triangles
				for( i = 0; i < m_vecTerTex.size(); i++ )
				{
					if( m_vecIBL3[i]->dwCount > 0 )
					{
						m_pd3dDevice->SetTexture( 0, m_vecTerTex[i]->GetSurfaceByIdx( 0 ) );				
						m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[nIndex], 0, m_dwVBSize,
									m_vecIBL3[i]->pIB, m_vecIBL3[i]->dwCount, 0 );
						m_dwTriangles += m_vecIBL3[i]->dwCount/3;
					}
				}	
			}

			// Set render state ( disable alpha blending )
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
		}
	}

	// Restore render state
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcBlend );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestBlend );

	// Restore texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, dwTexCoord0 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, dwTexCoord1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, dwColorOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColorArg01 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, dwColorArg02 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, dwColorOp1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, dwColorArg11 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, dwColorArg12 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwAlphaOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwAlphaArg01 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, dwAlphaOp1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, dwAlphaArg11 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESS, dwAddress1 );
}


//-----------------------------------------------------------------------------
// Name : RenderGrid()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RenderGrid( int nGridSize, DWORD dwColor )
{
	// Get texture stage state
	DWORD	dwColorOp0, dwColorArg1, dwColorOp1;
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &dwColorOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColorArg1 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLOROP, &dwColorOp1 );

	// Set texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	
	// Render grids
	int nRightX = 1 << m_nQuadTreeDepth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;

	if( m_nViewLayers == 1 || m_nViewLayers == 2 || m_nViewLayers == 4 )
	{
		dwColor = 0x00dd00dd;

		// Lower left
		m_aRootNode[0]->RenderLayerGrid( m_nQuadTreeDepth, dwColor, m_nViewLayers,
											0, 0, nRightX, nRightZ, VFCULL_OVERLAP );

		nCenterX = nRightX;		nCenterZ = nRightZ;
		nRightX <<= 1;			nRightZ <<= 1;

		if( m_nHMWidth == 257 )
		{
			// Lower right
			m_aRootNode[1]->RenderLayerGrid( m_nQuadTreeDepth, dwColor, m_nViewLayers,
												nCenterX, 0, nRightX, nCenterZ, VFCULL_OVERLAP );

			// Upper left
			m_aRootNode[2]->RenderLayerGrid( m_nQuadTreeDepth, dwColor, m_nViewLayers,
												0, nCenterZ, nCenterX, nRightZ, VFCULL_OVERLAP );

			// Upper right
			m_aRootNode[3]->RenderLayerGrid( m_nQuadTreeDepth, dwColor, m_nViewLayers,
												nCenterX, nCenterZ, nRightX, nRightZ, VFCULL_OVERLAP );
		}
	}
	else
	{
		// Lower left
		m_aRootNode[0]->RenderGrid( m_nQuadTreeDepth - nGridSize, dwColor, 0, 0,
									nRightX, nRightZ, VFCULL_OVERLAP );

		nCenterX = nRightX;		nCenterZ = nRightZ;
		nRightX <<= 1;			nRightZ <<= 1;

		if( m_nHMWidth == 257 )
		{
			// Lower right
			m_aRootNode[1]->RenderGrid( m_nQuadTreeDepth - nGridSize, dwColor, nCenterX, 0,
										nRightX, nCenterZ, VFCULL_OVERLAP );

			// Upper left
			m_aRootNode[2]->RenderGrid( m_nQuadTreeDepth - nGridSize, dwColor, 0, nCenterZ,
										nCenterX, nRightZ, VFCULL_OVERLAP );

			// Upper right
			m_aRootNode[3]->RenderGrid( m_nQuadTreeDepth - nGridSize, dwColor, nCenterX, nCenterZ,
										nRightX, nRightZ, VFCULL_OVERLAP );
		}
	}

	// Restore texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, dwColorOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, dwColorOp1 );
}


//-----------------------------------------------------------------------------
// Name : AddTexture()
// Desc : return 0 - over 15 textures
//        return 1 - fail to create a texture
//        return 2 - a same texture exists
//        return 3 - ok
//-----------------------------------------------------------------------------
int CNKTerrain::AddTexture( const TCHAR* szFilename )
{
	// Search if a texture exists
	int i = 0;
	for( i = 0; i < m_vecTerTex.size(); i++ )
	{
		if( _tcscmp( m_vecTerTex[i]->m_TextureList[0]->m_strName, szFilename ) == 0 )
			break;
	}

	// The max count of a texture is 15
	if( i == TEXTURE_MAX_COUNT )
	{
		MessageBox( NULL, _T("지원되는 텍스쳐는 최대 50장입니다."), _T("NKMapTool"), MB_OK );
		return 0;
	}


	if( i == m_vecTerTex.size() )
	{
		// Create a texture
		TCHAR	szName[128];
		TCHAR	szPath[128];
		_tcscpy( szName, szFilename );
		_tcscpy( szPath, _T("map\\") );
		_tcscat( szPath, m_szTerName );
		_tcscat( szPath, _T("\\") );
		szPath[_tcslen( m_szTerName ) + _tcslen( _T("map\\\\") )] = NULL;
		IndexedTexture*	pTexture = new IndexedTexture( 1 );
		pTexture->SetTexturePath( szPath );
		if( pTexture->CreateTextureFromFile( szName ) < 0 )
		{
			// Fail to create a texture
			TCHAR	szTemp[512];
			_tcscpy( szTemp, _T("'") );
			_tcscat( szTemp, szFilename );
			_tcscat( szTemp, _T("' ") );
			_tcscat( szTemp, _T("텍스쳐 생성에 실패했습니다.") );
			MessageBox( NULL, szTemp, _T("NKMapTool"), MB_OK );
			SAFE_DELETE( pTexture );
			return 1;
		}
		else
		{
			// Add a texture
			pTexture->RestoreAllTextures( m_pd3dDevice );
			m_vecTerTex.push_back( pTexture );
			
			// Add a index buffer
			int		nIndex = m_vecTerTex.size() - 1;
			TerIB*	pIB;
			pIB = new TerIB;
			m_vecIBL1[nIndex] = pIB;
			pIB->pIB = new WORD[128*128*6];
			pIB = new TerIB;
			m_vecIBL2[nIndex] = pIB;
			pIB->pIB = new WORD[128*128*6];
			pIB = new TerIB;
			m_vecIBL3[nIndex] = pIB;
			pIB->pIB = new WORD[128*128*6];
		}
	}
	else
	{
		TCHAR	szTemp[512];
		_tcscpy( szTemp, _T("'") );
		_tcscat( szTemp, szFilename );
		_tcscat( szTemp, _T("' ") );
		_tcscat( szTemp, _T("텍스쳐는 이미 등록되어 있습니다.") );
		MessageBox( NULL, szTemp, _T("NKMapTool"), MB_OK );
		return 2;
	}

	return 3;
}


//-----------------------------------------------------------------------------
// Name : ReplaceTexture()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::ReplaceTexture( int nIndex, const TCHAR* szFilename )
{
	if( nIndex < 0 )
		return false;

	// Search if a texture exists
	int i = 0;
	for( i = 0; i < m_vecTerTex.size(); i++ )
	{
		if( _tcscmp( m_vecTerTex[i]->m_TextureList[0]->m_strName, szFilename ) == 0 )
			break;
	}

	if( i == m_vecTerTex.size() )
	{
		// Create a texture
		TCHAR	szName[128];
		TCHAR	szPath[128];
		_tcscpy( szName, szFilename );
		_tcscpy( szPath, _T("map\\") );
		_tcscat( szPath, m_szTerName );
		_tcscat( szPath, _T("\\") );
		szPath[_tcslen( m_szTerName ) + _tcslen( _T("map\\\\") )] = NULL;
		IndexedTexture*	pTexture = new IndexedTexture( 1 );
		pTexture->SetTexturePath( szPath );
		if( pTexture->CreateTextureFromFile( szName ) < 0 )
		{
			// Fail to create a texture
			TCHAR	szTemp[512];
			_tcscpy( szTemp, _T("'") );
			_tcscat( szTemp, szFilename );
			_tcscat( szTemp, _T("' ") );
			_tcscat( szTemp, _T("텍스쳐 생성에 실패했습니다.") );
			MessageBox( NULL, szTemp, _T("NKMapTool"), MB_OK );
			SAFE_DELETE( pTexture );
			return false;
		}
		else
		{
			// Destroy a texture
			m_vecTerTex[nIndex]->InvalidateAllTextures();
			m_vecTerTex[nIndex]->DestroyTextureAll();
			SAFE_DELETE( m_vecTerTex[nIndex] );

			// Add a texture
			pTexture->RestoreAllTextures( m_pd3dDevice );
			m_vecTerTex[nIndex] = pTexture;
		}
	}
	else
	{
		TCHAR	szTemp[512];
		_tcscpy( szTemp, _T("'") );
		_tcscat( szTemp, szFilename );
		_tcscat( szTemp, _T("' ") );
		_tcscat( szTemp, _T("와 같은 이름의 텍스쳐가\n이미 등록되어 있습니다.") );
		MessageBox( NULL, szTemp, _T("NKMapTool"), MB_OK );
		
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : DeleteTexture()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::DeleteTexture( int nTexID )
{
	// Check if a texture to delete is used
	int		nX, nZ;
	bool	bExistence = false;

	for( nZ = 0; nZ < m_nHMWidth - 1; nZ++ )
	{
		for( nX = 0; nX < m_nHMWidth - 1; nX++ )
		{
			if( m_aQuadNodes[nZ][nX]->GetL1TexID() == nTexID ||
				m_aQuadNodes[nZ][nX]->GetL2TexID() == nTexID ||
				m_aQuadNodes[nZ][nX]->GetL3TexID() == nTexID )
			{
				bExistence = true;
				break;
			}
		}
	}

	// If a texture to delete is used
	if( bExistence )
	{
		if( IDNO == MessageBox( NULL, _T("삭제하고자 하는 텍스쳐가 사용중입니다.\n")
								_T("이 텍스쳐를 사용하는 타일은 그려지지 않습니다.\n\n")
								_T("정말로 삭제하시겠습니까?"),
								_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) )
			return false;
	}

	// If a tile uses a texture to delete, set a texture ID of the tile to 15 ( Non-texture )
	int	nID;
	for( nZ = 0; nZ < m_nHMWidth - 1; nZ++ )
	{
		for( nX = 0; nX < m_nHMWidth - 1; nX++ )
		{
			nID = m_aQuadNodes[nZ][nX]->GetL1TexID();
			if( nID == nTexID )
				m_aQuadNodes[nZ][nX]->SetL1TexID( TEXTURE_MAX_COUNT );
			else if( nID > nTexID && nID != TEXTURE_MAX_COUNT )
				m_aQuadNodes[nZ][nX]->SetL1TexID( nID - 1 );

			nID = m_aQuadNodes[nZ][nX]->GetL2TexID();
			if( nID == nTexID )
				m_aQuadNodes[nZ][nX]->SetL2TexID( TEXTURE_MAX_COUNT );
			else if( nID > nTexID && nID != TEXTURE_MAX_COUNT )
				m_aQuadNodes[nZ][nX]->SetL2TexID( nID - 1 );

			nID = m_aQuadNodes[nZ][nX]->GetL3TexID();
			if( nID == nTexID )
				m_aQuadNodes[nZ][nX]->SetL3TexID( TEXTURE_MAX_COUNT );
			else if( nID > nTexID && nID != TEXTURE_MAX_COUNT )
				m_aQuadNodes[nZ][nX]->SetL3TexID( nID - 1 );							
		}
	}

	// Delete a terrain texture
	vector<IndexedTexture*>::iterator iterTex = find( m_vecTerTex.begin(), m_vecTerTex.end(),
														m_vecTerTex[nTexID] );
	SAFE_DELETE( m_vecTerTex[nTexID] );
	m_vecTerTex.erase( iterTex );

	// Delete a index buffer
	SAFE_DELETE( m_vecIBL1[nTexID] );
	SAFE_DELETE( m_vecIBL2[nTexID] );
	SAFE_DELETE( m_vecIBL3[nTexID] );
	for( int i = nTexID; i < TEXTURE_MAX_COUNT-1; i++ )
	{
		m_vecIBL1[i] = m_vecIBL1[i+1];
		m_vecIBL2[i] = m_vecIBL2[i+1];
		m_vecIBL3[i] = m_vecIBL3[i+1];
	}
	m_vecIBL1[TEXTURE_MAX_COUNT-1] = NULL;
	m_vecIBL2[TEXTURE_MAX_COUNT-1] = NULL;
	m_vecIBL3[TEXTURE_MAX_COUNT-1] = NULL;

	return true;
}


//-----------------------------------------------------------------------------
// Name : GetPickedTile()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::GetPickedTile( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir, int& rZ, int& rX )
{
	float	fDist = FLT_MAX;
	rX = rZ = -1;

	m_aRootNode[0]->GetPickedPoint( rOrigin, rDir, fDist );

	if( m_nHMWidth == 257 )
	{
		for( int nIndex = 1; nIndex < 4; nIndex++ )
			m_aRootNode[nIndex]->GetPickedPoint( rOrigin, rDir, fDist );
	}
	
	if( fDist != FLT_MAX )
	{
		D3DXVECTOR3	vPoint;
		vPoint = rDir * fDist;
		vPoint += rOrigin;
		rX = vPoint.x / m_nPatchSize;
		rZ = vPoint.z / m_nPatchSize;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name : GetPickedPoint()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::GetPickedPoint( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir, D3DXVECTOR3& rPoint )
{
	float	fDist = FLT_MAX;

	m_aRootNode[0]->GetPickedPoint( rOrigin, rDir, fDist );

	if( m_nHMWidth == 257 )
	{
		for( int nIndex = 1; nIndex < 4; nIndex++ )
			m_aRootNode[nIndex]->GetPickedPoint( rOrigin, rDir, fDist );
	}
	
	if( fDist != FLT_MAX )
	{
		rPoint = rDir * fDist;
		rPoint += rOrigin;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name : GetHeights()
// Desc :
//-----------------------------------------------------------------------------
float CNKTerrain::GetHeights( float fZ, float fX ) const
{
	float	fMaxWidth = ( m_nHMWidth - 1 ) * m_nPatchSize;
	
	if( fX < 0.0f ) fX = 0.0f;
	else if( fX > fMaxWidth ) fX = fMaxWidth;
	if( fZ < 0.0f ) fZ = 0.0f;
	else if( fZ > fMaxWidth ) fZ = fMaxWidth;

	int		nX = (int)( fX / m_nPatchSize );
	int		nZ = (int)( fZ / m_nPatchSize );
	float	fRemainedX = fX - (float)( nX * m_nPatchSize );
	float	fRemainedZ = fZ - (float)( nZ * m_nPatchSize );

	D3DXPLANE	plTemp;
	D3DXVECTOR3	vP0;
	D3DXVECTOR3	vP1 = D3DXVECTOR3( (nX+1)*m_nPatchSize,
									m_pHeights[nX+1 + (nZ+1)*m_nHMWidth] * m_nHeightScale, (nZ+1)*m_nPatchSize );
	D3DXVECTOR3	vP2 = D3DXVECTOR3( nX*m_nPatchSize,
									m_pHeights[nX + nZ*m_nHMWidth] * m_nHeightScale, nZ*m_nPatchSize );
	
	if( fRemainedX < fRemainedZ )		// Upper left triangle
	{
		vP0 = D3DXVECTOR3( nX*m_nPatchSize,
							m_pHeights[nX + (nZ+1)*m_nHMWidth] * m_nHeightScale, (nZ+1)*m_nPatchSize );
	}
	else								// Lower right triangle
	{
		vP0 = D3DXVECTOR3( (nX+1)*m_nPatchSize,
							m_pHeights[nX+1 + nZ*m_nHMWidth] * m_nHeightScale, nZ*m_nPatchSize );
	}

	D3DXPlaneFromPoints( &plTemp, &vP0, &vP1, &vP2 );

	return -( plTemp.a*fX + plTemp.c*fZ + plTemp.d ) / plTemp.b;
}


//-----------------------------------------------------------------------------
// Name : DrawNonblendTile()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::DrawNonblendTile( int nSZ, int nSX, int nEZ, int nEX, int nIndex )
{
	int nX, nZ;

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			m_aQuadNodes[nZ][nX]->SetL1TexID( nIndex );
		}
	}	
}


//-----------------------------------------------------------------------------
// Name : DrawBlendTilePencil()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::DrawBlendTilePencil( int nSZ, int nSX, int nEZ, int nEX, int nLayer,
										int nIndex, int nAlpha, bool bPaint )
{
	int nX, nZ;
	int	nCX, nCZ;

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			nCX = nX << 1;		nCZ = nZ << 1;
			
			if( !bPaint ) nAlpha = 0;

			if( nLayer == 2 )
			{
				m_aAlphaMapL2[nCZ][nCX] = nAlpha;		m_aAlphaMapL2[nCZ][nCX+1] = nAlpha;
				m_aAlphaMapL2[nCZ+1][nCX] = nAlpha;		m_aAlphaMapL2[nCZ+1][nCX+1] = nAlpha;
				m_aQuadNodes[nZ][nX]->SetL2TexID( nIndex );
			}
			else
			{
				m_aAlphaMapL3[nCZ][nCX] = nAlpha;		m_aAlphaMapL3[nCZ][nCX+1] = nAlpha;
				m_aAlphaMapL3[nCZ+1][nCX] = nAlpha;		m_aAlphaMapL3[nCZ+1][nCX+1] = nAlpha;
				m_aQuadNodes[nZ][nX]->SetL3TexID( nIndex );
			}				
		}
	}

	if( bPaint )
	{
		nSX = nSX - 1 < 0 ? 0 : nSX - 1;
		nSZ = nSZ - 1 < 0 ? 0 : nSZ - 1;
		nEX = nEX + 1 > m_nHMWidth - 2 ? m_nHMWidth - 2 : nEX + 1;
		nEZ = nEZ + 1 > m_nHMWidth - 2 ? m_nHMWidth - 2 : nEZ + 1;

		for( nX = nSX; nX <= nEX; nX++ )
		{
			if( nLayer == 2 )
			{
				if( m_aQuadNodes[nSZ][nX]->GetL2TexID() != nIndex )
					m_aQuadNodes[nSZ][nX]->SetL2TexID( nIndex );
				if( m_aQuadNodes[nEZ][nX]->GetL2TexID() != nIndex )
					m_aQuadNodes[nEZ][nX]->SetL2TexID( nIndex );
			}
			else
			{
				if( m_aQuadNodes[nSZ][nX]->GetL3TexID() != nIndex )
					m_aQuadNodes[nSZ][nX]->SetL3TexID( nIndex );
				if( m_aQuadNodes[nEZ][nX]->GetL3TexID() != nIndex )
					m_aQuadNodes[nEZ][nX]->SetL3TexID( nIndex );
			}
		}

		for( nZ = nSZ; nZ <= nEZ; nZ++ )
		{
			if( nLayer == 2 )
			{
				if( m_aQuadNodes[nZ][nSX]->GetL2TexID() != nIndex )
					m_aQuadNodes[nZ][nSX]->SetL2TexID( nIndex );
				if( m_aQuadNodes[nZ][nEX]->GetL2TexID() != nIndex )
					m_aQuadNodes[nZ][nEX]->SetL2TexID( nIndex );
			}
			else
			{
				if( m_aQuadNodes[nZ][nSX]->GetL3TexID() != nIndex )
					m_aQuadNodes[nZ][nSX]->SetL3TexID( nIndex );
				if( m_aQuadNodes[nZ][nEX]->GetL3TexID() != nIndex )
					m_aQuadNodes[nZ][nEX]->SetL3TexID( nIndex );
			}
		}
	}
	else
	{
		int		x, z, sx, sz, ex, ez;
		bool	bOptimize;

		for( nZ = nSZ; nZ <= nEZ; nZ++ )
		{
			for( nX = nSX; nX <= nEX; nX++ )
			{
				nCX = nX << 1;		nCZ = nZ << 1;

				sx = nCX-1 < 0 ? 0 : nCX-1;
				ex = nCX+2 >= (m_nHMWidth-1)*2 ? (m_nHMWidth-1)*2-1 : nCX+2;
				sz = nCZ-1 < 0 ? 0 : nCZ-1;
				ez = nCZ+2 >= (m_nHMWidth-1)*2 ? (m_nHMWidth-1)*2-1 : nCZ+2;

				bOptimize = true;
				if( nLayer == 2 )
				{
					for( z = sz; z <= ez; z++ )
					{
						for( x = sx; x <= ex; x++ )
						{
							if( m_aAlphaMapL2[z][x] > 0 )
							{
								bOptimize = false;
								break;
							}
						}
					}
					if( bOptimize )
						m_aQuadNodes[nZ][nX]->SetL2TexID( TEXTURE_MAX_COUNT );
				}
				else
				{
					for( z = sz; z <= ez; z++ )
					{
						for( x = sx; x <= ex; x++ )
						{
							if( m_aAlphaMapL3[z][x] > 0 )
							{
								bOptimize = false;
								break;
							}
						}
					}
					if( bOptimize )
						m_aQuadNodes[nZ][nX]->SetL3TexID( TEXTURE_MAX_COUNT );
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name : DrawBlendTileBrush()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::DrawBlendTileBrush( int nZ, int nX, int nLayer, int nIndex, float aAlpha[], bool bPaint )
{
	// Set a alpha value of a color map
	int		nCX = nX << 1;
	int		nCZ = nZ << 1;
	
	if( nLayer == 2 )	// Layer 2
	{
		m_aAlphaL2[nCZ][nCX] += aAlpha[0];
		if( m_aAlphaL2[nCZ][nCX] < 0 ) m_aAlphaL2[nCZ][nCX] = 0;
		else if( m_aAlphaL2[nCZ][nCX] > 255 ) m_aAlphaL2[nCZ][nCX] = 255;
		m_aAlphaMapL2[nCZ][nCX] = m_aAlphaL2[nCZ][nCX];

		m_aAlphaL2[nCZ][nCX+1] += aAlpha[1];
		if( m_aAlphaL2[nCZ][nCX+1] < 0 ) m_aAlphaL2[nCZ][nCX+1] = 0;
		else if( m_aAlphaL2[nCZ][nCX+1] > 255 ) m_aAlphaL2[nCZ][nCX+1] = 255;
		m_aAlphaMapL2[nCZ][nCX+1] = m_aAlphaL2[nCZ][nCX+1];

		m_aAlphaL2[nCZ+1][nCX] += aAlpha[2];
		if( m_aAlphaL2[nCZ+1][nCX] < 0 ) m_aAlphaL2[nCZ+1][nCX] = 0;
		else if( m_aAlphaL2[nCZ+1][nCX] > 255 ) m_aAlphaL2[nCZ+1][nCX] = 255;
		m_aAlphaMapL2[nCZ+1][nCX] = m_aAlphaL2[nCZ+1][nCX];

		m_aAlphaL2[nCZ+1][nCX+1] += aAlpha[3];
		if( m_aAlphaL2[nCZ+1][nCX+1] < 0 ) m_aAlphaL2[nCZ+1][nCX+1] = 0;
		else if( m_aAlphaL2[nCZ+1][nCX+1] > 255 ) m_aAlphaL2[nCZ+1][nCX+1] = 255;
		m_aAlphaMapL2[nCZ+1][nCX+1] = m_aAlphaL2[nCZ+1][nCX+1];
	}
	else				// Layer 3
	{
		m_aAlphaL3[nCZ][nCX] += aAlpha[0];
		if( m_aAlphaL3[nCZ][nCX] < 0 ) m_aAlphaL3[nCZ][nCX] = 0;
		else if( m_aAlphaL3[nCZ][nCX] > 255 ) m_aAlphaL3[nCZ][nCX] = 255;
		m_aAlphaMapL3[nCZ][nCX] = m_aAlphaL3[nCZ][nCX];

		m_aAlphaL3[nCZ][nCX+1] += aAlpha[1];
		if( m_aAlphaL3[nCZ][nCX+1] < 0 ) m_aAlphaL3[nCZ][nCX+1] = 0;
		else if( m_aAlphaL3[nCZ][nCX+1] > 255 ) m_aAlphaL3[nCZ][nCX+1] = 255;
		m_aAlphaMapL3[nCZ][nCX+1] = m_aAlphaL3[nCZ][nCX+1];

		m_aAlphaL3[nCZ+1][nCX] += aAlpha[2];
		if( m_aAlphaL3[nCZ+1][nCX] < 0 ) m_aAlphaL3[nCZ+1][nCX] = 0;
		else if( m_aAlphaL3[nCZ+1][nCX] > 255 ) m_aAlphaL3[nCZ+1][nCX] = 255;
		m_aAlphaMapL3[nCZ+1][nCX] = m_aAlphaL3[nCZ+1][nCX];

		m_aAlphaL3[nCZ+1][nCX+1] += aAlpha[3];
		if( m_aAlphaL3[nCZ+1][nCX+1] < 0 ) m_aAlphaL3[nCZ+1][nCX+1] = 0;
		else if( m_aAlphaL3[nCZ+1][nCX+1] > 255 ) m_aAlphaL3[nCZ+1][nCX+1] = 255;
		m_aAlphaMapL3[nCZ+1][nCX+1] = m_aAlphaL3[nCZ+1][nCX+1];
	}

	if( bPaint )		// Painting mode
	{
		if( nLayer == 2 )	// Layer 2
			m_aQuadNodes[nZ][nX]->SetL2TexID( nIndex );
		else				// Layer 3
			m_aQuadNodes[nZ][nX]->SetL3TexID( nIndex );
	}
	else				// Erasing mode
	{
		int		x, z, nSX, nEX, nSZ, nEZ;
		bool	bOptimize;	

		nSX = nCX-1 < 0 ? 0 : nCX-1;
		nEX = nCX+2 >= (m_nHMWidth-1)*2 ? (m_nHMWidth-1)*2-1 : nCX+2;
		nSZ = nCZ-1 < 0 ? 0 : nCZ-1;
		nEZ	= nCZ+2 >= (m_nHMWidth-1)*2 ? (m_nHMWidth-1)*2-1 : nCZ+2;

		bOptimize = true;
		if( nLayer == 2 )
		{
			for( z = nSZ; z <= nEZ; z++ )
			{
				for( x = nSX; x <= nEX; x++ )
				{
					if( m_aAlphaMapL2[z][x] > 0 )
					{
						bOptimize = false;
						break;
					}
				}
			}
			if( bOptimize )
				m_aQuadNodes[nZ][nX]->SetL2TexID( TEXTURE_MAX_COUNT );
		}
		else
		{
			for( z = nSZ; z <= nEZ; z++ )
			{
				for( x = nSX; x <= nEX; x++ )
				{
					if( m_aAlphaMapL3[z][x] > 0 )
					{
						bOptimize = false;
						break;
					}
				}
			}
			if( bOptimize )
				m_aQuadNodes[nZ][nX]->SetL3TexID( TEXTURE_MAX_COUNT );
		}
	}
}


//-----------------------------------------------------------------------------
// Name : DrawShadowColor()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::DrawShadowColor( int nIndex, int nZ, int nX, float fR, float fG, float fB,
									float aR[4], float aG[4] , float aB[4], bool bPaint )
{
	// Set a shadow map
	int		nCX = nX << 1;
	int		nCZ = nZ << 1;

	if( nIndex < 0 )
		nIndex = m_nTimeID;
	
	if( bPaint )
	{
		m_aShadowR[nIndex][nCZ][nCX] -= aR[0];
		if( m_aShadowR[nIndex][nCZ][nCX] < fR ) m_aShadowR[nIndex][nCZ][nCX] = fR;
		m_aShadowG[nIndex][nCZ][nCX] -= aG[0];
		if( m_aShadowG[nIndex][nCZ][nCX] < fG ) m_aShadowG[nIndex][nCZ][nCX] = fG;
		m_aShadowB[nIndex][nCZ][nCX] -= aB[0];
		if( m_aShadowB[nIndex][nCZ][nCX] < fB ) m_aShadowB[nIndex][nCZ][nCX] = fB;
		
		m_aShadowR[nIndex][nCZ][nCX+1] -= aR[1];
		if( m_aShadowR[nIndex][nCZ][nCX+1] < fR ) m_aShadowR[nIndex][nCZ][nCX+1] = fR;
		m_aShadowG[nIndex][nCZ][nCX+1] -= aG[1];
		if( m_aShadowG[nIndex][nCZ][nCX+1] < fG ) m_aShadowG[nIndex][nCZ][nCX+1] = fG;
		m_aShadowB[nIndex][nCZ][nCX+1] -= aB[1];
		if( m_aShadowB[nIndex][nCZ][nCX+1] < fB ) m_aShadowB[nIndex][nCZ][nCX+1] = fB;

		m_aShadowR[nIndex][nCZ+1][nCX] -= aR[2];
		if( m_aShadowR[nIndex][nCZ+1][nCX] < fR ) m_aShadowR[nIndex][nCZ+1][nCX] = fR;
		m_aShadowG[nIndex][nCZ+1][nCX] -= aG[2];
		if( m_aShadowG[nIndex][nCZ+1][nCX] < fG ) m_aShadowG[nIndex][nCZ+1][nCX] = fG;
		m_aShadowB[nIndex][nCZ+1][nCX] -= aB[2];
		if( m_aShadowB[nIndex][nCZ+1][nCX] < fB ) m_aShadowB[nIndex][nCZ+1][nCX] = fB;

		m_aShadowR[nIndex][nCZ+1][nCX+1] -= aR[3];
		if( m_aShadowR[nIndex][nCZ+1][nCX+1] < fR ) m_aShadowR[nIndex][nCZ+1][nCX+1] = fR;
		m_aShadowG[nIndex][nCZ+1][nCX+1] -= aG[3];
		if( m_aShadowG[nIndex][nCZ+1][nCX+1] < fG ) m_aShadowG[nIndex][nCZ+1][nCX+1] = fG;
		m_aShadowB[nIndex][nCZ+1][nCX+1] -= aB[3];
		if( m_aShadowB[nIndex][nCZ+1][nCX+1] < fB ) m_aShadowB[nIndex][nCZ+1][nCX+1] = fB;
	}
	else
	{
		m_aShadowR[nIndex][nCZ][nCX] += aR[0];
		if( m_aShadowR[nIndex][nCZ][nCX] > 1.0f ) m_aShadowR[nIndex][nCZ][nCX] = 1.0f;
		m_aShadowG[nIndex][nCZ][nCX] += aG[0];
		if( m_aShadowG[nIndex][nCZ][nCX] > 1.0f ) m_aShadowG[nIndex][nCZ][nCX] = 1.0f;
		m_aShadowB[nIndex][nCZ][nCX] += aB[0];
		if( m_aShadowB[nIndex][nCZ][nCX] > 1.0f ) m_aShadowB[nIndex][nCZ][nCX] = 1.0f;
		
		m_aShadowR[nIndex][nCZ][nCX+1] += aR[1];
		if( m_aShadowR[nIndex][nCZ][nCX+1] > 1.0f ) m_aShadowR[nIndex][nCZ][nCX+1] = 1.0f;
		m_aShadowG[nIndex][nCZ][nCX+1] += aG[1];
		if( m_aShadowG[nIndex][nCZ][nCX+1] > 1.0f ) m_aShadowG[nIndex][nCZ][nCX+1] = 1.0f;
		m_aShadowB[nIndex][nCZ][nCX+1] += aB[1];
		if( m_aShadowB[nIndex][nCZ][nCX+1] > 1.0f ) m_aShadowB[nIndex][nCZ][nCX+1] = 1.0f;

		m_aShadowR[nIndex][nCZ+1][nCX] += aR[2];
		if( m_aShadowR[nIndex][nCZ+1][nCX] > 1.0f ) m_aShadowR[nIndex][nCZ+1][nCX] = 1.0f;
		m_aShadowG[nIndex][nCZ+1][nCX] += aG[2];
		if( m_aShadowG[nIndex][nCZ+1][nCX] > 1.0f ) m_aShadowG[nIndex][nCZ+1][nCX] = 1.0f;
		m_aShadowB[nIndex][nCZ+1][nCX] += aB[2];
		if( m_aShadowB[nIndex][nCZ+1][nCX] > 1.0f ) m_aShadowB[nIndex][nCZ+1][nCX] = 1.0f;

		m_aShadowR[nIndex][nCZ+1][nCX+1] += aR[3];
		if( m_aShadowR[nIndex][nCZ+1][nCX+1] > 1.0f ) m_aShadowR[nIndex][nCZ+1][nCX+1] = 1.0f;
		m_aShadowG[nIndex][nCZ+1][nCX+1] += aG[3];
		if( m_aShadowG[nIndex][nCZ+1][nCX+1] > 1.0f ) m_aShadowG[nIndex][nCZ+1][nCX+1] = 1.0f;
		m_aShadowB[nIndex][nCZ+1][nCX+1] += aB[3];
		if( m_aShadowB[nIndex][nCZ+1][nCX+1] > 1.0f ) m_aShadowB[nIndex][nCZ+1][nCX+1] = 1.0f;
	}
}


static HRESULT CALLBACK TextureSearch( DDPIXELFORMAT* pddpf, VOID* param )
{
	DDSURFACEDESC2* pddsd = (DDSURFACEDESC2*)param;

	// Skip unwanted formats
	if( pddpf->dwRGBBitCount != pddsd->dwFlags )
		return DDENUMRET_OK;
	if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_ALPHAPIXELS) )
		return DDENUMRET_OK;
	if( pddpf->dwFlags & (DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
		return DDENUMRET_OK;
	if( 0 != pddpf->dwFourCC )
		return DDENUMRET_OK;

	pddpf->dwRGBAlphaBitMask = 0xff000000;
	pddpf->dwRBitMask = 0x00ff0000;
	pddpf->dwGBitMask = 0x0000ff00;
	pddpf->dwBBitMask = 0x000000ff;
	pddpf->dwFlags |= DDPF_ALPHAPIXELS;

	memcpy( &pddsd->ddpfPixelFormat, pddpf, sizeof(DDPIXELFORMAT) );
	return DDENUMRET_CANCEL;
}


//-----------------------------------------------------------------------------
// Name : CreateClrMapTexture()
// Desc :
//-----------------------------------------------------------------------------
HRESULT	CNKTerrain::CreateClrMapTexture()
{
	if( m_szTerName == NULL )
		return S_OK;

	// Release any previously created objects
	int i = 0;
	for( i = 0; i < 4; i++ )
	{
		SAFE_RELEASE( m_aClrMapTexL2[i] );
		SAFE_RELEASE( m_aClrMapTexL3[i] );
	}

	// Get the device caps
	D3DDEVICEDESC7 ddDesc;
	if( FAILED( m_pd3dDevice->GetCaps( &ddDesc) ) )
		return E_FAIL;

	// Setup the new surface desc
	int nWidth = 1 << ( m_nQuadTreeDepth + 1 );
	DDSURFACEDESC2 ddsd;
	D3DUtil_InitSurfaceDesc( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	ddsd.dwTextureStage = 1;
	ddsd.dwWidth = nWidth;
	ddsd.dwHeight = nWidth;

	// Turn on texture management for hardware devices
	if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	// Search a texture format
	DDSURFACEDESC2 ddsdSearch;
	ddsdSearch.dwFlags = 32;
	m_pd3dDevice->EnumTextureFormats( TextureSearch, &ddsdSearch );

	if( 32 != ddsdSearch.ddpfPixelFormat.dwRGBBitCount )
		return E_FAIL;

	// Use a searched texture format
	memcpy( &ddsd.ddpfPixelFormat, &ddsdSearch.ddpfPixelFormat, sizeof(DDPIXELFORMAT) );

	// Get the DirectDraw interface for creating surfaces
	LPDIRECTDRAW7        pDD;
	LPDIRECTDRAWSURFACE7 pddsRender;
	m_pd3dDevice->GetRenderTarget( &pddsRender );
	pddsRender->GetDDInterface( (VOID**)&pDD );
	pddsRender->Release();

	HRESULT hr;
	// Create a new surface for the texture
	if( FAILED( hr = pDD->CreateSurface( &ddsd, &m_aClrMapTexL2[0], NULL ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}
	if( FAILED( UpdateClrMapTexture( 2, 0 ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}

	if( FAILED( hr = pDD->CreateSurface( &ddsd, &m_aClrMapTexL3[0], NULL ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}
	if( FAILED( UpdateClrMapTexture( 3, 0 ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}

	// [10/25/2007 Theodoric] 256 이하 크기의 맵에서는 Surface를 만들지 않아 버텍스 편집 시에 문제가 발생한다.
	// 고로 모든 맵에서 다 만들도로 주석을 잡아 놓는다. 문제가 생기면 풀도록하자.
	// if( m_nHMWidth == 257 )
	{
		for( i = 1; i < 4; i++ )
		{
			if( FAILED( hr = pDD->CreateSurface( &ddsd, &m_aClrMapTexL2[i], NULL ) ) )
			{
				pDD->Release();
				return E_FAIL;
			}
			if( FAILED( UpdateClrMapTexture( 2, i ) ) )
			{
				pDD->Release();
				return E_FAIL;
			}

			if( FAILED( hr = pDD->CreateSurface( &ddsd, &m_aClrMapTexL3[i], NULL ) ) )
			{
				pDD->Release();
				return E_FAIL;
			}
			if( FAILED( UpdateClrMapTexture( 3, i ) ) )
			{
				pDD->Release();
				return E_FAIL;
			}
		}
	}

	// Done with DDraw
	pDD->Release();

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : UpdateClrMapTexture()
// Desc :
//-----------------------------------------------------------------------------
HRESULT CNKTerrain::UpdateClrMapTexture( int nLayer, int nIndex )
{
	int	nWidth = 1 << ( m_nQuadTreeDepth + 1 );
	int	nSX = ( nIndex % 2 ) * nWidth;
	int	nSZ = ( nIndex / 2 ) * nWidth;
	int	nCount = 0;
	int	nX, nZ;

	// Get a DDraw object to create a temporary surface
	LPDIRECTDRAW7	pDD;
	if( nLayer == 2 )
		m_aClrMapTexL2[nIndex]->GetDDInterface( (VOID**)&pDD );
	else
		m_aClrMapTexL3[nIndex]->GetDDInterface( (VOID**)&pDD );

	// Setup the new surface desc
	DDSURFACEDESC2	ddsd;
	ddsd.dwSize = sizeof(ddsd);
	if( nLayer == 2 )
		m_aClrMapTexL2[nIndex]->GetSurfaceDesc( &ddsd );
	else
		m_aClrMapTexL3[nIndex]->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
    ddsd.ddsCaps.dwCaps2 = 0L;
	ddsd.dwTextureStage = 1;
    ddsd.dwWidth = nWidth;
    ddsd.dwHeight = nWidth;
	
	// Create a new surface for the texture
	LPDIRECTDRAWSURFACE7	pddsTempSurface;
	HRESULT hr;
	if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface, NULL ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}

	while( pddsTempSurface->Lock( NULL, &ddsd, 0, 0 ) == DDERR_WASSTILLDRAWING );
	BYTE*	pBits = (BYTE*)ddsd.lpSurface;
	if( nLayer == 2 )
	{
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			DWORD*			pTempBits;
			int				nClrMapWidth = ( m_nHMWidth - 1 ) * 2;
			unsigned char*	pTempA;
			unsigned char*	pTempR;
			unsigned char*	pTempG;
			unsigned char*	pTempB;
			
			for( nZ = nSZ; nZ < nSZ + nWidth; nZ++ )
			{
				pTempBits = (DWORD*)pBits;
				pTempA = &m_pAlphaMapL2[nSX + nZ * nClrMapWidth];
				pTempR = &m_pLightMapsR[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempG = &m_pLightMapsG[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempB = &m_pLightMapsB[m_nTimeID][nSX + nZ * nClrMapWidth];
				for( nX = nSX; nX < nSX + nWidth; nX++ )
				{
					*pTempBits = ( *pTempA << 24 ) | ( *pTempR << 16 ) | ( *pTempG << 8 ) | *pTempB;
					pTempA++;	pTempR++;	pTempG++;	pTempB++;	pTempBits++;
				}
				pBits += ddsd.lPitch;
			}
		}
		else
		{
			DWORD*	pTempBits;
			for( nZ = nSZ; nZ < nSZ + nWidth; nZ++ )
			{
				pTempBits = (DWORD*)pBits;
				for( nX = nSX; nX < nSX + nWidth; nX++ )
				{
					*pTempBits = ( m_aAlphaMapL2[nZ][nX] << 24 ) | m_aRGBMap[m_nTimeID][nZ][nX];
					pTempBits++;			
				}
				pBits += ddsd.lPitch;
			}
		}
	}
	else
	{
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			DWORD*			pTempBits;
			int				nClrMapWidth = ( m_nHMWidth - 1 ) * 2;
			unsigned char*	pTempA;
			unsigned char*	pTempR;
			unsigned char*	pTempG;
			unsigned char*	pTempB;
			
			for( nZ = nSZ; nZ < nSZ + nWidth; nZ++ )
			{
				pTempBits = (DWORD*)pBits;
				pTempA = &m_pAlphaMapL3[nSX + nZ * nClrMapWidth];
				pTempR = &m_pLightMapsR[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempG = &m_pLightMapsG[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempB = &m_pLightMapsB[m_nTimeID][nSX + nZ * nClrMapWidth];
				for( nX = nSX; nX < nSX + nWidth; nX++ )
				{
					*pTempBits = ( *pTempA << 24 ) | ( *pTempR << 16 ) | ( *pTempG << 8 ) | *pTempB;
					pTempA++;	pTempR++;	pTempG++;	pTempB++;	pTempBits++;
				}
				pBits += ddsd.lPitch;
			}
		}
		else
		{
			DWORD*	pTempBits;
			for( nZ = nSZ; nZ < nSZ + nWidth; nZ++ )
			{
				pTempBits = (DWORD*)pBits;
				for( nX = nSX; nX < nSX + nWidth; nX++ )
				{
					*pTempBits = ( m_aAlphaMapL3[nZ][nX] << 24 ) | m_aRGBMap[m_nTimeID][nZ][nX];
					pTempBits++;			
				}
				pBits += ddsd.lPitch;
			}
		}
	}
	pddsTempSurface->Unlock( 0 );

	// Copy the temp surface to the real texture surface
	if( nLayer == 2 )
		m_aClrMapTexL2[nIndex]->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );
	else
		m_aClrMapTexL3[nIndex]->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );

	// Done with the temp objects
	pddsTempSurface->Release();
	pDD->Release();

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : OptimizeLayers()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::OptimizeLayers( int nSZ, int nSX, int nEZ, int nEX )
{
	int		nX, nZ;
	int		x, z, sx, ex, sz, ez;
	bool	bOptimize;
	int		nWidth = ( m_nHMWidth - 1 ) * 2 - 1;

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			sx = nX*2-1 < 0 ? 0 : nX*2-1;
			ex = nX*2+2 > nWidth ? nWidth : nX*2+2;
			sz = nZ*2-1 < 0 ? 0 : nZ*2-1;
			ez = nZ*2+2 > nWidth ? nWidth : nZ*2+2;

			// If alpha channels are 0
			bOptimize = true;
			for( z = sz; z <= ez; z++ )
			{
				for( x = sx; x <= ex; x++ )
				{
					if( m_aAlphaMapL2[z][x] > 0 )
					{
						bOptimize = false;
						break;
					}
				}
			}
			if( bOptimize )
				m_aQuadNodes[nZ][nX]->SetL2TexID( TEXTURE_MAX_COUNT );

			bOptimize = true;
			for( z = sz; z <= ez; z++ )
			{
				for( x = sx; x <= ex; x++ )
				{
					if( m_aAlphaMapL3[z][x] > 0 )
					{
						bOptimize = false;
						break;
					}
				}
			}
			if( bOptimize )
				m_aQuadNodes[nZ][nX]->SetL3TexID( TEXTURE_MAX_COUNT );
		}
	}
}


//-----------------------------------------------------------------------------
// Name : SaveNmf()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::SaveNmf( const TCHAR* szFilename )
{
	FILE*	fp = fopen( szFilename, "wb" );
	
	if( fp == NULL )
		return false;

	// Magic( NMF + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	//DWORD	dwMagic = ( ( 'N' << 24 ) | ( 'M' << 16 ) | ( 'F' << 8 ) ) + 10;
	// [10/10/2007 Theodoric] 구번전을 다시 저장하면 무조건 신버전으로 만들자. 
	DWORD	dwMagic = ( ( 'N' << 24 ) | ( 'M' << 16 ) | ( 'F' << 8 ) ) + 11;
	fwrite( &dwMagic, sizeof(DWORD), 1, fp );

	// Terrain Name
	int	nLength = _tcslen( m_szTerName );
	fwrite( &nLength, sizeof(int), 1, fp );
	fwrite( m_szTerName, sizeof(TCHAR), nLength, fp );
	// Patch size
	fwrite( &m_nPatchSize, sizeof(int), 1, fp );
	// Height scale
	fwrite( &m_nHeightScale, sizeof(int), 1, fp );	

	// Height map width
	fwrite( &m_nHMWidth, sizeof(int), 1, fp );
	// Height map
	fwrite( m_pHeights, sizeof(unsigned char), m_nHMWidth*m_nHMWidth, fp );

	// Terrain Textures
	int	nCount = m_vecTerTex.size();
	fwrite( &nCount, sizeof(int), 1, fp );
	int i = 0;
	for( i = 0; i < nCount; i++ )
	{
		nLength = _tcslen( m_vecTerTex[i]->GetTextureName( 0 ) );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( m_vecTerTex[i]->GetTextureName( 0 ), sizeof(TCHAR), nLength, fp );
	}

	// Texture IDs
	int				nX, nZ;
	unsigned char	nTexID;
	for( nZ = 0; nZ < m_nHMWidth - 1; nZ++ )
	{
		for( nX = 0; nX < m_nHMWidth - 1; nX++ )
		{
			nTexID = m_aQuadNodes[nZ][nX]->GetL1TexID();
			fwrite( &nTexID, sizeof(unsigned char), 1, fp );
			nTexID = m_aQuadNodes[nZ][nX]->GetL2TexID();
			fwrite( &nTexID, sizeof(unsigned char), 1, fp );
			nTexID = m_aQuadNodes[nZ][nX]->GetL3TexID();
			fwrite( &nTexID, sizeof(unsigned char), 1, fp );
		}
	}

	// Attribute map
	fwrite( &m_nAttributeMapWidth, sizeof(int), 1, fp );
	if( m_nAttributeMapWidth != 0 )
		fwrite( m_pAttributeMap, sizeof(unsigned char), m_nAttributeMapWidth * m_nAttributeMapWidth, fp );
	
	// Color map
	int	nWidth = ( m_nHMWidth - 1 ) * 2;
	for( nZ = 0; nZ < nWidth; nZ++ )
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			// Alpha channel - layer 2
			fwrite( &m_aAlphaMapL2[nZ][nX], sizeof(unsigned char), 1, fp );
			// Alpha channel - layer 3
			fwrite( &m_aAlphaMapL3[nZ][nX], sizeof(unsigned char), 1, fp );
			// Shadow
			fwrite( &m_aShadowR[0][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowG[0][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowB[0][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowR[1][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowG[1][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowB[1][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowR[2][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowG[2][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowB[2][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowR[3][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowG[3][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowB[3][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowR[4][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowG[4][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowB[4][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowR[5][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowG[5][nZ][nX], sizeof(float), 1, fp );
			fwrite( &m_aShadowB[5][nZ][nX], sizeof(float), 1, fp );
		}
	}

	// Direcitonal light
	for( i = 0; i < 6; i++ )
	{
		// Direction
		fwrite( &m_aDirectLightDir[i], sizeof(D3DXVECTOR3), 1, fp );
		// Directional light color
		fwrite( &m_aDirectLightR[i], sizeof(float), 1, fp );
		fwrite( &m_aDirectLightG[i], sizeof(float), 1, fp );
		fwrite( &m_aDirectLightB[i], sizeof(float), 1, fp );
		// Ambient color
		fwrite( &m_aAmbientLightR[i], sizeof(float), 1, fp );
		fwrite( &m_aAmbientLightG[i], sizeof(float), 1, fp );
		fwrite( &m_aAmbientLightB[i], sizeof(float), 1, fp );		
	}

	fclose( fp );
  
	return true;
}


//-----------------------------------------------------------------------------
// Name : SaveMap()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::SaveMap( const TCHAR* szFilename )
{
	// Optimize map - replace invisible tiles
	if( !g_pMyNKApp->IsLoadedGameMap() )
		OptimizeTerrainMap();

	// Calculate count & size of index buffers
	int		i, j;
	int		nCount = m_vecTerTex.size();
	DWORD	dwL1IndexCount[TEXTURE_MAX_COUNT+1];
	DWORD	dwL2IndexCount[TEXTURE_MAX_COUNT+1];
	DWORD	dwL3IndexCount[TEXTURE_MAX_COUNT+1];
	for( j = 0; j <= TEXTURE_MAX_COUNT; j++ )
	{
		dwL1IndexCount[j] = 0;
		dwL2IndexCount[j] = 0;
		dwL3IndexCount[j] = 0;
	}

	for( i = 0; i < 4; i++ )
	{
		for( j = 0; j < nCount; j++ )
		{
			m_vecIBL1[j]->dwTotalCount = 0;
			m_vecIBL2[j]->dwTotalCount = 0;
			m_vecIBL3[j]->dwTotalCount = 0;
		}
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			m_vecIBL1[nCount]->dwTotalCount = 0;
			m_vecIBL2[nCount]->dwTotalCount = 0;
			m_vecIBL3[nCount]->dwTotalCount = 0;
		}
		else
		{
			m_vecIBL1[TEXTURE_MAX_COUNT]->dwTotalCount = 0;
			m_vecIBL2[TEXTURE_MAX_COUNT]->dwTotalCount = 0;
			m_vecIBL3[TEXTURE_MAX_COUNT]->dwTotalCount = 0;
		}

		if( i > 0 && m_nHMWidth < 257  )
			break;

		m_aRootNode[i]->CountIndices();

		for( j = 0; j < nCount; j++ )
		{
			if( dwL1IndexCount[j] < m_vecIBL1[j]->dwTotalCount )
				dwL1IndexCount[j] = m_vecIBL1[j]->dwTotalCount;
			if( dwL2IndexCount[j] < m_vecIBL2[j]->dwTotalCount )
				dwL2IndexCount[j] = m_vecIBL2[j]->dwTotalCount;
			if( dwL3IndexCount[j] < m_vecIBL3[j]->dwTotalCount )
				dwL3IndexCount[j] = m_vecIBL3[j]->dwTotalCount;
		}
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			if( dwL1IndexCount[nCount] < m_vecIBL1[nCount]->dwTotalCount )
				dwL1IndexCount[nCount] = m_vecIBL1[nCount]->dwTotalCount;
			if( dwL2IndexCount[nCount] < m_vecIBL2[nCount]->dwTotalCount )
				dwL2IndexCount[nCount] = m_vecIBL2[nCount]->dwTotalCount;
			if( dwL3IndexCount[nCount] < m_vecIBL3[nCount]->dwTotalCount )
				dwL3IndexCount[nCount] = m_vecIBL3[nCount]->dwTotalCount;
		}
		else
		{
			if( dwL1IndexCount[TEXTURE_MAX_COUNT] < m_vecIBL1[TEXTURE_MAX_COUNT]->dwTotalCount )
				dwL1IndexCount[TEXTURE_MAX_COUNT] = m_vecIBL1[TEXTURE_MAX_COUNT]->dwTotalCount;
			if( dwL2IndexCount[TEXTURE_MAX_COUNT] < m_vecIBL2[TEXTURE_MAX_COUNT]->dwTotalCount )
				dwL2IndexCount[TEXTURE_MAX_COUNT] = m_vecIBL2[TEXTURE_MAX_COUNT]->dwTotalCount;
			if( dwL3IndexCount[TEXTURE_MAX_COUNT] < m_vecIBL3[TEXTURE_MAX_COUNT]->dwTotalCount )
				dwL3IndexCount[TEXTURE_MAX_COUNT] = m_vecIBL3[TEXTURE_MAX_COUNT]->dwTotalCount;
		}
	}

	// Create the attribute map for the client
	int				nIndex, nOffset, nValue;
	int				nClientAttrMapHeight = ( m_nHMWidth - 1 ) * m_nPatchSize / 10;
	int				nClientAttrMapWidth = ( nClientAttrMapHeight - 1 ) / 8 + 1;	
	unsigned char*	pClientAttrMap = new unsigned char[nClientAttrMapWidth * nClientAttrMapHeight];
	memset( pClientAttrMap, 0, sizeof(unsigned char) * nClientAttrMapWidth * nClientAttrMapHeight );
	if( m_pAttributeMap )
	{
		for( i = 0; i < m_nAttributeMapWidth * m_nAttributeMapWidth; i++ )
		{
			nIndex = i / 8;
			nOffset = i % 8;

			if( m_pAttributeMap[i] == g_nUNABLETOWALK )
				nValue = 1;
			else
				nValue = 0;

			nValue <<= nOffset;
			pClientAttrMap[nIndex] |= nValue;
		}
	}

	// Create the light map for saving
	int				nX, nZ;
	int				nWidth = ( m_nHMWidth - 1 ) * 2;
	unsigned char	nR, nG, nB;
	unsigned char*	pLightMapR[6];
	unsigned char*	pLightMapG[6];
	unsigned char*	pLightMapB[6];
	for( i = 0; i < 6; i++ )
	{
		pLightMapR[i] = new unsigned char[nWidth * nWidth];
		pLightMapG[i] = new unsigned char[nWidth * nWidth];
		pLightMapB[i] = new unsigned char[nWidth * nWidth];
	}
	for( i = 0; i < 6; i++ )
	{
		nCount = 0;
		for( nZ = 0; nZ < nWidth; nZ++ )
		{
			for( nX = 0; nX < nWidth; nX++ )
			{
				nR = 255 * ( m_aLightR[i][nZ][nX] * m_aShadowR[i][nZ][nX] );
				nG = 255 * ( m_aLightG[i][nZ][nX] * m_aShadowG[i][nZ][nX] );
				nB = 255 * ( m_aLightB[i][nZ][nX] * m_aShadowB[i][nZ][nX] );
				pLightMapR[i][nCount] = nR;
				pLightMapG[i][nCount] = nG;
				pLightMapB[i][nCount++] = nB;
			}
		}
	}

	// Create texture IDs for saving
	nWidth = m_nHMWidth - 1;
	nCount = m_vecTerTex.size();
	unsigned char*	pL1TexID = new unsigned char[nWidth*nWidth];
	unsigned char*	pL2TexID = new unsigned char[nWidth*nWidth];
	unsigned char*	pL3TexID = new unsigned char[nWidth*nWidth];
	for( i = 0, nZ = 0; nZ < nWidth; nZ++ )
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			pL1TexID[i] = m_aQuadNodes[nZ][nX]->GetL1TexID();
			pL2TexID[i] = m_aQuadNodes[nZ][nX]->GetL2TexID();
			pL3TexID[i] = m_aQuadNodes[nZ][nX]->GetL3TexID();

			if( pL1TexID[i] == TEXTURE_MAX_COUNT ) pL1TexID[i] = nCount;
			if( pL2TexID[i] == TEXTURE_MAX_COUNT ) pL2TexID[i] = nCount;
			if( pL3TexID[i] == TEXTURE_MAX_COUNT ) pL3TexID[i] = nCount;
			i++;
		}
	}

	// Save map
	FILE*	fp = fopen( szFilename, "wb" );
	
	if( fp == NULL )
		return false;

	// [10/9/2007 Theodoric] 텍스처 제한을 15에서 50으로 늘리기 위해 무조건 신버전으로 저장한다.
	// Magic( MAP + Version ) - Version: 0.1 -> 1, 1.0 -> 10
	//DWORD	dwMagic = ( ( 'M' << 24 ) | ( 'A' << 16 ) | ( 'P' << 8 ) ) + 10;
	DWORD	dwMagic = ( ( 'M' << 24 ) | ( 'A' << 16 ) | ( 'P' << 8 ) ) + 11;
	fwrite( &dwMagic, sizeof(DWORD), 1, fp );

	// Patch size
	fwrite( &m_nPatchSize, sizeof(int), 1, fp );

	// Height scale
	fwrite( &m_nHeightScale, sizeof(int), 1, fp );

	// Height map
	fwrite( &m_nHMWidth, sizeof(int), 1, fp );										// width
	fwrite( m_pHeights, sizeof(unsigned char), m_nHMWidth*m_nHMWidth, fp );			// data

	// Attribute map
	fwrite( pClientAttrMap, sizeof(unsigned char), nClientAttrMapWidth * nClientAttrMapHeight, fp );
	
	// Alpha map
	nWidth = ( m_nHMWidth - 1 ) * 2;
	for( nZ = 0; nZ < nWidth; nZ++ )												// layer 2
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			fwrite( &m_aAlphaMapL2[nZ][nX], sizeof(unsigned char), 1, fp );
		}
	}
	for( nZ = 0; nZ < nWidth; nZ++ )												// layer 3
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			fwrite( &m_aAlphaMapL3[nZ][nX], sizeof(unsigned char), 1, fp );
		}
	}

	// Light map
	nWidth = ( m_nHMWidth - 1 ) * 2;
	for( i = 0; i < 6; i++ )
	{
		fwrite( pLightMapR[i], sizeof(unsigned char), nWidth * nWidth, fp );
		fwrite( pLightMapG[i], sizeof(unsigned char), nWidth * nWidth, fp );
		fwrite( pLightMapB[i], sizeof(unsigned char), nWidth * nWidth, fp );
	}

	// Texture IDs
	nWidth = m_nHMWidth - 1;
	fwrite( pL1TexID, sizeof(unsigned char), nWidth * nWidth, fp );					// layer 1
	fwrite( pL2TexID, sizeof(unsigned char), nWidth * nWidth, fp );					// layer 2
	fwrite( pL3TexID, sizeof(unsigned char), nWidth * nWidth, fp );					// layer 3

	// Index buffers
	nCount = m_vecTerTex.size();
	fwrite( &nCount, sizeof(int), 1, fp );
	for( i = 0; i < nCount; i++ )
	{
		fwrite( &(dwL1IndexCount[i]), sizeof(DWORD), 1, fp );						// layer 1
		fwrite( &(dwL2IndexCount[i]), sizeof(DWORD), 1, fp );						// layer 2
		fwrite( &(dwL3IndexCount[i]), sizeof(DWORD), 1, fp );						// layer 3
	}
	if( g_pMyNKApp->IsLoadedGameMap() )
	{
		fwrite( &(dwL1IndexCount[nCount]), sizeof(DWORD), 1, fp );					// layer 1
		fwrite( &(dwL2IndexCount[nCount]), sizeof(DWORD), 1, fp );					// layer 2
		fwrite( &(dwL3IndexCount[nCount]), sizeof(DWORD), 1, fp );					// layer 3
	}
	else
	{
		fwrite( &(dwL1IndexCount[TEXTURE_MAX_COUNT]), sizeof(DWORD), 1, fp );						// layer 1
		fwrite( &(dwL2IndexCount[TEXTURE_MAX_COUNT]), sizeof(DWORD), 1, fp );						// layer 2
		fwrite( &(dwL3IndexCount[TEXTURE_MAX_COUNT]), sizeof(DWORD), 1, fp );						// layer 3
	}

	// Textures
	int	nLength;
	nCount = m_vecTerTex.size();
	fwrite( &nCount, sizeof(int), 1, fp );											// count
	for( i = 0; i < nCount; i++ )
	{
		nLength = _tcslen( m_vecTerTex[i]->GetTextureName( 0 ) );
		fwrite( &nLength, sizeof(int), 1, fp );										// name length
		fwrite( m_vecTerTex[i]->GetTextureName( 0 ), sizeof(TCHAR), nLength, fp );	// name
	}

	// Direcitonal light
	for( i = 0; i < 6; i++ )
		fread( &m_aDirectLightDir[i], sizeof(D3DXVECTOR3), 1, fp );

	fclose( fp );

	SAFE_DELETE_ARRAY( pClientAttrMap );
	SAFE_DELETE_ARRAY( pL1TexID );
	SAFE_DELETE_ARRAY( pL2TexID );
	SAFE_DELETE_ARRAY( pL3TexID );
	for( i = 0; i < 6; i++ )
	{
		SAFE_DELETE_ARRAY( pLightMapR[i] );
		SAFE_DELETE_ARRAY( pLightMapG[i] );
		SAFE_DELETE_ARRAY( pLightMapB[i] );
	}

	return true;
}
//-----------------------------------------------------------------------------
// Name : LoadNmfVer()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::LoadNmf( const TCHAR* szFilename ) 
{
	FILE*	fp = fopen( szFilename, "rb" );

	if( fp == NULL )
		return false;

	// Magic( NMF + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic ;
	int		version = 0;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
	fclose( fp );

	// [10/10/2007 Theodoric] 버전 검사를 해서 텍스쳐 갯수를 변경한다. 
	// 텍스쳐 갯수가 바뀔때마다 버전을 끊어서 관리해야한다.
	if( dwMagic == ( ( 'N' << 24 ) | ( 'M' << 16 ) | ( 'F' << 8 ) ) + 10 )
		version = 10;
	else if( dwMagic == ( ( 'N' << 24 ) | ( 'M' << 16 ) | ( 'F' << 8 ) ) + 11 )
		version = 11;
	else
		return false;


	return 	LoadNmfVer( szFilename, version  ) ;
}


//-----------------------------------------------------------------------------
// Name : LoadNmfVer()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::LoadNmfVer( const TCHAR* szFilename, int version  ) 
{
	FILE*	fp = fopen( szFilename, "rb" );

	if( fp == NULL )
		return false;

	// Progress dialog
	g_pMyNKApp->SetProgressTitle( _T(" Load NMF File ") );
	g_pMyNKApp->SetProgressContent( _T("Load a map file for the NKMapTool.") );
	g_pMyNKApp->SetProgressRange( 0, 100 );
	g_pMyNKApp->SetProgressPos( 0 );

	// Magic( NMF + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic ;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
// [10/10/2007 Theodoric] 버전은 상위 함수에서 검사함으로 빽~ 
//	if( dwMagic != ( ( 'N' << 24 ) | ( 'M' << 16 ) | ( 'F' << 8 ) ) + 10 )
//		return false;

	// Terrain name
	int	nLength;
	fread( &nLength, sizeof(int), 1, fp );
	m_szTerName = new TCHAR[nLength+1];
	fread( m_szTerName, sizeof(TCHAR), nLength, fp );
	m_szTerName[nLength] = NULL;
	// Patch size
	fread( &m_nPatchSize, sizeof(int), 1, fp );
	// Height scale
	fread( &m_nHeightScale, sizeof(int), 1, fp );

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 1 );

	// Height map width
	fread( &m_nHMWidth, sizeof(int), 1, fp );
	// Height map		
	m_pHeights = new unsigned char[m_nHMWidth*m_nHMWidth];
	fread( m_pHeights, sizeof(unsigned char), m_nHMWidth*m_nHMWidth, fp );
	
	// Progress dialog
	g_pMyNKApp->SetProgressPos( 2 );

	// Terrain Textures
	int		nCount;
	TCHAR	szTexName[128];
	fread( &nCount, sizeof(int), 1, fp );
	float	fOffset = 5.0f / (float)nCount;
	float	fPos = 2;
	int i = 0;
	for( i = 0; i < nCount; i++ )
	{
		fread( &nLength, sizeof(int), 1, fp );
		fread( szTexName, sizeof(TCHAR), nLength, fp );
		szTexName[nLength] = NULL;
		if( AddTexture( szTexName ) < 3 )
		{			
			Destroy();
			fclose( fp );
			return false;
		}
		fPos += fOffset;
		g_pMyNKApp->SetProgressPos( (int)fPos );
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 7 );

	// Texture IDs
	int		nX, nZ;
	int		nWidth = m_nHMWidth - 1;
	fOffset = 20.0f / (float)(nWidth*nWidth);
	fPos = 7;
	unsigned char*	pL1TexID = new unsigned char[nWidth*nWidth];
	unsigned char*	pL2TexID = new unsigned char[nWidth*nWidth];
	unsigned char*	pL3TexID = new unsigned char[nWidth*nWidth];
	for( i = 0, nZ = 0; nZ < nWidth; nZ++ )
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			fread( &pL1TexID[i], sizeof(unsigned char), 1, fp );
			fread( &pL2TexID[i], sizeof(unsigned char), 1, fp );
			fread( &pL3TexID[i], sizeof(unsigned char), 1, fp );
			
			// [10/9/2007 Theodoric] 텍스처 제한을 15에서 50으로 늘린다.
			// 구버젼의 마지막(빈 텍스쳐 번호를 시버젼에 맞춰주는 작업이 꼭 필요하다.
			// 다시 텍스쳐 겟수를 늘리면 이작업이 필수적으로 이루어져야 한다.
			if( version == 10 )
			{				
				if(pL1TexID[i] == 15 ) pL1TexID[i] = TEXTURE_MAX_COUNT;
				if(pL2TexID[i] == 15 ) pL2TexID[i] = TEXTURE_MAX_COUNT;
				if(pL3TexID[i] == 15 ) pL3TexID[i] = TEXTURE_MAX_COUNT;
			}
			
			i++;
			fPos += fOffset;
			g_pMyNKApp->SetProgressPos( (int)fPos );
		}		
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 27 );

	// Attribute map
	fread( &m_nAttributeMapWidth, sizeof(int), 1, fp );
	if( m_nAttributeMapWidth != 0 )
	{
		SAFE_DELETE_ARRAY( m_pAttributeMap );
		m_pAttributeMap = new unsigned char[m_nAttributeMapWidth * m_nAttributeMapWidth];
		fread( m_pAttributeMap, sizeof(unsigned char), m_nAttributeMapWidth * m_nAttributeMapWidth, fp );
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 30 );

	// Color map
	nWidth = ( m_nHMWidth - 1 ) * 2;
	fOffset = 60.0f / (float)(nWidth*nWidth);
	fPos = 30;
	for( nZ = 0; nZ < nWidth; nZ++ )
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			// Alpha channel - layer 2
			fread( &m_aAlphaMapL2[nZ][nX], sizeof(unsigned char), 1, fp );
			// Alpha channel - layer 3
			fread( &m_aAlphaMapL3[nZ][nX], sizeof(unsigned char), 1, fp );
			// Shadow
			fread( &m_aShadowR[0][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowG[0][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowB[0][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowR[1][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowG[1][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowB[1][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowR[2][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowG[2][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowB[2][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowR[3][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowG[3][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowB[3][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowR[4][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowG[4][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowB[4][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowR[5][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowG[5][nZ][nX], sizeof(float), 1, fp );
			fread( &m_aShadowB[5][nZ][nX], sizeof(float), 1, fp );

			fPos += fOffset;
			g_pMyNKApp->SetProgressPos( (int)fPos );
		}		
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 90 );

	// Direcitonal light
	for( i = 0; i < 6; i++ )
	{
		// Direction
		fread( &m_aDirectLightDir[i], sizeof(D3DXVECTOR3), 1, fp );
		// Directional light color
		fread( &m_aDirectLightR[i], sizeof(float), 1, fp );
		fread( &m_aDirectLightG[i], sizeof(float), 1, fp );
		fread( &m_aDirectLightB[i], sizeof(float), 1, fp );
		// Ambient color
		fread( &m_aAmbientLightR[i], sizeof(float), 1, fp );
		fread( &m_aAmbientLightG[i], sizeof(float), 1, fp );
		fread( &m_aAmbientLightB[i], sizeof(float), 1, fp );		
	}

	fclose( fp );

	// Add index buffer(15) without a texture
	TerIB*	pIB;
	pIB = new TerIB;
	m_vecIBL1[TEXTURE_MAX_COUNT] = pIB;
	pIB->pIB = new WORD[128*128*6];
	pIB = new TerIB;
	m_vecIBL2[TEXTURE_MAX_COUNT] = pIB;
	pIB->pIB = new WORD[128*128*6];
	pIB = new TerIB;
	m_vecIBL3[TEXTURE_MAX_COUNT] = pIB;
	pIB->pIB = new WORD[128*128*6];

	// Vertex buffer width & size
	int	nWHSegment = m_nHMWidth == 257 ? 128 : m_nHMWidth - 1;
	m_nVBWidth = nWHSegment * 3 / 2;								// W/H size of vertex buffer is less than 256
	m_dwVBSize = m_nVBWidth * m_nVBWidth;

	// Get a quad tree depth
	m_nQuadTreeDepth = 0;
	while( nWHSegment >>= 1 )
		m_nQuadTreeDepth++;

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 92 );

	// Initialize quad tree
	if( !InitQuadTree( pL1TexID, pL2TexID, pL3TexID ) )
	{
		Destroy();
		return false;
	}
	SAFE_DELETE_ARRAY( pL1TexID );
	SAFE_DELETE_ARRAY( pL2TexID );
	SAFE_DELETE_ARRAY( pL3TexID );

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 95 );

	// Initialize vertex buffer
	if( !InitVertexBuffer() )
	{
		Destroy();
		return false;
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 99 );

	// Create a temporary heights, copy height values and calculate min/max heights
	m_pTempHeights = new float[m_nHMWidth * m_nHMWidth];
	m_nMinHeight = 255;
	m_nMaxHeight = 0;
	for( i = 0, nZ = 0; nZ < m_nHMWidth; nZ++ )
	{
		for( nX = 0; nX < m_nHMWidth; nX++ )
		{
			m_pTempHeights[i] = m_pHeights[i];
			if( m_pHeights[i] < m_nMinHeight )
				m_nMinHeight = m_pHeights[i];
			if( m_pHeights[i] > m_nMaxHeight )
				m_nMaxHeight = m_pHeights[i];
			i++;
		}
	}

	// Progress dialog
	g_pMyNKApp->SetProgressPos( 100 );

	// Make normals
	MakeNormals();

	// Copy alpha maps
	nWidth = ( m_nHMWidth - 1 ) * 2;
	for( nZ = 0; nZ < nWidth; nZ++ )
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			m_aAlphaL2[nZ][nX] = m_aAlphaMapL2[nZ][nX];
			m_aAlphaL3[nZ][nX] = m_aAlphaMapL3[nZ][nX];
		}
	}

	// Make rgb maps
	MakeAllRGBMaps();

	// Create color map tesxtures
	if( FAILED( CreateClrMapTexture() ) )
	{
		Destroy();
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : LoadMap()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::LoadMap( const TCHAR* szFilename )
{
	// Load map
	FILE*	fp = fopen( szFilename, "rb" );
	
	if( fp == NULL )
		return false;

	// Magic( MAP + Version ) - Version: 0.1 -> 1, 1.0 -> 10
	DWORD	dwMagic;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
	if( (dwMagic == ( ( 'M' << 24 ) | ( 'A' << 16 ) | ( 'P' << 8 ) ) + 10 ) ||
		(dwMagic == ( ( 'M' << 24 ) | ( 'A' << 16 ) | ( 'P' << 8 ) ) + 11 ) )
	{
		//
	}
	else
	{
		fclose( fp );
		return false;
	}

	// Patch size
	fread( &m_nPatchSize, sizeof(int), 1, fp );

	// Height scale
	fread( &m_nHeightScale, sizeof(int), 1, fp );	

	// Height map
	fread( &m_nHMWidth, sizeof(int), 1, fp );
	m_pHeights = new unsigned char[m_nHMWidth * m_nHMWidth];
	fread( m_pHeights, sizeof(unsigned char), m_nHMWidth * m_nHMWidth, fp );

	// Attribute map
	int gMapExtention = 50;
	int gMapWidth = 256;

	/*int	nClientAttrMapHeight = ( m_nHMWidth - 1 ) * m_nPatchSize / 10;
	//  속성값은 0 또는 1이면 충분하다 따라서 비트값을 사용
	//	1 byte == 8 bit
	int	nClientAttrMapWidth = ( nClientAttrMapHeight - 1 ) / 8 + 1;
	fseek( fp, sizeof(unsigned char) * nClientAttrMapWidth * nClientAttrMapHeight, SEEK_CUR );
	SAFE_DELETE_ARRAY( m_pAttributeMap );*/
	SAFE_DELETE_ARRAY( m_pAttributeMap );
	int nAttrMapWidth = gMapWidth * gMapExtention / 10;
	int nAttrMapByteWidth = ( nAttrMapWidth - 1 ) / 8 + 1;
	int nAttrMapHeight = nAttrMapWidth;
	m_pAttributeMap = new unsigned char[nAttrMapByteWidth * nAttrMapHeight];
	fread( m_pAttributeMap, sizeof(unsigned char), nAttrMapByteWidth * nAttrMapHeight, fp );

	
	// Alpha map
	int	nWidth = ( m_nHMWidth - 1 ) * 2;
	m_pAlphaMapL2 = new unsigned char[nWidth * nWidth];
	fread( m_pAlphaMapL2, sizeof(unsigned char), nWidth * nWidth, fp );
	m_pAlphaMapL3 = new unsigned char[nWidth * nWidth];
	fread( m_pAlphaMapL3, sizeof(unsigned char), nWidth * nWidth, fp );

	// Light map
	nWidth = ( m_nHMWidth - 1 ) * 2;
	int i = 0;
	for( i = 0; i < 6; i++ )
	{
		m_pLightMapsR[i] = new unsigned char[nWidth * nWidth];
		m_pLightMapsG[i] = new unsigned char[nWidth * nWidth];
		m_pLightMapsB[i] = new unsigned char[nWidth * nWidth];
	}
	for( i = 0; i < 6; i++ )
	{
		fread( m_pLightMapsR[i], sizeof(unsigned char), nWidth * nWidth, fp );
		fread( m_pLightMapsG[i], sizeof(unsigned char), nWidth * nWidth, fp );
		fread( m_pLightMapsB[i], sizeof(unsigned char), nWidth * nWidth, fp );
	}

	// Texture IDs
	nWidth = m_nHMWidth - 1;
	unsigned char*	pL1TexID = new unsigned char[nWidth * nWidth];
	unsigned char*	pL2TexID = new unsigned char[nWidth * nWidth];
	unsigned char*	pL3TexID = new unsigned char[nWidth * nWidth];
	fread( pL1TexID, sizeof(unsigned char), nWidth * nWidth, fp );
	fread( pL2TexID, sizeof(unsigned char), nWidth * nWidth, fp );
	fread( pL3TexID, sizeof(unsigned char), nWidth * nWidth, fp );

	// Index buffers
	int		nCount;
	DWORD	dwIndexCount;
	fread( &nCount, sizeof(int), 1, fp );
	for( i = 0; i <= nCount; i++ )
	{
		TerIB*	pIB;
		pIB = new TerIB;
		m_vecIBL1[i] = pIB;
		fread( &dwIndexCount, sizeof(DWORD), 1, fp );
		if( dwIndexCount > 0 )
			pIB->pIB = new WORD[dwIndexCount];
		else
			pIB->pIB = NULL;

		pIB = new TerIB;
		m_vecIBL2[i] = pIB;
		fread( &dwIndexCount, sizeof(DWORD), 1, fp );
		if( dwIndexCount > 0 )
			pIB->pIB = new WORD[dwIndexCount];
		else
			pIB->pIB = NULL;

		pIB = new TerIB;
		m_vecIBL3[i] = pIB;
		fread( &dwIndexCount, sizeof(DWORD), 1, fp );
		if( dwIndexCount > 0 )
			pIB->pIB = new WORD[dwIndexCount];
		else
			pIB->pIB = NULL;
	}

	// Textures
	TCHAR	szPath[256];
	_tcscpy( szPath, _T("map\\") );
	_tcscat( szPath, m_szTerName );
	_tcscat( szPath, _T("\\") );
	szPath[_tcslen( m_szTerName ) + _tcslen( _T("map\\\\") )] = NULL;

	TCHAR	szTexName[128];
	int		nLength;
	fread( &nCount, sizeof(int), 1, fp );
	for( i = 0; i < nCount; i++ )
	{
		fread( &nLength, sizeof(int), 1, fp );
		fread( szTexName, sizeof(TCHAR), nLength, fp );
		szTexName[nLength] = NULL;
		
		IndexedTexture*	pTexture = new IndexedTexture( 1 );
		pTexture->SetTexturePath( szPath );
		if( pTexture->CreateTextureFromFile( szTexName ) < 0 )
		{
			SAFE_DELETE_ARRAY( pL1TexID );
			SAFE_DELETE_ARRAY( pL2TexID );
			SAFE_DELETE_ARRAY( pL3TexID );
			Destroy();
			fclose( fp );
			return false;
		}

		pTexture->RestoreAllTextures( m_pd3dDevice );
		m_vecTerTex.push_back( pTexture );
	}

	// Direcitonal light
	for( i = 0; i < 6; i++ )
		fread( &m_aDirectLightDir[i], sizeof(D3DXVECTOR3), 1, fp );

	fclose( fp );

	// Vertex buffer width & size
	int	nWHSegment = m_nHMWidth == 257 ? 128 : m_nHMWidth - 1;
	m_nVBWidth = nWHSegment * 3 / 2;
	m_dwVBSize = m_nVBWidth * m_nVBWidth;

	// Get a quad tree depth
	m_nQuadTreeDepth = 0;
	while( nWHSegment >>= 1 )
		m_nQuadTreeDepth++;

	// Initialize quad tree
	if( !InitQuadTree( pL1TexID, pL2TexID, pL3TexID ) )
	{
		Destroy();
		return false;
	}
	SAFE_DELETE_ARRAY( pL1TexID );
	SAFE_DELETE_ARRAY( pL2TexID );
	SAFE_DELETE_ARRAY( pL3TexID );

	// Initialize vertex buffer
	if( !InitVertexBuffer() )
	{
		Destroy();
		return false;
	}

	// Create color map textures
	if( FAILED( CreateClrMapTexture() ) )
	{
		Destroy();
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : ResetMinMaxHeights()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ResetMinMaxHeights()
{
	int	i, nX, nZ;

	m_nMinHeight = 255;
	m_nMaxHeight = 0;
	for( i = 0, nZ = 0; nZ < m_nHMWidth; nZ++ )
	{
		for( nX = 0; nX < m_nHMWidth; nX++ )
		{
			if( m_pHeights[i] < m_nMinHeight )
				m_nMinHeight = m_pHeights[i];
			if( m_pHeights[i] > m_nMaxHeight )
				m_nMaxHeight = m_pHeights[i];
			i++;
		}
	}
}


//-----------------------------------------------------------------------------
// Name : RecalcBoundingBox()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RecalcAllBoundingBox()
{
	int nRightX = 1 << m_nQuadTreeDepth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;
	
	// Lower left
	m_aRootNode[0]->RecalcAllBoundingBox( 0, 0, nRightX, nRightZ );

	nCenterX = nRightX;
	nCenterZ = nRightZ;
	nRightX <<= 1;
	nRightZ <<= 1;

	if( m_nHMWidth == 257 )				
	{
		// Lower right
		m_aRootNode[1]->RecalcAllBoundingBox( nCenterX, 0, nRightX, nCenterZ );

		// Upper left
		m_aRootNode[2]->RecalcAllBoundingBox( 0, nCenterZ, nCenterX, nRightZ );

		// Upper right
		m_aRootNode[3]->RecalcAllBoundingBox( nCenterX, nCenterZ, nRightX, nRightZ );
	}
}


//-----------------------------------------------------------------------------
// Name : RemakeVertexBuffer()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RemakeAllVertexBuffers()
{
	int nWidth = 1 << m_nQuadTreeDepth;
	int nRightX = nWidth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;

	NKVERTEXT2*	pVB = new NKVERTEXT2[m_nVBWidth * m_nVBWidth];
	
	NKVERTEXT2*	pVert;
	// Lower left vertex buffer
	CNKTerQuadNode::ms_nVBIdxCnt = 0;
	m_aRootNode[0]->InitVertexBuffer( 0, 0, nRightX, nRightZ,	0, 0, pVB );	
	if( FAILED( m_aVB[0]->Lock( DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (LPVOID*)(&pVert), NULL ) ) )
	{
		SAFE_DELETE_ARRAY( pVB );
		return ;
	}
	memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
	m_aVB[0]->Unlock();

	nCenterX = nRightX;		nCenterZ = nRightZ;
	nRightX <<= 1;			nRightZ <<= 1;

	if( m_nHMWidth == 257 )
	{
		// Lower right vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[1]->InitVertexBuffer( nCenterX, 0, nRightX, nCenterZ,	-1, 0, pVB );
		if( FAILED( m_aVB[1]->Lock( DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return ;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[1]->Unlock();

		// Upper left vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[2]->InitVertexBuffer( 0, nCenterZ, nCenterX, nRightZ,	0, -1, pVB );
		if( FAILED( m_aVB[2]->Lock( DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return ;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[2]->Unlock();

		// Upper right vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[3]->InitVertexBuffer( nCenterX, nCenterZ, nRightX, nRightZ, -1, -1, pVB );
		if( FAILED( m_aVB[3]->Lock( DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			return ;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[3]->Unlock();
	}

	SAFE_DELETE_ARRAY( pVB );
}


//-----------------------------------------------------------------------------
// Name : ModifyPatchSize()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ModifyPatchSize( int nPatchSize )
{
	float	fScale = (float)nPatchSize / (float)m_nPatchSize;
	
	// Reset a patch size
	m_nPatchSize = nPatchSize;

	// Recalculate bounding boxes of a terrain
	RecalcAllBoundingBox();	

	// Remake vertex buffers
	RemakeAllVertexBuffers();

	// Scale a position of decoobjs in a decoobj quadtree
	CNKObjQuadNode::SetCellSize( m_nPatchSize * 4 );
	CNKObjQuadNode::ScaleAllObjPosition( fScale, 1 );

	// Recalculate normals
	ComputeNormals( 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );

	// Renew all rgb maps
	RenewAllRGBMaps( 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );

	// Update color map textures
	UpdateClrMapTexture( 2, 0 );
	UpdateClrMapTexture( 3, 0 );
	if( m_nHMWidth == 257 )
	{
		UpdateClrMapTexture( 2, 1 );
		UpdateClrMapTexture( 2, 2 );
		UpdateClrMapTexture( 2, 3 );
		UpdateClrMapTexture( 3, 1 );
		UpdateClrMapTexture( 3, 2 );		
		UpdateClrMapTexture( 3, 3 );
	}
}


//-----------------------------------------------------------------------------
// Name : ModifyHeightScale()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ModifyHeightScale( int nHeightScale )
{
	float	fScale = (float)nHeightScale / (float)m_nHeightScale;

	// Reset a height scale
	m_nHeightScale = nHeightScale;

	// Recalculate bounding boxed of a terrain
	RecalcAllBoundingBox();	

	// Remake vertex buffers
	RemakeAllVertexBuffers();

	// Scale a position of decoobjs in a decoobj quadtree	
	CNKObjQuadNode::ScaleAllObjPosition( 1, fScale );

	// Recalculate normals
	ComputeNormals( 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );

	// Renew all rgb maps
	RenewAllRGBMaps( 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );

	// Update color map textures
	UpdateClrMapTexture( 2, 0 );
	UpdateClrMapTexture( 3, 0 );
	if( m_nHMWidth == 257 )
	{
		UpdateClrMapTexture( 2, 1 );
		UpdateClrMapTexture( 2, 2 );
		UpdateClrMapTexture( 2, 3 );
		UpdateClrMapTexture( 3, 1 );
		UpdateClrMapTexture( 3, 2 );		
		UpdateClrMapTexture( 3, 3 );
	}
}


//-----------------------------------------------------------------------------
// Name : ReapplyHeightMap()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ReapplyHeightMap( const TCHAR* szFilename )
{
	// Reload a height map
	LoadHeightMap( szFilename );

	// Recalculate bounding boxes of a terrain
	RecalcAllBoundingBox();	

	// Remake vertex buffers
	RemakeAllVertexBuffers();

	// Recalculate normals
	ComputeNormals( 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );

	// Renew all rgb maps
	RenewAllRGBMaps( 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );

	// Update color map textures
	UpdateClrMapTexture( 2, 0 );
	UpdateClrMapTexture( 3, 0 );
	if( m_nHMWidth == 257 )
	{
		UpdateClrMapTexture( 2, 1 );
		UpdateClrMapTexture( 2, 2 );
		UpdateClrMapTexture( 2, 3 );
		UpdateClrMapTexture( 3, 1 );
		UpdateClrMapTexture( 3, 2 );		
		UpdateClrMapTexture( 3, 3 );
	}
}


//-----------------------------------------------------------------------------
// Name : ModifyAllHeights()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ModifyAllHeights( int nOffset )
{
	// Modify heights
	int				i, nX, nZ;
	unsigned char	nMin = 255, nMax = 0;
	for( i = 0, nZ = 0; nZ < m_nHMWidth; nZ++ )
	{
		for( nX = 0; nX < m_nHMWidth; nX++ )
		{			
			m_pTempHeights[i] += nOffset;
			m_pHeights[i] += nOffset;
			i++;
		}
	}

	// Reset min/max heights
	m_nMinHeight = nMin;
	m_nMaxHeight = nMax;

	// Recalculate bounding boxes of a terrain
	RecalcAllBoundingBox();	

	// Remake vertex buffers
	RemakeAllVertexBuffers();
}


//-----------------------------------------------------------------------------
// Name : RaiseTiles()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RaiseTiles( int nSZ, int nSX, int nEZ, int nEX, float fStrength )
{
	unsigned char*	pHeights;
	float*			pTempHeights;
	int				nX, nZ;

	// Raise heights
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pHeights = &(m_pHeights[nSX+nZ*m_nHMWidth]);
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);
		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			*pTempHeights += fStrength;
			if( *pTempHeights > 255.0f )
				*pTempHeights = 255.0f;

			*pHeights = (unsigned char)(*pTempHeights);

			pHeights++;
			pTempHeights++;
		}
	}

	// Renew vertex buffers
	RenewVertexBuffers( nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : LowerTiles()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::LowerTiles( int nSZ, int nSX, int nEZ, int nEX, float fStrength )
{
	unsigned char*	pHeights;
	float*			pTempHeights;
	int				nX, nZ;

	// Lower heights
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pHeights = &(m_pHeights[nSX+nZ*m_nHMWidth]);
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);
		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			*pTempHeights -= fStrength;
			if( *pTempHeights < 0.0f )
				*pTempHeights = 0.0f;

			*pHeights = (unsigned char)(*pTempHeights);

			pHeights++;
			pTempHeights++;
		}
	}

	// Renew vertex buffers
	RenewVertexBuffers( nSZ, nSX, nEZ, nEX );	
}


//-----------------------------------------------------------------------------
// Name : FlattenTiles()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::FlattenTiles( int nSZ, int nSX, int nEZ, int nEX, float fStrength )
{
	unsigned char*	pHeights;
	float*			pTempHeights;
	int				nX, nZ;
	float			fDiff;
	float			fAverageHeight = 0.0f;
	int				nCount = 0;

	// Calculate a average height
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);
		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			fAverageHeight += *pTempHeights;
			nCount++;
			pTempHeights++;
		}
	}

	fAverageHeight /= nCount;

	// Flatten heights
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pHeights = &(m_pHeights[nSX+nZ*m_nHMWidth]);
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);
		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			fDiff = ( fAverageHeight - *pTempHeights ) * fStrength;
			*pTempHeights += fDiff;
			*pHeights = (unsigned char)(*pTempHeights);

			pHeights++;
			pTempHeights++;
		}
	}

	// Renew vertex buffers
	RenewVertexBuffers( nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : SmoothTiles()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::SmoothTiles( int nSZ, int nSX, int nEZ, int nEX, float fStrength )
{
	unsigned char*	pHeights;
	float*			pTempHeights;
	int				nX, nZ;
	float			fDiff;
	float			fAverageHeight;

	// Smooth heights
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pHeights = &(m_pHeights[nSX+nZ*m_nHMWidth]);
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);
		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			fAverageHeight = 0.0f;
			if( nX - 1 >= 0 ) fAverageHeight += m_pTempHeights[nX-1 + nZ*m_nHMWidth];
			else fAverageHeight += m_pTempHeights[nZ*m_nHMWidth];
			if( nX + 1 < m_nHMWidth ) fAverageHeight += m_pTempHeights[nX+1 + nZ*m_nHMWidth];
			else fAverageHeight += m_pTempHeights[m_nHMWidth-1 + nZ*m_nHMWidth];
			if( nZ - 1 >= 0 ) fAverageHeight += m_pTempHeights[nX + (nZ-1)*m_nHMWidth];
			else fAverageHeight += m_pTempHeights[nX];
			if( nZ + 1 < m_nHMWidth ) fAverageHeight += m_pTempHeights[nX + (nZ+1)*m_nHMWidth];
			else fAverageHeight += m_pTempHeights[nX + (m_nHMWidth-1)*m_nHMWidth];
			
			fAverageHeight /= 4;
			fDiff = ( fAverageHeight - *pTempHeights ) * fStrength;
			*pTempHeights += fDiff;
			*pHeights = (unsigned char)(*pTempHeights);

			pHeights++;
			pTempHeights++;
		}
	}

	// Renew vertex buffers
	RenewVertexBuffers( nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : RaiseLowerVertex()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RaiseLowerVertex( int nZ, int nX, int nOffset, float fStrength )
{
	// Modify one vertex
	m_pTempHeights[nX + nZ*m_nHMWidth] += nOffset * fStrength;
	if( m_pTempHeights[nX + nZ*m_nHMWidth] < 0.0f )
		m_pTempHeights[nX + nZ*m_nHMWidth] = 0.0f;
	else if( m_pTempHeights[nX + nZ*m_nHMWidth] > 255.0f )
		m_pTempHeights[nX + nZ*m_nHMWidth] = 255.0f;

	m_pHeights[nX + nZ*m_nHMWidth] = (unsigned char)m_pTempHeights[nX + nZ*m_nHMWidth];

	// Renew vertex buffers
	RenewVertexBuffers( nZ, nX, nZ, nX );
}


//-----------------------------------------------------------------------------
// Name : RaiseVertices()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RaiseVertices( int nSZ, int nSX, int nEZ, int nEX,
								float fZ, float fX, float fRadius, float fStrength )
{
	unsigned char*	pHeights;
	float*			pTempHeights;
	float			fSqrDistX, fSqrDistZ, fSqrDist;
	float			fDistRatio;
	int				nX, nZ;

	fRadius *= fRadius;
	// Raise vertices
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pHeights = &(m_pHeights[nSX+nZ*m_nHMWidth]);
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);

		fSqrDistZ = nZ * m_nPatchSize - fZ;
		fSqrDistZ *= fSqrDistZ;

		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			fSqrDistX = nX * m_nPatchSize - fX;
			fSqrDistX *= fSqrDistX;
			fSqrDist = fSqrDistX + fSqrDistZ;

			if( fSqrDist < NK_EPSILON3 )
				fDistRatio = 0.0f;
			else if( fSqrDist > fRadius )
				fDistRatio = 1.0f;
			else
				fDistRatio = fSqrDist / fRadius;

			*pTempHeights += cosf( fDistRatio * NK_HALFPI ) * fStrength;
			if( *pTempHeights > 255.0f )
				*pTempHeights = 255.0f;

			*pHeights = (unsigned char)(*pTempHeights);

			pHeights++;
			pTempHeights++;
		}
	}

	// Renew vertex buffers
	RenewVertexBuffers( nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : LowerVertices()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::LowerVertices( int nSZ, int nSX, int nEZ, int nEX,
								float fZ, float fX, float fRadius, float fStrength )
{
	unsigned char*	pHeights;
	float*			pTempHeights;
	float			fSqrDistX, fSqrDistZ, fSqrDist;
	float			fDistRatio;
	int				nX, nZ;

	fRadius *= fRadius;
	// Raise vertices
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pHeights = &(m_pHeights[nSX+nZ*m_nHMWidth]);
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);

		fSqrDistZ = nZ * m_nPatchSize - fZ;
		fSqrDistZ *= fSqrDistZ;

		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			fSqrDistX = nX * m_nPatchSize - fX;
			fSqrDistX *= fSqrDistX;
			fSqrDist = fSqrDistX + fSqrDistZ;

			if( fSqrDist < NK_EPSILON3 )
				fDistRatio = 0.0f;
			else if( fSqrDist > fRadius )
				fDistRatio = 1.0f;
			else
				fDistRatio = fSqrDist / fRadius;

			*pTempHeights -= cosf( fDistRatio * NK_HALFPI ) * fStrength;
			if( *pTempHeights < 0.0f )
				*pTempHeights = 0.0f;

			*pHeights = (unsigned char)(*pTempHeights);

			pHeights++;
			pTempHeights++;
		}
	}

	// Renew vertex buffers
	RenewVertexBuffers( nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : FlattenVertices()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::FlattenVertices( int nSZ, int nSX, int nEZ, int nEX,
									float fZ, float fX, float fRadius, float fStrength)
{
	unsigned char*	pHeights;
	float*			pTempHeights;
	int				nX, nZ;
	float			fSqrDistX, fSqrDistZ, fSqrDist;
	float			fDiff;
	float			fAverageHeight = 0.0f;
	int				nCount = 0;

	fRadius *= fRadius;

	// Calculate a average height
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{		
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);

		fSqrDistZ = nZ * m_nPatchSize - fZ;
		fSqrDistZ *= fSqrDistZ;

		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			fSqrDistX = nX * m_nPatchSize - fX;
			fSqrDistX *= fSqrDistX;
			fSqrDist = fSqrDistX + fSqrDistZ;

			if( fSqrDist <= fRadius )
			{
				fAverageHeight += *pTempHeights;
				nCount++;
			}

			pTempHeights++;
		}
	}

	fAverageHeight /= nCount;

	// Flatten vertices
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pHeights = &(m_pHeights[nSX+nZ*m_nHMWidth]);
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);

		fSqrDistZ = nZ * m_nPatchSize - fZ;
		fSqrDistZ *= fSqrDistZ;

		for( nX = nSX; nX <= nEX + 1; nX++ )
		{
			fSqrDistX = nX * m_nPatchSize - fX;
			fSqrDistX *= fSqrDistX;
			fSqrDist = fSqrDistX + fSqrDistZ;

			if( fSqrDist <= fRadius )
			{
				fDiff = ( fAverageHeight - *pTempHeights ) * fStrength;
				*pTempHeights += fDiff;
				*pHeights = (unsigned char)(*pTempHeights);
			}

			pHeights++;
			pTempHeights++;
		}
	}

	// Renew vertex buffers
	RenewVertexBuffers( nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : SmoothVertices()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::SmoothVertices( int nSZ, int nSX, int nEZ, int nEX,
									float fZ, float fX, float fRadius, float fStrength)
{
	unsigned char*	pHeights;
	float*			pTempHeights;
	int				nX, nZ;
	float			fSqrDistX, fSqrDistZ, fSqrDist;
	float			fDiff;
	float			fH0, fH1, fH2, fH3;
	float			fA0, fA1, fA2, fA3;
	float			fAverageHeight;

	fRadius *= fRadius;

	// Smooth vertices
	for( nZ = nSZ; nZ <= nEZ + 1; nZ++ )
	{
		pHeights = &(m_pHeights[nSX+nZ*m_nHMWidth]);
		pTempHeights = &(m_pTempHeights[nSX+nZ*m_nHMWidth]);

		fSqrDistZ = nZ * m_nPatchSize - fZ;
		fSqrDistZ *= fSqrDistZ;

		for( nX = nSX; nX <= nEX + 1; nX++ )
		{			
			fSqrDistX = nX * m_nPatchSize - fX;
			fSqrDistX *= fSqrDistX;
			fSqrDist = fSqrDistX + fSqrDistZ;

			if( fSqrDist <= fRadius )
			{
				fAverageHeight = 0.0f;
				if( nX - 2 >= 0 ) fH0 = m_pTempHeights[nX-2 + nZ*m_nHMWidth];
				else fH0 = m_pTempHeights[nZ*m_nHMWidth];
				if( nX - 1 >= 0 ) fH1 = m_pTempHeights[nX-1 + nZ*m_nHMWidth];
				else fH1 = m_pTempHeights[nZ*m_nHMWidth];
				if( nX + 1 < m_nHMWidth ) fH2 = m_pTempHeights[nX+1 + nZ*m_nHMWidth];
				else fH2 = m_pTempHeights[m_nHMWidth-1 + nZ*m_nHMWidth];
				if( nX + 2 < m_nHMWidth ) fH3 = m_pTempHeights[nX+2 + nZ*m_nHMWidth];
				else fH3 = m_pTempHeights[m_nHMWidth-1 + nZ*m_nHMWidth];

				fA0 = fH3 - fH2 - fH0 + fH1;
				fA1 = fH0 - fH1 - fA0;
				fA2 = fH2 - fH0;
				fA3 = fH1;
				fAverageHeight += fA0 * 0.125f + fA1 * 0.25f + fA2 * 0.5f + fA3;

				if( nZ - 2 >= 0 ) fH0 = m_pTempHeights[nX + (nZ-2)*m_nHMWidth];
				else fH0 = m_pTempHeights[nX];
				if( nZ - 1 >= 0 ) fH1 = m_pTempHeights[nX + (nZ-1)*m_nHMWidth];
				else fH1 = m_pTempHeights[nX];
				if( nZ + 1 < m_nHMWidth ) fH2 = m_pTempHeights[nX + (nZ+1)*m_nHMWidth];
				else fH2 = m_pTempHeights[nX + (m_nHMWidth-1)*m_nHMWidth];
				if( nZ + 2 < m_nHMWidth ) fH3 = m_pTempHeights[nX + (nZ+2)*m_nHMWidth];
				else fH3 = m_pTempHeights[nX + (m_nHMWidth-1)*m_nHMWidth];

				fA0 = fH3 - fH2 - fH0 + fH1;
				fA1 = fH0 - fH1 - fA0;
				fA2 = fH2 - fH0;
				fA3 = fH1;
				fAverageHeight += fA0 * 0.125f + fA1 * 0.25f + fA2 * 0.5f + fA3;

				fAverageHeight /= 2;
				fDiff = ( fAverageHeight - *pTempHeights ) * fStrength;
				*pTempHeights += fDiff;
				*pHeights = (unsigned char)(*pTempHeights);
			}

			pHeights++;
			pTempHeights++;
		}
	}

	// Renew vertex buffers
	RenewVertexBuffers( nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : RecalcBoundingBox()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RecalcBoundingBox( int nSZ, int nSX, int nEZ, int nEX )
{
	int	nX, nZ;
	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			m_aQuadNodes[nZ][nX]->RecalcBoundingBox( nZ, nX );
		}
	}
}


//-----------------------------------------------------------------------------
// Name : RenewVertexBuffer()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RenewVertexBuffers( int nSZ, int nSX, int nEZ, int nEX )
{	
	int	nWidth = 1 << m_nQuadTreeDepth;
	nSX = nSX - 1 < 0 ? 0 : nSX - 1;
	nEX = nEX + 1 > m_nHMWidth - 2 ? m_nHMWidth - 2 : nEX + 1;
	nSZ = nSZ - 1 < 0 ? 0 : nSZ - 1;
	nEZ = nEZ + 1 > m_nHMWidth - 2 ? m_nHMWidth - 2 : nEZ + 1;
	
	// Renew vertex buffer
	if( nSX < nWidth )
	{
		if( nSZ < nWidth )
			RenewVertexBuffer( 0, nSZ, nSX, nEZ, nEX );
		if( nEZ >= nWidth )
			RenewVertexBuffer( 2, nSZ, nSX, nEZ, nEX );
	}
	if( nEX >= nWidth )
	{
		if( nSZ < nWidth )
			RenewVertexBuffer( 1, nSZ, nSX, nEZ, nEX );
		if( nEZ >= nWidth )
			RenewVertexBuffer( 3, nSZ, nSX, nEZ, nEX );
	}

	// Recalculate bounding boxes of a terrain
	RecalcBoundingBox( nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : RenewVertexBuffer()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RenewVertexBuffer( int nIndex, int nSZ, int nSX, int nEZ, int nEX )
{
	int			nX, nZ;
	WORD		wVBIndex;
	NKVERTEXT2*	pVert;

	if( FAILED( m_aVB[nIndex]->Lock( DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (LPVOID*)(&pVert), NULL ) ) )
		return ;

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			wVBIndex = m_aQuadNodes[nZ][nX]->GetVBIndex();
			pVert[wVBIndex].y = m_pHeights[nX + nZ*m_nHMWidth] * m_nHeightScale;
			pVert[wVBIndex+1].y = m_pHeights[nX+1 + nZ*m_nHMWidth] * m_nHeightScale;
			pVert[wVBIndex+3].y = m_pHeights[nX + (nZ+1)*m_nHMWidth] * m_nHeightScale;
			pVert[wVBIndex+4].y = m_pHeights[nX+1 + (nZ+1)*m_nHMWidth] * m_nHeightScale;
		}
	}

	m_aVB[nIndex]->Unlock();
}


//-----------------------------------------------------------------------------
// Name : ExportHeightMap()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::ExportHeightMap( const TCHAR* szFilename )
{
	FILE*	fp;

	fp = fopen( szFilename, "wb" );
	if( fp == NULL )
		return false;

	int nCount = 0;
	for( int nZ = m_nHMWidth - 1; nZ >= 0; nZ-- )
		nCount += fwrite( &m_pHeights[nZ * m_nHMWidth], sizeof(unsigned char), m_nHMWidth, fp );
	fclose( fp );

	if( nCount != m_nHMWidth * m_nHMWidth )
		return false;

	return true;
}


//-----------------------------------------------------------------------------
// Name : SetWorldTime()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::SetWorldTime( int nWorldTime )
{
	int		nTimeID;
	bool	bUpdateLightMap = false;

	if( nWorldTime >= 12 && nWorldTime <= 16 )
		nTimeID = 0;
	else if( nWorldTime >=17 && nWorldTime <= 21 )
		nTimeID = 1;
	else if( nWorldTime >= 22 && nWorldTime <= 27 )
		nTimeID = 2;
	else if( nWorldTime >=2 && nWorldTime <= 6 )
		nTimeID = 4;
	else if( nWorldTime >= 7 && nWorldTime <= 11 )
		nTimeID = 5;
	else nTimeID = 3;

	if( nTimeID != m_nTimeID )
		bUpdateLightMap = true;

	m_nTimeID = nTimeID;

	if( bUpdateLightMap && m_aClrMapTexL2[0] )
	{
		UpdateClrMapTexture( 2, 0 );
		UpdateClrMapTexture( 3, 0 );
		
		if( m_nHMWidth == 257 )
		{
			UpdateClrMapTexture( 2, 1 );
			UpdateClrMapTexture( 3, 1 );
			UpdateClrMapTexture( 2, 2 );
			UpdateClrMapTexture( 3, 2 );
			UpdateClrMapTexture( 2, 3 );			
			UpdateClrMapTexture( 3, 3 );
		}
	}
}


//-----------------------------------------------------------------------------
// Name : MakeNormals()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::MakeNormals()
{
	SAFE_DELETE_ARRAY( m_pNormals );
	m_pNormals = new D3DXVECTOR3[m_nHMWidth*m_nHMWidth];

	ComputeNormals( 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );
}


//-----------------------------------------------------------------------------
// Name : ComputeNormals()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ComputeNormals( int nSZ, int nSX, int nEZ, int nEX )
{
	// Progress dialog
	BOOL	bShowProgress = g_pMyNKApp->GetShowProgress();
	int		nProgressOffset;
	if( bShowProgress )
	{
		g_pMyNKApp->SetProgressTitle( _T(" Compute Terrain Normals ") );
		g_pMyNKApp->SetProgressContent( _T("Compute normals of a terrain for simulating a directional light.") );
		g_pMyNKApp->SetProgressRange( 0, (nEX-nSX)*(nEZ-nSZ) );
		g_pMyNKApp->SetProgressPos( 0 );
		nProgressOffset = nEX - nSX;
	}

	// Compute normals
	int			nX, nZ;
	D3DXVECTOR3	vVector1;
	D3DXVECTOR3	vVector2;
	D3DXVECTOR3	vTempNormal;
	D3DXVECTOR3	vResultNormal;
	float		fY1, fY2, fY;

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			vResultNormal = D3DXVECTOR3( 0, 0, 0 );
			fY1 = m_pHeights[nX + nZ*m_nHMWidth];

			if( nX > 0 )
			{
				if( nZ > 0 )			// SW
				{
					fY2 = m_pHeights[nX + (nZ-1)*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector1 = D3DXVECTOR3( 0, fY, -m_nPatchSize );
					fY2 = m_pHeights[nX-1 + (nZ-1)*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector2 = D3DXVECTOR3( -m_nPatchSize, fY, -m_nPatchSize );
					D3DXVec3Cross( &vTempNormal, &vVector1, &vVector2 );
					D3DXVec3Normalize( &vTempNormal, &vTempNormal );
					D3DXVec3Add( &vResultNormal, &vResultNormal, &vTempNormal );

					fY2 = m_pHeights[nX-1 + nZ*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector1 = D3DXVECTOR3( -m_nPatchSize, fY, 0 );
					D3DXVec3Cross( &vTempNormal, &vVector1, &vVector2 );
					D3DXVec3Normalize( &vTempNormal, &vTempNormal );
					D3DXVec3Add( &vResultNormal, &vResultNormal, &vTempNormal );
				}
				if( nZ < m_nHMWidth - 1)		// NW
				{
					fY2 = m_pHeights[nX-1 + nZ*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector1 = D3DXVECTOR3( -m_nPatchSize, fY, 0 );
					fY2 = m_pHeights[nX + (nZ+1)*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector2 = D3DXVECTOR3( 0, fY, m_nPatchSize );
					D3DXVec3Cross( &vTempNormal, &vVector1, &vVector2 );
					D3DXVec3Normalize( &vTempNormal, &vTempNormal );
					D3DXVec3Add( &vResultNormal, &vResultNormal, &vTempNormal );
				}
			}
			if( nX < m_nHMWidth - 1 )
			{
				if( nZ < m_nHMWidth - 1 )		// NE
				{
					fY2 = m_pHeights[nX + (nZ+1)*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector1 = D3DXVECTOR3( 0, fY, m_nPatchSize );
					fY2 = m_pHeights[nX+1 + (nZ+1)*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector2 = D3DXVECTOR3( m_nPatchSize, fY, m_nPatchSize );
					D3DXVec3Cross( &vTempNormal, &vVector1, &vVector2 );
					D3DXVec3Normalize( &vTempNormal, &vTempNormal );
					D3DXVec3Add( &vResultNormal, &vResultNormal, &vTempNormal );

					fY2 = m_pHeights[nX+1 + nZ*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector1 = D3DXVECTOR3( m_nPatchSize, fY, 0 );
					D3DXVec3Cross( &vTempNormal, &vVector1, &vVector2 );
					D3DXVec3Normalize( &vTempNormal, &vTempNormal );
					D3DXVec3Add( &vResultNormal, &vResultNormal, &vTempNormal );
				}
				if( nZ > 0 )				// SE
				{
					fY2 = m_pHeights[nX+1 + nZ*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector1 = D3DXVECTOR3( m_nPatchSize, fY, 0 );
					fY2 = m_pHeights[nX + (nZ-1)*m_nHMWidth];
					fY = ( fY2 - fY1 ) * m_nHeightScale;
					vVector2 = D3DXVECTOR3( 0, fY, -m_nPatchSize );
					D3DXVec3Cross( &vTempNormal, &vVector1, &vVector2 );
					D3DXVec3Normalize( &vTempNormal, &vTempNormal );
					D3DXVec3Add( &vResultNormal, &vResultNormal, &vTempNormal );
				}
			}

			D3DXVec3Normalize( &vResultNormal, &vResultNormal );
			m_pNormals[nX + nZ*m_nHMWidth] = vResultNormal;
		}
		
		if( bShowProgress )
			g_pMyNKApp->SetProgressOffsetPos( nProgressOffset );
	}
}


//-----------------------------------------------------------------------------
// Name : MakeAllRGBMaps()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::MakeAllRGBMaps()
{	
	if( g_pMyNKApp->GetShowProgress() )
	{
		// Directional light
		g_pMyNKApp->SetProgressTitle( _T(" Simulate Directional Light ") );
		g_pMyNKApp->SetProgressRange( 0, (m_nHMWidth-1)*(m_nHMWidth-1)*6 );
		g_pMyNKApp->SetProgressPos( 0 );		
		for( int i = 0; i < 6; i++ )
			SimulateDirectionalLight( i, 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );

		// Blur light maps
		g_pMyNKApp->SetProgressTitle( _T(" Blur Light Maps ") );
		g_pMyNKApp->SetProgressRange( 0, ((m_nHMWidth-1)*2-1)*((m_nHMWidth-1)*2-1)*6 );
		g_pMyNKApp->SetProgressPos( 0 );
		BlurAllLightMaps( 0, 0, (m_nHMWidth-1)*2-1, (m_nHMWidth-1)*2-1 );

		// Apply Shadow maps
		g_pMyNKApp->SetProgressTitle( _T(" Apply Shadow Maps ") );
		g_pMyNKApp->SetProgressRange( 0, ((m_nHMWidth-1)*2-1)*((m_nHMWidth-1)*2-1)*6 );
		g_pMyNKApp->SetProgressPos( 0 );
		ApplyAllShadowMaps( 0, 0, (m_nHMWidth-1)*2-1, (m_nHMWidth-1)*2-1 );
	}
	else
	{
		// Directional light
		for( int i = 0; i < 6; i++ )
			SimulateDirectionalLight( i, 0, 0, m_nHMWidth - 1, m_nHMWidth - 1 );

		// Blur light maps
		BlurAllLightMaps( 0, 0, (m_nHMWidth-1)*2-1, (m_nHMWidth-1)*2-1 );

		// Apply Shadow maps
		ApplyAllShadowMaps( 0, 0, (m_nHMWidth-1)*2-1, (m_nHMWidth-1)*2-1 );
	}

	// Eliminate boundary lines between textures
	if( m_nHMWidth == 257 )
	{
	}
}


//-----------------------------------------------------------------------------
// Name : RenewAllRGBMaps()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RenewAllRGBMaps( int nSZ, int nSX, int nEZ, int nEX )
{
	nSX = nSX - 1 < 0 ? 0 : nSX - 1;
	nEX = nEX + 1 > m_nHMWidth - 1 ? m_nHMWidth - 1 : nEX + 1;
	nSZ = nSZ - 1 < 0 ? 0 : nSZ - 1;
	nEZ = nEZ + 1 > m_nHMWidth - 1 ? m_nHMWidth - 1 : nEZ + 1;

	// Directional light
	for( int i = 0; i < 6; i++ )
		SimulateDirectionalLight( i, nSZ, nSX, nEZ, nEX );

	// Blur light maps
	BlurAllLightMaps( nSZ*2, nSX*2, nEZ*2-1, nEX*2-1 );

	// Apply shadow maps
	ApplyAllShadowMaps( nSZ*2, nSX*2, nEZ*2-1, nEX*2-1 );

	// Eliminate boundary lines between textures
	if( m_nHMWidth == 257 )
	{
	}
}


//-----------------------------------------------------------------------------
// Name : RenewRGBMap()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RenewRGBMap( int nIndex, int nSZ, int nSX, int nEZ, int nEX )
{
	nSX = nSX - 1 < 0 ? 0 : nSX - 1;
	nEX = nEX + 1 > m_nHMWidth - 1 ? m_nHMWidth - 1 : nEX + 1;
	nSZ = nSZ - 1 < 0 ? 0 : nSZ - 1;
	nEZ = nEZ + 1 > m_nHMWidth - 1 ? m_nHMWidth - 1 : nEZ + 1;

	// Directional light
	SimulateDirectionalLight( nIndex, nSZ, nSX, nEZ, nEX );

	// Blur light maps
	BlurLightMap( nIndex, nSZ*2, nSX*2, nEZ*2-1, nEX*2-1 );

	// Apply shadow maps
	ApplyShadowMaps( nIndex, nSZ*2, nSX*2, nEZ*2-1, nEX*2-1 );

	// Eliminate boundary lines between textures
	if( m_nHMWidth == 257 )
	{
	}
}


//-----------------------------------------------------------------------------
// Name : SimulateDirectionalLight()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::SimulateDirectionalLight( int nIndex, int nSZ, int nSX, int nEZ, int nEX )
{
	// Progress dialog
	BOOL	bShowProgress = g_pMyNKApp->GetShowProgress();
	int		nProgressOffset;
	if( bShowProgress )
	{
		TCHAR	szTemp[128];
		_stprintf( szTemp, _T("Simulate a directional light of a terrain.\n( %d / 6 )"), nIndex+1 );
		g_pMyNKApp->SetProgressContent( szTemp );
		nProgressOffset = nEX - nSX;
	}

	// Simulate a directional light
	int			nX, nZ, nCX, nCZ;
	float		fFactor;
	float		fR, fG, fB;
	D3DXVECTOR3	vDirection = -m_aDirectLightDir[nIndex];

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			fFactor = D3DXVec3Dot( &m_pNormals[nX + nZ*m_nHMWidth], &vDirection );
			if( fFactor < 0.0f ) fFactor = 0.0f;

			fR = m_aDirectLightR[nIndex] * fFactor + m_aAmbientLightR[nIndex];
			if( fR > 1.0f )	fR = 1.0f;
			fG = m_aDirectLightG[nIndex] * fFactor + m_aAmbientLightG[nIndex];
			if( fG > 1.0f )	fG = 1.0f;
			fB = m_aDirectLightB[nIndex] * fFactor + m_aAmbientLightB[nIndex];
			if( fB > 1.0f )	fB = 1.0f;

			nCX = nX << 1;
			nCZ = nZ << 1;

			if( nX > 0 )
			{
				if( nZ > 0 )
				{
					m_aLightR[nIndex][nCZ-1][nCX-1] = fR;
					m_aLightG[nIndex][nCZ-1][nCX-1] = fG;
					m_aLightB[nIndex][nCZ-1][nCX-1] = fB;
				}
				if( nZ < m_nHMWidth - 1 )
				{
					m_aLightR[nIndex][nCZ][nCX-1] = fR;
					m_aLightG[nIndex][nCZ][nCX-1] = fG;
					m_aLightB[nIndex][nCZ][nCX-1] = fB;
				}
			}
			if( nX < m_nHMWidth - 1 )
			{
				if( nZ < m_nHMWidth - 1 )
				{
					m_aLightR[nIndex][nCZ][nCX] = fR;
					m_aLightG[nIndex][nCZ][nCX] = fG;
					m_aLightB[nIndex][nCZ][nCX] = fB;
				}
				if( nZ > 0 )
				{
					m_aLightR[nIndex][nCZ-1][nCX] = fR;
					m_aLightG[nIndex][nCZ-1][nCX] = fG;
					m_aLightB[nIndex][nCZ-1][nCX] = fB;
				}
			}
		}

		if( bShowProgress )
			g_pMyNKApp->SetProgressOffsetPos( nProgressOffset );
	}
}


//-----------------------------------------------------------------------------
// Name : BlurAllLightMaps()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::BlurAllLightMaps( int nSZ, int nSX, int nEZ, int nEX )
{
	for( int i = 0; i < 6; i++ )
		BlurLightMap( i, nSZ, nSX, nEZ, nEX );	
}


//-----------------------------------------------------------------------------
// Name : BlurLightMap()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::BlurLightMap( int nIndex, int nSZ, int nSX, int nEZ, int nEX )
{
	// Progress dialog
	BOOL	bShowProgress = g_pMyNKApp->GetShowProgress();
	int		nProgressOffset;
	if( bShowProgress )
	{
		TCHAR	szTemp[128];
		_stprintf( szTemp, _T("Blur all regions of Light maps.\n( %d / 6 )"), nIndex+1 );
		g_pMyNKApp->SetProgressContent( szTemp );
		nProgressOffset = nEX - nSX;
	}

	// Blur
	int			nX, nZ, nCount;
	int			x, z, sx, sz, ex, ez;
	float		fR, fG, fB;
	int			nWidth = ( m_nHMWidth - 1 ) * 2 - 1;

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			sx = nX - 2 < 0 ? 0 : nX - 2;
			ex = nX + 2 > nWidth ? nWidth : nX + 2;
			sz = nZ - 2 < 0 ? 0 : nZ - 2;
			ez = nZ + 2 > nWidth ? nWidth : nZ + 2;

			nCount = 0;
			fR = 0.0f;		fG = 0.0f;		fB = 0.0f;
			for( z = sz; z <= ez; z++ )
			{
				for( x = sx; x <= ex; x++ )
				{
					fR += m_aLightR[nIndex][z][x];
					fG += m_aLightG[nIndex][z][x];
					fB += m_aLightB[nIndex][z][x];
					nCount++;
				}
			}
			m_aLightR[nIndex][nZ][nX] = fR / nCount;
			m_aLightG[nIndex][nZ][nX] = fG / nCount;
			m_aLightB[nIndex][nZ][nX] = fB / nCount;
		}
		if( bShowProgress )
			g_pMyNKApp->SetProgressOffsetPos( nProgressOffset );
	}

	// Erase boundary lines between textures
	if( m_nHMWidth == 257 )
	{
		if( nSX <= 256 || nEX >= 255 )
		{
			nSX = nSX - 1 < 0 ? 1 : nSX;
			nEX = nEX + 1 > 511 ? 510 : nEZ;
			fR = 0.0f;		fG = 0.0f;		fB = 0.0f;
			for( nX = nSX; nX <= nEX; nX++ )
			{
				fR = m_aLightR[nIndex][255][nX-1] + m_aLightR[nIndex][255][nX] + m_aLightR[nIndex][255][nX+1] +
						m_aLightR[nIndex][256][nX-1] + m_aLightR[nIndex][256][nX] + m_aLightR[nIndex][256][nX+1];
				fG = m_aLightG[nIndex][255][nX-1] + m_aLightG[nIndex][255][nX] + m_aLightG[nIndex][255][nX+1] +
						m_aLightG[nIndex][256][nX-1] + m_aLightG[nIndex][256][nX] + m_aLightG[nIndex][256][nX+1];
				fB = m_aLightB[nIndex][255][nX-1] + m_aLightB[nIndex][255][nX] + m_aLightB[nIndex][255][nX+1] +
						m_aLightB[nIndex][256][nX-1] + m_aLightB[nIndex][256][nX] + m_aLightB[nIndex][256][nX+1];
				fR /= 6;	fG /= 6;	fB /= 6;
				m_aLightR[nIndex][255][nX] = fR;	m_aLightR[nIndex][256][nX] = fR;
				m_aLightG[nIndex][255][nX] = fG;	m_aLightG[nIndex][256][nX] = fG;
				m_aLightB[nIndex][255][nX] = fB;	m_aLightB[nIndex][256][nX] = fB;
			}
		}
		if( nSZ <= 256 || nEZ >= 255 )
		{
			nSZ = nSZ - 1 < 0 ? 1 : nSZ;
			nEZ = nEZ + 1 > 511 ? 510 : nEZ;
			fR = 0.0f;		fG = 0.0f;		fB = 0.0f;
			for( nZ = nSZ; nZ <= nEZ; nZ++ )
			{
				fR = m_aLightR[nIndex][nZ-1][255] + m_aLightR[nIndex][nZ][255] + m_aLightR[nIndex][nZ+1][255] +
						m_aLightR[nIndex][nZ-1][256] + m_aLightR[nIndex][nZ][256] + m_aLightR[nIndex][nZ+1][256];
				fG = m_aLightG[nIndex][nZ-1][255] + m_aLightG[nIndex][nZ][255] + m_aLightG[nIndex][nZ+1][255] +
						m_aLightG[nIndex][nZ-1][256] + m_aLightG[nIndex][nZ][256] + m_aLightG[nIndex][nZ+1][256];
				fB = m_aLightB[nIndex][nZ-1][255] + m_aLightB[nIndex][nZ][255] + m_aLightB[nIndex][nZ+1][255] +
						m_aLightB[nIndex][nZ-1][256] + m_aLightB[nIndex][nZ][256] + m_aLightB[nIndex][nZ+1][256];
				fR /= 6;	fG /= 6;	fB /= 6;
				m_aLightR[nIndex][nZ][255] = fR;	m_aLightR[nIndex][nZ][256] = fR;
				m_aLightG[nIndex][nZ][255] = fG;	m_aLightG[nIndex][nZ][256] = fG;
				m_aLightB[nIndex][nZ][255] = fB;	m_aLightB[nIndex][nZ][256] = fB;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name : ApplyAllShadowMaps()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ApplyAllShadowMaps( int nSZ, int nSX, int nEZ, int nEX )
{
	for( int i = 0; i < 6; i++ )
		ApplyShadowMaps( i, nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : ApplyShadowMaps()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ApplyShadowMaps( int nIndex, int nSZ, int nSX, int nEZ, int nEX )
{
	// Progress dialog
	BOOL	bShowProgress = g_pMyNKApp->GetShowProgress();
	int		nProgressOffset;
	if( bShowProgress )
	{
		TCHAR	szTemp[128];
		_stprintf( szTemp, _T("Apply the shadow to RGB maps.\n( %d / 6 )"), nIndex+1 );
		g_pMyNKApp->SetProgressContent( szTemp );
		nProgressOffset = nEX - nSX;
	}

	int		nX, nZ;
	DWORD	dwR, dwG, dwB;

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			dwR = 255 * ( m_aLightR[nIndex][nZ][nX] * m_aShadowR[nIndex][nZ][nX] );
			dwG = 255 * ( m_aLightG[nIndex][nZ][nX] * m_aShadowG[nIndex][nZ][nX] );
			dwB = 255 * ( m_aLightB[nIndex][nZ][nX] * m_aShadowB[nIndex][nZ][nX] );
			m_aRGBMap[nIndex][nZ][nX] = ( dwR << 16 ) | ( dwG << 8 ) | dwB;
		}
		if( bShowProgress )
			g_pMyNKApp->SetProgressOffsetPos( nProgressOffset );
	}
}


//-----------------------------------------------------------------------------
// Name : UpdateRGBMapsByShadow()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::UpdateRGBMapsByShadow( BOOL bCurrentShadowMap, int nSZ, int nSX, int nEZ, int nEX )
{
	nSX <<= 1;		nEX = ( nEX << 1 ) + 1;
	nSZ <<= 1;		nEZ = ( nEZ << 1 ) + 1;

	// Apply shadow maps
	if( bCurrentShadowMap )
		ApplyShadowMaps( m_nTimeID, nSZ, nSX, nEZ, nEX );
	else
		ApplyAllShadowMaps( nSZ, nSX, nEZ, nEX );
		

	// Update color map textures
	int	nWidth = 1 << m_nQuadTreeDepth;
	if( nSX < nWidth )
	{
		if( nSZ < nWidth )
		{
			UpdateClrMapTexture( 2, 0 );
			UpdateClrMapTexture( 3, 0 );
		}
		if( nEZ >= nWidth )
		{
			UpdateClrMapTexture( 2, 2 );
			UpdateClrMapTexture( 3, 2 );
		}
	}
	if( nEX >= nWidth )
	{
		if( nSZ < nWidth )
		{
			UpdateClrMapTexture( 2, 1 );
			UpdateClrMapTexture( 3, 1 );
		}
		if( nEZ >= nWidth )
		{
			UpdateClrMapTexture( 2, 3 );
			UpdateClrMapTexture( 3, 3 );
		}
	}
}


//-----------------------------------------------------------------------------
// Name : UpdateRGBMapsByVertex()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::UpdateRGBMapsByVertex( int nSZ, int nSX, int nEZ, int nEX )
{
	int	nWidth = 1 << m_nQuadTreeDepth;
	nSX = nSX - 1 < 0 ? 0 : nSX - 1;
	nEX = nEX + 2 > m_nHMWidth - 1 ? m_nHMWidth - 1 : nEX + 2;
	nSZ = nSZ - 1 < 0 ? 0 : nSZ - 1;
	nEZ = nEZ + 2 > m_nHMWidth - 1 ? m_nHMWidth - 1 : nEZ + 2;

	// Recalculate normals
	ComputeNormals( nSZ, nSX, nEZ, nEX );

	// Renew all rgb maps
	RenewAllRGBMaps( nSZ, nSX, nEZ, nEX );

	// Update color map textures
	if( nSX < nWidth )
	{
		if( nSZ < nWidth )
		{
			UpdateClrMapTexture( 2, 0 );
			UpdateClrMapTexture( 3, 0 );
		}
		if( nEZ >= nWidth )
		{
			UpdateClrMapTexture( 2, 2 );
			UpdateClrMapTexture( 3, 2 );
		}
	}
	if( nEX >= nWidth )
	{
		if( nSZ < nWidth )
		{
			UpdateClrMapTexture( 2, 1 );
			UpdateClrMapTexture( 3, 1 );
		}
		if( nEZ >= nWidth )
		{
			UpdateClrMapTexture( 2, 3 );
			UpdateClrMapTexture( 3, 3 );
		}
	}
}


//-----------------------------------------------------------------------------
// Name : GetMapIndex()
// Desc :
//-----------------------------------------------------------------------------
int CNKTerrain::GetMapIndex( D3DXVECTOR3 vCoord )
{
	int	nTileX = vCoord.x / m_nPatchSize;
	int	nTileZ = vCoord.z / m_nPatchSize;

	if( nTileX < 0 || nTileX >= m_nHMWidth-1 || nTileZ < 0 || nTileZ >= m_nHMWidth-1 )
		return -1;

	return nTileX + nTileZ*(m_nHMWidth-1);
}


//-----------------------------------------------------------------------------
// Name : OptimizeTerrainMap()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::OptimizeTerrainMap()
{
	int		nX, nZ;
	int		x, z, sx, ex, sz, ez;
	bool	bOptimize;
	int		nTexID;
	int		nTileWidth = m_nHMWidth - 1;
	int		nWidth = ( m_nHMWidth - 1 ) * 2 - 1;

	// Erase the tile which alpha channels are 0
	for( nZ = 0; nZ < nTileWidth; nZ++ )
	{
		for( nX = 0; nX < nTileWidth; nX++ )
		{
			sx = nX*2-1 < 0 ? 0 : nX*2-1;
			ex = nX*2+2 > nWidth ? nWidth : nX*2+2;
			sz = nZ*2-1 < 0 ? 0 : nZ*2-1;
			ez = nZ*2+2 > nWidth ? nWidth : nZ*2+2;

			bOptimize = true;
			for( z = sz; z <= ez; z++ )
			{
				for( x = sx; x <= ex; x++ )
				{
					if( m_aAlphaMapL2[z][x] > 0 )
					{
						bOptimize = false;
						break;
					}
				}
			}

			if( bOptimize )
				m_aQuadNodes[nZ][nX]->SetL2TexID( TEXTURE_MAX_COUNT );
				//m_aQuadNodes[nZ][nX]->SetL2TexID( 15 );

			bOptimize = true;
			for( z = sz; z <= ez; z++ )
			{
				for( x = sx; x <= ex; x++ )
				{
					if( m_aAlphaMapL3[z][x] > 0 )
					{
						bOptimize = false;
						break;
					}
				}
			}

			if( bOptimize )
				m_aQuadNodes[nZ][nX]->SetL3TexID( TEXTURE_MAX_COUNT );
				//m_aQuadNodes[nZ][nX]->SetL3TexID( 15 );
		}
	}

	// Replace the tile which alpha channels are 255 to the tile of the layer 1
	for( nZ = 0; nZ < nTileWidth; nZ++ )
	{
		for( nX = 0; nX < nTileWidth; nX++ )
		{
			sx = nX*2-1 < 0 ? 0 : nX*2-1;
			ex = nX*2+2 > nWidth ? nWidth : nX*2+2;
			sz = nZ*2-1 < 0 ? 0 : nZ*2-1;
			ez = nZ*2+2 > nWidth ? nWidth : nZ*2+2;

			bOptimize = true;
			for( z = sz; z <= ez; z++ )
			{
				for( x = sx; x <= ex; x++ )
				{
					if( m_aAlphaMapL3[z][x] < 255 )
					{
						bOptimize = false;
						break;
					}
				}
			}

			if( bOptimize )
			{
				nTexID = m_aQuadNodes[nZ][nX]->GetL3TexID();

				if( nTexID != TEXTURE_MAX_COUNT )
				{
					m_aQuadNodes[nZ][nX]->SetL3TexID( TEXTURE_MAX_COUNT );
					m_aQuadNodes[nZ][nX]->SetL2TexID( TEXTURE_MAX_COUNT );
					m_aQuadNodes[nZ][nX]->SetL1TexID( nTexID );
				}
			}
			else
			{
				bOptimize = true;
				for( z = sz; z <= ez; z++ )
				{
					for( x = sx; x <= ex; x++ )
					{
						if( m_aAlphaMapL2[z][x] < 255 )
						{
							bOptimize = false;
							break;
						}
					}
				}
			
				if( bOptimize )
				{
					nTexID = m_aQuadNodes[nZ][nX]->GetL2TexID();
					if( nTexID != TEXTURE_MAX_COUNT )
					{
						m_aQuadNodes[nZ][nX]->SetL2TexID( TEXTURE_MAX_COUNT );
						m_aQuadNodes[nZ][nX]->SetL1TexID( nTexID );
					}
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name : ImportShadowMap()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::ImportShadowMap( int nIndex, const TCHAR* szFilename )
{
	// Load bitmap
	HBITMAP	hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage( NULL, szFilename, IMAGE_BITMAP, 0, 0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	
	if( hBitmap )
	{
		BITMAP	bm;
		GetObject( hBitmap, sizeof(BITMAP), &bm );

		// Check bit per pixel
		if( bm.bmBitsPixel != 24 )
		{
			MessageBox( NULL, _T("Bit per pixel이 24이어야 합니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
			DeleteObject( hBitmap );
			return false;
		}

		// Check width/height
		DWORD	dwWidth = ( m_nHMWidth - 1 ) * 2;
		if( bm.bmWidth != dwWidth || bm.bmHeight != dwWidth )
		{
			TCHAR	szTemp[128];
			_stprintf( szTemp, _T("Width/Height가 %d이어야 합니다."), dwWidth );
			MessageBox( NULL, szTemp, _T("NKMapTool"), MB_ICONWARNING | MB_OK );
			DeleteObject( hBitmap );
			return false;
		}

		// Allocate memory
		BYTE*	pBits = new BYTE[sizeof(BYTE) * 3 * dwWidth * dwWidth];
		if( pBits == NULL )
		{
			MessageBox( NULL, _T("메모리 할당에 실패했습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
			DeleteObject( hBitmap );
			return false;
		}

		// Store bit data
		DWORD	dwX, dwY;
		BYTE*	pSrcBits = (BYTE*)bm.bmBits;
		BYTE*	pDestBits = pBits;
		for( dwY = 0; dwY < dwWidth; dwY++ )
		{
			memcpy( pDestBits, pSrcBits, sizeof(BYTE) * 3 * dwWidth ); 
			pSrcBits += bm.bmWidthBytes;
			pDestBits += dwWidth * 3;
		}

		// Store shadow values
		pSrcBits = pBits;
		for( dwY = 0; dwY < dwWidth; dwY++ )
		{
			for( dwX = 0; dwX < dwWidth; dwX++ )
			{
				m_aShadowB[nIndex][dwY][dwX] = *( pSrcBits ) / 255.0f;
				m_aShadowG[nIndex][dwY][dwX] = *( pSrcBits + 1 ) / 255.0f;
				m_aShadowR[nIndex][dwY][dwX] = *( pSrcBits + 2 ) / 255.0f;
				pSrcBits += 3;
			}
		}

		DeleteObject( hBitmap );
		SAFE_DELETE_ARRAY( pBits );

		// Update shadow map
		ApplyShadowMaps( nIndex, 0, 0, dwWidth, dwWidth );

		// Update color map textures
		UpdateClrMapTexture( 2, 0 );
		UpdateClrMapTexture( 3, 0 );
		if( m_nHMWidth == 257 )
		{
			UpdateClrMapTexture( 2, 1 );
			UpdateClrMapTexture( 2, 2 );
			UpdateClrMapTexture( 2, 3 );
			UpdateClrMapTexture( 3, 1 );
			UpdateClrMapTexture( 3, 2 );		
			UpdateClrMapTexture( 3, 3 );
		}

		return true;
	}

	MessageBox( NULL, _T("파일을 열 수 없습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
	return false;
}


//-----------------------------------------------------------------------------
// Name : ExportShadowMap()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::ExportShadowMap( int nIndex, const TCHAR* szFilename )
{
	// Set headers
	BITMAPFILEHEADER	bmfHeader;
	bmfHeader.bfType = 0x4d42;
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER);
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);

	DWORD		dwWidth = ( m_nHMWidth - 1 ) * 2;
	BITMAPINFO	bmInfo;
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmInfo.bmiHeader.biWidth = dwWidth;
	bmInfo.bmiHeader.biHeight = dwWidth;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 24;
	bmInfo.bmiHeader.biCompression = BI_RGB;
	bmInfo.bmiHeader.biSizeImage = dwWidth * dwWidth * 3;
	bmInfo.bmiHeader.biXPelsPerMeter = 0;
	bmInfo.bmiHeader.biYPelsPerMeter = 0;
	bmInfo.bmiHeader.biClrUsed = 0;
	bmInfo.bmiHeader.biClrImportant = 0;

	// Allocate memory
	BYTE*	pBits = new BYTE[sizeof(BYTE) * 3 * dwWidth * dwWidth];
	if( pBits == NULL )
	{
		MessageBox( NULL, _T("메모리 할당에 실패했습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return false;
	}

	// Get shadow values
	BYTE*	pSrcBits = pBits;
	DWORD	dwX, dwY;
	for( dwY = 0; dwY < dwWidth; dwY++ )
	{
		for( dwX = 0; dwX < dwWidth; dwX++ )
		{
			*( pSrcBits ) = m_aShadowB[nIndex][dwY][dwX] * 255;
			*( pSrcBits + 1 ) = m_aShadowG[nIndex][dwY][dwX] * 255;
			*( pSrcBits + 2 ) = m_aShadowR[nIndex][dwY][dwX] * 255;
			pSrcBits += 3;
		}
	}

	// Write bitmap
	FILE*	fp = fopen( szFilename, "wb" );
	if( fp == NULL )
	{
		MessageBox( NULL, _T("파일을 열 수 없습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return false;
	}

	fwrite( &bmfHeader, sizeof(BITMAPFILEHEADER), 1, fp );
	fwrite( &bmInfo, sizeof(BITMAPINFO), 1, fp );
	fwrite( pBits, sizeof(BYTE), 3 * dwWidth * dwWidth, fp );

	SAFE_DELETE_ARRAY( pBits );
	fclose( fp );

	return true;
}


//-----------------------------------------------------------------------------
// Name : SetMapAttrColor()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::SetMapAttrColor( int nIndex, COLORREF crColor )
{
	DWORD	dwColor = ( ( crColor & 0x000000ff ) << 16 ) | ( crColor & 0x0000ff00 ) |
						( ( crColor & 0x00ff0000 ) >> 16 );
	m_aMapAttrColor[nIndex] = dwColor;
}


//-----------------------------------------------------------------------------
// Name : ImportServerAttributeMap()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::ImportServerAttributeMap( const TCHAR* szFilename )
{
	char szFileName[ 1000 ];
	strcpy( szFileName, szFilename );

	if( m_pAttributeMap )
	{
		if( MessageBox( NULL, _T("Attribute Map이 존재합니다.\n계속하시겠습니까?"),
						_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDNO )
			return false;
	}

	// Width of the attribute map
	int	nAttrMapWidth = ( m_nHMWidth - 1 ) * ( m_nPatchSize / 10 );

	// Alocate memory
	unsigned char* pTempAttrMap = new unsigned char[nAttrMapWidth * nAttrMapWidth];

	// Read the attribute map
	FILE*	fp = fopen( szFileName, "rb" );
	if( fp == NULL )
	{
		MessageBox( NULL, _T("파일을 열 수 없습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		SAFE_DELETE_ARRAY( pTempAttrMap );
		return false;
	}

	int	nSize = nAttrMapWidth * nAttrMapWidth;
	if( fread( pTempAttrMap, sizeof(unsigned char), nSize, fp ) != nSize )
	{
		MessageBox( NULL, _T("Attribute Map 읽기에 실패했습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		SAFE_DELETE_ARRAY( pTempAttrMap );
		return false;
	}

	fclose( fp );

	// Copy the attribute map
	m_nAttributeMapWidth = nAttrMapWidth;
	SAFE_DELETE_ARRAY( m_pAttributeMap );
	m_pAttributeMap = new unsigned char[nSize];
	memcpy( m_pAttributeMap, pTempAttrMap, sizeof(unsigned char) * nSize );
	SAFE_DELETE_ARRAY( pTempAttrMap );

	return true;
}


//-----------------------------------------------------------------------------
// Name : ExportServerAttributeMap()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::ExportServerAttributeMap( const TCHAR* szFilename )
{
	if( m_pAttributeMap == NULL )
	{
		MessageBox( NULL, _T("Attribute Map이 존재하지 않습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return;
	}

	FILE*	fp = fopen( szFilename, "wb" );
	if( fp == NULL )
	{
		MessageBox( NULL, _T("파일을 열 수 없습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return;
	}

	int	nSize = m_nAttributeMapWidth * m_nAttributeMapWidth;
	if( fwrite( m_pAttributeMap, sizeof(unsigned char), nSize, fp ) != nSize )
	{
		MessageBox( NULL, _T("Attribute Map 저장에 실패했습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return;
	}

	fclose( fp );

	ExportServerAttributeMapToBmp(szFilename);
}

struct SPixel
{
	BYTE r;
	BYTE g;
	BYTE b;
};

void CNKTerrain::ExportServerAttributeMapToBmp( const std::string& szFilename )
{
	std::string bmpFilename = szFilename;
	bmpFilename += ".bmp";

	BITMAPFILEHEADER bmFileHeader;
	BITMAPINFOHEADER bmInfoHeader;

	// BMP 헤더
	bmFileHeader.bfSize  = sizeof(BITMAPFILEHEADER);
	bmFileHeader.bfType  = 0x4D42;
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	bmInfoHeader.biSize    = sizeof(BITMAPINFOHEADER);
	bmInfoHeader.biWidth   = m_nAttributeMapWidth; // 너비
	bmInfoHeader.biHeight   = m_nAttributeMapWidth; // 높이
	bmInfoHeader.biPlanes   = 1;
	bmInfoHeader.biBitCount   = 24; // 색상 비트
	bmInfoHeader.biCompression = 0;
	bmInfoHeader.biSizeImage = bmInfoHeader.biWidth * bmInfoHeader.biHeight * (bmInfoHeader.biBitCount/8);

	bmInfoHeader.biXPelsPerMeter = 0;
	bmInfoHeader.biYPelsPerMeter = 0;
	bmInfoHeader.biClrUsed = 0;
	bmInfoHeader.biClrImportant = 0;

	int size = m_nAttributeMapWidth * m_nAttributeMapWidth;
	// BMP는 아래에서 위로 저장되는 형식이기 때문에 순서대로 저장하면 뒤집히므로 파일로 저장할 데이터를 뒤집기
	SPixel* rgbTemp = new SPixel[size]; // 파일저장할 데이터를 보관할 임시공간
	int r, g, b;
	for(int i=0; i<size; i++) 
	{
		if( m_pAttributeMap[i] == g_nABLETOWALK )
		{
			r = 0; g = 0; b = 0;
		}
		else if( m_pAttributeMap[i] = g_nUNABLETOWALK )
		{
			r = 255; g = 255; b = 255;
		}
		else if( m_pAttributeMap[i] = g_nWATERAREA )
		{
			r = 123; g = 121; b = 123;
		}
		else
		{
			r = 0; g = 0; b = 0;
		}

		rgbTemp[i].r = r;
		rgbTemp[i].g = g;
		rgbTemp[i].b = b;
// 		rgbTemp[i].r = g_rgb[BUFSIZE-1-i].r;
// 		rgbTemp[i].g = g_rgb[BUFSIZE-1-i].g;
// 		rgbTemp[i].b = g_rgb[BUFSIZE-1-i].b;
	}

	// 파일로 저장
	CFile file;
	file.Open(bmpFilename.c_str(), CFile::modeNoTruncate | CFile::modeCreate | CFile::modeReadWrite );
	file.Write(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	file.Write(&bmInfoHeader, sizeof(BITMAPINFOHEADER));
	file.Write(rgbTemp, bmInfoHeader.biSizeImage);
	file.Close();

	delete rgbTemp;
}


//-----------------------------------------------------------------------------
// Name : GenerateAttributeMap()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::GenerateAttributeMap()
{
	if( m_pAttributeMap )
	{
		if( MessageBox( NULL, _T("속성맵이 존재합니다.\n다시 생성하시겠습니까?"),
						_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDNO )
			return false;
		
		SAFE_DELETE_ARRAY( m_pAttributeMap );
	}

	// Width of the attribute map
	m_nAttributeMapWidth = ( m_nHMWidth - 1 ) * m_nPatchSize / 10;

	// Alocate memory
	m_pAttributeMap = new unsigned char[m_nAttributeMapWidth * m_nAttributeMapWidth];
	memset( m_pAttributeMap, 0, sizeof(unsigned char) * m_nAttributeMapWidth * m_nAttributeMapWidth );

	// Optimize water area
	if( !g_pMyNKApp->IsLoadedGameMap() )
		g_pObjTree->EraseInvisibleWaters();

	// Calculate water area
	g_pObjTree->CalcWaterMapAttr( m_pAttributeMap, m_nAttributeMapWidth );

	// Calculate area unable to walk
	g_pObjTree->CalcObjectMapAttr( m_nAttributeMapWidth, false );
	g_pObjTree->CalcObjectMapAttr( m_nAttributeMapWidth, true );

	return true;
}


//-----------------------------------------------------------------------------
// Name : RenderAttributeMap()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::RenderAttributeMap( int nRange, float fZ, float fX )
{
	if( m_pAttributeMap == NULL )
		return ;

	NKVERTEXL		aAttrMapVB[150];
	DWORD			dwCount, dwColor;
	unsigned char	nMapAttr;
	int				nX, nZ, nSX, nSZ, nEX, nEZ, x, z, sx, sz, ex, ez;
	nX = fX / m_nPatchSize;
	nZ = fZ / m_nPatchSize;
	nRange = ( nRange - 1 ) / 2;
	nSX = nX - nRange < 0 ? 0 : nX - nRange;
	nSZ = nZ - nRange < 0 ? 0 : nZ - nRange;
	nEX = nX + nRange > m_nHMWidth - 2 ? m_nHMWidth - 2 : nX + nRange;
	nEZ = nZ + nRange > m_nHMWidth - 2 ? m_nHMWidth - 2 : nZ + nRange;

	// Get texture stage state
	DWORD	dwColorOp0, dwColorArg1, dwColorOp1;
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &dwColorOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColorArg1 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLOROP, &dwColorOp1 );

	// Set texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );

	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
		{
			dwCount = 0;
			sx = nX * 5;		sz = nZ * 5;
			ex = sx + 5;		ez = sz + 5;
			for( z = sz; z < ez; z++ )
			{
				for( x = sx; x < ex; x++ )
				{
					nMapAttr = m_pAttributeMap[x + z * m_nAttributeMapWidth];
					if( nMapAttr > 0 )
					{
						switch( nMapAttr )
						{
						case g_nUNABLETOWALK:	dwColor = m_aMapAttrColor[1];	break;
						case g_nWATERAREA:		dwColor = m_aMapAttrColor[2];	break;
						case g_nINDOORAREA:		dwColor = m_aMapAttrColor[3];	break;
						case g_nINDOORBOX:		dwColor = m_aMapAttrColor[4];	break;
						case g_nOUTDOORBOX:		dwColor = m_aMapAttrColor[5];	break;
						}

						fX = x * 10.0f;		fZ = ( z + 1 ) * 10.0f;
						aAttrMapVB[dwCount].p = D3DXVECTOR3( fX, GetHeights( fZ, fX ) + 0.3f, fZ );
						aAttrMapVB[dwCount++].c = dwColor;
						aAttrMapVB[dwCount].p = D3DXVECTOR3( fX + 10.0f, GetHeights( fZ, fX + 10.0f ) + 0.3f, fZ );
						aAttrMapVB[dwCount++].c = dwColor;
						aAttrMapVB[dwCount].p = D3DXVECTOR3( fX, GetHeights( fZ - 10.0f, fX ) + 0.3f, fZ - 10.0f );
						aAttrMapVB[dwCount++].c = dwColor;
						aAttrMapVB[dwCount++] = aAttrMapVB[dwCount - 1];
						aAttrMapVB[dwCount++] = aAttrMapVB[dwCount - 3];
						aAttrMapVB[dwCount].p = D3DXVECTOR3( fX + 10.0f, GetHeights( fZ - 10.0f, fX + 10.0f ) + 0.3f, fZ - 10.0f );
						aAttrMapVB[dwCount++].c = dwColor;
					}
				}
			}

			if( dwCount > 0 )
				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, NKVERTEXL::FVF, aAttrMapVB, dwCount, 0 );
		}
	}

	// Restore texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, dwColorOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, dwColorOp1 );
}


//-----------------------------------------------------------------------------
// Name : DrawMapAttribute()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::DrawMapAttribute( int nMapAttr, int nZ, int nX, int BrushSize ) // 브러쉬 사이즈 조절 기능 추가( by 원석 )
{
	if( m_pAttributeMap == NULL )
		return ;

	unsigned char	nAttr;
	switch( nMapAttr )
	{
	case 0: nAttr = g_nABLETOWALK;		break;
	case 1: nAttr = g_nUNABLETOWALK;	break;
	case 2: nAttr = g_nWATERAREA;		break;
	case 3: nAttr = g_nINDOORAREA;		break;
	case 4: nAttr = g_nINDOORBOX;		break;
	case 5: nAttr = g_nOUTDOORBOX;		break;
	}

	int	nWidth = ( (GetHMWidth()-1) * 5 ) - 1;
	
	int TempHalfBrushSize = BrushSize / 2;
	
	if( TempHalfBrushSize != 0 ) // 0(브러쉬 사이즈 1)일땐 예외 처리.
	{
		for( int i = -TempHalfBrushSize ; i <= TempHalfBrushSize ; ++i )
		{
			for( int j = -TempHalfBrushSize ; j <= TempHalfBrushSize ; ++j )
			{
				if( nZ+j >= 0 && nZ+j <= nWidth && nX+i >= 0 && nX+i <= nWidth ) // 맵 범위에 맞을때만 속성을 적용.
				{
					m_pAttributeMap[(nX+i) + (nZ+j) * m_nAttributeMapWidth] = nAttr;
				}
			}
		}
	}
	else // 브러쉬 사이즈가 1일땐 따로 해줘야한다.(위의 공식상 루프를 돌지 못한다.)
	{
		m_pAttributeMap[nX + nZ * m_nAttributeMapWidth] = nAttr;
	}
}


//-----------------------------------------------------------------------------
// Name : DrawMapAttrLine()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::DrawMapAttrLine( int nSX, int nSY, int nEX, int nEY, unsigned char nMapAttr, bool bDrawInTemp )
{
	// Starting point of line
	int	x = nSX;
	int	y = nSY;

	// Direction of line
	int	dx = nEX - nSX;
	int	dy = nEY - nSY;

	// Increment or decrement depending on direction of line
	int	sx, sy;
	
	if( dx > 0 )
		sx = 1;
	else if( dx < 0 )
	{
		sx = -1;		dx = -dx;
	}
	else
		sx = 0;

	if( dy > 0 )
		sy = 1;
	else if( dy < 0 )
	{
		sy = -1;		dy = -dy;
	}
	else
		sy = 0;

	int	ax = 2 * dx;
	int	ay = 2 * dy;

	if( dy <= dx )
	{
		// Single step in x-direction
		for( int decy = ay - dx; ; x += sx, decy += ay )
		{
			if( bDrawInTemp )
				m_aTempAttrMap[y][x] = nMapAttr;
			else
				m_pAttributeMap[x + y * m_nAttributeMapWidth] = nMapAttr;

			// Take bresenham stemp
			if( x == nEX )
				break;

			if( decy >= 0 )
			{
				decy -= ax;		y += sy;
			}
		}
	}
	else
	{
		// Single stemp int y-direction
		for( int decx = ax - dy; ; y += sy, decx += ax )
		{
			if( bDrawInTemp )
				m_aTempAttrMap[y][x] = nMapAttr;
			else
				m_pAttributeMap[x + y * m_nAttributeMapWidth] = nMapAttr;

			// Take bresenham step
			if( y == nEY )
				break;

			if( decx >= 0 )
			{
				decx -= ay;		x += sx;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name : FillMapAttr()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::FillMapAttr( POINT ptLB, POINT ptLT, POINT ptRT, POINT ptRB, unsigned char nMapAttr )
{
	// Get min & max box
	POINT	ptMin = ptLB;
	POINT	ptMax = ptLB;
	if( ptLT.x < ptMin.x ) ptMin.x = ptLT.x;
	else if( ptLT.x > ptMax.x ) ptMax.x = ptLT.x;
	if( ptLT.y < ptMin.y ) ptMin.y = ptLT.y;
	else if( ptLT.y > ptMax.y ) ptMax.y = ptLT.y;
	if( ptRT.x < ptMin.x ) ptMin.x = ptRT.x;
	else if( ptRT.x > ptMax.x ) ptMax.x = ptRT.x;
	if( ptRT.y < ptMin.y ) ptMin.y = ptRT.y;
	else if( ptRT.y > ptMax.y ) ptMax.y = ptRT.y;
	if( ptRB.x < ptMin.x ) ptMin.x = ptRB.x;
	else if( ptRB.x > ptMax.x ) ptMax.x = ptRB.x;
	if( ptRB.y < ptMin.y ) ptMin.y = ptRB.y;
	else if( ptRB.y > ptMax.y ) ptMax.y = ptRB.y;

	// Get width/height of temp attribute map
	int	nWidth = ptMax.x - ptMin.x;
	int	nHeight = ptMax.y - ptMin.y;
	int	nX, nY;
	for( nY = 0; nY <= nHeight; nY++ )
	{
		for( nX = 0; nX <= nWidth; nX++ )
			m_aTempAttrMap[nY][nX] = 0;
	}	

	// Get new points
	POINT	ptNewLB, ptNewLT, ptNewRT, ptNewRB;
	ptNewLB.x = ptLB.x - ptMin.x;	ptNewLB.y = ptLB.y - ptMin.y;
	ptNewLT.x = ptLT.x - ptMin.x;	ptNewLT.y = ptLT.y - ptMin.y;
	ptNewRT.x = ptRT.x - ptMin.x;	ptNewRT.y = ptRT.y - ptMin.y;
	ptNewRB.x = ptRB.x - ptMin.x;	ptNewRB.y = ptRB.y - ptMin.y;

	// Draw map attribute line
	DrawMapAttrLine( ptNewLB.x, ptNewLB.y, ptNewLT.x, ptNewLT.y, nMapAttr, true );
	DrawMapAttrLine( ptNewLT.x, ptNewLT.y, ptNewRT.x, ptNewRT.y, nMapAttr, true );
	DrawMapAttrLine( ptNewRT.x, ptNewRT.y, ptNewRB.x, ptNewRB.y, nMapAttr, true );
	DrawMapAttrLine( ptNewRB.x, ptNewRB.y, ptNewLB.x, ptNewLB.y, nMapAttr, true );

	// Fill map attribute region
	int		nSX, nEX;
	for( nY = 0; nY <= nHeight; nY++ )
	{
		nSX = 0;	nEX = 0;
		// Find start point
		for( nX = 0; nX <= nWidth; nX++ )
		{
			if( m_aTempAttrMap[nY][nX] == nMapAttr )
			{
				nSX = nX;	break;
			}
		}

		// Find end point
		for( nX = nWidth; nX >= 0; nX-- )
		{
			if( m_aTempAttrMap[nY][nX] == nMapAttr )
			{
				nEX = nX;	break;
			}
		}

		// Fill attribute value from start point to end point
		if( nSX != nEX && nSX < nEX )
		{
			for( nX = nSX; nX <= nEX; nX++ )
				m_aTempAttrMap[nY][nX] = nMapAttr;
		}
	}

	// Copy temp attribute valuse to attribute map
	unsigned char*	pTemp;
	for( nY = 0; nY <= nHeight; nY++ )
	{
		pTemp = &m_pAttributeMap[ptMin.x + ( ptMin.y + nY ) * m_nAttributeMapWidth];
		for( nX = 0; nX <= nWidth; nX++ )
		{
			if( m_aTempAttrMap[nY][nX] > 0 )
				*pTemp = m_aTempAttrMap[nY][nX];
			pTemp++;
		}
	}
}


//-----------------------------------------------------------------------------
// Name : MergeMaps()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::MergeNmf( const TCHAR* szFilename, int nSZ, int nSX, int nEZ, int nEX )
{
	FILE*	fp = fopen( szFilename, "rb" );

	if( fp == NULL )
		return false;

	// Magic( NMF + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic ;
	fread( &dwMagic, sizeof(DWORD), 1, fp );

	if(		(dwMagic == ( ( 'N' << 24 ) | ( 'M' << 16 ) | ( 'F' << 8 ) ) + 10 )
		||	(dwMagic == ( ( 'N' << 24 ) | ( 'M' << 16 ) | ( 'F' << 8 ) ) + 11 ) )
	{

	}
	else
		return false;

	// Terrain name
	int	nLength;
	fread( &nLength, sizeof(int), 1, fp );
	fseek( fp, sizeof(TCHAR) * nLength, SEEK_CUR );

	// Patch size
	int	nPatchSize;
	fread( &nPatchSize, sizeof(int), 1, fp );
	if( m_nPatchSize != nPatchSize )
		return false;

	// Height scale
	int	nHeightScale;
	fread( &nHeightScale, sizeof(int), 1, fp );
	if( m_nHeightScale != nHeightScale )
		return false;

	// Height map width
	int	nHMWidth;
	fread( &nHMWidth, sizeof(int), 1, fp );
	if( m_nHMWidth != nHMWidth )
		return false;

	// Height map		
	unsigned char*	pHeights = new unsigned char[m_nHMWidth*m_nHMWidth];
	fread( pHeights, sizeof(unsigned char), m_nHMWidth*m_nHMWidth, fp );
	int	nX, nZ;
	for( nZ = nSZ; nZ <= nEZ; nZ++ )
	{
		for( nX = nSX; nX <= nEX; nX++ )
			m_pHeights[nX + nZ * m_nHMWidth] = pHeights[nX + nZ * m_nHMWidth];
	}
	SAFE_DELETE_ARRAY( pHeights );
	
	// Terrain Textures
	int		nCount;
	fread( &nCount, sizeof(int), 1, fp );
	int i = 0;
	for( i = 0; i < nCount; i++ )
	{
		fread( &nLength, sizeof(int), 1, fp );
		fseek( fp, sizeof(TCHAR) * nLength, SEEK_CUR );
	}

	// Texture IDs
	int		nWidth = m_nHMWidth - 1;
	unsigned char*	pL1TexID = new unsigned char[nWidth*nWidth];
	unsigned char*	pL2TexID = new unsigned char[nWidth*nWidth];
	unsigned char*	pL3TexID = new unsigned char[nWidth*nWidth];
	for( i = 0, nZ = 0; nZ < nWidth; nZ++ )
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			fread( &pL1TexID[i], sizeof(unsigned char), 1, fp );
			fread( &pL2TexID[i], sizeof(unsigned char), 1, fp );
			fread( &pL3TexID[i], sizeof(unsigned char), 1, fp );
			i++;
		}		
	}

	// Attribute map
	int	nAttributeMapWidth;
	fread( &nAttributeMapWidth, sizeof(int), 1, fp );
	if( nAttributeMapWidth != 0 )
		fseek( fp, sizeof(unsigned char) * m_nAttributeMapWidth * m_nAttributeMapWidth, SEEK_CUR );

	// Color map
	unsigned char	**pTempAlphaMapL2;
	unsigned char	**pTempAlphaMapL3;
	float			***pTempShadowR;
	float			***pTempShadowG;
	float			***pTempShadowB;
	pTempAlphaMapL2 = new unsigned char*[512];
	pTempAlphaMapL3 = new unsigned char*[512];
	for( i = 0; i < 512; i++ )
	{
		pTempAlphaMapL2[i] = new unsigned char[512];
		pTempAlphaMapL3[i] = new unsigned char[512];
	}
	pTempShadowR = new float**[6];
	pTempShadowG = new float**[6];
	pTempShadowB = new float**[6];
	for( i = 0; i < 6; i++ )
	{
		pTempShadowR[i] = new float*[512];
		pTempShadowG[i] = new float*[512];
		pTempShadowB[i] = new float*[512];
		for( int j = 0; j < 512; j++ )
		{
			pTempShadowR[i][j] = new float[512];
			pTempShadowG[i][j] = new float[512];
			pTempShadowB[i][j] = new float[512];
		}
	}

	nWidth = ( m_nHMWidth - 1 ) * 2;
	for( nZ = 0; nZ < nWidth; nZ++ )
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			// Alpha channel - layer 2
			fread( &pTempAlphaMapL2[nZ][nX], sizeof(unsigned char), 1, fp );
			// Alpha channel - layer 3
			fread( &pTempAlphaMapL3[nZ][nX], sizeof(unsigned char), 1, fp );
			// Shadow
			fread( &pTempShadowR[0][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowG[0][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowB[0][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowR[1][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowG[1][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowB[1][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowR[2][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowG[2][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowB[2][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowR[3][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowG[3][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowB[3][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowR[4][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowG[4][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowB[4][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowR[5][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowG[5][nZ][nX], sizeof(float), 1, fp );
			fread( &pTempShadowB[5][nZ][nX], sizeof(float), 1, fp );
		}		
	}

	int	nClrMapSX = nSX * 2;
	int	nClrMapEX = nEX * 2;	
	int	nClrMapSZ = nSZ * 2;
	int	nClrMapEZ = nEZ * 2;	
	for( nZ = nClrMapSZ; nZ <= nClrMapEZ; nZ++ )
	{
		for( nX = nClrMapSX; nX <= nClrMapEX; nX++ )
		{
			m_aAlphaMapL2[nZ][nX] = pTempAlphaMapL2[nZ][nX];
			m_aAlphaMapL3[nZ][nX] = pTempAlphaMapL3[nZ][nX];
			m_aAlphaL2[nZ][nX] = m_aAlphaMapL2[nZ][nX];
			m_aAlphaL3[nZ][nX] = m_aAlphaMapL3[nZ][nX];
			m_aShadowR[0][nZ][nX] = pTempShadowR[0][nZ][nX];
			m_aShadowG[0][nZ][nX] = pTempShadowG[0][nZ][nX];
			m_aShadowB[0][nZ][nX] = pTempShadowB[0][nZ][nX];
			m_aShadowR[1][nZ][nX] = pTempShadowR[0][nZ][nX];
			m_aShadowG[1][nZ][nX] = pTempShadowG[0][nZ][nX];
			m_aShadowB[1][nZ][nX] = pTempShadowB[0][nZ][nX];
			m_aShadowR[2][nZ][nX] = pTempShadowR[0][nZ][nX];
			m_aShadowG[2][nZ][nX] = pTempShadowG[0][nZ][nX];
			m_aShadowB[2][nZ][nX] = pTempShadowB[0][nZ][nX];
			m_aShadowR[3][nZ][nX] = pTempShadowR[0][nZ][nX];
			m_aShadowG[3][nZ][nX] = pTempShadowG[0][nZ][nX];
			m_aShadowB[3][nZ][nX] = pTempShadowB[0][nZ][nX];
			m_aShadowR[4][nZ][nX] = pTempShadowR[0][nZ][nX];
			m_aShadowG[4][nZ][nX] = pTempShadowG[0][nZ][nX];
			m_aShadowB[4][nZ][nX] = pTempShadowB[0][nZ][nX];
			m_aShadowR[5][nZ][nX] = pTempShadowR[0][nZ][nX];
			m_aShadowG[5][nZ][nX] = pTempShadowG[0][nZ][nX];
			m_aShadowB[5][nZ][nX] = pTempShadowB[0][nZ][nX];
		}
	}

	for( i = 0; i < 512; i++ )
	{
		SAFE_DELETE_ARRAY( pTempAlphaMapL2[i] );
		SAFE_DELETE_ARRAY( pTempAlphaMapL3[i] );
	}
	SAFE_DELETE_ARRAY( pTempAlphaMapL2 );
	SAFE_DELETE_ARRAY( pTempAlphaMapL3 );
	
	for( i = 0; i < 6; i++ )
	{
		for( int j = 0; j < 512; j++ )
		{
			SAFE_DELETE_ARRAY( pTempShadowR[i][j] );
			SAFE_DELETE_ARRAY( pTempShadowG[i][j] );
			SAFE_DELETE_ARRAY( pTempShadowB[i][j] );
		}
		SAFE_DELETE_ARRAY( pTempShadowR[i] );
		SAFE_DELETE_ARRAY( pTempShadowG[i] );
		SAFE_DELETE_ARRAY( pTempShadowB[i] );
	}
	SAFE_DELETE_ARRAY( pTempShadowR );
	SAFE_DELETE_ARRAY( pTempShadowG );
	SAFE_DELETE_ARRAY( pTempShadowB );

	fclose( fp );

	// Modify quad tree
	if( !ModifyQuadTree( nSZ, nSX, nEZ, nEX, pL1TexID, pL2TexID, pL3TexID ) )
	{
		Destroy();
		return false;
	}
	SAFE_DELETE_ARRAY( pL1TexID );
	SAFE_DELETE_ARRAY( pL2TexID );
	SAFE_DELETE_ARRAY( pL3TexID );

	RecalcAllBoundingBox();

	// Initialize vertex buffer
	for( i = 0; i < 4; i++ )
		SAFE_RELEASE( m_aVB[i] );
	if( !InitVertexBuffer() )
	{
		Destroy();
		return false;
	}

	// Create a temporary heights, copy height values and calculate min/max heights
	m_nMinHeight = 255;
	m_nMaxHeight = 0;
	for( i = 0, nZ = 0; nZ < m_nHMWidth; nZ++ )
	{
		for( nX = 0; nX < m_nHMWidth; nX++ )
		{
			m_pTempHeights[i] = m_pHeights[i];
			if( m_pHeights[i] < m_nMinHeight )
				m_nMinHeight = m_pHeights[i];
			if( m_pHeights[i] > m_nMaxHeight )
				m_nMaxHeight = m_pHeights[i];
			i++;
		}
	}

	// Make normals
	MakeNormals();

	/*// Copy alpha maps
	nWidth = ( m_nHMWidth - 1 ) * 2;
	for( nZ = 0; nZ < nWidth; nZ++ )
	{
		for( nX = 0; nX < nWidth; nX++ )
		{
			m_aAlphaL2[nZ][nX] = m_aAlphaMapL2[nZ][nX];
			m_aAlphaL3[nZ][nX] = m_aAlphaMapL3[nZ][nX];
		}
	}*/

	// Make rgb maps
	MakeAllRGBMaps();

	// Create color map tesxtures
	if( FAILED( CreateClrMapTexture() ) )
	{
		Destroy();
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : ModifyQuadTree()
// Desc :
//-----------------------------------------------------------------------------
bool CNKTerrain::ModifyQuadTree( int nSZ, int nSX, int nEZ, int nEX,
									unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID )
{
	int nWidth = 1 << m_nQuadTreeDepth;
	int nRightX = nWidth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;
	
	// Lower left vertex buffer
	m_aRootNode[0]->ModifyQuadTree( 0, 0, nRightX, nRightZ,
									nSZ, nSX, nEZ, nEX, pL1TexID, pL2TexID, pL3TexID );

	nCenterX = nRightX;
	nCenterZ = nRightZ;
	nRightX <<= 1;
	nRightZ <<= 1;

	if( m_nHMWidth == 257 )				
	{
		// Lower right vertex buffer
		m_aRootNode[1]->ModifyQuadTree( nCenterX, 0, nRightX, nCenterZ,
										nSZ, nSX, nEZ, nEX, pL1TexID, pL2TexID, pL3TexID );

		// Upper left vertex buffer
		m_aRootNode[2]->ModifyQuadTree( 0, nCenterZ, nCenterX, nRightZ,
										nSZ, nSX, nEZ, nEX, pL1TexID, pL2TexID, pL3TexID );

		// Upper right vertex buffer
		m_aRootNode[3]->ModifyQuadTree( nCenterX, nCenterZ, nRightX, nRightZ,
										nSZ, nSX, nEZ, nEX, pL1TexID, pL2TexID, pL3TexID );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : SetDefaultLightDirections()
// Desc :
//-----------------------------------------------------------------------------
void CNKTerrain::SetDefaultLightDirections()
{
	m_aDirectLightDir[0] = D3DXVECTOR3( 0.0f, -0.743241f, 0.669023f );
	m_aDirectLightDir[1] = D3DXVECTOR3( 0.615358f, -0.675493f, 0.406255f );
	m_aDirectLightDir[2] = D3DXVECTOR3( 0.88732f, -0.460501f, 0.0245227f );
	m_aDirectLightDir[3] = D3DXVECTOR3( 0.0f, -0.57631f, -0.817225f );
	m_aDirectLightDir[4] = D3DXVECTOR3( -0.88732f, -0.460501f, 0.0245227f );
	m_aDirectLightDir[5] = D3DXVECTOR3( -0.615358f, -0.675493f, 0.406255f );
	for( int i = 0; i < 6; i++ )
		D3DXVec3Normalize( &m_aDirectLightDir[i], &m_aDirectLightDir[i] );
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