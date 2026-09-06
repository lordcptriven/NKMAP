// ToolAtmosEditPage.cpp : implementation file
//

#include "stdafx.h"
#include "nkmaptool.h"
#include "ToolAtmosEditPage.h"
#include "MyNKApp.h"
#include "MyFileDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global variables
extern CMyNKApp*		g_pMyNKApp;
extern CNKObjQuadNode*	g_pObjTree;


/////////////////////////////////////////////////////////////////////////////
// CToolAtmosEditPage dialog


CToolAtmosEditPage::CToolAtmosEditPage(CWnd* pParent /*=NULL*/)
	: CPageCtrl(CToolAtmosEditPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolAtmosEditPage)
	m_strBaseWaterTex = _T("");
	m_nWaterType = 0;
	m_nEditType = 0;
	m_fWaterHeight = 0.0f;
	m_nWaterOpacity = 0;
	m_strWaterGroup = _T("");
	m_strWaveWaterTex2 = _T("");
	m_strWaveWaterTex1 = _T("");
	m_nWaterSegment = 4;
	m_strComboGroupName = _T("");
	m_bWaterGuideLine = TRUE;
	//}}AFX_DATA_INIT
}


void CToolAtmosEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPageCtrl::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolAtmosEditPage)
	DDX_Control(pDX, IDC_TOOL_ATMOS_WATER_GROUP, m_ctrlWaterGroup);
	DDX_Text(pDX, IDC_TOOL_ATMOS_WATER_BASE_TEXNAME, m_strBaseWaterTex);
	DDX_Radio(pDX, IDC_TOOL_ATMOS_WATER_TYPE_BASE, m_nWaterType);
	DDX_Radio(pDX, IDC_TOOL_ATMOS_WATER_EDIT_PAINT, m_nEditType);
	DDX_Text(pDX, IDC_TOOL_ATMOS_WATER_HEIGHT, m_fWaterHeight);
	DDX_Text(pDX, IDC_TOOL_ATMOS_WATER_OPACITY, m_nWaterOpacity);
	DDX_Text(pDX, IDC_TOOL_ATMOS_WATER_GROUP_NAME, m_strWaterGroup);
	DDX_Text(pDX, IDC_TOOL_ATMOS_WATER_WAVE_TEXNAME2, m_strWaveWaterTex2);
	DDX_Text(pDX, IDC_TOOL_ATMOS_WATER_WAVE_TEXNAME, m_strWaveWaterTex1);
	DDX_Text(pDX, IDC_TOOL_ATMOS_WATER_SEGMENT, m_nWaterSegment);
	DDX_CBString(pDX, IDC_TOOL_ATMOS_WATER_GROUP, m_strComboGroupName);
	DDX_Check(pDX, IDC_TOOL_ATMOS_WATER_GUIDE_LINE, m_bWaterGuideLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolAtmosEditPage, CPageCtrl)
	//{{AFX_MSG_MAP(CToolAtmosEditPage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_BASE_TEXNAME_OPEN, OnToolAtmosWaterBaseTexnameOpen)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_WAVE_TEXNAME_OPEN, OnToolAtmosWaterWaveTexnameOpen)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_TYPE_BASE, OnToolAtmosWaterTypeBase)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_TYPE_WAVE, OnToolAtmosWaterTypeWave)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_EDIT_ERASE, OnToolAtmosWaterEditErase)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_EDIT_PAINT, OnToolAtmosWaterEditPaint)
	ON_EN_CHANGE(IDC_TOOL_ATMOS_WATER_OPACITY, OnChangeToolAtmosWaterOpacity)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TOOL_ATMOS_WATER_OPACITY_SPIN, OnDeltaposToolAtmosWaterOpacitySpin)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_GROUP_ADD, OnToolAtmosWaterGroupAdd)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_GROUP_MODIFY, OnToolAtmosWaterGroupModify)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_GROUP_REMOVE, OnToolAtmosWaterGroupRemove)
	ON_CBN_SELCHANGE(IDC_TOOL_ATMOS_WATER_GROUP, OnSelchangeToolAtmosWaterGroup)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_WAVE_TEXNAME_OPEN2, OnToolAtmosWaterWaveTexnameOpen2)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_PAINT_ALL, OnToolAtmosWaterPaintAll)
	ON_EN_CHANGE(IDC_TOOL_ATMOS_WATER_SEGMENT, OnChangeToolAtmosWaterSegment)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TOOL_ATMOS_WATER_SEGMENT_SPIN, OnDeltaposToolAtmosWaterSegmentSpin)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_GUIDE_LINE, OnToolAtmosWaterGuideLine)
	ON_BN_CLICKED(IDC_TOOL_ATMOS_WATER_PAINT_ALLWAVE, OnToolAtmosWaterPaintAllwave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolAtmosEditPage message handlers


BOOL CToolAtmosEditPage::Create( CWnd *pParent )
{
	if( !CPageCtrl::Create( CToolAtmosEditPage::IDD, pParent ) )
		return FALSE;

	return TRUE;
}


BOOL CToolAtmosEditPage::OnInitDialog() 
{
	CPageCtrl::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bmPaint.LoadBitmap( IDB_TOOL_PAINT_S );
	m_bmErase.LoadBitmap( IDB_TOOL_ERASE_S );
	m_bmPaint_Check.LoadBitmap( IDB_TOOL_PAINT_ON_S );
	m_bmErase_Check.LoadBitmap( IDB_TOOL_ERASE_ON_S );
	m_bmBase.LoadBitmap( IDB_TOOL_WATER );
	m_bmWave.LoadBitmap( IDB_TOOL_WAVE );
	m_bmBase_Check.LoadBitmap( IDB_TOOL_WATER_ON );
	m_bmWave_Check.LoadBitmap( IDB_TOOL_WAVE_ON );

	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_EDIT_PAINT ))->SetBitmap( (HBITMAP)m_bmPaint_Check );
	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_EDIT_ERASE ))->SetBitmap( (HBITMAP)m_bmErase );
	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_TYPE_BASE ))->SetBitmap( (HBITMAP)m_bmBase_Check );
	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_TYPE_WAVE ))->SetBitmap( (HBITMAP)m_bmWave );

	GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_NAME )->EnableWindow( FALSE );
	GetDlgItem( IDC_TOOL_ATMOS_WATER_HEIGHT )->EnableWindow( FALSE );
	GetDlgItem( IDC_TOOL_ATMOS_WATER_SEGMENT )->EnableWindow( FALSE );
	GetDlgItem( IDC_TOOL_ATMOS_WATER_SEGMENT_SPIN )->EnableWindow( FALSE );

	m_nCurWaterGroup = -1;
	m_bGroupAddActive = FALSE;
	m_bGroupModifyActive = FALSE;
	m_bGroupRemoveActive = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CToolAtmosEditPage::OnDestroy() 
{
	CPageCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	m_ctrlWaterGroup.ResetContent();
}


void CToolAtmosEditPage::ResetControls()
{
	m_ctrlWaterGroup.ResetContent();
}


void CToolAtmosEditPage::UpdateControls()
{
	if( CNKObjQuadNode::GetBaseWaterTexID() != -1 )
		m_strBaseWaterTex = g_pMyNKApp->GetDecoTexture()->GetTextureName( CNKObjQuadNode::GetBaseWaterTexID() );
	else
		m_strBaseWaterTex = _T("");

	if( CNKObjQuadNode::GetWaveWaterTexID1() != -1 )
		m_strWaveWaterTex1 = g_pMyNKApp->GetDecoTexture()->GetTextureName( CNKObjQuadNode::GetWaveWaterTexID1() );
	else
		m_strWaveWaterTex1 = _T("");

	if( CNKObjQuadNode::GetWaveWaterTexID2() != -1 )
		m_strWaveWaterTex2 = g_pMyNKApp->GetDecoTexture()->GetTextureName( CNKObjQuadNode::GetWaveWaterTexID2() );
	else
		m_strWaveWaterTex2 = _T("");

	m_nWaterOpacity = CNKObjQuadNode::GetBaseWaterAlpha() >> 24;

	m_nCurWaterGroup = m_ctrlWaterGroup.GetCurSel();
	m_ctrlWaterGroup.ResetContent();
	for( int i = 0; i < 20; i++ )
	{
		if( CNKObjQuadNode::GetHeightOfWaterGroup( i ) >= 0.0f )
		{
			m_aWaterHeights[i] = CNKObjQuadNode::GetHeightOfWaterGroup( i );
			m_aGroupNames[i] = CNKObjQuadNode::GetNameOfWaterGroup( i );
			m_ctrlWaterGroup.AddString( m_aGroupNames[i] );
		}
		else
			break;
	}

	if( m_ctrlWaterGroup.GetCount() > 0 )
	{
		if( m_nCurWaterGroup == -1 )
			m_nCurWaterGroup = 0;
		
		m_strWaterGroup = m_aGroupNames[m_nCurWaterGroup];
		m_fWaterHeight = m_aWaterHeights[m_nCurWaterGroup];
		m_strComboGroupName = m_strWaterGroup;
	}
	else
	{
		m_nCurWaterGroup = -1;
		m_strWaterGroup = _T("");
		m_fWaterHeight = 0.0f;
		m_nWaterSegment = 4;
		m_strComboGroupName = _T("");
	}

	m_ctrlWaterGroup.SetCurSel( m_nCurWaterGroup );
	UpdateData( FALSE );

	g_pMyNKApp->SetWaterGroupID( m_nCurWaterGroup );
	g_pMyNKApp->SetWaterSegment( m_nWaterSegment );
	g_pMyNKApp->SetShowWaterGuideLine( m_bWaterGuideLine );
	g_pMyNKApp->GetTerrain()->SetViewLayers( 7 );
}


void CToolAtmosEditPage::OnToolAtmosWaterBaseTexnameOpen() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	// Get a name of base texture
	TCHAR szFilter[] = _T("Image Files (*.ntx, *.bmp)|*.ntx;*.bmp|Nako Texture Files (*.ntx)|*.ntx|")
						_T("Bitmap Files (*.bmp)|*.bmp||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%sdata\\building\\textr\\"), g_pMyNKApp->GetWorkingPath() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		CString	strTemp = FileDlg.GetFileName();
		if( strTemp.Compare( m_strBaseWaterTex ) )
		{
			m_strBaseWaterTex = strTemp;
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
			g_pMyNKApp->SetBaseWaterTexture( m_strBaseWaterTex );
			UpdateData( FALSE );
		}
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolAtmosEditPage::OnToolAtmosWaterWaveTexnameOpen() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	// Get a name of base texture
	TCHAR szFilter[] = _T("Image Files (*.ntx, *.bmp)|*.ntx;*.bmp|Nako Texture Files (*.ntx)|*.ntx|")
						_T("Bitmap Files (*.bmp)|*.bmp|");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%sdata\\building\\textr\\"), g_pMyNKApp->GetWorkingPath() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		CString	strTemp = FileDlg.GetFileName();
		if( strTemp.Compare( m_strWaveWaterTex1 ) )
		{
			m_strWaveWaterTex1 = strTemp;
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
			g_pMyNKApp->SetWaveWaterTexture1( m_strWaveWaterTex1 );
			UpdateData( FALSE );
		}
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolAtmosEditPage::OnToolAtmosWaterWaveTexnameOpen2() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	// Get a name of base texture
	TCHAR szFilter[] = _T("Image Files (*.ntx, *.bmp)|*.ntx;*.bmp|Nako Texture Files (*.ntx)|*.ntx|")
						_T("Bitmap Files (*.bmp)|*.bmp||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%sdata\\building\\textr\\"), g_pMyNKApp->GetWorkingPath() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		CString	strTemp = FileDlg.GetFileName();
		if( strTemp.Compare( m_strWaveWaterTex2 ) )
		{
			m_strWaveWaterTex2 = strTemp;
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
			g_pMyNKApp->SetWaveWaterTexture2( m_strWaveWaterTex2 );
			UpdateData( FALSE );
		}
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolAtmosEditPage::OnChangeToolAtmosWaterOpacity() 
{
	// TODO: Add your control notification handler code here
	float	fOldOpacity = m_nWaterOpacity;

	UpdateData( TRUE );

	if( m_nWaterOpacity < 0 || m_nWaterOpacity > 255 )
	{
		MessageBox( _T("Water Opacity는 1에서 255 사이의 정수이어야 합니다."), _T("NKMapTool"),
						MB_ICONWARNING | MB_OK );
		m_nWaterOpacity = fOldOpacity;
	}
	else
		CNKObjQuadNode::SetBaseWaterAlpha( m_nWaterOpacity );

	UpdateData( FALSE );
}


void CToolAtmosEditPage::OnDeltaposToolAtmosWaterOpacitySpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		m_nWaterOpacity--;
	else if( pNMUpDown->iDelta < 0 )
		m_nWaterOpacity++;

	if( m_nWaterOpacity < 0 ) m_nWaterOpacity = 255;
	else if( m_nWaterOpacity > 255 ) m_nWaterOpacity = 0;

	CNKObjQuadNode::SetBaseWaterAlpha( m_nWaterOpacity << 24 );
	UpdateData( FALSE );
	
	*pResult = 1;
}


void CToolAtmosEditPage::OnSelchangeToolAtmosWaterGroup() 
{
	// TODO: Add your control notification handler code here
	m_nCurWaterGroup = m_ctrlWaterGroup.GetCurSel();
	m_strWaterGroup = m_aGroupNames[m_nCurWaterGroup];
	m_strComboGroupName = m_strWaterGroup;
	m_fWaterHeight = m_aWaterHeights[m_nCurWaterGroup];
	g_pMyNKApp->SetWaterGroupID( m_nCurWaterGroup );
	UpdateData( FALSE );
}


void CToolAtmosEditPage::OnToolAtmosWaterGroupAdd() 
{
	// TODO: Add your control notification handler code here
	if( m_bGroupModifyActive || m_bGroupRemoveActive )
		return ;

	if( m_ctrlWaterGroup.GetCount() == 19 )
	{
		MessageBox( _T("최대 지원되는 Water Group은 19개입니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return ;
	}

	m_bGroupAddActive = !m_bGroupAddActive;

	if( m_bGroupAddActive )
	{
		GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_NAME )->EnableWindow( TRUE );
		GetDlgItem( IDC_TOOL_ATMOS_WATER_HEIGHT )->EnableWindow( TRUE );
		GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_ADD )->SetWindowText( _T("Apply") );
	}
	else
	{
		UpdateData( TRUE );

		if( m_fWaterHeight < 0.0f || m_fWaterHeight > g_pMyNKApp->GetTerrain()->GetHeightScale() * 255 )
		{
			CString	strTemp;
			int		nHeight = g_pMyNKApp->GetTerrain()->GetHeightScale() * 255;
			strTemp.Format( _T("물의 높이는 0에서 %d 사이어야 합니다."), nHeight );
			MessageBox( strTemp, _T("NKMapTool"), MB_ICONWARNING | MB_OK );
			m_bGroupAddActive = !m_bGroupAddActive;
			return ;
		}

		if( m_strWaterGroup.IsEmpty() )
		{
			MessageBox( _T("그룹 이름을 입력하세요."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
			m_bGroupAddActive = !m_bGroupAddActive;
			return ;
		}
	
		m_nCurWaterGroup = m_ctrlWaterGroup.GetCount();
		m_ctrlWaterGroup.AddString( m_strWaterGroup );
		m_aGroupNames[m_nCurWaterGroup] = m_strWaterGroup;
		m_aWaterHeights[m_nCurWaterGroup] = m_fWaterHeight;
		m_ctrlWaterGroup.SetCurSel( m_nCurWaterGroup );

		CNKObjQuadNode::SetNameOfWaterGroup( m_nCurWaterGroup, m_aGroupNames[m_nCurWaterGroup] );
		CNKObjQuadNode::SetHeightOfWaterGroup( m_nCurWaterGroup, m_fWaterHeight );
		
		g_pMyNKApp->SetWaterGroupID( m_nCurWaterGroup );

		GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_NAME )->EnableWindow( FALSE );
		GetDlgItem( IDC_TOOL_ATMOS_WATER_HEIGHT )->EnableWindow( FALSE );
		GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_ADD )->SetWindowText( _T("Add") );
	}
}


void CToolAtmosEditPage::OnToolAtmosWaterGroupModify() 
{
	// TODO: Add your control notification handler code here
	if( m_nCurWaterGroup == -1 )
		return ;

	if( m_bGroupAddActive || m_bGroupRemoveActive )
		return ;

	m_bGroupModifyActive = !m_bGroupModifyActive;

	if( m_bGroupModifyActive )
	{
		GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_NAME )->EnableWindow( TRUE );
		GetDlgItem( IDC_TOOL_ATMOS_WATER_HEIGHT )->EnableWindow( TRUE );
		GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_MODIFY )->SetWindowText( _T("Apply") );
	}
	else
	{
		UpdateData( TRUE );

		if( m_fWaterHeight < 0.0f || m_fWaterHeight > g_pMyNKApp->GetTerrain()->GetHeightScale() * 255 )
		{
			CString	strTemp;
			int		nHeight = g_pMyNKApp->GetTerrain()->GetHeightScale() * 255;
			strTemp.Format( _T("물의 높이는 0에서 %d사이어야 합니다."), nHeight );
			MessageBox( strTemp, _T("NKMapTool"),	MB_ICONWARNING | MB_OK );
			m_bGroupAddActive = !m_bGroupAddActive;
			return ;
		}

		if( m_strWaterGroup.IsEmpty() )
		{
			MessageBox( _T("그룹 이름을 입력하세요."), _T("NKMapTool"),	MB_ICONWARNING | MB_OK );
			m_bGroupAddActive = !m_bGroupAddActive;
			return ;
		}
	
		m_nCurWaterGroup = m_ctrlWaterGroup.GetCurSel();
		m_ctrlWaterGroup.DeleteString( m_nCurWaterGroup );
		m_ctrlWaterGroup.InsertString( m_nCurWaterGroup, m_strWaterGroup );
		m_aGroupNames[m_nCurWaterGroup] = m_strWaterGroup;
		m_aWaterHeights[m_nCurWaterGroup] = m_fWaterHeight;
		m_ctrlWaterGroup.SetCurSel( m_nCurWaterGroup );

		CNKObjQuadNode::SetNameOfWaterGroup( m_nCurWaterGroup, m_aGroupNames[m_nCurWaterGroup] );
		CNKObjQuadNode::SetHeightOfWaterGroup( m_nCurWaterGroup, m_fWaterHeight );
		g_pObjTree->ModifiedWaterHeight( m_nCurWaterGroup );

		g_pMyNKApp->SetWaterGroupID( m_nCurWaterGroup );
		g_pMyNKApp->SetModify( 2, TRUE );

		GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_NAME )->EnableWindow( FALSE );
		GetDlgItem( IDC_TOOL_ATMOS_WATER_HEIGHT )->EnableWindow( FALSE );
		GetDlgItem( IDC_TOOL_ATMOS_WATER_GROUP_MODIFY )->SetWindowText( _T("Modify") );
	}
}


void CToolAtmosEditPage::OnToolAtmosWaterGroupRemove() 
{
	// TODO: Add your control notification handler code here
	if( m_nCurWaterGroup == -1 )
		return ;

	if( m_bGroupAddActive || m_bGroupModifyActive )
		return ;

	if( MessageBox( _T("정말로 삭제하시겠습니까?"), _T("NKMapTool"),
		MB_ICONASTERISK | MB_YESNO ) == IDYES )
	{
		CNKObjQuadNode::RemoveWaterGroup( m_nCurWaterGroup );

		m_ctrlWaterGroup.DeleteString( m_nCurWaterGroup );
		for( int i = m_nCurWaterGroup; i < 19; i++ )
		{
			m_aGroupNames[i] = m_aGroupNames[i+1];
			m_aWaterHeights[i] = m_aWaterHeights[i+1];
			CNKObjQuadNode::SetNameOfWaterGroup( i, m_aGroupNames[i] );
			CNKObjQuadNode::SetHeightOfWaterGroup( i, m_aWaterHeights[i] );
		}

		if( m_ctrlWaterGroup.GetCount() == 0 )
			m_nCurWaterGroup = -1;
		else
			m_nCurWaterGroup--;

		m_ctrlWaterGroup.SetCurSel( m_nCurWaterGroup );
		g_pMyNKApp->SetWaterGroupID( m_nCurWaterGroup );
		g_pMyNKApp->SetModify( 2, TRUE );

		if( m_nCurWaterGroup != -1 )
		{
			m_strWaterGroup = m_aGroupNames[m_nCurWaterGroup];
			m_fWaterHeight = m_aWaterHeights[m_nCurWaterGroup];
		}

		UpdateData( FALSE );
	}
}


void CToolAtmosEditPage::OnToolAtmosWaterTypeBase() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetWaterType( m_nWaterType );
	g_pMyNKApp->SetBrushSize( 2 );

	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_TYPE_BASE ))->SetBitmap( (HBITMAP)m_bmBase_Check );
	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_TYPE_WAVE ))->SetBitmap( (HBITMAP)m_bmWave );

	GetDlgItem( IDC_TOOL_ATMOS_WATER_SEGMENT )->EnableWindow( FALSE );
	GetDlgItem( IDC_TOOL_ATMOS_WATER_SEGMENT_SPIN )->EnableWindow( FALSE );
}


void CToolAtmosEditPage::OnToolAtmosWaterTypeWave() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetWaterType( m_nWaterType );
	g_pMyNKApp->SetBrushSize( 1 );

	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_TYPE_BASE ))->SetBitmap( (HBITMAP)m_bmBase );
	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_TYPE_WAVE ))->SetBitmap( (HBITMAP)m_bmWave_Check );

	GetDlgItem( IDC_TOOL_ATMOS_WATER_SEGMENT )->EnableWindow( TRUE );
	GetDlgItem( IDC_TOOL_ATMOS_WATER_SEGMENT_SPIN )->EnableWindow( TRUE );
}


void CToolAtmosEditPage::OnChangeToolAtmosWaterSegment() 
{
	// TODO: Add your control notification handler code here
	int	nOldWaterSegment = m_nWaterSegment;

	UpdateData( TRUE );

	if( m_nWaterSegment < 1 || m_nWaterSegment > 8 )
	{
		MessageBox( _T("Wave Segment는 1에서 8 사이의 정수이어야 합니다."),
					_T("NKMapTool"), MB_ICONWARNING | MB_OK );
		m_nWaterSegment = nOldWaterSegment;
	}
	else
		g_pMyNKApp->SetWaterSegment( m_nWaterSegment );

	UpdateData( FALSE );
}


void CToolAtmosEditPage::OnDeltaposToolAtmosWaterSegmentSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( pNMUpDown->iDelta > 0 )
		m_nWaterSegment--;
	else if( pNMUpDown->iDelta < 0 )
		m_nWaterSegment++;

	if( m_nWaterSegment < 1 )
		m_nWaterSegment = 1;
	else if( m_nWaterSegment > 8 )
		m_nWaterSegment = 8;

	g_pMyNKApp->SetWaterSegment( m_nWaterSegment );
	UpdateData( FALSE );
	
	*pResult = 1;
}


void CToolAtmosEditPage::OnToolAtmosWaterPaintAll() 
{
	// TODO: Add your control notification handler code here
	int	nIndex = m_ctrlWaterGroup.GetCurSel();
	if( nIndex < 0 )
		return;

	g_pObjTree->PaintAllBaseWater( nIndex );
	g_pMyNKApp->SetModify( 2, TRUE );
}


void CToolAtmosEditPage::OnToolAtmosWaterPaintAllwave() 
{
	// TODO: Add your control notification handler code here
	int	nIndex = m_ctrlWaterGroup.GetCurSel();
	if( nIndex < 0 )
		return;

	CNKObjQuadNode::PaintAllWaveWater( nIndex );
	g_pMyNKApp->SetModify( 2, TRUE );
}


void CToolAtmosEditPage::OnToolAtmosWaterEditPaint() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetWaterEditType( m_nEditType );

	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_EDIT_PAINT ))->SetBitmap( (HBITMAP)m_bmPaint_Check );
	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_EDIT_ERASE ))->SetBitmap( (HBITMAP)m_bmErase );
}


void CToolAtmosEditPage::OnToolAtmosWaterEditErase() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetWaterEditType( m_nEditType );

	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_EDIT_PAINT ))->SetBitmap( (HBITMAP)m_bmPaint );
	((CButton*)GetDlgItem( IDC_TOOL_ATMOS_WATER_EDIT_ERASE ))->SetBitmap( (HBITMAP)m_bmErase_Check );
}


void CToolAtmosEditPage::SetWaveWaterSegment( int nSegment )
{
	m_nWaterSegment = nSegment;
	UpdateData( FALSE );
}


void CToolAtmosEditPage::OnToolAtmosWaterGuideLine() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetShowWaterGuideLine( m_bWaterGuideLine );
}


void CToolAtmosEditPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
/*	CRect		rc;
	CBrush		black( RGB( 0, 0, 0 ) );

	// Diffuse color of a dawn sky
	COLORREF	crDawn = ( m_nSkyTex1DiffuseB << 16 ) | ( m_nSkyTex1DiffuseG << 8 ) | m_nSkyTex1DiffuseR;
	CBrush		brDawn( crDawn );
	CPaintDC	dcDawn( GetDlgItem( IDC_TOOL_ATMOS_SKY_TEX1_COLOR ) );
	GetDlgItem( IDC_TOOL_ATMOS_SKY_TEX1_COLOR )->GetClientRect( rc );
	dcDawn.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcDawn.FillRect( &rc, &brDawn );

	// Diffuse color of a day sky
	COLORREF	crDay = ( m_nSkyTex2DiffuseB << 16 ) | ( m_nSkyTex2DiffuseG << 8 ) | m_nSkyTex2DiffuseR;
	CBrush		brDay( crDay );
	CPaintDC	dcDay( GetDlgItem( IDC_TOOL_ATMOS_SKY_TEX2_COLOR ) );
	GetDlgItem( IDC_TOOL_ATMOS_SKY_TEX2_COLOR )->GetClientRect( rc );
	dcDay.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcDay.FillRect( &rc, &brDay );

	// Diffuse color of a evening sky
	COLORREF	crEvening = ( m_nSkyTex3DiffuseB << 16 ) | ( m_nSkyTex3DiffuseG << 8 ) | m_nSkyTex3DiffuseR;
	CBrush		brEvening( crEvening );
	CPaintDC	dcEvening( GetDlgItem( IDC_TOOL_ATMOS_SKY_TEX3_COLOR ) );
	GetDlgItem( IDC_TOOL_ATMOS_SKY_TEX3_COLOR )->GetClientRect( rc );
	dcEvening.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcEvening.FillRect( &rc, &brEvening );
	
	// Diffuse color of a night sky
	COLORREF	crNight = ( m_nSkyTex4DiffuseB << 16 ) | ( m_nSkyTex4DiffuseG << 8 ) | m_nSkyTex4DiffuseR;
	CBrush		brNight( crNight );
	CPaintDC	dcNight( GetDlgItem( IDC_TOOL_ATMOS_SKY_TEX4_COLOR ) );
	GetDlgItem( IDC_TOOL_ATMOS_SKY_TEX4_COLOR )->GetClientRect( rc );
	dcNight.FillRect( &rc, &black );
	rc.DeflateRect( 1, 1 );
	dcNight.FillRect( &rc, &brNight );*/
}


