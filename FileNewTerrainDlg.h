#if !defined(AFX_FILENEWTERRAINDLG_H__9D88C866_BD76_4474_B93E_C2B5EB16146B__INCLUDED_)
#define AFX_FILENEWTERRAINDLG_H__9D88C866_BD76_4474_B93E_C2B5EB16146B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNewTerrainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileNewTerrainDlg dialog

class CFileNewTerrainDlg : public CDialog
{
// Construction
public:
	CFileNewTerrainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileNewTerrainDlg)
	enum { IDD = IDD_FILE_NEWTERRAIN };
	CComboBox	m_ctrlWHSegment;
	CString	m_strName;
	CString	m_strWHSegment;
	int		m_nPatchSize;
	CString	m_strTexture;
	CString	m_strHeightMap;
	int		m_nHeightScale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileNewTerrainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileNewTerrainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnNewterrainOpentex();
	afx_msg void OnNewterrainOpenhm();
	afx_msg void OnSelchangeNewterrainWhsegment();
	afx_msg void OnNewterrainGenerate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENEWTERRAINDLG_H__9D88C866_BD76_4474_B93E_C2B5EB16146B__INCLUDED_)
