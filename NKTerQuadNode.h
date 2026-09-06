//-----------------------------------------------------------------------------
// File : NKTerQuadNode.h
// Desc :
//-----------------------------------------------------------------------------

#ifndef _NKTERQUADNODE_H_
#define _NKTERQUADNODE_H_


#include "NKTerrain.h"


//-----------------------------------------------------------------------------
// Name : class CNKTerQuadNode
// Desc : The node of a quadtree for a terrain
//-----------------------------------------------------------------------------
class CNKTerQuadNode
{
protected:
	CNKTerQuadNode*		m_pLLeft;							// Lower left node
	CNKTerQuadNode*		m_pLRight;							// Lower right node
	CNKTerQuadNode*		m_pULeft;							// Upper left node
	CNKTerQuadNode*		m_pURight;							// Upper right node
	D3DXVECTOR3			m_vMin;								// Min & max vertices for bounding box
	D3DXVECTOR3			m_vMax;
	WORD				m_wVBIndex;							// Vertex buffer index
	unsigned char		m_nL1TexID;							// Texture ID of layer 1
	unsigned char		m_nL2TexID;							// Texture ID of layer 2
	unsigned char		m_nL3TexID;							// Texture ID of layer 3

	// Variables used in map tool
	CNKTerQuadNode*		m_pParent;							// Parent node

public:
	static CNKTerrain*	ms_pTerrain;						// Static variable to access index buffer of CNKTerrain
	static float		ms_fdt;								// Offset of texture coordinate
	static int			ms_nVBIdxCnt;						// Vertex buffer index count	

protected:
	// Variables used in map tool
	void	RecalcUpperBoundingBox();

public:
	CNKTerQuadNode();
	CNKTerQuadNode( int nLevel, int nLeftX, int nLeftZ, int nRightX, int nRightZ,
					unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID );
	virtual ~CNKTerQuadNode();	

	void	InitVertexBuffer( int nLeftX, int nLeftZ, int nRightX, int nRightZ,
								int nOffsetX, int nOffsetZ, NKVERTEXT2* pVB );
	void	Render( int nCullingState );
	void	GetPickedPoint( const D3DXVECTOR3& rOrigin, const D3DXVECTOR3& rDir, float& rDist );							

	// Functions used in map tool
	CNKTerQuadNode( CNKTerQuadNode* pParent, int nLevel, int nLeftX, int nLeftZ, int nRightX, int nRightZ );
	CNKTerQuadNode( CNKTerQuadNode* pParent, int nLevel, int nLeftX, int nLeftZ, int nRightX, int nRightZ,
					unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID );
	void	ModifyQuadTree( int nLeftX, int nLeftZ, int nRightX, int nRightZ,
							int nSZ, int nSX, int nEZ, int nEX,
							unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID );

	void	RenderGrid( int nLevel, DWORD dwColor, int nLeftX, int nLeftZ,
						int nRightX, int nRightZ, int nCullingState );
	void	RenderLayerGrid( int nLevel, DWORD dwColor, int nLayer, int nLeftX, int nLeftZ,
								int nRightX, int nRightZ, int nCullingState );

	WORD	GetVBIndex() const;
	unsigned char	GetL1TexID() const;
	unsigned char	GetL2TexID() const;
	unsigned char	GetL3TexID() const;
	void	SetL1TexID( unsigned char nTexID );
	void	SetL2TexID( unsigned char nTexID );
	void	SetL3TexID( unsigned char nTexID );

	void	RecalcAllBoundingBox( int nLeftX, int nLeftZ, int nRightX, int nRightZ );
	void	RecalcBoundingBox( int nZ, int nX );
	void	CountIndices();
};


// Inline functions
#include "NKTerQuadNode.inl"


#endif // _NKTERQUADNODE_H_