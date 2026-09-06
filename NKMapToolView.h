// NKMapToolView.h : interface of the CNKMapToolView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NKMAPTOOLVIEW_H__DE61C633_114C_4E1A_8432_78F16457086C__INCLUDED_)
#define AFX_NKMAPTOOLVIEW_H__DE61C633_114C_4E1A_8432_78F16457086C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNKMapToolView : public CView
{
protected: // create from serialization only
	CNKMapToolView();
	DECLARE_DYNCREATE(CNKMapToolView)

// Attributes
public:
	CNKMapToolDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNKMapToolView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNKMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNKMapToolView)
	afx_msg void OnHotkeyE();
	afx_msg void OnHotkeyQ();
	afx_msg void OnHotkeyR();
	afx_msg void OnHotkeyW();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NKMapToolView.cpp
inline CNKMapToolDoc* CNKMapToolView::GetDocument()
   { return (CNKMapToolDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NKMAPTOOLVIEW_H__DE61C633_114C_4E1A_8432_78F16457086C__INCLUDED_)
