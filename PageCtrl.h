#if !defined(AFX_PAGECTRL_H__3045F770_EEBF_426D_A2AC_208B02E31F73__INCLUDED_)
#define AFX_PAGECTRL_H__3045F770_EEBF_426D_A2AC_208B02E31F73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageCtrl dialog

class CPageCtrl : public CDialog
{
// Construction
public:
	BOOL EnableWindow( BOOL bEnable = TRUE );
	void CalcLayout();
	virtual void	UpdateControls() = 0;
	virtual void	OnOK();
	virtual void	OnCancel();
	CPageCtrl( UINT nIDTemplate, CWnd* pParent = NULL );   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageCtrl)
	enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nScrollPos;
	CPoint m_pt;

	// Generated message map functions
	//{{AFX_MSG(CPageCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGECTRL_H__3045F770_EEBF_426D_A2AC_208B02E31F73__INCLUDED_)
