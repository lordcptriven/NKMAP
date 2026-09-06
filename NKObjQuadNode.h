//-----------------------------------------------------------------------------
// File : NKObjQuadNode.h
// Desc :
//-----------------------------------------------------------------------------

#ifndef _NKOBJQUADNODE_H_
#define _NKOBJQUADNODE_H_


#include "NKVertexType.h"


class CNKCamera;
class CDecoObj;


//-----------------------------------------------------------------------------
// Name : class CNKObjQuadNode
// Desc : The node of a quadtree for object set
//-----------------------------------------------------------------------------
class CNKObjQuadNode
{
protected:
	static LPDIRECT3D7				ms_pD3D;						// The main D3D object
	static LPDIRECT3DDEVICE7		ms_pd3dDevice;					// The D3D rendering device
	static TCHAR*					ms_szFilename;					// Filename of object data to restore water vertex buffer

	// Quad node info
	CNKObjQuadNode*					m_pLLeft;						// Lower left node
	CNKObjQuadNode*					m_pLRight;						// Lower right node
	CNKObjQuadNode*					m_pULeft;						// Upper left node
	CNKObjQuadNode*					m_pURight;						// Upper right node
	D3DXVECTOR3						m_vMin;							// Min & max vertices for bounding box
	D3DXVECTOR3						m_vMax;
	unsigned char					m_nX;							// X index of a node
	unsigned char					m_nZ;							// Z index of a node
	static int						ms_nCellSize;					// Width & height of a cell ( one cell include 4x4 tiles )
	static int						ms_nTreeWidth;					// Width & height of object quadtree

	// DecoObj info
	list<CDecoObj*>					m_lstDecoObj;					// DecoObj list
	static CDecoObj*				ms_pOpenDeco;					// DecObj opened
	static float					ms_fRenderDist;					// Distance to render
	static float					ms_fBlendDist;					// Distance to blend
	
	// Water info
	unsigned char					m_nBaseWaterSegment;			// Segment of the base water ( used for flags in maptool - which water is drawn ( SW, SE, NW, NE ) )
	unsigned char					m_nWaveWaterSegment1;			// Segment of the wave water ( max 8 )
	unsigned char					m_nWaveWaterSegment2;			// Segment of the wave water ( max 8 )
	WORD							m_wBaseVBIndex;					// Vertex buffer index of the base water
	WORD							m_wWaveVBIndex;					// Vertex buffer index of the wave water
	static LPDIRECT3DVERTEXBUFFER7	ms_pBaseWaterVB;				// Vertex buffer of the base water
	static LPDIRECT3DVERTEXBUFFER7	ms_pWaveWaterVB;				// Vertex buffer of the wave water
	static DWORD					ms_dwBaseWaterVBSize;			// Size of vertex buffer of the base water
	static DWORD					ms_dwWaveWaterVBSize;			// Size of vertex buffer of the wave water
	static WORD*					ms_pBaseWaterIB;				// Index buffer of the base water
	static WORD*					ms_pWaveWaterIB;				// Index buffer of the wave water
	static DWORD					ms_dwBaseIndexCount;			// Count of vertex indices of the base water
	static DWORD					ms_dwWaveIndexCount;			// Count of vertex indices of the wave water
	static IndexedTexture*			ms_pIndexedTexture;				// Indexed texture for the water
	static int						ms_nBaseWaterTexID;				// Texture index of the base water
	static int						ms_nWaveWaterTexID1;			// Texture index of the wave water 1
	static int						ms_nWaveWaterTexID2;			// Texture index of the wave water 2	
	static float*					ms_pWaterHeights;				// Water height of each node
	static int						ms_nWaterHeightWidth;			// Width of the water heights
	static float					ms_fBaseWaterMove;				// Translation value of a base water texture
	static float					ms_fWaveWaterMove1;				// Translation value of a wave water 1 texture
	static float					ms_fWaveWaterMove2;				// Translation value of a wave water 2 texture
	static D3DMATRIX				ms_matTexTrans;					// Transform matrix for a translation of the water texture
	
	// Variables used in map tool
	CNKObjQuadNode*					m_pParent;						// Parent node
	static CNKObjQuadNode*			ms_aQuadNodes[64][64];			// Quad node array to access directly ( Nodes[z][x] )
	unsigned char					m_nBaseWaterFlags;				// Flags of base waters - which water is drawn ( SW, SE, NW, NE )
	unsigned char					m_nWaterGroupID;				// Group ID of the water
	NKVERTEXLT*						m_pWaveWaterVert1;				// Vertex buffer of the wave water
	NKVERTEXLT*						m_pWaveWaterVert2;				// Vertex buffer of the wave water
	static float*					ms_pHeightOfWaterGroup;			// Heights of water groups
	static TCHAR					ms_aNamesOfWaterGroup[20][20];	// Names of water groups
	static NKVERTEXLT*				ms_pBaseWaterTempVB;			// Temporary vertex buffer of the base water
	static NKVERTEXLT*				ms_pWaveWaterTempVB;			// Temporary vertex buffer of the wave water
	static DWORD					ms_dwBaseVertexCount;			// Vertex count of the base water
	static DWORD					ms_dwWaveVertexCount;			// Vertex count of the wave water
	static int						ms_nBaseWaterSize;				// Width & height of the base water
	static int						ms_nWaveWaterSize;				// Width & height of the wave water
	static unsigned char*			ms_pCoastLineData;				// Coastline data for generating the wave water
	static DWORD					ms_dwBaseWaterAlpha;			// Alpha of a base water

protected:
	static bool	InitWaterVertexBuffer();

	// Functions used in map tool
	void	RecalcUpperBoundingBox();

public:
	void GetPickedDecoObjByDrag(D3DXVECTOR3 nStartX, D3DXVECTOR3 nEndX, D3DXVECTOR3 nStartY, D3DXVECTOR3 nEndY, list<CDecoObj*> & ppReturn, int & nRet);
	CNKObjQuadNode();
	CNKObjQuadNode( int nLevel, int nLeftX, int nLeftZ, int nRightX, int nRightZ, FILE* fp );
	virtual ~CNKObjQuadNode();

	static bool	LoadObj( const TCHAR* szFilepath, const TCHAR* szFilename, IndexedTexture* pIndexedTexture );
	static bool	LoadWat( const TCHAR* szFilename );
	static void	Destroy();
	static void	PrepareRender();
	void	Render( int nCullingState );
	static void	RenderWater();
	static inline void	SetOpenDeco( CDecoObj* pDecoObj );
	static inline CDecoObj*	GetOpenDeco();
	static inline void	SetRenderDist( float fRenderDist );
	static inline void	SetBlendDist( float fBlendDist );
	static float	GetHeightOnWater( float fZ, float fX );
	static float	GetHeightOnObject( float fZ, float fX, float fY );
	static HRESULT	RestoreDeviceObjects( LPDIRECT3D7 pD3D, LPDIRECT3DDEVICE7 pd3dDevice );
	static HRESULT	InvalidateDeviceObjects();

	// Functions used in map tool
	CNKObjQuadNode( CNKObjQuadNode* pParent, int nLevel, int nLeftX, int nLeftZ,
					int nRightX, int nRightZ );
	CNKObjQuadNode( CNKObjQuadNode* pParent, int nLevel, int nLeftX, int nLeftZ,
					int nRightX, int nRightZ, FILE* fp );

	static bool	SaveNot( const TCHAR* szFilename );
	void	SaveNodeNot( FILE* fp );
	static bool	LoadNot( const TCHAR* szFilename, IndexedTexture* pIndexedTexture );
	static bool	LoadLst( const TCHAR* szFilename, IndexedTexture* pIndexedTexture );
	static bool	SaveObj( const TCHAR* szFilename );
	void	SaveNodeObj( FILE* fp );
	static bool SaveDecoList( const TCHAR* szFilename ); // 현재 등록된 내용을 토대로 DecoObjList.lst 파일을 생성해주는 함수.( .obj 파일을 단순 텍스트 형식으로 뽑은 파일, 구맵툴에서 쓰였던거 같다. ) 06-12-12 원석
	void AddRootList( CNKObjQuadNode* RootNode ); // 재귀호출로 루트 노드의 리스트에 오브젝트 플레이스 정보를 모아주는 함수.
	static bool	SaveWat( const TCHAR* szFilename );
	void	PrepareForSaving();
	void	CreateWaterVertexBufferForSaving();

	static inline CNKObjQuadNode*	GetQuadNode( int nZ, int nX );
	static inline void	SetCellSize( int nCellSize );
	static inline int	GetCellSize();
	static inline float	GetRenderDist();
	inline const D3DXVECTOR3&	GetMin() const;
	inline const D3DXVECTOR3&	GetMax() const;		

	void	FindQuadNode( CDecoObj* pDecoObj, CNKObjQuadNode** ppQuadNode, bool& bFind );
	bool	HasThisDecoObj( CDecoObj* pDecoObj );
	bool	IsDecoObjInNode( CDecoObj* pDecoObj );
	bool	IsAABBInNode( const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax );
	void	FindDecoObjFittedNode( CDecoObj* pDecoObj, CNKObjQuadNode** ppNode, float& fArea );
	void	FindAABBFittedNode( const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax,
								CNKObjQuadNode** ppNode, float& fArea );
	void	GetPickedDecoObj( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir,
								float& rDist, CDecoObj** ppPickedDecoObj,
								CNKObjQuadNode** ppPickedNode = NULL );

	void	AddDecoObj( CDecoObj* pDecoObj );
	void	ModifyDecoObjLolo( int nIndex );
	void	ModifyDecoObjLoloID( int nIndex, int nValue );
	void	RemoveDecoObj( CDecoObj* pDecoObj, bool bRelease = true );
	void	RemoveDecoObjs( int nIndex );
	void	RemoveAllDecoObjs();
	static void	RemoveAllDecoObjsInRegion( int nSX, int nSZ, int nEX, int nEZ );

	void	RecalcBoundingBox();	
	void	RecalcAllBoundingBox();
	void	RecalcAllBoundingBoxForSaving();
	static void	ScaleAllObjPosition( float fXZScale, float fYScale );
	static void	OffsetAllObjPosition( float fScale );
	void	ScaleObjPosition( float fXZScale, float fYScale );
	void	OffsetObjPosition( float fScale );

	static DWORD	GetBaseWaterAlpha();
	static float	GetBaseWaterMove();
	static inline float	GetWaveWaterMove1();
	static inline float	GetWaveWaterMove2();
	static inline void	SetBaseWaterAlpha( DWORD dwWaterAlpha );
	static inline void	SetHeightOfWaterGroup( int nGroupID, float fWaterHeight );
	static inline float	GetHeightOfWaterGroup( int nGroupID );
	static inline void	SetNameOfWaterGroup( int nGroupID, const TCHAR* szName );
	static inline TCHAR*	GetNameOfWaterGroup( int nGroupID );
	static inline int	GetBaseWaterTexID();
	static inline int	GetWaveWaterTexID1();
	static inline int	GetWaveWaterTexID2();
	static inline void	SetBaseWaterTexID( int nTextureID );
	static inline void	SetWaveWaterTexID1( int nTextureID );
	static inline void	SetWaveWaterTexID2( int nTextureID );
	static inline int	GetBaseWaterSize();
	static inline int	GetWaveWaterSize();
	void	PaintAllBaseWater( int nGroupID );
	static void	PaintAllWaveWater( int nGroupID );
	void	AutoPaintWaveWater( int nGroupID );
	void	PaintBaseWater( int nIndex, int nGroupID );
	void	EraseBaseWater( int nIndex, int nGroupID );	
	int		PaintWaveWater( int nGroupID, NKVERTEXLT aWaveWaterVB[27], int nSegment,
							const D3DXVECTOR3& rMin, const D3DXVECTOR3& rMax );
	bool	EraseWaveWater( int nGroupID, const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir );
	void	RemoveWaterWithSameGroup( int nGroupID );
	static void	RemoveWaterGroup( int nGroupID );
	bool	GetPickedWaveWater( int nGroupID, NKVERTEXLT aWaveWaterVB[27], int& rSegment,
								const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir );
	bool	GetWaveWaterVertices( int nGroupID, int nWhichWave,
									D3DXVECTOR3 aWaveWaterVert[27], int& rSegment );
	void	ModifiedWaterHeight( int nGroupID );
	void	EraseInvisibleWaters();
	void	CalcWaterMapAttr( unsigned char* pAttrMap, int nAttrMapWidth );
	void	CalcObjectMapAttr( int nAttrMapWidth, bool bInsideHouse );
	static bool	MergeNot( const TCHAR* szFilename, int nSZ, int nSX, int nEZ, int nEX );
	void	MergeNodeNot( FILE* fp, int nSZ, int nSX, int nEZ, int nEX );

	static inline void	ToggleShowLineEffect();
};


// Inline functions
#include "NKObjQuadNode.inl"


#endif // _NKOBJQUADNODE_H_