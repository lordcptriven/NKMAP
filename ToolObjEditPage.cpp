// ToolObjEditPage.cpp : implementation file
//

#include "stdafx.h"
#include "NKMapTool.h"
#include "ToolObjEditPage.h"
#include "MyFileDialog.h"
#include "MyNKApp.h"
#include "DecoObj.h"
#include "NKMath.h"
#include "Undo_Object.h"







#include "GUtil.h"


#include <vector>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global variables
extern CMyNKApp*		g_pMyNKApp;
extern CDecoObjSet*		g_pDecoObjSet;
extern CNKObjQuadNode*	g_pObjTree;

/////////////////////////////////////////////////////////////////////////////
// CToolObjEditPage dialog


CToolObjEditPage::CToolObjEditPage(CWnd* pParent /*=NULL*/)
	: CPageCtrl(CToolObjEditPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolObjEditPage)
	m_strLolo = _T("");
	m_nIndex = 0;
	m_strName = _T("");
	m_strType = _T("");
	m_nWorkType = 0;
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fRotate = 0.0f;
	m_bRandomRotation = FALSE;
	m_fOffset = 0.0f;
	m_bShowNodeBound = FALSE;
	m_nAxis = 0;
	m_nCount = 0;
	m_nReferenceCount = 0;
	m_bLocalAxis = FALSE;
	//}}AFX_DATA_INIT
}


void CToolObjEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPageCtrl::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolObjEditPage)
	DDX_Control(pDX, IDC_TOOL_OBJECT_TYPE, m_ctrlType);
	DDX_Control(pDX, IDC_TOOL_OBJECT_LISTBOX, m_ctrlObjList);
	DDX_Text(pDX, IDC_TOOL_OBJECT_LOLO, m_strLolo);
	DDX_Text(pDX, IDC_TOOL_OBJECT_INDEX, m_nIndex);
	DDX_Text(pDX, IDC_TOOL_OBJECT_NAME, m_strName);
	DDX_CBString(pDX, IDC_TOOL_OBJECT_TYPE, m_strType);
	DDX_Radio(pDX, IDC_TOOL_OBJECT_SELECT, m_nWorkType);
	DDX_Text(pDX, IDC_TOOL_OBJECT_X_EDIT, m_fX);
	DDX_Text(pDX, IDC_TOOL_OBJECT_Y_EDIT, m_fY);
	DDX_Text(pDX, IDC_TOOL_OBJECT_Z_EDIT, m_fZ);
	DDX_Text(pDX, IDC_TOOL_OBJECT_ROTATE_EDIT, m_fRotate);
	DDX_Check(pDX, IDC_TOOL_OBJECT_RANDOM, m_bRandomRotation);
	DDX_Text(pDX, IDC_TOOL_OBJECT_OFFSET, m_fOffset);
	DDX_Check(pDX, IDC_TOOL_OBJECT_NODEBOUND, m_bShowNodeBound);
	DDX_Radio(pDX, IDC_TOOL_OBJECT_AXIS_X, m_nAxis);
	DDX_Text(pDX, IDC_TOOL_OBJECT_COUNT, m_nCount);
	DDX_Text(pDX, IDC_TOOL_OBJECT_REFERENCE, m_nReferenceCount);
	DDX_Check(pDX, IDC_CHECK_LOCALAXIS, m_bLocalAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolObjEditPage, CPageCtrl)
	//{{AFX_MSG_MAP(CToolObjEditPage)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_INSERT, OnToolObjectInsert)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_MODIFY, OnToolObjectModify)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_REMOVE, OnToolObjectRemove)
	ON_LBN_SELCHANGE(IDC_TOOL_OBJECT_LISTBOX, OnSelchangeToolObjectListbox)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_OPENLOLO, OnToolObjectOpenlolo)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_SELECT, OnToolObjectSelect)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_PLACE, OnToolObjectPlace)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_MOVE, OnToolObjectMove)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_ROTATE, OnToolObjectRotate)
	ON_EN_KILLFOCUS(IDC_TOOL_OBJECT_X_EDIT, OnKillfocusToolObjectXEdit)
	ON_EN_KILLFOCUS(IDC_TOOL_OBJECT_Y_EDIT, OnKillfocusToolObjectYEdit)
	ON_EN_KILLFOCUS(IDC_TOOL_OBJECT_Z_EDIT, OnKillfocusToolObjectZEdit)
	ON_EN_KILLFOCUS(IDC_TOOL_OBJECT_ROTATE_EDIT, OnKillfocusToolObjectRotateEdit)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_NODEBOUND, OnToolObjectNodebound)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_RANDOM, OnToolObjectRandom)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_ALIGN, OnToolObjectAlign)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_ARRAY, OnToolObjectArray)
	ON_BN_CLICKED(IDC_TOOL_OBJECT_IMPORT, OnToolObjectImport)
	ON_BN_CLICKED(IDC_CHECK_LOCALAXIS, OnCheckLocalaxis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolObjEditPage message handlers

BOOL CToolObjEditPage::Create( CWnd *pParent )
{
	if( !CPageCtrl::Create( CToolObjEditPage::IDD, pParent ) )
		return FALSE;

	return TRUE;
}


// [2007/11/5 theodoric] 바닥 속성 추가
TCHAR	szTypes[13][15] =
{
	_T("집(들어감)"),	_T("건물(못들어감)"),	_T("나무"),		_T("탁자"),			_T("의자"),
	_T("기타"),			_T("이벤트1"),			_T("이벤트2"),	_T("샾"),			_T("날아다님"),		
	_T(""),				_T("별"),				_T("바닥")
};


BOOL CToolObjEditPage::OnInitDialog() 
{
	CPageCtrl::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlType.AddString( szTypes[0] );
	m_ctrlType.AddString( szTypes[1] );
	m_ctrlType.AddString( szTypes[2] );
	m_ctrlType.AddString( szTypes[3] );
	m_ctrlType.AddString( szTypes[4] );
	m_ctrlType.AddString( szTypes[5] );
	m_ctrlType.AddString( szTypes[6] );
	m_ctrlType.AddString( szTypes[7] );
	m_ctrlType.AddString( szTypes[8] );
	m_ctrlType.AddString( szTypes[9] );
	m_ctrlType.AddString( szTypes[10] );
	m_ctrlType.AddString( szTypes[11] );
	m_ctrlType.AddString( szTypes[12] ); // [2007/11/5 theodoric] 바닥 속성 추가
	m_ctrlType.SetCurSel( 0 );

	m_bmSelect.LoadBitmap( IDB_TOOL_SELECT );	
	m_bmPlace.LoadBitmap( IDB_TOOL_PLACE );	
	m_bmMove.LoadBitmap( IDB_TOOL_MOVE );	
	m_bmRotate.LoadBitmap( IDB_TOOL_ROTATE );	
	m_bmSelect_Check.LoadBitmap( IDB_TOOL_SELECT_ON );
	m_bmPlace_Check.LoadBitmap( IDB_TOOL_PLACE_ON );
	m_bmMove_Check.LoadBitmap( IDB_TOOL_MOVE_ON );
	m_bmRotate_Check.LoadBitmap( IDB_TOOL_ROTATE_ON );
	m_bmAlign.LoadBitmap( IDB_TOOL_ALIGN );
	m_bmArray.LoadBitmap( IDB_TOOL_ARRAY );

	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_SELECT ))->SetBitmap( (HBITMAP)m_bmSelect_Check );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_PLACE ))->SetBitmap( (HBITMAP)m_bmPlace );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_MOVE ))->SetBitmap( (HBITMAP)m_bmMove );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_ROTATE ))->SetBitmap( (HBITMAP)m_bmRotate );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_ALIGN ))->SetBitmap( (HBITMAP)m_bmAlign );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_ARRAY ))->SetBitmap( (HBITMAP)m_bmArray );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CToolObjEditPage::UpdateControls()
{
	if( g_pMyNKApp->GetWorkingFilename() )
		OnSelchangeToolObjectListbox();

	if( g_pMyNKApp->GetGridSize() >= 0 )
		g_pMyNKApp->SetGridSize( 2 );

	g_pMyNKApp->GetTerrain()->SetViewLayers( 7 );
}


void CToolObjEditPage::AddAllObjects()
{
	m_ctrlObjList.ResetContent();

	CString	strNick;
	if( g_pDecoObjSet )
	{
		for( int i = 0; i < g_pDecoObjSet->m_nLoloCount; i++ )
		{
			strNick.Format( _T("%3d : %s"), i, g_pDecoObjSet->m_vecNickName[i] );
			m_ctrlObjList.AddString( strNick );
		}
	}
}


void CToolObjEditPage::RemoveAllObjects()
{
	m_ctrlObjList.ResetContent();
}


void CToolObjEditPage::OnSelchangeToolObjectListbox() 
{
	// TODO: Add your control notification handler code here
	m_nIndex = m_ctrlObjList.GetCurSel();
	if( m_nIndex > 0 )
	{
		m_strName = g_pDecoObjSet->m_vecNickName[m_nIndex];
		m_strLolo = g_pDecoObjSet->m_vecFileName[m_nIndex];
		m_strType = szTypes[g_pDecoObjSet->m_vecObjType[m_nIndex]];
		m_ctrlType.SetCurSel( m_nIndex );
		m_nReferenceCount = g_pDecoObjSet->m_vecRefCount[m_nIndex];
	}
	else
	{
		m_strName = _T("");
		m_strLolo = _T("");
		m_strType = _T("");
		m_nReferenceCount = 0;
	}

	UpdateData( FALSE );
	g_pMyNKApp->SetCurDecoObj( m_nIndex );
}


void CToolObjEditPage::OnToolObjectOpenlolo() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	TCHAR szFilter[] = _T("Lolo Files (*.lol)|*.lol||");
	CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

	CString	strInitDir;
	strInitDir.Format( _T("%sdata\\building\\%s\\lolos\\"), g_pMyNKApp->GetWorkingPath(), 
						g_pMyNKApp->GetTerrain()->GetTerrainName() );
	FileDlg.m_ofn.lpstrInitialDir = strInitDir;

	if( FileDlg.DoModal() == IDOK )
	{
		m_strLolo = FileDlg.GetFileName();
		UpdateData( FALSE );
	}

	// Restore a current directory
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
}


void CToolObjEditPage::OnToolObjectInsert() 
{
	// TODO: Add your control notification handler code here
	if( m_ctrlObjList.GetCount() > 500 )
	{
		MessageBox( _T("허용되는 Object의 개수는 500개입니다."), _T("NKMapTool"), MB_OK | MB_ICONWARNING );
		return;
	}

	UpdateData( TRUE );

	if( m_strName.IsEmpty() )
	{
		MessageBox( _T("Object의 이름을 입력해야 합니다."), _T("NKMapTool"), MB_OK | MB_ICONWARNING );
		return;
	}
	if( m_strLolo.IsEmpty() )
	{
		MessageBox( _T("Lolo 파일의 이름을 입력해야 합니다."), _T("NKMapTool"), MB_OK | MB_ICONWARNING );
		return;
	}

	// Find a lolo file
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
	FILE*	fp = NULL;
	CString	strLoloPath;
	strLoloPath.Format( _T("data\\building\\%s\\lolos\\%s"),
						g_pMyNKApp->GetTerrain()->GetTerrainName(),	m_strLolo );
	fp = fopen( strLoloPath, "rb" );
	if( fp == NULL )
	{
		CString	strError;
		strError.Format( _T("'%s' 파일을 찾을 수 없습니다."), m_strLolo );
		MessageBox( strError, _T("NKMapTool"), MB_OK | MB_ICONWARNING );
		return;
	}

	int nOldTexIndex = g_pMyNKApp->GetDecoTexture()->m_Idx;

	// Insert a decoobj
	if( m_nIndex < 0 || m_nIndex >= m_ctrlObjList.GetCount() - 1 )
	{
		m_nIndex = m_ctrlObjList.GetCount();

		TCHAR*	szNickName = new char[m_strName.GetLength() + 1];
		_tcscpy( szNickName, m_strName );
		szNickName[m_strName.GetLength()] = NULL;

		TCHAR*	szFileName = new char[m_strLolo.GetLength() + 1];
		_tcscpy( szFileName, m_strLolo );
		szFileName[m_strLolo.GetLength()] = NULL;

		CLolos*	pLolos = new CLolos( g_pMyNKApp->GetDecoTexture() );
		pLolos->Read( fp, TRUE );

		g_pDecoObjSet->AddDecoObj( m_nIndex, m_ctrlType.GetCurSel(), szNickName, szFileName, pLolos );

		CString	strNick;
		strNick.Format( _T("%3d : %s"), m_nIndex, szNickName );
		m_ctrlObjList.AddString( strNick );
		m_ctrlObjList.SetCurSel( m_nIndex );
		OnSelchangeToolObjectListbox();
	}
	else
	{
		TCHAR*	szNickName = new char[m_strName.GetLength() + 1];
		_tcscpy( szNickName, m_strName );
		szNickName[m_strName.GetLength()] = NULL;

		TCHAR*	szFileName = new char[m_strLolo.GetLength() + 1];
		_tcscpy( szFileName, m_strLolo );
		szFileName[m_strLolo.GetLength()] = NULL;
		
		CLolos*	pLolos = new CLolos( g_pMyNKApp->GetDecoTexture() );
		pLolos->Read( fp, TRUE );

		g_pDecoObjSet->InsertDecoObj( m_nIndex + 1, m_ctrlType.GetCurSel(), szNickName, szFileName, pLolos );
		g_pObjTree->ModifyDecoObjLoloID( m_nIndex + 1, 1 );

		AddAllObjects();
		m_ctrlObjList.SetCurSel( m_nIndex + 1 );
		OnSelchangeToolObjectListbox();
	}

	fclose( fp );

	int nTexIndex = g_pMyNKApp->GetDecoTexture()->m_Idx;
	// Restore decoobj textures
	if( nOldTexIndex != nTexIndex )
	{
		for( int i = nOldTexIndex; i < nTexIndex; i++ )
			g_pMyNKApp->GetDecoTexture()->RestoreByIdx( i, g_pMyNKApp->GetD3DDevice() );
	}

	g_pMyNKApp->SetModify( 1, TRUE );
}


void CToolObjEditPage::OnToolObjectModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	// Can't modify a index of a decoobj
	if( m_nIndex != m_ctrlObjList.GetCurSel() )
	{
		MessageBox( _T("Index는 변경할 수 없습니다."), _T("NKMapTool"), MB_OK | MB_ICONWARNING );
		OnSelchangeToolObjectListbox();
		return;
	}

	if( m_nIndex >= m_ctrlObjList.GetCount() )
	{
		MessageBox( _T("존재하지 않는 Index입니다."), _T("NKMapTool"), MB_OK );
		OnSelchangeToolObjectListbox();
		return ;
	}

	// Find a lolo file
	SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
	FILE*	fp = NULL;
	CString	strLoloPath;
	strLoloPath.Format( _T("data\\building\\%s\\lolos\\%s"),
						g_pMyNKApp->GetTerrain()->GetTerrainName(),	m_strLolo );
	fp = fopen( strLoloPath, "rb" );
	if( fp == NULL )
	{
		CString	strError;
		strError.Format( _T("'%s' 파일을 찾을 수 없습니다."), m_strLolo );
		MessageBox( strError, _T("NKMapTool"), MB_OK | MB_ICONWARNING );
		OnSelchangeToolObjectListbox();
		return;
	}

	// ObjType
	g_pDecoObjSet->m_vecObjType[m_nIndex] = m_ctrlType.GetCurSel();
	
	// NickName
	if( m_strName.Compare( g_pDecoObjSet->m_vecNickName[m_nIndex] ) != 0 )
	{
		SAFE_DELETE_ARRAY( g_pDecoObjSet->m_vecNickName[m_nIndex] );
		g_pDecoObjSet->m_vecNickName[m_nIndex] = new char[m_strName.GetLength() + 1];
		_tcscpy( g_pDecoObjSet->m_vecNickName[m_nIndex], m_strName );
		g_pDecoObjSet->m_vecNickName[m_nIndex][m_strName.GetLength()] = NULL;
		m_ctrlObjList.DeleteString( m_nIndex );
		CString	strNick;
		strNick.Format( _T("%3d : %s"), m_nIndex, m_strName );
		m_ctrlObjList.InsertString( m_nIndex, strNick );
		m_ctrlObjList.SetCurSel( m_nIndex );
	}

	int nOldTexIndex = g_pMyNKApp->GetDecoTexture()->m_Idx;

	// FileName & Lolos
	if( m_strLolo.Compare( g_pDecoObjSet->m_vecFileName[m_nIndex] ) != 0 )
	{
		SAFE_DELETE_ARRAY( g_pDecoObjSet->m_vecFileName[m_nIndex] );
		g_pDecoObjSet->m_vecFileName[m_nIndex] = new char[m_strLolo.GetLength() + 1];
		_tcscpy( g_pDecoObjSet->m_vecFileName[m_nIndex], m_strLolo );
		g_pDecoObjSet->m_vecFileName[m_nIndex][m_strLolo.GetLength()] = NULL;

		int nOldTexIndex = g_pMyNKApp->GetDecoTexture()->m_Idx;

		SAFE_DELETE( g_pDecoObjSet->m_vecLoloSet[m_nIndex] );
		CLolos*	pLolos = new CLolos( g_pMyNKApp->GetDecoTexture() );
		pLolos->Read( fp, TRUE );
		g_pDecoObjSet->m_vecLoloSet[m_nIndex] = pLolos;

		int nTexIndex = g_pMyNKApp->GetDecoTexture()->m_Idx;
		// Restore decoobj textures
		if( nOldTexIndex != nTexIndex )
		{
			for( int i = nOldTexIndex; i < nTexIndex; i++ )
				g_pMyNKApp->GetDecoTexture()->RestoreByIdx( i, g_pMyNKApp->GetD3DDevice() );
		}

		// Update object quadtree
		g_pObjTree->ModifyDecoObjLolo( m_nIndex );

		g_pMyNKApp->SetModify( 2, TRUE );
	}

	fclose( fp );

	int nTexIndex = g_pMyNKApp->GetDecoTexture()->m_Idx;
	// Restore decoobj textures
	if( nOldTexIndex != nTexIndex )
	{
		for( int i = nOldTexIndex; i < nTexIndex; i++ )
			g_pMyNKApp->GetDecoTexture()->RestoreByIdx( i, g_pMyNKApp->GetD3DDevice() );
	}
	
	OnSelchangeToolObjectListbox();

	g_pMyNKApp->MakeDecoBoxMesh();
	g_pMyNKApp->SetModify( 1, TRUE );
}


void CToolObjEditPage::OnToolObjectRemove() 
{
	// TODO: Add your control notification handler code here
	int	nIndex = m_ctrlObjList.GetCurSel();
	if( nIndex < 0 )
		return ;

	if( g_pDecoObjSet->m_vecRefCount[nIndex] > 0 )
	{
		if( MessageBox( _T("사용중인 Object입니다.\n삭제하시겠습니까?"),
						_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDYES )
		{
			g_pMyNKApp->RemoveDecoObjsFromSelList( nIndex );
			g_pDecoObjSet->RemoveDecoObj( nIndex );
			g_pObjTree->RemoveDecoObjs( nIndex );
			g_pObjTree->ModifyDecoObjLoloID( nIndex, -1 );
			AddAllObjects();
		}
	}
	else
	{
		if( MessageBox( _T("정말로 삭제하시겠습니까?"),
						_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDYES )
		{
			g_pDecoObjSet->RemoveDecoObj( nIndex );
			g_pObjTree->ModifyDecoObjLoloID( nIndex, -1 );
			AddAllObjects();
		}
	}

	if( nIndex >= m_ctrlObjList.GetCount() )
		nIndex = m_ctrlObjList.GetCount() - 1;
	else if( m_ctrlObjList.GetCount() <= 0 )
		nIndex = -1;
	m_ctrlObjList.SetCurSel( nIndex );
	g_pMyNKApp->SetCurDecoObj( nIndex );
	OnSelchangeToolObjectListbox();

	g_pMyNKApp->SetModify( 1, TRUE );
	g_pMyNKApp->SetModify( 2, TRUE );
}


void CToolObjEditPage::OnToolObjectImport() 
{
	// TODO: Add your control notification handler code here
	g_pMyNKApp->ImportBldLst();
	g_pMyNKApp->SetCurDecoObj( -1 );
	g_pMyNKApp->SetModify( 1, TRUE );
	g_pMyNKApp->SetModify( 2, TRUE );
	AddAllObjects();
	m_ctrlObjList.SetCurSel( -1 );	
	OnSelchangeToolObjectListbox();
}


void CToolObjEditPage::OnToolObjectSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetObjEditType( m_nWorkType );

	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_SELECT ))->SetBitmap( (HBITMAP)m_bmSelect_Check );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_PLACE ))->SetBitmap( (HBITMAP)m_bmPlace );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_MOVE ))->SetBitmap( (HBITMAP)m_bmMove );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_ROTATE ))->SetBitmap( (HBITMAP)m_bmRotate );
}


void CToolObjEditPage::OnToolObjectPlace() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetObjEditType( m_nWorkType );

	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_SELECT ))->SetBitmap( (HBITMAP)m_bmSelect );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_PLACE ))->SetBitmap( (HBITMAP)m_bmPlace_Check );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_MOVE ))->SetBitmap( (HBITMAP)m_bmMove );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_ROTATE ))->SetBitmap( (HBITMAP)m_bmRotate );
}


void CToolObjEditPage::OnToolObjectMove() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetObjEditType( m_nWorkType );

	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_SELECT ))->SetBitmap( (HBITMAP)m_bmSelect );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_PLACE ))->SetBitmap( (HBITMAP)m_bmPlace );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_MOVE ))->SetBitmap( (HBITMAP)m_bmMove_Check );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_ROTATE ))->SetBitmap( (HBITMAP)m_bmRotate );
}


void CToolObjEditPage::OnToolObjectRotate() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetObjEditType( m_nWorkType );

	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_SELECT ))->SetBitmap( (HBITMAP)m_bmSelect );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_PLACE ))->SetBitmap( (HBITMAP)m_bmPlace );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_MOVE ))->SetBitmap( (HBITMAP)m_bmMove );
	((CButton*)GetDlgItem( IDC_TOOL_OBJECT_ROTATE ))->SetBitmap( (HBITMAP)m_bmRotate_Check );
}


void CToolObjEditPage::OnKillfocusToolObjectXEdit() 
{
	// TODO: Add your control notification handler code here
	float	fOldX = m_fX;

	UpdateData( TRUE );

	if( fOldX != m_fX )
	{
		// [2007/10/30 theodoric] 
		g_pMyNKApp->m_UndoObj.OnSelObjListBackup();

		g_pMyNKApp->MoveSelDecoObjs( m_fX - fOldX, 0.0f, 0.0f );
		g_pMyNKApp->UpdateChangedDecoObjs();

		// [2007/10/30 theodoric] 
		g_pMyNKApp->m_UndoObj.OnSelObjListUndoSet();
	}
}


void CToolObjEditPage::OnKillfocusToolObjectYEdit() 
{
	// TODO: Add your control notification handler code here
	float	fOldY = m_fY;

	UpdateData( TRUE );

	if( fOldY != m_fY )
	{
		// [2007/10/30 theodoric] 
		g_pMyNKApp->m_UndoObj.OnSelObjListBackup();

		g_pMyNKApp->MoveSelDecoObjs( 0.0f, m_fY - fOldY, 0.0f );
		g_pMyNKApp->UpdateChangedDecoObjs();

		// [2007/10/30 theodoric] 
		g_pMyNKApp->m_UndoObj.OnSelObjListUndoSet();
	}
}


void CToolObjEditPage::OnKillfocusToolObjectZEdit() 
{
	// TODO: Add your control notification handler code here
	float	fOldZ = m_fZ;

	UpdateData( TRUE );

	if( fOldZ != m_fZ )
	{
		// [2007/10/30 theodoric] 
		g_pMyNKApp->m_UndoObj.OnSelObjListBackup();

		g_pMyNKApp->MoveSelDecoObjs( 0.0f, 0.0f, m_fZ - fOldZ );
		g_pMyNKApp->UpdateChangedDecoObjs();

		// [2007/10/30 theodoric] 
		g_pMyNKApp->m_UndoObj.OnSelObjListUndoSet();
	}
}


void CToolObjEditPage::OnKillfocusToolObjectRotateEdit() 
{
	// TODO: Add your control notification handler code here
	float	fOldRotate = m_fRotate;

	UpdateData( TRUE );

	if( fOldRotate != m_fRotate )
	{
		// [2007/10/30 theodoric] 
		g_pMyNKApp->m_UndoObj.OnSelObjListBackup();

		g_pMyNKApp->RotateSelDecoObjs( m_fRotate - fOldRotate );
		g_pMyNKApp->UpdateChangedDecoObjs();

		// [2007/10/30 theodoric] 
		g_pMyNKApp->m_UndoObj.OnSelObjListUndoSet();
	}
}


void CToolObjEditPage::UpdateParameters( CDecoObj* pDecoObj )
{
	if( pDecoObj == NULL )
	{
		m_fX = 0;
		m_fY = 0;
		m_fZ = 0;
		m_fRotate = 0;
	}
	else
	{
		m_nIndex = pDecoObj->m_nLoloId;
		m_ctrlObjList.SetCurSel( m_nIndex );
		m_strName = g_pDecoObjSet->m_vecNickName[m_nIndex];
		m_strLolo = g_pDecoObjSet->m_vecFileName[m_nIndex];
		m_nReferenceCount = g_pDecoObjSet->m_vecRefCount[m_nIndex];
		m_strType = szTypes[g_pDecoObjSet->m_vecObjType[m_nIndex]];
		m_ctrlType.SetCurSel( g_pDecoObjSet->m_vecObjType[m_nIndex] );
		m_fX = pDecoObj->m_wx;
		m_fY = pDecoObj->m_wy;
		m_fZ = pDecoObj->m_wz;
		m_fRotate = fmodf( pDecoObj->m_dir, NK_TWOPI ) * NK_RADTODEG;
		if( fabsf( m_fRotate ) < NK_EPSILON4 )
			m_fRotate = 0.0f;
	}

	UpdateData( FALSE );
}


void CToolObjEditPage::OnToolObjectNodebound() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetShowNodeBound( m_bShowNodeBound );
}


void CToolObjEditPage::OnToolObjectRandom() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetRandomRotation( m_bRandomRotation );
}


void CToolObjEditPage::OnToolObjectAlign() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->AlignDecoObjs( m_nAxis );
}


void CToolObjEditPage::OnToolObjectArray() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->ArrayDecoObjs( m_nAxis, m_fOffset, m_nCount );
	m_nReferenceCount = g_pDecoObjSet->m_vecRefCount[m_nIndex];
	UpdateData( FALSE );
}


void CToolObjEditPage::UpdateReferenceCount( int nIndex )
{
	if( m_nIndex != nIndex )
		return ;

	m_nReferenceCount = g_pDecoObjSet->m_vecRefCount[m_nIndex];
	UpdateData( FALSE );
}

//////////////////////////////////////////////////////////////////////////////
// 수정자 : 이원석
// 수정일 : 05-02-22
// 수정내용 : 다이얼로그에 포커스가 맞춰져있을 경우 메인 핫키가 먹지않아
//            강제로 Q,W,E,R 의 단축키 적용 처리.
BOOL CToolObjEditPage::PreTranslateMessage( MSG* pMsg ) // 메시지를 받는 함수를 오버로딩.
{ 
	if( pMsg->message == WM_KEYDOWN  )
	{ 
		if( pMsg->wParam == 0x51 ) // Q
		{
			HK_ObjectSelect(); // 핫키는 실제 선택값을 바꿔주는 처리도 해줘야 해서 전용 함수를 따로 만듬.
		}
		else if( pMsg->wParam == 0x57 ) // W
		{
			HK_ToolObjectPlace(); // 핫키는 실제 선택값을 바꿔주는 처리도 해줘야 해서 전용 함수를 따로 만듬.
		}
		else if( pMsg->wParam == 0x45 ) // E
		{
			HK_ToolObjectMove(); // 핫키는 실제 선택값을 바꿔주는 처리도 해줘야 해서 전용 함수를 따로 만듬.
		}
		else if( pMsg->wParam == 0x52 ) // R
		{
			HK_ToolObjectRotate(); // 핫키는 실제 선택값을 바꿔주는 처리도 해줘야 해서 전용 함수를 따로 만듬.
		}
	} 
	if( pMsg->message == WM_CHAR)
	{
		if( (int)pMsg->wParam == 'z' || (int)pMsg->wParam == 'Z')
			g_pMyNKApp->m_UndoObj.OnUndo();
	}

	return CDialog::PreTranslateMessage( pMsg ); 
} 

void CToolObjEditPage::OnCheckLocalaxis() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	g_pMyNKApp->SetShowLocalAxis( m_bLocalAxis );
}
