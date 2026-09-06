//-----------------------------------------------------------------------------
// File : NKObjQuadNode.cpp
// Desc :
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "MyNKApp.h"
#include "NKObjQuadNode.h"
#include "NKCamera.h"
#include "DecoObj.h"
#include "NKMath.h"
#include "GUtil.h"


// Global variables
extern CMyNKApp*		g_pMyNKApp;
extern CDecoObjSet*		g_pDecoObjSet;
extern list<CDecoObj*>	g_lstAlphaDeco;
extern list<CDecoObj*>	g_lstLastDeco;
extern list<CDecoObj*>	g_lstFarDeco;
CNKObjQuadNode*			g_pObjTree = NULL;
list<CDecoObj*>			g_lstWalkObj[64][64];
//D3DXVECTOR3					ptCross;


//-----------------------------------------------------------------------------
// Static member variables
//-----------------------------------------------------------------------------
LPDIRECT3D7				CNKObjQuadNode::ms_pD3D = NULL;
LPDIRECT3DDEVICE7		CNKObjQuadNode::ms_pd3dDevice = NULL;
int						CNKObjQuadNode::ms_nCellSize = 0;
int						CNKObjQuadNode::ms_nTreeWidth = 0;
CDecoObj*				CNKObjQuadNode::ms_pOpenDeco = NULL;
TCHAR*					CNKObjQuadNode::ms_szFilename = NULL;
float					CNKObjQuadNode::ms_fRenderDist = 3500.0f;
float					CNKObjQuadNode::ms_fBlendDist = 2000.0f;
//float					CNKObjQuadNode::ms_fRenderDist = 1500.0f; // 그래픽 팀의 요청으로 시야 거리 2종을 늘림. (by 원석)
//float					CNKObjQuadNode::ms_fBlendDist = 1000.0f;  // 게임 클라이언트와의 차이로 인해 문제가 있을 수도 있으니 참고. (by 원석)
IndexedTexture*			CNKObjQuadNode::ms_pIndexedTexture = NULL;
int						CNKObjQuadNode::ms_nBaseWaterTexID = -1;
int						CNKObjQuadNode::ms_nWaveWaterTexID1 = -1;
int						CNKObjQuadNode::ms_nWaveWaterTexID2 = -1;
LPDIRECT3DVERTEXBUFFER7	CNKObjQuadNode::ms_pBaseWaterVB = NULL;
LPDIRECT3DVERTEXBUFFER7	CNKObjQuadNode::ms_pWaveWaterVB = NULL;
DWORD					CNKObjQuadNode::ms_dwBaseWaterVBSize = 0;
DWORD					CNKObjQuadNode::ms_dwWaveWaterVBSize = 0;
WORD*					CNKObjQuadNode::ms_pBaseWaterIB = NULL;
WORD*					CNKObjQuadNode::ms_pWaveWaterIB = NULL;
DWORD					CNKObjQuadNode::ms_dwBaseIndexCount = 0;
DWORD					CNKObjQuadNode::ms_dwWaveIndexCount = 0;
float					CNKObjQuadNode::ms_fBaseWaterMove = 0.0f;
float					CNKObjQuadNode::ms_fWaveWaterMove1 = 0.0f;
float					CNKObjQuadNode::ms_fWaveWaterMove2 = 0.0f;
D3DMATRIX				CNKObjQuadNode::ms_matTexTrans;
float*					CNKObjQuadNode::ms_pWaterHeights = NULL;
int						CNKObjQuadNode::ms_nWaterHeightWidth = 0;

CNKObjQuadNode*			CNKObjQuadNode::ms_aQuadNodes[64][64];
NKVERTEXLT*				CNKObjQuadNode::ms_pBaseWaterTempVB = NULL;
NKVERTEXLT*				CNKObjQuadNode::ms_pWaveWaterTempVB = NULL;
DWORD					CNKObjQuadNode::ms_dwBaseVertexCount = 0;
DWORD					CNKObjQuadNode::ms_dwWaveVertexCount = 0;
int						CNKObjQuadNode::ms_nBaseWaterSize = 0;
int						CNKObjQuadNode::ms_nWaveWaterSize = 0;
float*					CNKObjQuadNode::ms_pHeightOfWaterGroup = NULL;
TCHAR					CNKObjQuadNode::ms_aNamesOfWaterGroup[20][20];
unsigned char*			CNKObjQuadNode::ms_pCoastLineData = NULL;
DWORD					CNKObjQuadNode::ms_dwBaseWaterAlpha = 0x60000000;


//-----------------------------------------------------------------------------
// Name : CNKObjQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKObjQuadNode::CNKObjQuadNode()
{
	m_pParent = NULL;
	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;
}


//-----------------------------------------------------------------------------
// Name : CNKObjQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKObjQuadNode::CNKObjQuadNode( CNKObjQuadNode* pParent, int nLevel,
								int nLeftX, int nLeftZ, int nRightX, int nRightZ )
{
	int	nCenterX, nCenterZ;

	m_pParent = pParent;
	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;
	m_nWaterGroupID = 0;
	m_pWaveWaterVert1 = NULL;
	m_pWaveWaterVert2 = NULL;
	m_nBaseWaterSegment = 0;
	m_nWaveWaterSegment1 = 0;
	m_nWaveWaterSegment2 = 0;

	// Set a bounding box
	m_vMin = D3DXVECTOR3( nLeftX * ms_nCellSize, FLT_MAX, nLeftZ * ms_nCellSize );
	m_vMax = D3DXVECTOR3( nRightX * ms_nCellSize, -FLT_MAX, nRightZ * ms_nCellSize );

	if( nLevel < 1 )			// Lowest level node
	{
		// Set a quad tree node pointer to access directly in map tool
		ms_aQuadNodes[nLeftZ][nLeftX] = this;

		// Set a node index
		m_nX = nLeftX;
		m_nZ = nLeftZ;

		// Init water info
		m_nWaterGroupID = 0;
		m_nBaseWaterFlags = 0;
		m_nWaveWaterSegment1 = 0;
		m_nWaveWaterSegment2 = 0;

		return ;
	}

	// Prepare for a child node
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft = new CNKObjQuadNode( this, nLevel, nLeftX, nLeftZ, nCenterX, nCenterZ );
	m_pLRight = new CNKObjQuadNode( this, nLevel, nCenterX, nLeftZ, nRightX, nCenterZ );
	m_pULeft = new CNKObjQuadNode( this, nLevel, nLeftX, nCenterZ, nCenterX, nRightZ );
	m_pURight = new CNKObjQuadNode( this, nLevel, nCenterX, nCenterZ, nRightX, nRightZ );
}


//-----------------------------------------------------------------------------
// Name : CNKObjQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKObjQuadNode::CNKObjQuadNode( CNKObjQuadNode* pParent, int nLevel, int nLeftX, int nLeftZ,
								int nRightX, int nRightZ, FILE* fp )
{
	int		nCenterX, nCenterZ;

	m_pParent = pParent;
	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;
	m_nWaterGroupID = 0;
	m_pWaveWaterVert1 = NULL;
	m_pWaveWaterVert2 = NULL;
	m_nBaseWaterSegment = 0;
	m_nWaveWaterSegment1 = 0;
	m_nWaveWaterSegment2 = 0;

	if( nLevel < 1 )			// Lowest level node
	{
		// Set a quad tree node pointer to access directly in map tool
		ms_aQuadNodes[nLeftZ][nLeftX] = this;

		// Set a node index
		m_nX = nLeftX;
		m_nZ = nLeftZ;

		// Read DecoObj list
		// Count
		DWORD	dwCount;
		fread( &dwCount, sizeof(DWORD), 1, fp );

		//Bounding box
		fread( &(m_vMin.x), sizeof(float), 1, fp );
		fread( &(m_vMin.y), sizeof(float), 1, fp );
		fread( &(m_vMin.z), sizeof(float), 1, fp );
		fread( &(m_vMax.x), sizeof(float), 1, fp );
		fread( &(m_vMax.y), sizeof(float), 1, fp );
		fread( &(m_vMax.z), sizeof(float), 1, fp );

		// DecoObj
		CDecoObj*	pDecoObj;
		for( DWORD i = 0; i < dwCount; i++ )
		{
			pDecoObj = new CDecoObj();
			fread( &(pDecoObj->m_nLoloId), sizeof(int), 1, fp );
			fread( &(pDecoObj->m_wx), sizeof(float), 1, fp );
			fread( &(pDecoObj->m_wz), sizeof(float), 1, fp );
			fread( &(pDecoObj->m_wy), sizeof(float), 1, fp );
			fread( &(pDecoObj->m_dir), sizeof(float), 1, fp );

			pDecoObj->m_pLolo = g_pDecoObjSet->m_vecLoloSet[pDecoObj->m_nLoloId];
			pDecoObj->CalBoundBox();
			m_lstDecoObj.push_back( pDecoObj );			
			g_pDecoObjSet->m_vecRefCount[pDecoObj->m_nLoloId]++;

			if( pDecoObj->m_pLolo->m_WalkPlaneCount > 0 )
			{
				CDecoObj*	pDecoObj2 = new CDecoObj();
				pDecoObj2->m_nLoloId = pDecoObj->m_nLoloId;
				pDecoObj2->m_wx = pDecoObj->m_wx;
				pDecoObj2->m_wy = pDecoObj->m_wy;
				pDecoObj2->m_wz = pDecoObj->m_wz;
				pDecoObj2->m_dir = pDecoObj->m_dir;
				pDecoObj2->m_pLolo = pDecoObj->m_pLolo;
				pDecoObj2->CalBoundBox();
				g_lstWalkObj[m_nZ][m_nX].push_back( pDecoObj2 );
			}
		}

		// Read water info
		fread( &m_nWaterGroupID, sizeof(unsigned char), 1, fp );
		fread( &m_nBaseWaterFlags, sizeof(unsigned char), 1, fp );
		fread( &m_nWaveWaterSegment1, sizeof(unsigned char), 1, fp );
		fread( &m_nWaveWaterSegment2, sizeof(unsigned char), 1, fp );
		
		if( m_nWaveWaterSegment1 > 0 )
		{
			m_pWaveWaterVert1 = new NKVERTEXLT[27];
			fread( m_pWaveWaterVert1, sizeof(NKVERTEXLT), ( m_nWaveWaterSegment1 + 1 ) * 3, fp );
		}
		if( m_nWaveWaterSegment2 > 0 )
		{
			m_pWaveWaterVert2 = new NKVERTEXLT[27];
			fread( m_pWaveWaterVert2, sizeof(NKVERTEXLT), ( m_nWaveWaterSegment2 + 1 ) * 3, fp );
		}

		int				x, z, sx, sz, ex, ez;
		unsigned char	nMask = 1;
		float			fWaterHeight = ms_pHeightOfWaterGroup[m_nWaterGroupID];
		sx = m_nX << 1;				sz = m_nZ << 1;
		ex = ( m_nX + 1 ) << 1;		ez = ( m_nZ + 1 ) << 1;
		for( z = sz; z < ez; z++ )
		{
			for( x = sx; x < ex; x++ )
			{
				if( m_nBaseWaterFlags & nMask )
					ms_pWaterHeights[x + z * ms_nWaterHeightWidth] = fWaterHeight;
				else
					ms_pWaterHeights[x + z * ms_nWaterHeightWidth] = -1.0f;

				nMask <<= 1;
			}
		}

		return ;
	}

	// Prepare for a child node
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft = new CNKObjQuadNode( this, nLevel, nLeftX, nLeftZ, nCenterX, nCenterZ, fp );
	m_pLRight = new CNKObjQuadNode( this, nLevel, nCenterX, nLeftZ, nRightX, nCenterZ, fp );
	m_pULeft = new CNKObjQuadNode( this, nLevel, nLeftX, nCenterZ, nCenterX, nRightZ, fp );
	m_pURight = new CNKObjQuadNode( this, nLevel, nCenterX, nCenterZ, nRightX, nRightZ, fp );

	// Get min & max vertices of lower level node
	D3DXVECTOR3	vMin = m_pLLeft->m_vMin;
	D3DXVECTOR3	vMax = m_pLLeft->m_vMax;

	// Calculate min & max vertices of current level node
	if( vMin.x > m_pLRight->m_vMin.x ) vMin.x = m_pLRight->m_vMin.x;
	if( vMin.x > m_pULeft->m_vMin.x ) vMin.x = m_pULeft->m_vMin.x;
	if( vMin.x > m_pURight->m_vMin.x ) vMin.x = m_pURight->m_vMin.x;
	if( vMax.x < m_pLRight->m_vMax.x ) vMax.x = m_pLRight->m_vMax.x;
	if( vMax.x < m_pULeft->m_vMax.x ) vMax.x = m_pULeft->m_vMax.x;
	if( vMax.x < m_pURight->m_vMax.x ) vMax.x = m_pURight->m_vMax.x;

	if( vMin.y > m_pLRight->m_vMin.y ) vMin.y = m_pLRight->m_vMin.y;
	if( vMin.y > m_pULeft->m_vMin.y ) vMin.y = m_pULeft->m_vMin.y;
	if( vMin.y > m_pURight->m_vMin.y ) vMin.y = m_pURight->m_vMin.y;
	if( vMax.y < m_pLRight->m_vMax.y ) vMax.y = m_pLRight->m_vMax.y;
	if( vMax.y < m_pULeft->m_vMax.y ) vMax.y = m_pULeft->m_vMax.y;
	if( vMax.y < m_pURight->m_vMax.y ) vMax.y = m_pURight->m_vMax.y;

	if( vMin.z > m_pLRight->m_vMin.z ) vMin.z = m_pLRight->m_vMin.z;
	if( vMin.z > m_pULeft->m_vMin.z ) vMin.z = m_pULeft->m_vMin.z;
	if( vMin.z > m_pURight->m_vMin.z ) vMin.z = m_pURight->m_vMin.z;
	if( vMax.z < m_pLRight->m_vMax.z ) vMax.z = m_pLRight->m_vMax.z;
	if( vMax.z < m_pULeft->m_vMax.z ) vMax.z = m_pULeft->m_vMax.z;
	if( vMax.z < m_pURight->m_vMax.z ) vMax.z = m_pURight->m_vMax.z;

	// Set a bounding box of current level node
	m_vMin = vMin;
	m_vMax = vMax;
}


//-----------------------------------------------------------------------------
// Name : CNKObjQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKObjQuadNode::CNKObjQuadNode( int nLevel, int nLeftX, int nLeftZ, int nRightX, int nRightZ, FILE* fp )
{
	int	nCenterX, nCenterZ;

	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;
	m_pParent = NULL;
	m_pWaveWaterVert1 = NULL;
	m_pWaveWaterVert2 = NULL;
	m_nBaseWaterSegment = 0;
	m_nWaveWaterSegment1 = 0;
	m_nWaveWaterSegment2 = 0;
	m_wBaseVBIndex = 0;
	m_wWaveVBIndex = 0;

	if( nLevel < 1 )			// Lowest level node
	{
		// Set a node index
		m_nX = nLeftX;
		m_nZ = nLeftZ;

		//Bounding box
		fread( &(m_vMin.x), sizeof(float), 1, fp );
		fread( &(m_vMin.y), sizeof(float), 1, fp );
		fread( &(m_vMin.z), sizeof(float), 1, fp );
		fread( &(m_vMax.x), sizeof(float), 1, fp );
		fread( &(m_vMax.y), sizeof(float), 1, fp );
		fread( &(m_vMax.z), sizeof(float), 1, fp );

		// Read DecoObj list
		// Count
		DWORD	dwCount;
		fread( &dwCount, sizeof(DWORD), 1, fp );		

		// DecoObj
		CDecoObj*	pDecoObj;
		for( DWORD i = 0; i < dwCount; i++ )
		{
			pDecoObj = new CDecoObj();
			fread( &(pDecoObj->m_nLoloId), sizeof(int), 1, fp );
			fread( &(pDecoObj->m_wx), sizeof(float), 1, fp );
			fread( &(pDecoObj->m_wz), sizeof(float), 1, fp );
			fread( &(pDecoObj->m_wy), sizeof(float), 1, fp );
			fread( &(pDecoObj->m_dir), sizeof(float), 1, fp );

			pDecoObj->m_pLolo = g_pDecoObjSet->m_vecLoloSet[pDecoObj->m_nLoloId];
			pDecoObj->CalBoundBox();
			m_lstDecoObj.push_back( pDecoObj );			

			if( pDecoObj->m_pLolo->m_WalkPlaneCount > 0 )
			{
				CDecoObj*	pDecoObj2 = new CDecoObj();
				pDecoObj2->m_nLoloId = pDecoObj->m_nLoloId;
				pDecoObj2->m_wx = pDecoObj->m_wx;
				pDecoObj2->m_wy = pDecoObj->m_wy;
				pDecoObj2->m_wz = pDecoObj->m_wz;
				pDecoObj2->m_dir = pDecoObj->m_dir;
				pDecoObj2->m_pLolo = pDecoObj->m_pLolo;
				pDecoObj2->CalBoundBox();
				g_lstWalkObj[m_nZ][m_nX].push_back( pDecoObj2 );
			}
		}

		// Read water info
		fread( &m_nBaseWaterSegment, sizeof(unsigned char), 1, fp );
		fread( &m_nWaveWaterSegment1, sizeof(unsigned char), 1, fp );
		fread( &m_nWaveWaterSegment2, sizeof(unsigned char), 1, fp );
		fread( &m_wBaseVBIndex, sizeof(WORD), 1, fp );
		fread( &m_wWaveVBIndex, sizeof(WORD), 1, fp );

		return ;
	}

	// Prepare for a child node
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft = new CNKObjQuadNode( nLevel, nLeftX, nLeftZ, nCenterX, nCenterZ, fp );
	m_pLRight = new CNKObjQuadNode( nLevel, nCenterX, nLeftZ, nRightX, nCenterZ, fp );
	m_pULeft = new CNKObjQuadNode( nLevel, nLeftX, nCenterZ, nCenterX, nRightZ, fp );
	m_pURight = new CNKObjQuadNode( nLevel, nCenterX, nCenterZ, nRightX, nRightZ, fp );

	// Get min & max vertices of lower level node
	D3DXVECTOR3	vMin = m_pLLeft->m_vMin;
	D3DXVECTOR3	vMax = m_pLLeft->m_vMax;

	// Calculate min & max vertices of current level node
	if( vMin.x > m_pLRight->m_vMin.x ) vMin.x = m_pLRight->m_vMin.x;
	if( vMin.x > m_pULeft->m_vMin.x ) vMin.x = m_pULeft->m_vMin.x;
	if( vMin.x > m_pURight->m_vMin.x ) vMin.x = m_pURight->m_vMin.x;
	if( vMax.x < m_pLRight->m_vMax.x ) vMax.x = m_pLRight->m_vMax.x;
	if( vMax.x < m_pULeft->m_vMax.x ) vMax.x = m_pULeft->m_vMax.x;
	if( vMax.x < m_pURight->m_vMax.x ) vMax.x = m_pURight->m_vMax.x;

	if( vMin.y > m_pLRight->m_vMin.y ) vMin.y = m_pLRight->m_vMin.y;
	if( vMin.y > m_pULeft->m_vMin.y ) vMin.y = m_pULeft->m_vMin.y;
	if( vMin.y > m_pURight->m_vMin.y ) vMin.y = m_pURight->m_vMin.y;
	if( vMax.y < m_pLRight->m_vMax.y ) vMax.y = m_pLRight->m_vMax.y;
	if( vMax.y < m_pULeft->m_vMax.y ) vMax.y = m_pULeft->m_vMax.y;
	if( vMax.y < m_pURight->m_vMax.y ) vMax.y = m_pURight->m_vMax.y;

	if( vMin.z > m_pLRight->m_vMin.z ) vMin.z = m_pLRight->m_vMin.z;
	if( vMin.z > m_pULeft->m_vMin.z ) vMin.z = m_pULeft->m_vMin.z;
	if( vMin.z > m_pURight->m_vMin.z ) vMin.z = m_pURight->m_vMin.z;
	if( vMax.z < m_pLRight->m_vMax.z ) vMax.z = m_pLRight->m_vMax.z;
	if( vMax.z < m_pULeft->m_vMax.z ) vMax.z = m_pULeft->m_vMax.z;
	if( vMax.z < m_pURight->m_vMax.z ) vMax.z = m_pURight->m_vMax.z;

	// Set a bounding box of current level node
	m_vMin = vMin;
	m_vMax = vMax;
}


//-----------------------------------------------------------------------------
// Name : ~CNKObjQuadNode()
// Desc : Destructor
//-----------------------------------------------------------------------------
CNKObjQuadNode::~CNKObjQuadNode()
{
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); )
	{
		pDecoObj = *iterDeco;
		delete pDecoObj;
		iterDeco = m_lstDecoObj.erase( iterDeco );		
	}

	SAFE_DELETE( m_pWaveWaterVert1 );
	SAFE_DELETE( m_pWaveWaterVert2 );

	SAFE_DELETE( m_pLLeft );
	SAFE_DELETE( m_pLRight );
	SAFE_DELETE( m_pULeft );
	SAFE_DELETE( m_pURight );
}


//-----------------------------------------------------------------------------
// Name : LoadNot()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::LoadNot( const TCHAR* szFilename, IndexedTexture* pIndexedTexture  )
{
	FILE*	fp;
	fp = fopen( szFilename, "rb" );
	
	ms_nCellSize = g_pMyNKApp->GetTerrain()->GetPatchSize() * 4;
	ms_nBaseWaterSize = ms_nCellSize >> 1;
	ms_nWaveWaterSize = ms_nBaseWaterSize >> 1;
	ms_pIndexedTexture = pIndexedTexture;

	// Get a quadtree depth
	int		i;
	DWORD	dwTreeDepth = 0;
	DWORD	dwWidth = ( g_pMyNKApp->GetTerrain()->GetHMWidth() - 1 ) / 4;
	ms_nTreeWidth = dwWidth;
	ms_nWaterHeightWidth = ms_nTreeWidth * 2;
	int		nRight = dwWidth;
	while( dwWidth >>= 1 )
		dwTreeDepth++;

	// Height data
	ms_pHeightOfWaterGroup = new float[20];
	ms_pWaterHeights = new float[ms_nWaterHeightWidth * ms_nWaterHeightWidth];
	memset( ms_pHeightOfWaterGroup, -1, sizeof(float) * 20 );
	memset( ms_pWaterHeights, -1, sizeof(float) * ms_nWaterHeightWidth * ms_nWaterHeightWidth );

	// Build a quadtree
	if( fp == NULL)
	{
		for( i = 0; i < 20; i++ )
			ms_pHeightOfWaterGroup[i] = -1.0f;
		ms_dwBaseWaterAlpha = 0x60000000;
		ms_nBaseWaterTexID = ms_pIndexedTexture->CreateTextureFromFile( _T("water_test.ntx") );
		ms_nWaveWaterTexID1 = ms_pIndexedTexture->CreateTextureFromFile( _T("wave1_test.ntx") );
		ms_nWaveWaterTexID2 = ms_pIndexedTexture->CreateTextureFromFile( _T("wave2_test.ntx") );
		ms_pIndexedTexture->RestoreByIdx( ms_nBaseWaterTexID, g_pMyNKApp->GetD3DDevice() );
		ms_pIndexedTexture->RestoreByIdx( ms_nWaveWaterTexID1, g_pMyNKApp->GetD3DDevice() );
		ms_pIndexedTexture->RestoreByIdx( ms_nWaveWaterTexID2, g_pMyNKApp->GetD3DDevice() );
		
		g_pObjTree = new CNKObjQuadNode( NULL, dwTreeDepth, 0, 0, nRight, nRight );
		for( i = 0; i < ms_nWaterHeightWidth * ms_nWaterHeightWidth; i++ )
			ms_pWaterHeights[i] = -1.0f;
	}
	else
	{
		int		nLength;
		TCHAR	szBaseName[512], szWave1Name[512], szWave2Name[512];

		// Magic( NOT + Version ) - Version: 0.1 -> 1, 1.1 -> 11
		DWORD	dwMagic;
		fread( &dwMagic, sizeof(DWORD), 1, fp );
		if( dwMagic != ( ( 'N' << 24 ) | ( 'O' << 16 ) | ( 'T' << 8 ) ) + 11 )
			return false;

		// Name & height of water groups
		int	nCount;
		fread( &nCount, sizeof(int), 1, fp );
		for( i = 0; i < nCount; i++ )
		{
			fread( &nLength, sizeof(int), 1, fp );
			fread( ms_aNamesOfWaterGroup[i], sizeof(TCHAR), nLength, fp );
			fread( &ms_pHeightOfWaterGroup[i], sizeof(float), 1, fp );
		}

		// Water textures
		fread( &nLength, sizeof(int), 1, fp );
		if( nLength > 0 )
			fread( szBaseName, sizeof(TCHAR), nLength, fp );
		szBaseName[nLength] = NULL;
		fread( &nLength, sizeof(int), 1, fp );
		if( nLength > 0 )
			fread( szWave1Name, sizeof(TCHAR), nLength, fp );
		szWave1Name[nLength] = NULL;
		fread( &nLength, sizeof(int), 1, fp );
		if( nLength > 0 )
			fread( szWave2Name, sizeof(TCHAR), nLength, fp );
		szWave2Name[nLength] = NULL;

		// Alpha value of waters
		fread( &ms_dwBaseWaterAlpha, sizeof(DWORD), 1, fp );

		// Object tree
		g_pObjTree = new CNKObjQuadNode( NULL, dwTreeDepth, 0, 0, nRight, nRight, fp );

		fclose( fp );

		ms_nBaseWaterTexID = ms_pIndexedTexture->CreateTextureFromFile( szBaseName );
		ms_nWaveWaterTexID1 = ms_pIndexedTexture->CreateTextureFromFile( szWave1Name );
		ms_nWaveWaterTexID2 = ms_pIndexedTexture->CreateTextureFromFile( szWave2Name );
		ms_pIndexedTexture->RestoreByIdx( ms_nBaseWaterTexID, g_pMyNKApp->GetD3DDevice() );
		ms_pIndexedTexture->RestoreByIdx( ms_nWaveWaterTexID1, g_pMyNKApp->GetD3DDevice() );
		ms_pIndexedTexture->RestoreByIdx( ms_nWaveWaterTexID2, g_pMyNKApp->GetD3DDevice() );
	}

	// Temporary size
	DWORD	dwSize = g_pMyNKApp->GetCamera()->GetFrustumFar() / ms_nBaseWaterSize;
	ms_dwBaseWaterVBSize = dwSize * dwSize * 4;
	ms_pBaseWaterTempVB = new NKVERTEXLT[ms_dwBaseWaterVBSize];
	ms_pBaseWaterIB = new WORD[dwSize * dwSize * 6];

	dwSize = g_pMyNKApp->GetCamera()->GetFrustumFar() / ( ms_nBaseWaterSize * 2 );
	ms_dwWaveWaterVBSize = dwSize * dwSize * 54;	
	ms_pWaveWaterTempVB = new NKVERTEXLT[ms_dwWaveWaterVBSize];	
	ms_pWaveWaterIB = new WORD[dwSize * dwSize * 192];

	for( i = 0; i < ms_dwBaseWaterVBSize; i += 4 )
	{
		ms_pBaseWaterTempVB[i].u = 0.0f;		ms_pBaseWaterTempVB[i].v = 1.0f;
		ms_pBaseWaterTempVB[i+1].u = 1.0f;		ms_pBaseWaterTempVB[i+1].v = 1.0f;
		ms_pBaseWaterTempVB[i+2].u = 0.0f;		ms_pBaseWaterTempVB[i+2].v = 0.0f;
		ms_pBaseWaterTempVB[i+3].u = 1.0f;		ms_pBaseWaterTempVB[i+3].v = 0.0f;
	}

	D3DUtil_SetIdentityMatrix( ms_matTexTrans );

	return true;
}


//-----------------------------------------------------------------------------
// Name : LoadLst()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::LoadLst( const TCHAR* szFilename, IndexedTexture* pIndexedTexture )
{
	// Create a progress dialog
	g_pMyNKApp->InitProgress();

	ms_nCellSize = g_pMyNKApp->GetTerrain()->GetPatchSize() * 4;
	ms_nBaseWaterSize = ms_nCellSize >> 1;
	ms_nWaveWaterSize = ms_nBaseWaterSize >> 1;
	ms_pIndexedTexture = pIndexedTexture;

	// Get a quadtree depth
	DWORD	dwTreeDepth = 0;
	DWORD	dwWidth = ( g_pMyNKApp->GetTerrain()->GetHMWidth() - 1 ) / 4;
	ms_nTreeWidth = dwWidth;
	ms_nWaterHeightWidth = ms_nTreeWidth * 2;
	int		nRight = dwWidth;
	while( dwWidth >>= 1 )
		dwTreeDepth++;

	ms_dwBaseWaterAlpha = 0x60000000;
	ms_nBaseWaterTexID = ms_pIndexedTexture->CreateTextureFromFile( _T("water_test.ntx") );
	ms_nWaveWaterTexID1 = ms_pIndexedTexture->CreateTextureFromFile( _T("wave1_test.ntx") );
	ms_nWaveWaterTexID2 = ms_pIndexedTexture->CreateTextureFromFile( _T("wave2_test.ntx") );
	ms_pIndexedTexture->RestoreByIdx( ms_nBaseWaterTexID, g_pMyNKApp->GetD3DDevice() );
	ms_pIndexedTexture->RestoreByIdx( ms_nWaveWaterTexID1, g_pMyNKApp->GetD3DDevice() );
	ms_pIndexedTexture->RestoreByIdx( ms_nWaveWaterTexID2, g_pMyNKApp->GetD3DDevice() );

	g_pObjTree = new CNKObjQuadNode( NULL, dwTreeDepth, 0, 0, nRight, nRight );

	FILE*	fp;
	fp = fopen( szFilename, "rt" );

	if( fp )
	{
		int	listNum;
		fscanf(fp, "%d", &listNum);

		// Progress dialog
		g_pMyNKApp->SetProgressTitle( _T(" Import Bld, Lst ") );
		g_pMyNKApp->SetProgressContent( _T("Import objects of the previous version to the NKMapTool.") );
		g_pMyNKApp->SetProgressRange( 0, listNum );
		g_pMyNKApp->SetProgressPos( 0 );

		CDecoObj*	pDecoObj;
		int			nX, nZ;
		float		fArea;
		for( int nIndex = 0; nIndex < listNum; nIndex++ )
		{
			pDecoObj = new CDecoObj();
			// id, x, z, y, dir
			fscanf( fp, "%d %f %f %f %f", &(pDecoObj->m_nLoloId), &(pDecoObj->m_wx),
											&(pDecoObj->m_wz), &(pDecoObj->m_wy), &(pDecoObj->m_dir) );
			pDecoObj->m_pLolo = g_pDecoObjSet->m_vecLoloSet[pDecoObj->m_nLoloId];
			pDecoObj->CalBoundBox();

			nX = pDecoObj->m_wx / ms_nCellSize;
			nZ = pDecoObj->m_wz / ms_nCellSize;
			CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );
			
			if( !pObjQuadNode->IsDecoObjInNode( pDecoObj ) )
			{
				pObjQuadNode = NULL;
				fArea = FLT_MAX;
				g_pObjTree->FindDecoObjFittedNode( pDecoObj, &pObjQuadNode, fArea );
			}

			pObjQuadNode->AddDecoObj( pDecoObj );
			g_pDecoObjSet->m_vecRefCount[pDecoObj->m_nLoloId]++;
			
			// Progress dialog
			g_pMyNKApp->SetProgressOffsetPos( 1 );
		}

		fclose( fp );
	}

	DWORD	dwSize = g_pMyNKApp->GetCamera()->GetFrustumFar() / ms_nBaseWaterSize;
	ms_dwBaseWaterVBSize = dwSize * dwSize * 4;
	ms_dwWaveWaterVBSize = dwSize * dwSize * 4;

	ms_pBaseWaterTempVB = new NKVERTEXLT[ms_dwBaseWaterVBSize];
	ms_pWaveWaterTempVB = new NKVERTEXLT[ms_dwWaveWaterVBSize];
	ms_pBaseWaterIB = new WORD[dwSize * dwSize * 6];
	ms_pWaveWaterIB = new WORD[dwSize * dwSize * 6];

	int i = 0;
	for( i = 0; i < ms_dwBaseWaterVBSize; i += 4 )
	{
		ms_pBaseWaterTempVB[i].u = 0.0f;		ms_pBaseWaterTempVB[i].v = 1.0f;
		ms_pBaseWaterTempVB[i+1].u = 1.0f;		ms_pBaseWaterTempVB[i+1].v = 1.0f;
		ms_pBaseWaterTempVB[i+2].u = 0.0f;		ms_pBaseWaterTempVB[i+2].v = 0.0f;
		ms_pBaseWaterTempVB[i+3].u = 1.0f;		ms_pBaseWaterTempVB[i+3].v = 0.0f;
	}

	ms_pHeightOfWaterGroup = new float[20];
	for( i = 0; i < 20; i++ )
		ms_pHeightOfWaterGroup[i] = -1.0f;
	ms_pWaterHeights = new float[ms_nWaterHeightWidth * ms_nWaterHeightWidth];
	for( i = 0; i < ms_nWaterHeightWidth * ms_nWaterHeightWidth; i++ )
		ms_pWaterHeights[i] = -1.0f;

	D3DUtil_SetIdentityMatrix( ms_matTexTrans );

	// Destroy a progress dialog
	g_pMyNKApp->DestroyProgress();
	
	return true;
}


//-----------------------------------------------------------------------------
// Name : LoadObj()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::LoadObj( const TCHAR* szFilepath, const TCHAR* szFilename, IndexedTexture* pIndexedTexture )
{
	//ms_pIndexedTexture = pIndexedTexture;
	//ms_nCellSize = g_pMyNKApp->GetTerrain()->GetPatchSize() * 4;
	//ms_nTreeWidth = ( g_pMyNKApp->GetTerrain()->GetHMWidth() - 1 ) / 4;
	//ms_nWaterHeightWidth = ms_nTreeWidth * 2;

	//// Get a quadtree depth
	//DWORD	dwTreeDepth = 0;
	//DWORD	dwWidth = ms_nTreeWidth;
	//int		nRight = dwWidth;
	int gMapExtention = 50;
	int gMapWidth = 256;
	ms_pIndexedTexture = pIndexedTexture;
	ms_nBaseWaterSize = gMapExtention * 2;
	ms_nCellSize = gMapExtention * 4;
	ms_nTreeWidth = gMapWidth / 4;
	ms_nWaterHeightWidth = gMapWidth / 2;

	// Get a quadtree depth
	DWORD	dwTreeDepth = 0;
	DWORD	dwWidth = gMapWidth / 4;
	int		nRight = dwWidth;
	while( dwWidth >>= 1 )
		dwTreeDepth++;

	TCHAR	szFilePathName[512];

	// Load wat
	_tcscpy( szFilePathName, szFilepath );
	_tcscat( szFilePathName, szFilename );
	_tcscat( szFilePathName, ".wat" );
	if( !CNKObjQuadNode::LoadWat( szFilePathName ) )
		return false;

	// Load obj
	_tcscpy( szFilePathName, szFilepath);
	_tcscat( szFilePathName, szFilename );
	//_tcscat( szFilePathName, ".obj" ); // [2008/3/31 Theodoric] 인스톨 쉴드에서 확장자가 obj 가 있으면 에러가 남. 그래서 oba로 바꿈
	_tcscat( szFilePathName, ".oba" );
	FILE*	fp;
	fp = fopen( szFilePathName, "rb" );
	if( fp == NULL)
		return false;

	// Magic( NOT + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( ( 'O' << 24 ) | ( 'B' << 16 ) | ( 'J' << 8 ) ) + 10 )
		return false;

	// Object tree
	g_pObjTree = new CNKObjQuadNode( dwTreeDepth, 0, 0, nRight, nRight, fp );

	fclose( fp );

	return true;
}


//-----------------------------------------------------------------------------
// Name : LoadObj()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::LoadWat( const TCHAR* szFilename )
{
	SAFE_DELETE_ARRAY( ms_szFilename );
	ms_szFilename = new TCHAR[_tcslen( szFilename ) + 1];
	_tcscpy( ms_szFilename, szFilename );
	ms_szFilename[_tcslen( szFilename )] = NULL;

	FILE*	fp;
	fp = fopen( szFilename, "rb" );

	if( fp == NULL )
		return false;

	// Magic( WAT + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( ( 'W' << 24 ) | ( 'A' << 16 ) | ( 'T' << 8 ) ) + 10 )
		return false;

	// Data of vertex buffer of base water
	fread( &ms_dwBaseWaterVBSize, sizeof(DWORD), 1, fp );
	fseek( fp, sizeof(NKVERTEXLT) * ms_dwBaseWaterVBSize, SEEK_CUR );
	// Data of vertex buffer of wave water
	fread( &ms_dwWaveWaterVBSize, sizeof(DWORD), 1, fp );
	fseek( fp, sizeof(NKVERTEXLT) * ms_dwWaveWaterVBSize, SEEK_CUR );

	// Water heights of each nodes
	ms_pWaterHeights = new float[ms_nWaterHeightWidth * ms_nWaterHeightWidth];
	fread( ms_pWaterHeights, sizeof(float), ms_nWaterHeightWidth * ms_nWaterHeightWidth, fp );

	// Water textures
	int	nLength;
	TCHAR	szBaseName[512], szWave1Name[512], szWave2Name[512];
	fread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		fread( szBaseName, sizeof(TCHAR), nLength, fp );
	szBaseName[nLength] = NULL;
	fread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		fread( szWave1Name, sizeof(TCHAR), nLength, fp );
	szWave1Name[nLength] = NULL;
	fread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		fread( szWave2Name, sizeof(TCHAR), nLength, fp );
	szWave2Name[nLength] = NULL;

	fclose( fp );

	// Create vertex buffer
	if( !InitWaterVertexBuffer() )
		return false;

	ms_nBaseWaterTexID = ms_pIndexedTexture->CreateTextureFromFile( szBaseName );
	ms_nWaveWaterTexID1 = ms_pIndexedTexture->CreateTextureFromFile( szWave1Name );
	ms_nWaveWaterTexID2 = ms_pIndexedTexture->CreateTextureFromFile( szWave2Name );
	ms_pIndexedTexture->RestoreByIdx( ms_nBaseWaterTexID, g_pMyNKApp->GetD3DDevice() );
	ms_pIndexedTexture->RestoreByIdx( ms_nWaveWaterTexID1, g_pMyNKApp->GetD3DDevice() );
	ms_pIndexedTexture->RestoreByIdx( ms_nWaveWaterTexID2, g_pMyNKApp->GetD3DDevice() );

	DWORD	dwSize = g_pMyNKApp->GetCamera()->GetFrustumFar() / ms_nBaseWaterSize;
	SAFE_DELETE_ARRAY( ms_pBaseWaterIB );
	SAFE_DELETE_ARRAY( ms_pWaveWaterIB );
	ms_pBaseWaterIB = new WORD[dwSize * dwSize * 6];
	ms_pWaveWaterIB = new WORD[dwSize * dwSize * 6];
	memset( ms_pBaseWaterIB, 0, dwSize * dwSize * 6 );

	D3DUtil_SetIdentityMatrix( ms_matTexTrans );

	return true;
}


//-----------------------------------------------------------------------------
// Name : Destroy()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::Destroy()
{
	InvalidateDeviceObjects();

	SAFE_DELETE_ARRAY( ms_szFilename );
	SAFE_DELETE_ARRAY( ms_pWaterHeights );
	SAFE_DELETE_ARRAY( ms_pHeightOfWaterGroup );
	SAFE_DELETE_ARRAY( ms_pBaseWaterTempVB );
	SAFE_DELETE_ARRAY( ms_pWaveWaterTempVB );

	SAFE_DELETE_ARRAY( ms_pBaseWaterIB );
	SAFE_DELETE_ARRAY( ms_pWaveWaterIB );

	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	for( int h = 0; h < 64; h++ )
	{
		for( int w = 0; w < 64; w++ )
		{
			for( iterDeco = g_lstWalkObj[h][w].begin(); iterDeco != g_lstWalkObj[h][w].end(); )
			{
				pDecoObj = *iterDeco;
				delete pDecoObj;
				iterDeco = g_lstWalkObj[h][w].erase( iterDeco );
				if( iterDeco == g_lstWalkObj[h][w].end() )
					break;
			}
		}
	}

	SAFE_DELETE( g_pObjTree );
}


//-----------------------------------------------------------------------------
// Name : PrepareRender()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::PrepareRender()
{
	static float	fBaseWaterTime = 0.0f;
	static float	fWaveWaterTime = 0.0f;
	
	fBaseWaterTime += g_pMyNKApp->GetElapsedTime() * 0.07f;
	if( fBaseWaterTime > 1.0f )	fBaseWaterTime -= 1.0f;

	ms_fWaveWaterMove1 += g_pMyNKApp->GetElapsedTime() * 0.17f;
	if( ms_fWaveWaterMove1 > 1.0f )	ms_fWaveWaterMove1 -= 1.0f;

	ms_fWaveWaterMove2 += g_pMyNKApp->GetElapsedTime() * 0.12f;
	if( ms_fWaveWaterMove2 > 1.0f )	ms_fWaveWaterMove2 -= 1.0f;

	ms_fBaseWaterMove = sinf( fBaseWaterTime * NK_TWOPI ) * 0.1f;
	ms_dwBaseVertexCount = 0;
	ms_dwWaveVertexCount = 0;
	ms_dwBaseIndexCount = 0;
	ms_dwWaveIndexCount = 0;
}
/*


//-----------------------------------------------------------------------------
// Name : Render()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::Render( int nCullingState )
{
	if( nCullingState != VFCULL_INSIDE )
	{
		nCullingState = g_pMyNKApp->GetCamera()->CullAABB( m_vMin, m_vMax );
		if( nCullingState == VFCULL_OUTSIDE )
			return;
	}

	if( m_pLLeft == NULL )		// Render if this node is lowest level node
	{
		if( g_pMyNKApp->GetShowObject() )
		{
			BOOL						bAlpha = FALSE;
			BOOL						bOpenRoof = FALSE;
			CDecoObj*					pDecoObj;
			list<CDecoObj*>::iterator	iterDeco;

			for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
			{
				pDecoObj = *iterDeco;

				// Distance culling
				float	fDecoDist = DistPlToPlAbs( g_pMyNKApp->GetRohX(), g_pMyNKApp->GetRohZ(),
													pDecoObj->m_wx, pDecoObj->m_wz );
				if( fDecoDist <= ms_fRenderDist )	// DECO_DIST_CULLING in laghaim source is 1500.0f
				{
					// View frustum culling
					if( nCullingState == VFCULL_OVERLAP &&
						g_pMyNKApp->GetCamera()->CullPoints( (D3DXVECTOR3*)(pDecoObj->m_vBoundBox), 8 ) == VFCULL_OUTSIDE )
					{
						continue;
					}

					// Check if the roof of a decoobj is opened 
					if( g_pMyNKApp->GetApplyBlend() && g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId] <= BLD_HOUSE )
					{
						bOpenRoof = pDecoObj->IsInside( g_pMyNKApp->GetRohX(), g_pMyNKApp->GetRohZ() );
						if( bOpenRoof && pDecoObj->m_pLolo->m_WalkPlaneCount == 0 )
							ms_pOpenDeco = pDecoObj;
					}
					else
						bOpenRoof = FALSE;

					// Check if a character is hidden by a decoobj 
					if( g_pMyNKApp->GetApplyBlend() && g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId] <= BLD_TREE )
					{
						for( int i = 6; i > 1; i-- )
						{
							bAlpha = pDecoObj->IsInside( g_pMyNKApp->GetRohX() + ( g_pMyNKApp->GetCamera()->GetLocation().x - g_pMyNKApp->GetRohX() ) * i / 6,
														g_pMyNKApp->GetRohZ() + ( g_pMyNKApp->GetCamera()->GetLocation().z - g_pMyNKApp->GetRohZ() ) * i / 6 );
							if( bAlpha )
								break;
						}
					}
					else
						bAlpha = FALSE;

					if( bOpenRoof && bAlpha )
						bAlpha = FALSE;

					if( fDecoDist > ms_fBlendDist )		// DECO_DIST_BLEND in laghaim source is 1000.0f
					{
						pDecoObj->m_farFactor = ( ms_fRenderDist - fDecoDist ) / ( ms_fRenderDist - ms_fBlendDist );
						g_lstFarDeco.push_back( pDecoObj );
					}
					else if( bAlpha )
					{
						g_lstAlphaDeco.push_back( pDecoObj );
					}
					else if( pDecoObj->MustRenderLast() )
					{
						g_lstLastDeco.push_back( pDecoObj );
					}
					else
					{
						pDecoObj->Render( g_pMyNKApp->GetD3DDevice(), NULL, bOpenRoof );
					}
				}
			}
		}

		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			if( m_nBaseWaterSegment > 0 )
			{
				WORD	wVBIndex = m_wBaseVBIndex;
				for( int i = 0; i < m_nBaseWaterSegment; i++ )
				{
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 2;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 3;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 3;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 1;
					wVBIndex += 4;
				}

				if( m_nWaveWaterSegment1 > 0 )
				{
					wVBIndex = m_wWaveVBIndex;
					for( i = 0; i < m_nWaveWaterSegment1; i++ )
					{
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
						wVBIndex += 3;
					}

					if( m_nWaveWaterSegment2 > 0 )
					{
						wVBIndex += 3;
						for( i = 0; i < m_nWaveWaterSegment2; i++ )
						{
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
							wVBIndex += 3;
						}
					}
				}
			}
		}
		else
		{
			// Create water meshes
			if( g_pMyNKApp->GetShowWater() && m_nBaseWaterFlags > 0 )
			{
				int				i, x, z, nVert;
				WORD			wVBIndex;
				unsigned char	nMask = 1;
				float			fWaterHeight = ms_pHeightOfWaterGroup[m_nWaterGroupID];
				float			fX = m_nX * ms_nCellSize;
				float			fZ = m_nZ * ms_nCellSize;

				// Base water
				for( z = 0; z < 2; z++ )
				{
					for( x = 0; x < 2; x++ )
					{
						if( m_nBaseWaterFlags & nMask )
						{
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount + 2;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount + 3;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount + 3;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount + 1;

							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + x*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + z*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + (x+1)*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + z*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + x*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + (z+1)*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + (x+1)*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + (z+1)*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
						}
						nMask <<= 1;
					}
				}

				// Wave water
				if( m_nWaveWaterSegment1 > 0 )
				{
					wVBIndex = ms_dwWaveVertexCount;
					for( i = 0; i < m_nWaveWaterSegment1; i++ )
					{
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
						wVBIndex += 3;
					}

					nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
					for( i = 0; i < nVert; i++ )
						ms_pWaveWaterTempVB[ms_dwWaveVertexCount++] = m_pWaveWaterVert1[i];
				}

				if( m_nWaveWaterSegment2 > 0 )
				{
					wVBIndex = ms_dwWaveVertexCount;
					for( i = 0; i < m_nWaveWaterSegment2; i++ )
					{
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
						wVBIndex += 3;
					}

					nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
					for( i = 0; i < nVert; i++ )
						ms_pWaveWaterTempVB[ms_dwWaveVertexCount++] = m_pWaveWaterVert2[i];
				}
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->Render( nCullingState );
	m_pLRight->Render( nCullingState );
	m_pULeft->Render( nCullingState );
	m_pURight->Render( nCullingState );
}



  */

//-----------------------------------------------------------------------------
// Name : Render()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::Render( int nCullingState )
{
	if( nCullingState != VFCULL_INSIDE )
	{
		nCullingState = g_pMyNKApp->GetCamera()->CullAABB( m_vMin, m_vMax );
		if( nCullingState == VFCULL_OUTSIDE )
			return;
	}

	if( m_pLLeft == NULL )		// Render if this node is lowest level node
	{
		if( g_pMyNKApp->GetShowObject() )
		{
			BOOL						bAlpha = FALSE;
			BOOL						bOpenRoof = FALSE;
			CDecoObj*					pDecoObj;
			list<CDecoObj*>::iterator	iterDeco;

			for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
			{
				pDecoObj = *iterDeco;

				// Distance culling
				float	fDecoDist = DistPlToPlAbs( g_pMyNKApp->GetRohX(), g_pMyNKApp->GetRohZ(),
													pDecoObj->m_wx, pDecoObj->m_wz );

				const float BLD_FLOOR_Fist = 3.0f;// [2007/11/5 theodoric] 바닥 속성 추가
				
				// [2007/11/5 theodoric] 바닥 속성 추가 :: 카메라 컬링에 걸링에만 잘린다.
				if( fDecoDist <= ms_fRenderDist || 
					( g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId] == BLD_FLOOR && fDecoDist <= ms_fRenderDist * BLD_FLOOR_Fist ))	// DECO_DIST_CULLING in laghaim source is 1500.0f
				{
					// View frustum culling
					if( nCullingState == VFCULL_OVERLAP &&
						g_pMyNKApp->GetCamera()->CullPoints( (D3DXVECTOR3*)(pDecoObj->m_vBoundBox), 8 ) == VFCULL_OUTSIDE )
					{
						continue;
					}

					// Check if the roof of a decoobj is opened 
					if( g_pMyNKApp->GetApplyBlend() && g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId] <= BLD_HOUSE )
					{
						bOpenRoof = pDecoObj->IsInside( g_pMyNKApp->GetRohX(), g_pMyNKApp->GetRohZ() );
						if( bOpenRoof && pDecoObj->m_pLolo->m_WalkPlaneCount == 0 )
							ms_pOpenDeco = pDecoObj;
					}
					else
					{
						// [2007/11/5 theodoric] 바닥 속성 추가
						if( g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId] != BLD_FLOOR )
							bOpenRoof = FALSE;
					}

					// Check if a character is hidden by a decoobj 
					if( g_pMyNKApp->GetApplyBlend() && g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId] <= BLD_TREE )
					{
						for( int i = 6; i > 1; i-- )
						{
							bAlpha = pDecoObj->IsInside( g_pMyNKApp->GetRohX() + ( g_pMyNKApp->GetCamera()->GetLocation().x - g_pMyNKApp->GetRohX() ) * i / 6,
														g_pMyNKApp->GetRohZ() + ( g_pMyNKApp->GetCamera()->GetLocation().z - g_pMyNKApp->GetRohZ() ) * i / 6 );
							if( bAlpha )
								break;
						}
					}
					else
						bAlpha = FALSE;

					if( bOpenRoof && bAlpha )
						bAlpha = FALSE;

				
					if( g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId] == BLD_FLOOR && fDecoDist > ms_fBlendDist*BLD_FLOOR_Fist )
					{
						pDecoObj->m_farFactor = ( ms_fRenderDist*BLD_FLOOR_Fist - fDecoDist ) / ( ms_fRenderDist*BLD_FLOOR_Fist - ms_fBlendDist*BLD_FLOOR_Fist );
						g_lstFarDeco.push_back( pDecoObj );	
					}
					else if( fDecoDist > ms_fBlendDist &&g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId] != BLD_FLOOR ) 		// DECO_DIST_BLEND in laghaim source is 1000.0f
					{						
							pDecoObj->m_farFactor = ( ms_fRenderDist - fDecoDist ) / ( ms_fRenderDist - ms_fBlendDist );
							g_lstFarDeco.push_back( pDecoObj );
					}
					else if( bAlpha )
					{
						g_lstAlphaDeco.push_back( pDecoObj );
					}
					else if( pDecoObj->MustRenderLast() )
					{
						g_lstLastDeco.push_back( pDecoObj );
					}
					else
					{
						pDecoObj->Render( g_pMyNKApp->GetD3DDevice(), NULL, bOpenRoof );
					}										
				}
			}
		}

		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			if( m_nBaseWaterSegment > 0 )
			{
				WORD	wVBIndex = m_wBaseVBIndex;
				int i = 0;
				for( i = 0; i < m_nBaseWaterSegment; i++ )
				{
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 2;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 3;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 3;
					ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 1;
					wVBIndex += 4;
				}

				if( m_nWaveWaterSegment1 > 0 )
				{
					wVBIndex = m_wWaveVBIndex;
					for( i = 0; i < m_nWaveWaterSegment1; i++ )
					{
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
						wVBIndex += 3;
					}

					if( m_nWaveWaterSegment2 > 0 )
					{
						wVBIndex += 3;
						for( i = 0; i < m_nWaveWaterSegment2; i++ )
						{
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
							ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
							wVBIndex += 3;
						}
					}
				}
			}
		}
		else
		{
			// Create water meshes
			if( g_pMyNKApp->GetShowWater() && m_nBaseWaterFlags > 0 )
			{
				int				i, x, z, nVert;
				WORD			wVBIndex;
				unsigned char	nMask = 1;
				float			fWaterHeight = ms_pHeightOfWaterGroup[m_nWaterGroupID];
				float			fX = m_nX * ms_nCellSize;
				float			fZ = m_nZ * ms_nCellSize;

				// Base water
				for( z = 0; z < 2; z++ )
				{
					for( x = 0; x < 2; x++ )
					{
						if( m_nBaseWaterFlags & nMask )
						{
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount + 2;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount + 3;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount + 3;
							ms_pBaseWaterIB[ms_dwBaseIndexCount++] = ms_dwBaseVertexCount + 1;

							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + x*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + z*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + (x+1)*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + z*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + x*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + (z+1)*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + (x+1)*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + (z+1)*ms_nBaseWaterSize;
							ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
						}
						nMask <<= 1;
					}
				}

				// Wave water
				if( m_nWaveWaterSegment1 > 0 )
				{
					wVBIndex = ms_dwWaveVertexCount;
					for( i = 0; i < m_nWaveWaterSegment1; i++ )
					{
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
						wVBIndex += 3;
					}

					nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
					for( i = 0; i < nVert; i++ )
						ms_pWaveWaterTempVB[ms_dwWaveVertexCount++] = m_pWaveWaterVert1[i];
				}

				if( m_nWaveWaterSegment2 > 0 )
				{
					wVBIndex = ms_dwWaveVertexCount;
					for( i = 0; i < m_nWaveWaterSegment2; i++ )
					{
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
						wVBIndex += 3;
					}

					nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
					for( i = 0; i < nVert; i++ )
						ms_pWaveWaterTempVB[ms_dwWaveVertexCount++] = m_pWaveWaterVert2[i];
				}
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->Render( nCullingState );
	m_pLRight->Render( nCullingState );
	m_pULeft->Render( nCullingState );
	m_pURight->Render( nCullingState );
}


//-----------------------------------------------------------------------------
// Name : RenderWater()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RenderWater()
{
	// Get texture stage state
	DWORD	dwColorOp, dwColorArg1, dwAlphaOp, dwAlphaArg1;
	ms_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &dwColorOp );
	ms_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColorArg1 );
	ms_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwAlphaOp );
	ms_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg1 );

	// Get render state
	DWORD	dwSrcBlend, dwDestBlend;
	ms_pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcBlend );
	ms_pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestBlend );

	// Set texture stage state
	ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );

	// Set render state
	ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

	// Set world transform
	ms_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, (D3DMATRIX*)(&NKMAT_IDENTITY) );

	// Base water
	if( ms_dwBaseIndexCount > 0 )
	{
		// Set texture stage state
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );

		// Set render state
		ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
		
		ms_pd3dDevice->SetTexture( 0, ms_pIndexedTexture->GetSurfaceByIdx( ms_nBaseWaterTexID ) );
		ms_matTexTrans._31 = ms_fBaseWaterMove;
		ms_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &ms_matTexTrans );
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			ms_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, ms_pBaseWaterVB, 0, ms_dwBaseWaterVBSize,
													ms_pBaseWaterIB, ms_dwBaseIndexCount, 0 );
		}
		else
		{
			ms_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, NKVERTEXLT::FVF, ms_pBaseWaterTempVB,
													ms_dwBaseWaterVBSize, ms_pBaseWaterIB, ms_dwBaseIndexCount, 0 );
		}
	}

	// Wave water
	if( ms_dwWaveIndexCount > 0 )
	{
		// Set texture stage state
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// Set render state
		ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
		ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
		ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
				
		ms_matTexTrans._31 = ms_fWaveWaterMove1;
		ms_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &ms_matTexTrans );
		ms_pd3dDevice->SetTexture( 0, ms_pIndexedTexture->GetSurfaceByIdx( ms_nWaveWaterTexID1 ) );
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			ms_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, ms_pWaveWaterVB, 0, ms_dwWaveWaterVBSize,
													ms_pWaveWaterIB, ms_dwWaveIndexCount, 0 );
		}
		else
		{
			ms_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, NKVERTEXLT::FVF, ms_pWaveWaterTempVB,
													ms_dwWaveWaterVBSize, ms_pWaveWaterIB, ms_dwWaveIndexCount, 0 );
		}

		ms_matTexTrans._31 = ms_fWaveWaterMove2;
		ms_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &ms_matTexTrans );
		ms_pd3dDevice->SetTexture( 0, ms_pIndexedTexture->GetSurfaceByIdx( ms_nWaveWaterTexID2 ) );
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			ms_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, ms_pWaveWaterVB, 0, ms_dwWaveWaterVBSize,
													ms_pWaveWaterIB, ms_dwWaveIndexCount, 0 );
		}
		else
		{
			ms_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, NKVERTEXLT::FVF, ms_pWaveWaterTempVB,
													ms_dwWaveWaterVBSize, ms_pWaveWaterIB, ms_dwWaveIndexCount, 0 );
		}

		ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	}

	// Restore render state
	ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
	ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcBlend );
	ms_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestBlend );

	// Restore texture stage state
	ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, dwColorOp );
	ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColorArg1 );
	ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwAlphaOp );
	ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwAlphaArg1 );
	ms_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
}


//-----------------------------------------------------------------------------
// Name : IsDecoObjInNode()
// Desc :
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::IsDecoObjInNode( CDecoObj* pDecoObj )
{
	if( NKTestPointsInRect( m_vMin.x, m_vMin.z, m_vMax.x, m_vMax.z,
							(D3DXVECTOR3*)(pDecoObj->m_vBoundBox), 8 ) == 2 )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Name : IsAABBInNode()
// Desc :
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::IsAABBInNode( const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax )
{
	if( NKTestRectInRect( m_vMin.x, m_vMin.z, m_vMax.x, m_vMax.z, rMin.x, rMin.z, rMax.x, rMax.z ) == 2 )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Name : HasThisDecoObj()
// Desc :
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::HasThisDecoObj( CDecoObj* pDecoObj )
{
	CDecoObj*					pFindDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	iterDeco = find( m_lstDecoObj.begin(), m_lstDecoObj.end(), pDecoObj );

	pFindDecoObj = *iterDeco;	
	if( pFindDecoObj == pDecoObj )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Name : FindQuadNode()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::FindQuadNode( CDecoObj* pDecoObj, CNKObjQuadNode** ppQuadNode, bool& bFind )
{
	if( bFind )
		return ;

	if( m_pLLeft == NULL )		// If this node is lowest level node, check a list of decoobj
	{
		if( HasThisDecoObj( pDecoObj ) )
		{
			*ppQuadNode = this;
			bFind = true;
		}

		return ;
	}

	// Recursive call
	m_pLLeft->FindQuadNode( pDecoObj, ppQuadNode, bFind );
	m_pLRight->FindQuadNode( pDecoObj, ppQuadNode, bFind );
	m_pULeft->FindQuadNode( pDecoObj, ppQuadNode, bFind );
	m_pURight->FindQuadNode( pDecoObj, ppQuadNode, bFind );
}


//-----------------------------------------------------------------------------
// Name : FindDecoObjFittedNode()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::FindDecoObjFittedNode( CDecoObj* pDecoObj, CNKObjQuadNode** ppNode, float& fArea )
{
	if( NKTestPointsInRect( m_vMin.x, m_vMin.z, m_vMax.x, m_vMax.z,
							(D3DXVECTOR3*)(pDecoObj->m_vBoundBox), 8 ) == 0 )
		return ;

	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		D3DXVECTOR3	vMin = m_vMin;
		D3DXVECTOR3	vMax = m_vMax;
		for( int i = 0; i < 8; i++ )
		{
			if( pDecoObj->m_vBoundBox[i].x < vMin.x ) vMin.x = pDecoObj->m_vBoundBox[i].x;
			else if( pDecoObj->m_vBoundBox[i].x > vMax.x ) vMax.x = pDecoObj->m_vBoundBox[i].x;
			if( pDecoObj->m_vBoundBox[i].z < vMin.z ) vMin.z = pDecoObj->m_vBoundBox[i].z;
			else if( pDecoObj->m_vBoundBox[i].z > vMax.z ) vMax.z = pDecoObj->m_vBoundBox[i].z;
		}

		float	fArea2 = ( vMax.x - vMin.x ) * ( vMax.z - vMin.z );
		if( fArea2 < fArea )
		{
			*ppNode = this;
			fArea = fArea2;
		}

		return ;
	}

	// Recursive call
	m_pLLeft->FindDecoObjFittedNode( pDecoObj, ppNode, fArea );
	m_pLRight->FindDecoObjFittedNode( pDecoObj, ppNode, fArea );
	m_pULeft->FindDecoObjFittedNode( pDecoObj, ppNode, fArea );
	m_pURight->FindDecoObjFittedNode( pDecoObj, ppNode, fArea );
}


//-----------------------------------------------------------------------------
// Name : FindAABBFittedNode()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::FindAABBFittedNode( const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax,
											CNKObjQuadNode** ppNode, float& fArea )
{
	if( NKTestRectInRect( m_vMin.x, m_vMin.z, m_vMax.x, m_vMax.z, rMin.x, rMin.z, rMax.x, rMax.z ) == 0 )
		return ;

	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		D3DXVECTOR3	vMin = m_vMin;
		D3DXVECTOR3	vMax = m_vMax;

		if( rMin.x < vMin.x ) vMin.x = rMin.x;
		if( rMax.x > vMax.x ) vMax.x = rMax.x;
		if( rMin.z < vMin.z ) vMin.z = rMin.z;
		if( rMax.z > vMax.z ) vMax.z = rMax.z;

		float	fArea2 = ( vMax.x - vMin.x ) * ( vMax.z - vMin.z );
		if( fArea2 < fArea )
		{
			*ppNode = this;
			fArea = fArea2;
		}

		return ;
	}

	// Recursive call
	m_pLLeft->FindAABBFittedNode( rMin, rMax, ppNode, fArea );
	m_pLRight->FindAABBFittedNode( rMin, rMax, ppNode, fArea );
	m_pULeft->FindAABBFittedNode( rMin, rMax, ppNode, fArea );
	m_pURight->FindAABBFittedNode( rMin, rMax, ppNode, fArea );
}


//-----------------------------------------------------------------------------
// Name : GetPickedDecoObj()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::GetPickedDecoObj( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir,
										float& rDist, CDecoObj** ppPickedDecoObj, CNKObjQuadNode** ppPickedNode )
{
	if( !NKIntersectRayAABB( rOrigin, rDir, m_vMin, m_vMax ) )
		return ;

	// Test a intersection between a ray and OBB if this node is lowest level node
	if( m_pLLeft == NULL )
	{
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		float						fDist2;
		D3DXVECTOR3					aTriVert[3];
		bool						bFind;

		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			bFind = false;
			pDecoObj = *iterDeco;
			if( NKIntersectRayOBB( rOrigin, rDir, (D3DXVECTOR3*)(pDecoObj->m_vBoundBox), &fDist2 ) )
			{
				if( fDist2 < rDist )
				{
					bFind = true;
					rDist = fDist2;
					*ppPickedDecoObj = pDecoObj;
					if( ppPickedNode )
						*ppPickedNode = this;

/*
					fX = pDecoObj->m_wx;
					fY = pDecoObj->m_wy;
					fZ = pDecoObj->m_wz;

					for( nLolo = 0; bFind == false && nLolo < pDecoObj->m_pLolo->m_LoloObjCount; nLolo++ )
					{
						pLoloObj = &(pDecoObj->m_pLolo->m_LoloObjs[nLolo]);
						pVertices = pLoloObj->m_Vertices[0];
						pIndices = pLoloObj->m_Indexed;


						for( nFace = 0; bFind == false && nFace < pLoloObj->m_FaceCount; nFace++ )
						{
							LagVert = pVertices[pIndices[nFace*3]];
							aTriVert[0] = D3DXVECTOR3( LagVert.x + fX, LagVert.y + fY, LagVert.z + fZ );
							LagVert = pVertices[pIndices[nFace*3+1]];
							aTriVert[1] = D3DXVECTOR3( LagVert.x + fX, LagVert.y + fY, LagVert.z + fZ );
							LagVert = pVertices[pIndices[nFace*3+2]];
							aTriVert[2] = D3DXVECTOR3( LagVert.x + fX, LagVert.y + fY, LagVert.z + fZ );

							if( NKIntersectRayTri( rOrigin, rDir, aTriVert[0], aTriVert[1], aTriVert[2], &fDist2 ) )
							{
								if( fDist2 < rDist )
								{
									bFind = true;
									rDist = fDist2;
									*ppPickedDecoObj = pDecoObj;
									if( ppPickedNode )
										*ppPickedNode = this;
								}
							}
						}
					}
*/
				}
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->GetPickedDecoObj( rOrigin, rDir, rDist, ppPickedDecoObj, ppPickedNode );
	m_pLRight->GetPickedDecoObj( rOrigin, rDir, rDist, ppPickedDecoObj, ppPickedNode );
	m_pULeft->GetPickedDecoObj( rOrigin, rDir, rDist, ppPickedDecoObj, ppPickedNode );
	m_pURight->GetPickedDecoObj( rOrigin, rDir, rDist, ppPickedDecoObj, ppPickedNode );
}


//-----------------------------------------------------------------------------
// Name : AddDecoObj()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::AddDecoObj( CDecoObj* pDecoObj )
{
	// Add a decoobj into a list of decoobjs
	m_lstDecoObj.push_back( pDecoObj );

	// Recalculate a bounding box of a current node
	D3DXVECTOR3	vMin = m_vMin;
	D3DXVECTOR3 vMax = m_vMax;

	for( int i = 0; i < 8; i++ )
	{
		if( pDecoObj->m_vBoundBox[i].x < vMin.x ) vMin.x = pDecoObj->m_vBoundBox[i].x;
		else if( pDecoObj->m_vBoundBox[i].x > vMax.x ) vMax.x = pDecoObj->m_vBoundBox[i].x;
		if( pDecoObj->m_vBoundBox[i].y < vMin.y ) vMin.y = pDecoObj->m_vBoundBox[i].y;
		if( pDecoObj->m_vBoundBox[i].y > vMax.y ) vMax.y = pDecoObj->m_vBoundBox[i].y;
		if( pDecoObj->m_vBoundBox[i].z < vMin.z ) vMin.z = pDecoObj->m_vBoundBox[i].z;
		else if( pDecoObj->m_vBoundBox[i].z > vMax.z ) vMax.z = pDecoObj->m_vBoundBox[i].z;
	}

	if( vMin != m_vMin || vMax != m_vMax )
	{
		m_vMin = vMin;
		m_vMax = vMax;

		// Recalculate a bounding box of a upper node
		RecalcUpperBoundingBox();
	}
}


//-----------------------------------------------------------------------------
// Name : ModifyDecoObjLolo()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::ModifyDecoObjLolo( int nIndex )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		bool						bChanged = false;
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			if( pDecoObj->m_nLoloId == nIndex )
			{
				bChanged = true;
				pDecoObj->m_pLolo = g_pDecoObjSet->m_vecLoloSet[nIndex];
				pDecoObj->CalBoundBox();
			}
		}

		// Recalculate a bounding box of a current node and a upper node 
		if( bChanged )
			RecalcBoundingBox();

		return ;
	}

	// Recursive call
	m_pLLeft->ModifyDecoObjLolo( nIndex );
	m_pLRight->ModifyDecoObjLolo( nIndex );
	m_pULeft->ModifyDecoObjLolo( nIndex );
	m_pURight->ModifyDecoObjLolo( nIndex );
}


//-----------------------------------------------------------------------------
// Name : ModifyDecoObjLoloID()
// Desc : When a decoobj is inserted, nValue is +1
//        when a decoobj is removed, nValud is -1
//-----------------------------------------------------------------------------
void CNKObjQuadNode::ModifyDecoObjLoloID( int nIndex, int nValue )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			if( pDecoObj->m_nLoloId >= nIndex )
				pDecoObj->m_nLoloId += nValue;
		}

		return ;
	}

	// Recursive call
	m_pLLeft->ModifyDecoObjLoloID( nIndex, nValue );
	m_pLRight->ModifyDecoObjLoloID( nIndex, nValue );
	m_pULeft->ModifyDecoObjLoloID( nIndex, nValue );
	m_pURight->ModifyDecoObjLoloID( nIndex, nValue );
}


//-----------------------------------------------------------------------------
// Name : RemoveDecoObj()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RemoveDecoObj( CDecoObj* pDecoObj, bool bRelease )
{
	// Remove a decoobj from a list of decoobjs
	list<CDecoObj*>::iterator	iterDeco = find( m_lstDecoObj.begin(), m_lstDecoObj.end(), pDecoObj );
	m_lstDecoObj.erase( iterDeco );
	if( bRelease )
		delete pDecoObj;

	// Recalculate a bounding box of a current node and a upper node 
	RecalcBoundingBox();
}


//-----------------------------------------------------------------------------
// Name : RemoveDecoObj()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RemoveDecoObjs( int nIndex )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		// Remove decoobjs from a list of decoobjs
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); )
		{
			pDecoObj = *iterDeco;
			if( pDecoObj->m_nLoloId == nIndex )
			{
				iterDeco = m_lstDecoObj.erase( iterDeco );
				delete pDecoObj;
			}
			else
				iterDeco++;
		}

		// Recalculate a bounding box of a current node and a upper node 
		RecalcBoundingBox();

		return ;
	}

	// Recursive call
	m_pLLeft->RemoveDecoObjs( nIndex );
	m_pLRight->RemoveDecoObjs( nIndex );
	m_pULeft->RemoveDecoObjs( nIndex );
	m_pURight->RemoveDecoObjs( nIndex );
}


//-----------------------------------------------------------------------------
// Name : RecalcBoundingBox()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RecalcBoundingBox()
{
	// Recalculate a bounding box of a current node
	m_vMin = D3DXVECTOR3( m_nX * ms_nCellSize, FLT_MAX, m_nZ * ms_nCellSize );
	m_vMax = D3DXVECTOR3( ( m_nX + 1 ) * ms_nCellSize, -FLT_MAX, ( m_nZ + 1 ) * ms_nCellSize );

	int							i;
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		for( i = 0; i < 8; i++ )
		{
			if( pDecoObj->m_vBoundBox[i].x < m_vMin.x ) m_vMin.x = pDecoObj->m_vBoundBox[i].x;
			else if( pDecoObj->m_vBoundBox[i].x > m_vMax.x ) m_vMax.x = pDecoObj->m_vBoundBox[i].x;
			if( pDecoObj->m_vBoundBox[i].y < m_vMin.y ) m_vMin.y = pDecoObj->m_vBoundBox[i].y;
			if( pDecoObj->m_vBoundBox[i].y > m_vMax.y ) m_vMax.y = pDecoObj->m_vBoundBox[i].y;
			if( pDecoObj->m_vBoundBox[i].z < m_vMin.z ) m_vMin.z = pDecoObj->m_vBoundBox[i].z;
			else if( pDecoObj->m_vBoundBox[i].z > m_vMax.z ) m_vMax.z = pDecoObj->m_vBoundBox[i].z;
		}
	}

	if( m_nBaseWaterFlags > 0 )
	{
		if( m_vMin.y > ms_pHeightOfWaterGroup[m_nWaterGroupID] )
			m_vMin.y = ms_pHeightOfWaterGroup[m_nWaterGroupID];
		if( m_vMax.y < ms_pHeightOfWaterGroup[m_nWaterGroupID] + 0.3f )
			m_vMax.y = ms_pHeightOfWaterGroup[m_nWaterGroupID] + 0.3f;

		int	nVert;
		if( m_nWaveWaterSegment1 > 0 )
		{
			nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
			for( i = 0; i < nVert; i++ )
			{
				if( m_vMin.x > m_pWaveWaterVert1[i].x ) m_vMin.x = m_pWaveWaterVert1[i].x;
				else if( m_vMax.x < m_pWaveWaterVert1[i].x ) m_vMax.x = m_pWaveWaterVert1[i].x;
				if( m_vMin.y > m_pWaveWaterVert1[i].y ) m_vMin.y = m_pWaveWaterVert1[i].y;
				if( m_vMax.y < m_pWaveWaterVert1[i].y ) m_vMax.y = m_pWaveWaterVert1[i].y;
				if( m_vMin.z > m_pWaveWaterVert1[i].z ) m_vMin.z = m_pWaveWaterVert1[i].z;				
				else if( m_vMax.z < m_pWaveWaterVert1[i].z ) m_vMax.z = m_pWaveWaterVert1[i].z;
			}
		}

		if( m_nWaveWaterSegment2 > 0 )
		{
			nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
			for( i = 0; i < nVert; i++ )
			{
				if( m_vMin.x > m_pWaveWaterVert2[i].x ) m_vMin.x = m_pWaveWaterVert2[i].x;
				else if( m_vMax.x < m_pWaveWaterVert2[i].x ) m_vMax.x = m_pWaveWaterVert2[i].x;
				if( m_vMin.y > m_pWaveWaterVert2[i].y ) m_vMin.y = m_pWaveWaterVert2[i].y;
				if( m_vMax.y < m_pWaveWaterVert2[i].y ) m_vMax.y = m_pWaveWaterVert2[i].y;
				if( m_vMin.z > m_pWaveWaterVert2[i].z ) m_vMin.z = m_pWaveWaterVert2[i].z;				
				else if( m_vMax.z < m_pWaveWaterVert2[i].z ) m_vMax.z = m_pWaveWaterVert2[i].z;
			}
		}
	}

	// Recalculate a bounding box of a upper node
	RecalcUpperBoundingBox();
}


//-----------------------------------------------------------------------------
// Name : RecalcUpperBoundingBox()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RecalcUpperBoundingBox()
{
	// Recalaculate a bounding box
	if( m_pLLeft )
	{
		D3DXVECTOR3	vMin, vMax;
		D3DXVECTOR3	aVert[7];
		aVert[0] = m_pLLeft->m_vMin;
		aVert[1] = m_pLLeft->m_vMax;
		aVert[2] = m_pLRight->m_vMin;
		aVert[3] = m_pLRight->m_vMax;
		aVert[4] = m_pULeft->m_vMin;
		aVert[5] = m_pULeft->m_vMax;
		aVert[6] = m_pURight->m_vMin;
		vMin = m_pURight->m_vMax;
		vMax = vMin;

		for( int i = 0; i < 7; i++ )
		{
			if( aVert[i].x < vMin.x ) vMin.x = aVert[i].x;
			else if( aVert[i].x > vMax.x ) vMax.x = aVert[i].x;
			if( aVert[i].y < vMin.y ) vMin.y = aVert[i].y;
			else if( aVert[i].y > vMax.y ) vMax.y = aVert[i].y;
			if( aVert[i].z < vMin.z ) vMin.z = aVert[i].z;
			else if( aVert[i].z > vMax.z ) vMax.z = aVert[i].z;
		}

		m_vMin = vMin;
		m_vMax = vMax;
	}

	if( m_pParent == NULL )
		return ;

	// Recursive call
	m_pParent->RecalcUpperBoundingBox();
}


//-----------------------------------------------------------------------------
// Name : RecalcAllBoundingBox()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RecalcAllBoundingBox()
{
	if( m_pLLeft == NULL )			// Lowest level node
	{
		// Calculate a bounding box of lowest level node
		m_vMin = D3DXVECTOR3( m_nX * ms_nCellSize, FLT_MAX, m_nZ * ms_nCellSize );
		m_vMax = D3DXVECTOR3( ( m_nX + 1 ) * ms_nCellSize, -FLT_MAX, ( m_nZ + 1 ) * ms_nCellSize );

		int							i;
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			for( i = 0; i < 8; i++ )
			{
				if( pDecoObj->m_vBoundBox[i].x < m_vMin.x ) m_vMin.x = pDecoObj->m_vBoundBox[i].x;
				else if( pDecoObj->m_vBoundBox[i].x > m_vMax.x ) m_vMax.x = pDecoObj->m_vBoundBox[i].x;
				if( pDecoObj->m_vBoundBox[i].y < m_vMin.y ) m_vMin.y = pDecoObj->m_vBoundBox[i].y;
				if( pDecoObj->m_vBoundBox[i].y > m_vMax.y ) m_vMax.y = pDecoObj->m_vBoundBox[i].y;
				if( pDecoObj->m_vBoundBox[i].z < m_vMin.z ) m_vMin.z = pDecoObj->m_vBoundBox[i].z;
				else if( pDecoObj->m_vBoundBox[i].z > m_vMax.z ) m_vMax.z = pDecoObj->m_vBoundBox[i].z;
			}
		}

		if( m_nBaseWaterFlags > 0 )
		{
			if( m_vMin.y > ms_pHeightOfWaterGroup[m_nWaterGroupID] )
				m_vMin.y = ms_pHeightOfWaterGroup[m_nWaterGroupID];
			if( m_vMax.y < ms_pHeightOfWaterGroup[m_nWaterGroupID] + 0.3f )
				m_vMax.y = ms_pHeightOfWaterGroup[m_nWaterGroupID] + 0.3f;

			int	nVert;
			if( m_nWaveWaterSegment1 > 0 )
			{
				nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
				for( i = 0; i < nVert; i++ )
				{
					if( m_vMin.x > m_pWaveWaterVert1[i].x ) m_vMin.x = m_pWaveWaterVert1[i].x;
					else if( m_vMax.x < m_pWaveWaterVert1[i].x ) m_vMax.x = m_pWaveWaterVert1[i].x;
					if( m_vMin.y > m_pWaveWaterVert1[i].y ) m_vMin.y = m_pWaveWaterVert1[i].y;
					if( m_vMax.y < m_pWaveWaterVert1[i].y ) m_vMax.y = m_pWaveWaterVert1[i].y;
					if( m_vMin.z > m_pWaveWaterVert1[i].z ) m_vMin.z = m_pWaveWaterVert1[i].z;					
					else if( m_vMax.z < m_pWaveWaterVert1[i].z ) m_vMax.z = m_pWaveWaterVert1[i].z;
				}
			}

			if( m_nWaveWaterSegment2 > 0 )
			{
				nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
				for( i = 0; i < nVert; i++ )
				{
					if( m_vMin.x > m_pWaveWaterVert2[i].x ) m_vMin.x = m_pWaveWaterVert2[i].x;
					else if( m_vMax.x < m_pWaveWaterVert2[i].x ) m_vMax.x = m_pWaveWaterVert2[i].x;
					if( m_vMin.y > m_pWaveWaterVert2[i].y ) m_vMin.y = m_pWaveWaterVert2[i].y;
					if( m_vMax.y < m_pWaveWaterVert2[i].y ) m_vMax.y = m_pWaveWaterVert2[i].y;
					if( m_vMin.z > m_pWaveWaterVert2[i].z ) m_vMin.z = m_pWaveWaterVert2[i].z;
					else if( m_vMax.z < m_pWaveWaterVert2[i].z ) m_vMax.z = m_pWaveWaterVert2[i].z;
				}
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->RecalcAllBoundingBox();
	m_pLRight->RecalcAllBoundingBox();
	m_pULeft->RecalcAllBoundingBox();
	m_pURight->RecalcAllBoundingBox();


	// Calculate a bounding box of current level node
	D3DXVECTOR3	vMin, vMax;
	D3DXVECTOR3	aVert[7];
	aVert[0] = m_pLLeft->m_vMin;
	aVert[1] = m_pLLeft->m_vMax;
	aVert[2] = m_pLRight->m_vMin;
	aVert[3] = m_pLRight->m_vMax;
	aVert[4] = m_pULeft->m_vMin;
	aVert[5] = m_pULeft->m_vMax;
	aVert[6] = m_pURight->m_vMin;
	vMin = m_pURight->m_vMax;
	vMax = vMin;

	for( int i = 0; i < 7; i++ )
	{
		if( aVert[i].x < vMin.x ) vMin.x = aVert[i].x;
		else if( aVert[i].x > vMax.x ) vMax.x = aVert[i].x;
		if( aVert[i].y < vMin.y ) vMin.y = aVert[i].y;
		else if( aVert[i].y > vMax.y ) vMax.y = aVert[i].y;
		if( aVert[i].z < vMin.z ) vMin.z = aVert[i].z;
		else if( aVert[i].z > vMax.z ) vMax.z = aVert[i].z;
	}

	// Set a bounding box of current level node
	m_vMin = vMin;
	m_vMax = vMax;	
}


//-----------------------------------------------------------------------------
// Name : RecalcAllBoundingBoxForSaving()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RecalcAllBoundingBoxForSaving()
{
	if( m_pLLeft == NULL )			// Lowest level node
	{
		// Calculate a bounding box of lowest level node
		m_vMin = D3DXVECTOR3( FLT_MAX, FLT_MAX, FLT_MAX );
		m_vMax = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX );

		int							i;
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			for( i = 0; i < 8; i++ )
			{
				if( pDecoObj->m_vBoundBox[i].x < m_vMin.x ) m_vMin.x = pDecoObj->m_vBoundBox[i].x;
				if( pDecoObj->m_vBoundBox[i].x > m_vMax.x ) m_vMax.x = pDecoObj->m_vBoundBox[i].x;
				if( pDecoObj->m_vBoundBox[i].y < m_vMin.y ) m_vMin.y = pDecoObj->m_vBoundBox[i].y;
				if( pDecoObj->m_vBoundBox[i].y > m_vMax.y ) m_vMax.y = pDecoObj->m_vBoundBox[i].y;
				if( pDecoObj->m_vBoundBox[i].z < m_vMin.z ) m_vMin.z = pDecoObj->m_vBoundBox[i].z;
				if( pDecoObj->m_vBoundBox[i].z > m_vMax.z ) m_vMax.z = pDecoObj->m_vBoundBox[i].z;
			}
		}

		if( m_nBaseWaterFlags > 0 )
		{
			int				x, z;
			unsigned char	nMask = 1;
			float			fX = m_nX * ms_nCellSize;
			float			fZ = m_nZ * ms_nCellSize;
			D3DXVECTOR3		vMin, vMax;

			if( m_vMin.y > ms_pHeightOfWaterGroup[m_nWaterGroupID] )
				m_vMin.y = ms_pHeightOfWaterGroup[m_nWaterGroupID];
			if( m_vMax.y < ms_pHeightOfWaterGroup[m_nWaterGroupID] + 0.3f )
				m_vMax.y = ms_pHeightOfWaterGroup[m_nWaterGroupID] + 0.3f;

			for( z = 0; z < 2; z++ )
			{
				for( x = 0; x < 2; x++ )
				{
					if( m_nBaseWaterFlags & nMask )
					{
						vMin.x = fX + x * ms_nBaseWaterSize;
						vMin.z = fZ + z * ms_nBaseWaterSize;
						vMax.x = fX + ( x + 1 ) * ms_nBaseWaterSize;
						vMax.z = fZ + ( z + 1 ) * ms_nBaseWaterSize;
						if( m_vMin.x > vMin.x ) m_vMin.x = vMin.x;
						if( m_vMin.z > vMin.z ) m_vMin.z = vMin.z;
						if( m_vMax.x < vMax.x ) m_vMax.x = vMax.x;
						if( m_vMax.z < vMax.z ) m_vMax.z = vMax.z;
					}
					nMask <<= 1;
				}
			}

			int	nVert;
			if( m_nWaveWaterSegment1 > 0 )
			{
				nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
				for( i = 0; i < nVert; i++ )
				{
					if( m_vMin.x > m_pWaveWaterVert1[i].x ) m_vMin.x = m_pWaveWaterVert1[i].x;
					if( m_vMax.x < m_pWaveWaterVert1[i].x ) m_vMax.x = m_pWaveWaterVert1[i].x;
					if( m_vMin.y > m_pWaveWaterVert1[i].y ) m_vMin.y = m_pWaveWaterVert1[i].y;
					if( m_vMax.y < m_pWaveWaterVert1[i].y ) m_vMax.y = m_pWaveWaterVert1[i].y;
					if( m_vMin.z > m_pWaveWaterVert1[i].z ) m_vMin.z = m_pWaveWaterVert1[i].z;					
					if( m_vMax.z < m_pWaveWaterVert1[i].z ) m_vMax.z = m_pWaveWaterVert1[i].z;
				}
			}

			if( m_nWaveWaterSegment2 > 0 )
			{
				nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
				for( i = 0; i < nVert; i++ )
				{
					if( m_vMin.x > m_pWaveWaterVert2[i].x ) m_vMin.x = m_pWaveWaterVert2[i].x;
					if( m_vMax.x < m_pWaveWaterVert2[i].x ) m_vMax.x = m_pWaveWaterVert2[i].x;
					if( m_vMin.y > m_pWaveWaterVert2[i].y ) m_vMin.y = m_pWaveWaterVert2[i].y;
					if( m_vMax.y < m_pWaveWaterVert2[i].y ) m_vMax.y = m_pWaveWaterVert2[i].y;
					if( m_vMin.z > m_pWaveWaterVert2[i].z ) m_vMin.z = m_pWaveWaterVert2[i].z;
					if( m_vMax.z < m_pWaveWaterVert2[i].z ) m_vMax.z = m_pWaveWaterVert2[i].z;
				}
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->RecalcAllBoundingBoxForSaving();
	m_pLRight->RecalcAllBoundingBoxForSaving();
	m_pULeft->RecalcAllBoundingBoxForSaving();
	m_pURight->RecalcAllBoundingBoxForSaving();


	// Calculate a bounding box of current level node
	D3DXVECTOR3	vMin, vMax;
	D3DXVECTOR3	aVert[7];
	aVert[0] = m_pLLeft->m_vMin;
	aVert[1] = m_pLLeft->m_vMax;
	aVert[2] = m_pLRight->m_vMin;
	aVert[3] = m_pLRight->m_vMax;
	aVert[4] = m_pULeft->m_vMin;
	aVert[5] = m_pULeft->m_vMax;
	aVert[6] = m_pURight->m_vMin;
	vMin = m_pURight->m_vMax;
	vMax = vMin;

	for( int i = 0; i < 7; i++ )
	{
		if( aVert[i].x < vMin.x ) vMin.x = aVert[i].x;
		if( aVert[i].x > vMax.x ) vMax.x = aVert[i].x;
		if( aVert[i].y < vMin.y ) vMin.y = aVert[i].y;
		if( aVert[i].y > vMax.y ) vMax.y = aVert[i].y;
		if( aVert[i].z < vMin.z ) vMin.z = aVert[i].z;
		if( aVert[i].z > vMax.z ) vMax.z = aVert[i].z;
	}

	// Set a bounding box of current level node
	m_vMin = vMin;
	m_vMax = vMax;
}


//-----------------------------------------------------------------------------
// Name : ScaleObjPosition()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::ScaleObjPosition( float fXZScale, float fYScale )
{
	if( m_pLLeft == NULL )			// Lowest level node
	{
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			pDecoObj->m_wx *= fXZScale;
			pDecoObj->m_wy *= fYScale;
			pDecoObj->m_wz *= fXZScale;
			pDecoObj->CalBoundBox();
		}

		return ;
	}

	// Recursive call
	m_pLLeft->ScaleObjPosition( fXZScale, fYScale );
	m_pLRight->ScaleObjPosition( fXZScale, fYScale );
	m_pULeft->ScaleObjPosition( fXZScale, fYScale );
	m_pURight->ScaleObjPosition( fXZScale, fYScale );
}


//-----------------------------------------------------------------------------
// Name : ScaleObjPosition()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::ScaleAllObjPosition( float fXZScale, float fYScale )
{
	// Scale all object position
	g_pObjTree->ScaleObjPosition( fXZScale, fYScale );

	// Recalc all bounding boxes
	g_pObjTree->RecalcAllBoundingBox();
}


//-----------------------------------------------------------------------------
// Name : OffsetObjPosition()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::OffsetObjPosition( float fScale )
{
	if( m_pLLeft == NULL )			// Lowest level node
	{
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			pDecoObj->m_wy += fScale;
			pDecoObj->CalBoundBox();
		}

		int		i, nVert;
		float	fHeight = ms_pHeightOfWaterGroup[m_nWaterGroupID] + 0.3f;

		if( m_nWaveWaterSegment1 > 0 )
		{
			nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
			for( i = 0; i < nVert; i++ )
				m_pWaveWaterVert1[i].y = fHeight;
		}

		if( m_nWaveWaterSegment2 > 0 )
		{
			nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
			for( i = 0; i < nVert; i++ )
				m_pWaveWaterVert2[i].y = fHeight;
		}

		return ;
	}

	// Recursive call
	m_pLLeft->OffsetObjPosition( fScale );
	m_pLRight->OffsetObjPosition( fScale );
	m_pULeft->OffsetObjPosition( fScale );
	m_pURight->OffsetObjPosition( fScale );
}


//-----------------------------------------------------------------------------
// Name : OffsetAllObjPosition()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::OffsetAllObjPosition( float fScale )
{
	// Offset all water heights
	for( int i = 0; i < 20; i++ )
	{
		if( ms_pHeightOfWaterGroup[i] >= 0.0f )
			ms_pHeightOfWaterGroup[i] += fScale;
	}

	// Offset all object position
	g_pObjTree->OffsetObjPosition( fScale );

	// Recalc all bounding boxes
	g_pObjTree->RecalcAllBoundingBox();	
}


//-----------------------------------------------------------------------------
// Name : SaveNot()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::SaveNot( const TCHAR* szFilename )
{
	FILE*	fp;
	fp = fopen( szFilename, "wb" );

	if( fp == NULL )
		return false;

	// Magic( NOT + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic = ( ( 'N' << 24 ) | ( 'O' << 16 ) | ( 'T' << 8 ) ) + 11;
	fwrite( &dwMagic, sizeof(DWORD), 1, fp );

	// Get a count of water groups
	int nCount = 0;
	for( nCount = 0; nCount < 20; nCount++ )
	{
		if( ms_pHeightOfWaterGroup[nCount] < 0.0f )
			break;
	}

	// Name & height of water groups
	int	nLength;
	fwrite( &nCount, sizeof(int), 1, fp );
	for( int i = 0; i < nCount; i++ )
	{
		nLength = _tcslen( ms_aNamesOfWaterGroup[i] );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( ms_aNamesOfWaterGroup[i], sizeof(TCHAR), nLength, fp );
		fwrite( &ms_pHeightOfWaterGroup[i], sizeof(float), 1, fp );
	}

	// Water textures
	if( ms_nBaseWaterTexID != -1 )
	{
		nLength = _tcslen( ms_pIndexedTexture->GetTextureName( ms_nBaseWaterTexID ) );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( ms_pIndexedTexture->GetTextureName( ms_nBaseWaterTexID ), sizeof(TCHAR), nLength, fp );
	}
	else
	{
		nLength = 0;
		fwrite( &nLength, sizeof(int), 1, fp );
	}
	if( ms_nWaveWaterTexID1 != -1 )
	{
		nLength = _tcslen( ms_pIndexedTexture->GetTextureName( ms_nWaveWaterTexID1 ) );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( ms_pIndexedTexture->GetTextureName( ms_nWaveWaterTexID1 ), sizeof(TCHAR), nLength, fp );
	}
	else
	{
		nLength = 0;
		fwrite( &nLength, sizeof(int), 1, fp );
	}
	if( ms_nWaveWaterTexID2 != -1 )
	{
		nLength = _tcslen( ms_pIndexedTexture->GetTextureName( ms_nWaveWaterTexID2 ) );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( ms_pIndexedTexture->GetTextureName( ms_nWaveWaterTexID2 ), sizeof(TCHAR), nLength, fp );
	}
	else
	{
		nLength = 0;
		fwrite( &nLength, sizeof(int), 1, fp );
	}

	// Alpha value of waters
	fwrite( &ms_dwBaseWaterAlpha, sizeof(DWORD), 1, fp );

	// Save object tree
	g_pObjTree->SaveNodeNot( fp );
	
	fclose( fp );

	return true;
}


//-----------------------------------------------------------------------------
// Name : SaveNodeNot()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::SaveNodeNot( FILE* fp )
{
	if( m_pLLeft == NULL )			// Lowest level node
	{
		// Save DecoObj list
		// Count
		DWORD	dwCount = m_lstDecoObj.size();
		fwrite( &dwCount, sizeof(DWORD), 1, fp );

		//Bounding box
		fwrite( &(m_vMin.x), sizeof(float), 1, fp );
		fwrite( &(m_vMin.y), sizeof(float), 1, fp );
		fwrite( &(m_vMin.z), sizeof(float), 1, fp );
		fwrite( &(m_vMax.x), sizeof(float), 1, fp );
		fwrite( &(m_vMax.y), sizeof(float), 1, fp );
		fwrite( &(m_vMax.z), sizeof(float), 1, fp );

		// DecoObj
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			fwrite( &(pDecoObj->m_nLoloId), sizeof(int), 1, fp );
			fwrite( &(pDecoObj->m_wx), sizeof(float), 1, fp );
			fwrite( &(pDecoObj->m_wz), sizeof(float), 1, fp );
			fwrite( &(pDecoObj->m_wy), sizeof(float), 1, fp );
			fwrite( &(pDecoObj->m_dir), sizeof(float), 1, fp );
		}

		// Save water info
		fwrite( &m_nWaterGroupID, sizeof(unsigned char), 1, fp );
		fwrite( &m_nBaseWaterFlags, sizeof(unsigned char), 1, fp );
		fwrite( &m_nWaveWaterSegment1, sizeof(unsigned char), 1, fp );
		fwrite( &m_nWaveWaterSegment2, sizeof(unsigned char), 1, fp );
		if( m_nWaveWaterSegment1 > 0 )
			fwrite( m_pWaveWaterVert1, sizeof(NKVERTEXLT), ( m_nWaveWaterSegment1 + 1 ) * 3, fp );
		if( m_nWaveWaterSegment2 > 0 )
			fwrite( m_pWaveWaterVert2, sizeof(NKVERTEXLT), ( m_nWaveWaterSegment2 + 1 ) * 3, fp );

		return ;
	}

	// Recursive call
	m_pLLeft->SaveNodeNot( fp );
	m_pLRight->SaveNodeNot( fp );
	m_pULeft->SaveNodeNot( fp );
	m_pURight->SaveNodeNot( fp );
}


//-----------------------------------------------------------------------------
// Name : SaveObj()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::SaveObj( const TCHAR* szFilename )
{
	// Erase invisible waters
	g_pObjTree->EraseInvisibleWaters();

	// Recalculate bounding boxes for saving
	g_pObjTree->RecalcAllBoundingBoxForSaving();

	// Prepare for saving
	ms_dwBaseWaterVBSize = 0;
	ms_dwWaveWaterVBSize = 0;
	SAFE_DELETE_ARRAY( ms_pWaterHeights );
	ms_pWaterHeights = new float[ms_nWaterHeightWidth * ms_nWaterHeightWidth];
	g_pObjTree->PrepareForSaving();

	// Save obj
	FILE*	fp;
	fp = fopen( szFilename, "wb" );

	if( fp == NULL )
		return false;

	// Magic( OBJ + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic = ( ( 'O' << 24 ) | ( 'B' << 16 ) | ( 'J' << 8 ) ) + 10;
	fwrite( &dwMagic, sizeof(DWORD), 1, fp );

	// Save object tree
	g_pObjTree->SaveNodeObj( fp );
	
	fclose( fp );

	// Recalculate bounding boxes for the maptool
	g_pObjTree->RecalcAllBoundingBox();

	return true;
}


//-----------------------------------------------------------------------------
// Name : SaveNodeObj()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::SaveNodeObj( FILE* fp )
{
	if( m_pLLeft == NULL )			// Lowest level node
	{
		// Bounding box
		fwrite( &(m_vMin.x), sizeof(float), 1, fp );
		fwrite( &(m_vMin.y), sizeof(float), 1, fp );
		fwrite( &(m_vMin.z), sizeof(float), 1, fp );
		fwrite( &(m_vMax.x), sizeof(float), 1, fp );
		fwrite( &(m_vMax.y), sizeof(float), 1, fp );
		fwrite( &(m_vMax.z), sizeof(float), 1, fp );

		// Save DecoObj list
		// Count
		DWORD	dwCount = m_lstDecoObj.size();
		fwrite( &dwCount, sizeof(DWORD), 1, fp );

		// DecoObj
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			fwrite( &(pDecoObj->m_nLoloId), sizeof(int), 1, fp );
			fwrite( &(pDecoObj->m_wx), sizeof(float), 1, fp );
			fwrite( &(pDecoObj->m_wz), sizeof(float), 1, fp );
			fwrite( &(pDecoObj->m_wy), sizeof(float), 1, fp );
			fwrite( &(pDecoObj->m_dir), sizeof(float), 1, fp );
		}

		// Save water info
		fwrite( &m_nBaseWaterSegment, sizeof(unsigned char), 1, fp );
		fwrite( &m_nWaveWaterSegment1, sizeof(unsigned char), 1, fp );
		fwrite( &m_nWaveWaterSegment2, sizeof(unsigned char), 1, fp );
		fwrite( &m_wBaseVBIndex, sizeof(WORD), 1, fp );
		fwrite( &m_wWaveVBIndex, sizeof(WORD), 1, fp );

		return ;
	}

	// Recursive call
	m_pLLeft->SaveNodeObj( fp );
	m_pLRight->SaveNodeObj( fp );
	m_pULeft->SaveNodeObj( fp );
	m_pURight->SaveNodeObj( fp );
}

bool CNKObjQuadNode::SaveDecoList( const TCHAR* szFilename ) // 현재 등록된 내용을 토대로 DecoObjList.lst 파일을 생성해주는 함수.( .obj 파일을 단순 텍스트 형식으로 뽑은 파일, 구맵툴에서 쓰였던거 같다. ) 06-12-12 원석
{
	g_pObjTree->AddRootList( g_pObjTree ); // 루트의 오브젝트 리스트에 하위 리스트들을 전부 받아다 주는 함수.

	FILE*	fp;
	fp = fopen( szFilename, "wt" );

	if( fp == NULL )
		return false;

	DWORD	dwCount = g_pObjTree->m_lstDecoObj.size();
	fprintf( fp, "%d\n", dwCount );

	// DecoObj
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	for( iterDeco = g_pObjTree->m_lstDecoObj.begin(); iterDeco != g_pObjTree->m_lstDecoObj.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		fprintf( fp, "%d %0.7f %0.7f %0.7f %0.7f\n", pDecoObj->m_nLoloId, pDecoObj->m_wx, pDecoObj->m_wz, pDecoObj->m_wy, pDecoObj->m_dir ); // z랑 y 순서에 유의.
	}
	
	fclose( fp );

	return TRUE;
}

void CNKObjQuadNode::AddRootList( CNKObjQuadNode* RootNode ) // 재귀호출로 루트 노드의 리스트에 오브젝트 플레이스 정보를 모아주는 함수.
{
	if( m_pLLeft == NULL )			// Lowest level node
	{
		// DecoObj
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;

			RootNode->m_lstDecoObj.push_back( pDecoObj ); // 루트 노드의 리스트에 넣는다.
		}

		return ;
	}

	// Recursive call
	m_pLLeft->AddRootList( RootNode );
	m_pLRight->AddRootList( RootNode );
	m_pULeft->AddRootList( RootNode );
	m_pURight->AddRootList( RootNode );
}


//-----------------------------------------------------------------------------
// Name : SaveWat()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::SaveWat( const TCHAR* szFilename )
{
	// Create vertex buffer of waters
	SAFE_DELETE_ARRAY( ms_pBaseWaterTempVB );
	SAFE_DELETE_ARRAY( ms_pWaveWaterTempVB );
	ms_pBaseWaterTempVB = new NKVERTEXLT[ms_dwBaseWaterVBSize];
	ms_pWaveWaterTempVB = new NKVERTEXLT[ms_dwWaveWaterVBSize];
	g_pObjTree->CreateWaterVertexBufferForSaving();

	// Save wat
	FILE*	fp;
	fp = fopen( szFilename, "wb" );

	if( fp == NULL )
		return false;

	// Magic( WAT + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic = ( ( 'W' << 24 ) | ( 'A' << 16 ) | ( 'T' << 8 ) ) + 10;
	fwrite( &dwMagic, sizeof(DWORD), 1, fp );

	// Data of vertex buffer of base water
	fwrite( &ms_dwBaseWaterVBSize, sizeof(DWORD), 1, fp );
	fwrite( ms_pBaseWaterTempVB, sizeof(NKVERTEXLT), ms_dwBaseWaterVBSize, fp );

	// Data of vertex buffer of wave water
	fwrite( &ms_dwWaveWaterVBSize, sizeof(DWORD), 1, fp );
	fwrite( ms_pWaveWaterTempVB, sizeof(NKVERTEXLT), ms_dwWaveWaterVBSize, fp );

	// Water heights of each nodes
	fwrite( ms_pWaterHeights, sizeof(float), ms_nWaterHeightWidth * ms_nWaterHeightWidth, fp );

	// Water textures
	int	nLength;
	if( ms_nBaseWaterTexID != -1 )
	{
		nLength = _tcslen( ms_pIndexedTexture->GetTextureName( ms_nBaseWaterTexID ) );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( ms_pIndexedTexture->GetTextureName( ms_nBaseWaterTexID ), sizeof(TCHAR), nLength, fp );
	}
	else
	{
		nLength = 0;
		fwrite( &nLength, sizeof(int), 1, fp );
	}
	if( ms_nWaveWaterTexID1 != -1 )
	{
		nLength = _tcslen( ms_pIndexedTexture->GetTextureName( ms_nWaveWaterTexID1 ) );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( ms_pIndexedTexture->GetTextureName( ms_nWaveWaterTexID1 ), sizeof(TCHAR), nLength, fp );
	}
	else
	{
		nLength = 0;
		fwrite( &nLength, sizeof(int), 1, fp );
	}
	if( ms_nWaveWaterTexID2 != -1 )
	{
		nLength = _tcslen( ms_pIndexedTexture->GetTextureName( ms_nWaveWaterTexID2 ) );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( ms_pIndexedTexture->GetTextureName( ms_nWaveWaterTexID2 ), sizeof(TCHAR), nLength, fp );
	}
	else
	{
		nLength = 0;
		fwrite( &nLength, sizeof(int), 1, fp );
	}

	fclose( fp );

	SAFE_DELETE_ARRAY( ms_pBaseWaterTempVB );
	SAFE_DELETE_ARRAY( ms_pWaveWaterTempVB );

	DWORD	dwSize = g_pMyNKApp->GetCamera()->GetFrustumFar() / ms_nBaseWaterSize;
	ms_dwBaseWaterVBSize = dwSize * dwSize * 4;
	ms_dwWaveWaterVBSize = dwSize * dwSize * 4;

	ms_pBaseWaterTempVB = new NKVERTEXLT[ms_dwBaseWaterVBSize];
	ms_pWaveWaterTempVB = new NKVERTEXLT[ms_dwWaveWaterVBSize];

	for( int i = 0; i < ms_dwBaseWaterVBSize; i += 4 )
	{
		ms_pBaseWaterTempVB[i].u = 0.0f;		ms_pBaseWaterTempVB[i].v = 1.0f;
		ms_pBaseWaterTempVB[i+1].u = 1.0f;		ms_pBaseWaterTempVB[i+1].v = 1.0f;
		ms_pBaseWaterTempVB[i+2].u = 0.0f;		ms_pBaseWaterTempVB[i+2].v = 0.0f;
		ms_pBaseWaterTempVB[i+3].u = 1.0f;		ms_pBaseWaterTempVB[i+3].v = 0.0f;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : PaintBaseWater()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::PaintBaseWater( int nIndex, int nGroupID )
{
	unsigned char	nMask = 1 << nIndex;
	if( !( m_nBaseWaterFlags & nMask ) )
	{
		int		nDX = nIndex % 2;
		int		nDZ = nIndex / 2;
		int		nX = ( m_nX << 2 ) + ( nDX << 1 );
		int		nZ = ( m_nZ << 2 ) + ( nDZ << 1 );
		int		x, z;
		bool	bDraw = false;
		
		for( z = nZ; z <= nZ + 2; z++ )
		{
			for( x = nX; x <= nX + 2; x++ )
			{
				if( g_pMyNKApp->GetTerrain()->GetHeights( z, x ) < ms_pHeightOfWaterGroup[nGroupID] )
				{
					bDraw = true;
					break;
				}
			}
		}

		if( bDraw )
		{
			m_nWaterGroupID = nGroupID;
			m_nBaseWaterFlags |= nMask;

			x = ( m_nX << 1 ) + nDX;
			z = ( m_nZ << 1 ) + nDZ;
			ms_pWaterHeights[x + z * ms_nWaterHeightWidth] = ms_pHeightOfWaterGroup[nGroupID];

			// Modify a bounding box of a current node
			if( m_vMin.y > ms_pHeightOfWaterGroup[nGroupID] ) m_vMin.y = ms_pHeightOfWaterGroup[nGroupID];
			if( m_vMax.y < ms_pHeightOfWaterGroup[nGroupID] + 0.3f ) m_vMax.y = ms_pHeightOfWaterGroup[nGroupID] + 0.3f;

			// Recalculate a bounding box of a upper node
			RecalcUpperBoundingBox();
		}
	}
}


//-----------------------------------------------------------------------------
// Name : EraseBaseWater()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::EraseBaseWater( int nIndex, int nGroupID )
{
	if( !( m_nBaseWaterFlags & 0x0f ) || m_nWaterGroupID != nGroupID )
		return ;

	unsigned char	nMask = 1 << nIndex;
	m_nBaseWaterFlags &= ~nMask;

	if( !( m_nBaseWaterFlags & 0x0f ) )
	{
		m_nWaterGroupID = 0;
		m_nBaseWaterFlags = 0;
		m_nWaveWaterSegment1 = 0;
		m_nWaveWaterSegment2 = 0;
		SAFE_DELETE_ARRAY( m_pWaveWaterVert1 );
		SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );

		// Recalculate a bounding box of a current node and a upper node 
		RecalcBoundingBox();
	}
}


//-----------------------------------------------------------------------------
// Name : PaintWaveWater()
// Desc : return 0 - succeed
//               1 - the base water is not drawn
//               2 - two wave waters is alreay drawn
//               3 - Group ID is different
//-----------------------------------------------------------------------------
int CNKObjQuadNode::PaintWaveWater( int nGroupID, NKVERTEXLT aWaveWaterVB[], int nSegment,
									const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax )
{
	if( !( m_nBaseWaterFlags & 0x0f ) )
		return 1;

	if( nGroupID != m_nWaterGroupID )
		return 3;

	if( m_nWaveWaterSegment1 == 0 )
	{
		m_nWaveWaterSegment1 = nSegment;

		int	nVert = ( nSegment + 1 ) * 3;
		m_pWaveWaterVert1 = new NKVERTEXLT[nVert];
		
		for( int i = 0; i < nVert; i++ )
			m_pWaveWaterVert1[i] = aWaveWaterVB[i];

		// Modify a bounding box of a current node
		if( m_vMin.x > rMin.x ) m_vMin.x = rMin.x;
		if( m_vMin.y > rMin.y ) m_vMin.y = rMin.y;
		if( m_vMin.z > rMin.z ) m_vMin.z = rMin.z;
		if( m_vMax.x < rMax.x ) m_vMax.x = rMax.x;
		if( m_vMax.y < rMax.y ) m_vMax.y = rMax.y;
		if( m_vMax.z < rMax.z ) m_vMax.z = rMax.z;

		// Recalculate a bounding box of a upper node
		RecalcUpperBoundingBox();

		return 0;
	}

	if( m_nWaveWaterSegment2 == 0 )
	{
		m_nWaveWaterSegment2 = nSegment;

		int	nVert = ( nSegment + 1 ) * 3;
		m_pWaveWaterVert2 = new NKVERTEXLT[nVert];
		
		for( int i = 0; i < nVert; i++ )
			m_pWaveWaterVert2[i] = aWaveWaterVB[i];

		// Modify a bounding box of a current node
		if( m_vMin.x > rMin.x ) m_vMin.x = rMin.x;
		if( m_vMin.y > rMin.y ) m_vMin.y = rMin.y;
		if( m_vMin.z > rMin.z ) m_vMin.z = rMin.z;
		if( m_vMax.x < rMax.x ) m_vMax.x = rMax.x;
		if( m_vMax.y < rMax.y ) m_vMax.y = rMax.y;
		if( m_vMax.z < rMax.z ) m_vMax.z = rMax.z;

		// Recalculate a bounding box of a upper node
		RecalcUpperBoundingBox();

		return 0;
	}

	return 2;
}


//-----------------------------------------------------------------------------
// Name : EraseWaveWater()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::EraseWaveWater( int nGroupID, const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir )
{
	if( m_nWaterGroupID != nGroupID )
		return false;

	D3DXVECTOR3	vP0, vP1, vP2;
	float		fT;
	int			i;
	bool		bErase = false;

	if( m_nWaveWaterSegment1 > 0 )
	{
		for( i = 0; i < m_nWaveWaterSegment1; i++ )
		{
			vP0 = m_pWaveWaterVert1[i*3].p;
			vP1 = m_pWaveWaterVert1[i*3 + 2].p;
			vP2 = m_pWaveWaterVert1[i*3 + 3].p;
			if( NKIntersectRayTri( rOrigin, rDir, vP0, vP1, vP2, &fT ) )
			{
				m_nWaveWaterSegment1 = 0;
				SAFE_DELETE_ARRAY( m_pWaveWaterVert1 );
				bErase = true;
				break;
			}

			vP0 = m_pWaveWaterVert1[i*3 + 5].p;
			if( NKIntersectRayTri( rOrigin, rDir, vP2, vP1, vP0, &fT ) )
			{
				m_nWaveWaterSegment1 = 0;
				SAFE_DELETE_ARRAY( m_pWaveWaterVert1 );
				bErase = true;
				break;
			}
		}
	}

	if( !bErase )
	{
		if( m_nWaveWaterSegment2 > 0 )
		{
			for( i = 0; i < m_nWaveWaterSegment2; i++ )
			{
				vP0 = m_pWaveWaterVert2[i*3].p;
				vP1 = m_pWaveWaterVert2[i*3 + 2].p;
				vP2 = m_pWaveWaterVert2[i*3 + 3].p;
				if( NKIntersectRayTri( rOrigin, rDir, vP0, vP1, vP2, &fT ) )
				{
					m_nWaveWaterSegment2 = 0;
					SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );
					bErase = true;
					break;
				}

				vP0 = m_pWaveWaterVert2[i*3 + 5].p;
				if( NKIntersectRayTri( rOrigin, rDir, vP2, vP1, vP0, &fT ) )
				{
					m_nWaveWaterSegment2 = 0;
					SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );
					bErase = true;
					break;
				}
			}
		}
	}

	// Recalculate a bounding box of a current node and a upper node 
	if( bErase )
	{		
		RecalcBoundingBox();
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name : RemoveWaterGroup()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RemoveWaterGroup( int nGroupID )
{
	g_pObjTree->RemoveWaterWithSameGroup( nGroupID );

	for( int i = nGroupID; i < 19; i++ )
	{
		ms_pHeightOfWaterGroup[i] = ms_pHeightOfWaterGroup[i + 1];
		strcpy( ms_aNamesOfWaterGroup[i], ms_aNamesOfWaterGroup[i + 1] );
	}

	ms_pHeightOfWaterGroup[19] = -1.0f;;
	ms_aNamesOfWaterGroup[19][0] = NULL;
}


//-----------------------------------------------------------------------------
// Name : RemoveWaterWithSameGroup()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RemoveWaterWithSameGroup( int nGroupID )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		if( m_nWaterGroupID > nGroupID )
		{
			m_nWaterGroupID--;
			return ;
		}
		else if( m_nWaterGroupID == nGroupID )
		{
			m_nWaterGroupID = 0;
			m_nBaseWaterFlags = 0;
			m_nWaveWaterSegment1 = 0;
			m_nWaveWaterSegment2 = 0;
			SAFE_DELETE_ARRAY( m_pWaveWaterVert1 );
			SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );

			// Recalculate a bounding box of a current node and a upper node 
			RecalcBoundingBox();
		}

		return ;
	}

	// Recursive call
	m_pLLeft->RemoveWaterWithSameGroup( nGroupID );
	m_pLRight->RemoveWaterWithSameGroup( nGroupID );
	m_pULeft->RemoveWaterWithSameGroup( nGroupID );
	m_pURight->RemoveWaterWithSameGroup( nGroupID );
}


//-----------------------------------------------------------------------------
// Name : PaintAllBaseWater()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::PaintAllBaseWater( int nGroupID )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		if( m_nBaseWaterFlags != 0 )
			return ;

		unsigned char	nMask = 1;
		int				nX = m_nX << 2;
		int				nZ = m_nZ << 2;
		int				x, z, i, j;
		bool			bDraw;

		for( i = 0; i < 2; i++ )
		{
			for( j = 0; j < 2; j++ )
			{
				bDraw = false;
				for( z = nZ; z <= nZ + 2; z++ )
				{
					for( x = nX; x <= nX + 2; x++ )
					{
						if( g_pMyNKApp->GetTerrain()->GetHeights( z, x ) < ms_pHeightOfWaterGroup[nGroupID] )
							bDraw = true;
					}
				}

				if( bDraw )
				{
					x = m_nX << 1;		z = m_nZ << 1;
					ms_pWaterHeights[x + z * ms_nWaterHeightWidth] = ms_pHeightOfWaterGroup[nGroupID];
					ms_pWaterHeights[x + 1 + z * ms_nWaterHeightWidth] = ms_pHeightOfWaterGroup[nGroupID];
					ms_pWaterHeights[x + ( z + 1 ) * ms_nWaterHeightWidth] = ms_pHeightOfWaterGroup[nGroupID];
					ms_pWaterHeights[x + 1 + ( z + 1 ) * ms_nWaterHeightWidth] = ms_pHeightOfWaterGroup[nGroupID];

					m_nBaseWaterFlags |= nMask;
				}

				nMask <<= 1;
				nX += 2;
			}
			nZ += 2;
			nX = m_nX << 2;
		}

		// Recalculate a bounding box of a current node and a upper node 
		RecalcBoundingBox();
		
		return ;
	}

	// Recursive call
	m_pLLeft->PaintAllBaseWater( nGroupID );
	m_pLRight->PaintAllBaseWater( nGroupID );
	m_pULeft->PaintAllBaseWater( nGroupID );
	m_pURight->PaintAllBaseWater( nGroupID );
}


//-----------------------------------------------------------------------------
// Name : PaintAllWaveWater()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::PaintAllWaveWater( int nGroupID )
{
	SAFE_DELETE_ARRAY( ms_pCoastLineData );

	CNKObjQuadNode*	pNode = NULL;
	int				nX, nZ, x, z, nSX, nSZ, nEX, nEZ;
	float			fX, fZ;
	int				nNodeWidth = ms_nCellSize >> 2;
	int				nBufferWidth = ms_nTreeWidth * nNodeWidth;
	bool			bCoastLineExist = false;
	unsigned char*	pTempCoastLineData = NULL;

	ms_pCoastLineData = new unsigned char[nBufferWidth * nBufferWidth];
	memset( ms_pCoastLineData, 0, sizeof(unsigned char) * nBufferWidth * nBufferWidth );

	// Analyze the coastline
	for( nZ = 0; nZ < ms_nTreeWidth; nZ++ )
	{
		for( nX = 0; nX < ms_nTreeWidth; nX++ )
		{
			pNode = ms_aQuadNodes[nZ][nX];
			if( pNode->m_nWaterGroupID != nGroupID || pNode->m_nBaseWaterFlags == 0 )
				continue;

			bCoastLineExist = false;
			nSX = nX << 2;				nSZ = nZ << 2;
			nEX = ( nX + 1 ) << 2;		nEZ = ( nZ + 1 ) << 2;
			for( z = nSZ; z <= nEZ; z++ )
			{
				for( x = nSX; x <= nEX; x++ )
				{
					if( g_pMyNKApp->GetTerrain()->GetHeights( z, x ) >= ms_pHeightOfWaterGroup[nGroupID] )
					{
						bCoastLineExist = true;
						z = nEZ + 1;	x = nEX + 1;
					}
				}
			}

			if( bCoastLineExist )
			{
				nSX = nX * nNodeWidth;				nSZ = nZ * nNodeWidth;
				nEX = ( nX + 1 ) * nNodeWidth;		nEZ = ( nZ + 1 ) * nNodeWidth;
				for( z = nSZ; z < nEZ; z++ )
				{
					pTempCoastLineData = &ms_pCoastLineData[nSX + z * nBufferWidth];
					for( x = nSX; x < nEX; x++ )
					{
						fZ = ( z << 2 ) + 2;	fX = ( x << 2 ) + 2;
						if( g_pMyNKApp->GetTerrain()->GetHeights( fZ, fX ) >= ms_pHeightOfWaterGroup[nGroupID] )
							*pTempCoastLineData = 255;
						pTempCoastLineData++;
					}
				}
			}			
		}
	}

	// Test - start
	//FILE*	fp = fopen( "coastline.raw", "wb" );
	//fwrite( ms_pCoastLineData, sizeof(unsigned char), nBufferWidth * nBufferWidth, fp );
	//fclose( fp );
	// Test - end

	// Create outlines of the coastline


	// Create vertices of the wave water

	SAFE_DELETE_ARRAY( ms_pCoastLineData );
}


//-----------------------------------------------------------------------------
// Name : AutoPaintWaveWater()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::AutoPaintWaveWater( int nGroupID )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		if( m_nBaseWaterFlags == 0 || m_nWaterGroupID != nGroupID )
			return ;

		// Check if this node has the coastline
		int		nX, nZ, nSX, nSZ, nEX, nEZ;
		bool	bFind = false;
		nSX = m_nX << 2;			nSZ = m_nZ << 2;
		nEX = ( m_nX + 1 ) << 2;	nEZ = ( m_nZ + 1 ) << 2;
		for( nZ = nSZ; nZ <= nEZ; nZ++ )
		{
			for( nX = nSX; nX <= nEX; nX++ )
			{
				if( g_pMyNKApp->GetTerrain()->GetHeights( nZ, nX ) > ms_pHeightOfWaterGroup[nGroupID] )
				{
					bFind = true;
					break;
				}
			}
		}

		if( !bFind )
			return ;

		// Analyze the coastline of this node
		int				nWidth = ms_nCellSize >> 2;
		unsigned char*	pCoastLine = new unsigned char[nWidth*nWidth];
		unsigned char*	pTemp = pCoastLine;
		nSX = m_nX * ms_nCellSize;				nSZ = m_nZ * ms_nCellSize;
		nEX = ( m_nX + 1 ) * ms_nCellSize;		nEZ = ( m_nZ + 1 ) * ms_nCellSize;
		for( nZ = nSZ; nZ < nEZ; nZ += 4 )
		{
			for( nX = nSX; nX < nEX; nX += 4 )
			{
				if( g_pMyNKApp->GetTerrain()->GetHeights( (float)nZ, (float)nX ) <= ms_pHeightOfWaterGroup[nGroupID] )
					*pTemp = 255;
				else
					*pTemp = 0;
				pTemp++;
			}
		}

		// Create vertices of the wave water


		SAFE_DELETE_ARRAY( pCoastLine );
		
		return ;
	}

	// Recursive call
	m_pLLeft->AutoPaintWaveWater( nGroupID );
	m_pLRight->AutoPaintWaveWater( nGroupID );
	m_pULeft->AutoPaintWaveWater( nGroupID );
	m_pURight->AutoPaintWaveWater( nGroupID );
}


//-----------------------------------------------------------------------------
// Name : GetPickedWaveWater()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::GetPickedWaveWater( int nGroupID, NKVERTEXLT aWaveWaterVB[], int& rSegment,
											const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir )
{
	if( m_nWaterGroupID != nGroupID )
		return false;

	D3DXVECTOR3	vP0, vP1, vP2;
	float		fT;
	int			i, j, nVert;
	bool		bPicked = false;

	if( m_nWaveWaterSegment1 > 0 )
	{
		for( i = 0; i < m_nWaveWaterSegment1; i++ )
		{
			vP0 = m_pWaveWaterVert1[i*3].p;
			vP1 = m_pWaveWaterVert1[i*3 + 2].p;
			vP2 = m_pWaveWaterVert1[i*3 + 3].p;
			if( NKIntersectRayTri( rOrigin, rDir, vP0, vP1, vP2, &fT ) )
			{
				nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
				rSegment = m_nWaveWaterSegment1;

				for( j = 0; j < nVert; j++ )
					aWaveWaterVB[j] = m_pWaveWaterVert1[j];

				m_nWaveWaterSegment1 = 0;
				SAFE_DELETE_ARRAY( m_pWaveWaterVert1 );
				bPicked = true;
				break;
			}

			vP0 = m_pWaveWaterVert1[i*3 + 5].p;
			if( NKIntersectRayTri( rOrigin, rDir, vP2, vP1, vP0, &fT ) )
			{
				nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
				rSegment = m_nWaveWaterSegment1;

				for( j = 0; j < nVert; j++ )
					aWaveWaterVB[j] = m_pWaveWaterVert1[j];

				m_nWaveWaterSegment1 = 0;
				SAFE_DELETE_ARRAY( m_pWaveWaterVert1 );
				bPicked = true;
				break;
			}
		}
	}

	if( !bPicked )
	{
		if( m_nWaveWaterSegment2 > 0 )
		{
			for( i = 0; i < m_nWaveWaterSegment2; i++ )
			{
				vP0 = m_pWaveWaterVert2[i*3].p;
				vP1 = m_pWaveWaterVert2[i*3 + 2].p;
				vP2 = m_pWaveWaterVert2[i*3 + 3].p;
				if( NKIntersectRayTri( rOrigin, rDir, vP0, vP1, vP2, &fT ) )
				{
					nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
					rSegment = m_nWaveWaterSegment2;

					for( j = 0; j < nVert; j++ )
						aWaveWaterVB[j] = m_pWaveWaterVert2[j];

					m_nWaveWaterSegment2 = 0;
					SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );
					bPicked = true;
					break;
				}

				vP0 = m_pWaveWaterVert2[i*3 + 5].p;
				if( NKIntersectRayTri( rOrigin, rDir, vP2, vP1, vP0, &fT ) )
				{
					nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
					rSegment = m_nWaveWaterSegment2;

					for( j = 0; j < nVert; j++ )
						aWaveWaterVB[j] = m_pWaveWaterVert2[j];

					m_nWaveWaterSegment2 = 0;
					SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );
					bPicked = true;
					break;
				}
			}
		}
	}

	// Recalculate a bounding box of a current node and a upper node 
	if( bPicked )
	{		
		RecalcBoundingBox();
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name : GetWaveWaterVertices()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::GetWaveWaterVertices( int nGroupID, int nWhichWave,
											D3DXVECTOR3 aWaveWaterVert[], int& rSegment )
{
	if( m_nWaterGroupID != nGroupID )
		return false;

	int	i, nVert;
	if( nWhichWave == 1 )
	{
		if( m_nWaveWaterSegment1 > 0 )
		{
			rSegment = m_nWaveWaterSegment1;
			nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
			for( i = 0; i < nVert; i++ )
				aWaveWaterVert[i] = m_pWaveWaterVert1[i].p;
			
			return true;
		}
	}
	else
	{
		if( m_nWaveWaterSegment2 > 0 )
		{
			rSegment = m_nWaveWaterSegment2;
			nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
			for( i = 0; i < nVert; i++ )
				aWaveWaterVert[i] = m_pWaveWaterVert2[i].p;
			
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : ModifiedWaterHeight()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::ModifiedWaterHeight( int nGroupID )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		if( m_nWaterGroupID != nGroupID )
			return ;

		int		i, nVert;
		float	fHeight = ms_pHeightOfWaterGroup[nGroupID] + 0.3f;

		if( m_nWaveWaterSegment1 > 0 )
		{
			nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
			for( i = 0; i < nVert; i++ )
				m_pWaveWaterVert1[i].y = fHeight;
		}

		if( m_nWaveWaterSegment2 > 0 )
		{
			nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
			for( i = 0; i < nVert; i++ )
				m_pWaveWaterVert2[i].y = fHeight;
		}

		// Recalculate a bounding box of a current node and a upper node
		RecalcBoundingBox();

		return ;
	}

	// Recursive call
	m_pLLeft->ModifiedWaterHeight( nGroupID );
	m_pLRight->ModifiedWaterHeight( nGroupID );
	m_pULeft->ModifiedWaterHeight( nGroupID );
	m_pURight->ModifiedWaterHeight( nGroupID );
}


//-----------------------------------------------------------------------------
// Name : EraseInvisibleWaters()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::EraseInvisibleWaters()
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		if( m_nBaseWaterFlags == 0 )
			return ;

		unsigned char	nMask = 1;
		int				nX = m_nX << 2;
		int				nZ = m_nZ << 2;
		int				x, z, i, j, x2, z2;
		int				nEraseCount = 0;
		bool			bDraw;
		float			fWaterHeight;

		x2 = m_nX << 1;		z2 = m_nZ << 1;
		for( i = 0; i < 2; i++ )
		{
			for( j = 0; j < 2; j++ )
			{
				fWaterHeight = ms_pWaterHeights[x2 + j + ( z2 + i ) * ms_nWaterHeightWidth];
				bDraw = false;
				for( z = nZ; z <= nZ + 2; z++ )
				{
					for( x = nX; x <= nX + 2; x++ )
					{
						if( g_pMyNKApp->GetTerrain()->GetHeights( z, x ) < fWaterHeight)
							bDraw = true;
					}
				}

				if( !bDraw )
				{
					m_nBaseWaterFlags &= ~nMask;
					nEraseCount++;					
				}

				nMask <<= 1;
				nX += 2;
			}
			nZ += 2;
			nX = m_nX << 2;
		}

		if( nEraseCount == 4 )
		{
			m_nWaterGroupID = 0;
			m_nBaseWaterFlags = 0;
			m_nWaveWaterSegment1 = 0;
			m_nWaveWaterSegment2 = 0;
			SAFE_DELETE_ARRAY( m_pWaveWaterVert1 );
			SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );
		}

		// Recalculate a bounding box of a current node and a upper node 
		if( nEraseCount > 0 )
			RecalcBoundingBox();
		
		return ;
	}

	// Recursive call
	m_pLLeft->EraseInvisibleWaters();
	m_pLRight->EraseInvisibleWaters();
	m_pULeft->EraseInvisibleWaters();
	m_pURight->EraseInvisibleWaters();
}


//-----------------------------------------------------------------------------
// Name : CalcWaterMapAttr()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::CalcWaterMapAttr( unsigned char* pAttrMap, int nAttrMapWidth )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		if( m_nBaseWaterFlags == 0 )
			return ;

		unsigned char	nMask = 1;
		float			fWaterHeight;
		float			fX, fZ;
		int				nX, nZ, nSX, nSZ, nEX, nEZ, x, z, sx, sz, ex, ez;
		nSX = m_nX << 1;			nSZ = m_nZ << 1;
		nEX = ( m_nX + 1 ) << 1;	nEZ = ( m_nZ + 1 ) << 1;
		for( nZ = nSZ; nZ < nEZ; nZ++ )
		{
			for( nX = nSX; nX < nEX; nX++ )
			{
				if( m_nBaseWaterFlags & nMask )
				{
					fWaterHeight = ms_pWaterHeights[nX + nZ * ms_nWaterHeightWidth] - 10.0f;
					sx = nX * 10;			sz = nZ * 10;
					ex = ( nX + 1 ) * 10;	ez = ( nZ + 1 ) * 10;
					for( z = sz; z < ez; z++ )
					{
						for( x = sx; x < ex; x++ )
						{
							fX = x * 10 + 5;	fZ = z * 10 + 5;
							if( g_pMyNKApp->GetTerrain()->GetHeights( fZ, fX ) < fWaterHeight )
								pAttrMap[x + z * nAttrMapWidth] = g_nWATERAREA;
						}
					}
				}
				nMask <<= 1;
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->CalcWaterMapAttr( pAttrMap, nAttrMapWidth );
	m_pLRight->CalcWaterMapAttr( pAttrMap, nAttrMapWidth );
	m_pULeft->CalcWaterMapAttr( pAttrMap, nAttrMapWidth );
	m_pURight->CalcWaterMapAttr( pAttrMap, nAttrMapWidth );
}


//-----------------------------------------------------------------------------
// Name : CalcObjectMapAttr()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::CalcObjectMapAttr( int nAttrMapWidth, bool bInsideHouse )
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		int							i, nType;
		TCHAR						szWarningMsg[256];
		bool						bInsideThing;
		bool						bValideObject;
		D3DVECTOR					vLB, vLT, vRT, vRB;
		POINT						ptLB, ptLT, ptRT, ptRB;
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			nType = g_pDecoObjSet->m_vecObjType[pDecoObj->m_nLoloId];

			if( nType == BLD_TABLE || nType == BLD_CHAIR || nType == BLD_ETC )
				bInsideThing = true;
			else
				bInsideThing = false;

			if( bInsideHouse )
			{
				if( bInsideThing == false )
					continue;
			}
			else
			{
				if( bInsideThing == true )
					continue;
			}

			bValideObject = true;
			for( i = 0; bValideObject && i < pDecoObj->m_pLolo->m_FloorCount; i++ )
			{
				vLB = pDecoObj->m_FloorVect[i * 4];
				vLT = pDecoObj->m_FloorVect[i * 4 + 1];
				vRT = pDecoObj->m_FloorVect[i * 4 + 2];
				vRB = pDecoObj->m_FloorVect[i * 4 + 3];

				ptLB.x = (long)vLB.x / 10;
				ptLB.y = (long)vLB.z / 10;
				ptLT.x = (long)vLT.x / 10;
				ptLT.y = (long)vLT.z / 10;
				ptRT.x = (long)vRT.x / 10;
				ptRT.y = (long)vRT.z / 10;
				ptRB.x = (long)vRB.x / 10;
				ptRB.y = (long)vRB.z / 10;

				if( ptLB.x < 0 || ptLT.x < 0 || ptRT.x < 0 || ptRB.x < 0 ||
					ptLB.y < 0 || ptLT.y < 0 || ptRT.y < 0 || ptRB.y < 0 ||
					ptLB.x >= nAttrMapWidth || ptLT.x >= nAttrMapWidth ||
					ptRT.x >= nAttrMapWidth || ptRB.x >= nAttrMapWidth ||
					ptLB.y >= nAttrMapWidth || ptLT.y >= nAttrMapWidth ||
					ptRT.y >= nAttrMapWidth || ptRB.y >= nAttrMapWidth )
				{
					_stprintf( szWarningMsg, _T("오브젝트의 플로어가 속성 맵의 범위를 벗어났습니다.\n")
											_T("%d : %s ( %5.2f, %5.2f )"), pDecoObj->m_nLoloId,
											g_pDecoObjSet->m_vecNickName[pDecoObj->m_nLoloId],
											pDecoObj->m_wx, pDecoObj->m_wz );
					MessageBox( NULL, szWarningMsg, _T("NKMapTool"), MB_ICONWARNING | MB_OK );
					bValideObject = false;
					continue;
				}

				if( nType == BLD_HOUSE )
				{
					g_pMyNKApp->GetTerrain()->FillMapAttr( ptLB, ptLT, ptRT, ptRB, g_nINDOORAREA );

					g_pMyNKApp->GetTerrain()->DrawMapAttrLine( ptLB.x, ptLB.y, ptLT.x, ptLT.y, g_nUNABLETOWALK );
					g_pMyNKApp->GetTerrain()->DrawMapAttrLine( ptLT.x, ptLT.y, ptRT.x, ptRT.y, g_nUNABLETOWALK );
					g_pMyNKApp->GetTerrain()->DrawMapAttrLine( ptRT.x, ptRT.y, ptRB.x, ptRB.y, g_nUNABLETOWALK );
					g_pMyNKApp->GetTerrain()->DrawMapAttrLine( ptRB.x, ptRB.y, ptLB.x, ptLB.y, g_nUNABLETOWALK );
				}
				else
					g_pMyNKApp->GetTerrain()->FillMapAttr( ptLB, ptLT, ptRT, ptRB, g_nUNABLETOWALK );
			}

			if( bValideObject )
			{
				for( i = 0; i < pDecoObj->m_pLolo->m_InDoorCount; i++ )
				{
					vLB = pDecoObj->m_InDoorVect[i * 4];
					vLT = pDecoObj->m_InDoorVect[i * 4 + 1];
					vRT = pDecoObj->m_InDoorVect[i * 4 + 2];
					vRB = pDecoObj->m_InDoorVect[i * 4 + 3];

					ptLB.x = (long)vLB.x / 10;
					ptLB.y = (long)vLB.z / 10;
					ptLT.x = (long)vLT.x / 10;
					ptLT.y = (long)vLT.z / 10;
					ptRT.x = (long)vRT.x / 10;
					ptRT.y = (long)vRT.z / 10;
					ptRB.x = (long)vRB.x / 10;
					ptRB.y = (long)vRB.z / 10;

					g_pMyNKApp->GetTerrain()->FillMapAttr( ptLB, ptLT, ptRT, ptRB, g_nINDOORBOX );
				}

				for( i = 0; i < pDecoObj->m_pLolo->m_OutDoorCount; i++ )
				{
					vLB = pDecoObj->m_OutDoorVect[i * 4];
					vLT = pDecoObj->m_OutDoorVect[i * 4 + 1];
					vRT = pDecoObj->m_OutDoorVect[i * 4 + 2];
					vRB = pDecoObj->m_OutDoorVect[i * 4 + 3];

					ptLB.x = (long)vLB.x / 10;
					ptLB.y = (long)vLB.z / 10;
					ptLT.x = (long)vLT.x / 10;
					ptLT.y = (long)vLT.z / 10;
					ptRT.x = (long)vRT.x / 10;
					ptRT.y = (long)vRT.z / 10;
					ptRB.x = (long)vRB.x / 10;
					ptRB.y = (long)vRB.z / 10;

					g_pMyNKApp->GetTerrain()->FillMapAttr( ptLB, ptLT, ptRT, ptRB, g_nOUTDOORBOX );
				}
			}
		}		

		return ;
	}

	// Recursive call
	m_pLLeft->CalcObjectMapAttr( nAttrMapWidth, bInsideHouse );
	m_pLRight->CalcObjectMapAttr( nAttrMapWidth, bInsideHouse );
	m_pULeft->CalcObjectMapAttr( nAttrMapWidth, bInsideHouse );
	m_pURight->CalcObjectMapAttr( nAttrMapWidth, bInsideHouse );
}


//-----------------------------------------------------------------------------
// Name : PrepareForSaving()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::PrepareForSaving()
{
	m_nBaseWaterSegment = 0;
	m_wBaseVBIndex = 0;
	m_wWaveVBIndex = 0;

	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		if( m_nBaseWaterFlags > 0 )
		{
			int				nX, nZ, x, z;
			unsigned char	nMask = 1;
			float			fWaterHeight = ms_pHeightOfWaterGroup[m_nWaterGroupID];

			// Base water
			nX = m_nX << 1;		nZ = m_nZ << 1;
			for( z = 0; z < 2; z++ )
			{
				for( x = 0; x < 2; x++ )
				{
					if( m_nBaseWaterFlags & nMask )
					{
						ms_pWaterHeights[nX + x + ( nZ + z ) * ms_nWaterHeightWidth] = fWaterHeight;
						m_nBaseWaterSegment++;
					}
					else
						ms_pWaterHeights[nX + x + ( nZ + z ) * ms_nWaterHeightWidth] = -1.0f;

					nMask <<= 1;
				}
			}
			m_wBaseVBIndex = ms_dwBaseWaterVBSize;
			ms_dwBaseWaterVBSize += m_nBaseWaterSegment * 4;

			// Wave water
			if( m_nWaveWaterSegment1 == 0 && m_nWaveWaterSegment2 > 0 )
			{
				int	nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
				m_pWaveWaterVert1 = new NKVERTEXLT[nVert];
				for( int i = 0; i < nVert; i++ )
					m_pWaveWaterVert1[i] = m_pWaveWaterVert2[i];

				m_nWaveWaterSegment1 = m_nWaveWaterSegment2;
				m_nWaveWaterSegment2 = 0;
				SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );
			}

			if( m_nWaveWaterSegment1 > 0 )
			{
				m_wWaveVBIndex = ms_dwWaveWaterVBSize;
				ms_dwWaveWaterVBSize += ( m_nWaveWaterSegment1 + 1 ) * 3;
				
				if( m_nWaveWaterSegment2 > 0 )
					ms_dwWaveWaterVBSize += ( m_nWaveWaterSegment2 + 1 ) * 3; 
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->PrepareForSaving();
	m_pLRight->PrepareForSaving();
	m_pULeft->PrepareForSaving();
	m_pURight->PrepareForSaving();
}


//-----------------------------------------------------------------------------
// Name : CreateWaterVertexBufferForSaving()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::CreateWaterVertexBufferForSaving()
{
	if( m_pLLeft == NULL )		// If this node is lowest level node
	{
		if( m_nBaseWaterSegment > 0 )
		{
			int				i, nVert, x, z;
			unsigned char	nMask = 1;
			float			fWaterHeight = ms_pHeightOfWaterGroup[m_nWaterGroupID];
			float			fX = m_nX * ms_nCellSize;
			float			fZ = m_nZ * ms_nCellSize;

			// Base water
			ms_dwBaseVertexCount = m_wBaseVBIndex;
			for( z = 0; z < 2; z++ )
			{
				for( x = 0; x < 2; x++ )
				{
					if( m_nBaseWaterFlags & nMask )
					{
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + x*ms_nBaseWaterSize;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + z*ms_nBaseWaterSize;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].u = 0.0f;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].v = 1.0f;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + (x+1)*ms_nBaseWaterSize;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + z*ms_nBaseWaterSize;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].u = 1.0f;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].v = 1.0f;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + x*ms_nBaseWaterSize;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + (z+1)*ms_nBaseWaterSize;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].u = 0.0f;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].v = 0.0f;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].x = fX + (x+1)*ms_nBaseWaterSize;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].y = fWaterHeight;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].z = fZ + (z+1)*ms_nBaseWaterSize;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].u = 1.0f;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount].v = 0.0f;
						ms_pBaseWaterTempVB[ms_dwBaseVertexCount++].c = ms_dwBaseWaterAlpha;
					}
					nMask <<= 1;
				}
			}

			// Wave water
			ms_dwWaveVertexCount = m_wWaveVBIndex;
			if( m_nWaveWaterSegment1 > 0 )
			{
				nVert = ( m_nWaveWaterSegment1 + 1 ) * 3;
				for( i = 0; i < nVert; i++ )
					ms_pWaveWaterTempVB[ms_dwWaveVertexCount++] = m_pWaveWaterVert1[i];
				
				if( m_nWaveWaterSegment2 > 0 )
				{
					nVert = ( m_nWaveWaterSegment2 + 1 ) * 3;
					for( i = 0; i < nVert; i++ )
						ms_pWaveWaterTempVB[ms_dwWaveVertexCount++] = m_pWaveWaterVert2[i];
				}
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->CreateWaterVertexBufferForSaving();
	m_pLRight->CreateWaterVertexBufferForSaving();
	m_pULeft->CreateWaterVertexBufferForSaving();
	m_pURight->CreateWaterVertexBufferForSaving();
}


//-----------------------------------------------------------------------------
// Name : RestoreDeviceObjects()
// Desc : 
//-----------------------------------------------------------------------------
HRESULT CNKObjQuadNode::RestoreDeviceObjects( LPDIRECT3D7 pD3D, LPDIRECT3DDEVICE7 pd3dDevice )
{
	ms_pD3D = pD3D;
	ms_pd3dDevice = pd3dDevice;

	// Create water vertex buffer
	if( g_pMyNKApp->IsLoadedGameMap() )
	{
		if( !InitWaterVertexBuffer() )
			return E_FAIL;
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : InvalidateDeviceObjects()
// Desc : 
//-----------------------------------------------------------------------------
HRESULT CNKObjQuadNode::InvalidateDeviceObjects()
{
	SAFE_RELEASE( ms_pBaseWaterVB );
	SAFE_RELEASE( ms_pWaveWaterVB );

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : InitWaterVertexBuffer()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::InitWaterVertexBuffer()
{
	FILE*	fp = NULL;
	fp = fopen( ms_szFilename, "rb" );
	if( fp == NULL )
		return false;

	DWORD	dwMagic;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( ( 'W' << 24 ) | ( 'A' << 16 ) | ( 'T' << 8 ) ) + 10 )
		return false;

	// Data of vertex buffer of base water
	fread( &ms_dwBaseWaterVBSize, sizeof(DWORD), 1, fp );
	NKVERTEXLT*	pTempBaseVB = new NKVERTEXLT[ms_dwBaseWaterVBSize];
	fread( pTempBaseVB, sizeof(NKVERTEXLT), ms_dwBaseWaterVBSize, fp );

	// Data of vertex buffer of wave water
	fread( &ms_dwWaveWaterVBSize, sizeof(DWORD), 1, fp );
	NKVERTEXLT*	pTempWaveVB = new NKVERTEXLT[ms_dwWaveWaterVBSize];
	fread( pTempWaveVB, sizeof(NKVERTEXLT), ms_dwWaveWaterVBSize, fp );

	fclose( fp );

	// Create vertex buffer
	D3DVERTEXBUFFERDESC	VBDesc;
	VBDesc.dwSize = sizeof( VBDesc );
	VBDesc.dwCaps = D3DVBCAPS_WRITEONLY;
	VBDesc.dwFVF = NKVERTEXLT::FVF;
	VBDesc.dwNumVertices = ms_dwBaseWaterVBSize;

	NKVERTEXLT*	pVert;

	if( ms_dwBaseWaterVBSize )
	{
		if( FAILED( ms_pD3D->CreateVertexBuffer( &VBDesc, &ms_pBaseWaterVB, 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pTempBaseVB );
			SAFE_DELETE_ARRAY( pTempWaveVB );
			return false;
		}
		
		if( FAILED( ms_pBaseWaterVB->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
			(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pTempBaseVB );
			SAFE_DELETE_ARRAY( pTempWaveVB );
			return false;
		}
		memcpy( pVert, pTempBaseVB, ms_dwBaseWaterVBSize * sizeof(NKVERTEXLT) );
		ms_pBaseWaterVB->Unlock();
	}
	

	VBDesc.dwNumVertices = ms_dwWaveWaterVBSize;

	if( ms_dwWaveWaterVBSize )
	{
		if( FAILED( ms_pD3D->CreateVertexBuffer( &VBDesc, &ms_pWaveWaterVB, 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pTempBaseVB );
			SAFE_DELETE_ARRAY( pTempWaveVB );
			return false;
		}
		if( FAILED( ms_pWaveWaterVB->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
			(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pTempBaseVB );
			SAFE_DELETE_ARRAY( pTempWaveVB );
			return false;
		}
		memcpy( pVert, pTempWaveVB, ms_dwWaveWaterVBSize * sizeof(NKVERTEXLT) );
		ms_pWaveWaterVB->Unlock();
	}
	

	SAFE_DELETE_ARRAY( pTempBaseVB );
	SAFE_DELETE_ARRAY( pTempWaveVB );

	return true;
}


//-----------------------------------------------------------------------------
// Name : MergeNot()
// Desc : 
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::MergeNot( const TCHAR* szFilename, int nSZ, int nSX, int nEZ, int nEX )
{
	FILE*	fp;
	fp = fopen( szFilename, "rb" );
	
	if( fp == NULL )
		return false;
	
	// Get a quadtree depth
	int		i;
	DWORD	dwTreeDepth = 0;
	DWORD	dwWidth = ms_nTreeWidth;
	int		nRight = dwWidth;
	while( dwWidth >>= 1 )
		dwTreeDepth++;

	// Magic( NOT + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( ( 'N' << 24 ) | ( 'O' << 16 ) | ( 'T' << 8 ) ) + 11 )
		return false;

	// Name & height of water groups
	int	nCount, nLength;;
	fread( &nCount, sizeof(int), 1, fp );
	for( i = 0; i < nCount; i++ )
	{
		fread( &nLength, sizeof(int), 1, fp );
		if( ms_pHeightOfWaterGroup[i] < 0.0f )
		{
			fread( ms_aNamesOfWaterGroup[i], sizeof(TCHAR), nLength, fp );
			fread( &ms_pHeightOfWaterGroup[i], sizeof(float), 1, fp );
		}
		else
		{
			fseek( fp, sizeof(TCHAR) * nLength, SEEK_CUR );
			fseek( fp, sizeof(float), SEEK_CUR );
		}
	}

	// Water textures
	fread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		fseek( fp, sizeof(TCHAR) * nLength, SEEK_CUR );
	fread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		fseek( fp, sizeof(TCHAR) * nLength, SEEK_CUR );
	fread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		fseek( fp, sizeof(TCHAR) * nLength, SEEK_CUR );

	// Alpha value of waters
	fseek( fp, sizeof(DWORD), SEEK_CUR );

	// Object tree
	nSX >>= 2;		nSZ >>= 2;
	nEX >>= 2;		nEZ >>= 2;
	g_pObjTree->MergeNodeNot( fp, nSZ, nSX, nEZ, nEX );
	
	fclose( fp );

	g_pObjTree->RecalcAllBoundingBox();

	return true;
}


//-----------------------------------------------------------------------------
// Name : MergeNodeNot()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::MergeNodeNot( FILE* fp, int nSZ, int nSX, int nEZ, int nEX )
{
	if( m_pLLeft == NULL )			// If this node is lowest level node
	{
		if( m_nX >= nSX && m_nX <= nEX && m_nZ >= nSZ && m_nZ <= nEZ )
		{
			// Read DecoObj list
			// Count
			DWORD	dwCount;
			fread( &dwCount, sizeof(DWORD), 1, fp );

			//Bounding box
			fread( &(m_vMin.x), sizeof(float), 1, fp );
			fread( &(m_vMin.y), sizeof(float), 1, fp );
			fread( &(m_vMin.z), sizeof(float), 1, fp );
			fread( &(m_vMax.x), sizeof(float), 1, fp );
			fread( &(m_vMax.y), sizeof(float), 1, fp );
			fread( &(m_vMax.z), sizeof(float), 1, fp );

			// DecoObj
			CDecoObj*	pDecoObj;
			for( DWORD i = 0; i < dwCount; i++ )
			{
				pDecoObj = new CDecoObj();
				fread( &(pDecoObj->m_nLoloId), sizeof(int), 1, fp );
				fread( &(pDecoObj->m_wx), sizeof(float), 1, fp );
				fread( &(pDecoObj->m_wz), sizeof(float), 1, fp );
				fread( &(pDecoObj->m_wy), sizeof(float), 1, fp );
				fread( &(pDecoObj->m_dir), sizeof(float), 1, fp );

				pDecoObj->m_pLolo = g_pDecoObjSet->m_vecLoloSet[pDecoObj->m_nLoloId];
				m_lstDecoObj.push_back( pDecoObj );
				pDecoObj->CalBoundBox();
				g_pDecoObjSet->m_vecRefCount[pDecoObj->m_nLoloId]++;
			}

			// Read water info
			fread( &m_nWaterGroupID, sizeof(unsigned char), 1, fp );
			fread( &m_nBaseWaterFlags, sizeof(unsigned char), 1, fp );
			fread( &m_nWaveWaterSegment1, sizeof(unsigned char), 1, fp );
			fread( &m_nWaveWaterSegment2, sizeof(unsigned char), 1, fp );
			
			SAFE_DELETE_ARRAY( m_pWaveWaterVert1 );
			SAFE_DELETE_ARRAY( m_pWaveWaterVert2 );
			if( m_nWaveWaterSegment1 > 0 )
			{				
				m_pWaveWaterVert1 = new NKVERTEXLT[27];
				fread( m_pWaveWaterVert1, sizeof(NKVERTEXLT), ( m_nWaveWaterSegment1 + 1 ) * 3, fp );
			}
			if( m_nWaveWaterSegment2 > 0 )
			{				
				m_pWaveWaterVert2 = new NKVERTEXLT[27];
				fread( m_pWaveWaterVert2, sizeof(NKVERTEXLT), ( m_nWaveWaterSegment2 + 1 ) * 3, fp );
			}

			int				x, z, sx, sz, ex, ez;
			unsigned char	nMask = 1;
			float			fWaterHeight = ms_pHeightOfWaterGroup[m_nWaterGroupID];
			sx = m_nX << 1;				sz = m_nZ << 1;
			ex = ( m_nX + 1 ) << 1;		ez = ( m_nZ + 1 ) << 1;
			for( z = sz; z < ez; z++ )
			{
				for( x = sx; x < ex; x++ )
				{
					ms_pWaterHeights[x + z * ms_nWaterHeightWidth] = -1.0f;
					if( m_nBaseWaterFlags & nMask )
						ms_pWaterHeights[x + z * ms_nWaterHeightWidth] = fWaterHeight;

					nMask <<= 1;
				}
			}
		}
		else
		{
			// Read DecoObj list
			// Count
			DWORD	dwCount;
			fread( &dwCount, sizeof(DWORD), 1, fp );

			//Bounding box
			fseek( fp, sizeof(float) * 6, SEEK_CUR );

			// DecoObj
			for( DWORD i = 0; i < dwCount; i++ )
			{
				fseek( fp, sizeof(int), SEEK_CUR );
				fseek( fp, sizeof(float), SEEK_CUR );
				fseek( fp, sizeof(float), SEEK_CUR );
				fseek( fp, sizeof(float), SEEK_CUR );
				fseek( fp, sizeof(float), SEEK_CUR );
			}

			// Read water info
			unsigned char	nWaveWaterSegment1;
			unsigned char	nWaveWaterSegment2;
			fseek( fp, sizeof(unsigned char), SEEK_CUR );
			fseek( fp, sizeof(unsigned char), SEEK_CUR );
			fread( &nWaveWaterSegment1, sizeof(unsigned char), 1, fp );
			fread( &nWaveWaterSegment2, sizeof(unsigned char), 1, fp );

			if( nWaveWaterSegment1 > 0 )
			{				
				fseek( fp, sizeof(NKVERTEXLT) * ( ( nWaveWaterSegment1 + 1 ) * 3 ), SEEK_CUR );
			}
			if( nWaveWaterSegment2 > 0 )
			{				
				fseek( fp, sizeof(NKVERTEXLT) * ( ( nWaveWaterSegment2 + 1 ) * 3 ), SEEK_CUR );
			}
		}

		return ;
	}

	// Recursive call
	m_pLLeft->MergeNodeNot( fp, nSZ, nSX, nEZ, nEX );
	m_pLRight->MergeNodeNot( fp, nSZ, nSX, nEZ, nEX );
	m_pULeft->MergeNodeNot( fp, nSZ, nSX, nEZ, nEX );
	m_pURight->MergeNodeNot( fp, nSZ, nSX, nEZ, nEX );
}


//-----------------------------------------------------------------------------
// Name : GetHeightOnWater()
// Desc :
//-----------------------------------------------------------------------------
float CNKObjQuadNode::GetHeightOnWater( float fZ, float fX )
{
	int	nX = fX / ms_nBaseWaterSize;
	int	nZ = fZ / ms_nBaseWaterSize;

	if( nX < 0 || nZ < 0 || nX >= ms_nWaterHeightWidth || nZ >= ms_nWaterHeightWidth )
		return -1.0f;

	return ms_pWaterHeights[nX + nZ * ms_nWaterHeightWidth];
}


//-----------------------------------------------------------------------------
// Name : GetHeightOnObject()
// Desc : 
//-----------------------------------------------------------------------------
float CNKObjQuadNode::GetHeightOnObject( float fZ, float fX, float fY )
{
	int	nX = fX / ms_nCellSize;
	int	nZ = fZ / ms_nCellSize;

	if( nX < 2 || nZ < 2 || nX >= ms_nTreeWidth - 2 || nZ >= ms_nTreeWidth - 2 )
		return -1.0f;

	int							x, z, i, nVert;
	float						fDist, fHeight;
	D3DXVECTOR3					vP0, vP1, vP2;
	D3DXVECTOR3					vOrig = D3DXVECTOR3( fX, fY, fZ );
	D3DXVECTOR3					vDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	CDecoObj*					pDecoObj;
	list<CDecoObj *>::iterator	iterDeco;
	int	sx = nX - 2;
	int	sz = nZ - 2;
	int	ex = nX + 2;
	int	ez = nZ + 2;
	for( z = sz; z <= ez; z++ )
	{
		for( x = sx; x <= ex; x++ )
		{
			for( iterDeco = g_lstWalkObj[z][x].begin(); iterDeco != g_lstWalkObj[z][x].end(); iterDeco++ )
			{
				pDecoObj = *iterDeco;
				nVert = pDecoObj->m_pLolo->m_WalkPlaneCount * 4;
				for( i = 0; i < nVert; i += 4 )
				{
					// vP0, vP1, vP2
					vP0 = pDecoObj->m_WalkPlaneVect[i];
					vP1 = pDecoObj->m_WalkPlaneVect[i + 1];
					vP2 = pDecoObj->m_WalkPlaneVect[i + 2];
					if( NKIntersectRayTri( vOrig, vDir, vP0, vP1, vP2, &fDist ) )
					{
						fHeight = vDir.y * fDist + fY;
						//if( fHeight > fY + 10.0f )
						//	return -1.0f;
						//else
						return fHeight;
					}

					// vP2, vP1, vP0
					vP0 = pDecoObj->m_WalkPlaneVect[i + 3];
					if( NKIntersectRayTri( vOrig, vDir, vP2, vP1, vP0, &fDist ) )
					{
						fHeight = vDir.y * fDist + fY;
						//if( fHeight > fY + 10.0f )
						//	return -1.0f;
						//else
						return fHeight;
					}
				}
			}
		}
	}

	return -1.0f;
}


//-----------------------------------------------------------------------------
// Name : RemoveAllDecoObjsInRegion()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RemoveAllDecoObjsInRegion( int nSX, int nSZ, int nEX, int nEZ )
{
	int	nX, nZ;
	int	nWidth = ( ( g_pMyNKApp->GetTerrain()->GetHMWidth() - 1 ) >> 2 ) - 1;
	int	ex = nEX >> 2;
	int	ez = nWidth - ( nSZ >> 2 );
	int	sx = nSX >> 2;
	int	sz = nWidth - ( nEZ >> 2 );

	// Remove all decoobjs
	for( nZ = sz; nZ <= ez; nZ++ )
	{
		for( nX = sx; nX <= ex; nX++ )
		{
			CNKObjQuadNode*	pTempNode = ms_aQuadNodes[nZ][nX];
			pTempNode->RemoveAllDecoObjs();
		}
	}

	// Recalculate bounding boxes
	g_pObjTree->RecalcAllBoundingBox();
}


//-----------------------------------------------------------------------------
// Name : RemoveAllDecoObjs()
// Desc : 
//-----------------------------------------------------------------------------
void CNKObjQuadNode::RemoveAllDecoObjs()
{
	// Remove decoobjs from a list of decoobjs
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); )
	{
		pDecoObj = *iterDeco;
		iterDeco = m_lstDecoObj.erase( iterDeco );
		g_pDecoObjSet->m_vecRefCount[pDecoObj->m_nLoloId]--;
		delete pDecoObj;
	}
}


//-----------------------------------------------------------------------------
// Name : ()
// Desc : 
//-----------------------------------------------------------------------------

int Min(int n1, int n2, int n3, int n4)
{
	int ret = n1;
	if (ret > n2)	ret = n2;
	if (ret > n3)	ret = n3;
	if (ret > n4)	ret = n4;
	return ret;
}

int Max(int n1, int n2, int n3, int n4)
{
	int ret = n1;
	if (ret < n2)	ret = n2;
	if (ret < n3)	ret = n3;
	if (ret < n4)	ret = n4;
	return ret;
}

//ulyssesme-20061002 드래그 처리 함수
//-----------------------------------------------------------------------------
// Name : GetPickedDecoObjByDrag()
// Desc : 드래그 영역에서 오브젝트 뽑아내기
//-----------------------------------------------------------------------------
void CNKObjQuadNode::GetPickedDecoObjByDrag(D3DXVECTOR3 nStartX, D3DXVECTOR3 nEndX, D3DXVECTOR3 nStartY, D3DXVECTOR3 nEndY, list<CDecoObj*> & ppReturn, int & nRet)
{
	D3DXVECTOR3 vMouseBox[4];
	vMouseBox[0] = nStartX;
	vMouseBox[1] = nEndX;
	vMouseBox[2] = nStartY;
	vMouseBox[3] = nEndY;
	float nMinX = Min(nStartX.x, nEndX.x, nStartY.x, nEndY.x);
	float nMinZ = Min(nStartX.z, nEndX.z, nStartY.z, nEndY.z);
	float nMaxX = Max(nStartX.x, nEndX.x, nStartY.x, nEndY.x);
	float nMaxZ = Max(nStartX.z, nEndX.z, nStartY.z, nEndY.z);
	if (nMinX > m_vMax.x || nMaxX < m_vMin.x || nMinZ > m_vMax.z || nMaxZ < m_vMin.z)	return;

	static int a_nPos[5] = {0,1,2,3,0};

	// Test a intersection between a ray and OBB if this node is lowest level node
	if( m_pLLeft == NULL )
	{
		CDecoObj*					pDecoObj;
		list<CDecoObj*>::iterator	iterDeco;
	
		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			float vMinX = pDecoObj->m_vBoundBox[0].x;
			float vMinZ = pDecoObj->m_vBoundBox[0].z;
			float vMaxX = pDecoObj->m_vBoundBox[0].x;
			float vMaxZ = pDecoObj->m_vBoundBox[0].z;
			for (int l = 1; l < 4; l++)
			{
				if (pDecoObj->m_vBoundBox[l].x < vMinX)
					vMinX = pDecoObj->m_vBoundBox[l].x;
				if (pDecoObj->m_vBoundBox[l].x > vMaxX)
					vMaxX = pDecoObj->m_vBoundBox[l].x;
				if (pDecoObj->m_vBoundBox[l].z < vMinZ)
					vMinZ = pDecoObj->m_vBoundBox[l].z;
				if (pDecoObj->m_vBoundBox[l].z > vMaxZ)
					vMaxZ = pDecoObj->m_vBoundBox[l].z;
			}

			if (nMinX > vMaxX || nMaxX < vMinX || nMinZ > vMaxZ || nMaxZ < vMinZ)	continue;	//1차 대충 검사
			// 2차 정밀 검사
			bool bCross = false;
			for (int n = 0; n < 4; n++)
			{
				float x3 = vMouseBox[a_nPos[n]].x;
				float z3 = vMouseBox[a_nPos[n]].z;
				bCross = true;
				for (int m = 0; m < 4; m++)
				{
					float x1 = pDecoObj->m_vBoundBox[a_nPos[m]].x;
					float z1 = pDecoObj->m_vBoundBox[a_nPos[m]].z;
					float x2 = pDecoObj->m_vBoundBox[a_nPos[m + 1]].x;
					float z2 = pDecoObj->m_vBoundBox[a_nPos[m + 1]].z;
					float a1 = x2 - x1;
					float b1 = z2 - z1;
					float a2 = x3 - x1;
					float b2 = z3 - z1;
					if (a1 == 0)	a1 = 0.000000000001f;
					if (a2 == 0)	a2 = 0.000000000001f;
					float q1 = atan(b1 / a1);
					if (a1 < 0)	q1 = q1 + NK_PI;
					float q2 = atan(b2 / a2);
					if (a2 < 0)	q2 = q2 + NK_PI;
					if (q1 < 0) q1 = q1 + 2 * NK_PI;
					if (q2 < 0) q2 = q2 + 2 * NK_PI;
					float q = q2 - q1;
					if (q < 0) q = q + 2 * NK_PI;
					if (q < NK_PI && q > 0)
					{
						bCross = false;
						break;
					}
				}
				if (bCross)	break;
			}
			if (!bCross)
			{
				for (int n = 0; n < 4; n++)
				{
					float x3 = pDecoObj->m_vBoundBox[a_nPos[n]].x;
					float z3 = pDecoObj->m_vBoundBox[a_nPos[n]].z;
					bCross = true;
					for (int m = 0; m < 4; m++)
					{
						float x1 = vMouseBox[a_nPos[m]].x;
						float z1 = vMouseBox[a_nPos[m]].z;
						float x2 = vMouseBox[a_nPos[m + 1]].x;
						float z2 = vMouseBox[a_nPos[m + 1]].z;
						float a1 = x2 - x1;
						float b1 = z2 - z1;
						float a2 = x3 - x1;
						float b2 = z3 - z1;
						if (a1 == 0)	a1 = 0.000000000001f;
						if (a2 == 0)	a2 = 0.000000000001f;
						float q1 = atan(b1 / a1);
						if (a1 < 0)	q1 = q1 + NK_PI;
						float q2 = atan(b2 / a2);
						if (a2 < 0)	q2 = q2 + NK_PI;
						if (q1 < 0) q1 = q1 + 2 * NK_PI;
						if (q2 < 0) q2 = q2 + 2 * NK_PI;
						float q = q2 - q1;
						if (q < 0) q = q + 2 * NK_PI;
						if (q < NK_PI && q > 0)
						{
							bCross = false;
							break;
						}
					}
					if (bCross)	break;
				}
			}
			if (bCross)
			{
				ppReturn.push_back(pDecoObj);
				nRet++;
			}
		}
		return;
	}

	// Recursive call
	m_pLLeft->GetPickedDecoObjByDrag(nStartX, nEndX, nStartY, nEndY, ppReturn, nRet);
	m_pLRight->GetPickedDecoObjByDrag(nStartX, nEndX, nStartY, nEndY, ppReturn, nRet);
	m_pULeft->GetPickedDecoObjByDrag(nStartX, nEndX, nStartY, nEndY, ppReturn, nRet);
	m_pURight->GetPickedDecoObjByDrag(nStartX, nEndX, nStartY, nEndY, ppReturn, nRet);

	return;
}
