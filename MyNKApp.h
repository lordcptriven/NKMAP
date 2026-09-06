// MyNKApp.h: interface for the CMyNKApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYNKAPP_H__D8ED989B_BD10_4189_9158_D373FFE5A234__INCLUDED_)
#define AFX_MYNKAPP_H__D8ED989B_BD10_4189_9158_D373FFE5A234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3dapp.h"
#include "NKVertexType.h"
#include "NKCamera.h"
#include "NKTerrain.h"
#include "NKObjQuadNode.h"
#include "Undo_Object.h"


class IndexedTexture;
class CShadowRect;
class CDecoObj;
class CNkCharacter;
class CSkyPlane;
class CLightCtrl;
class CProgressDlg;
class CRect;


//-----------------------------------------------------------------------------
// Working state
//-----------------------------------------------------------------------------
#define	NONE				0
#define	VERTEX_EDIT			1
#define TEXTURE_PAINT		2
#define	OBJECT_EDIT			3
#define	LIGHT_EDIT			4
#define	ATMOSPHERE_EDIT		5
#define ATTRIBUTE_EDIT		6


//-----------------------------------------------------------------------------
// Name : class CNKMapToolApp
// Desc : 
//-----------------------------------------------------------------------------
class CMyNKApp : public CD3DApplication  
{
protected:
	// Work state & misc..
	int					m_nWorkingState;				// Working state
	TCHAR*				m_szWorkingFilename;			// Working filename
	TCHAR*				m_szWorkingPath;				// Working path
	BOOL				m_bModify[3];					// 0: Nmf or Map file, 1: Bld file, 2: Not file
	int					m_nMouseDragStartX;
	int					m_nMouseDragStartZ;
	int					m_nMouseDragEndX;
	int					m_nMouseDragEndZ;
	BOOL				m_bDrag;
	BOOL				m_bAdd;
	BOOL				m_bDeselect;
	int					m_nMouseX, m_nMouseY;			// Screen coordinate of mouse
	D3DXVECTOR3			m_vMouseDrag1, m_vMouseDrag2;	//ulyssesme-20060928 : 드래그 시작지점
	D3DXVECTOR3			m_vMouseDrag3, m_vMouseDrag4;	//ulyssesme-20060928 : 드래그 끝지점
	DWORD				m_dwBgColor;					// Backgroung color
	int					m_nTileSX, m_nTileEX;			// X region of working tiles
	int					m_nTileSZ, m_nTileEZ;			// Z region of working tiles
	int					m_nDirtyRegionSX;				// Start x of edited region
	int					m_nDirtyRegionEX;				// End x of edited region
	int					m_nDirtyRegionSZ;				// Start z of edited region
	int					m_nDirtyRegionEZ;				// End z of edited region
	BOOL				m_bLoadedGameMap;				// If the map for the game is loaded
	
	// Camera
	CNKCamera*			m_pCamera;						// CNKCamera pointer
	int					m_nCameraType;					// 0: Observing mode, 1: Flying mode
	D3DXVECTOR3			m_vTarget;						// Position of target	
	D3DXVECTOR3			m_vPickDirection;				// Direction of a picking ray
	D3DXVECTOR3			m_vPickOrigin;					// Origin of a picking ray
	D3DXVECTOR3			m_vPickedPoint;					// Picked point	

	// Terrain
	CNKTerrain*			m_pTerrain;						// CNKTerrain pointer
	BOOL				m_bShowTerrain;					// If a terrain is shown
	BOOL				m_bShowAttr_Map;				// nate 2006-01-12 : 속성맵 보이기
	int					m_nGridSize;					// Grid size
	DWORD				m_dwGridColor;					// Grid color

	// Brush
	NKVERTEXL			m_aBrushVB[145];				// Vertex buffer for a brush mesh
	DWORD				m_dwBrushVertNum;				// Number of brush vertices
	DWORD				m_dwBrushColor;					// Brush color
	int					m_nBrushSize;					// Brush size
	int					m_nBrushStrength;				// Brush strength
	BOOL				m_bCircleBrush;					// Brush shape

	// Axes & director
	NKVERTEXL			m_aAxesVB[12];					// Vertex buffer for axes and a director

	// Gizmo
	D3DXVECTOR3			m_vGizmoPosition;				// Position of gizmo
	D3DXVECTOR3			m_vGizmoRelativePos;			// Relative position of gizmo
	NKVERTEXL			m_aGizmoAxesVB[10];				// Vertex buffer of axes of gizmo
	NKVERTEXL			m_aGizmoXTailVB[6];				// Vertex buffer of x tail of gizmo
	NKVERTEXL			m_aGizmoYTailVB[6];				// Vertex buffer of y tail of gizmo
	NKVERTEXL			m_aGizmoZTailVB[6];				// Vertex buffer of z tail of gizmo
	D3DXVECTOR3			m_aGizmoAABB[10];				// Bounding box of gizmo
	int					m_nSelAxis;						// Selected axis ( 0: none, 1: x, 2: y, 3: z, 4: xz );

	// Terrain texture painting
	int					m_nEditingLayer;				// Editing layer ( Layer1: 1, layer2: 2, layer3: 3 )
	int					m_nTexEditType;					// Texture Editing type ( 0: paint, 1: erase )
	int					m_nCurTileTexture;				// Index of a current selected tile texture

	// Object editing
	BOOL				m_bShowObject;					// If a object is shown
	int					m_nObjEditType;					// Object editing type ( 0: select, 1: place, 2: move, 3: rotate )
	int					m_nCurDecoObj;					// Index of a current selected decoobj
	NKVERTEXL *			m_aDecoBoxVB;					// Vertex buffer for bounding boxes of selected decoobjs
	DWORD				m_dwBoxVertNum;					// Number of bounding boxe vertices
	BOOL				m_bShowNodeBound;				// If boundary planes of a node including a decoobj selected is shown
	NKVERTEXL			m_aNodeBoundVB[10];				// Vertex buffer of boundary planes of a node
	DWORD				m_dwNodeBoundColor;				// Color of boundary planes
	BOOL				m_bPlaceRandomRotation;			// Placing with random rotation angle
	float				m_fObjMinXZ;					// Minimum x & z of decoobj that can be located
	float				m_fObjMaxXZ;					// Maximum x & z of decoobj that can be located
	BOOL				m_bShowLocalAxis;               // 오브젝트의 액시스를 보여줄때 상대축으로 보여줄지에 대한 체크 여부.

	// Terrain vertex editing
	int					m_nControlUnit;					// Control unit for editing vertex ( 0: tile, 1: vertex )
	int					m_nVertEditType;				// Vertex editing type ( 0: raise, 1: lower, 2: flatten, 3: smooth )
	int					m_nPickVertexX;					// X index of a picked vertex
	int					m_nPickVertexZ;					// Z index of a picked vertex

	// Terrain light editing
	BOOL				m_bStopWorldTime;				// If the world time is stopped for fixing the terrain light
	BOOL				m_bDirectLightControl;			// Direction controller of a directional light of a terrain
	D3DXVECTOR3			m_vLightCtrlRelativePos;		// Relative position of a light controller
	int					m_nLightTimeID;					// World time ID for the light
	DWORD				m_dwLightCtrlColor;				// Color of a light controller
	D3DXVECTOR3			m_aLightDirection[6];			// Directions of a directional light
	NKVERTEXL			m_aLightCtrlXZVB[145];			// Vertex buffer(XZ) of a light controller
	NKVERTEXL			m_aLightCtrlXYVB[73];			// Vertex buffer(XY) of a light controller
	NKVERTEXL			m_aLightCtrlZYVB[73];			// Vertex buffer(ZY) of a light controller
	NKVERTEXL			m_aSunTraceVB[73];				// Vertex buffer of a trace helper of the sun
	NKVERTEXL			m_aSunLatitudeVB[145];			// Vertex buffer of a latitude helper of the sun
	float				m_fSunLatitude;					// Height of a latitude helper of the sun
	float				m_fSunTrace;					// Angle of a tracer helper of the sun
	NKVERTEXL			m_aLightCtrlExtraVB[18];		// Vertex buffer(extra) of a light controller
	int					m_nShadowEditType;				// Shadow edit type ( 0: paint, 1: erase )
	BOOL				m_bDrawShadowCurrentMaps;		// If shadow color is only drawn in a current map
	float				m_fShadowR;						// R channel of a Shadow color
	float				m_fShadowG;						// G channel of a Shadow color
	float				m_fShadowB;						// B channel of a Shadow color

	// Water editing
	BOOL				m_bShowWater;					// If a water is shown
	NKVERTEXLT			m_aBaseWaterVB[4];				// Vertex buffer of the base water plane
	NKVERTEXL			m_aWaterGuideVB[48];			// Vertex buffer of the water guide line
	NKVERTEXL			m_aNeighborWaterVert[800];		// Vertex buffer of neighbor waters
	NKVERTEXLT			m_aWaveWaterVB[27];				// Vertex buffer of the wave water plane
	WORD				m_aWaveWaterIB[96];				// Index buffer of the wave water plane
	DWORD				m_dwWaveIndexCount;				// Index count of the wave water plane
	DWORD				m_dwNeighWaterCount;			// Count of neighbor waters
	D3DXVECTOR3			m_aNeighWaveVert[324];			// Vertices of neighbor wave waters for the snap
	DWORD				m_dwNeighWaveVertCount;			// Count of neighbor wave waters for snap
	D3DXVECTOR3			m_vWaveWaterCenter;				// Center of the wave water plane
	BOOL				m_bWaveWaterModify;
	DWORD				m_dwSelWaterVertex;
	BOOL				m_bWaveWaterRotate;
	BOOL				m_bWaveWaterErase;	
	D3DMATRIX			m_matWaterTrans;				// Transform matrix of the water plane
	BOOL				m_bShowWaterGuideLine;			// If guide lines are shown
	int					m_nWaterType;					// Water type ( base or wave )
	int					m_nWaterEditType;				// Water editing type ( 0: paint, 1: erase )
	int					m_nWaterGroupID;				// Group ID of the water
	int					m_nWaterSegment;				// Segment of the wave water

	// Map Attribute
	int					m_nCurMapAttr;					// Current map attribute
	int					m_nAttMapVisibleRange;			// Visible range of the attribute map

	// World time
	int					m_nWorldHour;					// World time ( hour )
	float				m_fAccumulatedTime;				// Accumulated system time	

	// Fog
	BOOL				m_bShowFog;						// If a fog is shown
	DWORD				m_aFogColor[30];				// Fog color
	float				m_aFogStart[30];				// Fog start distance
	float				m_aFogEnd[30];					// Fog end distance

	// Light
	CLightCtrl*			m_pLightCtrl;					// Light control
	float				m_fPointRColor;					// R channel of a point light
	float				m_fPointGColor;					// G channel of a point light
	float				m_fPointBColor;					// B channel of a point light

	// Textures
	IndexedTexture*		m_pDecoTexture;					// Decoration textures
	IndexedTexture*		m_pTestTexture;					// Test textures
	
	// Character
	CNkCharacter*		m_pRoh;							// Character
	BOOL				m_bShowCharacter;				// If a character is shown
	CShadowRect*		m_pShadowRect;					// Simple shadow of a character

	// Sky
	CSkyPlane*			m_pSkyPlane;					// Sky dome
	BOOL				m_bShowSky;						// If the sky is shown

	// Progress state
	CProgressDlg*		m_pProgressDlg;					// Progress dialog
	BOOL				m_bShowProgress;				// If a progress dialog is shown

	// Etc
	BOOL				m_bApplyObjBlending;

public:

	list<CDecoObj*>		m_lstSelDecoObjs;				// List of selected DecoObjs
	list<CDecoObj*>		m_lstDragSelDecoObjs;			//ulyssesme-20060928 :  List of Draged, selected DecoObjs

protected:
	void	RenderDecoObjs();
	void	RenderAlphaObjs();
	void	RenderInterfaces();

	void	DrawTileTexture();
	void	DrawShadow();

	void	ResetSelDecoObjList();
	
	void	PlaceDecoObj();
	void	RemoveDecoObjsFromTree();
	void	UpdateObjEditParameters( CDecoObj* pDecoObj = NULL );
	

	void	ModifyHeights();
	void	ModifyOneVertex( int nOffset );
	void	PickVertex();
	void	ResetTerrainMinMaxHeights();

	void	ChangeLightDirection( BOOL bLatitude = FALSE, BOOL bTrace = FALSE, BOOL bModify = FALSE );

	void	MakeWaterMesh();
	void	GetPickedWaterPlane();
	void	GetPickedWaveWaterVertex();
	void	PaintBaseWater();
	void	EraseBaseWater();
	void	PaintWaveWater();
	void	EraseWaveWater();
	void	ResetWaveWaterVertex();
	void	ModifyWaveWaterVertex( BOOL bSnap = FALSE );
	void	RotateWaveWaterVertices( float fdDeg );
	void	GetPickedWaveWaterFromNode();

	void	CreateSky();
	void	InitFogParameters();

	void	ToggleGrid();
	void	InitAxes();
	void	InitGizmo();
	void	InitWaterMesh();
	
	void	CheckPickingAxes();
	void	SetHighlightAxis();
	void	MakeBrushMesh();
	void	SetFogParameters( BOOL bSetFog );

	static HRESULT ConfirmDevice( DDCAPS*, D3DDEVICEDESC7* );
	HRESULT OneTimeSceneInit();
	HRESULT FinalCleanup();
	HRESULT InitDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT Render();
	HRESULT FrameMove();

public:

	void	MakeNodeBoundMesh();
	void	CalcGizmoPosition();
	void	UpdateObjEditReferenceCount();

	void DragMouse();
	BOOL	NewTerrain( int nWHSegment, int nPatchSize,
						int nHeightScale, const TCHAR* szHeightMap );
	BOOL	LoadNmf( const TCHAR* szDirectory, const TCHAR* szFilename );
	BOOL	SaveNmf( const TCHAR* szFilename );
	BOOL	LoadMap( const TCHAR* szDirectory, const TCHAR* szFilename );
	BOOL	SaveMap( const TCHAR* szFilename );
	BOOL	ImportBldLst();
	BOOL	MergeMaps( const TCHAR* szNmfName, const TCHAR* szNotName, int nSZ, int nSX, int nEZ, int nEX );

	int		GetWorkingState() const;
	void	SetWorkingState( int nWorkingState );
	const TCHAR* GetWorkingFilename() const;
	void	SetWorkingFilename( const TCHAR* szFilename );
	void	DeleteWorkingFilename();
	const TCHAR* GetWorkingPath() const;	
	void	SetWorkingPath( const TCHAR* szPath );	
	BOOL	GetModify(int nWhich ) const;
	void	SetModify( int nWhich, BOOL bModify );
	BOOL	IsLoadedGameMap() const;

	void	MapMovement( int nUnit, int nZ, int nX );

	CNKCamera*	GetCamera() const;
	int		GetCameraType() const;
	void	SetCameraType( int nType );
	void	ResetCameraYPosition();
	float	GetRohX() const;
	float	GetRohZ() const;

	CNKTerrain*	GetTerrain() const;
	int		GetGridSize() const;
	void	SetGridSize( int nGridSize );
	void	SetGridColor( DWORD dwGridColor );
	void	ToggleShowTerrain();
	void	ToggleShowAttr_Map();
	void	ToggleShowObject();
	void	ToggleShowFog();
	void	ToggleShowCharacter();
	void	ToggleShowSky();
	void	ToggleShowWater();
	BOOL	GetShowTerrain() const;	
	BOOL	GetShowAttr_Map() const;
	BOOL	GetShowObject() const;
	BOOL	GetShowFog() const;	
	BOOL	GetShowCharacter() const;
	BOOL	GetShowSky() const;
	BOOL	GetShowWater() const;

	void	SetBrushSize( int nBrushSize );
	void	SetBrushStrength( int nBrushStrength );
	void	SetCircleBrush( BOOL bCircleBrush );

	void	SetEditingLayer( int nEditingLayer );
	void	SetTexEditType( int nTexEditType );
	void	SetCurSelTexture( int nIndex );
	void	SetFiltering( BOOL bFiltering );
	
	void	MakeDecoBoxMesh();
	void	SetShowNodeBound( BOOL bShowNodeBound );
	void	SetObjEditType( int nObjEditType );
	void	SetCurDecoObj( int nCurDecoObj );
	void	RemoveDecoObjsFromSelList( int nIndex );	
	void	MoveSelDecoObjs( float fdX, float fdY, float fdZ );
	void	RotateSelDecoObjs( float fdDeg, BOOL bSnap = FALSE );
	void	RotateSelDecoObjsByPivot( float fdDeg, D3DVECTOR Pivot ); // 특정좌표를 기준으로 회전시켜주는 함수. 06-09-12 원석 (Y는 무시)
	void	UpdateChangedDecoObjs();
	void	SetRandomRotation( BOOL bRandomRotation );
	void	AlignDecoObjs( int nAxis );
	void	ArrayDecoObjs( int nAxis, float fOffset, int nCount );
	void	SetShowLocalAxis( BOOL Value ) { m_bShowLocalAxis = Value; }

	void	SetControlUnit( int nControlUnit );
	void	SetVertEditType( int nVertEditType );

	void	SetDirectLightControl( BOOL bDirControl );
	void	SetLightTimeID( int nTimeID );
	void	SetLightDirection( int nTimeID, float fX, float fY, float fZ );
	void	MakeLightControlMesh();
	void	SetShadowEditType( int nShadowEditType );
	void	SetDrawShadowCurrentMap( BOOL bDrawShadowCurrentMaps );
	void	SetShadowColor( float fR, float fG, float fB );

	void	SetWaterGroupID( int nGroupID );
	void	SetWaterType( int nWaterType );
	void	SetWaterEditType( int nEditType );
	void	SetBaseWaterTexture( const TCHAR* szFilename );
	void	SetWaveWaterTexture1( const TCHAR* szFilename );
	void	SetWaveWaterTexture2( const TCHAR* szFilename );
	void	SetWaterSegment( int nSegment );
	void	SetShowWaterGuideLine( BOOL bWaterGuideLine );

	void	SetCurMapAttribute( int nIndex );
	void	SetAttmapVisibleRange( int nRange );

	BOOL	GetApplyBlend() const;

	void	ChangeWorldTime( int nWorldHour );
	int		GetWorldTime() const;

	void	InitProgress();
	void	DestroyProgress();
	void	SetProgressTitle( const TCHAR* szTitle );
	void	SetProgressContent( const TCHAR* szContent );
	void	SetProgressRange( int nLower, int nUpper );
	void	SetProgressPos( int nPos );
	void	SetProgressOffsetPos( int nOffsetPos );
	BOOL	GetShowProgress() const;

	DWORD	GetClientWidth() const;
	DWORD	GetClientHeight() const;
	FLOAT	GetElapsedTime() const;
	IndexedTexture*	GetDecoTexture() const;
	LPDIRECT3DDEVICE7	GetD3DDevice() const;

	float m_fCameraSpeed; // 카메라 스피드 조절 기능 확장(by 원석)
	float m_bCameraUnlimit; // 카메라 시야제한을 없애는 체크 기능. ///071022 추가

	CMyNKApp();
	~CMyNKApp();
	LRESULT	MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	// [10/26/2007 Theodoric] Undo를 위한 추가 함수

	CUndoObject m_UndoObj;
	void	MoveAndRotateSelDecoObjs( CDecoObj* pDecoObj, float fdX, float fdY, float fdZ, float fdir );
	int		GetCurDecoObj(){return m_nCurDecoObj;}

	// [2007/11/1 theodoric] 카메라 탑뷰
	BOOL		m_bCameraUnlimitBack;
	CNKCamera*	GetCamera(){return m_pCamera;}
	BOOL		IsCameraTopView(){ return m_pCamera->IsTopView();}
	void		ToggleCameraTopView(){SetCameraTopView(!IsCameraTopView());}
	void		SetCameraTopView(BOOL topView);
};


// Inline functions
#include "MyNKApp.inl"


#endif // !defined(AFX_MYNKAPP_H__D8ED989B_BD10_4189_9158_D373FFE5A234__INCLUDED_)
