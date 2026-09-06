// MyNKApp.cpp: implementation of the CMyNKApp class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include "NKMapTool.h"
#include "MainFrm.h"
#include "MyNKApp.h"
#include "NKMath.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "DecoObj.h"
#include "NkCharacter.h"
#include "SkyPlane.h"
#include "LightCtrl.h"
#include "GUtil.h"
#include "ProgressDlg.h"
#include "Config.h"

#include "D3DMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IndexedTexture*			g_pCapsyongTexture = NULL;
extern CBiped*			g_pBipedList[];
extern CCapsyong*		g_pBodyList[];
extern CDecoObjSet*		g_pDecoObjSet;
extern CNKObjQuadNode*	g_pObjTree;
list<CDecoObj*>			g_lstAlphaDeco;
list<CDecoObj*>			g_lstLastDeco;
list<CDecoObj*>			g_lstFarDeco;
DWORD					g_dwObjTriangles;
 
extern void PlayerInitialLoading();
extern void DeletePlayerData();
extern void DecoObjInitialLoading( const char *strFilePath, const char *strFilename );
extern void DeleteDecoObjData();


//-----------------------------------------------------------------------------
// Name: CMyNKApp()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyNKApp::CMyNKApp()
		:CD3DApplication()
{
	m_bDeselect = FALSE;
	m_aDecoBoxVB = new NKVERTEXL[24 * 10000];
	m_bDrag = FALSE;
	m_bAdd = FALSE;
	m_strWindowTitle  = TEXT( "NKMapTool" );
	m_bAppUseZBuffer  = TRUE;
	m_fnConfirmDevice = ConfirmDevice;

	m_szWorkingFilename = _T('\0');
	m_szWorkingPath = _T('\0');
	m_nWorkingState = NONE;
	m_dwBgColor = 0x00bbbbbb;
	m_bModify[0] = FALSE;
	m_bModify[1] = FALSE;
	m_bModify[2] = FALSE;
	m_bLoadedGameMap = FALSE;

	m_pCamera = NULL;
	m_nCameraType = 0;
	m_vTarget = NKVEC_ZERO;
	m_vPickDirection = NKVEC_ZERO;
	m_vPickOrigin = NKVEC_ZERO;
	m_vPickedPoint = NKVEC_ZERO;

	m_pTerrain = NULL;
	m_bShowTerrain = TRUE;
	m_bShowAttr_Map = false;
	m_nGridSize = -1;
	m_dwGridColor = 0x00dddd00;
	
	m_dwBrushColor = 0x000000ff;
	m_dwBrushVertNum = 0;
	m_nBrushSize = 1;
	m_nBrushStrength = 15;
	m_bCircleBrush = FALSE;

	m_nTexEditType = 0;
	m_nEditingLayer = 1;	
	m_nTileSX = -1;					m_nTileEX = -1;
	m_nTileSZ = -1;					m_nTileEZ = -1;
	m_nDirtyRegionSX = INT_MAX;		m_nDirtyRegionEX = INT_MIN;
	m_nDirtyRegionSZ = INT_MAX;		m_nDirtyRegionEZ = INT_MIN;

	m_bShowObject = TRUE;
	m_nObjEditType = 0;
	m_nCurDecoObj = -1;
	m_dwBoxVertNum = 0;
	m_bShowNodeBound = FALSE;
	m_dwNodeBoundColor = 0x6080e0e0;
	m_bPlaceRandomRotation = FALSE;
	m_bShowLocalAxis = FALSE;

	m_nControlUnit = 0;
	m_nVertEditType = 0;
	m_nPickVertexX = -1;
	m_nPickVertexZ = -1;

	m_bDirectLightControl = FALSE;
	m_bStopWorldTime = FALSE;
	m_dwLightCtrlColor = 0x00ff8000;
	m_nShadowEditType = 0;
	m_bDrawShadowCurrentMaps = FALSE;
	m_fSunTrace = 42.0f * NK_DEGTORAD;
	m_fSunLatitude = 74.31448f;

	m_bShowWater = TRUE;
	m_nWaterGroupID = -1;
	m_nWaterType = 0;
	m_nWaterEditType = 0;
	m_bWaveWaterModify = FALSE;
	m_bWaveWaterRotate = FALSE;
	m_bWaveWaterErase = FALSE;
	m_dwSelWaterVertex = -1;
	m_nWaterSegment = 4;
	m_bShowWaterGuideLine = TRUE;
	m_dwNeighWaterCount = 0;
	m_dwNeighWaveVertCount = 0;

	m_nCurMapAttr = 0;
	m_nAttMapVisibleRange = 9;

	m_nWorldHour = 14;
	m_fAccumulatedTime = 0.0f;	

	m_bShowFog = FALSE;
	m_bShowSky = FALSE;

	m_pRoh = NULL;
	m_pShadowRect = NULL;
	m_bShowCharacter = FALSE;
	m_pLightCtrl = NULL;
	m_pSkyPlane = NULL;
	m_pDecoTexture = NULL;
	m_pTestTexture = NULL;
	m_pProgressDlg = NULL;

	m_bApplyObjBlending = TRUE;

	m_fCameraSpeed = 1.0f; // 카메라 스피드 조절 기능 확장(by 원석)
	m_bCameraUnlimit = FALSE; // 카메라 시야제한을 없애는 체크 기능. ///071022 추가
}


//-----------------------------------------------------------------------------
// Name : ~CMyNKApp()
// Desc :
//-----------------------------------------------------------------------------
CMyNKApp::~CMyNKApp()
{
	delete [] m_aDecoBoxVB;
}


//-----------------------------------------------------------------------------
// Name: App_ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyNKApp::ConfirmDevice( DDCAPS* pddDriverCaps, D3DDEVICEDESC7* pd3dDeviceDesc )
{
    // This sample uses alpha textures and/or straight alpha. Make sure the 
    // device supports them
    //DWORD dwDeviceCaps = pd3dDeviceDesc->dpcTriCaps.dwTextureCaps;
    //if( dwDeviceCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
    //    return S_OK;
    //if( dwDeviceCaps & D3DPTEXTURECAPS_ALPHA )
    //    return S_OK;

    //return E_FAIL;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : MsgProc()
// Desc :
//-----------------------------------------------------------------------------
LRESULT	CMyNKApp::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// For mouse state
	static int							nOldX, nOldY;
	static short						nZDelta;
	// For picking decoobjs
	static CDecoObj*					pDecoObj;
	static list<CDecoObj*>::iterator	iterDeco;
	static float						fDist;
	// For picking axes
	static int							nPreSelAxis = 0;
	// For moving or rotating decoobjs
	static BOOL							bRotatingOrMoving = FALSE;
	static float						fdX, fdY, fdZ;
	
	if( m_szWorkingFilename )
	{
		switch( uMsg )
		{
		case WM_MOUSEMOVE:
			// Mouse client coordinate
			m_nMouseX = LOWORD( lParam );
			m_nMouseY = HIWORD( lParam );
			
			//{ulyssesme-20060928
			if (wParam & MK_LBUTTON && m_nWorkingState == OBJECT_EDIT && m_nObjEditType == 0 && g_pObjTree)	//드래그중이면서 오브젝트 선택모드일 경우
			{
				m_bDrag = TRUE;
				m_nMouseDragEndX = m_nMouseX;
				m_nMouseDragEndZ = m_nMouseY;

				DragMouse();
				break;
			}
			//}ulyssesme-20060928

			// Get a picking ray
			m_pCamera->GetPickingRay( m_vPickOrigin, m_vPickDirection, m_nMouseX, m_nMouseY );

			if( m_nWorkingState == TEXTURE_PAINT )								// Texture painting
			{
				// Get a picked point
				if( m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint ) )
				{
					// Make a brush mesh
					MakeBrushMesh();

					// Set a texture of tiles
					if( ( wParam & MK_LBUTTON ) && m_nCurTileTexture >= 0 )
					{
						DrawTileTexture();
						m_bModify[0] = TRUE;
					}
				}
			}
			else if( m_nWorkingState == VERTEX_EDIT )							// Vertex editing
			{
				// Get a picked point
				if( m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint ) )
				{				
					// Modify heights of a terrain
					if( wParam & MK_LBUTTON )
					{
						if( m_nControlUnit == 1 && m_nBrushSize == 1 && m_nPickVertexX != -1 )
							ModifyOneVertex( nOldY - m_nMouseY );
						else
							ModifyHeights();

						m_bModify[0] = TRUE;
					}

					// Make a brush mesh
					if( m_nControlUnit == 1 && m_nBrushSize == 1 && m_nPickVertexX != -1 )
						m_dwBrushVertNum = 0;
					else
						MakeBrushMesh();
				}
			}
			else if( m_nWorkingState == OBJECT_EDIT && m_nObjEditType > 1 )		// Object editing ( move, rotate mode )
			{
				if( ( wParam & MK_MBUTTON ) && m_nSelAxis > 0 )
				{
					if( m_nObjEditType == 2 )	// move
					{
						fdX = fdY = fdZ = 0.0f;						

						if( m_pCamera->GetLocation().y > m_vGizmoPosition.y )
						{
							switch( m_nSelAxis )
							{
							case 1:		// x
								fdX = ( m_nMouseX - nOldX ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( nOldY - m_nMouseY ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							case 2:		// y
								fdY = nOldY - m_nMouseY; 
								break;
							case 3:		// z
								fdZ = ( nOldX - m_nMouseX ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( nOldY - m_nMouseY ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							case 4:		// xz
								fdX = ( m_nMouseX - nOldX ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( nOldY - m_nMouseY ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
								fdZ = ( nOldX - m_nMouseX ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( nOldY - m_nMouseY ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							}
						}
						else
						{
							switch( m_nSelAxis )
							{
							case 1:		// x
								fdX = ( m_nMouseX - nOldX ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( m_nMouseY - nOldY ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							case 2:		// y
								fdY = nOldY - m_nMouseY; 
								break;
							case 3:		// z
								fdZ = ( nOldX - m_nMouseX ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( m_nMouseY - nOldY) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							case 4:		// xz
								fdX = ( m_nMouseX - nOldX ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( m_nMouseY - nOldY ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
								fdZ = ( nOldX - m_nMouseX ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( m_nMouseY - nOldY ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							}
						}

						if( wParam & MK_CONTROL )
							MoveSelDecoObjs( fdX * 0.1f, fdY * 0.1f, fdZ * 0.1f );
						else if( wParam & MK_SHIFT )
							MoveSelDecoObjs( fdX, fdY, fdZ );
						else
							MoveSelDecoObjs( fdX * 0.4f, fdY * 0.4f, fdZ * 0.4f );

						bRotatingOrMoving = TRUE;
					}
					else						// rotate
					{
						if( ( wParam & MK_CONTROL ) && ( wParam & MK_SHIFT ) )
						{
							if( m_nMouseX - nOldX > 0 )
								RotateSelDecoObjs( 16.0f, TRUE );
							else
								RotateSelDecoObjs( -16.0f, TRUE );
						}
						else if( wParam & MK_CONTROL )
							RotateSelDecoObjs( ( m_nMouseX - nOldX ) * 0.1f );
						else if( wParam & MK_SHIFT )
							RotateSelDecoObjs( m_nMouseX - nOldX );
						else
							RotateSelDecoObjs( ( m_nMouseX - nOldX ) * 0.4f );

						bRotatingOrMoving = TRUE;
					}
				}
				else if( ( wParam & MK_LBUTTON ) ) // 가운데 버튼으로 오브젝트 회전시 여러개 돌리기(좌표 회전) 06-09-12 원석
				{
					if( m_nObjEditType == 2 ) // 이동 명령.
					{
						fdX = fdY = fdZ = 0.0f;						

						if( m_pCamera->GetLocation().y > m_vGizmoPosition.y )
						{
							switch( m_nSelAxis )
							{
							case 1:		// x
								fdX = ( m_nMouseX - nOldX ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( nOldY - m_nMouseY ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							case 2:		// y
								fdY = nOldY - m_nMouseY; 
								break;
							case 3:		// z
								fdZ = ( nOldX - m_nMouseX ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( nOldY - m_nMouseY ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							case 4:		// xz
								fdX = ( m_nMouseX - nOldX ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( nOldY - m_nMouseY ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
								fdZ = ( nOldX - m_nMouseX ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( nOldY - m_nMouseY ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							}
						}
						else
						{
							switch( m_nSelAxis )
							{
							case 1:		// x
								fdX = ( m_nMouseX - nOldX ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( m_nMouseY - nOldY ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							case 2:		// y
								fdY = nOldY - m_nMouseY; 
								break;
							case 3:		// z
								fdZ = ( nOldX - m_nMouseX ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( m_nMouseY - nOldY) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							case 4:		// xz
								fdX = ( m_nMouseX - nOldX ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( m_nMouseY - nOldY ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
								fdZ = ( nOldX - m_nMouseX ) * sinf( m_pCamera->GetYaw() * NK_DEGTORAD ) +
										( m_nMouseY - nOldY ) * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
								break;
							}
						}

						if( wParam & MK_CONTROL )
							MoveSelDecoObjs( fdX * 0.1f, fdY * 0.1f, fdZ * 0.1f );
						else if( wParam & MK_SHIFT )
							MoveSelDecoObjs( fdX, fdY, fdZ );
						else
							MoveSelDecoObjs( fdX * 0.4f, fdY * 0.4f, fdZ * 0.4f );

						bRotatingOrMoving = TRUE;
					}
					else // 회전
					{
						float RotateSpeed = 0.4f; // 회전속도.
						
						if( ( wParam & MK_CONTROL ) && ( wParam & MK_SHIFT ) ) // 매우 빠름.
							RotateSpeed = 3.0f;
						else if( wParam & MK_CONTROL ) // 느림.
							RotateSpeed = 0.1f;
						else if( wParam & MK_SHIFT ) // 약간빠름.
							RotateSpeed = 1.0f;
						else // 보통
							RotateSpeed = 0.4f;
						
						if( m_nSelAxis > 0 ) // 선택된 축이 있으면 중심을 기준으로 좌표 회전
						{
							RotateSelDecoObjsByPivot( (m_nMouseX-nOldX)*RotateSpeed, D3DVECTOR(m_vGizmoPosition.x,m_vGizmoPosition.y,m_vGizmoPosition.z) );
						}
						else // 없으면 마우스 좌표 기준 회전.
						{
						/*                      MBUTTON_DOWN 시에 픽킹 좌표를 구해서 그 좌표를 기준으로 해야할듯. 귀찮아서 보류.
						if( m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint ) )
						{
						if( DistPlToPlAbs( m_vTarget.x, m_vTarget.z, m_vPickedPoint.x, m_vPickedPoint.z ) < CNKObjQuadNode::GetRenderDist() )
						{
						RotateSelDecoObjsByPivot( (m_nMouseX-nOldX)*RotateSpeed
						, D3DVECTOR(m_vPickedPoint.x, m_pTerrain->GetHeights(m_vPickedPoint.z,m_vPickedPoint.x), m_vPickedPoint.z) );
						}
						}
							*/
						}
					}
				}
				else
				{
					CheckPickingAxes();
					if( nPreSelAxis != m_nSelAxis )
					{
						SetHighlightAxis();
						nPreSelAxis = m_nSelAxis;
					}
				}
			}
			else if( m_nWorkingState == LIGHT_EDIT )
			{
				if( !m_bDirectLightControl )
				{
					// Get a picked point
					if( m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint ) )
					{
						// Make a brush mesh
						MakeBrushMesh();
				
						if( wParam & MK_LBUTTON )		// Draw the shadow
						{
							DrawShadow();
							m_bModify[0] = TRUE;
						}
					}
				}
				else
				{
					m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );	// For showing a coordinate in status bar
					if( ( wParam & MK_LBUTTON ) )
						ChangeLightDirection( wParam & MK_SHIFT, wParam & MK_CONTROL );
				}
			}
			else if( m_nWorkingState == ATMOSPHERE_EDIT )
			{
				// Get a picked point
				GetPickedWaterPlane();

				if( m_nWaterType == 0 )		// Base water
				{
					// Make the water mesh
					MakeWaterMesh();

					if( wParam & MK_LBUTTON )
					{
						if( m_nWaterEditType == 0 )			// Paint the base water
							PaintBaseWater();
						else								// Erase the wave water
							EraseBaseWater();
					}
				}
				else						// Wave water
				{
					if( m_nWaterEditType == 0 )
					{
						if( ( wParam & MK_CONTROL ) && ( wParam & MK_SHIFT ) )		// Modify a vertex of the wave water ( snap )
						{
							m_bWaveWaterModify = TRUE;
							if( ( wParam & MK_LBUTTON ) && m_dwSelWaterVertex != -1 )
								ModifyWaveWaterVertex( TRUE );
						}
						else if( wParam & MK_CONTROL )								// Modify a vertex of the wave water
						{
							m_bWaveWaterModify = TRUE;
							if( ( wParam & MK_LBUTTON ) && m_dwSelWaterVertex != -1 )
								ModifyWaveWaterVertex();
						}
						else if( wParam & MK_SHIFT )								// Rotate vertices of the wave water
						{
							m_bWaveWaterRotate = TRUE;
							if( wParam & MK_LBUTTON ) 
								RotateWaveWaterVertices( m_nMouseX - nOldX );
						}
						else
						{
							m_bWaveWaterModify = FALSE;
							m_bWaveWaterRotate = FALSE;
						}
					}
					// Make the water mesh
					MakeWaterMesh();
				}
			}
			else if( m_nWorkingState == ATTRIBUTE_EDIT )
			{
				// Get a picked point
				m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );

				// Make a brush mesh
				MakeBrushMesh();

				// Draw map attribute
				if( wParam & MK_LBUTTON )
				{
					m_pTerrain->DrawMapAttribute( m_nCurMapAttr, m_nTileSZ, m_nTileSX, m_nBrushSize );
					m_bModify[0] = TRUE;
				}
			}
			else
			{
				// Get a picked point
				m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );
			}
			
			if( wParam & MK_RBUTTON )											// Camera rotation
			{
				m_pCamera->AddPitch( (float)(m_nMouseY - nOldY) * 0.1f );
				m_pCamera->AddYaw( (float)(m_nMouseX - nOldX) * 0.1f );
			}

			nOldX = m_nMouseX;
			nOldY = m_nMouseY;

			break;

		case WM_LBUTTONDOWN:
			nOldX = m_nMouseX;
			nOldY = m_nMouseY;
			m_nMouseDragStartX = m_nMouseX;	//ulyssesme-20060928
			m_nMouseDragStartZ = m_nMouseY;	//ulyssesme-20060928
			m_nMouseDragEndX = m_nMouseX;	//ulyssesme-20060928
			m_nMouseDragEndZ = m_nMouseY;	//ulyssesme-20060928
			if (wParam & MK_CONTROL)	m_bAdd = TRUE;	else	m_bAdd = FALSE;	//ulyssesme-20060929
			if ((GetAsyncKeyState(VK_MENU) & 0x8000) == 0x8000)	m_bDeselect = TRUE;	else	m_bDeselect = FALSE;
			
			if( m_nWorkingState == TEXTURE_PAINT && m_nCurTileTexture >= 0 )	// Texture painting
			{
				if( m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint ) )
				{
					DrawTileTexture();
					m_bModify[0] = TRUE;
				}
			}
			else if( m_nWorkingState == VERTEX_EDIT )							// Vertex editing
			{
				if( m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint ) )
				{
					m_nDirtyRegionSX = INT_MAX;		m_nDirtyRegionEX = INT_MIN;
					m_nDirtyRegionSZ = INT_MAX;		m_nDirtyRegionEZ = INT_MIN;

					if( m_nControlUnit == 1 && m_nBrushSize == 1 && m_nPickVertexX < 0 )
						PickVertex();
					else
					{
						ModifyHeights();
						m_bModify[0] = TRUE;
					}

					// Make a brush mesh
					MakeBrushMesh();
				}
			}
			else if( m_nWorkingState == OBJECT_EDIT )							// Object editing
			{
				if( m_nObjEditType == 1 )			// Place mode
				{
					if( m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint ) )
					{
						if( DistPlToPlAbs( m_vTarget.x, m_vTarget.z, m_vPickedPoint.x, m_vPickedPoint.z ) <
							CNKObjQuadNode::GetRenderDist() )
						{
							PlaceDecoObj();
							m_bModify[2] = TRUE;
						}
					}
				}
				else								// Select, move, rotate mode
				{
					m_UndoObj.OnSelObjListBackup(); // [2007/10/29 theodoric] undo

					if( m_nObjEditType != 0 )			// Move, rotate mode
					{
						CheckPickingAxes();					// Check a picked axis
						if( nPreSelAxis != m_nSelAxis )
						{
							SetHighlightAxis();
							nPreSelAxis = m_nSelAxis;
						}
						if( m_nSelAxis > 0 )
							break;
					}

					/*	//ulyssesme-20060929 WM_LBUTTONUP으로 옮김
					fDist = FLT_MAX;
					pDecoObj = NULL;
					g_pObjTree->GetPickedDecoObj( m_vPickOrigin, m_vPickDirection, fDist, &pDecoObj );

					if( pDecoObj == NULL )		// If a picked decoobj is not existent
					{
						if( !( wParam & MK_CONTROL) )
						{
							m_lstSelDecoObjs.erase( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end() );
							MakeDecoBoxMesh();
						}
					}
					else
					{
						if( wParam & MK_CONTROL )		// Multiple selection
						{
							iterDeco = find( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end(), pDecoObj );
							if( *iterDeco == pDecoObj )
							{
								m_lstSelDecoObjs.erase( iterDeco );
								UpdateObjEditParameters();
							}
							else
							{
								if( m_lstSelDecoObjs.size() < 500 )		// Maximum count of the selectoin is 500
								{
									m_lstSelDecoObjs.push_back( pDecoObj );
									UpdateObjEditParameters( pDecoObj );
								}
							}

							if( m_lstSelDecoObjs.size() != 1 )
								UpdateObjEditParameters();
						}
						else							// Single selection
						{
							m_lstSelDecoObjs.erase( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end() );
							m_lstSelDecoObjs.push_back( pDecoObj );
							UpdateObjEditParameters( pDecoObj );
						}

						MakeNodeBoundMesh();
						MakeDecoBoxMesh();
						CalcGizmoPosition();
					}
					*/
				}
			}
			else if( m_nWorkingState == LIGHT_EDIT )
			{
				if( m_bDirectLightControl )			// Set a direction of the directional light
				{
					m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );	// For showing a coordinate in status bar
					ChangeLightDirection( wParam & MK_SHIFT, wParam & MK_CONTROL );
				}
				else								// Draw the shadow
				{
					if( m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint ) )
					{
						DrawShadow();
						m_bModify[0] = TRUE;
					}
				}
			}
			else if( m_nWorkingState == ATMOSPHERE_EDIT )
			{
				// Get a picked point
				GetPickedWaterPlane();

				if( m_nWaterType == 0 )			// Base water
				{
					// Make the water mesh
					MakeWaterMesh();

					// Paint a base water
					if( m_nWaterEditType == 0 )					// Paint the base water	
						PaintBaseWater();
					else										// Erase the base water
						EraseBaseWater();
				}
				else							// Wave water
				{
					if( m_nWaterEditType == 0 )
					{
						if( ( wParam & MK_CONTROL ) && ( wParam & MK_SHIFT ) )	// Modify a vertex of the wave water ( snap )
						{
							GetPickedWaveWaterVertex();
							m_bWaveWaterModify = TRUE;
						}
						if( wParam & MK_CONTROL )								// Modify a vertex of the wave water
						{
							GetPickedWaveWaterVertex();
							m_bWaveWaterModify = TRUE;
						}
						else if( wParam & MK_SHIFT )							// Rotate vertices of the wave water
							m_bWaveWaterRotate = TRUE;
						else													// Paint the wave water
						{
							m_bWaveWaterModify = FALSE;
							m_bWaveWaterRotate = FALSE;

							PaintWaveWater();
						}						
					}
					else														// Erase the wave water
						EraseWaveWater();

					// Make the water mesh
					MakeWaterMesh();
				}
			}
			else if( m_nWorkingState == ATTRIBUTE_EDIT )
			{
				// Get a picked point
				m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );

				// Make a brush mesh
				MakeBrushMesh();

				// Draw map attribute
				m_pTerrain->DrawMapAttribute( m_nCurMapAttr, m_nTileSZ, m_nTileSX, m_nBrushSize );

				m_bModify[0] = TRUE;
			}
			else
			{
				// Get a picked point
				m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );
			}
			break;

		case WM_LBUTTONUP:
			if( m_nWorkingState == VERTEX_EDIT )								// Vertex editing
			{
				// Make a brush mesh
				if( m_nControlUnit == 1 && m_nBrushSize == 1 )
					MakeBrushMesh();

				m_pTerrain->UpdateRGBMapsByVertex( m_nDirtyRegionSZ, m_nDirtyRegionSX,
													m_nDirtyRegionEZ, m_nDirtyRegionEX );
				
				m_nDirtyRegionSX = INT_MAX;		m_nDirtyRegionEX = INT_MIN;
				m_nDirtyRegionSZ = INT_MAX;		m_nDirtyRegionEZ = INT_MIN;

				m_nPickVertexX = -1;
				m_nPickVertexZ = -1;
				ResetTerrainMinMaxHeights();
			}
			else if( m_nWorkingState == OBJECT_EDIT )							// Object editing
			{
				m_UndoObj.OnSelObjListUndoSet();

				if( m_nObjEditType >= 2 && bRotatingOrMoving )		// Move, rotate mode
				{
					
					UpdateChangedDecoObjs();
					
					bRotatingOrMoving = FALSE;
					m_bModify[2] = TRUE;
				}
				//{ulyssesme-20060929 WM_LBUTTONDOWN에서 옮겨옴
				else if (m_nObjEditType == 0)	//select
				{
					if (!m_bDrag)	//드래그중이었을 경우엔 무시
					{
						fDist = FLT_MAX;
						pDecoObj = NULL;
						g_pObjTree->GetPickedDecoObj( m_vPickOrigin, m_vPickDirection, fDist, &pDecoObj );
						
						if( pDecoObj == NULL )		// If a picked decoobj is not existent
						{
							if( !( wParam & MK_CONTROL) )
							{
								m_lstSelDecoObjs.erase( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end() );
								MakeDecoBoxMesh();
							}
						}
						else
						{
							if( wParam & MK_CONTROL )		// Multiple selection
							{
								iterDeco = find( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end(), pDecoObj );
								if( *iterDeco == pDecoObj )
								{
									m_lstSelDecoObjs.erase( iterDeco );
									UpdateObjEditParameters();
								}
								else
								{
									if( m_lstSelDecoObjs.size() < 10000 )		// Maximum count of the selectoin is 500
									{
										m_lstSelDecoObjs.push_back( pDecoObj );
										UpdateObjEditParameters( pDecoObj );
									}
								}
								
								if( m_lstSelDecoObjs.size() != 1 )
									UpdateObjEditParameters();
							}
							else							// Single selection
							{
								m_lstSelDecoObjs.erase( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end() );
								m_lstSelDecoObjs.push_back( pDecoObj );
								UpdateObjEditParameters( pDecoObj );
							}
							
							MakeNodeBoundMesh();
							MakeDecoBoxMesh();
							CalcGizmoPosition();
						}
					}
					else
					{
						if (m_lstDragSelDecoObjs.size() > 0)
							UpdateObjEditParameters(*m_lstDragSelDecoObjs.rbegin());
					}
					m_lstDragSelDecoObjs.clear();
				}
				//}ulyssesme-20060929 WM_LBUTTONDOWN에서 옮겨옴
			}
			else if( m_nWorkingState == LIGHT_EDIT && m_bDirectLightControl )	// Light editing
				ChangeLightDirection( wParam & MK_SHIFT, wParam & MK_CONTROL, TRUE );
			m_bDrag = FALSE;
			break;

		case WM_RBUTTONDOWN:
			nOldX = LOWORD( lParam );
			nOldY = HIWORD( lParam );
			m_nMouseX = LOWORD( lParam );
			m_nMouseY = HIWORD( lParam );

			if( m_nWorkingState == ATMOSPHERE_EDIT )
			{
				if( m_nWaterType == 1 && m_nWaterEditType == 0 )
				{
					if( wParam & MK_CONTROL )
					{
						GetPickedWaveWaterFromNode();

						// Make the water mesh
						MakeWaterMesh();
					}
				}
			}
			break;
		
		case WM_MOUSEWHEEL:
			nZDelta = HIWORD( wParam );											// Camera zoom in/out

			// 휠로 브러쉬 크기 조절 기능 추가 (by 원석)
			if( LOWORD( wParam ) & MK_MBUTTON ) // 휠키를 누르고 돌리면 브러쉬 크기 조절.
			{
				CMainFrame*	pFrame = NULL;
				pFrame = (CMainFrame*)AfxGetMainWnd();

				CToolAttrEditPage *RefAttrEdit = NULL;

				// 현재는 Att 편집시만 지원.
				if( m_nWorkingState == ATTRIBUTE_EDIT )
				{
					if( pFrame && pFrame->m_wndToolCtrlBar )
					{
						RefAttrEdit = &(pFrame->m_wndToolCtrlBar.m_pageAttrEdit);
					}
					
					if( RefAttrEdit )
					{
						if( nZDelta < 0 ) // 아래로 당길때
						{
							RefAttrEdit->UpdateBrushSizeSlider( +1 );
						}
						else if( nZDelta > 0 ) // 위로 밀때
						{
							RefAttrEdit->UpdateBrushSizeSlider( -1 );
						}
					}
				}
			}
			else if( m_nCameraType == 0 )
			{
				if( LOWORD( wParam ) & MK_SHIFT )
					m_pCamera->AddDistance( -nZDelta / WHEEL_DELTA * m_pTerrain->GetPatchSize() * 1.5f );
				else if( LOWORD( wParam ) & MK_CONTROL )
					m_pCamera->AddDistance( -nZDelta / WHEEL_DELTA * m_pTerrain->GetPatchSize() * 0.05f );
				else
					m_pCamera->AddDistance( -nZDelta / WHEEL_DELTA * m_pTerrain->GetPatchSize() * 0.2f );
			}
			break;

		case WM_CHAR:
			switch( (int)wParam )
			{
			case 'a':
			case 'A':
				ToggleShowAttr_Map();
				break;

			case 'b':
			case 'B':
				m_bApplyObjBlending = !m_bApplyObjBlending;
				break;

			case 'g':
			case 'G':
				ToggleGrid();
				break;

			case 'c':
			case 'C':
				SetCameraType( m_nCameraType + 1 > 1 ? 0 : m_nCameraType + 1 );
				break;

			case 't':
			case 'T':
				ToggleShowTerrain();
				break;

			case 'o':
			case 'O':
				ToggleShowObject();
				break;

			case 'w': // 현재 w키는 그래픽팀의 요청으로 인해 다른 단축키로 쓰이고있다.(버튼 단축키)
			case 'W': // 그래서 현재는 이쪽 메시지로 들어오질 않는다. (현재 여기 정의되어있는 단축키중 w키만..)
				ToggleShowWater();
				break;

			case 'f':
			case 'F':
				ToggleShowFog();
				break;

			case 'k':
			case 'K':
				ToggleShowCharacter();
				break;

			case 's':
			case 'S':
				ToggleShowSky();
				break;

			case '-':
				if( m_nWorkingState != LIGHT_EDIT )
				{
					m_bStopWorldTime = TRUE;
					ChangeWorldTime( m_nWorldHour - 1 );
				}
				break;

			case '=':
				if( m_nWorkingState != LIGHT_EDIT )
				{
					m_bStopWorldTime = TRUE;
					ChangeWorldTime( m_nWorldHour + 1 );
				}
				break;

			case '0':
				if( m_nWorkingState != LIGHT_EDIT )
					m_bStopWorldTime = FALSE;
				break;

			// [10/26/2007 Theodoric]  Undo
			case 'z':
			case 'Z':
					m_UndoObj.OnUndo();
				break;


			case 'x':
			case 'X':
				ToggleCameraTopView();
			}
			break;

		case WM_KEYDOWN:
			switch( (int)wParam )
			{
			case VK_DELETE:
				if( m_nWorkingState == OBJECT_EDIT )
				{
					RemoveDecoObjsFromTree();
					UpdateObjEditReferenceCount();
				}
				break;
			}
			break;
		}
	}

	return CD3DApplication::MsgProc( uMsg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Name : SetWorkingFilename()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetWorkingFilename( const TCHAR* szFilename )
{
	SAFE_DELETE_ARRAY( m_szWorkingFilename );

	m_szWorkingFilename = new TCHAR[_tcslen( szFilename ) + 1];
	_tcscpy( m_szWorkingFilename, szFilename );
	m_szWorkingFilename[_tcslen( szFilename )] = NULL;
}


//-----------------------------------------------------------------------------
// Name : SetWorkingPath()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetWorkingPath( const TCHAR* szPath )
{
	SAFE_DELETE_ARRAY( m_szWorkingPath );

	m_szWorkingPath = new TCHAR[_tcslen( szPath ) + 1];
	_tcscpy( m_szWorkingPath, szPath );
	m_szWorkingPath[_tcslen( szPath )] = NULL;
}


//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyNKApp::OneTimeSceneInit()
{
	srand( (unsigned int)timeGetTime() );

	g_pCapsyongTexture = new IndexedTexture( 10 );
	m_pTestTexture = new IndexedTexture( 10 );
	m_pTestTexture->SetTexturePath( "data/testtex/" );
	m_pDecoTexture = new IndexedTexture( 350 );
	m_pDecoTexture->SetTexturePath( "data/building/textr/" );
	
	m_pCamera = new CNKCamera;
	m_pCamera->SetTarget( m_vTarget );
	
	m_pTerrain = new CNKTerrain();

	m_pShadowRect = new CShadowRect;
	m_pShadowRect->Init( m_pTerrain, m_pTestTexture );
	
	PlayerInitialLoading();
	m_pRoh = new CNkCharacter( m_pShadowRect );
	m_pRoh->m_dir = 0.0f;
	m_pRoh->SetBiped( g_pBipedList[0] );
	m_pRoh->SetCapsyong( g_pBodyList[0], 0 );
	m_pLightCtrl = new CLightCtrl;

	InitFogParameters();
	InitAxes();
	InitGizmo();

	D3DUtil_SetIdentityMatrix( m_matWaterTrans );

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyNKApp::InitDeviceObjects()
{
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
		
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x00888888 );

	m_pCamera->UpdateProjMatrix( NK_QUATERPI, m_dwClientWidth, m_dwClientHeight, 30.0f, 10000.0f, 30.0f, 100000.0f ); ///071102 플러스텀 far 값 조절.(해당 값은 버택스 버퍼 메모리 할당에 영향을 미치기 때문에 umlimit ㄹ 위하여 미리 늘려둔다.)
	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION,
								(LPD3DMATRIX)( &(m_pCamera->GetProjMatrix()) ) );

	if( m_pTerrain )
	{
		if( FAILED( m_pTerrain->RestoreDeviceObjects( m_pD3D, m_pd3dDevice ) ) )
			return E_FAIL;
	}

	if( FAILED( CNKObjQuadNode::RestoreDeviceObjects( m_pD3D, m_pd3dDevice ) ) )
		return E_FAIL;

	if( m_pSkyPlane )
		m_pSkyPlane->SetDevice( m_pd3dDevice );

	if( g_pCapsyongTexture )
	{
		if( FAILED( g_pCapsyongTexture->RestoreAllTextures( m_pd3dDevice ) ) )
			return E_FAIL;
	}

	if( m_pTestTexture )
	{
		if( FAILED( m_pTestTexture->RestoreAllTextures( m_pd3dDevice ) ) )
			return E_FAIL;
	}

	if( m_pDecoTexture )
	{
		if( FAILED( m_pDecoTexture->RestoreAllTextures( m_pd3dDevice ) ) )
			return E_FAIL;
	}

	// Set a material
	D3DMATERIAL7	mtrl;
    D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
    m_pd3dDevice->SetMaterial( &mtrl );

	// Intialize the Direct3D lights
	for( int i = 0; i < 8; i++ )
		m_pd3dDevice->LightEnable( i, FALSE );

	m_pLightCtrl->LightEnable( m_pd3dDevice, NKLIGHT_DIRECTIONAL, TRUE );
	m_pLightCtrl->InitLight( NKLIGHT_DIRECTIONAL, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f );
	m_pLightCtrl->SetLight( m_pd3dDevice, NKLIGHT_DIRECTIONAL );
	m_pLightCtrl->LightEnable( m_pd3dDevice, NKLIGHT_POINT, TRUE );
	m_pLightCtrl->LightingEnalbe( m_pd3dDevice, TRUE );
	m_pLightCtrl->SetAmbientLight( m_pd3dDevice, 0x008f96b3 );

	ChangeWorldTime( m_nWorldHour );

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyNKApp::FrameMove()
{
	if( m_szWorkingFilename )
	{
		// World move
		float	fWaterHeight, fHeight;
		float	fDist = m_pTerrain->GetPatchSize() * 5.0f * m_fElapsedTime;
		if( ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) == 0x8000 )
			fDist *= 0.25f; //0.4f // [2007/11/12 theodoric] 게임상에서 캐긹터가 움직이는 속도 // 맞을진 나도 모른다. ㅜㅜ
		else if( ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) == 0x8000 )
			fDist *= 2.5f;

		fDist *= m_fCameraSpeed; // 카메라 스피드 조절 기능 확장(by 원석)

		if( ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ) == 0x8000 )
		{			
			m_vTarget.x -= fDist * sinf( ( m_pCamera->GetYaw() - 90 ) * NK_DEGTORAD );
			m_vTarget.z -= fDist * cosf( ( m_pCamera->GetYaw() - 90 ) * NK_DEGTORAD );
			m_pRoh->m_wx = m_vTarget.x;
			m_pRoh->m_wz = m_vTarget.z;
			m_vTarget.y = CNKObjQuadNode::GetHeightOnObject( m_pRoh->m_wz, m_pRoh->m_wx, m_pRoh->m_wy );
			fHeight = m_pTerrain->GetHeights( m_vTarget.z, m_vTarget.x );
			if( m_vTarget.y < fHeight )
				m_vTarget.y = fHeight;
			m_pRoh->m_wy = m_vTarget.y;			
			m_pRoh->m_dir = -m_pCamera->GetYaw() * NK_DEGTORAD;
			
			if( m_bShowCharacter && m_bShowWater )
			{
				fWaterHeight = CNKObjQuadNode::GetHeightOnWater( m_pRoh->m_wz, m_pRoh->m_wx );
				if( m_pRoh->m_wy < fWaterHeight - 11.0f )
				{
					m_pRoh->m_wy = fWaterHeight;
					//m_pRoh->SetMotionState( CHA_SWIM_STAND );
				}
				else
					m_pRoh->SetMotionState( CHA_STAND );

				m_vTarget.y = m_pRoh->m_wy + 15.0f;
			}
			m_pCamera->SetTarget( m_vTarget );
			if (m_bDrag) DragMouse();
		}
		else if( ( GetAsyncKeyState( VK_LEFT ) & 0x8000 ) == 0x8000 )
		{
			m_vTarget.x += fDist * sinf( ( m_pCamera->GetYaw() - 90 ) * NK_DEGTORAD );
			m_vTarget.z += fDist * cosf( ( m_pCamera->GetYaw() - 90 ) * NK_DEGTORAD );
			m_pRoh->m_wx = m_vTarget.x;			
			m_pRoh->m_wz = m_vTarget.z;
			m_vTarget.y = CNKObjQuadNode::GetHeightOnObject( m_pRoh->m_wz, m_pRoh->m_wx, m_pRoh->m_wy );
			fHeight = m_pTerrain->GetHeights( m_vTarget.z, m_vTarget.x );
			if( m_vTarget.y < fHeight )
				m_vTarget.y = fHeight;
			m_pRoh->m_wy = m_vTarget.y;
			m_pRoh->m_dir = -( m_pCamera->GetYaw() + 180.0f ) * NK_DEGTORAD;
			if( m_bShowCharacter && m_bShowWater )
			{
				fWaterHeight = CNKObjQuadNode::GetHeightOnWater( m_pRoh->m_wz, m_pRoh->m_wx );
				if( m_pRoh->m_wy < fWaterHeight - 11.0f )
				{
					m_pRoh->m_wy = fWaterHeight;
					//m_pRoh->SetMotionState( CHA_SWIM_STAND );
				}
				else
					m_pRoh->SetMotionState( CHA_STAND );

				m_vTarget.y = m_pRoh->m_wy + 15.0f;
			}
			m_pCamera->SetTarget( m_vTarget );
			if (m_bDrag) DragMouse();
		}

		if( ( GetAsyncKeyState( VK_UP ) & 0x8000 ) == 0x8000 )
		{
			m_vTarget.x += fDist * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
			m_vTarget.z += fDist * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
			m_pRoh->m_wx = m_vTarget.x;			
			m_pRoh->m_wz = m_vTarget.z;
			m_vTarget.y = CNKObjQuadNode::GetHeightOnObject( m_pRoh->m_wz, m_pRoh->m_wx, m_pRoh->m_wy );
			fHeight = m_pTerrain->GetHeights( m_vTarget.z, m_vTarget.x );
			if( m_vTarget.y < fHeight )
				m_vTarget.y = fHeight;
			m_pRoh->m_wy = m_vTarget.y;
			m_pRoh->m_dir = -( m_pCamera->GetYaw() - 90.0f ) * NK_DEGTORAD;
			if( m_bShowCharacter && m_bShowWater )
			{
				fWaterHeight = CNKObjQuadNode::GetHeightOnWater( m_pRoh->m_wz, m_pRoh->m_wx );
				if( m_pRoh->m_wy < fWaterHeight - 11.0f )
				{
					m_pRoh->m_wy = fWaterHeight;
					//m_pRoh->SetMotionState( CHA_SWIM_STAND );
				}
				else
					m_pRoh->SetMotionState( CHA_STAND );

				m_vTarget.y = m_pRoh->m_wy + 15.0f;
			}
			m_pCamera->SetTarget( m_vTarget );
			if (m_bDrag) DragMouse();
		}
		else if( ( GetAsyncKeyState( VK_DOWN ) & 0x8000 ) == 0x8000 )
		{
			m_vTarget.x -= fDist * sinf( m_pCamera->GetYaw() * NK_DEGTORAD );
			m_vTarget.z -= fDist * cosf( m_pCamera->GetYaw() * NK_DEGTORAD );
			m_pRoh->m_wx = m_vTarget.x;			
			m_pRoh->m_wz = m_vTarget.z;
			m_vTarget.y = CNKObjQuadNode::GetHeightOnObject( m_pRoh->m_wz, m_pRoh->m_wx, m_pRoh->m_wy );
			fHeight = m_pTerrain->GetHeights( m_vTarget.z, m_vTarget.x );
			if( m_vTarget.y < fHeight )
				m_vTarget.y = fHeight;
			m_pRoh->m_wy = m_vTarget.y;
			m_pRoh->m_dir = -( m_pCamera->GetYaw() + 90.0f ) * NK_DEGTORAD;
			if( m_bShowCharacter && m_bShowWater )
			{
				fWaterHeight = CNKObjQuadNode::GetHeightOnWater( m_pRoh->m_wz, m_pRoh->m_wx );
				if( m_pRoh->m_wy < fWaterHeight - 11.0f )
				{
					m_pRoh->m_wy = fWaterHeight;
					//m_pRoh->SetMotionState( CHA_SWIM_STAND );
				}
				else
					m_pRoh->SetMotionState( CHA_STAND );

				m_vTarget.y = m_pRoh->m_wy + 15.0f;
			}
			m_pCamera->SetTarget( m_vTarget );
			if (m_bDrag) DragMouse();
		}


		if( m_nCameraType == 0 )
			m_pCamera->UpdateViewMatrix();
		else
		{
			if( ( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ) == 0x8000 )
				m_pCamera->AddMove( fDist, 0.0f, 0.0f );
			else if( ( GetAsyncKeyState( VK_LEFT ) & 0x8000 ) == 0x8000 )
				m_pCamera->AddMove( -fDist, 0.0f, 0.0f );

			if( ( GetAsyncKeyState( VK_UP ) & 0x8000 ) == 0x8000 )
				m_pCamera->AddMove( 0.0f, 0.0f, fDist );
			else if( ( GetAsyncKeyState( VK_DOWN ) & 0x8000 ) == 0x8000 )
				m_pCamera->AddMove( 0.0f, 0.0f, -fDist );

			m_pCamera->UpdateFlyViewMatrix();
		}

		// Set view transform
		m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_VIEW,
									(LPD3DMATRIX)( &(m_pCamera->GetViewMatrix()) ) );

		// World time
		if( !m_bStopWorldTime )
		{
			m_fAccumulatedTime += m_fElapsedTime;
			if( m_fAccumulatedTime > 6.0f )
			{
				ChangeWorldTime( m_nWorldHour + 1 );
				m_fAccumulatedTime -= 6.0f;
			}
		}
	}

	// Update status bar
	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
	CString strStatus;

	strStatus.Format( _T("FPS : %4d"), (int)m_fFPS );
	pFrame->GetStatusBar()->SetPaneText( 1, strStatus );

	strStatus.Format( _T("World Time : %2dh"), m_nWorldHour );
	pFrame->GetStatusBar()->SetPaneText( 2, strStatus );

	strStatus.Format( _T("Coord : (%3d,%3d)[Tile], (%5.3f/%5.3f/%5.3f)[World]"),
							(int)(m_vPickedPoint.x / m_pTerrain->GetPatchSize()),
							m_pTerrain->GetHMWidth() - (int)(m_vPickedPoint.z / m_pTerrain->GetPatchSize()) - 2,
							m_vPickedPoint.x, m_vPickedPoint.y, m_vPickedPoint.z );
	pFrame->GetStatusBar()->SetPaneText( 3, strStatus );

	strStatus.Format( _T("Tris : %5d[Ter], %5d[Obj]"), m_pTerrain->GetTriangleCount(), g_dwObjTriangles );
	pFrame->GetStatusBar()->SetPaneText( 4, strStatus );

	strStatus.Format( _T("%4dx%d"), m_dwClientWidth, m_dwClientHeight );
	pFrame->GetStatusBar()->SetPaneText( 5, strStatus );

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: App_Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CMyNKApp::Render()
{
	if( !m_szWorkingFilename ) // 맵을 로드하기전 상태에서 렌더에 들어오면 지포스 계열에선 아래의 클리어 함수에서 무지 느려지는 문제가 있어서 맵 로드전이면 아래 스킵하도록 수정.(아마도 ZBUFFER 초기화 문제 등이 아닐까 싶다.)
		return E_FAIL;


    // Clear the viewport
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_dwBgColor, 1.0f, 0L );

    // Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
        // Draw all items in the scene
		if( m_szWorkingFilename )
		{
			// Turn on lighting
			float	dx, dz;
			m_pLightCtrl->LightingEnalbe( m_pd3dDevice, TRUE );
			
			if( m_pLightCtrl->IsEnabled( m_pd3dDevice, NKLIGHT_POINT ) )
			{
				GetDirectionElement( m_pRoh->m_dir, dx, dz );
				m_pLightCtrl->InitLight( NKLIGHT_POINT, D3DLIGHT_POINT,
								m_vTarget.x + dx*5.0f, m_pRoh->m_wy + 30.0f, m_vTarget.z + dz*5.0f,
								m_fPointRColor, m_fPointGColor, m_fPointBColor );
				m_pLightCtrl->SetLight( m_pd3dDevice, NKLIGHT_POINT );
				m_pLightCtrl->LightEnable( m_pd3dDevice, NKLIGHT_POINT, TRUE );
			}

			// Render sky
			if( m_bShowSky )
				m_pSkyPlane->Render( m_pRoh->m_wx, 1800.0f, m_pRoh->m_wz, TRUE );

			// Set a fog
			SetFogParameters( TRUE );

			// DecoObjs ////////////////////////////////////////
			g_dwObjTriangles = 0;
			// Turn on lighting
			m_pLightCtrl->LightingEnalbe( m_pd3dDevice, TRUE );
			// Render DecoObjs
			RenderDecoObjs();

			// Terrain /////////////////////////////////////////			
			// Set a world transform matrix
			m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, (D3DMATRIX*)(&NKMAT_IDENTITY) );
			// Turn off lighting
			m_pLightCtrl->LightingEnalbe( m_pd3dDevice, FALSE );
			// Render a terrian
			if( m_bShowTerrain )
				m_pTerrain->Render();
			// Render a terrain grid
			if( m_nGridSize >= 0 )
				m_pTerrain->RenderGrid( m_nGridSize, m_dwGridColor );
			// Render a attribute map of a terrain
			if( m_nWorkingState == ATTRIBUTE_EDIT || m_bShowAttr_Map )	// nate 2006-01-12 : 속성맵 보이기
				m_pTerrain->RenderAttributeMap( m_nAttMapVisibleRange, m_pRoh->m_wz, m_pRoh->m_wx );

			// Character ///////////////////////////////////////
			// Turn on lighting
			m_pLightCtrl->LightingEnalbe( m_pd3dDevice, TRUE );
			// Render a character
			if( m_bShowCharacter && m_nCameraType == 0 )
				m_pRoh->Render( m_pd3dDevice, NULL );

			// DecoObjs Alphablended & Water ///////////////////
			// Render alpha decoobjs
			if( m_bShowObject )
				RenderAlphaObjs();

			m_pLightCtrl->LightingEnalbe( m_pd3dDevice, FALSE );
			if( m_bShowWater )
				CNKObjQuadNode::RenderWater();

			SetFogParameters( FALSE );

			// Interfaces //////////////////////////////////////
			// Set a world transform matrix
			m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, (D3DMATRIX*)(&NKMAT_IDENTITY) );
			// Turn the lighting
			m_pLightCtrl->LightingEnalbe( m_pd3dDevice, FALSE );
			// Render interfaces
			RenderInterfaces();
		}

        // End the scene.
        m_pd3dDevice->EndScene();
    }
	if (m_bDrag)	//ulyssesme-20061122 드래그중일경우 박스 표시
	{
		HBRUSH hBrush = CreateSolidBrush(RGB(255,0,0));
		HDC hdc;
		RECT rect = {min(m_nMouseDragStartX, m_nMouseDragEndX), min(m_nMouseDragStartZ, m_nMouseDragEndZ),
			max(m_nMouseDragStartX, m_nMouseDragEndX), max(m_nMouseDragStartZ, m_nMouseDragEndZ)};
		m_pddsRenderTarget->GetDC(&hdc);
		
		/*extern D3DXVECTOR3					ptCross;
		D3DVIEWPORT7 vp;
		m_pd3dDevice->GetViewport ( &vp );
		D3DXMATRIX matCombine = m_pCamera->GetViewMatrix() * m_pCamera->GetProjMatrix();
		D3DXVec3TransformCoord(&ptCross, &ptCross, &matCombine);
		ptCross.x += 1.0f;
		ptCross.y += 1.0f;
		
		ptCross.x = ( vp.dwWidth * (ptCross.x) ) / 2.0f + vp.dwX;
		ptCross.y = ( vp.dwHeight * (2.0f - ptCross.y) ) / 2.0f + vp.dwY;
		
		POINT pt;
		
		pt.x = (long)ptCross.x;
		pt.y = (long)ptCross.y; 

		MoveToEx(hdc, pt.x - 10, pt.y, NULL);
		LineTo(hdc, pt.x + 10, pt.y);
		MoveToEx(hdc, pt.x - 10, pt.y - 10, NULL);
		LineTo(hdc, pt.x + 10, pt.y + 10);
		MoveToEx(hdc, pt.x, pt.y - 10, NULL);
		LineTo(hdc, pt.x, pt.y + 10);
		MoveToEx(hdc, pt.x + 10, pt.y - 10, NULL);
		LineTo(hdc, pt.x - 10, pt.y + 10);*/
		
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		FrameRect(hdc, &rect, hBrush);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
		/*//디버깅정보
		CString strMsg;
		strMsg.Format("m_lstDragSelDecoObjs:%d, m_lstSelDecoObjs:%d", m_lstDragSelDecoObjs.size(), m_lstSelDecoObjs.size());
		TextOut(hdc, 0, 70, strMsg, strMsg.GetLength());
		strMsg.Format("m_vDrag1:%.1f,%.1f m_vDrag2:%.1f,%.1f m_vDrag3:%.1f,%.1f m_vDrag4:%.1f,%.1f",
			m_vMouseDrag1.x, m_vMouseDrag1.z, m_vMouseDrag2.x, m_vMouseDrag2.z, m_vMouseDrag3.x, m_vMouseDrag3.z, m_vMouseDrag4.x, m_vMouseDrag4.z);
		TextOut(hdc, 0, 85, strMsg, strMsg.GetLength());
		strMsg.Format("m_bAdd = %d", m_bAdd);
		TextOut(hdc, 0, 100, strMsg, strMsg.GetLength());
		
		LineTo(hdc, m_nMouseDragEndX, m_nMouseDragStartZ);
		LineTo(hdc, m_nMouseDragEndX, m_nMouseDragEndZ);
		LineTo(hdc, m_nMouseDragStartX, m_nMouseDragEndZ);
		LineTo(hdc, m_nMouseDragStartX, m_nMouseDragStartZ);
		*/
		m_pddsRenderTarget->ReleaseDC(hdc);
	}

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exitting, or the device is being changed,
//       this function deletes any device dependant objects.
//-----------------------------------------------------------------------------
HRESULT CMyNKApp::DeleteDeviceObjects()
{
	if( m_pTerrain )
		m_pTerrain->InvalidateDeviceObjects();

	CNKObjQuadNode::InvalidateDeviceObjects();

	if( g_pCapsyongTexture )
		g_pCapsyongTexture->InvalidateAllTextures();

	if( m_pDecoTexture )
		m_pDecoTexture->InvalidateAllTextures();

	if( m_pTestTexture )
		m_pTestTexture->InvalidateAllTextures();

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name : FinalCleanup()
// Desc :
//-----------------------------------------------------------------------------
HRESULT CMyNKApp::FinalCleanup()
{
	DeletePlayerData();
	DeleteDecoObjData();
	ResetSelDecoObjList();
	
	SAFE_DELETE( m_pRoh );
	SAFE_DELETE( m_pDecoTexture );
	SAFE_DELETE( m_pTestTexture );
	SAFE_DELETE( m_pShadowRect );
	SAFE_DELETE( m_pTerrain );
	SAFE_DELETE( m_pSkyPlane );
	SAFE_DELETE( m_pCamera );
	SAFE_DELETE( m_pLightCtrl );
	SAFE_DELETE_ARRAY( m_szWorkingFilename );
	SAFE_DELETE_ARRAY( m_szWorkingPath );

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : SetWorkingState()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetWorkingState( int nWorkingState )
{
	switch( m_nWorkingState )
	{
	case VERTEX_EDIT:
		break;

	case TEXTURE_PAINT:
		if( m_pTerrain && m_nDirtyRegionSX != INT_MAX )
		{
			m_pTerrain->OptimizeLayers( m_nDirtyRegionSZ, m_nDirtyRegionSX,
										m_nDirtyRegionEZ, m_nDirtyRegionEX );
		}		
		m_nDirtyRegionSX = INT_MAX;		m_nDirtyRegionEX = INT_MIN;
		m_nDirtyRegionSZ = INT_MAX;		m_nDirtyRegionEZ = INT_MIN;
		break;

	case OBJECT_EDIT:
		ResetSelDecoObjList();
		break;

	case LIGHT_EDIT:
		break;

	case ATMOSPHERE_EDIT:
		m_dwWaveIndexCount = 0;
		if( g_pObjTree )
			g_pObjTree->EraseInvisibleWaters();
		break;

	case ATTRIBUTE_EDIT:
		break;
	}

	m_nWorkingState = nWorkingState;

	switch( m_nWorkingState )
	{
	case NONE:
		break;

	case VERTEX_EDIT:
		break;

	case TEXTURE_PAINT:
		break;

	case OBJECT_EDIT:
		if( m_nGridSize >= 0 )
			m_nGridSize = 2;
		break;

	case LIGHT_EDIT:
		m_bStopWorldTime = TRUE;
		break;

	case ATMOSPHERE_EDIT:
		break;

	case ATTRIBUTE_EDIT:
		break;
	}	
}


//-----------------------------------------------------------------------------
// Name : SetControlUnit()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetControlUnit( int nControlUnit )
{
	m_nControlUnit = nControlUnit;

	if( m_nControlUnit == 0 )
		m_bCircleBrush = FALSE;
	else
		m_bCircleBrush = TRUE;
}


//-----------------------------------------------------------------------------
// Name : ResetCameraYPosition()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ResetCameraYPosition()
{
	m_vTarget.y = m_pTerrain->GetHeights( m_vTarget.z, m_vTarget.x );
	m_pCamera->SetTarget( m_vTarget );
}


//-----------------------------------------------------------------------------
// Name : NewTerrain()
// Desc :
//-----------------------------------------------------------------------------
BOOL CMyNKApp::NewTerrain( int nWHSegment, int nPatchSize, int nHeightScale, const TCHAR* szHeightMap )
{
	if( IsCameraTopView() )
		ToggleCameraTopView();

	m_bLoadedGameMap = FALSE;

	// Destroy deco textures
	if( m_pDecoTexture )
	{
		m_pDecoTexture->InvalidateAllTextures();
		m_pDecoTexture->DestroyTextureAll();
	}

	// Delete decoobjs
	DeleteDecoObjData();
	ResetSelDecoObjList();

	// Create a new terrain
	ChangeWorldTime( m_nWorldHour );
	if( m_pTerrain->Init( nWHSegment, nPatchSize, nHeightScale, szHeightMap ) == FALSE )
	{
		m_nWorkingState = NONE;
		return FALSE;
	}

	// Load decoobjs
	CString	strDecoPath;
	strDecoPath.Format( _T("data\\building\\%s\\"), m_pTerrain->GetTerrainName() );
	DecoObjInitialLoading( strDecoPath, m_pTerrain->GetTerrainName() );

	// Set ranges that decoobjs can be located
	m_fObjMinXZ = 0.0f;
	m_fObjMaxXZ = ( m_pTerrain->GetHMWidth() - 1 ) * m_pTerrain->GetPatchSize();
	
	// Restore deco textures
	m_pDecoTexture->RestoreAllTextures( m_pd3dDevice );

	// Set current decoobj
	SetCurDecoObj( -1 );

	// Camera reset
	m_pCamera->SetPitch( 45.0f );
	m_pCamera->SetYaw( 0.0f );
	m_pRoh->SetInitPos( 10.0f, 10.0f );
	m_vTarget = D3DXVECTOR3( m_pRoh->m_wx, m_pRoh->m_wy, m_pRoh->m_wz );
	if( m_bShowCharacter )
		m_vTarget.y += 15.0f;		
	m_pCamera->SetTarget( m_vTarget );
	m_pCamera->UpdateViewMatrix();
	if( m_nCameraType == 1 )
		m_pCamera->UpdateFlyViewMatrix();

	// Create the sky
	CreateSky();
	g_pCapsyongTexture->RestoreAllTextures( m_pd3dDevice );

	// Init water control mesh
	InitWaterMesh();

	m_bModify[0] = TRUE;
	m_bModify[1] = FALSE;
	m_bModify[2] = FALSE;

	m_bShowTerrain = TRUE;
	m_bShowObject = TRUE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name : LoadNmf()
// Desc :
//-----------------------------------------------------------------------------
BOOL CMyNKApp::LoadNmf( const TCHAR* szDirectory, const TCHAR* szFilename )
{
	if( IsCameraTopView() )
		ToggleCameraTopView();

	m_bLoadedGameMap = FALSE;

	// Destroy deco textures
	if( m_pDecoTexture )
	{
		m_pDecoTexture->InvalidateAllTextures();
		m_pDecoTexture->DestroyTextureAll();
	}

	// Delete decoobjs
	DeleteDecoObjData();
	ResetSelDecoObjList();
	m_UndoObj.Destory();

	// Remove textures in a list box of a texture paintor tool
	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndToolCtrlBar.m_pageTexPaint.RemoveAllTextures();

	// Get a full path
	CString	strFullPath;
	strFullPath.Format( _T("%smap\\%s\\%s.nmf"), m_szWorkingPath, szDirectory, szFilename );

	// Load nmf
	ChangeWorldTime( m_nWorldHour );
	if( !m_pTerrain->LoadNmf( strFullPath ) )
	{
		CString	strTemp;
		strTemp.Format( _T("'%s.nmf' 파일을 읽을 수 없습니다."), szFilename );
		MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
		m_nWorkingState = NONE;
		return FALSE;
	}

	// Add textures in a list box of a texture paintor tool
	for( int i = 0; i < m_pTerrain->GetTextureCount(); i++ )
		pFrame->m_wndToolCtrlBar.m_pageTexPaint.AddTexture( m_pTerrain->GetTextureName( i ) );
	
	// Load decoobjs
	CString	strDecoPath;
	strDecoPath.Format( _T("data\\building\\%s\\"), m_pTerrain->GetTerrainName() );
	DecoObjInitialLoading( strDecoPath, szFilename );

	// Set ranges that decoobjs can be located
	m_fObjMinXZ = 0.0f;
	m_fObjMaxXZ = ( m_pTerrain->GetHMWidth() - 1 ) * m_pTerrain->GetPatchSize();
	
	// Restore deco textures
	m_pDecoTexture->RestoreAllTextures( m_pd3dDevice );

	// Set current decoobj
	SetCurDecoObj( -1 );

	// Camera reset
	m_pCamera->SetPitch( 45.0f );
	m_pCamera->SetYaw( 0.0f );
	m_pRoh->SetInitPos( 10.0f, 10.0f );
	m_vTarget = D3DXVECTOR3( m_pRoh->m_wx, m_pRoh->m_wy, m_pRoh->m_wz );
	if( m_bShowCharacter )
		m_vTarget.y += 15.0f;
	m_pCamera->SetTarget( m_vTarget );
	m_pCamera->UpdateViewMatrix();
	if( m_nCameraType == 1 )
		m_pCamera->UpdateFlyViewMatrix();

	// Create the sky
	CreateSky();
	g_pCapsyongTexture->RestoreAllTextures( m_pd3dDevice );

	// Init water control mesh
	InitWaterMesh();

	m_bModify[0] = FALSE;
	m_bModify[1] = FALSE;
	m_bModify[2] = FALSE;

	m_bShowTerrain = TRUE;
	m_bShowObject = TRUE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name : LoadMap()
// Desc :
//-----------------------------------------------------------------------------
BOOL CMyNKApp::LoadMap( const TCHAR* szDirectory, const TCHAR* szFilename )
{
	if( IsCameraTopView() )
		ToggleCameraTopView();

	m_bLoadedGameMap = TRUE;

	// Destroy deco textures
	if( m_pDecoTexture )
	{
		m_pDecoTexture->InvalidateAllTextures();
		m_pDecoTexture->DestroyTextureAll();
	}

	// Delete decoobjs
	DeleteDecoObjData();
	ResetSelDecoObjList();

	// Remove textures in a list box of a texture paintor tool
	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndToolCtrlBar.m_pageTexPaint.RemoveAllTextures();

	// Get a full path
	CString	strFullPath;
	strFullPath.Format( _T("%smap\\%s\\%s.map"), m_szWorkingPath, szDirectory, szFilename );

	// Load decoobjs
	CString	strDecoPath;
	strDecoPath.Format( _T("data\\building\\%s\\"), m_pTerrain->GetTerrainName() );
	DecoObjInitialLoading( strDecoPath, szFilename );

	// Restore deco textures
	m_pDecoTexture->RestoreAllTextures( m_pd3dDevice );

	// Load nmf
	ChangeWorldTime( m_nWorldHour );
	if( !m_pTerrain->LoadMap( strFullPath ) )
	{
		CString	strTemp;
		strTemp.Format( _T("'%s.map' 파일을 읽을 수 없습니다."), szFilename );
		MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
		m_nWorkingState = NONE;
		return FALSE;
	}

	// Camera reset
	m_pCamera->SetPitch( 45.0f );
	m_pCamera->SetYaw( 0.0f );
	m_pRoh->SetInitPos( 10.0f, 10.0f );
	m_vTarget = D3DXVECTOR3( m_pRoh->m_wx, m_pRoh->m_wy, m_pRoh->m_wz );
	if( m_bShowCharacter )
		m_vTarget.y += 15.0f;
	m_pCamera->SetTarget( m_vTarget );
	m_pCamera->UpdateViewMatrix();
	if( m_nCameraType == 1 )
		m_pCamera->UpdateFlyViewMatrix();

	// Create the sky
	CreateSky();
	g_pCapsyongTexture->RestoreAllTextures( m_pd3dDevice );

	m_bModify[0] = FALSE;
	m_bModify[1] = FALSE;
	m_bModify[2] = FALSE;

	m_bShowTerrain = TRUE;
	m_bShowObject = TRUE;
	m_bShowWater = TRUE;
	m_bShowFog = TRUE;
	m_bShowCharacter = TRUE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name : ImportBldLst()
// Desc :
//-----------------------------------------------------------------------------
BOOL CMyNKApp::ImportBldLst()
{
	// Destroy deco textures
	if( m_pDecoTexture )
	{
		m_pDecoTexture->InvalidateAllTextures();
		m_pDecoTexture->DestroyTextureAll();
	}

	// Delete decoobjs
	DeleteDecoObjData();
	ResetSelDecoObjList();

	// Load decoobjs
	CString	strDecoPath;
	strDecoPath.Format( _T("data\\building\\%s\\"), m_pTerrain->GetTerrainName() );
	DecoObjInitialLoading( strDecoPath, NULL );

	// Restore deco textures
	m_pDecoTexture->RestoreAllTextures( m_pd3dDevice );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name : SaveNmf()
// Desc :
//-----------------------------------------------------------------------------
BOOL CMyNKApp::SaveNmf( const TCHAR* szFilename )
{		
	if( IsCameraTopView() )
		ToggleCameraTopView();

	CString	strFullPath, strTemp;
	FILE*	fp;
	
	if( !m_bModify[0] )		// Nmf file
	{
		// Get a full path
		strFullPath.Format( _T("%smap\\%s\\%s.nmf"), m_szWorkingPath, m_pTerrain->GetTerrainName(), szFilename );
		
		fp = fopen( strFullPath, "rb" );
		if( fp == NULL )
			m_bModify[0] = TRUE;
		else
			fclose( fp );
	}
	if( m_bModify[0] )
	{
		// Get a full path
		strFullPath.Format( _T("%smap\\%s\\%s.nmf"), m_szWorkingPath,
								m_pTerrain->GetTerrainName(), szFilename );

		if( m_pTerrain->SaveNmf( strFullPath ) )
			m_bModify[0] = FALSE;
		else
		{
			strTemp.Format( _T("'%s.nmf' 파일을 저장할 수 없습니다."), szFilename );
			MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
			return FALSE;
		}
	}

	if( m_bModify[1] )		// Bld file
	{
		// Get a full path
		strFullPath.Format( _T("%sdata\\building\\%s\\%s.nbl"), m_szWorkingPath,
								m_pTerrain->GetTerrainName(), szFilename );

		if( g_pDecoObjSet->SaveDataFile( strFullPath ) )
			m_bModify[1] = FALSE;
		else
		{
			strTemp.Format( _T("'%s.nbl' 파일을 저장할 수 없습니다."), szFilename );
			MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
			return FALSE;
		}
	}

	if( m_bModify[2] )		// Not file
	{
		// Get a full path
		strFullPath.Format( _T("%sdata\\building\\%s\\%s.not"), m_szWorkingPath,
								m_pTerrain->GetTerrainName(), szFilename );

		if( CNKObjQuadNode::SaveNot( strFullPath ) )
			m_bModify[2] = FALSE;
		else
		{
			strTemp.Format( _T("'%s.not' 파일을 저장할 수 없습니다."), szFilename );
			MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
			return FALSE;
		}			
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name : SaveMap()
// Desc :
//-----------------------------------------------------------------------------
BOOL CMyNKApp::SaveMap( const TCHAR* szFilename )
{	
	if( IsCameraTopView() )
		ToggleCameraTopView();

	CString	strFullPath, strTemp;

	// Map file
	strFullPath.Format( _T("%smap\\%s\\%s.map"), m_szWorkingPath, m_pTerrain->GetTerrainName(), szFilename );
	if( !m_pTerrain->SaveMap( strFullPath ) )
	{
		strTemp.Format( _T("'%s.map' 파일을 저장할 수 없습니다."), szFilename );
		MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
		return FALSE;
	}

	// Bld file
	strFullPath.Format( _T("%sdata\\building\\%s\\%s.bld"), m_szWorkingPath, m_pTerrain->GetTerrainName(), szFilename );

	if( !g_pDecoObjSet->SaveDataFile( strFullPath ) )
	{
		strTemp.Format( _T("'%s.bld' 파일을 저장할 수 없습니다."), szFilename );
		MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
		return FALSE;
	}

/*
	// [2008/3/31 Theodoric] 인스톨 쉴드에서 확장자가 obj 가 있으면 에러가 남. 그래서 oba로 바꿈
	// Obj file 
	strFullPath.Format( _T("%sdata\\building\\%s\\%s.obj"), m_szWorkingPath, m_pTerrain->GetTerrainName(), szFilename );
	if( !CNKObjQuadNode::SaveObj( strFullPath ) )
	{
		strTemp.Format( _T("'%s.obj' 파일을 저장할 수 없습니다."), szFilename );
		MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
		return FALSE;
	}
*/
	// Obj file
	strFullPath.Format( _T("%sdata\\building\\%s\\%s.oba"), m_szWorkingPath, m_pTerrain->GetTerrainName(), szFilename );
	if( !CNKObjQuadNode::SaveObj( strFullPath ) )
	{
		strTemp.Format( _T("'%s.oba' 파일을 저장할 수 없습니다."), szFilename );
		MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
		return FALSE;
	}

	// DecoObjList.lst (구맵툴에 쓰이던 텍스트 형식 오브젝트 배치 리스트) (로그인 존 등에 필요해서 추가함)
	strFullPath.Format( _T("%sdata\\building\\%s\\DecoObjList.lst"), m_szWorkingPath,
										m_pTerrain->GetTerrainName() );
	if( !CNKObjQuadNode::SaveDecoList( strFullPath ) )
	{
		strTemp.Format( _T("'DecoObjList.lst' 파일을 저장할 수 없습니다.") );
		MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
		return FALSE;
	}

	
	// Wat file
	strFullPath.Format( _T("%sdata\\building\\%s\\%s.wat"), m_szWorkingPath,
										m_pTerrain->GetTerrainName(), szFilename );
	if( !CNKObjQuadNode::SaveWat( strFullPath ) )
	{
		strTemp.Format( _T("'%s.wat' 파일을 저장할 수 없습니다."), szFilename );
		MessageBox( NULL, strTemp, _T("NKMapTool"), MB_OK );
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name : SetCurTileTexture()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetCurSelTexture( int nIndex )
{
	if( m_nCurTileTexture != nIndex )			// If a current texture is changed, set an index of a new texture
	{
		m_nCurTileTexture = nIndex;

		if( nIndex == -1 )
		{
			m_nDirtyRegionSX = INT_MAX;		m_nDirtyRegionEX = INT_MIN;
			m_nDirtyRegionSZ = INT_MAX;		m_nDirtyRegionEZ = INT_MIN;
		}
		else if( m_nDirtyRegionSX != INT_MAX )			// If dirty regions are changed, optimize layers
		{
			m_pTerrain->OptimizeLayers( m_nDirtyRegionSZ, m_nDirtyRegionSX,
										m_nDirtyRegionEZ, m_nDirtyRegionEX );
			m_nDirtyRegionSX = INT_MAX;		m_nDirtyRegionEX = INT_MIN;
			m_nDirtyRegionSZ = INT_MAX;		m_nDirtyRegionEZ = INT_MIN;
		}
	}
}


//-----------------------------------------------------------------------------
// Name : DrawTileTexture()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::DrawTileTexture()
{
	int nX, nZ;

	if( m_nEditingLayer == 2 || m_nEditingLayer == 3 )		// Layer 2 or 3 ( blending )
	{
		if( m_bCircleBrush )
		{
			float	aValue[4];
			int		nStrength = 255.0f * ( m_nBrushStrength / 100.0f );
			float	fPatch = m_pTerrain->GetPatchSize();
			float	fQuaterPatch = fPatch * 0.25f;
			float	fThreeQuaterPatch = fPatch * 0.75f;
			float	fSqrDistX, fSqrDistZ, fSqrDist;
			float	fSqrRadius = m_nBrushSize * m_pTerrain->GetPatchSize() * 0.25f;
			fSqrRadius *= fSqrRadius;

			for( nZ = m_nTileSZ; nZ <= m_nTileEZ; nZ++ )
			{
				for( nX = m_nTileSX; nX <= m_nTileEX; nX++ )
				{
					// Calculate a alpha value
					fSqrDistX = m_vPickedPoint.x - ( nX*fPatch + fQuaterPatch );		fSqrDistX *= fSqrDistX;
					fSqrDistZ = m_vPickedPoint.z - ( nZ*fPatch + fQuaterPatch );		fSqrDistZ *= fSqrDistZ;
					fSqrDist = fSqrDistX + fSqrDistZ;
					aValue[0] = nStrength * ( 1.0f - ( fSqrDist / fSqrRadius ) );
					if( aValue[0] < 0 ) aValue[0] = 0;
					else if( aValue[0] > 255 ) aValue[0] = 255;

					fSqrDistX = m_vPickedPoint.x - ( nX*fPatch + fThreeQuaterPatch );	fSqrDistX *= fSqrDistX;
					fSqrDistZ = m_vPickedPoint.z - ( nZ*fPatch + fQuaterPatch );		fSqrDistZ *= fSqrDistZ;
					fSqrDist = fSqrDistX + fSqrDistZ;
					aValue[1] = nStrength * ( 1.0f - ( fSqrDist / fSqrRadius ) );
					if( aValue[1] < 0 ) aValue[1] = 0;
					else if( aValue[1] > 255 ) aValue[1] = 255;

					fSqrDistX = m_vPickedPoint.x - ( nX*fPatch + fQuaterPatch );		fSqrDistX *= fSqrDistX;
					fSqrDistZ = m_vPickedPoint.z - ( nZ*fPatch + fThreeQuaterPatch );	fSqrDistZ *= fSqrDistZ;
					fSqrDist = fSqrDistX + fSqrDistZ;
					aValue[2] = nStrength * ( 1.0f - ( fSqrDist / fSqrRadius ) );
					if( aValue[2] < 0 ) aValue[2] = 0;
					else if( aValue[2] > 255 ) aValue[2] = 255;

					fSqrDistX = m_vPickedPoint.x - ( nX*fPatch + fThreeQuaterPatch );	fSqrDistX *= fSqrDistX;
					fSqrDistZ = m_vPickedPoint.z - ( nZ*fPatch + fThreeQuaterPatch );	fSqrDistZ *= fSqrDistZ;
					fSqrDist = fSqrDistX + fSqrDistZ;
					aValue[3] = nStrength * ( 1.0f - ( fSqrDist / fSqrRadius ) );
					if( aValue[3] < 0 ) aValue[3] = 0;
					else if( aValue[3] > 255 ) aValue[3] = 255;

					if( m_nTexEditType == 1 )		// Erasing mode
					{
						aValue[0] = -aValue[0];
						aValue[1] = -aValue[1];
						aValue[2] = -aValue[2];
						aValue[3] = -aValue[3];
						m_pTerrain->DrawBlendTileBrush( nZ, nX, m_nEditingLayer, m_nCurTileTexture, aValue, false );
					}
					else
						m_pTerrain->DrawBlendTileBrush( nZ, nX, m_nEditingLayer, m_nCurTileTexture, aValue, true );
				}
			}			
		}
		else
		{
			DWORD	dwStrength = 255.0f * ( m_nBrushStrength / 100.0f );
			m_pTerrain->DrawBlendTilePencil( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX, m_nEditingLayer,
												m_nCurTileTexture, dwStrength, m_nTexEditType == 0 );
		}

		// Update a color map
		int	nWidth = 1 << m_pTerrain->GetQuadTreeDepth();
		if( m_nTileSX < nWidth )
		{
			if( m_nTileSZ < nWidth ) m_pTerrain->UpdateClrMapTexture( m_nEditingLayer, 0 );
			if( m_nTileSZ >= nWidth ) m_pTerrain->UpdateClrMapTexture( m_nEditingLayer, 2 );
		}
		if( m_nTileEX >= nWidth )
		{
			if( m_nTileSZ < nWidth ) m_pTerrain->UpdateClrMapTexture( m_nEditingLayer, 1 );
			if( m_nTileSZ >= nWidth ) m_pTerrain->UpdateClrMapTexture( m_nEditingLayer, 3 );
		}

		// Renew dirty regions
		if( m_nTileSX < m_nDirtyRegionSX ) m_nDirtyRegionSX = m_nTileSX;
		if( m_nTileEX > m_nDirtyRegionEX ) m_nDirtyRegionEX = m_nTileEX;
		if( m_nTileSZ < m_nDirtyRegionSZ ) m_nDirtyRegionSZ = m_nTileSZ;
		if( m_nTileEZ > m_nDirtyRegionEZ ) m_nDirtyRegionEZ = m_nTileEZ;
	}
	else if( m_nEditingLayer == 1 )				// Layer 1 ( non-blending )
	{
		if( m_nTexEditType == 0 )				// Painting mode
			m_pTerrain->DrawNonblendTile( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX, m_nCurTileTexture );
		else									// Erasing mode
			m_pTerrain->DrawNonblendTile( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX, TEXTURE_MAX_COUNT ); 
	}	
}


//-----------------------------------------------------------------------------
// Name : DrawShadow()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::DrawShadow()
{
	int		i, nX, nZ;
	float	aR[4], aG[4], aB[4];
	float	fPatch = m_pTerrain->GetPatchSize();
	float	fQuaterPatch = fPatch * 0.25f;
	float	fThreeQuaterPatch = fPatch * 0.75f;
	float	fSqrDistX, fSqrDistZ, fSqrDist;
	float	fSqrRadius = m_nBrushSize * m_pTerrain->GetPatchSize() * 0.25f;
	fSqrRadius *= fSqrRadius;
	float	r, g, b;
	
	if( m_nShadowEditType == 0 )
	{
		r = 1.0f - m_fShadowR;
		g = 1.0f - m_fShadowG;
		b = 1.0f - m_fShadowB;
	}
	else
	{
		r = g = b = 1.0f;
	}

	r *= m_nBrushStrength / 100.0f;
	g *= m_nBrushStrength / 100.0f;
	b *= m_nBrushStrength / 100.0f;

	for( nZ = m_nTileSZ; nZ <= m_nTileEZ; nZ++ )
	{
		for( nX = m_nTileSX; nX <= m_nTileEX; nX++ )
		{
			// Calculate a shadow color
			fSqrDistX = m_vPickedPoint.x - ( nX*fPatch + fQuaterPatch );		fSqrDistX *= fSqrDistX;
			fSqrDistZ = m_vPickedPoint.z - ( nZ*fPatch + fQuaterPatch );		fSqrDistZ *= fSqrDistZ;
			fSqrDist = fSqrDistX + fSqrDistZ;
			aR[0] = r * ( 1.0f - ( fSqrDist / fSqrRadius ) );
			if( aR[0] < 0 ) aR[0] = 0;
			else if( aR[0] > r ) aR[0] = r;
			aG[0] = g * ( 1.0f - (fSqrDist / fSqrRadius) );
			if( aG[0] < 0 ) aG[0] = 0;
			else if( aG[0] > g ) aG[0] = g;
			aB[0] = b * ( 1.0f - (fSqrDist / fSqrRadius) );
			if( aB[0] < 0 ) aB[0] = 0;
			else if( aB[0] > b ) aB[0] = b;
			
			fSqrDistX = m_vPickedPoint.x - ( nX*fPatch + fThreeQuaterPatch );	fSqrDistX *= fSqrDistX;
			fSqrDistZ = m_vPickedPoint.z - ( nZ*fPatch + fQuaterPatch );		fSqrDistZ *= fSqrDistZ;
			fSqrDist = fSqrDistX + fSqrDistZ;
			aR[1] = r * ( 1.0f - ( fSqrDist / fSqrRadius ) );
			if( aR[1] < 0 ) aR[1] = 0;
			else if( aR[0] > r ) aR[1] = r;
			aG[1] = g * ( 1.0f - (fSqrDist / fSqrRadius) );
			if( aG[1] < 0 ) aG[1] = 0;
			else if( aG[1] > g ) aG[1] = g;
			aB[1] = b * ( 1.0f - (fSqrDist / fSqrRadius) );
			if( aB[1] < 0 ) aB[1] = 0;
			else if( aB[1] > b ) aB[1] = b;

			fSqrDistX = m_vPickedPoint.x - ( nX*fPatch + fQuaterPatch );		fSqrDistX *= fSqrDistX;
			fSqrDistZ = m_vPickedPoint.z - ( nZ*fPatch + fThreeQuaterPatch );	fSqrDistZ *= fSqrDistZ;
			fSqrDist = fSqrDistX + fSqrDistZ;
			aR[2] = r * ( 1.0f - ( fSqrDist / fSqrRadius ) );
			if( aR[2] < 0 ) aR[2] = 0;
			else if( aR[2] > r ) aR[2] = r;
			aG[2] = g * ( 1.0f - (fSqrDist / fSqrRadius) );
			if( aG[2] < 0 ) aG[2] = 0;
			else if( aG[2] > g ) aG[2] = g;
			aB[2] = b * ( 1.0f - (fSqrDist / fSqrRadius) );
			if( aB[2] < 0 ) aB[2] = 0;
			else if( aB[2] > b ) aB[2] = b;

			fSqrDistX = m_vPickedPoint.x - ( nX*fPatch + fThreeQuaterPatch );	fSqrDistX *= fSqrDistX;
			fSqrDistZ = m_vPickedPoint.z - ( nZ*fPatch + fThreeQuaterPatch );	fSqrDistZ *= fSqrDistZ;
			fSqrDist = fSqrDistX + fSqrDistZ;
			aR[3] = r * ( 1.0f - ( fSqrDist / fSqrRadius ) );
			if( aR[3] < 0 ) aR[3] = 0;
			else if( aR[3] > r ) aR[3] = r;
			aG[3] = g * ( 1.0f - (fSqrDist / fSqrRadius) );
			if( aG[3] < 0 ) aG[3] = 0;
			else if( aG[3] > g ) aG[3] = g;
			aB[3] = b * ( 1.0f - (fSqrDist / fSqrRadius) );
			if( aB[3] < 0 ) aB[3] = 0;
			else if( aB[3] > b ) aB[3] = b;

			if( m_bDrawShadowCurrentMaps )
			{
				m_pTerrain->DrawShadowColor( -1, nZ, nX, m_fShadowR, m_fShadowG, m_fShadowB,
												aR, aG, aB, m_nShadowEditType == 0 );
			}
			else
			{
				for( i = 0 ; i < 6; i++ )
					m_pTerrain->DrawShadowColor( i, nZ, nX, m_fShadowR, m_fShadowG, m_fShadowB,
													aR, aG, aB, m_nShadowEditType == 0 );
			}
		}
	}

	// Update RGB maps
	m_pTerrain->UpdateRGBMapsByShadow( m_bDrawShadowCurrentMaps, m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX );

	// Update a color map
	int	nWidth = 1 << m_pTerrain->GetQuadTreeDepth();
	if( m_nTileSX < nWidth )
	{
		if( m_nTileSZ < nWidth )
		{
			m_pTerrain->UpdateClrMapTexture( 2, 0 );
			m_pTerrain->UpdateClrMapTexture( 3, 0 );
		}
		if( m_nTileSZ >= nWidth )
		{
			m_pTerrain->UpdateClrMapTexture( 2, 2 );
			m_pTerrain->UpdateClrMapTexture( 3, 2 );
		}
	}
	if( m_nTileEX >= nWidth )
	{
		if( m_nTileSZ < nWidth )
		{
			m_pTerrain->UpdateClrMapTexture( 2, 1 );
			m_pTerrain->UpdateClrMapTexture( 3, 1 );
		}
		if( m_nTileSZ >= nWidth )
		{
			m_pTerrain->UpdateClrMapTexture( 2, 3 );
			m_pTerrain->UpdateClrMapTexture( 3, 3 );
		}
	}
}


//-----------------------------------------------------------------------------
// Name : MakeBrushMesh()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::MakeBrushMesh()
{
	float		fPatchSize = m_pTerrain->GetPatchSize();
	float		fInvPatchSize = 1.0f / fPatchSize;
	int			nX, nZ;

	if( m_bCircleBrush )		// Circle
	{
		float	fBrushRadius;
		if( m_nWorkingState == TEXTURE_PAINT || m_nWorkingState == LIGHT_EDIT )
			fBrushRadius = m_nBrushSize * fPatchSize * 0.25f;
		else
			fBrushRadius = m_nBrushSize * fPatchSize * 0.5f;

		m_nTileSX = ( m_vPickedPoint.x - fBrushRadius + NK_EPSILON3 ) * fInvPatchSize;
		m_nTileEX = ( m_vPickedPoint.x + fBrushRadius - NK_EPSILON3 ) * fInvPatchSize;
		m_nTileSZ = ( m_vPickedPoint.z - fBrushRadius + NK_EPSILON3 ) * fInvPatchSize;
		m_nTileEZ = ( m_vPickedPoint.z + fBrushRadius - NK_EPSILON3 ) * fInvPatchSize;

		// Check a valid region
		m_nTileSX = m_nTileSX < 0 ? 0 : m_nTileSX;
		m_nTileSZ = m_nTileSZ < 0 ? 0 : m_nTileSZ;
		m_nTileEX = m_nTileEX > m_pTerrain->GetHMWidth() - 2 ? m_pTerrain->GetHMWidth() - 2 : m_nTileEX;
		m_nTileEZ = m_nTileEZ > m_pTerrain->GetHMWidth() - 2 ? m_pTerrain->GetHMWidth() - 2 : m_nTileEZ;

		// Make a brush mesh
		m_dwBrushVertNum = 0;

		float	fDelta = NK_TWOPI / 144.0f;
		float	fMaxXZ = ( m_pTerrain->GetHMWidth() - 1 ) * m_pTerrain->GetPatchSize();
		float	fX, fZ;

		for( float fRadian = 0.0f; fRadian < NK_TWOPI; fRadian += fDelta )
		{
			fX = m_vPickedPoint.x + fBrushRadius*cosf( fRadian );
			fZ = m_vPickedPoint.z + fBrushRadius*sinf( fRadian );
			if( fX >= 0.0f && fX <= fMaxXZ && fZ >= 0.0f && fZ <= fMaxXZ )
			{
				m_aBrushVB[m_dwBrushVertNum].x = fX;
				m_aBrushVB[m_dwBrushVertNum].y = m_pTerrain->GetHeights( fZ, fX ) + 1.0f;
				m_aBrushVB[m_dwBrushVertNum].z = fZ;
				m_aBrushVB[m_dwBrushVertNum++].c = m_dwBrushColor;
			}
		}
		
		m_aBrushVB[m_dwBrushVertNum].x = m_aBrushVB[0].x;
		m_aBrushVB[m_dwBrushVertNum].y = m_aBrushVB[0].y;
		m_aBrushVB[m_dwBrushVertNum].z = m_aBrushVB[0].z;
		m_aBrushVB[m_dwBrushVertNum++].c = m_dwBrushColor;
	}
	else							// Rectangle
	{
		if( m_nWorkingState == ATTRIBUTE_EDIT )
		{
			int	nWidth = ( m_pTerrain->GetHMWidth() - 1 ) * 5;
			m_nTileSX = m_vPickedPoint.x / 10.0f;
			m_nTileSZ = m_vPickedPoint.z / 10.0f;
			
			if( m_nTileSX < 0 )
				m_nTileSX = 0;
			else if( m_nTileSX >= nWidth )
				m_nTileSX = nWidth - 1;
			if( m_nTileSZ < 0 )
				m_nTileSZ = 0;
			else if( m_nTileSZ >= nWidth )
				m_nTileSZ = nWidth - 1;

			int TempHalfBrushSize = m_nBrushSize / 2;
			float TempHalfArea = 10.0f * (float)TempHalfBrushSize;
			
			float	fX = m_nTileSX * 10.0f;
			float	fZ = ( m_nTileSZ + 1 ) * 10.0f;
			m_aBrushVB[0].c = m_aBrushVB[1].c = m_aBrushVB[2].c = m_aBrushVB[3].c =
				m_pTerrain->GetMapAttrColor( m_nCurMapAttr );

			// 브러쉬 크기 조절 기능 추가(by 원석)
			m_aBrushVB[0].p = D3DXVECTOR3( fX - TempHalfArea, m_pTerrain->GetHeights( fZ, fX ) + 0.5f, fZ + TempHalfArea );
			m_aBrushVB[1].p = D3DXVECTOR3( fX + 10.0f + TempHalfArea, m_pTerrain->GetHeights( fZ, fX + 10.0f ) + 0.5f, fZ + TempHalfArea );
			m_aBrushVB[2].p = D3DXVECTOR3( fX - TempHalfArea, m_pTerrain->GetHeights( fZ - 10.0f, fX ) + 0.5f, fZ - 10.0f - TempHalfArea );
			m_aBrushVB[3].p = D3DXVECTOR3( fX + 10.0f + TempHalfArea, m_pTerrain->GetHeights( fZ - 10.0f, fX + 10.0f ) + 0.5f, fZ - 10.0f - TempHalfArea );			
/*          // 옛날 버젼 소스(브러쉬 크기조절 없을 때)
			m_aBrushVB[0].p = D3DXVECTOR3( fX, m_pTerrain->GetHeights( fZ, fX ) + 0.5f, fZ );
			m_aBrushVB[1].p = D3DXVECTOR3( fX + 10.0f, m_pTerrain->GetHeights( fZ, fX + 10.0f ) + 0.5f, fZ );
			m_aBrushVB[2].p = D3DXVECTOR3( fX, m_pTerrain->GetHeights( fZ - 10.0f, fX ) + 0.5f, fZ - 10.0f );
			m_aBrushVB[3].p = D3DXVECTOR3( fX + 10.0f, m_pTerrain->GetHeights( fZ - 10.0f, fX + 10.0f ) + 0.5f, fZ - 10.0f );			
*/
			m_dwBrushVertNum = 4;

			return ;
		}

		m_nTileSX = m_vPickedPoint.x * fInvPatchSize;
		m_nTileSZ = m_vPickedPoint.z * fInvPatchSize;
		if( m_nBrushSize % 2 )		// 1, 3, 5, 7, ...
		{
			m_nTileSX -= m_nBrushSize >> 1;
			m_nTileSZ -= m_nBrushSize >> 1;
		}
		else						// 2, 4, 6, 8, ...
		{
			if( m_vPickedPoint.x < m_nTileSX*fPatchSize + fPatchSize/2 ) m_nTileSX--;
			if( m_vPickedPoint.z < m_nTileSZ*fPatchSize + fPatchSize/2 ) m_nTileSZ--;
			m_nTileSX -= ( m_nBrushSize >> 1 ) - 1;
			m_nTileSZ -= ( m_nBrushSize >> 1 ) - 1;
		}
		m_nTileEX = m_nTileSX + m_nBrushSize - 1;
		m_nTileEZ = m_nTileSZ + m_nBrushSize - 1;
		
		// Check a valid region
		m_nTileSX = m_nTileSX < 0 ? 0 : m_nTileSX;
		m_nTileSZ = m_nTileSZ < 0 ? 0 : m_nTileSZ;
		m_nTileEX = m_nTileEX > m_pTerrain->GetHMWidth() - 2 ? m_pTerrain->GetHMWidth() - 2 : m_nTileEX;
		m_nTileEZ = m_nTileEZ > m_pTerrain->GetHMWidth() - 2 ? m_pTerrain->GetHMWidth() - 2 : m_nTileEZ;

		// Renew a brush mesh
		m_dwBrushVertNum = 0;
		for( nX = m_nTileSX; nX <= m_nTileEX; nX++ )
		{
			m_aBrushVB[m_dwBrushVertNum].x = nX * fPatchSize;
			m_aBrushVB[m_dwBrushVertNum].y = m_pTerrain->GetHeights( m_nTileSZ, nX ) + 1.0f;
			m_aBrushVB[m_dwBrushVertNum].z = m_nTileSZ * fPatchSize;
			m_aBrushVB[m_dwBrushVertNum++].c = m_dwBrushColor;
		}
		for( nZ = m_nTileSZ; nZ <= m_nTileEZ; nZ++ )
		{
			m_aBrushVB[m_dwBrushVertNum].x = ( m_nTileEX + 1 ) * fPatchSize;
			m_aBrushVB[m_dwBrushVertNum].y = m_pTerrain->GetHeights( nZ, m_nTileEX + 1 ) + 1.0f;
			m_aBrushVB[m_dwBrushVertNum].z = nZ * fPatchSize;
			m_aBrushVB[m_dwBrushVertNum++].c = m_dwBrushColor;
		}
		for( nX = m_nTileEX + 1; nX > m_nTileSX; nX-- )
		{
			m_aBrushVB[m_dwBrushVertNum].x = nX * fPatchSize;
			m_aBrushVB[m_dwBrushVertNum].y = m_pTerrain->GetHeights( m_nTileEZ + 1, nX ) + 1.0f;
			m_aBrushVB[m_dwBrushVertNum].z = ( m_nTileEZ + 1 ) * fPatchSize;
			m_aBrushVB[m_dwBrushVertNum++].c = m_dwBrushColor;
		}
		for( nZ = m_nTileEZ + 1; nZ >= m_nTileSZ; nZ-- )
		{
			m_aBrushVB[m_dwBrushVertNum].x = m_nTileSX * fPatchSize;
			m_aBrushVB[m_dwBrushVertNum].y = m_pTerrain->GetHeights( nZ, m_nTileSX ) + 1.0f;
			m_aBrushVB[m_dwBrushVertNum].z = nZ * fPatchSize;
			m_aBrushVB[m_dwBrushVertNum++].c = m_dwBrushColor;
		}
	}
}


//-----------------------------------------------------------------------------
// Name : MakeWaterMesh()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::MakeWaterMesh()
{
	if( m_nWaterGroupID == -1 )
	{
		m_dwBrushVertNum = 0;
		m_dwWaveIndexCount = 0;
		return ;
	}

	int		i, j, nCount, nWidth;
	float	fX, fZ;
	float	fHeight = CNKObjQuadNode::GetHeightOfWaterGroup( m_nWaterGroupID );

	// Water guide line
	nWidth = CNKObjQuadNode::GetCellSize();
	m_nTileSX = m_vPickedPoint.x / nWidth;
	m_nTileSZ = m_vPickedPoint.z / nWidth;
	m_nTileSX -= 1;					m_nTileSZ -= 1;
	m_nTileEX = m_nTileSX + 3;		m_nTileEZ = m_nTileSZ + 3;
	nCount = 0;
	for( i = m_nTileSX;	i <= m_nTileEX; i++ )
	{
		fX = i * nWidth;
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( fX, fHeight, m_nTileSZ * nWidth );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( fX, fHeight, ( m_nTileSZ + 1 ) * nWidth );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( fX, fHeight, ( m_nTileSZ + 1 ) * nWidth );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( fX, fHeight, ( m_nTileSZ + 2 ) * nWidth );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( fX, fHeight, ( m_nTileSZ + 2 ) * nWidth );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( fX, fHeight, ( m_nTileSZ + 3 ) * nWidth );
	}
	for( i = m_nTileSZ;	i <= m_nTileEZ; i++ )
	{
		fZ = i * nWidth;
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( m_nTileSX * nWidth, fHeight, fZ );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( ( m_nTileSX + 1 ) * nWidth, fHeight, fZ );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( ( m_nTileSX + 1 ) * nWidth, fHeight, fZ );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( ( m_nTileSX + 2 ) * nWidth, fHeight, fZ );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( ( m_nTileSX + 2 ) * nWidth, fHeight, fZ );
		m_aWaterGuideVB[nCount++].p = D3DXVECTOR3( ( m_nTileSX + 3 ) * nWidth, fHeight, fZ );
	}

	// Outline of the water plane
	if( m_nWaterType == 0 )
	{
		fHeight += 0.5f;
		nWidth = CNKObjQuadNode::GetBaseWaterSize();
		m_aBaseWaterVB[0].x = 0.0f;		m_aBaseWaterVB[0].y = fHeight;	m_aBaseWaterVB[0].z = nWidth;
		m_aBaseWaterVB[1].x = nWidth;	m_aBaseWaterVB[1].y = fHeight;	m_aBaseWaterVB[1].z = nWidth;
		m_aBaseWaterVB[2].x = 0.0f;		m_aBaseWaterVB[2].y = fHeight;	m_aBaseWaterVB[2].z = 0.0f;
		m_aBaseWaterVB[3].x = nWidth;	m_aBaseWaterVB[3].y = fHeight;	m_aBaseWaterVB[3].z = 0.0f;

		m_nTileSX = m_vPickedPoint.x / nWidth;
		m_nTileSZ = m_vPickedPoint.z / nWidth;
		fX = m_nTileSX * nWidth;
		fZ = m_nTileSZ * nWidth;
		m_aBaseWaterVB[0].x += fX;		m_aBaseWaterVB[0].z += fZ;
		m_aBaseWaterVB[1].x += fX;		m_aBaseWaterVB[1].z += fZ;
		m_aBaseWaterVB[2].x += fX;		m_aBaseWaterVB[2].z += fZ;
		m_aBaseWaterVB[3].x += fX;		m_aBaseWaterVB[3].z += fZ;

		m_dwBrushVertNum = 5;
		m_aBrushVB[0].x = m_aBaseWaterVB[0].x;	m_aBrushVB[0].y = m_aBaseWaterVB[0].y + 0.5f;
		m_aBrushVB[0].z = m_aBaseWaterVB[0].z;	m_aBrushVB[0].c = 0x00dd00dd;
		m_aBrushVB[1].x = m_aBaseWaterVB[1].x;	m_aBrushVB[1].y = m_aBrushVB[0].y;
		m_aBrushVB[1].z = m_aBaseWaterVB[1].z;	m_aBrushVB[1].c = 0x00dd00dd;
		m_aBrushVB[2].x = m_aBaseWaterVB[3].x;	m_aBrushVB[3].y = m_aBrushVB[0].y;
		m_aBrushVB[2].z = m_aBaseWaterVB[3].z;	m_aBrushVB[3].c = 0x00dd00dd;
		m_aBrushVB[3].x = m_aBaseWaterVB[2].x;	m_aBrushVB[2].y = m_aBrushVB[0].y;
		m_aBrushVB[3].z = m_aBaseWaterVB[2].z;	m_aBrushVB[2].c = 0x00dd00dd;
		m_aBrushVB[4].x = m_aBaseWaterVB[0].x;	m_aBrushVB[4].y = m_aBrushVB[0].y;
		m_aBrushVB[4].z = m_aBaseWaterVB[0].z;	m_aBrushVB[4].c = 0x00dd00dd;

		m_dwNeighWaterCount = 0;
		m_dwNeighWaveVertCount = 0;
	}
	else
	{
		// Neighbors
		nWidth = ( m_pTerrain->GetHMWidth() - 1 ) / 4;
		m_nTileEX -= 1;		m_nTileEZ -= 1;
		if( m_nTileSX < 0 ) m_nTileSX = 0;
		if( m_nTileSZ < 0 ) m_nTileSZ = 0;
		if( m_nTileEX >= nWidth ) m_nTileEX = nWidth - 1;
		if( m_nTileEZ >= nWidth ) m_nTileEZ = nWidth - 1;

		int				x, z, nVert, nSegment;
		D3DXVECTOR3		aTemp[27];
		CNKObjQuadNode*	pObjQuadNode;
		m_dwNeighWaterCount = 0;
		m_dwNeighWaveVertCount = 0;
		for( z = m_nTileSZ; z <= m_nTileEZ; z++ )
		{
			for( x = m_nTileSX; x <= m_nTileEX; x++ )
			{
				pObjQuadNode = CNKObjQuadNode::GetQuadNode( z, x );
				for( j = 1; j < 3; j++ )
				{
					if( pObjQuadNode->GetWaveWaterVertices( m_nWaterGroupID, j, aTemp, nSegment ) )
					{
						nVert = ( nSegment + 1 ) * 3;
						for( i = 0; i < nVert; i++ )
						{
							if( i % 3 != 1 )
								m_aNeighWaveVert[m_dwNeighWaveVertCount++] = aTemp[i];
						}

						for( i = 0; i < nSegment; i++ )
						{
							m_aNeighborWaterVert[m_dwNeighWaterCount++].p = aTemp[i*3];
							m_aNeighborWaterVert[m_dwNeighWaterCount++].p = aTemp[i*3 + 2];
							m_aNeighborWaterVert[m_dwNeighWaterCount++].p = aTemp[i*3];
							m_aNeighborWaterVert[m_dwNeighWaterCount++].p = aTemp[i*3 + 3];
							m_aNeighborWaterVert[m_dwNeighWaterCount++].p = aTemp[i*3 + 2];
							m_aNeighborWaterVert[m_dwNeighWaterCount++].p = aTemp[i*3 + 5];
						}
						m_aNeighborWaterVert[m_dwNeighWaterCount++].p = aTemp[i*3];
						m_aNeighborWaterVert[m_dwNeighWaterCount++].p = aTemp[i*3 + 2];
					}
				}
			}
		}

		if( m_nWaterEditType == 0 )
		{
			if( m_bWaveWaterModify == FALSE && m_bWaveWaterRotate == FALSE )
			{
				float	fOffsetX = m_vPickedPoint.x - m_vWaveWaterCenter.x;
				float	fOffsetZ = m_vPickedPoint.z - m_vWaveWaterCenter.z;
				
				for( int i = 0; i < 27; i++ )
				{
					m_aWaveWaterVB[i].x += fOffsetX;
					m_aWaveWaterVB[i].z += fOffsetZ;
				}
				m_vWaveWaterCenter.x = m_vPickedPoint.x;
				m_vWaveWaterCenter.z = m_vPickedPoint.z;
			}

			m_dwWaveIndexCount = m_nWaterSegment * 12;

			m_dwBrushVertNum = 0;
			for( int i = 0; i < m_nWaterSegment; i++ )
			{
				m_aBrushVB[m_dwBrushVertNum].p = m_aWaveWaterVB[i*3].p;
				m_aBrushVB[m_dwBrushVertNum++].c = 0x00dd00dd;
				m_aBrushVB[m_dwBrushVertNum].p = m_aWaveWaterVB[i*3 + 2].p;
				m_aBrushVB[m_dwBrushVertNum++].c = 0x00dd00dd;
				m_aBrushVB[m_dwBrushVertNum].p = m_aWaveWaterVB[i*3].p;
				m_aBrushVB[m_dwBrushVertNum++].c = 0x00dd00dd;
				m_aBrushVB[m_dwBrushVertNum].p = m_aWaveWaterVB[i*3 + 3].p;
				m_aBrushVB[m_dwBrushVertNum++].c = 0x00dd00dd;
				m_aBrushVB[m_dwBrushVertNum].p = m_aWaveWaterVB[i*3 + 2].p;
				m_aBrushVB[m_dwBrushVertNum++].c = 0x00dd00dd;
				m_aBrushVB[m_dwBrushVertNum].p = m_aWaveWaterVB[i*3 + 5].p;
				m_aBrushVB[m_dwBrushVertNum++].c = 0x00dd00dd;
			}
			m_aBrushVB[m_dwBrushVertNum].p = m_aWaveWaterVB[i*3].p;
			m_aBrushVB[m_dwBrushVertNum++].c = 0x00dd00dd;
			m_aBrushVB[m_dwBrushVertNum].p = m_aWaveWaterVB[i*3 + 2].p;
			m_aBrushVB[m_dwBrushVertNum++].c = 0x00dd00dd;
		}
		else
		{
			m_dwBrushVertNum = 0;
			m_dwWaveIndexCount = 0;
		}
	}
}


//-----------------------------------------------------------------------------
// Name : MakeLightControlMesh()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::MakeLightControlMesh()
{
	// Make a half sphere
	float	fDelta = NK_TWOPI / 144.0f;
	float	fX, fY, fZ, fLX, fLZ;
	float	fRadius = 100.0f * sinf( m_fSunTrace );
	int		nCount;

	nCount = 0;
	float fRadian;
	for( fRadian = 0.0f; fRadian < NK_TWOPI; fRadian += fDelta )
	{
		fX = sinf( fRadian );
		fZ = cosf( fRadian );
		fLX = fRadius * fX;
		fLZ = fRadius * fZ;
		fX *= 100.0f;
		fZ *= 100.0f;

		m_aLightCtrlXZVB[nCount].x = fX;
		m_aLightCtrlXZVB[nCount].y = 0.0f;
		m_aLightCtrlXZVB[nCount].z = fZ;
		m_aLightCtrlXZVB[nCount].c = m_dwLightCtrlColor;		
		m_aSunLatitudeVB[nCount].x = fLX;
		m_aSunLatitudeVB[nCount].y = m_fSunLatitude;
		m_aSunLatitudeVB[nCount].z = fLZ;
		m_aSunLatitudeVB[nCount].c = 0xffff80ff;

		nCount++;
	}
	m_aLightCtrlXZVB[nCount].x = m_aLightCtrlXZVB[0].x;
	m_aLightCtrlXZVB[nCount].y = m_aLightCtrlXZVB[0].y;
	m_aLightCtrlXZVB[nCount].z = m_aLightCtrlXZVB[0].z;
	m_aLightCtrlXZVB[nCount].c = m_dwLightCtrlColor;
	m_aSunLatitudeVB[nCount].x = m_aSunLatitudeVB[0].x;
	m_aSunLatitudeVB[nCount].y = m_aSunLatitudeVB[0].y;
	m_aSunLatitudeVB[nCount].z = m_aSunLatitudeVB[0].z;
	m_aSunLatitudeVB[nCount].c = 0xffff80ff;

	nCount = 0;
	for( fRadian = 0.0f; fRadian <= NK_PI; fRadian += fDelta )
	{
		fY = 100.0f * sinf( fRadian );
		fX = 100.0f * cosf( fRadian );
		fZ = fX;

		m_aLightCtrlXYVB[nCount].x = fX;
		m_aLightCtrlXYVB[nCount].y = fY;
		m_aLightCtrlXYVB[nCount].z = 0.0f;
		m_aLightCtrlXYVB[nCount].c = m_dwLightCtrlColor;

		m_aLightCtrlZYVB[nCount].x = 0.0f;
		m_aLightCtrlZYVB[nCount].y = fY;
		m_aLightCtrlZYVB[nCount].z = fZ;
		m_aLightCtrlZYVB[nCount].c = m_dwLightCtrlColor;
		nCount++;
	}

	D3DXQUATERNION	quat;
	NKMATRIX3		mat;
	NKQuaternionFromYawPitch( quat, 0.0f, -m_fSunTrace );
	NKQuaternionToMatrix( mat, quat );
	int i = 0;
	for( i = 0; i < 73; i++ )
	{
		NKVectorMatrixMultiply( m_aSunTraceVB[i].p, m_aLightCtrlXYVB[i].p, mat );
		m_aSunTraceVB[i].c = 0xfff0f000;
	}

	// Make directions
	D3DXVECTOR3	vTemp;
	for( i = 0; i < 6; i++ )
	{
		m_aLightCtrlExtraVB[i*2].x = 0.0f;
		m_aLightCtrlExtraVB[i*2].y = 0.0f;
		m_aLightCtrlExtraVB[i*2].z = 0.0f;
		m_aLightCtrlExtraVB[i*2].c = 0xff00dd00;
		D3DXVec3Scale( &vTemp, &m_aLightDirection[i], -100.0f );
		m_aLightCtrlExtraVB[i*2+1].x = vTemp.x;
		m_aLightCtrlExtraVB[i*2+1].y = vTemp.y;
		m_aLightCtrlExtraVB[i*2+1].z = vTemp.z;
		m_aLightCtrlExtraVB[i*2+1].c = 0xff00dd00;
	}
	m_aLightCtrlExtraVB[m_nLightTimeID*2].c = 0xff0000ff;
	m_aLightCtrlExtraVB[m_nLightTimeID*2+1].c = 0xff0000ff;

	// Make axes
	m_aLightCtrlExtraVB[12].x = -100.0f;			// X
	m_aLightCtrlExtraVB[12].y = 0.0f;
	m_aLightCtrlExtraVB[12].z = 0.0f;
	m_aLightCtrlExtraVB[12].c = m_dwLightCtrlColor;
	m_aLightCtrlExtraVB[13].x = 100.0f;
	m_aLightCtrlExtraVB[13].y = 0.0f;
	m_aLightCtrlExtraVB[13].z = 0.0f;
	m_aLightCtrlExtraVB[13].c = m_dwLightCtrlColor;
	m_aLightCtrlExtraVB[14].x = 0.0f;			// Y
	m_aLightCtrlExtraVB[14].y = 0.0f;
	m_aLightCtrlExtraVB[14].z = 0.0f;
	m_aLightCtrlExtraVB[14].c = m_dwLightCtrlColor;
	m_aLightCtrlExtraVB[15].x = 0.0f;
	m_aLightCtrlExtraVB[15].y = 100.0f;
	m_aLightCtrlExtraVB[15].z = 0.0f;
	m_aLightCtrlExtraVB[15].c = m_dwLightCtrlColor;
	m_aLightCtrlExtraVB[16].x = 0.0f;			// Z
	m_aLightCtrlExtraVB[16].y = 0.0f;
	m_aLightCtrlExtraVB[16].z = 100.0f;
	m_aLightCtrlExtraVB[16].c = m_dwLightCtrlColor;
	m_aLightCtrlExtraVB[17].x = 0.0f;
	m_aLightCtrlExtraVB[17].y = 0.0f;
	m_aLightCtrlExtraVB[17].z = -100.0f;
	m_aLightCtrlExtraVB[17].c = m_dwLightCtrlColor;
}


//-----------------------------------------------------------------------------
// Name : RenderDecoObjs()
// Desc : Port this code to CUIMgr::RenderDecoObjs() in laghaim source
//-----------------------------------------------------------------------------
void CMyNKApp::RenderDecoObjs()
{
	// Clear the list of alpha decoobjs
	g_lstAlphaDeco.clear();
	g_lstFarDeco.clear();
	g_lstLastDeco.clear();

	// Check if a decoobj exists
	if( CNKObjQuadNode::GetOpenDeco() )
	{
		if( !CNKObjQuadNode::GetOpenDeco()->IsInside( m_vTarget.x, m_vTarget.z ) )
			CNKObjQuadNode::SetOpenDeco( NULL );
	}

	// Render decoobjs
	CNKObjQuadNode::PrepareRender();
	g_pObjTree->Render( VFCULL_OVERLAP );
}


//-----------------------------------------------------------------------------
// Name : RenderAlphaObjs()
// Desc : Port this code to CUIMgr::RenderAlphaObjs() in laghaim source
//-----------------------------------------------------------------------------
void CMyNKApp::RenderAlphaObjs()
{
	list<CDecoObj*>::iterator	iterDeco;
	CDecoObj*					pDecoObj;
	DWORD						dwZWriteEnable;

	m_pd3dDevice->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWriteEnable );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );

	for( iterDeco = g_lstAlphaDeco.begin(); iterDeco != g_lstAlphaDeco.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		pDecoObj->Render( m_pd3dDevice, FALSE, DECO_ALPHA_SEE );
	}

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWriteEnable );

	for( iterDeco = g_lstLastDeco.begin(); iterDeco != g_lstLastDeco.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		pDecoObj->Render( m_pd3dDevice, FALSE );
	}

	for( iterDeco = g_lstFarDeco.begin(); iterDeco != g_lstFarDeco.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		pDecoObj->Render( m_pd3dDevice, FALSE, DECO_ALPHA_FAR );
	}
}


//-----------------------------------------------------------------------------
// Name : RenderInterfaces()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::RenderInterfaces()
{
	static D3DMATRIX	matWorld;
	static D3DXVECTOR3	vOrigin, vDir;

	D3DUtil_SetIdentityMatrix( matWorld );

	// Get texture stage state
	DWORD	dwColorOp0, dwColorArg1, dwColorArg2, dwColorOp1, dwAlphaOp0, dwAlphaArg1;
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &dwColorOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColorArg1 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG2, &dwColorArg2 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwAlphaOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg1 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLOROP, &dwColorOp1 );

	// Set texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );

	// Render a brush  /////////////////////////////////////////////////////////
	if( m_dwBrushVertNum > 0 )
	{
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, FALSE );
		
		if( m_nWorkingState == TEXTURE_PAINT ||	m_nWorkingState == VERTEX_EDIT ||
			( m_nWorkingState == LIGHT_EDIT && !m_bDirectLightControl ) )
		{
			m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, m_aBrushVB, m_dwBrushVertNum, 0 );
		}
		else if( m_nWorkingState == ATTRIBUTE_EDIT )
		{
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, NKVERTEXL::FVF, m_aBrushVB, m_dwBrushVertNum, 0 );
		}
		else if( m_nWorkingState == ATMOSPHERE_EDIT )
		{
			if( m_bShowWaterGuideLine )
			{
				m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, NKVERTEXL::FVF, m_aWaterGuideVB, 48, 0 );
				if( m_dwNeighWaterCount > 0 )
					m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, NKVERTEXL::FVF, m_aNeighborWaterVert,
													m_dwNeighWaterCount, 0 );
			}
			
			if( m_nWaterType == 0 )
				m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, m_aBrushVB, m_dwBrushVertNum, 0 );
			else
				m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, NKVERTEXL::FVF, m_aBrushVB, m_dwBrushVertNum, 0 );
		}

		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
	}

	if( m_lstSelDecoObjs.size() > 0 )
	{
		// Render bounding boxes of selected decoobjs /////////////////////////
		m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, NKVERTEXL::FVF, m_aDecoBoxVB, m_dwBoxVertNum, 0 );

		// Get render state
		DWORD	dwCullMode;
		m_pd3dDevice->GetRenderState( D3DRENDERSTATE_CULLMODE, &dwCullMode );

		// Set render state
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE );

		if( m_bShowNodeBound && m_lstSelDecoObjs.size() == 1 )
		{
			// Get render state
			DWORD	dwSrcBlend, dwDestBlend;
			m_pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcBlend );
			m_pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestBlend );

			// Set render state
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

			// Set texture stage state
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );

			// Render boundary planes of a node including a selected decoobj //////
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, NKVERTEXL::FVF, m_aNodeBoundVB, 10, 0 );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );

			// Restore render state
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcBlend );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestBlend );
		}

		// Set texture stage state
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// Render gizmo ///////////////////////////////////////////////////////
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, FALSE );
		// Set a world transform matrix
		vOrigin = m_pCamera->GetLocation();
		D3DXVec3Subtract( &vDir, &m_vGizmoPosition, &vOrigin );
		D3DXVec3Normalize( &vDir, &vDir );
		D3DXVec3Scale( &vDir, &vDir, 300 );
		D3DXVec3Add( &m_vGizmoRelativePos, &vOrigin, &vDir );
		matWorld._41 = m_vGizmoRelativePos.x;
		matWorld._42 = m_vGizmoRelativePos.y;
		matWorld._43 = m_vGizmoRelativePos.z;
		m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		//////////////////////////////////////////////////////////////////////////////////
		// 수정자 : 이원석
		// 수정일 : 05-02-23
		// 수정내용 : 로컬좌표축 표시기능의 추가.
		if( m_bShowLocalAxis ) // 여기서 로컬 좌표축 표시 모드인지를 체크.
		{
			// 상대 좌표축 체크 모드이면....
			// 처음 선택된 녀석의 dir을 기준으로 축을 회전시킨다.
			list<CDecoObj*>::iterator	iterDeco = m_lstSelDecoObjs.begin();
			CDecoObj*   pDecoObj = NULL;
			pDecoObj = *iterDeco;

			if( pDecoObj ) // 사실 굳이 체크안해줘도 될듯.
			{
				D3DMATRIX	matGizmoWorld;
				D3DUtil_SetIdentityMatrix( matGizmoWorld );

				float TempRad =  pDecoObj->m_dir - g_2_PI; // 이렇게 해줘야 정상 방향으로 따라간다.

				Nk3DUtil_SetRotateCCWYMatrix( matGizmoWorld, TempRad );
			
				matGizmoWorld._41 = m_vGizmoRelativePos.x;
				matGizmoWorld._42 = m_vGizmoRelativePos.y;
				matGizmoWorld._43 = m_vGizmoRelativePos.z;
		
				m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matGizmoWorld );
			}
		}

		if( m_nObjEditType == 2 )
		{
			// Gizmo Axes
			m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, NKVERTEXL::FVF, m_aGizmoAxesVB, 10, 0 );
			// Gizmo x tail
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, NKVERTEXL::FVF, m_aGizmoXTailVB, 6, 0 );			
			// Gizmo y tail
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, NKVERTEXL::FVF, m_aGizmoYTailVB, 6, 0 );
			// Gizmo z tail
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, NKVERTEXL::FVF, m_aGizmoZTailVB, 6, 0 );
		}
		else
		{
			// Gizmo Axes
			m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, NKVERTEXL::FVF, m_aGizmoAxesVB, 6, 0 );
			// Gizmo y tail
			if( m_nObjEditType == 3 )
				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, NKVERTEXL::FVF, m_aGizmoYTailVB, 6, 0 );
		}
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );

		// Restore render state
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_CULLMODE, dwCullMode );

		// 만약 로컬 좌표축 표시 모드였다면...
		if( m_bShowLocalAxis ) // 여기서 로컬 좌표축 표시 모드인지를 체크.
		{
			// 월드 행렬을 다시 돌려놓아준다.
			m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		}
	}

	// Render water plane /////////////////////////////////////////////////////
	if( m_nWorkingState == ATMOSPHERE_EDIT && m_nWaterGroupID != -1 )
	{
		// Get render state
		DWORD	dwSrcBlend, dwDestBlend;
		m_pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcBlend );
		m_pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestBlend );

		// Set texture stage state
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );

		// Set render state
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

		if( m_nWaterType == 0 )
		{
			DWORD	dwDiffuse = CNKObjQuadNode::GetBaseWaterAlpha();
			m_aBaseWaterVB[0].c = dwDiffuse;	m_aBaseWaterVB[1].c = dwDiffuse;
			m_aBaseWaterVB[2].c = dwDiffuse;	m_aBaseWaterVB[3].c = dwDiffuse;
			
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			
			m_matWaterTrans._31 = CNKObjQuadNode::GetBaseWaterMove();
			m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &m_matWaterTrans );

			m_pd3dDevice->SetTexture( 0, m_pDecoTexture->GetSurfaceByIdx( CNKObjQuadNode::GetBaseWaterTexID() ) );
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, NKVERTEXLT::FVF, m_aBaseWaterVB, 4, 0 );
		}
		else
		{
			if( m_dwWaveIndexCount > 0 )
			{
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

				m_matWaterTrans._31 = CNKObjQuadNode::GetWaveWaterMove1();
				m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &m_matWaterTrans );

				m_pd3dDevice->SetTexture( 0, m_pDecoTexture->GetSurfaceByIdx( CNKObjQuadNode::GetWaveWaterTexID1() ) );
				m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, NKVERTEXLT::FVF,
													m_aWaveWaterVB, 27,	m_aWaveWaterIB, m_dwWaveIndexCount, 0 );

				m_matWaterTrans._31 = CNKObjQuadNode::GetWaveWaterMove2();
				m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &m_matWaterTrans );

				m_pd3dDevice->SetTexture( 0, m_pDecoTexture->GetSurfaceByIdx( CNKObjQuadNode::GetWaveWaterTexID2() ) );
				m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, NKVERTEXLT::FVF,
													m_aWaveWaterVB, 27,	m_aWaveWaterIB, m_dwWaveIndexCount, 0 );

				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			}
		}

		// Restore texture stage state
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

		// Restore render state
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcBlend );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestBlend );
	}


	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, FALSE );
	// Render axes & a director ///////////////////////////////////////////////
	m_pCamera->GetPickingRay( vOrigin, vDir, 45, 40 );
	D3DXVec3Normalize( &vDir, &vDir );
	D3DXVec3Scale( &vDir, &vDir, 1000 );
	D3DXVec3Add( &vOrigin, &vOrigin, &vDir );
	matWorld._41 = vOrigin.x;
	matWorld._42 = vOrigin.y;
	matWorld._43 = vOrigin.z;
	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, NKVERTEXL::FVF, m_aAxesVB, 12, 0 );

	// Render a direction controller of a directional light ///////////////////
	if( m_nWorkingState == LIGHT_EDIT && m_bDirectLightControl )
	{
		vOrigin = m_pCamera->GetLocation();
		vDir = m_pCamera->GetDirection();
		D3DXVec3Normalize( &vDir, &vDir );
		D3DXVec3Scale( &vDir, &vDir, 500 );
		D3DXVec3Add( &m_vLightCtrlRelativePos, &vOrigin, &vDir );
		matWorld._41 = m_vLightCtrlRelativePos.x;
		matWorld._42 = m_vLightCtrlRelativePos.y;
		matWorld._43 = m_vLightCtrlRelativePos.z;
		m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, m_aLightCtrlXZVB, 145, 0 );
		m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, m_aLightCtrlXYVB, 73, 0 );
		m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, m_aLightCtrlZYVB, 73, 0 );
		m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, m_aSunTraceVB, 73, 0 );
		m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, NKVERTEXL::FVF, m_aSunLatitudeVB, 145, 0 );		
		m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, NKVERTEXL::FVF, m_aLightCtrlExtraVB, 18, 0 );
	}

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );

	// Restore texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, dwColorOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, dwColorArg2 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwAlphaOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwAlphaArg1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, dwColorOp1 );
}


//-----------------------------------------------------------------------------
// Name : ToggleGrid()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ToggleGrid()
{
	if( m_nWorkingState == OBJECT_EDIT )
	{
		if( m_nGridSize == -1 )
			m_nGridSize = 2;
		else if( m_nGridSize == 2 )
			m_nGridSize = -1;
	}
	else
	{
		m_nGridSize++;
		if( m_nGridSize > 2 )
			m_nGridSize = -1;
	}
}


//-----------------------------------------------------------------------------
// Name : PlaceDecoObj()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::PlaceDecoObj()
{
	if( m_nCurDecoObj >= 0 )
	{
		float	fDir = 0.0f;
		if( m_bPlaceRandomRotation )
			fDir = 360.0f * ( (float)rand() / RAND_MAX );

		CDecoObj*	pDecoObj = new CDecoObj();
		pDecoObj->m_wx = m_vPickedPoint.x;
		pDecoObj->m_wz = m_vPickedPoint.z;
		pDecoObj->m_wy = m_pTerrain->GetHeights( m_vPickedPoint.z, m_vPickedPoint.x );
		pDecoObj->m_dir = fDir;

		pDecoObj->m_pLolo = g_pDecoObjSet->m_vecLoloSet[m_nCurDecoObj];
		pDecoObj->m_nLoloId = m_nCurDecoObj;

		pDecoObj->CalBoundBox();

		int	nX, nZ;
		nX = m_vPickedPoint.x / CNKObjQuadNode::GetCellSize();
		nZ = m_vPickedPoint.z / CNKObjQuadNode::GetCellSize();
		CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );

		if( !pObjQuadNode->IsDecoObjInNode( pDecoObj ) )
		{
			pObjQuadNode = NULL;
			float	fArea = FLT_MAX;
			g_pObjTree->FindDecoObjFittedNode( pDecoObj, &pObjQuadNode, fArea );
		}

		pObjQuadNode->AddDecoObj( pDecoObj );
		g_pDecoObjSet->m_vecRefCount[m_nCurDecoObj]++;

		if( m_lstSelDecoObjs.size() > 0 )
			m_lstSelDecoObjs.erase( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end() );

		m_lstSelDecoObjs.push_back( pDecoObj );

		MakeDecoBoxMesh();
		MakeNodeBoundMesh();
		CalcGizmoPosition();

		UpdateObjEditParameters( pDecoObj );
		UpdateObjEditReferenceCount();
	}
}


//-----------------------------------------------------------------------------
// Name : RemoveDecoObjsFromTree()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::RemoveDecoObjsFromTree()
{
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;

	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;

		int	nX, nZ;
		nX = pDecoObj->m_wx / CNKObjQuadNode::GetCellSize();
		nZ = pDecoObj->m_wz / CNKObjQuadNode::GetCellSize();
		CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );
		
		if( !pObjQuadNode->HasThisDecoObj( pDecoObj ) )
		{
			bool	bFind = false;
			pObjQuadNode = NULL;
			g_pObjTree->FindQuadNode( pDecoObj, &pObjQuadNode, bFind );
		}

		// [2007/10/30 theodoric] 오브젝트가 삭제되면 undo와 관련된 내용은 모두 삭제해준다
		m_UndoObj.DeleteUndo(pDecoObj);
		g_pDecoObjSet->m_vecRefCount[pDecoObj->m_nLoloId]--;
		pObjQuadNode->RemoveDecoObj( pDecoObj );

		m_bModify[2] = TRUE;
	}

	m_lstSelDecoObjs.erase( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end() );

	MakeDecoBoxMesh();
	CalcGizmoPosition();
}


//-----------------------------------------------------------------------------
// Name : MakeDecoBoxMesh()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::MakeDecoBoxMesh()
{

	if( m_lstSelDecoObjs.size() <= 0 )
		return;


	m_dwBoxVertNum = 0;

	int							i;
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	int							debug = 0;
	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		debug++;
		pDecoObj = *iterDeco;

		for( i = 0; i < 3; i++ )
		{
			m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[i];
			m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
			m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[i+1];
			m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
			m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[i+4];
			m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
			m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[i+5];
			m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
			m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[i];
			m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
			m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[i+4];
			m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
		}
		m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[0];
		m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
		m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[3];
		m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
		m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[4];
		m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
		m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[7];
		m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
		m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[3];
		m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
		m_aDecoBoxVB[m_dwBoxVertNum].p = pDecoObj->m_vBoundBox[7];
		m_aDecoBoxVB[m_dwBoxVertNum++].c = 0xffffffff;
	}
}


//-----------------------------------------------------------------------------
// Name : MakeNodeBoundMesh()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::MakeNodeBoundMesh()
{
	if( m_lstSelDecoObjs.size() != 1 )
		return ;

	list<CDecoObj*>::iterator	iterDeco = m_lstSelDecoObjs.begin();
	CDecoObj*	pDecoObj = *iterDeco;
	
	int	nX, nZ;
	nX = pDecoObj->m_wx / CNKObjQuadNode::GetCellSize();
	nZ = pDecoObj->m_wz / CNKObjQuadNode::GetCellSize();
	CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );
	
	if( !pObjQuadNode->HasThisDecoObj( pDecoObj ) )
	{
		bool	bFind = false;
		pObjQuadNode = NULL;
		g_pObjTree->FindQuadNode( pDecoObj, &pObjQuadNode, bFind );
	}

	D3DXVECTOR3	vMin = pObjQuadNode->GetMin();
	D3DXVECTOR3	vMax = pObjQuadNode->GetMax();

	m_aNodeBoundVB[0].p = D3DXVECTOR3( vMin.x, vMax.y, vMin.z );
	m_aNodeBoundVB[1].p = vMin;
	m_aNodeBoundVB[2].p = D3DXVECTOR3( vMin.x, vMax.y, vMax.z );
	m_aNodeBoundVB[3].p = D3DXVECTOR3( vMin.x, vMin.y, vMax.z );
	m_aNodeBoundVB[4].p = vMax;
	m_aNodeBoundVB[5].p = D3DXVECTOR3( vMax.x, vMin.y, vMax.z );
	m_aNodeBoundVB[6].p = D3DXVECTOR3( vMax.x, vMax.y, vMin.z );
	m_aNodeBoundVB[7].p = D3DXVECTOR3( vMax.x, vMin.y, vMin.z );
	
	for( int i = 0; i < 8; i++ )
		m_aNodeBoundVB[i].c = m_dwNodeBoundColor;

	m_aNodeBoundVB[8] = m_aNodeBoundVB[0];
	m_aNodeBoundVB[9] = m_aNodeBoundVB[1];
}


//-----------------------------------------------------------------------------
// Name : UpdateObjEditParameters()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::UpdateObjEditParameters( CDecoObj* pDecoObj )
{
	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndToolCtrlBar.m_pageObjEdit.UpdateParameters( pDecoObj );
}


//-----------------------------------------------------------------------------
// Name : UpdateObjEditReferenceCount()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::UpdateObjEditReferenceCount()
{
	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndToolCtrlBar.m_pageObjEdit.UpdateReferenceCount( m_nCurDecoObj );
}


//-----------------------------------------------------------------------------
// Name : CheckPickingAxes()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::CheckPickingAxes()
{
	D3DXVECTOR3	vMin, vMax;
	float		fOldT, fT;
	int			i;

	// Check which axis is picked
	fOldT = FLT_MAX;
	m_nSelAxis = 0;

	for( i = 0; i < 5; i++ )
	{
		if( m_nObjEditType == 3 && i != 1 )
			continue;

		D3DXVec3Add( &vMin, &(m_aGizmoAABB[i*2]), &m_vGizmoRelativePos );
		D3DXVec3Add( &vMax, &(m_aGizmoAABB[i*2+1]), &m_vGizmoRelativePos );

		if( NKIntersectRayAABB( m_vPickOrigin, m_vPickDirection, vMin, vMax, &fT ) )
		{
			if( fT < fOldT )
			{
				fOldT = fT;
				m_nSelAxis = i + 1;
			}
		}
	}

	if( m_nSelAxis > 4 ) m_nSelAxis = 4;
}


//-----------------------------------------------------------------------------
// Name : SetHighlightAxis()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetHighlightAxis()
{
	m_aGizmoAxesVB[0].c = 0xff0000ff;
	m_aGizmoAxesVB[1].c = 0xff0000ff;
	m_aGizmoAxesVB[2].c = 0xff00ff00;
	m_aGizmoAxesVB[3].c = 0xff00ff00;
	m_aGizmoAxesVB[4].c = 0xffff0000;
	m_aGizmoAxesVB[5].c = 0xffff0000;
	m_aGizmoAxesVB[6].c = 0xffff00ff;
	m_aGizmoAxesVB[7].c = 0xffff00ff;
	m_aGizmoAxesVB[8].c = 0xffff00ff;
	m_aGizmoAxesVB[9].c = 0xffff00ff;

	switch( m_nSelAxis )
	{
	case 1:	// x
		m_aGizmoAxesVB[0].c = 0xffffff00;
		m_aGizmoAxesVB[1].c = 0xffffff00;
		break;

	case 2:	// y
		m_aGizmoAxesVB[2].c = 0xffffff00;
		m_aGizmoAxesVB[3].c = 0xffffff00;
		break;

	case 3:	// z
		m_aGizmoAxesVB[4].c = 0xffffff00;
		m_aGizmoAxesVB[5].c = 0xffffff00;
		break;

	case 4:	// xz
		m_aGizmoAxesVB[6].c = 0xffffff00;
		m_aGizmoAxesVB[7].c = 0xffffff00;
		m_aGizmoAxesVB[8].c = 0xffffff00;
		m_aGizmoAxesVB[9].c = 0xffffff00;
		break;
	}
}


//-----------------------------------------------------------------------------
// Name : InitWaterMesh()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::InitWaterMesh()
{
	m_aBaseWaterVB[0].u = 0.0f;		m_aBaseWaterVB[0].v = 0.0f;
	m_aBaseWaterVB[1].u = 0.5f;		m_aBaseWaterVB[1].v = 0.0f;
	m_aBaseWaterVB[2].u = 0.0f;		m_aBaseWaterVB[2].v = 1.0f;
	m_aBaseWaterVB[3].u = 0.5f;		m_aBaseWaterVB[3].v = 1.0f;

	int	nWidth = CNKObjQuadNode::GetWaveWaterSize();
	int	nHalfWidth = nWidth / 2;
	int i = 0;
	for( i = 0; i < 9; i++ )
	{
		m_aWaveWaterVB[i*3].x = 0.0f;
		m_aWaveWaterVB[i*3 + 1].x = nHalfWidth;
		m_aWaveWaterVB[i*3 + 2].x = nWidth;
		m_aWaveWaterVB[i*3].y = 0.0f;
		m_aWaveWaterVB[i*3 + 1].y = 0.0f;
		m_aWaveWaterVB[i*3 + 2].y = 0.0f;
		m_aWaveWaterVB[i*3].z = -( nWidth * i );
		m_aWaveWaterVB[i*3 + 1].z = m_aWaveWaterVB[i*3].z;
		m_aWaveWaterVB[i*3 + 2].z = m_aWaveWaterVB[i*3].z;

		m_aWaveWaterVB[i*3].c = 0xff000000;
		m_aWaveWaterVB[i*3 + 1].c = 0xffffffff;
		m_aWaveWaterVB[i*3 + 2].c = 0xff000000;

		m_aWaveWaterVB[i*3].u = 0.0f;
		m_aWaveWaterVB[i*3 + 1].u = 0.5f;
		m_aWaveWaterVB[i*3 + 2].u = 1.0f;
		m_aWaveWaterVB[i*3].v = i;
		m_aWaveWaterVB[i*3 + 1].v = i;
		m_aWaveWaterVB[i*3 + 2].v = i;
	}

	m_dwWaveIndexCount = 0;
	for( WORD wVBIndex = 0; wVBIndex < 24; wVBIndex += 3 )
	{
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 1;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 3;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 3;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 1;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 4;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 1;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 2;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 4;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 4;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 2;
		m_aWaveWaterIB[m_dwWaveIndexCount++] = wVBIndex + 5;
	}

	for( i = 0; i < 48; i++ )
		m_aWaterGuideVB[i].c = 0xff00dddd;

	for( i = 0; i < 800; i++ )
		m_aNeighborWaterVert[i].c = 0xff8800ee;
}


//-----------------------------------------------------------------------------
// Name : InitAxes()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::InitAxes()
{
	// Director
	m_aAxesVB[0].p = D3DXVECTOR3( 0.0f, 0.0f, -15.0f );
	m_aAxesVB[0].c = 0xffffff00;
	m_aAxesVB[1].p = D3DXVECTOR3( 0.0f, 0.0f, 15.0f );
	m_aAxesVB[1].c = 0xffffff00;
	m_aAxesVB[2].p = D3DXVECTOR3( 0.0f, 0.0f, 15.0f );
	m_aAxesVB[2].c = 0xffffff00;
	m_aAxesVB[3].p = D3DXVECTOR3( -15.0f, 0.0f, 0.0f );
	m_aAxesVB[3].c = 0xffffff00;
	m_aAxesVB[4].p = D3DXVECTOR3( -15.0f, 0.0f, 0.0f );
	m_aAxesVB[4].c = 0xffffff00;
	m_aAxesVB[5].p = D3DXVECTOR3( 15.0f, 0.0f, 0.0f );
	m_aAxesVB[5].c = 0xffffff00;

	// Axes
	m_aAxesVB[6].p = D3DXVECTOR3( 15.0f, 0.0f, 0.0f );
	m_aAxesVB[6].c = 0xffff0000;
	m_aAxesVB[7].p = D3DXVECTOR3( 30.0f, 0.0f, 0.0f );
	m_aAxesVB[7].c = 0xffff0000;
	m_aAxesVB[8].p = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_aAxesVB[8].c = 0xff00ff00;
	m_aAxesVB[9].p = D3DXVECTOR3( 0.0f, 25.0f, 0.0f );
	m_aAxesVB[9].c = 0xff00ff00;
	m_aAxesVB[10].p = D3DXVECTOR3( 0.0f, 0.0f, 15.0f );
	m_aAxesVB[10].c = 0xff0000ff;
	m_aAxesVB[11].p = D3DXVECTOR3( 0.0f, 0.0f, 30.0f );
	m_aAxesVB[11].c = 0xff0000ff;

}


//-----------------------------------------------------------------------------
// Name : InitGizmo()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::InitGizmo()
{
	// Set gizmo mesh
	m_aGizmoAxesVB[0].p = D3DXVECTOR3( 5.0f,	0.0f,	0.0f );		// X axis
	m_aGizmoAxesVB[0].c = 0xffff0000;
	m_aGizmoAxesVB[1].p = D3DXVECTOR3( 25.0f,	0.0f,	0.0f );
	m_aGizmoAxesVB[1].c = 0xffff0000;
	m_aGizmoAxesVB[2].p = D3DXVECTOR3( 0.0f,	5.0f,	0.0f );		// Y axis
	m_aGizmoAxesVB[2].c = 0xff00ff00;
	m_aGizmoAxesVB[3].p = D3DXVECTOR3( 0.0f,	25.0f,	0.0f );
	m_aGizmoAxesVB[3].c = 0xff00ff00;
	m_aGizmoAxesVB[4].p = D3DXVECTOR3( 0.0f,	0.0f,	5.0f );		// Z axis
	m_aGizmoAxesVB[4].c = 0xff0000ff;
	m_aGizmoAxesVB[5].p = D3DXVECTOR3( 0.0f,	0.0f,	25.0f );
	m_aGizmoAxesVB[5].c = 0xff0000ff;
	m_aGizmoAxesVB[6].p = D3DXVECTOR3( 10.0f,	0.0f,	0.0f );		// XZ plane
	m_aGizmoAxesVB[6].c = 0xffff00ff;
	m_aGizmoAxesVB[7].p = D3DXVECTOR3( 10.0f,	0.0f,	10.0f );
	m_aGizmoAxesVB[7].c = 0xffff00ff;
	m_aGizmoAxesVB[8].p = D3DXVECTOR3( 0.0f,	0.0f,	10.0f );
	m_aGizmoAxesVB[8].c = 0xffff00ff;
	m_aGizmoAxesVB[9].p = D3DXVECTOR3( 10.0f,	0.0f,	10.0f );
	m_aGizmoAxesVB[9].c = 0xffff00ff;

	m_aGizmoXTailVB[0].p = D3DXVECTOR3( 30.0f,	0.0f,	0.0f );		// X tail
	m_aGizmoXTailVB[0].c = 0xffff0000;
	m_aGizmoXTailVB[1].p = D3DXVECTOR3( 25.0f,	1.0f,	0.0f );
	m_aGizmoXTailVB[1].c = 0xffff0000;
	m_aGizmoXTailVB[2].p = D3DXVECTOR3( 25.0f,	0.0f,	1.0f );
	m_aGizmoXTailVB[2].c = 0xffff0000;
	m_aGizmoXTailVB[3].p = D3DXVECTOR3( 25.0f,	-1.0f,	0.0f );
	m_aGizmoXTailVB[3].c = 0xffff0000;
	m_aGizmoXTailVB[4].p = D3DXVECTOR3( 25.0f,	0.0f,	-1.0f );
	m_aGizmoXTailVB[4].c = 0xffff0000;
	m_aGizmoXTailVB[5].p = D3DXVECTOR3( 25.0f,	1.0f,	0.0f );
	m_aGizmoXTailVB[5].c = 0xffff0000;

	m_aGizmoYTailVB[0].p = D3DXVECTOR3( 0.0f,	30.0f,	0.0f );		// Y tail
	m_aGizmoYTailVB[0].c = 0xff00ff00;
	m_aGizmoYTailVB[1].p = D3DXVECTOR3( 1.0f,	25.0f,	0.0f );
	m_aGizmoYTailVB[1].c = 0xff00ff00;
	m_aGizmoYTailVB[2].p = D3DXVECTOR3( 0.0f,	25.0f,	1.0f );
	m_aGizmoYTailVB[2].c = 0xff00ff00;
	m_aGizmoYTailVB[3].p = D3DXVECTOR3( -1.0f,	25.0f,	0.0f );
	m_aGizmoYTailVB[3].c = 0xff00ff00;
	m_aGizmoYTailVB[4].p = D3DXVECTOR3( 0.0f,	25.0f,	-1.0f );
	m_aGizmoYTailVB[4].c = 0xff00ff00;
	m_aGizmoYTailVB[5].p = D3DXVECTOR3( 1.0f,	25.0f,	0.0f );
	m_aGizmoYTailVB[5].c = 0xff00ff00;

	m_aGizmoZTailVB[0].p = D3DXVECTOR3( 0.0f,	0.0f,	30.0f );	// Z tail
	m_aGizmoZTailVB[0].c = 0xff0000ff;
	m_aGizmoZTailVB[1].p = D3DXVECTOR3( 1.0f,	0.0f,	25.0f );
	m_aGizmoZTailVB[1].c = 0xff0000ff;
	m_aGizmoZTailVB[2].p = D3DXVECTOR3( 0.0f,	1.0f,	25.0f );
	m_aGizmoZTailVB[2].c = 0xff0000ff;
	m_aGizmoZTailVB[3].p = D3DXVECTOR3( -1.0f,	0.0f,	25.0f );
	m_aGizmoZTailVB[3].c = 0xff0000ff;
	m_aGizmoZTailVB[4].p = D3DXVECTOR3( 0.0f,	-1.0f,	25.0f );
	m_aGizmoZTailVB[4].c = 0xff0000ff;
	m_aGizmoZTailVB[5].p = D3DXVECTOR3( 1.0f,	0.0f,	25.0f );
	m_aGizmoZTailVB[5].c = 0xff0000ff;

	// Set gizmo bounding boxes
	// X axis
	m_aGizmoAABB[0] = D3DXVECTOR3( m_aGizmoAxesVB[0].p.x, m_aGizmoAxesVB[0].y - 1.0f, m_aGizmoAxesVB[0].z - 1.0f );
	m_aGizmoAABB[1] = D3DXVECTOR3( m_aGizmoAxesVB[1].p.x, m_aGizmoAxesVB[1].y + 1.0f, m_aGizmoAxesVB[1].z + 1.0f );
	// Y axis
	m_aGizmoAABB[2] = D3DXVECTOR3( m_aGizmoAxesVB[2].p.x - 1.0f, m_aGizmoAxesVB[2].y, m_aGizmoAxesVB[2].z - 1.0f );
	m_aGizmoAABB[3] = D3DXVECTOR3( m_aGizmoAxesVB[3].p.x + 1.0f, m_aGizmoAxesVB[3].y, m_aGizmoAxesVB[3].z + 1.0f );
	// Z axis
	m_aGizmoAABB[4] = D3DXVECTOR3( m_aGizmoAxesVB[4].p.x - 1.0f, m_aGizmoAxesVB[4].y - 1.0f, m_aGizmoAxesVB[4].z );
	m_aGizmoAABB[5] = D3DXVECTOR3( m_aGizmoAxesVB[5].p.x + 1.0f, m_aGizmoAxesVB[5].y + 1.0f, m_aGizmoAxesVB[5].z );
	// XZ plane
	m_aGizmoAABB[6] = D3DXVECTOR3( m_aGizmoAxesVB[6].p.x - 1.0f, m_aGizmoAxesVB[6].y - 1.0f, m_aGizmoAxesVB[6].z );
	m_aGizmoAABB[7] = D3DXVECTOR3( m_aGizmoAxesVB[7].p.x + 1.0f, m_aGizmoAxesVB[7].y + 1.0f, m_aGizmoAxesVB[7].z );
	m_aGizmoAABB[8]  = D3DXVECTOR3( m_aGizmoAxesVB[8].p.x, m_aGizmoAxesVB[8].y - 1.0f, m_aGizmoAxesVB[8].z - 1.0f );
	m_aGizmoAABB[9]  = D3DXVECTOR3( m_aGizmoAxesVB[9].p.x, m_aGizmoAxesVB[9].y + 1.0f, m_aGizmoAxesVB[9].z + 1.0f );
}


//-----------------------------------------------------------------------------
// Name : CalcGizmoPosition()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::CalcGizmoPosition()
{
	if( m_lstSelDecoObjs.size() == 0 )
		return ;

	int							nCount = 0;
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;

	m_vGizmoPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		nCount++;

		m_vGizmoPosition.x += pDecoObj->m_wx;
		m_vGizmoPosition.y += pDecoObj->m_wy;
		m_vGizmoPosition.z += pDecoObj->m_wz;
	}

	m_vGizmoPosition.x /= nCount;
	m_vGizmoPosition.y /= nCount;
	m_vGizmoPosition.z /= nCount;
}


//-----------------------------------------------------------------------------
// Name : RemoveSelDecoObjs()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::RemoveDecoObjsFromSelList( int nIndex )
{
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); )
	{
		pDecoObj = *iterDeco;
		if( pDecoObj->m_nLoloId == nIndex )
			iterDeco = m_lstSelDecoObjs.erase( iterDeco );
		else
			iterDeco++;
	}

	MakeDecoBoxMesh();
	MakeNodeBoundMesh();
	CalcGizmoPosition();
}


//-----------------------------------------------------------------------------
// Name : MoveSelDecoObjs()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::MoveSelDecoObjs( float fdX, float fdY, float fdZ )
{
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	float						fX, fZ;
	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		fX = pDecoObj->m_wx + fdX;
		fZ = pDecoObj->m_wz + fdZ;

		if( fX < m_fObjMinXZ || fX > m_fObjMaxXZ || fZ < m_fObjMinXZ || fZ > m_fObjMaxXZ )
			continue;

		pDecoObj->m_wx = fX;
		pDecoObj->m_wy += fdY;
		pDecoObj->m_wz = fZ;
		pDecoObj->CalBoundBox();
	}

	MakeDecoBoxMesh();
	CalcGizmoPosition();

	if( m_lstSelDecoObjs.size() == 1 )
		UpdateObjEditParameters( pDecoObj );
	else
		UpdateObjEditParameters();
}


//-----------------------------------------------------------------------------
// Name : RotateSelDecoObjs()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::RotateSelDecoObjs( float fdDeg, BOOL bSnap )
{
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;

	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;

		pDecoObj->m_dir += fdDeg * NK_DEGTORAD;
		fmodf( pDecoObj->m_dir, NK_TWOPI );
		if( pDecoObj->m_dir < 0 ) pDecoObj->m_dir += NK_TWOPI;
		else if( pDecoObj->m_dir > NK_TWOPI ) pDecoObj->m_dir -= NK_TWOPI;
		if( bSnap )
		{
			float	fSnapAngle = NK_PI / 12.0f;
			int		nAngle = (int)( pDecoObj->m_dir / fSnapAngle );
			pDecoObj->m_dir = (float)nAngle * fSnapAngle;
		}

		pDecoObj->CalBoundBox();
	}

	MakeDecoBoxMesh();
	CalcGizmoPosition();

	if( m_lstSelDecoObjs.size() == 1 )
		UpdateObjEditParameters( pDecoObj );
	else
		UpdateObjEditParameters();
}

void CMyNKApp::RotateSelDecoObjsByPivot( float fdDeg, D3DVECTOR Pivot ) // 특정좌표를 기준으로 회전시켜주는 함수.(즉, 실제로는 회전이 아닌 좌표이동이 된다.) 06-09-12 원석 (Y는 무시)
{
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	D3DVECTOR TempVec;

	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;

		pDecoObj->m_dir -= fdDeg * NK_DEGTORAD;
		fmodf( pDecoObj->m_dir, NK_TWOPI );
		if( pDecoObj->m_dir < 0 ) pDecoObj->m_dir += NK_TWOPI;
		else if( pDecoObj->m_dir > NK_TWOPI ) pDecoObj->m_dir -= NK_TWOPI;
/*
		if( bSnap ) // 이게 뭔지 모르겠어서 이 함수에선 뺐다.
		{
			float	fSnapAngle = NK_PI / 12.0f;
			int		nAngle = (int)( pDecoObj->m_dir / fSnapAngle );
			pDecoObj->m_dir = (float)nAngle * fSnapAngle;
		}
*/
		TempVec = D3DVECTOR(pDecoObj->m_wx,pDecoObj->m_wy,pDecoObj->m_wz);
		Nk3dMath_VectorRotateYByVertex( TempVec, Pivot, fdDeg * NK_DEGTORAD );
		pDecoObj->m_wx = TempVec.x;
		pDecoObj->m_wy = TempVec.y;
		pDecoObj->m_wz = TempVec.z;

		pDecoObj->CalBoundBox();
	}

	MakeDecoBoxMesh();
//	CalcGizmoPosition(); // 이걸로 좌표축이 바뀌면 곤란하다.

	if( m_lstSelDecoObjs.size() == 1 )
		UpdateObjEditParameters( pDecoObj );
	else
		UpdateObjEditParameters();
}

//-----------------------------------------------------------------------------
// Name : UpdateNodeBoundingBox()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::UpdateChangedDecoObjs()
{
	int							debug = 0;
	int							nX, nZ;
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;

	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		debug++;
		pDecoObj = *iterDeco;

		nX = pDecoObj->m_wx / CNKObjQuadNode::GetCellSize();
		nZ = pDecoObj->m_wz / CNKObjQuadNode::GetCellSize();
		CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );
		
		if( !pObjQuadNode->HasThisDecoObj( pDecoObj ) )
		{
			bool	bFind = false;
			pObjQuadNode = NULL;
			g_pObjTree->FindQuadNode( pDecoObj, &pObjQuadNode, bFind );
		}
		
		pObjQuadNode->RemoveDecoObj( pDecoObj, false );
	}

	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;

		nX = m_vPickedPoint.x / CNKObjQuadNode::GetCellSize();
		nZ = m_vPickedPoint.z / CNKObjQuadNode::GetCellSize();
		CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );
		
		if( !pObjQuadNode->IsDecoObjInNode( pDecoObj ) )
		{
			pObjQuadNode = NULL;
			float	fArea = FLT_MAX;
			g_pObjTree->FindDecoObjFittedNode( pDecoObj, &pObjQuadNode, fArea );
		}

		pObjQuadNode->AddDecoObj( pDecoObj );		
	}

	MakeNodeBoundMesh();

	if( m_lstSelDecoObjs.size() == 1 )
		UpdateObjEditParameters( pDecoObj );
	else
		UpdateObjEditParameters();
}


//-----------------------------------------------------------------------------
// Name : ModifyHeights()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ModifyHeights()
{
	if( m_nControlUnit == 0 )			// Tile
	{
		float	fStrength = m_nBrushStrength / 70.0f;
		switch( m_nVertEditType )
		{
		case 0:			// Raise
			m_pTerrain->RaiseTiles( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX, fStrength );
			break;

		case 1:			// Lower
			m_pTerrain->LowerTiles( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX, fStrength );
			break;

		case 2:			// Flatten
			m_pTerrain->FlattenTiles( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX, fStrength * 0.2f );
			break;

		case 3:			// Smooth
			m_pTerrain->SmoothTiles( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX, fStrength * 0.2f );
			break;
		}
	}
	else								// Vertex
	{
		float	fStrength = m_nBrushStrength / 70.0f;
		float	fRadius = m_pTerrain->GetPatchSize() * m_nBrushSize * 0.5f;
		switch( m_nVertEditType )
		{
		case 0:			// Raise
			m_pTerrain->RaiseVertices( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX,
										m_vPickedPoint.z, m_vPickedPoint.x, fRadius, fStrength );
			break;

		case 1:			// Lower
			m_pTerrain->LowerVertices( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX,
										m_vPickedPoint.z, m_vPickedPoint.x, fRadius, fStrength );
			break;

		case 2:			// Flatten
			m_pTerrain->FlattenVertices( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX,
											m_vPickedPoint.z, m_vPickedPoint.x, fRadius, fStrength * 0.2f );
			break;

		case 3:			// Smooth
			m_pTerrain->SmoothVertices( m_nTileSZ, m_nTileSX, m_nTileEZ, m_nTileEX,
										m_vPickedPoint.z, m_vPickedPoint.x, fRadius, fStrength * 0.2f );
			break;
		}
	}

	// Renew dirty regions
	if( m_nTileSX < m_nDirtyRegionSX ) m_nDirtyRegionSX = m_nTileSX;
	if( m_nTileEX > m_nDirtyRegionEX ) m_nDirtyRegionEX = m_nTileEX;
	if( m_nTileSZ < m_nDirtyRegionSZ ) m_nDirtyRegionSZ = m_nTileSZ;
	if( m_nTileEZ > m_nDirtyRegionEZ ) m_nDirtyRegionEZ = m_nTileEZ;
}


//-----------------------------------------------------------------------------
// Name : ModifyOneVertex()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ModifyOneVertex( int nOffset )
{
	float	fStrength = m_nBrushStrength / 200.0f;
	m_pTerrain->RaiseLowerVertex( m_nPickVertexZ, m_nPickVertexX, nOffset, fStrength );

	// Renew dirty regions
	if( m_nTileSX < m_nDirtyRegionSX ) m_nDirtyRegionSX = m_nTileSX;
	if( m_nTileEX > m_nDirtyRegionEX ) m_nDirtyRegionEX = m_nTileEX;
	if( m_nTileSZ < m_nDirtyRegionSZ ) m_nDirtyRegionSZ = m_nTileSZ;
	if( m_nTileEZ > m_nDirtyRegionEZ ) m_nDirtyRegionEZ = m_nTileEZ;
}


//-----------------------------------------------------------------------------
// Name : PickVertex()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::PickVertex()
{
	int		nX, nZ;
	float	fSqrDist, fSqrDistX, fSqrDistZ, fMinSqrDist;
	float	fPatchSize = m_pTerrain->GetPatchSize();
	float	fSqrRadius = fPatchSize * 0.5f;
	fSqrRadius *= fSqrRadius;
	fMinSqrDist = FLT_MAX;

	for( nZ = m_nTileSZ; nZ <= m_nTileEZ; nZ++ )
	{
		fSqrDistZ = ( nZ * fPatchSize - m_vPickedPoint.z );
		fSqrDistZ *= fSqrDistZ;

		for( nX = m_nTileSX; nX <= m_nTileEX; nX++ )
		{
			fSqrDistX = ( nX * fPatchSize - m_vPickedPoint.x );
			fSqrDistX *= fSqrDistX;			

			fSqrDist = fSqrDistX + fSqrDistZ;
			if( fSqrDist < fSqrRadius && fSqrDist < fMinSqrDist )
			{
				m_nPickVertexX = nX;
				m_nPickVertexZ = nZ;
				fMinSqrDist = fSqrDist;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name : ResetTerrainMinMaxHeights()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ResetTerrainMinMaxHeights()
{
	m_pTerrain->ResetMinMaxHeights();

	// Reset min/max heights of a terrain in a vertex editor tool
	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndToolCtrlBar.m_pageVertEdit.UpdateTerrainInfo();
}


//-----------------------------------------------------------------------------
// Name : SetFogParameters()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetFogParameters( BOOL bSetFog )
{
	if( bSetFog && m_bShowFog )
	{
		float	fNear = m_pCamera->GetNear();
		float	fFar = m_pCamera->GetFar();
		float	fStart = ( m_aFogStart[m_nWorldHour] * ( fFar - fNear ) ) + fNear;
		float	fEnd = ( m_aFogEnd[m_nWorldHour] * ( fFar - fNear ) ) + fNear;
		float	fDensity = 0.0f;

		if( m_bShowSky )
			m_dwBgColor = m_aFogColor[m_nWorldHour];

		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGENABLE, TRUE );
		
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGCOLOR, m_aFogColor[m_nWorldHour] );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLESTART, *( (DWORD*)(&fStart) ) );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEEND, *( (DWORD*)(&fEnd) ) );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEDENSITY, *( (DWORD*)(&fDensity) ) );

		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_LINEAR );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, FALSE );
	}
	else
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
}


//-----------------------------------------------------------------------------
// Name : ChangeWorldTime()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ChangeWorldTime( int nWorldHour )
{
	m_nWorldHour = nWorldHour;

	if( m_nWorldHour < 0 )
		m_nWorldHour = 29;
	else if( m_nWorldHour > 29 )
		m_nWorldHour = 0;

	static BOOL	bPointLight[30] = 
	{
		TRUE, TRUE, TRUE, TRUE, TRUE,
		TRUE, TRUE, FALSE, FALSE, FALSE,
		FALSE, FALSE, FALSE, FALSE, FALSE,
		FALSE, FALSE, FALSE, FALSE, FALSE,
		FALSE, FALSE, TRUE, TRUE, TRUE,
		TRUE, TRUE, TRUE, TRUE, TRUE
	};

	static DWORD	dwAmbient[30] =
	{
		0x00666666, 0x00666666, 0x00707070, 0x00707070, 0x00777777,
		0x00777777, 0x00808080, 0x00888888, 0x00888888, 0x00888888,
		0x00888888, 0x00888888, 0x00888888, 0x00888888, 0x00888888,
		0x00888888, 0x00888888, 0x00888888, 0x00888888, 0x00888888,
		0x00888888, 0x00888888, 0x00808080, 0x00777777, 0x00777777,
		0x00777777, 0x00707070, 0x00707070, 0x00666666, 0x00666666
	};

	struct NkFColorDir
	{
		float r, g, b;
	};

	static NkFColorDir	PointColor[30] =
	{
		0.8f, 0.8f, 0.8f,		// 0
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.7f, 0.7f, 0.7f,		// 4
		0.7f, 0.7f, 0.7f,		// 5
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 10
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 15
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 20
		1.0f, 1.0f, 1.0f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,		// 25
		0.7f, 0.7f, 0.7f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
	};

	static int	point_light_range[30] =
	{
		100, 100, 90, 80, 70,
		70,  60,  60, 60,  60,
		0,   0,   0,  0,   0,
		0,   0,   0,  0,   0,
		60,   60,   60, 60, 70,
		70,  80,  90, 100, 100,
	};

	// 지형 바닥에 스포트 라이트 처리도 해줘야 함..
	if( m_pTerrain )
		m_pTerrain->SetWorldTime( m_nWorldHour );

	// Ambient color
	m_pLightCtrl->SetAmbientLight( m_pd3dDevice, dwAmbient[m_nWorldHour] );

	// Directional light
	m_pLightCtrl->LightEnable( m_pd3dDevice, NKLIGHT_DIRECTIONAL, !bPointLight[m_nWorldHour] );
	static float	fDirX = -1.0f, fDirY = -1.0f, fDirZ = 0.0f;
	if( m_pTerrain )
		m_pTerrain->GetLightDirection( fDirX, fDirY, fDirZ );
	m_pLightCtrl->InitLight( NKLIGHT_DIRECTIONAL, D3DLIGHT_DIRECTIONAL, fDirX, fDirY, fDirZ, 1.0f, 1.0f, 1.0f );

	// Point light
	m_pLightCtrl->LightEnable( m_pd3dDevice, NKLIGHT_POINT, bPointLight[m_nWorldHour] );
	if( bPointLight[m_nWorldHour] )
	{
		float dx, dz;
		GetDirectionElement( m_pRoh->m_dir, dx, dz );
		m_pLightCtrl->InitLight( NKLIGHT_POINT, D3DLIGHT_POINT, m_vTarget.x + dx*10.0f, m_pRoh->m_wy + 25.0f,
									m_vTarget.z + dz*10.0f, PointColor[m_nWorldHour].r,
									PointColor[m_nWorldHour].g, PointColor[m_nWorldHour].b );
		m_fPointRColor = PointColor[m_nWorldHour].r;
		m_fPointGColor = PointColor[m_nWorldHour].g;
		m_fPointBColor = PointColor[m_nWorldHour].b;
		m_pLightCtrl->SetPointLightRange( point_light_range[m_nWorldHour] );
	}
}


//-----------------------------------------------------------------------------
// Name : SetFiltering()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetFiltering( BOOL bFiltering )
{
	if( bFiltering )
	{
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
		m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_LINEAR );
		m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
	}
	else
	{
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_POINT );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_POINT );
		m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_POINT );
		m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFG_POINT );
	}
}


//-----------------------------------------------------------------------------
// Name : nAxis()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::AlignDecoObjs( int nAxis )
{
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	float						fCoord1 = 0.0f;
	float						fCoord2 = 0.0f;
	
	// [2007/11/1 theodoric] UNDO
	m_UndoObj.OnSelObjListBackup();

	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		switch( nAxis )
		{
		case 0:		// X axis
			fCoord1 += pDecoObj->m_wy;
			fCoord2 += pDecoObj->m_wz;
			break;
		case 1:		// Y axis
			fCoord1 += pDecoObj->m_wx;
			fCoord2 += pDecoObj->m_wz;
			break;
		case 2:		// Z axis
			fCoord1 += pDecoObj->m_wx;
			fCoord2 += pDecoObj->m_wy;
			break;
		}
	}

	fCoord1 /= m_lstSelDecoObjs.size();
	fCoord2 /= m_lstSelDecoObjs.size();

	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		switch( nAxis )
		{
		case 0:
			pDecoObj->m_wy = fCoord1;
			pDecoObj->m_wz = fCoord2;
			break;
		case 1:
			pDecoObj->m_wx = fCoord1;
			pDecoObj->m_wz = fCoord2;
			break;
		case 2:
			pDecoObj->m_wx = fCoord1;
			pDecoObj->m_wy = fCoord2;
			break;
		}

		pDecoObj->CalBoundBox();
	}

	MakeDecoBoxMesh();
	CalcGizmoPosition();

	UpdateChangedDecoObjs();

	// [2007/11/1 theodoric] undo
	m_UndoObj.OnSelObjListUndoSet();
}


//-----------------------------------------------------------------------------
// Name : ArrayDecoObjs()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ArrayDecoObjs( int nAxis, float fOffset, int nCount )
{
	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;
	list<CDecoObj*>				lstAddDecoObj;
	CDecoObj*					pAddDecoObj;
	CNKObjQuadNode*				pObjQuadNode;
	float						fCoord, fArea;
	int							i, nX, nZ;

	for( iterDeco = m_lstSelDecoObjs.begin(); iterDeco != m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		switch( nAxis )
		{
		case 0:			// X axis
			fCoord = pDecoObj->m_wx;
			break;
		case 1:			// Y axis
			fCoord = pDecoObj->m_wy;
			break;
		case 2:			// Z axis
			fCoord = pDecoObj->m_wz;
			break;
		}
		
		for( i = 0; i < nCount; i++ )
		{
			//////////////////////////////////////////////////////////////////////////////////
			// 수정자 : 이원석
			// 수정일 : 05-02-23
			// 수정내용 : 로컬좌표축으로 Array가능 기능 추가.
			if( m_bShowLocalAxis ) // 로컬 좌표축으로 Array요청을 한거라면 최종 좌표만 살짝 바꿔준다.
			{
				D3DVECTOR ResultVec;

				switch( nAxis )
				{
				case 0:
					ResultVec = D3DVECTOR( 1.0f, 0.0f, 0.0f );
					break;
				case 1:
					ResultVec = D3DVECTOR( 0.0f, 1.0f, 0.0f );
					break;
				case 2:
					ResultVec = D3DVECTOR( 0.0f, 0.0f, 1.0f );
					break;
				}

				ResultVec *= (i+1) * fOffset;

				D3DMATRIX	matLocalRotate;
				D3DUtil_SetIdentityMatrix( matLocalRotate );

				float TempRad =  pDecoObj->m_dir - g_2_PI; // 이렇게 해줘야 정상 방향으로 따라간다.

				Nk3DUtil_SetRotateCCWYMatrix( matLocalRotate, TempRad );
				matLocalRotate._41 = pDecoObj->m_wx;
				matLocalRotate._42 = pDecoObj->m_wy;
				matLocalRotate._43 = pDecoObj->m_wz;

				D3DMath_VectorMatrixMultiply( ResultVec, ResultVec, matLocalRotate );

				if( (ResultVec.x<m_fObjMinXZ||ResultVec.x>m_fObjMaxXZ)
					|| (ResultVec.z<m_fObjMinXZ||ResultVec.z>m_fObjMaxXZ) ) // 범위 밖으로 넘어가 버리면..
				{
					continue;
				}

				pAddDecoObj = new CDecoObj();
				pAddDecoObj->m_wx = ResultVec.x;
				pAddDecoObj->m_wy = ResultVec.y;
				pAddDecoObj->m_wz = ResultVec.z;
				pAddDecoObj->m_dir = pDecoObj->m_dir;
				pAddDecoObj->m_nLoloId = pDecoObj->m_nLoloId;
				pAddDecoObj->m_pLolo = pDecoObj->m_pLolo;
			}
			else
			{
				fCoord += fOffset;
				if( ( nAxis == 0 || nAxis == 2 ) && ( fCoord < m_fObjMinXZ || fCoord > m_fObjMaxXZ ) )
					continue;
				
				pAddDecoObj = new CDecoObj();
				pAddDecoObj->m_wx = pDecoObj->m_wx;
				pAddDecoObj->m_wy = pDecoObj->m_wy;
				pAddDecoObj->m_wz = pDecoObj->m_wz;
				pAddDecoObj->m_dir = pDecoObj->m_dir;
				pAddDecoObj->m_nLoloId = pDecoObj->m_nLoloId;
				pAddDecoObj->m_pLolo = pDecoObj->m_pLolo;
				
				switch( nAxis )
				{
				case 0:
					pAddDecoObj->m_wx = fCoord;
					break;
				case 1:
					pAddDecoObj->m_wy = fCoord;
					break;
				case 2:
					pAddDecoObj->m_wz = fCoord;
					break;
				}
			}

			pAddDecoObj->CalBoundBox();

			nX = pAddDecoObj->m_wx / CNKObjQuadNode::GetCellSize();
			nZ = pAddDecoObj->m_wz / CNKObjQuadNode::GetCellSize();
			pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );
			
			if( !pObjQuadNode->IsDecoObjInNode( pAddDecoObj ) )
			{
				pObjQuadNode = NULL;
				fArea = FLT_MAX;
				g_pObjTree->FindDecoObjFittedNode( pAddDecoObj, &pObjQuadNode, fArea );
			}

			pObjQuadNode->AddDecoObj( pAddDecoObj );
			g_pDecoObjSet->m_vecRefCount[pAddDecoObj->m_nLoloId]++;
			//g_pDecoObjSet->m_vecRefCount[m_nCurDecoObj]++;


			lstAddDecoObj.push_back( pAddDecoObj );
		}
	}

	for( iterDeco = lstAddDecoObj.begin(); iterDeco != lstAddDecoObj.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;

		//////////////////////////////////////////////////////////////////////////
		// [2007/10/30 theodoric] 생성된 오브젝트를 undo 스텍에 쌓자
		UNDO_OBJ Temp;
	
		Temp.type = UNDO_OBJ_ARRAY;
		Temp.pDecObject = pDecoObj;

		m_UndoObj.AddUndoObject(&Temp);
		//////////////////////////////////////////////////////////////////////////

		if( m_lstSelDecoObjs.size() < 500 )
		{			
			pDecoObj = *iterDeco;
			m_lstSelDecoObjs.push_back( pDecoObj );
		}
	}

	lstAddDecoObj.erase( lstAddDecoObj.begin(), lstAddDecoObj.end() );

	MakeDecoBoxMesh();
	CalcGizmoPosition();

	UpdateObjEditParameters();
}


//-----------------------------------------------------------------------------
// Name : InitProgress()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::InitProgress()
{
	// Create a progress dialog
	m_pProgressDlg = new CProgressDlg;
	m_pProgressDlg->Create( IDD_PROGRESSDLG );
	CRect	rc;
	m_pProgressDlg->GetWindowRect( &rc );
	DWORD	dwWidth = m_dwClientWidth / 2 - rc.Width() / 2;
	DWORD	dwHeight = m_dwClientHeight / 2 - rc.Height() / 2;
	rc.OffsetRect( dwWidth, dwHeight );
	m_pProgressDlg->MoveWindow( &rc );
	m_pProgressDlg->ShowWindow( SW_SHOW );
	m_pProgressDlg->UpdateWindow();
	m_bShowProgress = TRUE;	
}


//-----------------------------------------------------------------------------
// Name : DestroyProgress()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::DestroyProgress()
{
	SAFE_DELETE( m_pProgressDlg );
	m_bShowProgress = FALSE;
}


//-----------------------------------------------------------------------------
// Name : SetProgressTitle()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetProgressTitle( const TCHAR* szTitle )
{
	m_pProgressDlg->SetTitle( szTitle );
}


//-----------------------------------------------------------------------------
// Name : SetProgressContent()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetProgressContent( const TCHAR* szContent )
{
	m_pProgressDlg->SetContent( szContent );
}


//-----------------------------------------------------------------------------
// Name : SetProgressRange()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetProgressRange( int nLower, int nUpper )
{
	m_pProgressDlg->SetRange( nLower, nUpper );
}


//-----------------------------------------------------------------------------
// Name : SetProgressPos()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetProgressPos( int nPos )
{
	m_pProgressDlg->SetPos( nPos );
}
	

//-----------------------------------------------------------------------------
// Name : SetProgressOffsetPos()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetProgressOffsetPos( int nOffsetPos )
{
	m_pProgressDlg->SetOffsetPos( nOffsetPos );
}


//-----------------------------------------------------------------------------
// Name : ChangeLightDirection()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ChangeLightDirection( BOOL bLatitude, BOOL bTrace, BOOL bModify )
{
	float	fT;
	if( NKIntersectRaySphere( m_vPickOrigin, m_vPickDirection,
								m_vLightCtrlRelativePos, 100.0f, &fT ) )
	{
		D3DXVECTOR3	vDir, vPick;
		D3DXVec3Scale( &vDir, &m_vPickDirection, fT );
		D3DXVec3Add( &vPick, &m_vPickOrigin, &vDir );
		D3DXVec3Subtract( &vPick, &vPick, &m_vLightCtrlRelativePos );

		if( vPick.y > 0.0f )
		{
			if( bLatitude )
			{
				float	fDelta = NK_TWOPI / 144.0f;
				float	fX, fZ;
				float	fRadius = 100.0f * sinf( -atan2f( vPick.z, vPick.y ) );
				int		nCount = 0;
				m_fSunLatitude = 100.0f * cosf( -atan2f( vPick.z, vPick.y ) );
				for( float fRadian = 0.0f; fRadian < NK_TWOPI; fRadian += fDelta )
				{
					fX = fRadius * sinf( fRadian );
					fZ = fRadius * cosf( fRadian );

					m_aSunLatitudeVB[nCount].x = fX;
					m_aSunLatitudeVB[nCount].y = m_fSunLatitude;
					m_aSunLatitudeVB[nCount].z = fZ;
					m_aSunLatitudeVB[nCount].c = 0xffff80ff;
					nCount++;
				}
				m_aSunLatitudeVB[nCount].x = m_aSunLatitudeVB[0].x;
				m_aSunLatitudeVB[nCount].y = m_aSunLatitudeVB[0].y;
				m_aSunLatitudeVB[nCount].z = m_aSunLatitudeVB[0].z;
				m_aSunLatitudeVB[nCount].c = 0xffff80ff;
			}
			else if( bTrace )
			{
				m_fSunTrace = -atan2f( vPick.z, vPick.y );
				D3DXQUATERNION	quat;
				NKMATRIX3		mat;
				NKQuaternionFromYawPitch( quat, 0.0f, -m_fSunTrace );
				NKQuaternionToMatrix( mat, quat );
				for( int i = 0; i < 73; i++ )
					NKVectorMatrixMultiply( m_aSunTraceVB[i].p, m_aLightCtrlXYVB[i].p, mat );
			}
			else
			{
				m_aLightCtrlExtraVB[m_nLightTimeID*2+1].x = vPick.x;
				m_aLightCtrlExtraVB[m_nLightTimeID*2+1].y = vPick.y;
				m_aLightCtrlExtraVB[m_nLightTimeID*2+1].z = vPick.z;

				if( bModify )
				{
					D3DXVec3Normalize( &vPick, &vPick );
					m_aLightDirection[m_nLightTimeID] = -vPick;
					CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
					pFrame->m_wndToolCtrlBar.m_pageLightEdit.SetDirection( -vPick.x, -vPick.y, -vPick.z );
					ChangeWorldTime( m_nWorldHour );
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ToggleShowCharacter()
{
	if( m_nCameraType == 0 )
	{
		m_bShowCharacter = !m_bShowCharacter;

		m_vTarget.y = m_pTerrain->GetHeights( m_vTarget.z, m_vTarget.x );
		m_pRoh->m_wy = m_vTarget.y;
		if( m_bShowCharacter )
			m_vTarget.y = m_pRoh->m_wy + 15.0f;
		m_pCamera->SetTarget( m_vTarget );
		m_pCamera->UpdateViewMatrix();
	}
}


//-----------------------------------------------------------------------------
// Name : CreateSky()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::CreateSky()
{
	if( !m_pSkyPlane )
	{
		m_pSkyPlane = new CSkyPlane;
		m_pSkyPlane->Init( 280.0f, 295.0f, 5.0f, 5.0f, 142.0f );
		m_pSkyPlane->SetDevice( m_pd3dDevice );
	}
}


//-----------------------------------------------------------------------------
// Name : ToggleShowSky()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ToggleShowSky()
{
	m_bShowSky = !m_bShowSky;
	if( m_bShowSky )
		m_dwBgColor = m_aFogColor[m_nWorldHour];
	else
		m_dwBgColor = 0x00bbbbbb;
}


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::InitFogParameters()
{
	m_aFogColor[0] = RGBA_MAKE(25, 53, 115, 255);			// 0
	m_aFogColor[1] = RGBA_MAKE(25, 53, 115, 255);			// 1
	m_aFogColor[2] = RGBA_MAKE(135, 198, 255, 255);			// 새벽 2
	m_aFogColor[3] = RGBA_MAKE(135, 198, 255, 255);			// 3
	m_aFogColor[4] = RGBA_MAKE(135, 198, 255, 255);			// 4
	m_aFogColor[5] = RGBA_MAKE(135, 198, 255, 255);			// 5
	m_aFogColor[6] = RGBA_MAKE(135, 198, 255, 255);			// 6
	m_aFogColor[7] = RGBA_MAKE(134, 177, 210, 255);			// 낮 7
	m_aFogColor[8] = RGBA_MAKE(134, 177, 210, 255);			// 8
	m_aFogColor[9] = RGBA_MAKE(134, 177, 210, 255);			// 9
	m_aFogColor[10] = RGBA_MAKE(134, 177, 210, 255);		// 10
	m_aFogColor[11] = RGBA_MAKE(134, 177, 210, 255);		// 11
	m_aFogColor[12] = RGBA_MAKE(134, 177, 210, 255);		// 12
	m_aFogColor[13] = RGBA_MAKE(134, 177, 210, 255);		// 13
	m_aFogColor[14] = RGBA_MAKE(134, 177, 210, 255);		// 14
	m_aFogColor[15] = RGBA_MAKE(134, 177, 210, 255);		// 15
	m_aFogColor[16] = RGBA_MAKE(134, 177, 210, 255);		// 16
	m_aFogColor[17] = RGBA_MAKE(134, 177, 210, 255);		// 17
	m_aFogColor[18] = RGBA_MAKE(134, 177, 210, 255);		// 18
	m_aFogColor[19] = RGBA_MAKE(134, 177, 210, 255);		// 19
	m_aFogColor[20] = RGBA_MAKE(134, 177, 210, 255);		// 20
	m_aFogColor[21] = RGBA_MAKE(134, 177, 210, 255);		// 21
	m_aFogColor[22] = RGBA_MAKE(255, 185, 140, 255);		// 저녁 22
	m_aFogColor[23] = RGBA_MAKE(255, 185, 140, 255);		// 23
	m_aFogColor[24] = RGBA_MAKE(255, 185, 140, 255);		// 24
	m_aFogColor[25] = RGBA_MAKE(255, 185, 140, 255);		// 25
	m_aFogColor[26] = RGBA_MAKE(255, 185, 140, 255);		// 26
	m_aFogColor[27] = RGBA_MAKE(255, 185, 140, 255);		// 27
	m_aFogColor[28] = RGBA_MAKE(25, 53, 115, 255);			// 밤 28
	m_aFogColor[29] = RGBA_MAKE(25, 53, 115, 255);			// 29

	m_aFogStart[0] = 0.057f;
	m_aFogStart[1] = 0.069f;
	m_aFogStart[2] = 0.080f;
	m_aFogStart[3] = 0.091f;
	m_aFogStart[4] = 0.101f;
	m_aFogStart[5] = 0.111f;
	m_aFogStart[6] = 0.121f;
	m_aFogStart[7] = 0.133f;
	m_aFogStart[8] = 0.133f;
	m_aFogStart[9] = 0.133f;
	m_aFogStart[10] = 0.133f;
	m_aFogStart[11] = 0.133f;
	m_aFogStart[12] = 0.133f;
	m_aFogStart[13] = 0.133f;
	m_aFogStart[14] = 0.143f;
	m_aFogStart[15] = 0.143f;
	m_aFogStart[16] = 0.133f;
	m_aFogStart[17] = 0.133f;
	m_aFogStart[18] = 0.133f;
	m_aFogStart[19] = 0.133f;
	m_aFogStart[20] = 0.133f;
	m_aFogStart[21] = 0.133f;
	m_aFogStart[22] = 0.133f;
	m_aFogStart[23] = 0.121f;
	m_aFogStart[24] = 0.111f;
	m_aFogStart[25] = 0.101f;
	m_aFogStart[26] = 0.091f;
	m_aFogStart[27] = 0.080f;
	m_aFogStart[28] = 0.069f;
	m_aFogStart[29] = 0.057f;

	m_aFogEnd[0] = 0.223f;
	m_aFogEnd[1] = 0.227f;
	m_aFogEnd[2] = 0.233f;
	m_aFogEnd[3] = 0.238f;
	m_aFogEnd[4] = 0.244f;
	m_aFogEnd[5] = 0.255f;
	m_aFogEnd[6] = 0.223f;
	m_aFogEnd[7] = 0.223f;
	m_aFogEnd[8] = 0.223f;
	m_aFogEnd[9] = 0.223f;
	m_aFogEnd[10] = 0.223f;
	m_aFogEnd[11] = 0.223f;
	m_aFogEnd[12] = 0.223f;
	m_aFogEnd[13] = 0.223f;
	m_aFogEnd[14] = 0.223f;
	m_aFogEnd[15] = 0.223f;
	m_aFogEnd[16] = 0.223f;
	m_aFogEnd[17] = 0.223f;
	m_aFogEnd[18] = 0.223f;
	m_aFogEnd[19] = 0.223f;
	m_aFogEnd[20] = 0.223f;
	m_aFogEnd[21] = 0.223f;
	m_aFogEnd[22] = 0.223f;
	m_aFogEnd[23] = 0.223f;
	m_aFogEnd[24] = 0.223f;
	m_aFogEnd[25] = 0.244f;
	m_aFogEnd[26] = 0.238f;
	m_aFogEnd[27] = 0.233f;
	m_aFogEnd[28] = 0.227f;
	m_aFogEnd[29] = 0.223f;
}


//-----------------------------------------------------------------------------
// Name : SetBaseWaterTexture()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetBaseWaterTexture( const TCHAR* szFilename )
{
	TCHAR	szTemp[512];
	_tcscpy( szTemp, szFilename );
	int	nTexID = CNKObjQuadNode::GetBaseWaterTexID();
	if( nTexID != -1 )
		m_pDecoTexture->DestroyTextureByIdx( nTexID );
	nTexID = m_pDecoTexture->CreateTextureFromFile( szTemp, nTexID );
	CNKObjQuadNode::SetBaseWaterTexID( nTexID );
	if( nTexID != -1 )
		m_pDecoTexture->RestoreByIdx( nTexID, m_pd3dDevice );
	m_bModify[2] = TRUE;
}


//-----------------------------------------------------------------------------
// Name : SetWaveWaterTexture1()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetWaveWaterTexture1( const TCHAR* szFilename )
{
	TCHAR	szTemp[512];
	_tcscpy( szTemp, szFilename );
	int	nTexID = CNKObjQuadNode::GetWaveWaterTexID1();
	if( nTexID != -1 )
		m_pDecoTexture->DestroyTextureByIdx( nTexID );
	nTexID = m_pDecoTexture->CreateTextureFromFile( szTemp, nTexID );
	CNKObjQuadNode::SetWaveWaterTexID1( nTexID );
	if( nTexID != -1 )
		m_pDecoTexture->RestoreByIdx( nTexID, m_pd3dDevice );
	m_bModify[2] = TRUE;
}


//-----------------------------------------------------------------------------
// Name : SetWaveWaterTexture2()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetWaveWaterTexture2( const TCHAR* szFilename )
{
	TCHAR	szTemp[512];
	_tcscpy( szTemp, szFilename );
	int	nTexID = CNKObjQuadNode::GetWaveWaterTexID2();
	if( nTexID != -1 )
		m_pDecoTexture->DestroyTextureByIdx( nTexID );
	nTexID = m_pDecoTexture->CreateTextureFromFile( szTemp, nTexID );
	CNKObjQuadNode::SetWaveWaterTexID2( nTexID );
	if( nTexID != -1 )
		m_pDecoTexture->RestoreByIdx( nTexID, m_pd3dDevice );
	m_bModify[2] = TRUE;
}


//-----------------------------------------------------------------------------
// Name : SetWaterGroupID()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetWaterGroupID( int nGroupID )
{
	m_nWaterGroupID = nGroupID;

	ResetWaveWaterVertex();
}


//-----------------------------------------------------------------------------
// Name : SetWaterSegment()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetWaterSegment( int nSegment )
{
	m_nWaterSegment = nSegment;

	m_dwWaveIndexCount = nSegment * 12;
	int			nVert = ( nSegment + 1 ) * 3;
	float		fX = 0.0f;
	float		fZ = 0.0f;
	
	for( int i = 0; i < nVert; i++ )
	{
		fX += m_aWaveWaterVB[i].x;
		fZ += m_aWaveWaterVB[i].z;
	}
	m_vWaveWaterCenter.x = fX / nVert;
	m_vWaveWaterCenter.z = fZ / nVert;

	MakeWaterMesh();
}


//-----------------------------------------------------------------------------
// Name : GetPickedWaterPlane()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::GetPickedWaterPlane()
{
	D3DXPLANE	plWater;
	D3DXVECTOR3	aPoints[3];
	float		fWaterHeight;
	float		fA, fB, fT = 1.0f;

	if( m_nWaterGroupID == -1 )
		fWaterHeight = 0.0f;
	else
		fWaterHeight = CNKObjQuadNode::GetHeightOfWaterGroup( m_nWaterGroupID );
	
	aPoints[0].x = 0.0f;	aPoints[0].y = fWaterHeight;	aPoints[0].z = 0.0f;
	aPoints[1].x = 0.0f;	aPoints[1].y = fWaterHeight;	aPoints[1].z = 100.0f;
	aPoints[2].x = 100.0f;	aPoints[2].y = fWaterHeight;	aPoints[2].z = 100.0f;
	D3DXPlaneFromPoints( &plWater, &aPoints[0], &aPoints[1], &aPoints[2] );
	
	fA = m_vPickDirection.x * plWater.a + m_vPickDirection.y * plWater.b + m_vPickDirection.z * plWater.c;
	fB = -( m_vPickOrigin.x * plWater.a + m_vPickOrigin.y * plWater.b + m_vPickOrigin.z * plWater.c + plWater.d );
	
	if( fabsf( fA ) > NK_EPSILON5 )
		fT = fB / fA;

	m_vPickedPoint.x = m_vPickOrigin.x + m_vPickDirection.x * fT;
	m_vPickedPoint.y = m_vPickOrigin.y + m_vPickDirection.y * fT;
	m_vPickedPoint.z = m_vPickOrigin.z + m_vPickDirection.z * fT;
}


//-----------------------------------------------------------------------------
// Name : PaintBaseWater()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::PaintBaseWater()
{
	if( m_nWaterGroupID == -1 )
		return ;

	int	nX, nZ;
	int	nWidth = ( m_pTerrain->GetHMWidth() - 1 ) / 4;
	nX = m_vPickedPoint.x / CNKObjQuadNode::GetCellSize();
	nZ = m_vPickedPoint.z / CNKObjQuadNode::GetCellSize();
	
	if( nX < 0 || nX >= nWidth || nZ < 0 || nZ >= nWidth )
		return ;

	CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );

	int	nWaterX, nWaterZ, nWaterIndex;
	nWaterX = m_vPickedPoint.x / CNKObjQuadNode::GetBaseWaterSize();
	nWaterZ = m_vPickedPoint.z / CNKObjQuadNode::GetBaseWaterSize();
	nWaterX %= 2;	nWaterZ %= 2;
	nWaterIndex = nWaterX + nWaterZ * 2;
	pObjQuadNode->PaintBaseWater( nWaterIndex, m_nWaterGroupID );

	m_bModify[2] = TRUE;
}


//-----------------------------------------------------------------------------
// Name : EraseBaseWater()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::EraseBaseWater()
{
	if( m_nWaterGroupID == -1 )
		return ;

	int	nX, nZ;
	int	nWidth = ( m_pTerrain->GetHMWidth() - 1 ) / 4;
	nX = m_vPickedPoint.x / CNKObjQuadNode::GetCellSize();
	nZ = m_vPickedPoint.z / CNKObjQuadNode::GetCellSize();

	if( nX < 0 || nX >= nWidth || nZ < 0 || nZ >= nWidth )
		return ;

	CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );

	int	nWaterX, nWaterZ, nWaterIndex;
	nWaterX = m_vPickedPoint.x / CNKObjQuadNode::GetBaseWaterSize();
	nWaterZ = m_vPickedPoint.z / CNKObjQuadNode::GetBaseWaterSize();
	nWaterX %= 2;	nWaterZ %= 2;
	nWaterIndex = nWaterX + nWaterZ * 2;
	pObjQuadNode->EraseBaseWater( nWaterIndex, m_nWaterGroupID );

	m_bModify[2] = TRUE;
}


//-----------------------------------------------------------------------------
// Name : GetPickedWaveWaterVertex()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::GetPickedWaveWaterVertex()
{
	int		nVert = ( m_nWaterSegment + 1 ) * 3;
	float	fDist = FLT_MAX;

	m_dwSelWaterVertex = -1;
	for( int i = 0; i < nVert; i++ )
	{
		if( i % 3 == 1 )
			continue;

		float	fNewDist = FLT_MAX;
		if( NKIntersectRaySphere( m_vPickOrigin, m_vPickDirection,
									m_aWaveWaterVB[i].p, 3.0f, &fNewDist ) )
		{
			if( fNewDist < fDist )
			{
				m_dwSelWaterVertex = i;
				fDist = fNewDist;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name : ResetWaveWaterVertex()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ResetWaveWaterVertex()
{
	float	fHeight = CNKObjQuadNode::GetHeightOfWaterGroup( m_nWaterGroupID ) + 0.3f;

	int	nWidth = CNKObjQuadNode::GetWaveWaterSize();
	int	nHalfWidth = nWidth / 2;
	int i = 0;
	for( i = 0; i < 9; i++ )
	{
		m_aWaveWaterVB[i*3].x = 0.0f;
		m_aWaveWaterVB[i*3 + 1].x = nHalfWidth;
		m_aWaveWaterVB[i*3 + 2].x = nWidth;
		m_aWaveWaterVB[i*3].y = fHeight;
		m_aWaveWaterVB[i*3 + 1].y = fHeight;
		m_aWaveWaterVB[i*3 + 2].y = fHeight;
		m_aWaveWaterVB[i*3].z = -( nWidth * i );
		m_aWaveWaterVB[i*3 + 1].z = m_aWaveWaterVB[i*3].z;
		m_aWaveWaterVB[i*3 + 2].z = m_aWaveWaterVB[i*3].z;

		m_aWaveWaterVB[i*3].c = 0xff000000;
		m_aWaveWaterVB[i*3 + 1].c = 0xffffffff;
		m_aWaveWaterVB[i*3 + 2].c = 0xff000000;

		m_aWaveWaterVB[i*3].u = 0.0f;
		m_aWaveWaterVB[i*3 + 1].u = 0.5f;
		m_aWaveWaterVB[i*3 + 2].u = 1.0f;
		m_aWaveWaterVB[i*3].v = i;
		m_aWaveWaterVB[i*3 + 1].v = i;
		m_aWaveWaterVB[i*3 + 2].v = i;
	}

	int		nVert = ( m_nWaterSegment + 1 ) * 3;
	float	fX = 0.0f;
	float	fZ = 0.0f;
	for( i = 0; i < nVert; i++ )
	{
		fX += m_aWaveWaterVB[i].x;
		fZ += m_aWaveWaterVB[i].z;
	}
	m_vWaveWaterCenter.x = fX / nVert;
	m_vWaveWaterCenter.z = fZ / nVert;
}


//-----------------------------------------------------------------------------
// Name : ModifyWaveWaterVertex()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::ModifyWaveWaterVertex( BOOL bSnap )
{
	m_aWaveWaterVB[m_dwSelWaterVertex].x = m_vPickedPoint.x;
	m_aWaveWaterVB[m_dwSelWaterVertex].z = m_vPickedPoint.z;

	float	fX, fZ;
	if( bSnap )
	{
		int		nSelVert = -1;
		float	fDist, fOldDist = FLT_MAX;
		
		for( int i = 0; i < m_dwNeighWaveVertCount; i++ )
		{
			fX = m_aWaveWaterVB[m_dwSelWaterVertex].x - m_aNeighWaveVert[i].x;
			fZ = m_aWaveWaterVB[m_dwSelWaterVertex].z - m_aNeighWaveVert[i].z;
			fDist = fX*fX + fZ*fZ;
			if( fDist < 100.0f && fDist < fOldDist )
			{
				nSelVert = i;
				fOldDist = fDist;
			}
		}

		if( nSelVert >= 0 )
		{
			m_aWaveWaterVB[m_dwSelWaterVertex].x = m_aNeighWaveVert[nSelVert].x;
			m_aWaveWaterVB[m_dwSelWaterVertex].z = m_aNeighWaveVert[nSelVert].z;
		}
	}

	if( m_dwSelWaterVertex % 3 == 0 )
	{
		fX = m_aWaveWaterVB[m_dwSelWaterVertex].x + m_aWaveWaterVB[m_dwSelWaterVertex+2].x;
		fZ = m_aWaveWaterVB[m_dwSelWaterVertex].z + m_aWaveWaterVB[m_dwSelWaterVertex+2].z;
		fX *= 0.5f;		fZ *= 0.5f;
		m_aWaveWaterVB[m_dwSelWaterVertex+1].x = fX;
		m_aWaveWaterVB[m_dwSelWaterVertex+1].z = fZ;
	}
	else
	{
		fX = m_aWaveWaterVB[m_dwSelWaterVertex].x + m_aWaveWaterVB[m_dwSelWaterVertex-2].x;
		fZ = m_aWaveWaterVB[m_dwSelWaterVertex].z + m_aWaveWaterVB[m_dwSelWaterVertex-2].z;
		fX *= 0.5f;		fZ *= 0.5f;
		m_aWaveWaterVB[m_dwSelWaterVertex-1].x = fX;
		m_aWaveWaterVB[m_dwSelWaterVertex-1].z = fZ;
	}
}


//-----------------------------------------------------------------------------
// Name : RotateWaveWaterVertices()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::RotateWaveWaterVertices( float fdDeg )
{
	float	fAngle = fdDeg * NK_DEGTORAD;
	float	fSin = sinf( fAngle );
	float	fCos = cosf( fAngle );
	int i = 0;
	
	for( i = 0; i < 27; i++ )
	{
		m_aWaveWaterVB[i].x -= m_vWaveWaterCenter.x;
		m_aWaveWaterVB[i].z -= m_vWaveWaterCenter.z;
	}

	float	fX, fZ;
	for( i = 0; i < 27; i++ )
	{
		fX = m_aWaveWaterVB[i].x;
		fZ = m_aWaveWaterVB[i].z;
		
		m_aWaveWaterVB[i].x = fX * fCos + fZ * fSin;
		m_aWaveWaterVB[i].z = -fX * fSin + fZ * fCos;
	}

	for( i = 0; i < 27; i++ )
	{
		m_aWaveWaterVB[i].x += m_vWaveWaterCenter.x;
		m_aWaveWaterVB[i].z += m_vWaveWaterCenter.z;
	}
}


//-----------------------------------------------------------------------------
// Name : PaintWaveWater()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::PaintWaveWater()
{
	if( m_nWaterGroupID == -1 )
		return ;

	int	nX, nZ;
	int	nWidth = ( m_pTerrain->GetHMWidth() - 1 ) / 4;
	nX = m_vPickedPoint.x / CNKObjQuadNode::GetCellSize();
	nZ = m_vPickedPoint.z / CNKObjQuadNode::GetCellSize();
	
	if( nX < 0 || nX >= nWidth || nZ < 0 || nZ >= nWidth )
		return ;

	CNKObjQuadNode*	pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );

	float		fHeight = m_aWaveWaterVB[0].y;
	D3DXVECTOR3	vMin = D3DXVECTOR3( FLT_MAX, fHeight - 0.3f, FLT_MAX );
	D3DXVECTOR3	vMax = D3DXVECTOR3( -FLT_MAX, fHeight, -FLT_MAX );
	int			nVert = ( m_nWaterSegment + 1 ) * 3;
	for( int i = 0; i < nVert; i++ )
	{
		if( vMin.x > m_aWaveWaterVB[i].x ) vMin.x = m_aWaveWaterVB[i].x;
		if( vMax.x < m_aWaveWaterVB[i].x ) vMax.x = m_aWaveWaterVB[i].x;
		if( vMin.z > m_aWaveWaterVB[i].z ) vMin.z = m_aWaveWaterVB[i].z;		
		if( vMax.z < m_aWaveWaterVB[i].z ) vMax.z = m_aWaveWaterVB[i].z;
	}

	if( !pObjQuadNode->IsAABBInNode( vMin, vMax ) )
	{
		pObjQuadNode = NULL;
		float	fArea = FLT_MAX;
		g_pObjTree->FindAABBFittedNode( vMin, vMax, &pObjQuadNode, fArea );
	}

	if( pObjQuadNode == NULL )
		return ;

	int	nResult = pObjQuadNode->PaintWaveWater( m_nWaterGroupID, m_aWaveWaterVB, m_nWaterSegment, vMin, vMax );

	if( nResult == 1 )
		MessageBox( NULL, _T("Base Water가 그려지지 않은 영역입니다."), _T("NKMapTool"), MB_OK );
	else if( nResult == 2 )
		MessageBox( NULL, _T("이미 두 개의 Wave Water가 그려진 영역입니다."), _T("NKMapTool"), MB_OK );
	else if( nResult == 3 )
		MessageBox( NULL, _T("다른 Water Group에 그리기를 시도했습니다."), _T("NKMapTool"), MB_OK );
	else
	{
		m_bModify[2] = TRUE;
		ResetWaveWaterVertex();
	}
}


//-----------------------------------------------------------------------------
// Name : EraseWaveWater()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::EraseWaveWater()
{
	if( m_nWaterGroupID == -1 )
		return ;

	int	nSX, nSZ, nEX, nEZ, x, z;
	int	nWidth = ( m_pTerrain->GetHMWidth() - 1 ) / 4;
	nSX = m_vPickedPoint.x / CNKObjQuadNode::GetCellSize();
	nSZ = m_vPickedPoint.z / CNKObjQuadNode::GetCellSize();
	nEX = nSX + 1;		nEZ = nSZ + 1;
	nSX -= 1;			nSZ -= 1;

	if( nSX < 0 ) nSX = 0;
	if( nSZ < 0 ) nSZ = 0;
	if( nEX >= nWidth ) nEX = nWidth - 1;
	if( nEZ >= nWidth ) nEZ = nWidth - 1;

	CNKObjQuadNode*	pObjQuadNode;
	for( z = nSZ; z <= nEZ; z++ )
	{
		for( x = nSX; x <= nEX; x++ )
		{
			pObjQuadNode = CNKObjQuadNode::GetQuadNode( z, x );
			if( pObjQuadNode->EraseWaveWater( m_nWaterGroupID, m_vPickOrigin, m_vPickDirection ) )
				break;
		}
	}

	m_bModify[2] = TRUE;
}


//-----------------------------------------------------------------------------
// Name : GetPickedWaveWaterFromNode()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::GetPickedWaveWaterFromNode()
{
	if( m_nWaterGroupID == -1 )
		return ;

	int	nSX, nSZ, nEX, nEZ, x, z;
	int	nWidth = ( m_pTerrain->GetHMWidth() - 1 ) / 4;
	nSX = m_vPickedPoint.x / CNKObjQuadNode::GetCellSize();
	nSZ = m_vPickedPoint.z / CNKObjQuadNode::GetCellSize();
	nEX = nSX + 1;		nEZ = nSZ + 1;
	nSX -= 1;			nSZ -= 1;

	if( nSX < 0 ) nSX = 0;
	if( nSZ < 0 ) nSZ = 0;
	if( nEX >= nWidth ) nEX = nWidth - 1;
	if( nEZ >= nWidth ) nEZ = nWidth - 1;

	CNKObjQuadNode*	pObjQuadNode;
	BOOL			bGetData = FALSE;
	for( z = nSZ; z <= nEZ; z++ )
	{
		for( x = nSX; x <= nEX; x++ )
		{
			pObjQuadNode = CNKObjQuadNode::GetQuadNode( z, x );
			if( pObjQuadNode->GetPickedWaveWater( m_nWaterGroupID, m_aWaveWaterVB, m_nWaterSegment,
													m_vPickOrigin, m_vPickDirection ) )
			{
				bGetData = TRUE;
				break;
			}
		}
	}

	if( bGetData )
	{
		int		nVert = ( m_nWaterSegment + 1 ) * 3;
		float	fX = 0.0f;
		float	fZ = 0.0f;
		int i = 0;
		for( i = 0; i < nVert; i++ )
		{
			fX += m_aWaveWaterVB[i].x;
			fZ += m_aWaveWaterVB[i].z;
		}
		m_vWaveWaterCenter.x = fX / nVert;
		m_vWaveWaterCenter.z = fZ / nVert;

		float	fOffsetX, fOffsetZ;
		for( i = nVert; i < 27; i++ )
		{
			fOffsetX = m_aWaveWaterVB[i - 3].x - m_aWaveWaterVB[i - 6].x;
			fOffsetZ = m_aWaveWaterVB[i - 3].z - m_aWaveWaterVB[i - 6].z;
			m_aWaveWaterVB[i].x = m_aWaveWaterVB[i - 3].x + fOffsetX;
			m_aWaveWaterVB[i].z = m_aWaveWaterVB[i - 3].z + fOffsetZ;
			m_aWaveWaterVB[i].y = m_aWaveWaterVB[i - 3].y;
		}

		CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->m_wndToolCtrlBar.m_pageAtmosEdit.SetWaveWaterSegment( m_nWaterSegment );
	}

	m_bModify[2] = TRUE;
}


//-----------------------------------------------------------------------------
// Name : MapMovement()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::MapMovement( int nUnit, int nZ, int nX )
{
	float	fX, fZ;

	if( nUnit == 0 )	// Tile coordinate
	{
		int	nMapWidth = m_pTerrain->GetHMWidth() - 2;
		if( nX < 0 || nZ < 0 || nX > nMapWidth || nZ > nMapWidth )
			return ;
		fX = nX * m_pTerrain->GetPatchSize();
		fZ = ( nMapWidth - nZ ) * m_pTerrain->GetPatchSize();
	}
	else				// World coordinate
	{
		int	nMapWidth = ( m_pTerrain->GetHMWidth() - 1 ) * m_pTerrain->GetPatchSize();
		if( nX < 0 || nZ < 0 || nX >= nMapWidth || nZ >= nMapWidth )
			return ;
		fX = nX;	fZ = nZ;
	}

	// Camera reset
	m_pRoh->SetInitPos( fX, fZ );
	m_vTarget = D3DXVECTOR3( m_pRoh->m_wx, m_pRoh->m_wy, m_pRoh->m_wz );
	if( m_bShowCharacter )
		m_vTarget.y += 15.0f;
	m_pCamera->SetTarget( m_vTarget );
	m_pCamera->UpdateViewMatrix();
	if( m_nCameraType == 1 )
		m_pCamera->UpdateFlyViewMatrix();
}


//-----------------------------------------------------------------------------
// Name : SetCameraType()
// Desc :
//-----------------------------------------------------------------------------
void CMyNKApp::SetCameraType( int nType )
{
	if( m_nCameraType != nType )
	{
		m_nCameraType = nType;

		if( m_nCameraType == 0 )
		{
			m_pRoh->SetInitPos( m_pRoh->m_wx, m_pRoh->m_wz );
			m_vTarget = D3DXVECTOR3( m_pRoh->m_wx, m_pRoh->m_wy, m_pRoh->m_wz );
			if( m_bShowCharacter )
				m_vTarget.y += 15.0f;
			m_pCamera->SetTarget( m_vTarget );
			m_pCamera->UpdateViewMatrix();
		}
	}
}


//-----------------------------------------------------------------------------
// Name : MergeMaps()
// Desc :
//-----------------------------------------------------------------------------
BOOL CMyNKApp::MergeMaps( const TCHAR* szNmfName, const TCHAR* szNotName,
							int nSZ, int nSX, int nEZ, int nEX )
{
	nSZ = 255 - nSZ;		nEZ = 255 - nEZ;
	int	nTemp = nEZ;
	nEZ = nSZ;				nSZ = nTemp;

	CString	strFullPath;
	strFullPath.Format( _T("map\\%s\\%s"), m_pTerrain->GetTerrainName(), szNmfName );
	if( !m_pTerrain->MergeNmf( strFullPath, nSZ, nSX, nEZ, nEX ) )
		return FALSE;

	if( _tcscmp( szNotName, _T("") ) != 0 )
	{
		strFullPath.Format( _T("data\\building\\%s\\%s"), m_pTerrain->GetTerrainName(), szNotName );
		if( !CNKObjQuadNode::MergeNot( strFullPath, nSZ, nSX, nEZ, nEX ) )
			return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name : ()
// Desc :
//-----------------------------------------------------------------------------

void CMyNKApp::DragMouse()
{
	m_pCamera->GetPickingRay( m_vPickOrigin, m_vPickDirection, m_nMouseDragStartX, m_nMouseDragStartZ);
	// Get a picked point
	m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );
	m_vMouseDrag1 = m_vPickedPoint;	//ulyssesme-20060928
	
	m_pCamera->GetPickingRay( m_vPickOrigin, m_vPickDirection, m_nMouseDragEndX, m_nMouseDragStartZ);
	// Get a picked point
	m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );
	m_vMouseDrag2 = m_vPickedPoint;	//ulyssesme-20060928
	
	m_pCamera->GetPickingRay( m_vPickOrigin, m_vPickDirection, m_nMouseDragEndX, m_nMouseDragEndZ);
	// Get a picked point
	m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );
	m_vMouseDrag3 = m_vPickedPoint;	//ulyssesme-20060928
	
	m_pCamera->GetPickingRay( m_vPickOrigin, m_vPickDirection, m_nMouseDragStartX, m_nMouseDragEndZ);
	// Get a picked point
	m_pTerrain->GetPickedPoint( m_vPickOrigin, m_vPickDirection, m_vPickedPoint );
	m_vMouseDrag4 = m_vPickedPoint;	//ulyssesme-20060928
	if ((m_nMouseDragEndX - m_nMouseDragStartX < 0 && m_nMouseDragEndZ - m_nMouseDragStartZ >= 0)
		|| (m_nMouseDragEndX - m_nMouseDragStartX >= 0 && m_nMouseDragEndZ - m_nMouseDragStartZ < 0))
	{
		D3DXVECTOR3 vTemp = m_vMouseDrag2;
		m_vMouseDrag2 = m_vMouseDrag4;
		m_vMouseDrag4 = vTemp;
	}
	
	if (!m_bAdd)	//컨트롤 키가 안눌려 있으면 기존 선택 지우기
	{
		m_lstSelDecoObjs.erase( m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end() );
		m_bAdd = TRUE;
	}
	//이전 선택 영역 지우기
	/*for (list<CDecoObj*>::iterator pOldDrag = m_lstDragSelDecoObjs.begin(); pOldDrag != m_lstDragSelDecoObjs.end(); pOldDrag++)
	{
		list<CDecoObj*>::iterator pOldObj = find(m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end(), *pOldDrag);
		if (*pOldObj == *pOldDrag)
		{
			m_lstSelDecoObjs.erase(pOldObj);
			UpdateObjEditParameters( *pOldDrag );
			MakeNodeBoundMesh();
			MakeDecoBoxMesh();
		}
	}*/
	//m_lstDragSelDecoObjs.clear();
	//새로운 선택 영역
	list<CDecoObj*> pDecoObjByDrag;
	int nObjByDrag = 0;
	g_pObjTree->GetPickedDecoObjByDrag(m_vMouseDrag1, m_vMouseDrag2, m_vMouseDrag3, m_vMouseDrag4, pDecoObjByDrag, nObjByDrag);
	if (!m_bDeselect)	//선택모드
	{
		int nMaxSelect = 0;
		for (list<CDecoObj*>::iterator pNewDrag = pDecoObjByDrag.begin(); pNewDrag != pDecoObjByDrag.end(); pNewDrag++)
		{
			if (m_lstSelDecoObjs.size() > 10000)	break;
			list<CDecoObj*>::iterator pMulObj = find(m_lstDragSelDecoObjs.begin(), m_lstDragSelDecoObjs.end(), *pNewDrag);
			list<CDecoObj*>::iterator pOldObj = find(m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end(), *pNewDrag);
			if (*pMulObj != *pNewDrag && *pOldObj != *pNewDrag)
			{
				CDecoObj * pNewDeco = *pNewDrag;
				m_lstDragSelDecoObjs.push_back(pNewDeco);
				m_lstSelDecoObjs.push_back(pNewDeco);
				//UpdateObjEditParameters(pNewDeco);
			}
		}
		CDecoObj **pRemoveList;
		pRemoveList = new CDecoObj * [m_lstDragSelDecoObjs.size()];
		int nRemoveCount = 0;
		for (list<CDecoObj*>::iterator pMulObj = m_lstDragSelDecoObjs.begin(); pMulObj != m_lstDragSelDecoObjs.end(); pMulObj++)
		{
			CDecoObj * pMulDeco = *pMulObj;
			list<CDecoObj*>::iterator pNewDrag = find(pDecoObjByDrag.begin(), pDecoObjByDrag.end(), pMulDeco);
			CDecoObj * pNewDeco = (pDecoObjByDrag.end() == pNewDrag ? NULL: *pNewDrag);
			if (pMulDeco != pNewDeco)	//선택되지 않으면 제거
			{
				pRemoveList[nRemoveCount] = pMulDeco;
				nRemoveCount++;
			}
		}
		for (int nCurRemove = 0; nCurRemove < nRemoveCount; nCurRemove++)
		{
			list<CDecoObj *>::iterator pRemove = find(m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end(), pRemoveList[nCurRemove]);
			list<CDecoObj *>::iterator pRemove2 = find(m_lstDragSelDecoObjs.begin(), m_lstDragSelDecoObjs.end(), pRemoveList[nCurRemove]);
			if (pRemove != m_lstSelDecoObjs.end())
			{
				m_lstSelDecoObjs.erase(pRemove);
				m_lstDragSelDecoObjs.erase(pRemove2);
				//UpdateObjEditParameters(pRemoveList[nCurRemove]);
			}
		}
		delete [] pRemoveList;
	}
	else	//해제모드
	{
		for (list<CDecoObj*>::iterator pNewDrag = pDecoObjByDrag.begin(); pNewDrag != pDecoObjByDrag.end(); pNewDrag++)
		{
			list<CDecoObj*>::iterator pMulObj = find(m_lstDragSelDecoObjs.begin(), m_lstDragSelDecoObjs.end(), *pNewDrag);
			list<CDecoObj*>::iterator pOldObj = find(m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end(), *pNewDrag);
			if (*pMulObj != *pNewDrag && *pOldObj == *pNewDrag)
			{
				CDecoObj * pNewDeco = *pNewDrag;
				m_lstDragSelDecoObjs.push_back(pNewDeco);
				m_lstSelDecoObjs.erase(pOldObj);
				//UpdateObjEditParameters(pNewDeco);
			}
		}
		CDecoObj **pRemoveList;
		pRemoveList = new CDecoObj * [m_lstDragSelDecoObjs.size()];
		int nRemoveCount = 0;
		for (list<CDecoObj*>::iterator pMulObj = m_lstDragSelDecoObjs.begin(); pMulObj != m_lstDragSelDecoObjs.end(); pMulObj++)
		{
			CDecoObj * pMulDeco = *pMulObj;
			list<CDecoObj*>::iterator pNewDrag = find(pDecoObjByDrag.begin(), pDecoObjByDrag.end(), pMulDeco);
			CDecoObj * pNewDeco = (pDecoObjByDrag.end() == pNewDrag ? NULL: *pNewDrag);
			if (pMulDeco != pNewDeco)	//선택되지 않으면 추가
			{
				pRemoveList[nRemoveCount] = pMulDeco;
				nRemoveCount++;
			}
		}
		for (int nCurRemove = 0; nCurRemove < nRemoveCount; nCurRemove++)
		{
			if (pRemoveList[nCurRemove])
			{
				list<CDecoObj *>::iterator pRemove = find(m_lstSelDecoObjs.begin(), m_lstSelDecoObjs.end(), pRemoveList[nCurRemove]);
				list<CDecoObj *>::iterator pRemove2 = find(m_lstDragSelDecoObjs.begin(), m_lstDragSelDecoObjs.end(), pRemoveList[nCurRemove]);
				if (pRemove == m_lstSelDecoObjs.end() && pRemove2 != m_lstDragSelDecoObjs.end())
				{
					m_lstSelDecoObjs.push_back(pRemoveList[nCurRemove]);
					m_lstDragSelDecoObjs.erase(pRemove2);
					//UpdateObjEditParameters(pRemoveList[nCurRemove]);
				}
			}
		}
		delete [] pRemoveList;
	}
	pDecoObjByDrag.clear();
	MakeNodeBoundMesh();
	MakeDecoBoxMesh();
	CalcGizmoPosition();
}


// [2007/11/1 theodoric] undo
void CMyNKApp::MoveAndRotateSelDecoObjs(CDecoObj* pDecoObj, float fdX, float fdY, float fdZ, float fdir) 
{
	pDecoObj->m_wx = fdX;
	pDecoObj->m_wy = fdY;
	pDecoObj->m_wz = fdZ;
	pDecoObj->m_dir= fdir;
	pDecoObj->CalBoundBox();

	MakeDecoBoxMesh();
	CalcGizmoPosition();

	if( m_lstSelDecoObjs.size() == 1 )
		UpdateObjEditParameters( pDecoObj );
	else
		UpdateObjEditParameters();
}


// [2007/11/1 theodoric] 카메라 탑뷰
void CMyNKApp::SetCameraTopView(BOOL topView)
{
	CMainFrame*	pFrame = NULL;
	pFrame = (CMainFrame*)AfxGetMainWnd();

	if(!IsCameraTopView())
	{
		m_bCameraUnlimitBack = m_bCameraUnlimit;
		m_bCameraUnlimit = FALSE;
	}
	else
	{
		m_bCameraUnlimit = m_bCameraUnlimitBack;
	}
	
	pFrame->OnCameraUnlimitUpdate(); 
	m_pCamera->SetTopView(topView);
	m_pCamera->SetRecalViewMatrix(true);
	m_pCamera->UpdateViewMatrix();
	FrameMove();
	Render();
}

