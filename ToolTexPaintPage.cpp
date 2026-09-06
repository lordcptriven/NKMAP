// ToolTexPaintPage.cpp : implementation file
//

#include "stdafx.h"
#include "NKMapTool.h"
#include "ToolTexPaintPage.h"
#include "MyNKApp.h"
#include "MyFileDialog.h"
#include "Config.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global variables
extern CMyNKApp*	g_pMyNKApp;


/////////////////////////////////////////////////////////////////////////////
// CToolTexPaintPage dialog


CToolTexPaintPage::CToolTexPaintPage(CWnd* pParent /*=NULL*/)
	: CPageCtrl(CToolTexPaintPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolTexPaintPage)
	m_nSize = 2;
	m_nStrength = 100;
	m_bFilter = TRUE;
	m_bShowLayer1 = TRUE;
	m_bShowLayer2 = TRUE;
	m_bShowLayer3 = TRUE;
	m_nEditLayer = 0;
	m_bBrushTool = FALSE;
	m_nEditType = 0;
	//}}AFX_DATA_INIT
}


void CToolTexPaintPage::DoDataExchange(CDataExchange* pDX)
{
	CPageCtrl::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolTexPaintPage)
	DDX_Control(pDX, IDC_TOOL_PAINT_TOOL_TYPE, m_ctrlToolType);
	DDX_Control(pDX, IDC_TOOL_PAINT_SHOW_LAYER3, m_ctrlShowLayer3);
	DDX_Control(pDX, IDC_TOOL_PAINT_SHOW_LAYER2, m_ctrlShowLayer2);
	DDX_Control(pDX, IDC_TOOL_PAINT_SHOW_LAYER1, m_ctrlShowLayer1);
	DDX_Control(pDX, IDC_TOOL_PAINT_STRENGTH_SLIDER, m_ctrlStrength);
	DDX_Control(pDX, IDC_TOOL_PAINT_SIZE_SLIDER, m_ctrlSize);
	DDX_Text(pDX, IDC_TOOL_PAINT_SIZE_EDIT, m_nSize);
	DDX_Text(pDX, IDC_TOOL_PAINT_STRENGTH_EDIT, m_nStrength);
	DDX_Check(pDX, IDC_TOOL_PAINT_FILTER, m_bFilter);
	DDX_Check(pDX, IDC_TOOL_PAINT_SHOW_LAYER1, m_bShowLayer1);
	DDX_Check(pDX, IDC_TOOL_PAINT_SHOW_LAYER2, m_bShowLayer2);
	DDX_Check(pDX, IDC_TOOL_PAINT_SHOW_LAYER3, m_bShowLayer3);
	DDX_Radio(pDX, IDC_TOOL_PAINT_LAYER1, m_nEditLayer);
	DDX_Check(pDX, IDC_TOOL_PAINT_TOOL_TYPE, m_bBrushTool);
	DDX_Radio(pDX, IDC_TOOL_PAINT_TYPE_P, m_nEditType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolTexPaintPage, CPageCtrl)
	//{{AFX_MSG_MAP(CToolTexPaintPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TOOL_PAINT_NEW, OnToolPaintNew)
	ON_BN_CLICKED(IDC_TOOL_PAINT_DELETE, OnToolPaintDelete)
	ON_BN_CLICKED(IDC_TOOL_PAINT_TYPE_P, OnToolPaintTypeP)
	ON_BN_CLICKED(IDC_TOOL_PAINT_TYPE_E, OnToolPaintTypeE)
	ON_EN_CHANGE(IDC_TOOL_PAINT_SIZE_EDIT, OnChangeToolPaintSizeEdit)
	ON_EN_CHANGE(IDC_TOOL_PAINT_STRENGTH_EDIT, OnChangeToolPaintStrengthEdit)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_TOOL_PAINT_FILTER, OnToolPaintFilter)
	ON_BN_CLICKED(IDC_TOOL_PAINT_LAYER1, OnToolPaintLayer1)
	ON_BN_CLICKED(IDC_TOOL_PAINT_LAYER2, OnToolPaintLayer2)
	ON_BN_CLICKED(IDC_TOOL_PAINT_LAYER3, OnToolPaintLayer3)
	ON_BN_CLICKED(IDC_TOOL_PAINT_SHOW_LAYER1, OnToolPaintShowLayer1)
	ON_BN_CLICKED(IDC_TOOL_PAINT_SHOW_LAYER2, OnToolPaintShowLayer2)
	ON_BN_CLICKED(IDC_TOOL_PAINT_SHOW_LAYER3, OnToolPaintShowLayer3)
	ON_BN_CLICKED(IDC_TOOL_PAINT_TOOL_TYPE, OnToolPaintToolType)
	ON_BN_CLICKED(IDC_TOOL_PAINT_REPLACE, OnToolPaintReplace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTexPaintPage message handlers

BOOL CToolTexPaintPage::Create( CWnd *pParent )
{
	if( !CPageCtrl::Create( CToolTexPaintPage::IDD, pParent ) )
		return FALSE;

	return TRUE;
}


BOOL CToolTexPaintPage::OnInitDialog() 
{
	CPageCtrl::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ListBox.SubclassDlgItem( IDC_TOOL_PAINT_LISTBOX, this );	

	m_bmHideIcon.LoadBitmap( IDB_TOOL_HIDE_ICON );
	m_bmShowIcon.LoadBitmap( IDB_TOOL_SHOW_ICON );
	m_bmLayer1.LoadBitmap( IDB_TOOL_LAYER1 );
	m_bmLayer2.LoadBitmap( IDB_TOOL_LAYER2 );
	m_bmLayer3.LoadBitmap( IDB_TOOL_LAYER3 );
	m_bmLayer1_Check.LoadBitmap( IDB_TOOL_LAYER1_ON );
	m_bmLayer2_Check.LoadBitmap( IDB_TOOL_LAYER2_ON );
	m_bmLayer3_Check.LoadBitmap( IDB_TOOL_LAYER3_ON );
	m_bmPaint.LoadBitmap( IDB_TOOL_PAINT );
	m_bmErase.LoadBitmap( IDB_TOOL_ERASE );
	m_bmPaint_Check.LoadBitmap( IDB_TOOL_PAINT_ON );
	m_bmErase_Check.LoadBitmap( IDB_TOOL_ERASE_ON );
	m_bmPencil.LoadBitmap( IDB_TOOL_PENCIL );
	m_bmBrush.LoadBitmap( IDB_TOOL_BRUSH );	

	m_ctrlShowLayer1.SetBitmap( (HBITMAP)m_bmShowIcon );
	m_ctrlShowLayer2.SetBitmap( (HBITMAP)m_bmShowIcon );
	m_ctrlShowLayer3.SetBitmap( (HBITMAP)m_bmShowIcon );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER1 ))->SetBitmap( (HBITMAP)m_bmLayer1_Check );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER2 ))->SetBitmap( (HBITMAP)m_bmLayer2 );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER3 ))->SetBitmap( (HBITMAP)m_bmLayer3 );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_TYPE_P ))->SetBitmap( (HBITMAP)m_bmPaint_Check );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_TYPE_E ))->SetBitmap( (HBITMAP)m_bmErase );
	m_ctrlToolType.SetBitmap( (HBITMAP)m_bmPencil );
	m_bLayer23Brush = TRUE;

	m_ctrlSize.SetRange( 1, 16, TRUE );
	m_ctrlSize.SetPos( 2 );
	m_ctrlStrength.SetRange( 1, 100, TRUE );
	m_ctrlStrength.SetPos( 100 );
	m_ctrlStrength.EnableWindow( FALSE );
	GetDlgItem( IDC_TOOL_PAINT_STRENGTH_EDIT )->EnableWindow( FALSE );	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CToolTexPaintPage::OnDestroy() 
{
	CPageCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	m_ListBox.RemoveAllListItems();
}


void CToolTexPaintPage::UpdateControls()
{
	m_ListBox.SetFocus();
	ShowLayers();
	g_pMyNKApp->SetCircleBrush( m_bBrushTool );	
	g_pMyNKApp->SetBrushSize( m_nSize );
	g_pMyNKApp->SetBrushStrength( m_nStrength );
}


void CToolTexPaintPage::OnToolPaintNew() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilter[] = _T("Image Files (*.ntx, *.bmp)|*.ntx;*.bmp|Nako Texture Files (*.ntx)|*.ntx|")
						_T("Bitmap Files (*.bmp)|*.bmp||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								OFN_ALLOWMULTISELECT, szFilter, NULL );

	TCHAR	szBuffer[MAX_PATH * 20];
	memset( szBuffer, 0, sizeof(szBuffer) );
	FileDlg.m_ofn.lpstrFile = szBuffer;
	FileDlg.m_ofn.nMaxFile = MAX_PATH * 20;

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s"), g_pMyNKApp->GetWorkingPath(),
							g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );

		CString		strTemp, strTemp2;
		POSITION	pos = FileDlg.GetStartPosition();
		while( pos )
		{
			CString	strTemp = FileDlg.GetNextPathName( pos );
			int	nFind = strTemp.ReverseFind( '\\' );
			strTemp2 = strTemp.Right( strTemp.GetLength() - nFind - 1 );
	
			int	nResult = g_pMyNKApp->GetTerrain()->AddTexture( strTemp2 );
			if( nResult == 3 )
			{
				AddTexture( strTemp2 );
				g_pMyNKApp->SetModify( 0, TRUE );
			}
			else if( nResult == 0 )
				break;			
		}
	}

	m_ListBox.SetFocus();

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolTexPaintPage::OnToolPaintReplace() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilter[] = _T("Image Files (*.ntx, *.bmp)|*.ntx;*.bmp|Nako Texture Files (*.ntx)|*.ntx|")
						_T("Bitmap Files (*.bmp)|*.bmp||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								OFN_ALLOWMULTISELECT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s"), g_pMyNKApp->GetWorkingPath(),
							g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );

		if( g_pMyNKApp->GetTerrain()->ReplaceTexture( m_ListBox.GetCurSel(), FileDlg.GetFileName() ) )
		{
			int	nIndex = m_ListBox.GetCurSel();
			m_ListBox.AddListItem( FileDlg.GetFileName(), nIndex );
			g_pMyNKApp->SetModify( 0, TRUE );
			m_ListBox.SetCurSel( nIndex );
		}
	}

	m_ListBox.SetFocus();

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolTexPaintPage::OnToolPaintDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ListBox.GetCurSel();
	if( nIndex >= 0 )
	{
		if( g_pMyNKApp->GetTerrain()->DeleteTexture( nIndex ) )
		{
			m_ListBox.RemoveListItem( nIndex );

			if( m_ListBox.GetCount() == 0 )
			{
				g_pMyNKApp->SetCurSelTexture( -1 );
				m_ListBox.SetFocus();
				m_ListBox.SetCurSel( -1 );
			}
			else
			{
				if( nIndex == 0 )
				{
					g_pMyNKApp->SetCurSelTexture( nIndex );
					m_ListBox.SetFocus();
					m_ListBox.SetCurSel( nIndex );
				}
				else
				{
					g_pMyNKApp->SetCurSelTexture( nIndex - 1 );
					m_ListBox.SetFocus();
					m_ListBox.SetCurSel( nIndex - 1 );
				}
			}
			
			g_pMyNKApp->SetModify( 0, TRUE );
		}
	}
}


void CToolTexPaintPage::AddTexture(const TCHAR *szFilename)
{
	m_ListBox.AddListItem( szFilename );
	
	if( m_ListBox.GetCount() == 1 )
	{
		g_pMyNKApp->SetCurSelTexture( 0 );
		m_ListBox.SetCurSel( 0 );
	}
	
	m_ListBox.SetFocus();
}


void CToolTexPaintPage::RemoveAllTextures()
{
	m_ListBox.RemoveAllListItems();
	g_pMyNKApp->SetCurSelTexture( -1 );
}


void CToolTexPaintPage::OnToolPaintLayer1() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetEditingLayer( m_nEditLayer + 1 );
	m_bBrushTool = FALSE;
	g_pMyNKApp->SetCircleBrush( m_bBrushTool );
	m_ctrlToolType.SetBitmap( (HBITMAP)m_bmPencil );
	UpdateData( FALSE );
	GetDlgItem( IDC_TOOL_PAINT_STRENGTH_EDIT )->EnableWindow( FALSE );
	m_ctrlStrength.EnableWindow( FALSE );

	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER1 ))->SetBitmap( (HBITMAP)m_bmLayer1_Check );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER2 ))->SetBitmap( (HBITMAP)m_bmLayer2 );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER3 ))->SetBitmap( (HBITMAP)m_bmLayer3 );
}



void CToolTexPaintPage::OnToolPaintLayer2() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetEditingLayer( m_nEditLayer + 1 );
	m_bBrushTool = m_bLayer23Brush;
	UpdateData( FALSE );
	OnToolPaintToolType();
	GetDlgItem( IDC_TOOL_PAINT_STRENGTH_EDIT )->EnableWindow( TRUE );
	m_ctrlStrength.EnableWindow( TRUE );

	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER1 ))->SetBitmap( (HBITMAP)m_bmLayer1 );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER2 ))->SetBitmap( (HBITMAP)m_bmLayer2_Check );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER3 ))->SetBitmap( (HBITMAP)m_bmLayer3 );
}


void CToolTexPaintPage::OnToolPaintLayer3() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetEditingLayer( m_nEditLayer + 1 );
	m_bBrushTool = m_bLayer23Brush;
	UpdateData( FALSE );
	OnToolPaintToolType();
	GetDlgItem( IDC_TOOL_PAINT_STRENGTH_EDIT )->EnableWindow( TRUE );
	m_ctrlStrength.EnableWindow( TRUE );

	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER1 ))->SetBitmap( (HBITMAP)m_bmLayer1 );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER2 ))->SetBitmap( (HBITMAP)m_bmLayer2 );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_LAYER3 ))->SetBitmap( (HBITMAP)m_bmLayer3_Check );
}


void CToolTexPaintPage::OnToolPaintShowLayer1() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	ShowLayers();
}


void CToolTexPaintPage::OnToolPaintShowLayer2() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	ShowLayers();
}


void CToolTexPaintPage::OnToolPaintShowLayer3() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	ShowLayers();
}


void CToolTexPaintPage::ShowLayers() 
{
	// TODO: Add your control notification handler code here
	int	nShowLayers = 0;
	
	if( m_bShowLayer1 )
	{
		m_ctrlShowLayer1.SetBitmap( (HBITMAP)m_bmShowIcon );
		nShowLayers |= 1;
	}
	else
		m_ctrlShowLayer1.SetBitmap( (HBITMAP)m_bmHideIcon );

	if( m_bShowLayer2 )
	{
		m_ctrlShowLayer2.SetBitmap( (HBITMAP)m_bmShowIcon );
		nShowLayers |= 2;
	}
	else
		m_ctrlShowLayer2.SetBitmap( (HBITMAP)m_bmHideIcon );

	if( m_bShowLayer3 )
	{
		m_ctrlShowLayer3.SetBitmap( (HBITMAP)m_bmShowIcon );
		nShowLayers |= 4;
	}
	else
		m_ctrlShowLayer3.SetBitmap( (HBITMAP)m_bmHideIcon );

	g_pMyNKApp->GetTerrain()->SetViewLayers( nShowLayers );
}


void CToolTexPaintPage::ShowAllLayers()
{
	m_bShowLayer1 = TRUE;
	m_bShowLayer2 = TRUE;
	m_bShowLayer3 = TRUE;
	UpdateData( FALSE );

	m_ctrlShowLayer1.SetBitmap( (HBITMAP)m_bmShowIcon );
	m_ctrlShowLayer2.SetBitmap( (HBITMAP)m_bmShowIcon );
	m_ctrlShowLayer3.SetBitmap( (HBITMAP)m_bmShowIcon );

	g_pMyNKApp->GetTerrain()->SetViewLayers( 7 );
}


void CToolTexPaintPage::OnToolPaintFilter() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetFiltering( m_bFilter );
}


void CToolTexPaintPage::OnToolPaintTypeP()		// Painting
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetTexEditType( m_nEditType );
	
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_TYPE_P ))->SetBitmap( (HBITMAP)m_bmPaint_Check );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_TYPE_E ))->SetBitmap( (HBITMAP)m_bmErase );
}


void CToolTexPaintPage::OnToolPaintTypeE()		// Erasing
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetTexEditType( m_nEditType );

	((CButton*)GetDlgItem( IDC_TOOL_PAINT_TYPE_P ))->SetBitmap( (HBITMAP)m_bmPaint );
	((CButton*)GetDlgItem( IDC_TOOL_PAINT_TYPE_E ))->SetBitmap( (HBITMAP)m_bmErase_Check );
}


void CToolTexPaintPage::OnToolPaintToolType() 
{
	// TODO: Add your control notification handler code here
	if( m_nEditLayer != 0 )
	{
		UpdateData( TRUE );
		g_pMyNKApp->SetCircleBrush( m_bBrushTool );
		m_bLayer23Brush = m_bBrushTool;

		if( m_bBrushTool )
			m_ctrlToolType.SetBitmap( (HBITMAP)m_bmBrush );
		else
			m_ctrlToolType.SetBitmap( (HBITMAP)m_bmPencil );
	}
}


void CToolTexPaintPage::OnChangeToolPaintSizeEdit() 
{
	// TODO: Add your control notification handler code here
	int	nOldSize = m_nSize;
	UpdateData( TRUE );
	if( m_nSize < 1 || m_nSize > TEXTURE_MAX_COUNT+1 ) 
		m_nSize = nOldSize;
	else
	{
		g_pMyNKApp->SetBrushSize( m_nSize );
		m_ctrlSize.SetPos( m_nSize );
	}
	UpdateData( FALSE );
}


void CToolTexPaintPage::OnChangeToolPaintStrengthEdit() 
{
	// TODO: Add your control notification handler code here
	int	nOldStrength = m_nStrength;
	UpdateData( TRUE );
	if( m_nStrength < 1 || m_nStrength > 100 )
		m_nStrength = nOldStrength;
	else
	{
		g_pMyNKApp->SetBrushStrength( m_nStrength );
		m_ctrlStrength.SetPos( m_nStrength );
	}
	UpdateData( FALSE );
}


void CToolTexPaintPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl*	pSlider = (CSliderCtrl*)pScrollBar;

	switch( pSlider->GetDlgCtrlID() )
	{
	case IDC_TOOL_PAINT_SIZE_SLIDER:
		m_nSize = m_ctrlSize.GetPos();
		g_pMyNKApp->SetBrushSize( m_nSize );	
		UpdateData( FALSE );
		GetDlgItem( IDC_TOOL_PAINT_SIZE_EDIT )->SetFocus();
		((CEdit*)GetDlgItem( IDC_TOOL_PAINT_SIZE_EDIT ))->SetSel( 0, -1 );
		break;

	case IDC_TOOL_PAINT_STRENGTH_SLIDER:
		m_nStrength = m_ctrlStrength.GetPos();
		g_pMyNKApp->SetBrushStrength( m_nStrength );
		UpdateData( FALSE );
		GetDlgItem( IDC_TOOL_PAINT_STRENGTH_EDIT )->SetFocus();
		((CEdit*)GetDlgItem( IDC_TOOL_PAINT_STRENGTH_EDIT ))->SetSel( 0, -1 );
		break;
	}
	
	CPageCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////////////////
// 수정자 : 이원석
// 수정일 : 05-02-22
// 수정내용 : 다이얼로그에 포커스가 맞춰져있을 경우 메인 핫키가 먹지않아
//            강제로 Q,W,E,R 의 단축키 적용 처리.
BOOL CToolTexPaintPage::PreTranslateMessage( MSG* pMsg ) // 메시지를 받는 함수를 오버로딩.
{ 
	if( pMsg->message == WM_KEYDOWN  )
	{ 
		if( pMsg->wParam == 0x51 ) // Q
		{
			HK_PaintTypeP();
		}
		else if( pMsg->wParam == 0x57 ) // W
		{
			HK_PaintTypeE();
		}
/*
		else if( pMsg->wParam == 0x45 ) // E
		{
			OnToolObjectMove();
		}
		else if( pMsg->wParam == 0x52 ) // R
		{
			OnToolObjectRotate();
		}
*/
	} 

	return CDialog::PreTranslateMessage( pMsg ); 
} 
