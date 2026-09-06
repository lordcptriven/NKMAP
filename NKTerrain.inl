//-----------------------------------------------------------------------------
// File : NKTerrain.inl
// Desc :
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name : GetTriangleCount()
// Desc :
//-----------------------------------------------------------------------------
inline DWORD CNKTerrain::GetTriangleCount() const
{
	return m_dwTriangles;
}


//-----------------------------------------------------------------------------
// Name : GetD3DDevice()
// Desc :
//-----------------------------------------------------------------------------
inline LPDIRECT3DDEVICE7 CNKTerrain::GetD3DDevice() const
{
	return m_pd3dDevice;
}


//-----------------------------------------------------------------------------
// Name : GetHMWidth()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKTerrain::GetHMWidth() const
{
	return m_nHMWidth;
}


//-----------------------------------------------------------------------------
// Name : GetPatchSize()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKTerrain::GetPatchSize() const
{
	return m_nPatchSize;
}


//-----------------------------------------------------------------------------
// Name : GetHeightScale()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKTerrain::GetHeightScale() const
{
	return m_nHeightScale;
}


//-----------------------------------------------------------------------------
// Name : GetHeights()
// Desc :
//-----------------------------------------------------------------------------
inline float CNKTerrain::GetHeights( int nZ, int nX ) const
{
	return m_pHeights[ nX + nZ*m_nHMWidth] * m_nHeightScale;
}


//-----------------------------------------------------------------------------
// Name : SetQuadNodePointer()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::SetQuadNodePointer( int nZ, int nX, CNKTerQuadNode* pNode )
{
	m_aQuadNodes[nZ][nX] = pNode;
}


//-----------------------------------------------------------------------------
// Name : GetQuadTreeDepth()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKTerrain::GetQuadTreeDepth() const
{
	return m_nQuadTreeDepth;
}


//-----------------------------------------------------------------------------
// Name : SetViewLayers()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::SetViewLayers( int nViewLayers )
{
	m_nViewLayers = nViewLayers;
}


//-----------------------------------------------------------------------------
// Name : GetTextureCount()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKTerrain::GetTextureCount() const
{
	return m_vecTerTex.size();
}


//-----------------------------------------------------------------------------
// Name : GetTextureName()
// Desc :
//-----------------------------------------------------------------------------
inline TCHAR* CNKTerrain::GetTextureName( int nIndex ) const
{
	return m_vecTerTex[nIndex]->GetTextureName( 0 );
}


//-----------------------------------------------------------------------------
// Name : GetTerrainName()
// Desc :
//-----------------------------------------------------------------------------
inline const TCHAR* CNKTerrain::GetTerrainName() const
{
	return m_szTerName;
}


//-----------------------------------------------------------------------------
// Name : GetMinHeight()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKTerrain::GetMinHeight() const
{
	return m_nMinHeight * m_nHeightScale;
}


//-----------------------------------------------------------------------------
// Name : GetMaxHeight()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKTerrain::GetMaxHeight() const
{
	return m_nMaxHeight * m_nHeightScale;
}


//-----------------------------------------------------------------------------
// Name : GetTimeID()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKTerrain::GetTimeID() const
{
	return m_nTimeID;
}


//-----------------------------------------------------------------------------
// Name : SetLightDirection()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::SetLightDirection( int nTimeID, float fX, float fY, float fZ )
{
	m_aDirectLightDir[nTimeID] = D3DXVECTOR3( fX, fY, fZ );
}


//-----------------------------------------------------------------------------
// Name : GetLightDirection()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::GetLightDirection( int nTimeID, float& rX,float& rY, float& rZ )
{
	rX = m_aDirectLightDir[nTimeID].x;
	rY = m_aDirectLightDir[nTimeID].y;
	rZ = m_aDirectLightDir[nTimeID].z;
}


//-----------------------------------------------------------------------------
// Name : GetLightDirection()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::GetLightDirection( float& rX,float& rY, float& rZ )
{
	rX = m_aDirectLightDir[m_nTimeID].x;
	rY = m_aDirectLightDir[m_nTimeID].y;
	rZ = m_aDirectLightDir[m_nTimeID].z;
}


//-----------------------------------------------------------------------------
// Name : SetDirectLightColor()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::SetDirectLightColor( int nTimeID, float fR, float fG, float fB )
{
	m_aDirectLightR[nTimeID] = fR;
	m_aDirectLightG[nTimeID] = fG;
	m_aDirectLightB[nTimeID] = fB;
}


//-----------------------------------------------------------------------------
// Name : GetDirectLightColor()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::GetDirectLightColor( int nTimeID, int& rR, int& rG, int& rB )
{
	rR = m_aDirectLightR[nTimeID] * 255;
	rG = m_aDirectLightG[nTimeID] * 255;
	rB = m_aDirectLightB[nTimeID] * 255;
}


//-----------------------------------------------------------------------------
// Name : SetAmbientColor()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::SetAmbientColor( int nTimeID, float fR, float fG, float fB )
{
	m_aAmbientLightR[nTimeID] = fR;
	m_aAmbientLightG[nTimeID] = fG;
	m_aAmbientLightB[nTimeID] = fB;
}


//-----------------------------------------------------------------------------
// Name : GetAmbientColor()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKTerrain::GetAmbientColor( int nTimeID, int& rR, int& rG, int& rB )
{
	rR = m_aAmbientLightR[nTimeID] * 255;
	rG = m_aAmbientLightG[nTimeID] * 255;
	rB = m_aAmbientLightB[nTimeID] * 255;
}


//-----------------------------------------------------------------------------
// Name : GetMapAttrColor()
// Desc :
//-----------------------------------------------------------------------------
inline DWORD CNKTerrain::GetMapAttrColor( int nIndex )
{
	return m_aMapAttrColor[nIndex];
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