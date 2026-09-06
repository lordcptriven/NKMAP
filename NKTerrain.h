//-----------------------------------------------------------------------------
// File : NKTerrain.h
// Desc :
//-----------------------------------------------------------------------------

#ifndef _NKTERRAIN_H_
#define _NKTERRAIN_H_


#include "NKVertexType.h"
#include "IndexedTexture.h"


class CNKTerQuadNode;
class CNKCamera;
class IndexedTexture;


//-----------------------------------------------------------------------------
// Map attributes
//-----------------------------------------------------------------------------
const unsigned char	g_nABLETOWALK = 0;
const unsigned char	g_nUNABLETOWALK = 255;
const unsigned char	g_nINDOORAREA = 50;
const unsigned char	g_nINDOORBOX = 100;
const unsigned char	g_nOUTDOORBOX = 150;
const unsigned char	g_nWATERAREA = 120;


//-----------------------------------------------------------------------------
// Name : struct TerrainIndexBuffer
// Desc : Index buffer for CNKTerrain
//-----------------------------------------------------------------------------
typedef struct _TerrainIndexBuffer
{
	DWORD	dwCount;
	DWORD	dwTotalCount;
	WORD*	pIB;
} TerIB;


//-----------------------------------------------------------------------------
// Name : class CNKTerrain
// Desc : 
//-----------------------------------------------------------------------------
class CNKTerrain
{
protected:
	LPDIRECT3D7					m_pD3D;						// The main D3D object
	LPDIRECT3DDEVICE7			m_pd3dDevice;				// The D3D rendering device
	TCHAR*						m_szTerName;				// Terrain name ( this is a working directory and a file name )
	int							m_nHMWidth;					// Width of height map ( width = height )
	unsigned char*				m_pHeights;					// Height values
	int							m_nHeightScale;				// Vertical extension
	int							m_nPatchSize;				// Horizontal extension
	unsigned char*				m_pAttributeMap;			// Attribute map
	int							m_nAttributeMapWidth;		// Width of attribute map
	CNKTerQuadNode*				m_aRootNode[4];				// Root nodes of quad tree
	int							m_nQuadTreeDepth;			// Depth of quad tree ( in case of 256x256, depth is 8 )
	LPDIRECT3DVERTEXBUFFER7		m_aVB[4];					// Terrain Vertex buffers
	DWORD						m_dwVBSize;					// Vertex count of vertex buffer
	int							m_nVBWidth;					// Width of vertex buffer
	vector<IndexedTexture*>		m_vecTerTex;				// Terrain texture group
	LPDIRECTDRAWSURFACE7		m_aClrMapTexL2[4];			// Color(alpha & light) map texture ( layer 2 )
	LPDIRECTDRAWSURFACE7		m_aClrMapTexL3[4];			// Color(alpha & light) map texture ( layer 3 )
	unsigned char*				m_pAlphaMapL2;				// Alpha map ( layer 2 )
	unsigned char*				m_pAlphaMapL3;				// Alpha map ( layer 3 )
	unsigned char*				m_pLightMapsR[6];			// Light maps ( r channel )
	unsigned char*				m_pLightMapsG[6];			// Light maps ( g channel )
	unsigned char*				m_pLightMapsB[6];			// Light maps ( b channel )
	D3DXVECTOR3					m_aDirectLightDir[6];		// Directions of directional lights
	int							m_nTimeID;					// World time ID ( 0, 1, 2, 3, 4, 5 )
	
	// Variables used in map tool
	CNKTerQuadNode*				m_aQuadNodes[256][256];		// Quad node array to access directly ( Nodes[z][x] )
	DWORD						m_dwTriangles;				// Number of triangles rendered
	int							m_nViewLayers;				// Layer1: 1, layer2: 2, layer3: 4, all: 7
	unsigned char				m_aAlphaMapL2[512][512];	// Alpha map ( layer 2 )
	unsigned char				m_aAlphaMapL3[512][512];	// Alpha map ( layer 3 )
	DWORD						m_aRGBMap[6][512][512];		// RGB maps
	float						m_aAlphaL2[512][512];		// Alpha map ( layer 2 ) for a detailed control
	float						m_aAlphaL3[512][512];		// Alpha map ( layer 3 ) for a detailed control
	float						m_aLightR[6][512][512];		// Light maps ( r channel ) for a detailed control
	float						m_aLightG[6][512][512];		// Light maps ( g channel ) for a detailed control
	float						m_aLightB[6][512][512];		// Light maps ( b channel ) for a detailed control
	float						m_aShadowR[6][512][512];	// Shadow maps ( r channel ) for a detailed control
	float						m_aShadowG[6][512][512];	// Shadow maps ( g channel ) for a detailed control
	float						m_aShadowB[6][512][512];	// Shadow maps ( b channel ) for a detailed control
	float*						m_pTempHeights;				// Height values for a detailed control
	unsigned char				m_nMinHeight;				// Minimum height
	unsigned char				m_nMaxHeight;				// Maximum height
	D3DXVECTOR3*				m_pNormals;					// Normals of terrain vertices for simulating a light
	float						m_aDirectLightR[6];			// R channels of directional lights
	float						m_aDirectLightG[6];			// G channels of directional lights
	float						m_aDirectLightB[6];			// B channels of directional lights
	float						m_aAmbientLightR[6];		// R channels of ambient ligths
	float						m_aAmbientLightG[6];		// G channels of ambient ligths
	float						m_aAmbientLightB[6];		// B channels of ambient ligths
	DWORD						m_aMapAttrColor[6];			// Colors of the attribute map
	unsigned char				m_aTempAttrMap[200][200];	// Temp attribute map

public:
	vector<TerIB*>				m_vecIBL1;					// Terrain index buffer - layer 1
	vector<TerIB*>				m_vecIBL2;					// Terrain index buffer - layer 2
	vector<TerIB*>				m_vecIBL3;					// Terrain index buffer - layer 3

protected:
	bool	InitQuadTree( unsigned char* pL1TexID = NULL, unsigned char* pL2TexID = NULL,
							unsigned char* pL3TexID = NULL );
	bool	InitVertexBuffer();
	HRESULT	CreateClrMapTexture();

	// Functions used in map tool
	void	LoadHeightMap( const TCHAR*	szFilename );
	void	RecalcAllBoundingBox();
	void	RemakeAllVertexBuffers();
	void	RenewVertexBuffers( int nSZ, int nSX, int nEZ, int nEX );
	void	RenewVertexBuffer( int nIndex, int nSZ, int nSX, int nEZ, int nEX );
	void	MakeNormals();
	void	ComputeNormals( int nSZ, int nSX, int nEZ, int nEX );
	void	SimulateDirectionalLight( int nIndex, int nSZ, int nSX, int nEZ, int nEX );
	void	BlurAllLightMaps( int nSZ, int nSX, int nEZ, int nEX );
	void	BlurLightMap( int nIndex, int nSZ, int nSX, int nEZ, int nEX );
	void	MakeAllLightMaps( int nSZ, int nSX, int nEZ, int nEX );
	void	MakeLightMaps( int nIndex, int nSZ, int nSX, int nEZ, int nEX );
	void	ApplyAllShadowMaps( int nSZ, int nSX, int nEZ, int nEX );
	void	ApplyShadowMaps( int nIndex, int nSZ, int nSX, int nEZ, int nEX );
	void	MakeAllRGBMaps();
	void	RenewAllRGBMaps( int nSZ, int nSX, int nEZ, int nEX );
	void	OptimizeTerrainMap();
	bool	ModifyQuadTree( int nSZ, int nSX, int nEZ, int nEX, unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID );

public:
	CNKTerrain();
	virtual ~CNKTerrain();	
	
	bool	LoadMap( const TCHAR* szFilename );
	void	Destroy();
	void	Render();
	bool	GetPickedPoint( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir,
							D3DXVECTOR3& rPoint );	
	int		GetHMWidth() const;
	int		GetPatchSize() const;
	int		GetHeightScale() const;
	float	GetHeights( int nZ, int nX ) const;
	float	GetHeights( float fZ, float fX ) const;	
	HRESULT	UpdateClrMapTexture( int nLayer, int nIndex );
	HRESULT	RestoreDeviceObjects( LPDIRECT3D7 pD3D, LPDIRECT3DDEVICE7 pd3dDevice );
	HRESULT	InvalidateDeviceObjects();
	void	SetWorldTime( int nWorldTime );
	void	GetLightDirection( float& rX,float& rY, float& rZ );
	int		GetMapIndex( D3DXVECTOR3 vCoord );			// For a CShadowRect class in laghaim source

	// Functions used in map tool
	LPDIRECT3DDEVICE7	GetD3DDevice() const;

	void	SetTerrainName( const TCHAR* szTerrainName );
	const TCHAR*	GetTerrainName() const;
	bool	Init( int nWHSegment, int nPatchSize, int nHeightScale, const TCHAR* szHeightMap );
	bool	LoadNmf( const TCHAR* szFilename );
	
	// [10/10/2007 Theodoric] 텍스쳐 갯수를 15에서 50으로 확장하면 읽는 것 때문에 버전으로 나눠서 읽어 들인다.
	bool	LoadNmfVer(const TCHAR* szFilename, int version  );
	
	bool	SaveNmf( const TCHAR* szFilename );
	bool	SaveMap( const TCHAR* szFilename );
	bool	ExportHeightMap( const TCHAR* szFilename );
	bool	ExportServerAttrMap( const TCHAR* szFilename );
	void	RenderGrid( int nGridSize, DWORD dwColor );
	void	SetQuadNodePointer( int nZ, int nX, CNKTerQuadNode* pNode );	
	int		GetQuadTreeDepth() const;
	DWORD	GetTriangleCount() const;
	bool	GetPickedTile( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir, int& rZ, int& rX );

	int		AddTexture( const TCHAR* szFilename );
	bool	ReplaceTexture( int nIndex, const TCHAR* szFilename );
	bool	DeleteTexture( int nTexID );
	TCHAR*	GetTextureName( int nIndex ) const;
	int		GetTextureCount() const;
	void	DrawNonblendTile( int nSZ, int nSX, int nEZ, int nEX, int nIndex );
	void	DrawBlendTilePencil( int nSZ, int nSX, int nEZ, int nEX, int nLayer, int nIndex, int nAlpha, bool bPaint );
	void	DrawBlendTileBrush( int nZ, int nX, int nLayer, int nIndex, float aAlpha[4], bool bPaint );	
	void	DrawShadowColor( int nIndex, int nZ, int nX, float fR, float fG, float fB, float aR[4], float aG[4] , float aB[4], bool bPaint );
	void	SetViewLayers( int nViewLayers );
	void	OptimizeLayers( int nSZ, int nSX, int nEZ, int nEX );	
		
	int		GetMinHeight() const;
	int		GetMaxHeight() const;
	void	ResetMinMaxHeights();
	void	ModifyPatchSize( int nPatchSize );
	void	ModifyHeightScale( int nHeightScale );
	void	ReapplyHeightMap( const TCHAR* szFilename );
	void	ModifyAllHeights( int nOffset );
	void	RaiseTiles( int nSZ, int nSX, int nEZ, int nEX, float fStrength );
	void	LowerTiles( int nSZ, int nSX, int nEZ, int nEX, float fStrength );
	void	FlattenTiles( int nSZ, int nSX, int nEZ, int nEX, float fStrength );
	void	SmoothTiles( int nSZ, int nSX, int nEZ, int nEX, float fStrength );
	void	RaiseLowerVertex( int nZ, int nX, int nOffset, float fStrength );
	void	RaiseVertices( int nSZ, int nSX, int nEZ, int nEX, float fZ, float fX, float fRadius, float fStrength );
	void	LowerVertices( int nSZ, int nSX, int nEZ, int nEX, float fZ, float fX, float fRadius, float fStrength );
	void	FlattenVertices( int nSZ, int nSX, int nEZ, int nEX, float fZ, float fX, float fRadius, float fStrength);
	void	SmoothVertices( int nSZ, int nSX, int nEZ, int nEX, float fZ, float fX, float fRadius, float fStrength);
	void	RecalcBoundingBox( int nSZ, int nSX, int nEZ, int nEX );

	void	RenewRGBMap( int nIndex, int nSZ, int nSX, int nEZ, int nEX );
	void	UpdateRGBMapsByVertex( int nSZ, int nSX, int nEZ, int nEX );
	void	UpdateRGBMapsByShadow( BOOL bCurrentShadowMap, int nSZ, int nSX, int nEZ, int nEX );
	bool	ImportShadowMap( int nIndex, const TCHAR* szFilename );
	bool	ExportShadowMap( int nIndex, const TCHAR* szFilename );

	int		GetTimeID() const;
	void	SetDefaultLightDirections();
	void	SetLightDirection( int nTimeID, float fX, float fY, float fZ );
	void	GetLightDirection( int nTimeID, float& rX,float& rY, float& rZ );
	void	SetDirectLightColor( int nTimeID, float fR, float fG, float fB );
	void	GetDirectLightColor( int nTimeID, int& rR, int& rG, int& rB );
	void	SetAmbientColor( int nTimeID, float fR, float fG, float fB );
	void	GetAmbientColor( int nTimeID, int& rR, int& rG, int& rB );

	bool	GenerateAttributeMap();
	bool	ImportServerAttributeMap( const TCHAR* szFilename );
	void	ExportServerAttributeMap( const TCHAR* szFilename );
	void	ExportServerAttributeMapToBmp( const std::string& szFilename );
	void	SetMapAttrColor( int nIndex, COLORREF crColor );
	DWORD	GetMapAttrColor( int nIndex );
	void	RenderAttributeMap( int nRange, float fZ, float fX );
	void	DrawMapAttribute( int nMapAttr, int nZ, int nX, int BrushSize ); // 브러쉬 크기 조절 기능 추가(by 원석)
	void	FillMapAttr( POINT ptLB, POINT ptLT, POINT ptRT, POINT ptRB, unsigned char nMapAttr );
	void	DrawMapAttrLine( int nSX, int nSY, int nEX, int nEY, unsigned char nMapAttr, bool bDrawInTemp = false );
	bool	MergeNmf( const TCHAR* szFilename, int nSZ, int nSX, int nEZ, int nEX );	
};


// Inline functions
#include "NKTerrain.inl"


#endif // _NKTERRAIN_H_
