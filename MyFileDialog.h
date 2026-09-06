#if !defined(AFX_MYFILEDIALOG_H__0FB40E28_284A_4DDE_AF71_039A4D3D3E45__INCLUDED_)
#define AFX_MYFILEDIALOG_H__0FB40E28_284A_4DDE_AF71_039A4D3D3E45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog dialog

class CMyFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CMyFileDialog)

public:
	CMyFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	CBitmap m_bmDefault;
	void	DrawNonePreview();
	void	DrawBMP();
	void	DrawNTX();
	virtual void OnFileNameChange();
	//{{AFX_MSG(CMyFileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEDIALOG_H__0FB40E28_284A_4DDE_AF71_039A4D3D3E45__INCLUDED_)
