// NKMapToolDoc.h : interface of the CNKMapToolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NKMAPTOOLDOC_H__2AB0CFD6_BD62_429A_A2F7_38AE930D8773__INCLUDED_)
#define AFX_NKMAPTOOLDOC_H__2AB0CFD6_BD62_429A_A2F7_38AE930D8773__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNKMapToolDoc : public CDocument
{
protected: // create from serialization only
	CNKMapToolDoc();
	DECLARE_DYNCREATE(CNKMapToolDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNKMapToolDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNKMapToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNKMapToolDoc)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileClose();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileOpenGameMap();
	afx_msg void OnFileSaveGameMap();
	afx_msg void OnFileMergeMap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NKMAPTOOLDOC_H__2AB0CFD6_BD62_429A_A2F7_38AE930D8773__INCLUDED_)
