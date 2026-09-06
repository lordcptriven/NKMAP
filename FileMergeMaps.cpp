// FileMergeMaps.cpp : implementation file
//

#include "stdafx.h"
#include "nkmaptool.h"
#include "FileMergeMaps.h"
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
// CFileMergeMaps dialog


CFileMergeMaps::CFileMergeMaps(CWnd* pParent /*=NULL*/)
	: CDialog(CFileMergeMaps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileMergeMaps)
	m_strNmfName = _T("");
	m_strNotName = _T("");
	m_nMergeSX = 0;
	m_nMergeSZ = 0;
	m_nMergeEX = 0;
	m_nMergeEZ = 0;
	//}}AFX_DATA_INIT
}


void CFileMergeMaps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileMergeMaps)
	DDX_Text(pDX, IDC_MERGEMAP_NMF_NAME, m_strNmfName);
	DDX_Text(pDX, IDC_MERGEMAP_NOT_NAME, m_strNotName);
	DDX_Text(pDX, IDC_MERGEMAP_REGION_SX, m_nMergeSX);
	DDX_Text(pDX, IDC_MERGEMAP_REGION_SZ, m_nMergeSZ);
	DDX_Text(pDX, IDC_MERGEMAP_REGION_EX, m_nMergeEX);
	DDX_Text(pDX, IDC_MERGEMAP_REGION_EZ, m_nMergeEZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileMergeMaps, CDialog)
	//{{AFX_MSG_MAP(CFileMergeMaps)
	ON_BN_CLICKED(IDC_MERGEMAP_NMF_OPEN, OnMergemapNmfOpen)
	ON_BN_CLICKED(IDC_MERGEMAP_NOT_OPEN, OnMergemapNotOpen)
	ON_EN_CHANGE(IDC_MERGEMAP_REGION_SX, OnChangeMergemapRegionSx)
	ON_EN_CHANGE(IDC_MERGEMAP_REGION_SZ, OnChangeMergemapRegionSz)
	ON_EN_CHANGE(IDC_MERGEMAP_REGION_EX, OnChangeMergemapRegionEx)
	ON_EN_CHANGE(IDC_MERGEMAP_REGION_EZ, OnChangeMergemapRegionEz)
	ON_BN_CLICKED(IDC_MERGEMAP_MERGE, OnMergemapMerge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileMergeMaps message handlers

BOOL CFileMergeMaps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() )
		{
			m_nMergeEX = g_pMyNKApp->GetTerrain()->GetHMWidth() - 2;
			m_nMergeEZ = m_nMergeEX;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFileMergeMaps::OnMergemapNmfOpen() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	// Get a name of height map
	TCHAR szFilter[] = _T("Nmf Files (*.nmf)|*.nmf||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%smap\\%s\\"), g_pMyNKApp->GetWorkingPath(),
											g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		m_strNmfName = FileDlg.GetFileName();
		UpdateData( FALSE );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CFileMergeMaps::OnMergemapNotOpen() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilter[] = _T("Nmf Files (*.not)|*.not||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%sdata\\building\\%s\\"), g_pMyNKApp->GetWorkingPath(),
											g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		m_strNotName = FileDlg.GetFileName();
		UpdateData( FALSE );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CFileMergeMaps::OnChangeMergemapRegionSx() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_nMergeSX < 0 )
		m_nMergeSX = 0;
	else if( m_nMergeSX > g_pMyNKApp->GetTerrain()->GetHMWidth() - 2 )
		m_nMergeSX = g_pMyNKApp->GetTerrain()->GetHMWidth() - 2;

	UpdateData( FALSE );		
}


void CFileMergeMaps::OnChangeMergemapRegionSz() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_nMergeSZ < 0 )
		m_nMergeSZ = 0;
	else if( m_nMergeSZ > g_pMyNKApp->GetTerrain()->GetHMWidth() - 2 )
		m_nMergeSZ = g_pMyNKApp->GetTerrain()->GetHMWidth() - 2;

	UpdateData( FALSE );
}


void CFileMergeMaps::OnChangeMergemapRegionEx() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_nMergeEX < 0 )
		m_nMergeEX = 0;
	else if( m_nMergeEX > g_pMyNKApp->GetTerrain()->GetHMWidth() - 2 )
		m_nMergeEX = g_pMyNKApp->GetTerrain()->GetHMWidth() - 2;

	UpdateData( FALSE );
}


void CFileMergeMaps::OnChangeMergemapRegionEz() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_nMergeEZ < 0 )
		m_nMergeEZ = 0;
	else if( m_nMergeEZ > g_pMyNKApp->GetTerrain()->GetHMWidth() - 2 )
		m_nMergeEZ = g_pMyNKApp->GetTerrain()->GetHMWidth() - 2;

	UpdateData( FALSE );
}


void CFileMergeMaps::OnMergemapMerge() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( m_strNmfName.IsEmpty() )
	{
		MessageBox( _T("Nmf 파일 이름이 없습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return ;
	}

	if( m_nMergeSX % 4 || m_nMergeSZ % 4 )
	{
		MessageBox( _T("Start region은 4의 배수이어야 합니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return ;
	}

	if( ( m_nMergeEX + 1 ) % 4 || ( m_nMergeEZ + 1 ) % 4 )
	{
		MessageBox( _T("End region은 4의 배수 - 1 이어야 합니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return ;
	}

	if( m_nMergeSX >= m_nMergeEX || m_nMergeSZ >= m_nMergeEZ )
	{
		MessageBox( _T("Start region이 End region보다 크거나 같습니다."), _T("NKMapTool"), MB_ICONWARNING | MB_OK );
		return ;
	}

	CDialog::OnOK();
}
