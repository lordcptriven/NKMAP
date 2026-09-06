// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__72B96D26_AA88_4E96_A5BA_A6D5F55A2370__INCLUDED_)
#define AFX_MAINFRM_H__72B96D26_AA88_4E96_A5BA_A6D5F55A2370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ToolCtrlBar.h"


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	CToolCtrlBar m_wndToolCtrlBar;
	CStatusBar*	GetStatusBar() { return &m_wndStatusBar; }
	
	// [2007/11/1 theodoric] Ä«¸Þ¶ó Å¾ºä
	void OnCameraUnlimitUpdate(){OnCameraUnlimit();}
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	BOOL CreateToolBar();
	BOOL CreateDockingBar();
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnToolVertexEdit();
	afx_msg void OnToolTexturePaint();
	afx_msg void OnToolObjectEdit();
	afx_msg void OnUpdateToolVertexEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolTexturePaint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolObjectEdit(CCmdUI* pCmdUI);
	afx_msg void OnViewGridNone();
	afx_msg void OnViewGridSmall();
	afx_msg void OnViewGridMedium();
	afx_msg void OnViewGridLarge();
	afx_msg void OnUpdateViewGridNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGridSmall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGridMedium(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGridLarge(CCmdUI* pCmdUI);
	afx_msg void OnViewObject();
	afx_msg void OnUpdateViewTerrain(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewObject(CCmdUI* pCmdUI);
	afx_msg void OnCameraObserve();
	afx_msg void OnCameraFly();
	afx_msg void OnUpdateCameraObserve(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraFly(CCmdUI* pCmdUI);
	afx_msg void OnToolVertexEditIcon();
	afx_msg void OnToolTexturePaintIcon();
	afx_msg void OnToolObjectEditIcon();
	afx_msg void OnToolAttributeEdit();
	afx_msg void OnUpdateToolAttributeEdit(CCmdUI* pCmdUI);
	afx_msg void OnToolAttributeEditIcon();
	afx_msg void OnViewFog();
	afx_msg void OnUpdateViewFog(CCmdUI* pCmdUI);
	afx_msg void OnViewTerrain();
	afx_msg void OnViewAttr_Map();
	afx_msg void OnToolLightEdit();
	afx_msg void OnUpdateToolLightEdit(CCmdUI* pCmdUI);
	afx_msg void OnToolLightEditIcon();
	afx_msg void OnViewCharacter();
	afx_msg void OnUpdateViewCharacter(CCmdUI* pCmdUI);
	afx_msg void OnViewSky();
	afx_msg void OnUpdateViewSky(CCmdUI* pCmdUI);
	afx_msg void OnToolAtmosphereEdit();
	afx_msg void OnUpdateToolAtmosphereEdit(CCmdUI* pCmdUI);
	afx_msg void OnToolAtmosEditIcon();
	afx_msg void OnToolMapmove();
	afx_msg void OnUpdateToolMapmove(CCmdUI* pCmdUI);
	afx_msg void OnToolMapmoveIcon();
	afx_msg void OnViewWater();
	afx_msg void OnUpdateViewWater(CCmdUI* pCmdUI);
	afx_msg void OnCameraSpeed1x();
	afx_msg void OnCameraSpeed2x();
	afx_msg void OnCameraSpeed4x();
	afx_msg void OnUpdateCameraSpeed1x(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraSpeed2x(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraSpeed4x(CCmdUI* pCmdUI);
	afx_msg void OnCameraSpeed8x();
	afx_msg void OnCameraSpeed10x();
	afx_msg void OnUpdateCameraSpeed10x(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraSpeed8x(CCmdUI* pCmdUI);
	afx_msg void OnCameraUnlimit();
	afx_msg void OnUpdateCameraUnlimit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__72B96D26_AA88_4E96_A5BA_A6D5F55A2370__INCLUDED_)
