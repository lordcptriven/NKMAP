// ToolMapMoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nkmaptool.h"
#include "ToolMapMoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolMapMoveDlg dialog


CToolMapMoveDlg::CToolMapMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolMapMoveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolMapMoveDlg)
	m_nMoveUnit = 0;
	m_nCoordX = 0;
	m_nCoordZ = 0;
	//}}AFX_DATA_INIT
}


void CToolMapMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolMapMoveDlg)
	DDX_Radio(pDX, IDC_TOOL_MAPMOVE_TILE, m_nMoveUnit);
	DDX_Text(pDX, IDC_TOOL_MAPMOVE_X, m_nCoordX);
	DDX_Text(pDX, IDC_TOOL_MAPMOVE_Z, m_nCoordZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolMapMoveDlg, CDialog)
	//{{AFX_MSG_MAP(CToolMapMoveDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolMapMoveDlg message handlers
