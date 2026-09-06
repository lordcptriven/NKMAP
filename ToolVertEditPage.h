#if !defined(AFX_TOOLVERTEDITPAGE_H__129E3331_5DA8_4866_BBD3_547C57998D47__INCLUDED_)
#define AFX_TOOLVERTEDITPAGE_H__129E3331_5DA8_4866_BBD3_547C57998D47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolVertEditPage.h : header file
//


#include "PageCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CToolVertEditPage dialog

class CToolVertEditPage : public CPageCtrl
{
// Construction
public:
	void	UpdateTerrainInfo();

	void	UpdateControls();
	BOOL	Create( CWnd* pParent );
	CToolVertEditPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolVertEditPage)
	enum { IDD = IDD_TOOL_VERTEX_EDIT };
	CSliderCtrl	m_ctrlBrushStrength;
	CSliderCtrl	m_ctrlBrushSize;
	CEdit	m_ctrlHeightScale;
	CEdit	m_ctrlPatchSize;
	CString	m_strName;
	int		m_nSegment;
	int		m_nPatchSize;
	int		m_nHeightScale;
	CString	m_strMinMaxHeight;
	int		m_nControlUnit;
	int		m_nBrushSize;
	int		m_nBrushStrength;
	int		m_nOffset;
	int		m_nEditType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolVertEditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBitmap		m_bmTile, m_bmTile_Check;
	CBitmap		m_bmVertex, m_bmVertex_Check;
	CBitmap		m_bmRaise, m_bmRaise_Check;
	CBitmap		m_bmLower, m_bmLower_Check;
	CBitmap		m_bmFlatten, m_bmFlatten_Check;
	CBitmap		m_bmSmooth, m_bmSmooth_Check;
	CBitmap		m_bmRaiseAll, m_bmLowerAll;
	BOOL		m_bPatchSizeCanInput;
	BOOL		m_bHeightScaleCanInput;

	void	AffectNeighbor();

	// Generated message map functions
	//{{AFX_MSG(CToolVertEditPage)
	afx_msg void OnToolVertexPatchsizeModify();
	afx_msg void OnToolVertexHeightscaleModify();
	virtual BOOL OnInitDialog();
	afx_msg void OnToolVertexHeightmapReapply();
	afx_msg void OnToolVertexRaiseEntire();
	afx_msg void OnToolVertexLowerEntire();
	afx_msg void OnToolVertexControlTile();
	afx_msg void OnToolVertexControlVertex();
	afx_msg void OnToolVertexRaise();
	afx_msg void OnToolVertexLower();
	afx_msg void OnToolVertexFlatten();
	afx_msg void OnToolVertexSmooth();
	afx_msg void OnChangeToolVertexBrushSizeEdit();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeToolVertexBrushStrengthEdit();
	afx_msg void OnToolVertexHeightmapExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLVERTEDITPAGE_H__129E3331_5DA8_4866_BBD3_547C57998D47__INCLUDED_)
