// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "NKMapTool.h"

#include "MainFrm.h"
#include "ToolMapMoveDlg.h"
#include "MyNKApp.h"

#include "NkMath.h"  ///071022 추가


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_COMMAND(ID_TOOL_VERTEX_EDIT, OnToolVertexEdit)
	ON_COMMAND(ID_TOOL_TEXTURE_PAINT, OnToolTexturePaint)
	ON_COMMAND(ID_TOOL_OBJECT_EDIT, OnToolObjectEdit)
	ON_UPDATE_COMMAND_UI(ID_TOOL_VERTEX_EDIT, OnUpdateToolVertexEdit)
	ON_UPDATE_COMMAND_UI(ID_TOOL_TEXTURE_PAINT, OnUpdateToolTexturePaint)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OBJECT_EDIT, OnUpdateToolObjectEdit)
	ON_COMMAND(ID_VIEW_GRID_NONE, OnViewGridNone)
	ON_COMMAND(ID_VIEW_GRID_SMALL, OnViewGridSmall)
	ON_COMMAND(ID_VIEW_GRID_MEDIUM, OnViewGridMedium)
	ON_COMMAND(ID_VIEW_GRID_LARGE, OnViewGridLarge)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID_NONE, OnUpdateViewGridNone)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID_SMALL, OnUpdateViewGridSmall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID_MEDIUM, OnUpdateViewGridMedium)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID_LARGE, OnUpdateViewGridLarge)
	ON_COMMAND(ID_VIEW_OBJECT, OnViewObject)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TERRAIN, OnUpdateViewTerrain)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECT, OnUpdateViewObject)
	ON_COMMAND(ID_CAMERA_OBSERVE, OnCameraObserve)
	ON_COMMAND(ID_CAMERA_FLY, OnCameraFly)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_OBSERVE, OnUpdateCameraObserve)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_FLY, OnUpdateCameraFly)
	ON_COMMAND(ID_TOOL_VERTEX_EDIT_ICON, OnToolVertexEditIcon)
	ON_COMMAND(ID_TOOL_TEXTURE_PAINT_ICON, OnToolTexturePaintIcon)
	ON_COMMAND(ID_TOOL_OBJECT_EDIT_ICON, OnToolObjectEditIcon)
	ON_COMMAND(ID_TOOL_ATTRIBUTE_EDIT, OnToolAttributeEdit)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ATTRIBUTE_EDIT, OnUpdateToolAttributeEdit)
	ON_COMMAND(ID_TOOL_ATTRIBUTE_EDIT_ICON, OnToolAttributeEditIcon)
	ON_COMMAND(ID_VIEW_FOG, OnViewFog)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FOG, OnUpdateViewFog)
	ON_COMMAND(ID_VIEW_TERRAIN, OnViewTerrain)
	ON_COMMAND(ID_VIEW_ATTR_MAP, OnViewAttr_Map)
	ON_COMMAND(ID_TOOL_LIGHT_EDIT, OnToolLightEdit)
	ON_UPDATE_COMMAND_UI(ID_TOOL_LIGHT_EDIT, OnUpdateToolLightEdit)
	ON_COMMAND(ID_TOOL_LIGHT_EDIT_ICON, OnToolLightEditIcon)
	ON_COMMAND(ID_VIEW_CHARACTER, OnViewCharacter)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHARACTER, OnUpdateViewCharacter)
	ON_COMMAND(ID_VIEW_SKY, OnViewSky)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SKY, OnUpdateViewSky)
	ON_COMMAND(ID_TOOL_ATMOSPHERE_EDIT, OnToolAtmosphereEdit)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ATMOSPHERE_EDIT, OnUpdateToolAtmosphereEdit)
	ON_COMMAND(ID_TOOL_ATMOS_EDIT_ICON, OnToolAtmosEditIcon)
	ON_COMMAND(ID_TOOL_MAPMOVE, OnToolMapmove)
	ON_UPDATE_COMMAND_UI(ID_TOOL_MAPMOVE, OnUpdateToolMapmove)
	ON_COMMAND(ID_TOOL_MAPMOVE_ICON, OnToolMapmoveIcon)
	ON_COMMAND(ID_VIEW_WATER, OnViewWater)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WATER, OnUpdateViewWater)
	ON_COMMAND(ID_CAMERA_SPEED1X, OnCameraSpeed1x)
	ON_COMMAND(ID_CAMERA_SPEED2X, OnCameraSpeed2x)
	ON_COMMAND(ID_CAMERA_SPEED4X, OnCameraSpeed4x)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEED1X, OnUpdateCameraSpeed1x)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEED2X, OnUpdateCameraSpeed2x)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEED4X, OnUpdateCameraSpeed4x)
	ON_COMMAND(ID_CAMERA_SPEED8X, OnCameraSpeed8x)
	ON_COMMAND(ID_CAMERA_SPEED10X, OnCameraSpeed10x)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEED10X, OnUpdateCameraSpeed10x)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SPEED8X, OnUpdateCameraSpeed8x)
	ON_COMMAND(ID_CAMERA_UNLIMIT, OnCameraUnlimit)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_UNLIMIT, OnUpdateCameraUnlimit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
//	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_COPYRIGHT,
	ID_INDICATOR_FPS,
	ID_INDICATOR_TIME,
	ID_INDICATOR_COORD,	
	ID_INDICATOR_TRIS,
	ID_INDICATOR_SIZE,	
};


// Global variable
extern CMyNKApp*	g_pMyNKApp;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if( !CreateToolBar() )
	{
		TRACE0( _T("Failed to create toolbar\n") );
		return -1;      // fail to create
	}
	
	if( !CreateDockingBar() )
	{
		TRACE0( _T("Failed to create docking bar\n") );
		return -1;      // fail to create
	}

	SetTimer( 1, 1000, NULL );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::CreateDockingBar()
{
	if( !m_wndToolCtrlBar.Create( this ) ) return FALSE;
	
	m_wndToolCtrlBar.EnableDocking( CBRS_ALIGN_RIGHT );
	DockControlBar( &m_wndToolCtrlBar, AFX_IDW_DOCKBAR_RIGHT );

	m_wndToolCtrlBar.SetFloatFixedWidth( TRUE );
	m_wndToolCtrlBar.SetMinSize( CSize( 242, 10 ) );
	m_wndToolCtrlBar.SetDockingFixedWidth( TRUE );

	ShowControlBar( &m_wndToolCtrlBar, FALSE, FALSE );

	return TRUE;
}


BOOL CMainFrame::CreateToolBar()
{
	EnableDocking( CBRS_ALIGN_ANY );

	if( !m_wndToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) || !m_wndToolBar.LoadToolBar( IDR_MAINFRAME ) )
		return FALSE;

	m_wndToolBar.EnableDocking( CBRS_ALIGN_TOP );
	DockControlBar( &m_wndToolBar );

	return TRUE;
}


void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
	// TODO: Add your message handler code here
	if( g_pMyNKApp)
	{
		CRect	rc;
		if( !m_wndToolBar.IsFloating() )
		{
			m_wndToolBar.GetWindowRect( &rc );
			y += rc.Height();
		}
		
		g_pMyNKApp->MsgProc( WM_MOVE, NULL, MAKELPARAM( (WORD)x, (WORD)y ) );
	}
}


void CMainFrame::OnToolVertexEdit() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
	{
		if( m_wndToolCtrlBar.IsVisible() )
		{
			if( g_pMyNKApp->GetWorkingState() == VERTEX_EDIT )
			{
				g_pMyNKApp->SetWorkingState( NONE );
				ShowControlBar( &m_wndToolCtrlBar, FALSE, FALSE );
			}
			else
			{
				g_pMyNKApp->SetWorkingState( VERTEX_EDIT );
				m_wndToolCtrlBar.UpdatePage();
			}
		}
		else
		{
			g_pMyNKApp->SetWorkingState( VERTEX_EDIT );
			m_wndToolCtrlBar.UpdatePage();
			ShowControlBar( &m_wndToolCtrlBar, TRUE, FALSE );
		}
	}
}


void CMainFrame::OnToolVertexEditIcon() 
{
	// TODO: Add your command handler code here
	OnToolVertexEdit();
}


void CMainFrame::OnToolTexturePaint() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
	{
		if( m_wndToolCtrlBar.IsVisible() )
		{
			if( g_pMyNKApp->GetWorkingState() == TEXTURE_PAINT )
			{
				g_pMyNKApp->SetWorkingState( NONE );
				ShowControlBar( &m_wndToolCtrlBar, FALSE, FALSE );
			}
			else
			{
				g_pMyNKApp->SetWorkingState( TEXTURE_PAINT );
				m_wndToolCtrlBar.UpdatePage();
			}
		}
		else
		{
			g_pMyNKApp->SetWorkingState( TEXTURE_PAINT );
			m_wndToolCtrlBar.UpdatePage();
			ShowControlBar( &m_wndToolCtrlBar, TRUE, FALSE );
		}
	}
}


void CMainFrame::OnToolTexturePaintIcon() 
{
	// TODO: Add your command handler code here
	OnToolTexturePaint();
}


void CMainFrame::OnToolObjectEdit() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
	{
		if( m_wndToolCtrlBar.IsVisible() )
		{
			if( g_pMyNKApp->GetWorkingState() == OBJECT_EDIT )
			{
				g_pMyNKApp->SetWorkingState( NONE );
				ShowControlBar( &m_wndToolCtrlBar, FALSE, FALSE );
			}
			else
			{
				g_pMyNKApp->SetWorkingState( OBJECT_EDIT );
				m_wndToolCtrlBar.UpdatePage();
			}
		}
		else
		{
			g_pMyNKApp->SetWorkingState( OBJECT_EDIT );
			m_wndToolCtrlBar.UpdatePage();
			ShowControlBar( &m_wndToolCtrlBar, TRUE, FALSE );
		}
	}
}


void CMainFrame::OnToolObjectEditIcon() 
{
	// TODO: Add your command handler code here
	OnToolObjectEdit();
}


void CMainFrame::OnToolLightEdit() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
	{
		if( m_wndToolCtrlBar.IsVisible() )
		{
			if( g_pMyNKApp->GetWorkingState() == LIGHT_EDIT )
			{
				g_pMyNKApp->SetWorkingState( NONE );
				ShowControlBar( &m_wndToolCtrlBar, FALSE, FALSE );
			}
			else
			{
				g_pMyNKApp->SetWorkingState( LIGHT_EDIT );
				m_wndToolCtrlBar.UpdatePage();
			}
		}
		else
		{
			g_pMyNKApp->SetWorkingState( LIGHT_EDIT );
			m_wndToolCtrlBar.UpdatePage();
			ShowControlBar( &m_wndToolCtrlBar, TRUE, FALSE );
		}
	}
}


void CMainFrame::OnToolLightEditIcon() 
{
	// TODO: Add your command handler code here
	OnToolLightEdit();
}


void CMainFrame::OnToolAtmosphereEdit() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
	{
		if( m_wndToolCtrlBar.IsVisible() )
		{
			if( g_pMyNKApp->GetWorkingState() == ATMOSPHERE_EDIT )
			{
				g_pMyNKApp->SetWorkingState( NONE );
				ShowControlBar( &m_wndToolCtrlBar, FALSE, FALSE );
			}
			else
			{
				g_pMyNKApp->SetWorkingState( ATMOSPHERE_EDIT );
				m_wndToolCtrlBar.UpdatePage();
			}
		}
		else
		{
			g_pMyNKApp->SetWorkingState( ATMOSPHERE_EDIT );
			m_wndToolCtrlBar.UpdatePage();
			ShowControlBar( &m_wndToolCtrlBar, TRUE, FALSE );
		}		
	}
}


void CMainFrame::OnToolAtmosEditIcon() 
{
	// TODO: Add your command handler code here
	OnToolAtmosphereEdit();
}


void CMainFrame::OnToolAttributeEdit() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		if( m_wndToolCtrlBar.IsVisible() )
		{
			if( g_pMyNKApp->GetWorkingState() == ATTRIBUTE_EDIT )
			{
				g_pMyNKApp->SetWorkingState( NONE );
				ShowControlBar( &m_wndToolCtrlBar, FALSE, FALSE );
			}
			else
			{
				g_pMyNKApp->SetWorkingState( ATTRIBUTE_EDIT );
				m_wndToolCtrlBar.UpdatePage();
			}
		}
		else
		{
			g_pMyNKApp->SetWorkingState( ATTRIBUTE_EDIT );
			m_wndToolCtrlBar.UpdatePage();
			ShowControlBar( &m_wndToolCtrlBar, TRUE, FALSE );
		}		

		// 브러쉬 크기를 다이얼로그의 에디트 박스에 기억된 값으로 다시 셋팅해준다.( 여기서 해주는게 작업하기 편해서 여기다 함.(by 원석) )
		// 다른 창의 경우 이런 방식을 사용하지 않는다. 다른 창은 매 갱신 시간마다 다시 셋팅해주더라.(비효율적이라고 생각해 새로 추가된 이녀석은 여기다 함.)(by 원석)
		g_pMyNKApp->SetBrushSize( m_wndToolCtrlBar.m_pageAttrEdit.m_nBrushSize ); 

	}
}


void CMainFrame::OnToolAttributeEditIcon() 
{
	// TODO: Add your command handler code here
	OnToolAttributeEdit();
}


void CMainFrame::OnToolMapmove() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		CToolMapMoveDlg	MapMoveDlg;

		if( MapMoveDlg.DoModal() == IDOK )
		{
			g_pMyNKApp->MapMovement( MapMoveDlg.m_nMoveUnit, MapMoveDlg.m_nCoordZ, MapMoveDlg.m_nCoordX );
		}
	}
}


void CMainFrame::OnToolMapmoveIcon() 
{
	// TODO: Add your command handler code here
	OnToolMapmove();
}


void CMainFrame::OnUpdateToolVertexEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
		{
			pCmdUI->Enable( TRUE );
			if( g_pMyNKApp->GetWorkingState() == VERTEX_EDIT )
				pCmdUI->SetCheck( 1 );
			else
				pCmdUI->SetCheck( 0 );
		}
		else
		{
			pCmdUI->SetCheck( 0 );
			pCmdUI->Enable( FALSE );
		}
	}
}


void CMainFrame::OnUpdateToolTexturePaint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
		{
			pCmdUI->Enable( TRUE );
			if( g_pMyNKApp->GetWorkingState() == TEXTURE_PAINT )
				pCmdUI->SetCheck( 1 );
			else
				pCmdUI->SetCheck( 0 );
		}
		else
		{
			pCmdUI->SetCheck( 0 );
			pCmdUI->Enable( FALSE );
		}
	}
}


void CMainFrame::OnUpdateToolObjectEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
		{
			pCmdUI->Enable( TRUE );
			if( g_pMyNKApp->GetWorkingState() == OBJECT_EDIT )
				pCmdUI->SetCheck( 1 );
			else
				pCmdUI->SetCheck( 0 );
		}
		else
		{
			pCmdUI->SetCheck( 0 );
			pCmdUI->Enable( FALSE );
		}
	}
}


void CMainFrame::OnUpdateToolLightEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
		{
			pCmdUI->Enable( TRUE );
			if( g_pMyNKApp->GetWorkingState() == LIGHT_EDIT )
				pCmdUI->SetCheck( 1 );
			else
				pCmdUI->SetCheck( 0 );
		}
		else
		{
			pCmdUI->SetCheck( 0 );
			pCmdUI->Enable( FALSE );
		}
	}
}


void CMainFrame::OnUpdateToolAtmosphereEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() && !g_pMyNKApp->IsLoadedGameMap() )
		{
			pCmdUI->Enable( TRUE );
			if( g_pMyNKApp->GetWorkingState() == ATMOSPHERE_EDIT )
				pCmdUI->SetCheck( 1 );
			else
				pCmdUI->SetCheck( 0 );
		}
		else
		{
			pCmdUI->SetCheck( 0 );
			pCmdUI->Enable( FALSE );
		}
	}
}


void CMainFrame::OnUpdateToolAttributeEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() )
		{
			pCmdUI->Enable( TRUE );
			if( g_pMyNKApp->GetWorkingState() == ATTRIBUTE_EDIT )
				pCmdUI->SetCheck( 1 );
			else
				pCmdUI->SetCheck( 0 );
		}
		else
		{
			pCmdUI->SetCheck( 0 );
			pCmdUI->Enable( FALSE );
		}
	}
}


void CMainFrame::OnUpdateToolMapmove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() )
			pCmdUI->Enable( TRUE );
		else
			pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnViewTerrain() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->ToggleShowTerrain();
}

void CMainFrame::OnViewAttr_Map()
{
	if( g_pMyNKApp )
		g_pMyNKApp->ToggleShowAttr_Map();
}


void CMainFrame::OnViewObject() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->ToggleShowObject();		
}


void CMainFrame::OnViewGridNone() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->SetGridSize( -1 );
}


void CMainFrame::OnViewGridSmall() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->SetGridSize( 0 );
}


void CMainFrame::OnViewGridMedium() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->SetGridSize( 1 );
}


void CMainFrame::OnViewGridLarge() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->SetGridSize( 2 );
}

void CMainFrame::OnViewCharacter() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetCameraType() == 0 )
		g_pMyNKApp->ToggleShowCharacter();
}


void CMainFrame::OnViewWater() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->ToggleShowWater();
}


void CMainFrame::OnViewSky() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->ToggleShowSky();
}


void CMainFrame::OnViewFog() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->ToggleShowFog();
}


void CMainFrame::OnCameraObserve() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->SetCameraType( 0 );
}


void CMainFrame::OnCameraFly() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
		g_pMyNKApp->SetCameraType ( 1 );
}


void CMainFrame::OnUpdateViewGridNone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetGridSize() == -1 )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewGridSmall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() &&
		g_pMyNKApp->GetWorkingState() != OBJECT_EDIT )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetGridSize() == 0 )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewGridMedium(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() &&
		g_pMyNKApp->GetWorkingState() != OBJECT_EDIT )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetGridSize() == 1 )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewGridLarge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() &&
		g_pMyNKApp->GetWorkingState() != ATMOSPHERE_EDIT )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetGridSize() == 2 )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewCharacter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() && g_pMyNKApp->GetCameraType() == 0 )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetShowCharacter() )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewSky(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetShowSky() )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewFog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetShowFog() )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewTerrain(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetShowTerrain() )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewObject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetShowObject() )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateViewWater(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetShowWater() )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateCameraObserve(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetCameraType() == 0 )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::OnUpdateCameraFly(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp && g_pMyNKApp->GetWorkingFilename() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pMyNKApp->GetCameraType() == 1 )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnCameraSpeed1x() 
{
	// TODO: Add your command handler code here
	g_pMyNKApp->m_fCameraSpeed = 1.0f; // 카메라 스피드 조절 기능 확장(by 원석)
}

void CMainFrame::OnCameraSpeed2x() 
{
	// TODO: Add your command handler code here
	g_pMyNKApp->m_fCameraSpeed = 2.0f; // 카메라 스피드 조절 기능 확장(by 원석)
}

void CMainFrame::OnCameraSpeed4x() 
{
	// TODO: Add your command handler code here
	g_pMyNKApp->m_fCameraSpeed = 4.0f; // 카메라 스피드 조절 기능 확장(by 원석)
}

void CMainFrame::OnUpdateCameraSpeed1x(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp->m_fCameraSpeed == 1.0f )
	{
		pCmdUI->SetCheck( TRUE );
	}
	else
	{
		pCmdUI->SetCheck( FALSE );
	}	
}

void CMainFrame::OnUpdateCameraSpeed2x(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp->m_fCameraSpeed == 2.0f )
	{
		pCmdUI->SetCheck( TRUE );
	}
	else
	{
		pCmdUI->SetCheck( FALSE );
	}	
}

void CMainFrame::OnUpdateCameraSpeed4x(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp->m_fCameraSpeed == 4.0f )
	{
		pCmdUI->SetCheck( TRUE );
	}
	else
	{
		pCmdUI->SetCheck( FALSE );
	}	
}

void CMainFrame::OnCameraSpeed8x() 
{
	// TODO: Add your command handler code here
	g_pMyNKApp->m_fCameraSpeed = 8.0f; // 카메라 스피드 조절 기능 확장(by 원석)
}

void CMainFrame::OnCameraSpeed10x() 
{
	// TODO: Add your command handler code here
	g_pMyNKApp->m_fCameraSpeed = 10.0f; // 카메라 스피드 조절 기능 확장(by 원석)
}

void CMainFrame::OnUpdateCameraSpeed8x(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp->m_fCameraSpeed == 8.0f )
	{
		pCmdUI->SetCheck( TRUE );
	}
	else
	{
		pCmdUI->SetCheck( FALSE );
	}		
}

void CMainFrame::OnUpdateCameraSpeed10x(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp->m_fCameraSpeed == 10.0f )
	{
		pCmdUI->SetCheck( TRUE );
	}
	else
	{
		pCmdUI->SetCheck( FALSE );
	}		
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT tempID;
	UINT tempStyle;
	int Width;

	GetStatusBar()->GetPaneInfo( 3, tempID, tempStyle, Width );
	GetStatusBar()->SetPaneInfo( 3, tempID, tempStyle, 450 );
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CMainFrame::OnCameraUnlimit() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp->m_bCameraUnlimit ) // 카메라 시야제한을 없애는 체크 기능. ///071022 추가
	{
		g_pMyNKApp->m_bCameraUnlimit = FALSE;
		CNKObjQuadNode::SetRenderDist( 3500.0f );
		CNKObjQuadNode::SetBlendDist( 2000.0f );
		g_pMyNKApp->GetCamera()->UpdateProjMatrix( NK_QUATERPI, g_pMyNKApp->GetClientWidth(), g_pMyNKApp->GetClientHeight(), 30.0f, 10000.0f, 30.0f, 100000.0f ); ///071102 플러스텀 far 값 조절.(해당 값은 버택스 버퍼 메모리 할당에 영향을 미치기 때문에 미리 늘려둔다.)
		g_pMyNKApp->GetD3DDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION,	(LPD3DMATRIX)( &(g_pMyNKApp->GetCamera()->GetProjMatrix()) ) );
	}
	else
	{
		g_pMyNKApp->m_bCameraUnlimit = TRUE;
		CNKObjQuadNode::SetRenderDist( 35000.0f );
		CNKObjQuadNode::SetBlendDist( 20000.0f );
		g_pMyNKApp->GetD3DDevice()->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
		g_pMyNKApp->GetCamera()->UpdateProjMatrix( NK_QUATERPI, g_pMyNKApp->GetClientWidth(), g_pMyNKApp->GetClientHeight(), 30.0f, 1000000.0f, 30.0f, 100000.0f );
		g_pMyNKApp->GetD3DDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION,	(LPD3DMATRIX)( &(g_pMyNKApp->GetCamera()->GetProjMatrix()) ) );
	}
}

void CMainFrame::OnUpdateCameraUnlimit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pMyNKApp->m_bCameraUnlimit )  // 카메라 시야제한을 없애는 체크 기능. ///071022 추가
	{
		pCmdUI->SetCheck( 1 );
	}
	else
	{
		pCmdUI->SetCheck( 0 );
	}
}
