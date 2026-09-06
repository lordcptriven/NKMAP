#if !defined(AFX_FILEMERGEMAPS_H__A71758B0_6F3F_4CDB_9059_EAE77CDB1681__INCLUDED_)
#define AFX_FILEMERGEMAPS_H__A71758B0_6F3F_4CDB_9059_EAE77CDB1681__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileMergeMaps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileMergeMaps dialog

class CFileMergeMaps : public CDialog
{
// Construction
public:
	CFileMergeMaps(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileMergeMaps)
	enum { IDD = IDD_FILE_MERGE_MAPS };
	CString	m_strNmfName;
	CString	m_strNotName;
	int		m_nMergeSX;
	int		m_nMergeSZ;
	int		m_nMergeEX;
	int		m_nMergeEZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileMergeMaps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileMergeMaps)
	afx_msg void OnMergemapNmfOpen();
	afx_msg void OnMergemapNotOpen();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeMergemapRegionSx();
	afx_msg void OnChangeMergemapRegionSz();
	afx_msg void OnChangeMergemapRegionEx();
	afx_msg void OnChangeMergemapRegionEz();
	afx_msg void OnMergemapMerge();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEMERGEMAPS_H__A71758B0_6F3F_4CDB_9059_EAE77CDB1681__INCLUDED_)
