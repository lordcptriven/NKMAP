// ToolVertEditPage.cpp : implementation file
//

#include "stdafx.h"
#include "NKMapTool.h"
#include "ToolVertEditPage.h"
#include "MyNKApp.h"
#include "MyFileDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global variables
extern CMyNKApp*	g_pMyNKApp;
extern CNKObjQuadNode*	g_pObjTree;


/////////////////////////////////////////////////////////////////////////////
// CToolVertEditPage dialog


CToolVertEditPage::CToolVertEditPage(CWnd* pParent /*=NULL*/)
	: CPageCtrl(CToolVertEditPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolVertEditPage)
	m_strName = _T("None");
	m_nSegment = 0;
	m_nPatchSize = 0;
	m_nHeightScale = 0;
	m_strMinMaxHeight = _T("0 / 0");
	m_nControlUnit = 0;
	m_nBrushSize = 1;
	m_nBrushStrength = 100;
	m_nOffset = 0;
	m_nEditType = 0;
	//}}AFX_DATA_INIT
}


void CToolVertEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPageCtrl::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolVertEditPage)
	DDX_Control(pDX, IDC_TOOL_VERTEX_BRUSH_STRENGTH_SLIDER, m_ctrlBrushStrength);
	DDX_Control(pDX, IDC_TOOL_VERTEX_BRUSH_SIZE_SLIDER, m_ctrlBrushSize);
	DDX_Control(pDX, IDC_TOOL_VERTEX_HEIGHTSCALE, m_ctrlHeightScale);
	DDX_Control(pDX, IDC_TOOL_VERTEX_PATCHSIZE, m_ctrlPatchSize);
	DDX_Text(pDX, IDC_TOOL_VERTEX_NAME, m_strName);
	DDX_Text(pDX, IDC_TOOL_VERTEX_SEGMENT, m_nSegment);
	DDX_Text(pDX, IDC_TOOL_VERTEX_PATCHSIZE, m_nPatchSize);
	DDX_Text(pDX, IDC_TOOL_VERTEX_HEIGHTSCALE, m_nHeightScale);
	DDX_Text(pDX, IDC_TOOL_VERTEX_MINMAXHEIGHT, m_strMinMaxHeight);
	DDX_Radio(pDX, IDC_TOOL_VERTEX_CONTROL_TILE, m_nControlUnit);
	DDX_Text(pDX, IDC_TOOL_VERTEX_BRUSH_SIZE_EDIT, m_nBrushSize);
	DDX_Text(pDX, IDC_TOOL_VERTEX_BRUSH_STRENGTH_EDIT, m_nBrushStrength);
	DDX_Text(pDX, IDC_TOOL_VERTEX_OFFSET, m_nOffset);
	DDX_Radio(pDX, IDC_TOOL_VERTEX_RAISE, m_nEditType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolVertEditPage, CPageCtrl)
	//{{AFX_MSG_MAP(CToolVertEditPage)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_PATCHSIZE_MODIFY, OnToolVertexPatchsizeModify)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_HEIGHTSCALE_MODIFY, OnToolVertexHeightscaleModify)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_HEIGHTMAP_REAPPLY, OnToolVertexHeightmapReapply)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_RAISE_ENTIRE, OnToolVertexRaiseEntire)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_LOWER_ENTIRE, OnToolVertexLowerEntire)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_CONTROL_TILE, OnToolVertexControlTile)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_CONTROL_VERTEX, OnToolVertexControlVertex)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_RAISE, OnToolVertexRaise)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_LOWER, OnToolVertexLower)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_FLATTEN, OnToolVertexFlatten)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_SMOOTH, OnToolVertexSmooth)
	ON_EN_CHANGE(IDC_TOOL_VERTEX_BRUSH_SIZE_EDIT, OnChangeToolVertexBrushSizeEdit)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_TOOL_VERTEX_BRUSH_STRENGTH_EDIT, OnChangeToolVertexBrushStrengthEdit)
	ON_BN_CLICKED(IDC_TOOL_VERTEX_HEIGHTMAP_EXPORT, OnToolVertexHeightmapExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolVertEditPage message handlers

BOOL CToolVertEditPage::Create( CWnd *pParent )
{
	if( !CPageCtrl::Create( CToolVertEditPage::IDD, pParent ) )
		return FALSE;

	return TRUE;
}


BOOL CToolVertEditPage::OnInitDialog() 
{
	CPageCtrl::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bmTile.LoadBitmap( IDB_TOOL_TILE );
	m_bmTile_Check.LoadBitmap( IDB_TOOL_TILE_ON );
	m_bmVertex.LoadBitmap( IDB_TOOL_VERTEX );
	m_bmVertex_Check.LoadBitmap( IDB_TOOL_VERTEX_ON );
	m_bmRaise.LoadBitmap( IDB_TOOL_RAISE );
	m_bmRaise_Check.LoadBitmap( IDB_TOOL_RAISE_ON );
	m_bmLower.LoadBitmap( IDB_TOOL_LOWER );
	m_bmLower_Check.LoadBitmap( IDB_TOOL_LOWER_ON );
	m_bmFlatten.LoadBitmap( IDB_TOOL_FLATTEN );
	m_bmFlatten_Check.LoadBitmap( IDB_TOOL_FLATTEN_ON );
	m_bmSmooth.LoadBitmap( IDB_TOOL_SMOOTH );
	m_bmSmooth_Check.LoadBitmap( IDB_TOOL_SMOOTH_ON );
	m_bmRaiseAll.LoadBitmap( IDB_TOOL_RAISEALL );
	m_bmLowerAll.LoadBitmap( IDB_TOOL_LOWERALL );

	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_CONTROL_TILE ))->SetBitmap( (HBITMAP)m_bmTile_Check );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_CONTROL_VERTEX ))->SetBitmap( (HBITMAP)m_bmVertex );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_RAISE ))->SetBitmap( (HBITMAP)m_bmRaise_Check );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_LOWER ))->SetBitmap( (HBITMAP)m_bmLower );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_FLATTEN ))->SetBitmap( (HBITMAP)m_bmFlatten );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_SMOOTH ))->SetBitmap( (HBITMAP)m_bmSmooth );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_RAISE_ENTIRE ))->SetBitmap( (HBITMAP)m_bmRaiseAll );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_LOWER_ENTIRE ))->SetBitmap( (HBITMAP)m_bmLowerAll );
	
	m_ctrlBrushSize.SetRange( 1, 8, TRUE );
	m_ctrlBrushSize.SetPos( 1 );
	m_ctrlBrushStrength.SetRange( 1, 100, TRUE );
	m_ctrlBrushStrength.SetPos( 100 );
	m_bPatchSizeCanInput = FALSE;
	m_bHeightScaleCanInput = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CToolVertEditPage::UpdateControls()
{	
	g_pMyNKApp->SetBrushSize( m_nBrushSize );
	g_pMyNKApp->SetBrushStrength( m_nBrushStrength );
	g_pMyNKApp->GetTerrain()->SetViewLayers( 7 );
	if( m_nControlUnit == 0 )
		g_pMyNKApp->SetCircleBrush(	FALSE );
	else
		g_pMyNKApp->SetCircleBrush(	TRUE );
}


void CToolVertEditPage::UpdateTerrainInfo()
{
	m_strName = g_pMyNKApp->GetTerrain()->GetTerrainName();
	m_nSegment = g_pMyNKApp->GetTerrain()->GetHMWidth() - 1;
	m_nPatchSize = g_pMyNKApp->GetTerrain()->GetPatchSize();
	m_nHeightScale = g_pMyNKApp->GetTerrain()->GetHeightScale();
	m_strMinMaxHeight.Format( _T("%d / %d"), g_pMyNKApp->GetTerrain()->GetMinHeight(),
												g_pMyNKApp->GetTerrain()->GetMaxHeight() );
	UpdateData( FALSE );
}


void CToolVertEditPage::OnToolVertexPatchsizeModify() 
{
	// TODO: Add your control notification handler code here
	if( m_bPatchSizeCanInput )
	{
		int	nOldPatchSize = m_nPatchSize;

		if( UpdateData( TRUE ) == FALSE )
		{
			m_nPatchSize = nOldPatchSize;
			UpdateData( FALSE );
		}
		else if( m_nPatchSize < 10 || m_nPatchSize > 100 )
		{
			MessageBox( _T("Patch Size는 10에서 100 사이의 정수이어야 합니다."), _T("NKMapTool"),
						MB_ICONWARNING | MB_OK );
			m_nPatchSize = nOldPatchSize;
			UpdateData( FALSE );
		}
		else if( m_nPatchSize % 2 )
		{
			MessageBox( _T("Patch Size는 2의 배수이어야 합니다."), _T("NKMapTool"),
						MB_ICONWARNING | MB_OK );
			m_nPatchSize = nOldPatchSize;
			UpdateData( FALSE );
		}
		else
		{
			if( m_nPatchSize != nOldPatchSize )
			{
				g_pMyNKApp->GetTerrain()->ModifyPatchSize( m_nPatchSize );
				g_pMyNKApp->SetModify( 0, TRUE );
				g_pMyNKApp->SetModify( 2, TRUE );
				UpdateTerrainInfo();
				g_pMyNKApp->ResetCameraYPosition();
			}
		}

		m_ctrlPatchSize.SetReadOnly( TRUE );
		m_bPatchSizeCanInput = FALSE;
	}
	else
	{
		m_ctrlPatchSize.SetReadOnly( FALSE );
		m_bPatchSizeCanInput = TRUE;
	}
}


void CToolVertEditPage::OnToolVertexHeightscaleModify() 
{
	// TODO: Add your control notification handler code here
	if( m_bHeightScaleCanInput )
	{
		int	nOldHeightScale = m_nHeightScale;

		if( UpdateData( TRUE ) == FALSE )
		{
			m_nHeightScale = nOldHeightScale;
			UpdateData( FALSE );
		}
		else if( m_nHeightScale < 1 || m_nHeightScale > 10 )
		{
			MessageBox( _T("Height Scale은 1에서 10 사이의 정수이어야 합니다."), _T("NKMapTool"),
						MB_ICONWARNING | MB_OK );
			m_nHeightScale = nOldHeightScale;
			UpdateData( FALSE );
		}
		else
		{
			if( m_nHeightScale != nOldHeightScale )
			{
				g_pMyNKApp->GetTerrain()->ModifyHeightScale( m_nHeightScale );
				g_pMyNKApp->SetModify( 0, TRUE );
				g_pMyNKApp->SetModify( 2, TRUE );
				g_pMyNKApp->ResetCameraYPosition();
				g_pMyNKApp->GetTerrain()->ResetMinMaxHeights();
				UpdateTerrainInfo();
			}
		}

		m_ctrlHeightScale.SetReadOnly( TRUE );
		m_bHeightScaleCanInput = FALSE;
	}
	else
	{
		m_ctrlHeightScale.SetReadOnly( FALSE );
		m_bHeightScaleCanInput = TRUE;
	}
}


void CToolVertEditPage::OnToolVertexControlTile()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetControlUnit( m_nControlUnit );	
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_CONTROL_TILE ))->SetBitmap( (HBITMAP)m_bmTile_Check );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_CONTROL_VERTEX ))->SetBitmap( (HBITMAP)m_bmVertex );
}


void CToolVertEditPage::OnToolVertexControlVertex()
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetControlUnit( m_nControlUnit );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_CONTROL_TILE ))->SetBitmap( (HBITMAP)m_bmTile );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_CONTROL_VERTEX ))->SetBitmap( (HBITMAP)m_bmVertex_Check );
}


void CToolVertEditPage::OnToolVertexRaise() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetVertEditType( m_nEditType );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_RAISE ))->SetBitmap( (HBITMAP)m_bmRaise_Check );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_LOWER ))->SetBitmap( (HBITMAP)m_bmLower );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_FLATTEN ))->SetBitmap( (HBITMAP)m_bmFlatten );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_SMOOTH ))->SetBitmap( (HBITMAP)m_bmSmooth );
}


void CToolVertEditPage::OnToolVertexLower() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetVertEditType( m_nEditType );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_RAISE ))->SetBitmap( (HBITMAP)m_bmRaise );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_LOWER ))->SetBitmap( (HBITMAP)m_bmLower_Check );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_FLATTEN ))->SetBitmap( (HBITMAP)m_bmFlatten );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_SMOOTH ))->SetBitmap( (HBITMAP)m_bmSmooth );
}


void CToolVertEditPage::OnToolVertexFlatten() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetVertEditType( m_nEditType );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_RAISE ))->SetBitmap( (HBITMAP)m_bmRaise );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_LOWER ))->SetBitmap( (HBITMAP)m_bmLower );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_FLATTEN ))->SetBitmap( (HBITMAP)m_bmFlatten_Check );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_SMOOTH ))->SetBitmap( (HBITMAP)m_bmSmooth );
}


void CToolVertEditPage::OnToolVertexSmooth() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetVertEditType( m_nEditType );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_RAISE ))->SetBitmap( (HBITMAP)m_bmRaise );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_LOWER ))->SetBitmap( (HBITMAP)m_bmLower );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_FLATTEN ))->SetBitmap( (HBITMAP)m_bmFlatten );
	((CButton*)GetDlgItem( IDC_TOOL_VERTEX_SMOOTH ))->SetBitmap( (HBITMAP)m_bmSmooth_Check );
}


void CToolVertEditPage::OnChangeToolVertexBrushSizeEdit() 
{
	// TODO: Add your control notification handler code here
	int	nOldBrushSize = m_nBrushSize;
	UpdateData( TRUE );
	if( m_nBrushSize < 1 || m_nBrushSize > 8 )
		m_nBrushSize = nOldBrushSize;
	else
	{
		g_pMyNKApp->SetBrushSize( m_nBrushSize );
		m_ctrlBrushSize.SetPos( m_nBrushSize );
	}
	UpdateData( FALSE );
}


void CToolVertEditPage::OnChangeToolVertexBrushStrengthEdit() 
{
	// TODO: Add your control notification handler code here
	int	nOldBrushStrength = m_nBrushStrength;
	UpdateData( TRUE );
	if( m_nBrushStrength < 1 || m_nBrushStrength > 100 )
		m_nBrushStrength = nOldBrushStrength;
	else
	{
		g_pMyNKApp->SetBrushStrength( m_nBrushStrength );
		m_ctrlBrushStrength.SetPos( m_nBrushStrength );
	}
	UpdateData( FALSE );
}


void CToolVertEditPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl*	pSlider = (CSliderCtrl*)pScrollBar;

	switch( pSlider->GetDlgCtrlID() )
	{
	case IDC_TOOL_VERTEX_BRUSH_SIZE_SLIDER:
		m_nBrushSize = m_ctrlBrushSize.GetPos();
		g_pMyNKApp->SetBrushSize( m_nBrushSize );	
		UpdateData( FALSE );
		GetDlgItem( IDC_TOOL_VERTEX_BRUSH_SIZE_EDIT )->SetFocus();
		((CEdit*)GetDlgItem( IDC_TOOL_VERTEX_BRUSH_SIZE_EDIT ))->SetSel( 0, -1 );
		break;

	case IDC_TOOL_VERTEX_BRUSH_STRENGTH_SLIDER:
		m_nBrushStrength = m_ctrlBrushStrength.GetPos();
		g_pMyNKApp->SetBrushStrength( m_nBrushStrength );
		UpdateData( FALSE );
		GetDlgItem( IDC_TOOL_VERTEX_BRUSH_STRENGTH_EDIT )->SetFocus();
		((CEdit*)GetDlgItem( IDC_TOOL_VERTEX_BRUSH_STRENGTH_EDIT ))->SetSel( 0, -1 );
		break;
	}

	CPageCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CToolVertEditPage::OnToolVertexHeightmapReapply() 
{
	// TODO: Add your control notification handler code here
	
	// Get a name of height map
	TCHAR szFilter[] = _T("Raw Files (*.raw)|*.raw||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(), m_strName );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
		g_pMyNKApp->GetTerrain()->ReapplyHeightMap( FileDlg.GetFileName() );
		g_pMyNKApp->SetModify( 0, TRUE );
		g_pMyNKApp->ResetCameraYPosition();
		g_pMyNKApp->GetTerrain()->ResetMinMaxHeights();
		UpdateTerrainInfo();
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolVertEditPage::OnToolVertexHeightmapExport() 
{
	// TODO: Add your control notification handler code here

	// Get a name of height map
	TCHAR	szFilter[] = _T("Raw Files (*.raw)|*.raw||");
	CString	strDefault;
	strDefault.Format( _T("%s.raw"), g_pMyNKApp->GetWorkingFilename() );
	CMyFileDialog	FileDlg( FALSE, _T("raw"), strDefault, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(), m_strName );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		if( !g_pMyNKApp->GetTerrain()->ExportHeightMap( FileDlg.GetPathName() ) )
			MessageBox( _T("Height Map 저장을 실패했습니다."), _T("NKMapTool"), MB_OK );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolVertEditPage::OnToolVertexRaiseEntire() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_nOffset == 0 )
		return ;

	CString	strTemp;
	int	nHeightScale = g_pMyNKApp->GetTerrain()->GetHeightScale();
	if( m_nOffset % nHeightScale )
	{
		strTemp.Format( _T("Offset은 Height Scale( %d )의 배수이어야 합니다."), nHeightScale );
		MessageBox( strTemp, _T("NKMapTool"), MB_OK );
		return;
	}

	int	nMaxHeight = g_pMyNKApp->GetTerrain()->GetMaxHeight();
	if( nMaxHeight > 254 * nHeightScale )
	{
		MessageBox( _T("더 이상 지형을 올릴 수 없습니다."), _T("NKMapTool"), MB_OK );
		return ;
	}

	if( nMaxHeight + m_nOffset > 255 * nHeightScale )
	{
		strTemp.Format( _T("적용할 수 있는 최대 Offset은 %d입니다."), 255 * nHeightScale - nMaxHeight );
		MessageBox( strTemp, _T("NKMapTool"), MB_OK );
		return ;
	}

	int	nRealOffset = m_nOffset / nHeightScale;
	g_pMyNKApp->GetTerrain()->ModifyAllHeights( nRealOffset );
	CNKObjQuadNode::OffsetAllObjPosition( m_nOffset );
	g_pMyNKApp->SetModify( 0, TRUE );
	g_pMyNKApp->SetModify( 2, TRUE );
	g_pMyNKApp->ResetCameraYPosition();
	g_pMyNKApp->GetTerrain()->ResetMinMaxHeights();
	UpdateTerrainInfo();
}


void CToolVertEditPage::OnToolVertexLowerEntire() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_nOffset == 0 )
		return ;

	CString	strTemp;
	int	nHeightScale = g_pMyNKApp->GetTerrain()->GetHeightScale();
	if( m_nOffset % nHeightScale )
	{
		strTemp.Format( _T("Offset은 Height Scale( %d )의 배수이어야 합니다."), nHeightScale );
		MessageBox( strTemp, _T("NKMapTool"), MB_OK );
		return;
	}

	int	nMinHeight = g_pMyNKApp->GetTerrain()->GetMinHeight();
	if( nMinHeight < nHeightScale )
	{
		MessageBox( _T("더 이상 지형을 내릴 수 없습니다."), _T("NKMapTool"), MB_OK );
		return ;
	}

	if( nMinHeight - m_nOffset < 0 )
	{
		strTemp.Format( _T("적용할 수 있는 최대 Offset은 %d입니다."), nMinHeight );
		MessageBox( strTemp, _T("NKMapTool"), MB_OK );
		return ;
	}

	int	nRealOffset = -( m_nOffset / nHeightScale );
	g_pMyNKApp->GetTerrain()->ModifyAllHeights( nRealOffset );
	CNKObjQuadNode::OffsetAllObjPosition( -m_nOffset );
	g_pMyNKApp->SetModify( 0, TRUE );
	g_pMyNKApp->SetModify( 2, TRUE );
	g_pMyNKApp->ResetCameraYPosition();
	g_pMyNKApp->GetTerrain()->ResetMinMaxHeights();
	UpdateTerrainInfo();
}
