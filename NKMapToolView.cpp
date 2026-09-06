// NKMapToolView.cpp : implementation of the CNKMapToolView class
//

#include "stdafx.h"
#include "NKMapTool.h"

#include "NKMapToolDoc.h"
#include "NKMapToolView.h"

#include "MainFrm.h"
#include "MyNKApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNKMapToolView

IMPLEMENT_DYNCREATE(CNKMapToolView, CView)

BEGIN_MESSAGE_MAP(CNKMapToolView, CView)
	//{{AFX_MSG_MAP(CNKMapToolView)
	ON_COMMAND(ID_HOTKEY_E, OnHotkeyE)
	ON_COMMAND(ID_HOTKEY_Q, OnHotkeyQ)
	ON_COMMAND(ID_HOTKEY_R, OnHotkeyR)
	ON_COMMAND(ID_HOTKEY_W, OnHotkeyW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Global variables
CMyNKApp*	g_pMyNKApp = NULL;


/////////////////////////////////////////////////////////////////////////////
// CNKMapToolView construction/destruction

CNKMapToolView::CNKMapToolView()
{
	// TODO: add construction code here

}

CNKMapToolView::~CNKMapToolView()
{
	SAFE_DELETE( g_pMyNKApp );
}

BOOL CNKMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNKMapToolView drawing

void CNKMapToolView::OnDraw(CDC* pDC)
{
	CNKMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if( g_pMyNKApp )
		g_pMyNKApp->Render3DEnvironment();
}

/////////////////////////////////////////////////////////////////////////////
// CNKMapToolView diagnostics

#ifdef _DEBUG
void CNKMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CNKMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNKMapToolDoc* CNKMapToolView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNKMapToolDoc)));
	return (CNKMapToolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNKMapToolView message handlers

void CNKMapToolView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	// Create MyNKApp instance for D3D rendering
	g_pMyNKApp = new CMyNKApp;
	if( FAILED( g_pMyNKApp->Create( GetSafeHwnd() ) ) )
	{
		SAFE_DELETE( g_pMyNKApp );
		return ;
	}

	TCHAR	szPath[512];
	GetCurrentDirectory( 512, szPath );
	_tcscat( szPath, _T("\\") );
	g_pMyNKApp->SetWorkingPath( szPath );
}


LRESULT CNKMapToolView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( g_pMyNKApp )
		g_pMyNKApp->MsgProc( message, wParam, lParam );

	return CView::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////////
// 수정자 : 이원석
// 수정일 : 05-02-22
// 수정내용 : Q,W,E,R 의 단축키 적용.(차일드 다이얼로그들 버튼 단축키)
void CNKMapToolView::OnHotkeyQ() 
{
	// TODO: Add your command handler code here
	CMainFrame*	pFrame = NULL;
	pFrame = (CMainFrame*)AfxGetMainWnd();

	CToolObjEditPage *RefObjEdit = NULL;
	CToolTexPaintPage *RefTexPaint = NULL;

	if( g_pMyNKApp->GetWorkingState() == OBJECT_EDIT ) // 오브젝트 에디트 다이얼로그 모드 일 경우.
	{
		if( pFrame && pFrame->m_wndToolCtrlBar )
		{
			RefObjEdit = &(pFrame->m_wndToolCtrlBar.m_pageObjEdit);
		}
					
		if( RefObjEdit )
		{
			RefObjEdit->HK_ObjectSelect();
		}
	}
	else if( g_pMyNKApp->GetWorkingState() == TEXTURE_PAINT ) // 텍스쳐 페인트 다이얼로그 모드 일 경우.
	{
		if( pFrame && pFrame->m_wndToolCtrlBar )
		{
			RefTexPaint = &(pFrame->m_wndToolCtrlBar.m_pageTexPaint);
		}
					
		if( RefTexPaint )
		{
			RefTexPaint->HK_PaintTypeP();
		}
	}
}

void CNKMapToolView::OnHotkeyW() 
{
	// TODO: Add your command handler code here
	CMainFrame*	pFrame = NULL;
	pFrame = (CMainFrame*)AfxGetMainWnd();

	CToolObjEditPage *RefObjEdit = NULL;
	CToolTexPaintPage *RefTexPaint = NULL;

	if( g_pMyNKApp->GetWorkingState() == OBJECT_EDIT ) // 오브젝트 에디트 다이얼로그 모드 일 경우.
	{
		if( pFrame && pFrame->m_wndToolCtrlBar )
		{
			RefObjEdit = &(pFrame->m_wndToolCtrlBar.m_pageObjEdit);
		}
					
		if( RefObjEdit )
		{
			RefObjEdit->HK_ToolObjectPlace();
		}
	}
	else if( g_pMyNKApp->GetWorkingState() == TEXTURE_PAINT ) // 텍스쳐 페인트 다이얼로그 모드 일 경우.
	{
		if( pFrame && pFrame->m_wndToolCtrlBar )
		{
			RefTexPaint = &(pFrame->m_wndToolCtrlBar.m_pageTexPaint);
		}
					
		if( RefTexPaint )
		{
			RefTexPaint->HK_PaintTypeE();
		}
	}
}

void CNKMapToolView::OnHotkeyE() 
{
	// TODO: Add your command handler code here
	CMainFrame*	pFrame = NULL;
	pFrame = (CMainFrame*)AfxGetMainWnd();

	CToolObjEditPage *RefObjEdit = NULL;

	if( g_pMyNKApp->GetWorkingState() == OBJECT_EDIT ) // 오브젝트 에디트 다이얼로그 모드 일 경우.
	{
		if( pFrame && pFrame->m_wndToolCtrlBar )
		{
			RefObjEdit = &(pFrame->m_wndToolCtrlBar.m_pageObjEdit);
		}
					
		if( RefObjEdit )
		{
			RefObjEdit->HK_ToolObjectMove();
		}
	}
}

void CNKMapToolView::OnHotkeyR() 
{
	// TODO: Add your command handler code here
	CMainFrame*	pFrame = NULL;
	pFrame = (CMainFrame*)AfxGetMainWnd();

	CToolObjEditPage *RefObjEdit = NULL;

	if( g_pMyNKApp->GetWorkingState() == OBJECT_EDIT ) // 오브젝트 에디트 다이얼로그 모드 일 경우.
	{
		if( pFrame && pFrame->m_wndToolCtrlBar )
		{
			RefObjEdit = &(pFrame->m_wndToolCtrlBar.m_pageObjEdit);
		}
					
		if( RefObjEdit )
		{
			RefObjEdit->HK_ToolObjectRotate();
		}
	}
}
