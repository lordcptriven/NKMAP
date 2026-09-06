// ToolObjectRemoveRegion.cpp : implementation file
//

#include "stdafx.h"
#include "nkmaptool.h"
#include "ToolObjectRemoveRegion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolObjectRemoveRegion dialog


CToolObjectRemoveRegion::CToolObjectRemoveRegion(CWnd* pParent /*=NULL*/)
	: CDialog(CToolObjectRemoveRegion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolObjectRemoveRegion)
	m_nEndX = 0;
	m_nEndZ = 0;
	m_nStartX = 0;
	m_nStartZ = 0;
	//}}AFX_DATA_INIT
}


void CToolObjectRemoveRegion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolObjectRemoveRegion)
	DDX_Text(pDX, IDC_REMOVE_OBJECTS_EX, m_nEndX);
	DDX_Text(pDX, IDC_REMOVE_OBJECTS_EZ, m_nEndZ);
	DDX_Text(pDX, IDC_REMOVE_OBJECTS_SX, m_nStartX);
	DDX_Text(pDX, IDC_REMOVE_OBJECTS_SZ, m_nStartZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolObjectRemoveRegion, CDialog)
	//{{AFX_MSG_MAP(CToolObjectRemoveRegion)
	ON_BN_CLICKED(IDC_REMOVE_OBJECTS, OnRemoveObjects)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolObjectRemoveRegion message handlers

void CToolObjectRemoveRegion::OnRemoveObjects() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CDialog::OnOK();
}
