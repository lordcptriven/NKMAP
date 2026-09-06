// ToolLightEditPage.cpp : implementation file
//

#include "stdafx.h"
#include "NKMapTool.h"
#include "ToolLightEditPage.h"
#include "MyNKApp.h"
#include "MyFileDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global variables
extern CMyNKApp*		g_pMyNKApp;


/////////////////////////////////////////////////////////////////////////////
// CToolLightEditPage dialog


CToolLightEditPage::CToolLightEditPage(CWnd* pParent /*=NULL*/)
	: CPageCtrl(CToolLightEditPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolLightEditPage)
	m_fDirectionX = 0.0f;
	m_fDirectionY = 0.0f;
	m_fDirectionZ = 0.0f;
	m_nLightColorR = 0;
	m_nLightColorG = 0;
	m_nLightColorB = 0;
	m_nAmbientColorR = 0;
	m_nAmbientColorG = 0;
	m_nAmbientColorB = 0;
	m_bDirControl = FALSE;
	m_nEditType = 0;
	m_nShadowR = 150;
	m_nShadowG = 150;
	m_nShadowB = 150;
	m_nBrushSize = 2;
	m_nBrushStrength = 100;
	m_bDrawCurrentMap = FALSE;
	//}}AFX_DATA_INIT
}


void CToolLightEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPageCtrl::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolLightEditPage)
	DDX_Control(pDX, IDC_TOOL_LIGHT_BRUSH_STRENGTH_SLIDER, m_ctrlBrushStrength);
	DDX_Control(pDX, IDC_TOOL_LIGHT_BRUSH_SIZE_SLIDER, m_ctrlBrushSize);
	DDX_Control(pDX, IDC_TOOL_LIGHT_TIME, m_ctrlWorldTime);
	DDX_Text(pDX, IDC_TOOL_LIGHT_DIRX, m_fDirectionX);
	DDX_Text(pDX, IDC_TOOL_LIGHT_DIRY, m_fDirectionY);
	DDX_Text(pDX, IDC_TOOL_LIGHT_DIRZ, m_fDirectionZ);
	DDX_Text(pDX, IDC_TOOL_LIGHT_LIGHTR_EDIT, m_nLightColorR);
	DDX_Text(pDX, IDC_TOOL_LIGHT_LIGHTG_EDIT, m_nLightColorG);
	DDX_Text(pDX, IDC_TOOL_LIGHT_LIGHTB_EDIT, m_nLightColorB);
	DDX_Text(pDX, IDC_TOOL_LIGHT_AMBIENTR_EDIT, m_nAmbientColorR);
	DDX_Text(pDX, IDC_TOOL_LIGHT_AMBIENTG_EDIT, m_nAmbientColorG);
	DDX_Text(pDX, IDC_TOOL_LIGHT_AMBIENTB_EDIT, m_nAmbientColorB);
	DDX_Check(pDX, IDC_TOOL_LIGHT_DIRCONTROL, m_bDirControl);
	DDX_Radio(pDX, IDC_TOOL_LIGHT_PAINT, m_nEditType);
	DDX_Text(pDX, IDC_TOOL_LIGHT_SHADOWR_EDIT, m_nShadowR);
	DDX_Text(pDX, IDC_TOOL_LIGHT_SHADOWG_EDIT, m_nShadowG);
	DDX_Text(pDX, IDC_TOOL_LIGHT_SHADOWB_EDIT, m_nShadowB);
	DDX_Text(pDX, IDC_TOOL_LIGHT_BRUSH_SIZE_EDIT, m_nBrushSize);
	DDX_Text(pDX, IDC_TOOL_LIGHT_BRUSH_STRENGTH_EDIT, m_nBrushStrength);
	DDX_Check(pDX, IDC_TOOL_LIGHT_SHADOW_CURMAP, m_bDrawCurrentMap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolLightEditPage, CPageCtrl)
	//{{AFX_MSG_MAP(CToolLightEditPage)
	ON_CBN_SELCHANGE(IDC_TOOL_LIGHT_TIME, OnSelchangeToolLightTime)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TOOL_LIGHT_LIGHTCOLOR, OnToolLightLightcolor)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_AMBIENTCOLOR, OnToolLightAmbientcolor)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_DIRCONTROL, OnToolLightDircontrol)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_SHADOWCOLOR, OnToolLightShadowcolor)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_TOOL_LIGHT_BRUSH_SIZE_EDIT, OnChangeToolLightBrushSizeEdit)
	ON_EN_CHANGE(IDC_TOOL_LIGHT_BRUSH_STRENGTH_EDIT, OnChangeToolLightBrushStrengthEdit)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_PAINT, OnToolLightPaint)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_ERASE, OnToolLightErase)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_SHADOW_CURMAP, OnToolLightShadowCurmap)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_SHADOW_IMPORT, OnToolLightShadowImport)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_SHADOW_EXPORT, OnToolLightShadowExport)
	ON_BN_CLICKED(IDC_TOOL_LIGHT_DEFAULT, OnToolLightDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolLightEditPage message handlers

BOOL CToolLightEditPage::Create( CWnd *pParent )
{
	if( !CPageCtrl::Create( CToolLightEditPage::IDD, pParent ) )
		return FALSE;

	return TRUE;
}


BOOL CToolLightEditPage::OnInitDialog() 
{
	CPageCtrl::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlWorldTime.AddString( _T("12 ~ 16 : 정오") );
	m_ctrlWorldTime.AddString( _T("17 ~ 21 : 오후") );
	m_ctrlWorldTime.AddString( _T("22 ~ 27 : 저녁") );
	m_ctrlWorldTime.AddString( _T("28 ~ 01 : 밤") );
	m_ctrlWorldTime.AddString( _T("02 ~ 06 : 아침") );
	m_ctrlWorldTime.AddString( _T("07 ~ 11 : 오전") );
	m_ctrlWorldTime.SetCurSel( 0 );

	m_bmDirControl.LoadBitmap( IDB_TOOL_LIGHT_CONTROL );
	m_bmDirControl_Check.LoadBitmap( IDB_TOOL_LIGHT_CONTROL_ON );
	m_bmPaint.LoadBitmap( IDB_TOOL_PAINT_S );
	m_bmErase.LoadBitmap( IDB_TOOL_ERASE_S );
	m_bmPaint_Check.LoadBitmap( IDB_TOOL_PAINT_ON_S );
	m_bmErase_Check.LoadBitmap( IDB_TOOL_ERASE_ON_S );

	((CButton*)GetDlgItem( IDC_TOOL_LIGHT_DIRCONTROL ))->SetBitmap( (HBITMAP)m_bmDirControl );
	((CButton*)GetDlgItem( IDC_TOOL_LIGHT_PAINT ))->SetBitmap( (HBITMAP)m_bmPaint_Check );
	((CButton*)GetDlgItem( IDC_TOOL_LIGHT_ERASE ))->SetBitmap( (HBITMAP)m_bmErase );

	m_ctrlBrushSize.SetRange( 1, 16, TRUE );
	m_ctrlBrushSize.SetPos( 2 );
	m_ctrlBrushStrength.SetRange( 1, 100, TRUE );
	m_ctrlBrushStrength.SetPos( 100 );

	m_nTimeID = 0;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CToolLightEditPage::UpdateControls()
{	
	m_nTimeID = g_pMyNKApp->GetTerrain()->GetTimeID();
	m_ctrlWorldTime.SetCurSel( m_nTimeID );
	UpdateDirectLightInfo();
	
	g_pMyNKApp->SetLightTimeID( m_nTimeID );
	float	fX, fY, fZ;
	for( int i = 0; i < 6; i++ )
	{
		g_pMyNKApp->GetTerrain()->GetLightDirection( i, fX, fY, fZ );
		g_pMyNKApp->SetLightDirection( i, fX, fY, fZ );
	}

	g_pMyNKApp->SetShadowColor( m_nShadowR / 255.0f, m_nShadowG / 255.0f, m_nShadowB / 255.0f );
	g_pMyNKApp->SetCircleBrush( TRUE );
	g_pMyNKApp->SetBrushSize( m_nBrushSize );
	g_pMyNKApp->SetBrushStrength( m_nBrushStrength );	

	g_pMyNKApp->GetTerrain()->SetViewLayers( 7 );
}


void CToolLightEditPage::OnSelchangeToolLightTime() 
{
	// TODO: Add your control notification handler code here
	m_nTimeID = m_ctrlWorldTime.GetCurSel();
	int	nWorldTime;
	
	switch( m_nTimeID )
	{
	case 0:
		nWorldTime = 14;	break;
	case 1:
		nWorldTime = 19;	break;
	case 2:
		nWorldTime = 24;	break;
	case 3:
		nWorldTime = 0;		break;
	case 4:
		nWorldTime = 4;		break;
	case 5:
		nWorldTime = 9;		break;
	}
	
	g_pMyNKApp->ChangeWorldTime( nWorldTime );
	g_pMyNKApp->SetLightTimeID( m_nTimeID );
	if( m_bDirControl )
		g_pMyNKApp->MakeLightControlMesh();

	UpdateDirectLightInfo();
}


void CToolLightEditPage::UpdateDirectLightInfo()
{
	g_pMyNKApp->GetTerrain()->GetLightDirection( m_nTimeID, m_fDirectionX, m_fDirectionY, m_fDirectionZ );
	g_pMyNKApp->GetTerrain()->GetDirectLightColor( m_nTimeID, m_nLightColorR, m_nLightColorG, m_nLightColorB );
	g_pMyNKApp->GetTerrain()->GetAmbientColor( m_nTimeID, m_nAmbientColorR, m_nAmbientColorG, m_nAmbientColorB );
	
	UpdateData( FALSE );
	Invalidate( TRUE );
}


void CToolLightEditPage::OnToolLightDircontrol() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	g_pMyNKApp->SetDirectLightControl( m_bDirControl );
	g_pMyNKApp->MakeLightControlMesh();

	if( m_bDirControl )
		((CButton*)GetDlgItem( IDC_TOOL_LIGHT_DIRCONTROL ))->SetBitmap( (HBITMAP)m_bmDirControl_Check );
	else
		((CButton*)GetDlgItem( IDC_TOOL_LIGHT_DIRCONTROL ))->SetBitmap( (HBITMAP)m_bmDirControl );
}


void CToolLightEditPage::OnToolLightLightcolor() 
{
	// TODO: Add your control notification handler code here
	COLORREF		color = ( m_nLightColorB << 16 ) | ( m_nLightColorG << 8 ) | m_nLightColorR;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		color = ColorDlg.GetColor();
		m_nLightColorR = color & 0x000000ff;
		m_nLightColorG = ( color >> 8 ) & 0x000000ff;
		m_nLightColorB = ( color >> 16 ) & 0x000000ff;

		UpdateData( FALSE );
		Invalidate( TRUE );

		// Set colors of directional lights
		g_pMyNKApp->GetTerrain()->SetDirectLightColor( m_nTimeID, m_nLightColorR / 255.0f,
														m_nLightColorG / 255.0f, m_nLightColorB / 255.0f );

		// renew light maps
		int	nWidth = g_pMyNKApp->GetTerrain()->GetHMWidth() - 1;
		g_pMyNKApp->GetTerrain()->RenewRGBMap( m_nTimeID, 0, 0, nWidth, nWidth );

		// Update color map textures
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 0 );
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 0 );
		if( nWidth == 256 )
		{
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 1 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 2 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 3 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 1 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 2 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 3 );
		}

		g_pMyNKApp->SetModify( 0, TRUE );
	}
}


void CToolLightEditPage::OnToolLightAmbientcolor() 
{
	// TODO: Add your control notification handler code here
	COLORREF		color = ( m_nAmbientColorB << 16 ) | ( m_nAmbientColorG << 8 ) | m_nAmbientColorR;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		color = ColorDlg.GetColor();
		m_nAmbientColorR = color & 0x000000ff;
		m_nAmbientColorG = ( color >> 8 ) & 0x000000ff;
		m_nAmbientColorB = ( color >> 16 ) & 0x000000ff;

		UpdateData( FALSE );
		Invalidate( TRUE );

		// Set colors of ambient colors
		g_pMyNKApp->GetTerrain()->SetAmbientColor( m_nTimeID, m_nAmbientColorR / 255.0f,
													m_nAmbientColorG / 255.0f, m_nAmbientColorB / 255.0f );

		// renew light maps
		int	nWidth = g_pMyNKApp->GetTerrain()->GetHMWidth() - 1;
		g_pMyNKApp->GetTerrain()->RenewRGBMap( m_nTimeID, 0, 0, nWidth, nWidth );

		// Update color map textures
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 0 );
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 0 );
		if( nWidth == 256 )
		{
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 1 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 2 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 3 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 1 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 2 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 3 );
		}

		g_pMyNKApp->SetModify( 0, TRUE );
	}
}


void CToolLightEditPage::OnToolLightPaint() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetShadowEditType( m_nEditType );

	((CButton*)GetDlgItem( IDC_TOOL_LIGHT_PAINT ))->SetBitmap( (HBITMAP)m_bmPaint_Check );
	((CButton*)GetDlgItem( IDC_TOOL_LIGHT_ERASE ))->SetBitmap( (HBITMAP)m_bmErase );
}


void CToolLightEditPage::OnToolLightErase() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetShadowEditType( m_nEditType );

	((CButton*)GetDlgItem( IDC_TOOL_LIGHT_PAINT ))->SetBitmap( (HBITMAP)m_bmPaint );
	((CButton*)GetDlgItem( IDC_TOOL_LIGHT_ERASE ))->SetBitmap( (HBITMAP)m_bmErase_Check );
}


void CToolLightEditPage::OnToolLightShadowCurmap() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetDrawShadowCurrentMap( m_bDrawCurrentMap );
}


void CToolLightEditPage::OnToolLightShadowcolor() 
{
	// TODO: Add your control notification handler code here
	COLORREF		color = ( m_nShadowB << 16 ) | ( m_nShadowG << 8 ) | m_nShadowR;
	CColorDialog	ColorDlg( color, CC_FULLOPEN );

	if( ColorDlg.DoModal() == IDOK )
	{
		color = ColorDlg.GetColor();
		m_nShadowR = color & 0x000000ff;
		m_nShadowG = ( color >> 8 ) & 0x000000ff;
		m_nShadowB = ( color >> 16 ) & 0x000000ff;

		UpdateData( FALSE );
		Invalidate( TRUE );

		// Set shadow color
		g_pMyNKApp->SetShadowColor( m_nShadowR / 255.0f, m_nShadowG / 255.0f, m_nShadowB / 255.0f );
	}
}


void CToolLightEditPage::OnChangeToolLightBrushSizeEdit() 
{
	// TODO: Add your control notification handler code here
	int	nOldSize = m_nBrushSize;
	UpdateData( TRUE );
	if( m_nBrushSize < 1 || m_nBrushSize > 16 )
		m_nBrushSize = nOldSize;
	else
	{
		g_pMyNKApp->SetBrushSize( m_nBrushSize );
		m_ctrlBrushSize.SetPos( m_nBrushSize );
	}
	UpdateData( FALSE );
}


void CToolLightEditPage::OnChangeToolLightBrushStrengthEdit() 
{
	// TODO: Add your control notification handler code here
	int	nOldStrength = m_nBrushStrength;
	UpdateData( TRUE );
	if( m_nBrushStrength < 1 || m_nBrushStrength > 100 )
		m_nBrushStrength = nOldStrength;
	else
	{
		g_pMyNKApp->SetBrushStrength( m_nBrushStrength );
		m_ctrlBrushStrength.SetPos( m_nBrushStrength );
	}
	UpdateData( FALSE );
}


void CToolLightEditPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl*	pSlider = (CSliderCtrl*)pScrollBar;

	switch( pSlider->GetDlgCtrlID() )
	{
	case IDC_TOOL_LIGHT_BRUSH_SIZE_SLIDER:
		m_nBrushSize = m_ctrlBrushSize.GetPos();
		g_pMyNKApp->SetBrushSize( m_nBrushSize );	
		UpdateData( FALSE );
		GetDlgItem( IDC_TOOL_LIGHT_BRUSH_SIZE_EDIT )->SetFocus();
		((CEdit*)GetDlgItem( IDC_TOOL_LIGHT_BRUSH_SIZE_EDIT ))->SetSel( 0, -1 );
		break;

	case IDC_TOOL_LIGHT_BRUSH_STRENGTH_SLIDER:
		m_nBrushStrength = m_ctrlBrushStrength.GetPos();
		g_pMyNKApp->SetBrushStrength( m_nBrushStrength );
		UpdateData( FALSE );
		GetDlgItem( IDC_TOOL_LIGHT_BRUSH_STRENGTH_EDIT )->SetFocus();
		((CEdit*)GetDlgItem( IDC_TOOL_LIGHT_BRUSH_STRENGTH_EDIT ))->SetSel( 0, -1 );
		break;
	}
	
	CPageCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CToolLightEditPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect		rc;
	CBrush		black( RGB( 0, 0, 0 ) );

	// Directional light color
	COLORREF	crDirectional = ( m_nLightColorB << 16 ) | ( m_nLightColorG << 8 ) | m_nLightColorR;
	CBrush		brDirctional( crDirectional );
	CPaintDC	dcDirectional( GetDlgItem( IDC_TOOL_LIGHT_LIGHTCOLOR ) );
	GetDlgItem( IDC_TOOL_LIGHT_LIGHTCOLOR )->GetClientRect( rc );
	dcDirectional.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcDirectional.FillRect( &rc, &brDirctional );

	// Ambient color
	COLORREF	crAmbient = ( m_nAmbientColorB << 16 ) | ( m_nAmbientColorG << 8 ) | m_nAmbientColorR;
	CBrush		brAmbient( crAmbient );
	CPaintDC	dcAmbient( GetDlgItem( IDC_TOOL_LIGHT_AMBIENTCOLOR ) );
	GetDlgItem( IDC_TOOL_LIGHT_AMBIENTCOLOR )->GetClientRect( rc );
	dcAmbient.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );	
	dcAmbient.FillRect( &rc, &brAmbient );

	// Shadow color
	COLORREF	crShadow = ( m_nShadowB << 16 ) | ( m_nShadowG << 8 ) | m_nShadowR;
	CBrush		brShadow( crShadow );
	CPaintDC	dcShadow( GetDlgItem( IDC_TOOL_LIGHT_SHADOWCOLOR ) );
	GetDlgItem( IDC_TOOL_LIGHT_SHADOWCOLOR )->GetClientRect( rc );
	dcShadow.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );	
	dcShadow.FillRect( &rc, &brShadow );
}


void CToolLightEditPage::SetDirection( float fX, float fY, float fZ )
{
	if( fX != m_fDirectionX && fY != m_fDirectionY && fZ != m_fDirectionZ )
	{
		m_fDirectionX = fX;
		m_fDirectionY = fY;
		m_fDirectionZ = fZ;
		UpdateData( FALSE );

		// Set directions of a directional light
		g_pMyNKApp->GetTerrain()->SetLightDirection( m_nTimeID, fX, fY, fZ );

		// renew light maps
		int	nWidth = g_pMyNKApp->GetTerrain()->GetHMWidth() - 1;
		g_pMyNKApp->GetTerrain()->RenewRGBMap( m_nTimeID, 0, 0, nWidth, nWidth );

		// Update color map textures
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 0 );
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 0 );
		if( nWidth == 256 )
		{
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 1 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 2 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 3 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 1 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 2 );
			g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 3 );
		}

		g_pMyNKApp->SetModify( 0, TRUE );
	}
}


void CToolLightEditPage::OnToolLightShadowImport() 
{
	// TODO: Add your control notification handler code here
	
	// Get a name of shadow map
	TCHAR szFilter[] = _T("Bitmap Files (*.bmp)|*.bmp||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(),
											g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		if( g_pMyNKApp->GetTerrain()->ImportShadowMap( m_nTimeID, FileDlg.GetPathName() ) )
			g_pMyNKApp->SetModify( 0, TRUE );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolLightEditPage::OnToolLightShadowExport() 
{
	// TODO: Add your control notification handler code here
	
	// Get a name of shadow map
	TCHAR	szFilter[] = _T("Bitmap Files (*.bmp)|*.bmp||");
	CString	strDefault, strTime;
	if( m_nTimeID == 0 )
		strTime = _T("12~16");
	else if( m_nTimeID == 1 )
		strTime = _T("17~21");
	else if( m_nTimeID == 2 )
		strTime = _T("22~27");
	else if( m_nTimeID == 3 )
		strTime = _T("28~1");
	else if( m_nTimeID == 4 )
		strTime = _T("2~6");
	else if( m_nTimeID == 5 )
		strTime = _T("7~11");
	strDefault.Format( _T("%s_shadow(%s).bmp"), g_pMyNKApp->GetWorkingFilename(), strTime );

	CMyFileDialog	FileDlg( FALSE, _T("bmp"), strDefault, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(),
											g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		if( !g_pMyNKApp->GetTerrain()->ExportShadowMap( m_nTimeID, FileDlg.GetPathName() ) )
			MessageBox( _T("Shadow Map 저장을 실패했습니다."), _T("NKMapTool"), MB_OK );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolLightEditPage::OnToolLightDefault() 
{
	// Set default light direction
	g_pMyNKApp->GetTerrain()->SetDefaultLightDirections();

	// Reset light direction information
	g_pMyNKApp->GetTerrain()->GetLightDirection( m_nTimeID, m_fDirectionX, m_fDirectionY, m_fDirectionZ );
	UpdateData( FALSE );
	float	fX, fY, fZ;
	int i = 0;
	for( i = 0; i < 6; i++ )
	{
		g_pMyNKApp->GetTerrain()->GetLightDirection( i, fX, fY, fZ );
		g_pMyNKApp->SetLightDirection( i, fX, fY, fZ );
	}

	// renew light maps
	int	nWidth = g_pMyNKApp->GetTerrain()->GetHMWidth() - 1;
	for( i = 0; i < 6; i++ )
		g_pMyNKApp->GetTerrain()->RenewRGBMap( i, 0, 0, nWidth, nWidth );

	// Update color map textures
	g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 0 );
	g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 0 );
	if( nWidth == 256 )
	{
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 1 );
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 2 );
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 2, 3 );
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 1 );
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 2 );
		g_pMyNKApp->GetTerrain()->UpdateClrMapTexture( 3, 3 );
	}

	g_pMyNKApp->SetModify( 0, TRUE );
}
