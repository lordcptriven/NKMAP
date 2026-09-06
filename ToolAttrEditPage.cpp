// ToolAttrEditPage.cpp : implementation file
//

#include "stdafx.h"
#include "NKMapTool.h"
#include "ToolAttrEditPage.h"
#include "MyNKApp.h"
#include "MyFileDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global variables
extern CMyNKApp*	g_pMyNKApp;


/////////////////////////////////////////////////////////////////////////////
// CToolAttrEditPage dialog


CToolAttrEditPage::CToolAttrEditPage(CWnd* pParent /*=NULL*/)
	: CPageCtrl(CToolAttrEditPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolAttrEditPage)
	m_nVisibleRange = 9;
	m_nCurMapAttribute = 0;
	m_nBrushSize = 0;
	//}}AFX_DATA_INIT
}


void CToolAttrEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPageCtrl::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolAttrEditPage)
	DDX_Control(pDX, IDC_TOOL_ATTMAP_BRUSH_SIZE_SLIDER, m_ctrlBrushSize);
	DDX_Control(pDX, IDC_TOOL_ATTMAP_RANGE_SPIN, m_ctrlVisibleRange);
	DDX_Text(pDX, IDC_TOOL_ATTMAP_RANGE, m_nVisibleRange);
	DDX_Radio(pDX, IDC_TOOL_ATTMAP_ABLETOWALK, m_nCurMapAttribute);
	DDX_Text(pDX, IDC_TOOL_ATTMAP_BRUSH_SIZE_EDIT, m_nBrushSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolAttrEditPage, CPageCtrl)
	//{{AFX_MSG_MAP(CToolAttrEditPage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_UNABLETOWALK_COLOR, OnToolAttmapUnabletowalkColor)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_WATER_COLOR, OnToolAttmapWaterColor)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_INDOOR_COLOR, OnToolAttmapIndoorColor)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_INDOORBOX_COLOR, OnToolAttmapIndoorboxColor)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_OUTDOORBOX_COLOR, OnToolAttmapOutdoorboxColor)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_GENERATE, OnToolAttmapGenerate)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_IMPORT, OnToolAttmapImport)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_EXPORT, OnToolAttmapExport)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_ABLETOWALK, OnToolAttmapAbletowalk)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_UNABLETOWALK, OnToolAttmapUnabletowalk)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_WATER, OnToolAttmapWater)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_INDOOR, OnToolAttmapIndoor)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_INDOORBOX, OnToolAttmapIndoorbox)
	ON_BN_CLICKED(IDC_TOOL_ATTMAP_OUTDOORBOX, OnToolAttmapOutdoorbox)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TOOL_ATTMAP_RANGE_SPIN, OnDeltaposToolAttmapRangeSpin)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolAttrEditPage message handlers

BOOL CToolAttrEditPage::Create( CWnd *pParent )
{
	if( !CPageCtrl::Create( CToolAttrEditPage::IDD, pParent ) )
		return FALSE;

	return TRUE;
}


void CToolAttrEditPage::UpdateControls()
{
	g_pMyNKApp->GetTerrain()->SetMapAttrColor( 0, m_crAbleToWalk );
	g_pMyNKApp->GetTerrain()->SetMapAttrColor( 1, m_crUnableToWalk );
	g_pMyNKApp->GetTerrain()->SetMapAttrColor( 2, m_crWater );
	g_pMyNKApp->GetTerrain()->SetMapAttrColor( 3, m_crIndoor );
	g_pMyNKApp->GetTerrain()->SetMapAttrColor( 4, m_crIndoorBox );
	g_pMyNKApp->GetTerrain()->SetMapAttrColor( 5, m_crOutdoorBox );

	g_pMyNKApp->SetCircleBrush( FALSE );
	g_pMyNKApp->GetTerrain()->SetViewLayers( 7 );
}


BOOL CToolAttrEditPage::OnInitDialog()
{
	CPageCtrl::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_crAbleToWalk = 0x00bbbbbb;
	m_crUnableToWalk = 0x00ff00ff;
	m_crWater = 0x00ffff00;
	m_crIndoor = 0x0055ff55;
	m_crIndoorBox = 0x005588cc;
	m_crOutdoorBox = 0x0055ffdd;
	
	m_ctrlBrushSize.SetRange( 1, 20, TRUE );
	m_ctrlBrushSize.SetPos( 1 );
	m_nBrushSize = 1;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CToolAttrEditPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect		rc;
	CBrush		black( RGB( 0, 0, 0 ) );

	// Able to walk
	CPaintDC	dcAbleToWalk( GetDlgItem( IDC_TOOL_ATTMAP_ABLETOWALK_COLOR ) );
	GetDlgItem( IDC_TOOL_ATTMAP_ABLETOWALK_COLOR )->GetClientRect( rc );
	dcAbleToWalk.FrameRect( &rc, &black );

	// Unable to walk
	COLORREF	crUnableToWalk = m_crUnableToWalk;
	CBrush		brUnableToWalk( crUnableToWalk );
	CPaintDC	dcUnableToWalk( GetDlgItem( IDC_TOOL_ATTMAP_UNABLETOWALK_COLOR ) );
	GetDlgItem( IDC_TOOL_ATTMAP_UNABLETOWALK_COLOR )->GetClientRect( rc );
	dcUnableToWalk.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcUnableToWalk.FillRect( &rc, &brUnableToWalk );

	// Water
	COLORREF	crWater = m_crWater;
	CBrush		brWater( crWater );
	CPaintDC	dcWater( GetDlgItem( IDC_TOOL_ATTMAP_WATER_COLOR ) );
	GetDlgItem( IDC_TOOL_ATTMAP_WATER_COLOR )->GetClientRect( rc );
	dcWater.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcWater.FillRect( &rc, &brWater );

	// Indoor
	COLORREF	crIndoor = m_crIndoor;
	CBrush		brIndoor( crIndoor );
	CPaintDC	dcIndoor( GetDlgItem( IDC_TOOL_ATTMAP_INDOOR_COLOR ) );
	GetDlgItem( IDC_TOOL_ATTMAP_INDOOR_COLOR )->GetClientRect( rc );
	dcIndoor.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcIndoor.FillRect( &rc, &brIndoor );

	// Indoor box
	COLORREF	crIndoorBox = m_crIndoorBox;
	CBrush		brIndoorBox( crIndoorBox );
	CPaintDC	dcIndoorBox( GetDlgItem( IDC_TOOL_ATTMAP_INDOORBOX_COLOR ) );
	GetDlgItem( IDC_TOOL_ATTMAP_INDOORBOX_COLOR )->GetClientRect( rc );
	dcIndoorBox.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcIndoorBox.FillRect( &rc, &brIndoorBox );

	// Outdoor box
	COLORREF	crOutdoorBox = m_crOutdoorBox;
	CBrush		brOutdoorBox( crOutdoorBox );
	CPaintDC	dcOutdoorBox( GetDlgItem( IDC_TOOL_ATTMAP_OUTDOORBOX_COLOR ) );
	GetDlgItem( IDC_TOOL_ATTMAP_OUTDOORBOX_COLOR )->GetClientRect( rc );
	dcOutdoorBox.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcOutdoorBox.FillRect( &rc, &brOutdoorBox );
}


void CToolAttrEditPage::OnToolAttmapAbletowalk() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetCurMapAttribute( m_nCurMapAttribute );
}


void CToolAttrEditPage::OnToolAttmapUnabletowalk() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetCurMapAttribute( m_nCurMapAttribute );
}


void CToolAttrEditPage::OnToolAttmapWater() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetCurMapAttribute( m_nCurMapAttribute );
}


void CToolAttrEditPage::OnToolAttmapIndoor() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetCurMapAttribute( m_nCurMapAttribute );
}


void CToolAttrEditPage::OnToolAttmapIndoorbox() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetCurMapAttribute( m_nCurMapAttribute );
}


void CToolAttrEditPage::OnToolAttmapOutdoorbox() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetCurMapAttribute( m_nCurMapAttribute );
}


void CToolAttrEditPage::OnToolAttmapUnabletowalkColor() 
{
	// TODO: Add your control notification handler code here
	COLORREF		color = m_crUnableToWalk;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		m_crUnableToWalk = ColorDlg.GetColor();
		Invalidate( TRUE );
		g_pMyNKApp->GetTerrain()->SetMapAttrColor( 1, m_crUnableToWalk );
	}
}


void CToolAttrEditPage::OnToolAttmapWaterColor() 
{
	// TODO: Add your control notification handler code here
	COLORREF		color = m_crWater;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		m_crWater = ColorDlg.GetColor();
		Invalidate( TRUE );
		g_pMyNKApp->GetTerrain()->SetMapAttrColor( 2, m_crWater );
	}
}


void CToolAttrEditPage::OnToolAttmapIndoorColor() 
{
	// TODO: Add your control notification handler code here
	COLORREF		color = m_crIndoor;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		m_crIndoor = ColorDlg.GetColor();
		Invalidate( TRUE );
		g_pMyNKApp->GetTerrain()->SetMapAttrColor( 3, m_crIndoor );
	}
}


void CToolAttrEditPage::OnToolAttmapIndoorboxColor() 
{
	// TODO: Add your control notification handler code here
	COLORREF		color = m_crIndoorBox;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		m_crIndoorBox = ColorDlg.GetColor();
		Invalidate( TRUE );
		g_pMyNKApp->GetTerrain()->SetMapAttrColor( 4, m_crIndoorBox );
	}
}


void CToolAttrEditPage::OnToolAttmapOutdoorboxColor() 
{
	// TODO: Add your control notification handler code here
	COLORREF		color = m_crOutdoorBox;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		m_crOutdoorBox = ColorDlg.GetColor();
		Invalidate( TRUE );
		g_pMyNKApp->GetTerrain()->SetMapAttrColor( 5, m_crOutdoorBox );
	}
}


void CToolAttrEditPage::OnToolAttmapGenerate() 
{
	// TODO: Add your control notification handler code here
	if( g_pMyNKApp->IsLoadedGameMap() )
		return ;

	if( g_pMyNKApp->GetTerrain()->GenerateAttributeMap() )
		g_pMyNKApp->SetModify( 0, TRUE );
}


void CToolAttrEditPage::OnToolAttmapImport() 
{
	// TODO: Add your control notification handler code here

	// Get a name of server attribute map
	TCHAR szFilter[] = _T("Attribute Files (*.att, *.raw)|*.att;*.raw||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(),
											g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		if( g_pMyNKApp->GetTerrain()->ImportServerAttributeMap( FileDlg.GetPathName() ) )
			g_pMyNKApp->SetModify( 0, TRUE );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolAttrEditPage::OnToolAttmapExport() 
{
	// TODO: Add your control notification handler code here

	// Get a name of server attribute map
	TCHAR szFilter[] = _T("Server Attribute Files (*.att)|*.att||");
	CString	strDefault;
	strDefault.Format( _T("%s.att"), g_pMyNKApp->GetWorkingFilename() );
	CMyFileDialog	FileDlg( FALSE, _T("att"), strDefault, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(),
											g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
		g_pMyNKApp->GetTerrain()->ExportServerAttributeMap( FileDlg.GetPathName() );

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolAttrEditPage::OnDeltaposToolAttmapRangeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		m_nVisibleRange -= 2;
	else if( pNMUpDown->iDelta < 0 )
		m_nVisibleRange += 2;

	if( m_nVisibleRange < 1 )
		m_nVisibleRange = 1;
	else if( m_nVisibleRange > 49 )
		m_nVisibleRange = 49;

	g_pMyNKApp->SetAttmapVisibleRange( m_nVisibleRange );
	UpdateData( FALSE );
	
	*pResult = 1;
}

// 브러쉬 크기 조절 기능 추가(by 원석)
void CToolAttrEditPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl*	pSlider = (CSliderCtrl*)pScrollBar;

	switch( pSlider->GetDlgCtrlID() )
	{
	case IDC_TOOL_ATTMAP_BRUSH_SIZE_SLIDER:
		
		m_nBrushSize = m_ctrlBrushSize.GetPos();

		m_nBrushSize += (m_nBrushSize-1); // 홀수 단위 1,3,5,7,9로 변환.

		g_pMyNKApp->SetBrushSize( m_nBrushSize );	
		UpdateData( FALSE );

		break;
	}
	
	CPageCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CToolAttrEditPage::UpdateBrushSizeSlider( int AddValue ) // 증감값이 들어온다.
{
	m_nBrushSize = m_ctrlBrushSize.GetPos() + AddValue; // 여기의 m_nBrushSize는 에디트 박스에 연결된 값이고 실제 브러쉬의 사이즈가 아님에 유의.

	if( m_nBrushSize < m_ctrlBrushSize.GetRangeMin() )
	{
		m_nBrushSize = m_ctrlBrushSize.GetRangeMin();
	}
	else if( m_nBrushSize > m_ctrlBrushSize.GetRangeMax() )
	{
		m_nBrushSize = m_ctrlBrushSize.GetRangeMax();
	}

	m_ctrlBrushSize.SetPos( m_nBrushSize );

	m_nBrushSize += (m_nBrushSize-1); // 홀수 단위 1,3,5,7,9로 변환.
	g_pMyNKApp->SetBrushSize( m_nBrushSize ); // 이녀석이 실제 브러쉬 사이즈 적용.

	UpdateData( FALSE );
}
