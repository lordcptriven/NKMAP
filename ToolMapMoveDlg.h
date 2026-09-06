#if !defined(AFX_TOOLMAPMOVEDLG_H__9E9F5CDE_F1AE_4565_9319_ECE05E8F2A3B__INCLUDED_)
#define AFX_TOOLMAPMOVEDLG_H__9E9F5CDE_F1AE_4565_9319_ECE05E8F2A3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolMapMoveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolMapMoveDlg dialog

class CToolMapMoveDlg : public CDialog
{
// Construction
public:
	CToolMapMoveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolMapMoveDlg)
	enum { IDD = IDD_TOOL_MAPMOVE };
	int		m_nMoveUnit;
	int		m_nCoordX;
	int		m_nCoordZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolMapMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolMapMoveDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLMAPMOVEDLG_H__9E9F5CDE_F1AE_4565_9319_ECE05E8F2A3B__INCLUDED_)
