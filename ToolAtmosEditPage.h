#if !defined(AFX_TOOLATMOSEDITPAGE_H__71BDBEE2_7CD5_4730_8424_6EB91B6DDE48__INCLUDED_)
#define AFX_TOOLATMOSEDITPAGE_H__71BDBEE2_7CD5_4730_8424_6EB91B6DDE48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolAtmosEditPage.h : header file
//


#include "PageCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CToolAtmosEditPage dialog

class CToolAtmosEditPage : public CPageCtrl
{
// Construction
public:
	void	SetWaveWaterSegment( int nSegment );
	void	ResetControls();
	void	UpdateControls();
	BOOL	Create( CWnd* pParent );
	CToolAtmosEditPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolAtmosEditPage)
	enum { IDD = IDD_TOOL_ATMOS_EDIT };
	CComboBox	m_ctrlWaterGroup;
	CString	m_strBaseWaterTex;
	int		m_nWaterType;
	int		m_nEditType;
	float	m_fWaterHeight;
	int		m_nWaterOpacity;
	CString	m_strWaterGroup;
	CString	m_strWaveWaterTex2;
	CString	m_strWaveWaterTex1;
	int		m_nWaterSegment;
	CString	m_strComboGroupName;
	BOOL	m_bWaterGuideLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolAtmosEditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		m_nCurWaterGroup;
	CString	m_aGroupNames[20];
	float	m_aWaterHeights[20];
	BOOL	m_bGroupAddActive;
	BOOL	m_bGroupModifyActive;
	BOOL	m_bGroupRemoveActive;

	CBitmap		m_bmPaint, m_bmPaint_Check;
	CBitmap		m_bmErase, m_bmErase_Check;
	CBitmap		m_bmBase, m_bmBase_Check;
	CBitmap		m_bmWave, m_bmWave_Check;

	// Generated message map functions
	//{{AFX_MSG(CToolAtmosEditPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnToolAtmosWaterBaseTexnameOpen();
	afx_msg void OnToolAtmosWaterWaveTexnameOpen();
	afx_msg void OnToolAtmosWaterTypeBase();
	afx_msg void OnToolAtmosWaterTypeWave();
	afx_msg void OnToolAtmosWaterEditErase();
	afx_msg void OnToolAtmosWaterEditPaint();
	afx_msg void OnChangeToolAtmosWaterOpacity();
	afx_msg void OnDeltaposToolAtmosWaterOpacitySpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolAtmosWaterGroupAdd();
	afx_msg void OnToolAtmosWaterGroupModify();
	afx_msg void OnToolAtmosWaterGroupRemove();
	afx_msg void OnSelchangeToolAtmosWaterGroup();
	afx_msg void OnDestroy();
	afx_msg void OnToolAtmosWaterWaveTexnameOpen2();
	afx_msg void OnToolAtmosWaterPaintAll();
	afx_msg void OnChangeToolAtmosWaterSegment();
	afx_msg void OnDeltaposToolAtmosWaterSegmentSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolAtmosWaterGuideLine();
	afx_msg void OnToolAtmosWaterPaintAllwave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLATMOSEDITPAGE_H__71BDBEE2_7CD5_4730_8424_6EB91B6DDE48__INCLUDED_)
