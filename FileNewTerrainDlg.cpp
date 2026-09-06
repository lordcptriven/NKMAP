// FileNewTerrainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NKMapTool.h"
#include "FileNewTerrainDlg.h"
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
// CFileNewTerrainDlg dialog


CFileNewTerrainDlg::CFileNewTerrainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileNewTerrainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileNewTerrainDlg)
	m_strName = _T("");
	m_strWHSegment = _T("256");
	m_nPatchSize = 50;
	m_strTexture = _T("");
	m_strHeightMap = _T("");
	m_nHeightScale = 2;
	//}}AFX_DATA_INIT
}


void CFileNewTerrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileNewTerrainDlg)
	DDX_Control(pDX, IDC_NEWTERRAIN_WHSEGMENT, m_ctrlWHSegment);
	DDX_Text(pDX, IDC_NEWTERRAIN_NAME, m_strName);
	DDX_CBString(pDX, IDC_NEWTERRAIN_WHSEGMENT, m_strWHSegment);
	DDX_Text(pDX, IDC_NEWTERRAIN_PATCHSIZE, m_nPatchSize);
	DDX_Text(pDX, IDC_NEWTERRAIN_BASETEXNAME, m_strTexture);
	DDX_Text(pDX, IDC_NEWTERRAIN_HMNAME, m_strHeightMap);
	DDX_Text(pDX, IDC_NEWTERRAIN_HEIGHTSCALE, m_nHeightScale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileNewTerrainDlg, CDialog)
	//{{AFX_MSG_MAP(CFileNewTerrainDlg)
	ON_BN_CLICKED(IDC_NEWTERRAIN_OPENTEX, OnNewterrainOpentex)
	ON_BN_CLICKED(IDC_NEWTERRAIN_OPENHM, OnNewterrainOpenhm)
	ON_CBN_SELCHANGE(IDC_NEWTERRAIN_WHSEGMENT, OnSelchangeNewterrainWhsegment)
	ON_BN_CLICKED(IDC_NEWTERRAIN_GENERATE, OnNewterrainGenerate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileNewTerrainDlg message handlers

BOOL CFileNewTerrainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlWHSegment.AddString( _T("256") );
	m_ctrlWHSegment.AddString( _T("128") );
	m_ctrlWHSegment.AddString( _T("64") );
	m_ctrlWHSegment.AddString( _T("32") );
	m_ctrlWHSegment.SetCurSel( 0 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFileNewTerrainDlg::OnNewterrainOpentex() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	// Get a name of base texture
	TCHAR szFilter[] = _T("Image Files (*.ntx, *.bmp)|*.ntx;*.bmp|Nako Texture Files (*.ntx)|*.ntx|")
						_T("Bitmap Files (*.bmp)|*.bmp||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(), m_strName );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		m_strTexture = FileDlg.GetFileName();
		UpdateData( FALSE );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CFileNewTerrainDlg::OnNewterrainOpenhm() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	// Get a name of height map
	TCHAR szFilter[] = _T("Raw Files (*.raw)|*.raw||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(), m_strName );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		m_strHeightMap = FileDlg.GetFileName();
		UpdateData( FALSE );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CFileNewTerrainDlg::OnSelchangeNewterrainWhsegment() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	switch( m_ctrlWHSegment.GetCurSel() )
	{
	case 0:
		m_strWHSegment = _T("256");
		break;
	case 1:
		m_strWHSegment = _T("128");
		break;
	case 2:
		m_strWHSegment = _T("64");
		break;
	case 3:
		m_strWHSegment = _T("32");
		break;
	}

	UpdateData( FALSE );
}


void CFileNewTerrainDlg::OnNewterrainGenerate() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	// Check a patch size
	if( m_nPatchSize < 10 || m_nPatchSize > 100 )
	{
		MessageBox( _T("Patch Size는 10에서 100 사이의 정수이어야 합니다."), _T("NKMapTool"),
					MB_ICONWARNING | MB_OK );
		return ;
	}
	if( m_nPatchSize % 2 )
	{
		MessageBox( _T("Patch Size는 2의 배수이어야 합니다."), _T("NKMapTool"),
					MB_ICONWARNING | MB_OK );
		return ;
	}

	// Check a height scale
	if( m_nHeightScale < 1 || m_nHeightScale > 10 )
	{
		MessageBox( _T("Height Scale은 1에서 10 사이의 정수이어야 합니다."), _T("NKMapTool"),
					MB_ICONWARNING | MB_OK );
		return ;
	}

	// Check a terrain name
	if( m_strName.Compare( _T("") ) == 0 )
	{
		MessageBox( _T("지형 이름을 입력해야 합니다."),
					_T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return ;
	}

	CDialog::OnOK();
}
