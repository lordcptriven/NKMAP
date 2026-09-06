#if !defined(AFX_TOOLCTRLBAR_H__838D9295_04EA_444F_9F8B_C3244C44BDB7__INCLUDED_)
#define AFX_TOOLCTRLBAR_H__838D9295_04EA_444F_9F8B_C3244C44BDB7__INCLUDED_

#include "ToolTexPaintPage.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolCtrlBar.h : header file
//


#include "sizecbar.h"
#include "ToolVertEditPage.h"
#include "ToolTexPaintPage.h"
#include "ToolObjEditPage.h"
#include "ToolLightEditPage.h"
#include "ToolAtmosEditPage.h"
#include "ToolAttrEditPage.h"


/////////////////////////////////////////////////////////////////////////////
// CToolCtrlBar window

class CToolCtrlBar : public CSizingControlBar
{
// Construction
public:
	CToolCtrlBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolCtrlBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	CToolVertEditPage	m_pageVertEdit;
	CToolTexPaintPage	m_pageTexPaint;
	CToolObjEditPage	m_pageObjEdit;
	CToolLightEditPage	m_pageLightEdit;
	CToolAtmosEditPage	m_pageAtmosEdit;
	CToolAttrEditPage	m_pageAttrEdit;

	void	ResetPage();
	void	UpdatePage();
	BOOL	Create( CWnd *pParent );
	virtual ~CToolCtrlBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLCTRLBAR_H__838D9295_04EA_444F_9F8B_C3244C44BDB7__INCLUDED_)
