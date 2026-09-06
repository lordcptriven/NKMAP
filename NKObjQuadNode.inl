//-----------------------------------------------------------------------------
// File : NKObjQuadNode.inl
// Desc :
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetCellSize( int nCellSize )
{
	ms_nCellSize = nCellSize;
}


//-----------------------------------------------------------------------------
// Name : GetQuadNodePointer()
// Desc :
//-----------------------------------------------------------------------------
inline CNKObjQuadNode* CNKObjQuadNode::GetQuadNode( int nZ, int nX )
{
	return ms_aQuadNodes[nZ][nX];
}


//-----------------------------------------------------------------------------
// Name : SetOpenDeco()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetOpenDeco( CDecoObj* pOpenDeco )
{
	ms_pOpenDeco = pOpenDeco;
}


//-----------------------------------------------------------------------------
// Name : GetOpenDeco()
// Desc :
//-----------------------------------------------------------------------------
inline CDecoObj* CNKObjQuadNode::GetOpenDeco()
{
	return ms_pOpenDeco;
}


//-----------------------------------------------------------------------------
// Name : SetRenderDist()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetRenderDist( float fRenderDist )
{
	ms_fRenderDist = fRenderDist;
}


//-----------------------------------------------------------------------------
// Name : GetRenderDist()
// Desc :
//-----------------------------------------------------------------------------
inline float CNKObjQuadNode::GetRenderDist()
{
	return ms_fRenderDist;
}


//-----------------------------------------------------------------------------
// Name : SetBlendDist()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetBlendDist( float fBlendDist )
{
	ms_fBlendDist = fBlendDist;
}


//-----------------------------------------------------------------------------
// Name : GetCellSize()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKObjQuadNode::GetCellSize()
{
	return ms_nCellSize;
}


//-----------------------------------------------------------------------------
// Name : GetMin()
// Desc :
//-----------------------------------------------------------------------------
inline const D3DXVECTOR3& CNKObjQuadNode::GetMin() const
{
	return m_vMin;
}


//-----------------------------------------------------------------------------
// Name : GetMax()
// Desc :
//-----------------------------------------------------------------------------
inline const D3DXVECTOR3& CNKObjQuadNode::GetMax() const
{
	return m_vMax;
}


//-----------------------------------------------------------------------------
// Name : GetBaseWaterTexID()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKObjQuadNode::GetBaseWaterTexID()
{
	return ms_nBaseWaterTexID;
}


//-----------------------------------------------------------------------------
// Name : GetWaveWaterTexID1()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKObjQuadNode::GetWaveWaterTexID1()
{
	return ms_nWaveWaterTexID1;
}


//-----------------------------------------------------------------------------
// Name : GetWaveWaterTexID2()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKObjQuadNode::GetWaveWaterTexID2()
{
	return ms_nWaveWaterTexID2;
}


//-----------------------------------------------------------------------------
// Name : SetBaseWaterTexID()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetBaseWaterTexID( int nTextureID )
{
	ms_nBaseWaterTexID = nTextureID;
}


//-----------------------------------------------------------------------------
// Name : SetWaveWaterTexID1()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetWaveWaterTexID1( int nTextureID )
{
	ms_nWaveWaterTexID1 = nTextureID;
}


//-----------------------------------------------------------------------------
// Name : SetWaveWaterTexID2()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetWaveWaterTexID2( int nTextureID )
{
	ms_nWaveWaterTexID2 = nTextureID;
}


//-----------------------------------------------------------------------------
// Name : GetBaseWaterSize()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKObjQuadNode::GetBaseWaterSize()
{
	return ms_nBaseWaterSize;
}


//-----------------------------------------------------------------------------
// Name : GetWaveWaterSize()
// Desc :
//-----------------------------------------------------------------------------
inline int CNKObjQuadNode::GetWaveWaterSize()
{
	return ms_nWaveWaterSize;
}


//-----------------------------------------------------------------------------
// Name : GetBaseWaterMove()
// Desc :
//-----------------------------------------------------------------------------
inline float CNKObjQuadNode::GetBaseWaterMove()
{
	return ms_fBaseWaterMove;
}


//-----------------------------------------------------------------------------
// Name : GetWaveWaterMove1()
// Desc :
//-----------------------------------------------------------------------------
inline float CNKObjQuadNode::GetWaveWaterMove1()
{
	return ms_fWaveWaterMove1;
}


//-----------------------------------------------------------------------------
// Name : GetWaveWaterMove2()
// Desc :
//-----------------------------------------------------------------------------
inline float CNKObjQuadNode::GetWaveWaterMove2()
{
	return ms_fWaveWaterMove2;
}


//-----------------------------------------------------------------------------
// Name : GetBaseWaterAlpha()
// Desc :
//-----------------------------------------------------------------------------
inline DWORD CNKObjQuadNode::GetBaseWaterAlpha()
{
	return ms_dwBaseWaterAlpha;
}


//-----------------------------------------------------------------------------
// Name : SetBaseWaterAlpha()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetBaseWaterAlpha( DWORD dwWaterAlpha )
{
	ms_dwBaseWaterAlpha = dwWaterAlpha;
}


//-----------------------------------------------------------------------------
// Name : SetHeightOfWaterGroup()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetHeightOfWaterGroup( int nGroupID, float fWaterHeight )
{
	ms_pHeightOfWaterGroup[nGroupID] = fWaterHeight;
}


//-----------------------------------------------------------------------------
// Name : GetHeightOfWaterGroup()
// Desc :
//-----------------------------------------------------------------------------
inline float CNKObjQuadNode::GetHeightOfWaterGroup( int nGroupID )
{
	return ms_pHeightOfWaterGroup[nGroupID];
}


//-----------------------------------------------------------------------------
// Name : SetNameOfWaterGroup()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetNameOfWaterGroup( int nGroupID, const TCHAR* szName )
{
	_tcscpy( ms_aNamesOfWaterGroup[nGroupID], szName );
	ms_aNamesOfWaterGroup[nGroupID][_tcslen(szName)] = NULL;
}


//-----------------------------------------------------------------------------
// Name : GetNameOfWaterGroup()
// Desc :
//-----------------------------------------------------------------------------
inline TCHAR* CNKObjQuadNode::GetNameOfWaterGroup( int nGroupID )
{
	return ms_aNamesOfWaterGroup[nGroupID];
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