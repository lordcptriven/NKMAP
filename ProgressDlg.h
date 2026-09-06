#if !defined(AFX_PROGRESSDLG_H__C8CCC253_F6A6_4774_B69B_9E83AEBB02A3__INCLUDED_)
#define AFX_PROGRESSDLG_H__C8CCC253_F6A6_4774_B69B_9E83AEBB02A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

class CProgressDlg : public CDialog
{
// Construction
public:
	void	SetTitle( CString strTitle ) { m_ctrlTitle.SetWindowText( strTitle); }
	void	SetContent( CString strContent ) { m_ctrlContent.SetWindowText( strContent ); }
	void	SetRange( int nLower, int nUpper ) { m_ctrlProgress.SetRange32( nLower, nUpper ); }
	void	SetPos( int nPos ) { m_ctrlProgress.SetPos( nPos ); }
	void	SetOffsetPos( int nOffsetPos ) { m_ctrlProgress.OffsetPos( nOffsetPos ); }
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGRESSDLG };
	CStatic	m_ctrlContent;
	CButton	m_ctrlTitle;
	CProgressCtrl	m_ctrlProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__C8CCC253_F6A6_4774_B69B_9E83AEBB02A3__INCLUDED_)
