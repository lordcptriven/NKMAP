//-----------------------------------------------------------------------------
// File : MyNKApp.inl
// Desc :
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name : GetWorkingState()
// Desc :
//-----------------------------------------------------------------------------
inline int CMyNKApp::GetWorkingState() const
{
	return m_nWorkingState;
}


//-----------------------------------------------------------------------------
// Name : GetWorkingFilename()
// Desc :
//-----------------------------------------------------------------------------
inline const TCHAR* CMyNKApp::GetWorkingFilename() const
{
	return m_szWorkingFilename;
}


//-----------------------------------------------------------------------------
// Name : GetWorkingPath()
// Desc :
//-----------------------------------------------------------------------------
inline const TCHAR* CMyNKApp::GetWorkingPath() const
{
	return m_szWorkingPath;
}


//-----------------------------------------------------------------------------
// Name : DeleteWorkingFilename()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::DeleteWorkingFilename()
{
	SAFE_DELETE_ARRAY( m_szWorkingFilename );
}


//-----------------------------------------------------------------------------
// Name : GetD3DDevice()
// Desc :
//-----------------------------------------------------------------------------
inline LPDIRECT3DDEVICE7 CMyNKApp::GetD3DDevice() const
{
	return m_pd3dDevice;
}


//-----------------------------------------------------------------------------
// Name : GetTerrain()
// Desc :
//-----------------------------------------------------------------------------
inline CNKTerrain* CMyNKApp::GetTerrain() const
{
	return m_pTerrain;
}


//-----------------------------------------------------------------------------
// Name : GetCamera()
// Desc :
//-----------------------------------------------------------------------------
inline CNKCamera* CMyNKApp::GetCamera() const
{
	return m_pCamera;
}


//-----------------------------------------------------------------------------
// Name : GetCameraType()
// Desc :
//-----------------------------------------------------------------------------
inline int CMyNKApp::GetCameraType() const
{
	return m_nCameraType;
}


//-----------------------------------------------------------------------------
// Name : GetGridSize()
// Desc :
//-----------------------------------------------------------------------------
inline int CMyNKApp::GetGridSize() const
{
	return m_nGridSize;
}


//-----------------------------------------------------------------------------
// Name : SetGridColor()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetGridColor( DWORD dwGridColor )
{
	m_dwGridColor = dwGridColor;
}


//-----------------------------------------------------------------------------
// Name : SetGridSize()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetGridSize( int nGridSize )
{
	m_nGridSize = nGridSize;
}


//-----------------------------------------------------------------------------
// Name : GetRohX()
// Desc :
//-----------------------------------------------------------------------------
inline float CMyNKApp::GetRohX() const
{
	return m_vTarget.x;
}


//-----------------------------------------------------------------------------
// Name : GetRohZ()
// Desc :
//-----------------------------------------------------------------------------
inline float CMyNKApp::GetRohZ() const
{
	return m_vTarget.z;
}


//-----------------------------------------------------------------------------
// Name : SetBrushSize()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetBrushSize( int nBrushSize )
{
	m_nBrushSize = nBrushSize;
	MakeBrushMesh();
}


//-----------------------------------------------------------------------------
// Name : SetCircleBrush()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetCircleBrush( BOOL bCircleBrush )
{
	m_bCircleBrush = bCircleBrush;
}


//-----------------------------------------------------------------------------
// Name : SetBrushStrength()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetBrushStrength( int nBrushStrength )
{
	m_nBrushStrength = nBrushStrength;
}


//-----------------------------------------------------------------------------
// Name : SetEditingLayer()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetEditingLayer( int nEditingLayer )
{
	m_nEditingLayer = nEditingLayer;
}


//-----------------------------------------------------------------------------
// Name : SetTexEditType()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetTexEditType( int nTexEditType )
{
	m_nTexEditType = nTexEditType;
}


//-----------------------------------------------------------------------------
// Name : SetShadowEditType()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetShadowEditType( int nShadowEditType )
{
	m_nShadowEditType = nShadowEditType;
}


//-----------------------------------------------------------------------------
// Name : GetElapsedTime()
// Desc :
//-----------------------------------------------------------------------------
inline FLOAT CMyNKApp::GetElapsedTime() const
{
	return m_fElapsedTime;
}


//-----------------------------------------------------------------------------
// Name : GetDecoTexture()
// Desc :
//-----------------------------------------------------------------------------
inline IndexedTexture* CMyNKApp::GetDecoTexture() const
{
	return m_pDecoTexture;
}


//-----------------------------------------------------------------------------
// Name : SetObjEditType()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetObjEditType( int nObjEditType )
{
	m_nObjEditType = nObjEditType;
}


//-----------------------------------------------------------------------------
// Name : SetCurDecoObj()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetCurDecoObj( int nCurDecoObj )
{
	m_nCurDecoObj = nCurDecoObj;
}


//-----------------------------------------------------------------------------
// Name : ResetSelDecoObjList()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::ResetSelDecoObjList()
{
	m_lstSelDecoObjs.erase( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end() );
}


//-----------------------------------------------------------------------------
// Name : SetShowBBoxOfNode()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetShowNodeBound( BOOL bShowNodeBound )
{
	m_bShowNodeBound = bShowNodeBound;
}


//-----------------------------------------------------------------------------
// Name : SetModify()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetModify( int nWhich, BOOL bModify )
{
	m_bModify[nWhich] = bModify;
}


//-----------------------------------------------------------------------------
// Name : GetModify()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetModify( int nWhich ) const
{
	return m_bModify[nWhich];
}


//-----------------------------------------------------------------------------
// Name : ToggleShowTerrain()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::ToggleShowTerrain()
{
	m_bShowTerrain = !m_bShowTerrain;
}


//-----------------------------------------------------------------------------
// Name : GetShowTerrain()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetShowTerrain() const
{
	return m_bShowAttr_Map;
}

// nate 2006-01-12 : 속성맵 보이기
inline void CMyNKApp::ToggleShowAttr_Map()
{
	m_bShowAttr_Map = !m_bShowAttr_Map;
}

inline BOOL CMyNKApp::GetShowAttr_Map() const
{
	return m_bShowAttr_Map;
}

//-----------------------------------------------------------------------------
// Name : ToggleShowObject()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::ToggleShowObject()
{
	m_bShowObject = !m_bShowObject;
}

//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetShowObject() const
{
	return m_bShowObject;
}


//-----------------------------------------------------------------------------
// Name : SetVertEditType()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetVertEditType( int nVertEditType )
{
	m_nVertEditType = nVertEditType;
}


//-----------------------------------------------------------------------------
// Name : ToggleShowFog()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::ToggleShowFog()
{
	m_bShowFog = !m_bShowFog;
}


//-----------------------------------------------------------------------------
// Name : GetShowFog()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetShowFog() const
{
	return m_bShowFog;
}


//-----------------------------------------------------------------------------
// Name : GetShowCharacter()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetShowCharacter() const
{
	return m_bShowCharacter;
}


//-----------------------------------------------------------------------------
// Name : GetShowSky()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetShowSky() const
{
	return m_bShowSky;
}


//-----------------------------------------------------------------------------
// Name : ToggleShowWater()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::ToggleShowWater()
{
	m_bShowWater = !m_bShowWater;
}


//-----------------------------------------------------------------------------
// Name : GetShowWater()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetShowWater() const
{
	return m_bShowWater;
}


//-----------------------------------------------------------------------------
// Name : SetRandomRotation()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetRandomRotation( BOOL bRandomRotation )
{
	m_bPlaceRandomRotation = bRandomRotation;
}


//-----------------------------------------------------------------------------
// Name : SetLightTimeID()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetLightTimeID( int nTimeID )
{
	m_nLightTimeID = nTimeID;
}


//-----------------------------------------------------------------------------
// Name : SetLightDirection()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetLightDirection( int nTimeID, float fX, float fY, float fZ )
{
	m_aLightDirection[nTimeID] = D3DXVECTOR3( fX, fY, fZ );
}


//-----------------------------------------------------------------------------
// Name : SetDirectLightControl()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetDirectLightControl( BOOL bDirControl )
{
	m_bDirectLightControl = bDirControl;
}


//-----------------------------------------------------------------------------
// Name : GetClientWidth()
// Desc :
//-----------------------------------------------------------------------------
inline DWORD CMyNKApp::GetClientWidth() const
{
	return m_dwClientWidth;
}


//-----------------------------------------------------------------------------
// Name : GetClientHeight()
// Desc :
//-----------------------------------------------------------------------------
inline DWORD CMyNKApp::GetClientHeight() const
{
	return m_dwClientHeight;
}


//-----------------------------------------------------------------------------
// Name : GetShowProgress()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetShowProgress() const
{
	return m_bShowProgress;
}


//-----------------------------------------------------------------------------
// Name : SetShadowColor()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetShadowColor( float fR, float fG, float fB )
{
	m_fShadowR = fR;
	m_fShadowG = fG;
	m_fShadowB = fB;
}


//-----------------------------------------------------------------------------
// Name : SetDrawShadowCurrentMap()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetDrawShadowCurrentMap( BOOL bDrawShadowCurrentMaps )
{
	m_bDrawShadowCurrentMaps = bDrawShadowCurrentMaps;
}


//-----------------------------------------------------------------------------
// Name : GetWorldTime()
// Desc :
//-----------------------------------------------------------------------------
inline int CMyNKApp::GetWorldTime() const
{
	return m_nWorldHour;
}


//-----------------------------------------------------------------------------
// Name : SetWaterType()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetWaterType( int nWaterType )
{
	m_nWaterType = nWaterType;
}


//-----------------------------------------------------------------------------
// Name : SetWaterEditType()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetWaterEditType( int nEditType )
{
	m_nWaterEditType = nEditType;
}


//-----------------------------------------------------------------------------
// Name : SetShowWaterGuideLine()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetShowWaterGuideLine( BOOL bWaterGuideLine )
{
	m_bShowWaterGuideLine = bWaterGuideLine;
}


//-----------------------------------------------------------------------------
// Name : SetCurMapAttribute()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetCurMapAttribute( int nIndex )
{
	m_nCurMapAttr = nIndex;
}


//-----------------------------------------------------------------------------
// Name : SetAttmapVisibleRange()
// Desc :
//-----------------------------------------------------------------------------
inline void CMyNKApp::SetAttmapVisibleRange( int nRange )
{
	m_nAttMapVisibleRange = nRange;
}


//-----------------------------------------------------------------------------
// Name : IsLoadedGameMap()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::IsLoadedGameMap() const
{
	return m_bLoadedGameMap;
}


//-----------------------------------------------------------------------------
// Name : GetApplyBlend()
// Desc :
//-----------------------------------------------------------------------------
inline BOOL CMyNKApp::GetApplyBlend() const
{
	return m_bApplyObjBlending;
}


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------