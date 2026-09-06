// ToolCtrlBar.cpp : implementation file
//

#include "stdafx.h"
#include "NKMapTool.h"
#include "ToolCtrlBar.h"
#include "MainFrm.h"

#include "MyNKApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global variables
extern CMyNKApp*	g_pMyNKApp;


/////////////////////////////////////////////////////////////////////////////
// CToolCtrlBar

CToolCtrlBar::CToolCtrlBar()
{
}

CToolCtrlBar::~CToolCtrlBar()
{
}


BEGIN_MESSAGE_MAP(CToolCtrlBar, CSizingControlBar)
	//{{AFX_MSG_MAP(CToolCtrlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolCtrlBar message handlers

int CToolCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if( !m_pageVertEdit.Create( this ) )
		return -1;

	if( !m_pageTexPaint.Create( this ) )
		return -1;

	if( !m_pageObjEdit.Create( this ) )
		return -1;

	if( !m_pageLightEdit.Create( this ) )
		return -1;

	if( !m_pageAtmosEdit.Create( this ) )
		return -1;

	if( !m_pageAttrEdit.Create( this ) )
		return -1;
	
	return 0;
}


void CToolCtrlBar::UpdatePage()
{
	m_pageVertEdit.ShowWindow( SW_HIDE );
	m_pageTexPaint.ShowWindow( SW_HIDE );
	m_pageObjEdit.ShowWindow( SW_HIDE );
	m_pageLightEdit.ShowWindow( SW_HIDE );
	m_pageAtmosEdit.ShowWindow( SW_HIDE );
	m_pageAttrEdit.ShowWindow( SW_HIDE );

	switch( g_pMyNKApp->GetWorkingState() )
	{
	case VERTEX_EDIT:
		m_pageVertEdit.ShowWindow( SW_SHOW );
		m_pageVertEdit.UpdateControls();
		break;

	case TEXTURE_PAINT:
		m_pageTexPaint.ShowWindow( SW_SHOW );
		m_pageTexPaint.UpdateControls();
		break;

	case OBJECT_EDIT:
		m_pageObjEdit.ShowWindow( SW_SHOW );
		m_pageObjEdit.UpdateControls();
		break;

	case LIGHT_EDIT:
		m_pageLightEdit.ShowWindow( SW_SHOW );
		m_pageLightEdit.UpdateControls();
		break;

	case ATMOSPHERE_EDIT:
		m_pageAtmosEdit.ShowWindow( SW_SHOW );
		m_pageAtmosEdit.UpdateControls();
		break;

	case ATTRIBUTE_EDIT:
		m_pageAttrEdit.ShowWindow( SW_SHOW );
		m_pageAttrEdit.UpdateControls();
		break;
	}
}


void CToolCtrlBar::ResetPage()
{
}


void CToolCtrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CPageCtrl*	pWnd = (CPageCtrl*)GetWindow( GW_CHILD );
	while( pWnd != NULL )
	{
		pWnd->CalcLayout();
		pWnd = (CPageCtrl*)pWnd->GetNextWindow();
	}

	static BOOL	bFloating = IsFloating();
	if( bFloating != IsFloating() )
	{
		CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->SendMessage( WM_SIZE );
		
		bFloating = IsFloating();
	}
}


BOOL CToolCtrlBar::Create( CWnd* pParent )
{
	if( !CSizingControlBar::Create( _T("Tools"), pParent, CSize( 242, 590 ), TRUE, AFX_IDW_TOOLBAR + 10 ) )
		return FALSE;
	SetBarStyle( GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	return TRUE;
}
