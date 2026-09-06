// NKMapToolDoc.cpp : implementation of the CNKMapToolDoc class
//

#include "stdafx.h"
#include "NKMapTool.h"

#include "NKMapToolDoc.h"
#include "MainFrm.h"
#include "NKMapToolView.h"
#include "MyNKApp.h"
#include "FileNewTerrainDlg.h"
#include "MyFileDialog.h"
#include "FileMergeMaps.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern void DeleteDecoObjData();


/////////////////////////////////////////////////////////////////////////////
// CNKMapToolDoc

IMPLEMENT_DYNCREATE(CNKMapToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CNKMapToolDoc, CDocument)
	//{{AFX_MSG_MAP(CNKMapToolDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN_GAME_MAP, OnFileOpenGameMap)
	ON_COMMAND(ID_FILE_SAVE_GAME_MAP, OnFileSaveGameMap)
	ON_COMMAND(ID_FILE_MERGE_MAP, OnFileMergeMap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Global variables
extern CMyNKApp*	g_pMyNKApp;


/////////////////////////////////////////////////////////////////////////////
// CNKMapToolDoc construction/destruction

CNKMapToolDoc::CNKMapToolDoc()
{
	// TODO: add one-time construction code here

}

CNKMapToolDoc::~CNKMapToolDoc()
{
}

BOOL CNKMapToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNKMapToolDoc serialization

void CNKMapToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNKMapToolDoc diagnostics

#ifdef _DEBUG
void CNKMapToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNKMapToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNKMapToolDoc commands

void CNKMapToolDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() &&
			( g_pMyNKApp->GetModify( 0 ) || g_pMyNKApp->GetModify( 1 ) || g_pMyNKApp->GetModify( 2 ) ) )
		{
			if( MessageBox( NULL, _T("변경된 내용을 저장하시겠습니까?"),
							_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDYES )
			{
				OnFileSave();
			}		
		}

		CFileNewTerrainDlg dlg;
		if(dlg.DoModal() == IDOK)
		{
			// Refresh the client screen
			POSITION	pos = GetFirstViewPosition();
			CNKMapToolView*	pView = (CNKMapToolView*)GetNextView( pos );
			pView->UpdateWindow();

			// Create a progress dialog
			g_pMyNKApp->InitProgress();

			// Initialize a terrain
			g_pMyNKApp->GetTerrain()->Destroy();

			// Set a name of the terrain
			g_pMyNKApp->GetTerrain()->SetTerrainName( dlg.m_strName );

			// Restore a current directory
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );

			// Load a texture
			if( !dlg.m_strTexture.IsEmpty() )
			{
				if( g_pMyNKApp->GetTerrain()->AddTexture( dlg.m_strTexture ) < 3 )
				{
					g_pMyNKApp->DeleteWorkingFilename();
					return ;
				}
			}			
			
			// Create a terrain
			int nWHSegment =  _ttoi( dlg.m_strWHSegment );
			if( g_pMyNKApp->NewTerrain( nWHSegment, dlg.m_nPatchSize, dlg.m_nHeightScale,
										dlg.m_strHeightMap ) == FALSE )
			{
				MessageBox( NULL, _T("새로운 지형 생성에 실패했습니다."),
							_T("NKMapTool"), MB_ICONWARNING | MB_OK );
				return ;
			}
			
			// Add a texture to a list box of a texture painting tool
			CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
			pFrame->m_wndToolCtrlBar.m_pageTexPaint.RemoveAllTextures();
			if( dlg.m_strTexture.IsEmpty() )
				g_pMyNKApp->SetGridSize( 0 );
			else
			{
				g_pMyNKApp->SetGridSize( -1 );
				pFrame->m_wndToolCtrlBar.m_pageTexPaint.AddTexture( dlg.m_strTexture );
			}

			// Set a visible state of layers
			pFrame->m_wndToolCtrlBar.m_pageTexPaint.ShowAllLayers();

			// Reset a object editing tool
			pFrame->m_wndToolCtrlBar.m_pageObjEdit.UpdateControls();
			pFrame->m_wndToolCtrlBar.m_pageObjEdit.AddAllObjects();

			// Update a terrain information
			pFrame->m_wndToolCtrlBar.m_pageVertEdit.UpdateTerrainInfo();

			// Store a working filename
			CString strFilename;
			strFilename = dlg.m_strName;
			g_pMyNKApp->SetWorkingFilename( strFilename );

			// Set a window title
			CString	strTitle = strFilename + _T(" - NKMapTool");
			pFrame->SetWindowText( strTitle );

			// Destroy a progress dialog
			g_pMyNKApp->DestroyProgress();
		}
	}
}


void CNKMapToolDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() &&
			( g_pMyNKApp->GetModify( 0 ) || g_pMyNKApp->GetModify( 1 ) || g_pMyNKApp->GetModify( 2 ) ) )
		{
			if( MessageBox( NULL, _T("변경된 내용을 저장하시겠습니까?"),
							_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDYES )
			{
				OnFileSave();
			}
		}

		TCHAR szFilter[] = _T("NKMapTool Files (*.nmf)|*.nmf||");
		CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

		CString	strInitDir;
		strInitDir.Format( _T("%smap\\"), g_pMyNKApp->GetWorkingPath() );
		FileDlg.m_ofn.lpstrInitialDir = strInitDir;
		
		if( FileDlg.DoModal() == IDOK )
		{
			// Refresh the client screen
			POSITION	pos = GetFirstViewPosition();
			CNKMapToolView*	pView = (CNKMapToolView*)GetNextView( pos );
			pView->UpdateWindow();

			// Create a progress dialog
			g_pMyNKApp->InitProgress();
			
			// Destroy a terrain
			g_pMyNKApp->GetTerrain()->Destroy();
			g_pMyNKApp->DeleteWorkingFilename();
			
			// Get a directory name
			CString	strDir, strTemp;
			int		nFind;
			strDir = FileDlg.GetPathName();
			nFind = strDir.ReverseFind( '\\' );
			strTemp = strDir.Left( nFind );
			nFind = strTemp.ReverseFind( '\\' );
			strDir = strTemp.Right( strTemp.GetLength() - nFind - 1 );

			// Load a terrain
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );			
			if( g_pMyNKApp->LoadNmf( strDir, FileDlg.GetFileTitle() ) )
			{
				g_pMyNKApp->SetWorkingFilename( FileDlg.GetFileTitle() );

				// Set a window title
				CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
				CString	strTitle = FileDlg.GetFileTitle() + _T(" - NKMapTool");
				pFrame->SetWindowText( strTitle );

				// Set a state of layers
				pFrame->m_wndToolCtrlBar.m_pageTexPaint.ShowAllLayers();

				// Update a terrain information
				pFrame->m_wndToolCtrlBar.m_pageVertEdit.UpdateTerrainInfo();

				// Reset a object editor tool
				pFrame->m_wndToolCtrlBar.m_pageObjEdit.AddAllObjects();
				pFrame->m_wndToolCtrlBar.m_pageObjEdit.UpdateControls();
			}

			// Destroy a progress dialog
			g_pMyNKApp->DestroyProgress();
		}

		// Restore a current directory
		SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
	}
}


void CNKMapToolDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() )
			g_pMyNKApp->SaveNmf( g_pMyNKApp->GetWorkingFilename() );
		else
			MessageBox( NULL, _T("작업중인 내용이 없습니다."), _T("NKMapTool"), MB_OK );
	}
}


void CNKMapToolDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() )
		{
			TCHAR szFilter[] = _T("NKMapTool Files (*.nmf)|*.nmf||");
			CMyFileDialog	FileDlg( FALSE, _T("nmf"), g_pMyNKApp->GetWorkingFilename(), OFN_HIDEREADONLY |
									OFN_OVERWRITEPROMPT, szFilter, NULL );

			CString	strInitDir;
			strInitDir.Format( _T("%smap\\%s"), g_pMyNKApp->GetWorkingPath(),
								g_pMyNKApp->GetTerrain()->GetTerrainName() );
			FileDlg.m_ofn.lpstrInitialDir = strInitDir;

			if( FileDlg.DoModal() == IDOK )
			{
				g_pMyNKApp->SetModify( 0, TRUE );
				g_pMyNKApp->SetModify( 1, TRUE );
				g_pMyNKApp->SetModify( 2, TRUE );
				SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
				if( g_pMyNKApp->SaveNmf( FileDlg.GetFileTitle() ) )
				{
					g_pMyNKApp->SetWorkingFilename( FileDlg.GetFileTitle() );

					// Set a window title
					CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
					CString	strTitle = FileDlg.GetFileTitle() + _T(" - NKMapTool");
					pFrame->SetWindowText( strTitle );
				}
			}

			// Restore a current directory
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
		}
		else
		{
			MessageBox( NULL, _T("작업중인 내용이 없습니다."), _T("NKMapTool"), MB_OK );
		}
	}
}


void CNKMapToolDoc::OnFileClose() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() &&
			( g_pMyNKApp->GetModify( 0 ) || g_pMyNKApp->GetModify( 1 ) || g_pMyNKApp->GetModify( 2 ) ) )
		{
			if( MessageBox( NULL, _T("변경된 내용을 저장하시겠습니까?"),
							_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDYES )
			{
				OnFileSave();
			}
		}

		// Reset a working state and a tools
		g_pMyNKApp->SetWorkingState( NONE );

		// Delete decoobjs
		DeleteDecoObjData();

		// Destory a terrain
		g_pMyNKApp->GetTerrain()->Destroy();
		g_pMyNKApp->DeleteWorkingFilename();

		// Reset background color
		if( g_pMyNKApp->GetShowSky() )
			g_pMyNKApp->ToggleShowSky();

		// Remove textures in a list box of a texture painting tool
		CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->m_wndToolCtrlBar.m_pageTexPaint.RemoveAllTextures();

		// Reset a object editing tool
		pFrame->m_wndToolCtrlBar.m_pageObjEdit.UpdateControls();

		// Reset a tools
		pFrame->ShowControlBar( &pFrame->m_wndToolCtrlBar, FALSE, FALSE );

		// Set a window title
		CString	strTitle = _T("제목없음 - NKMapTool");
		pFrame->SetWindowText( strTitle );
	}
}


void CNKMapToolDoc::OnFileOpenGameMap() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() &&
			( g_pMyNKApp->GetModify( 0 ) || g_pMyNKApp->GetModify( 1 ) || g_pMyNKApp->GetModify( 2 ) ) )
		{
			if( MessageBox( NULL, _T("변경된 내용을 저장하시겠습니까?"),
							_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDYES )
			{
				OnFileSave();
			}
		}

		TCHAR szFilter[] = _T("Game Map Files (*.map)|*.map||");
		CMyFileDialog	FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );

		CString	strInitDir;
		strInitDir.Format( _T("%smap\\"), g_pMyNKApp->GetWorkingPath() );
		FileDlg.m_ofn.lpstrInitialDir = strInitDir;
		
		if( FileDlg.DoModal() == IDOK )
		{
			// Destroy a terrain
			g_pMyNKApp->GetTerrain()->Destroy();
			g_pMyNKApp->DeleteWorkingFilename();
			
			// Get a directory name
			CString	strDir, strTemp;
			int		nFind;
			strDir = FileDlg.GetPathName();
			nFind = strDir.ReverseFind( '\\' );
			strTemp = strDir.Left( nFind );
			nFind = strTemp.ReverseFind( '\\' );
			strDir = strTemp.Right( strTemp.GetLength() - nFind - 1 );

			// Set a name of the terrain
			g_pMyNKApp->GetTerrain()->SetTerrainName( strDir );

			// Load a terrain
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );			
			if( g_pMyNKApp->LoadMap( strDir, FileDlg.GetFileTitle() ) )
			{
				g_pMyNKApp->SetWorkingFilename( FileDlg.GetFileTitle() );

				// Set a window title
				CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
				CString	strTitle = FileDlg.GetFileTitle() + _T(" - NKMapTool");
				pFrame->SetWindowText( strTitle );
			}
		}

		// Restore a current directory
		SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
	}
}


void CNKMapToolDoc::OnFileSaveGameMap() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() )
		{
			if( !g_pMyNKApp->IsLoadedGameMap() &&
				( g_pMyNKApp->GetModify( 0 ) || g_pMyNKApp->GetModify( 1 ) || g_pMyNKApp->GetModify( 2 ) ) )
			{
				if( MessageBox( NULL, _T("Map 파일로 저장하면 작업중인 내용이 변경되므로\n")
									  _T("먼저 Nmf 파일로 저장하기를 추천합니다.\n")
									  _T("작업중인 내용을 Nmf 파일로 저장하시겠습니까?"),
								_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDYES )
				{
					OnFileSaveAs();
				}
			}

			TCHAR szFilter[] = _T("NKMapTool Files (*.map)|*.map||");
			CMyFileDialog	FileDlg( FALSE, _T("map"), g_pMyNKApp->GetWorkingFilename(), OFN_HIDEREADONLY |
									OFN_OVERWRITEPROMPT, szFilter, NULL );

			CString	strInitDir;
			strInitDir.Format( _T("%smap\\%s"), g_pMyNKApp->GetWorkingPath(),
								g_pMyNKApp->GetTerrain()->GetTerrainName() );
			FileDlg.m_ofn.lpstrInitialDir = strInitDir;

			if( FileDlg.DoModal() == IDOK )
			{
				SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
				if( g_pMyNKApp->SaveMap( FileDlg.GetFileTitle() ) )
				{
					g_pMyNKApp->SetWorkingFilename( FileDlg.GetFileTitle() );

					// Set a window title
					CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
					CString	strTitle = FileDlg.GetFileTitle() + _T(" - NKMapTool");
					pFrame->SetWindowText( strTitle );
				}
			}

			// Restore a current directory
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );
		}
		else
		{
			MessageBox( NULL, _T("작업중인 내용이 없습니다."), _T("NKMapTool"), MB_OK );
		}
	}
}


void CNKMapToolDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( g_pMyNKApp )
	{
		if( g_pMyNKApp->GetWorkingFilename() )
		{
			if( g_pMyNKApp->GetModify( 0 ) || g_pMyNKApp->GetModify( 1 ) || g_pMyNKApp->GetModify( 2 ) )
			{
				if( MessageBox( NULL, _T("작업중인 내용이 존재합니다.\n")
									  _T("작업중인 내용을 저장하시겠습니까?"),
								_T("NKMapTool"), MB_ICONWARNING | MB_YESNO ) == IDYES )
				{
					if( g_pMyNKApp->IsLoadedGameMap() )
						OnFileSaveGameMap();
					else
						OnFileSaveAs();
				}				
			}
		}
	}
	
	CDocument::OnCloseDocument();
}


void CNKMapToolDoc::OnFileMergeMap() 
{
	// TODO: Add your command handler code here
	if( g_pMyNKApp )
	{
		if( !g_pMyNKApp->GetWorkingFilename() )
			return ;

		CFileMergeMaps	dlg;
		if(dlg.DoModal() == IDOK)
		{
			// Restore a current directory
			SetCurrentDirectory( g_pMyNKApp->GetWorkingPath() );

			if( !g_pMyNKApp->MergeMaps( dlg.m_strNmfName, dlg.m_strNotName,
										dlg.m_nMergeSZ, dlg.m_nMergeSX, dlg.m_nMergeEZ, dlg.m_nMergeEX ) )
			{
				g_pMyNKApp->SetModify( 0, FALSE );
				g_pMyNKApp->SetModify( 1, FALSE );
				g_pMyNKApp->SetModify( 2, FALSE );

				OnFileClose();
			}
		}		
	}
}
