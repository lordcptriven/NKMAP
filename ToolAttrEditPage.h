#if !defined(AFX_TOOLATTREDITPAGE_H__209F566B_7E7A_482A_A7F5_81C06DAC4821__INCLUDED_)
#define AFX_TOOLATTREDITPAGE_H__209F566B_7E7A_482A_A7F5_81C06DAC4821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolAttrEditPage.h : header file
//


#include "PageCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CToolAttrEditPage dialog

class CToolAttrEditPage : public CPageCtrl
{
// Construction
public:
	void	UpdateControls();
	BOOL	Create( CWnd* pParent );
	CToolAttrEditPage(CWnd* pParent = NULL);   // standard constructor

	void UpdateBrushSizeSlider( int AddValue ); // 브러쉬 사이즈 조절 슬라이더를 외부에서 조절한다.(by 원석)


// Dialog Data
	//{{AFX_DATA(CToolAttrEditPage)
	enum { IDD = IDD_TOOL_ATTMAP_EDIT };
	CSliderCtrl	m_ctrlBrushSize;
	CSpinButtonCtrl	m_ctrlVisibleRange;
	int		m_nVisibleRange;
	int		m_nCurMapAttribute;
	int		m_nBrushSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolAttrEditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COLORREF m_crAbleToWalk;
	COLORREF m_crUnableToWalk;
	COLORREF m_crWater;
	COLORREF m_crIndoor;
	COLORREF m_crIndoorBox;
	COLORREF m_crOutdoorBox;

	// Generated message map functions
	//{{AFX_MSG(CToolAttrEditPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnToolAttmapUnabletowalkColor();
	afx_msg void OnToolAttmapWaterColor();
	afx_msg void OnToolAttmapIndoorColor();
	afx_msg void OnToolAttmapIndoorboxColor();
	afx_msg void OnToolAttmapOutdoorboxColor();
	afx_msg void OnToolAttmapGenerate();
	afx_msg void OnToolAttmapImport();
	afx_msg void OnToolAttmapExport();
	afx_msg void OnToolAttmapAbletowalk();
	afx_msg void OnToolAttmapUnabletowalk();
	afx_msg void OnToolAttmapWater();
	afx_msg void OnToolAttmapIndoor();
	afx_msg void OnToolAttmapIndoorbox();
	afx_msg void OnToolAttmapOutdoorbox();
	afx_msg void OnDeltaposToolAttmapRangeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLATTREDITPAGE_H__209F566B_7E7A_482A_A7F5_81C06DAC4821__INCLUDED_)
