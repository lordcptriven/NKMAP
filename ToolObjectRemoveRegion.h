#if !defined(AFX_TOOLOBJECTREMOVEREGION_H__831CA38B_3358_4649_B263_5BAD4DC56744__INCLUDED_)
#define AFX_TOOLOBJECTREMOVEREGION_H__831CA38B_3358_4649_B263_5BAD4DC56744__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolObjectRemoveRegion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolObjectRemoveRegion dialog

class CToolObjectRemoveRegion : public CDialog
{
// Construction
public:
	CToolObjectRemoveRegion(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolObjectRemoveRegion)
	enum { IDD = IDD_FILE_REMOVE_OBJS };
	int		m_nEndX;
	int		m_nEndZ;
	int		m_nStartX;
	int		m_nStartZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolObjectRemoveRegion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolObjectRemoveRegion)
	afx_msg void OnRemoveObjects();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLOBJECTREMOVEREGION_H__831CA38B_3358_4649_B263_5BAD4DC56744__INCLUDED_)
