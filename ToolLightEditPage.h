#if !defined(AFX_TOOLLIGHTEDITPAGE_H__B2C3A443_CEF9_41DC_8988_C1C9C0C64FF5__INCLUDED_)
#define AFX_TOOLLIGHTEDITPAGE_H__B2C3A443_CEF9_41DC_8988_C1C9C0C64FF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolLightEditPage.h : header file
//


#include "PageCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CToolLightEditPage dialog

class CToolLightEditPage : public CPageCtrl
{
// Construction
public:
	void	SetDirection( float fX, float fY, float fZ );
	void	UpdateDirectLightInfo();

	void	UpdateControls();
	BOOL	Create( CWnd* pParent );
	CToolLightEditPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolLightEditPage)
	enum { IDD = IDD_TOOL_LIGHT_EDIT };
	CSliderCtrl	m_ctrlBrushStrength;
	CSliderCtrl	m_ctrlBrushSize;
	CComboBox	m_ctrlWorldTime;
	float	m_fDirectionX;
	float	m_fDirectionY;
	float	m_fDirectionZ;
	int		m_nLightColorR;
	int		m_nLightColorG;
	int		m_nLightColorB;
	int		m_nAmbientColorR;
	int		m_nAmbientColorG;
	int		m_nAmbientColorB;
	BOOL	m_bDirControl;
	int		m_nEditType;
	int		m_nShadowR;
	int		m_nShadowG;
	int		m_nShadowB;
	int		m_nBrushSize;
	int		m_nBrushStrength;
	BOOL	m_bDrawCurrentMap;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolLightEditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBitmap		m_bmDirControl, m_bmDirControl_Check;
	CBitmap		m_bmPaint, m_bmPaint_Check;
	CBitmap		m_bmErase, m_bmErase_Check;
	int			m_nTimeID;

	// Generated message map functions
	//{{AFX_MSG(CToolLightEditPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeToolLightTime();
	afx_msg void OnPaint();
	afx_msg void OnToolLightLightcolor();
	afx_msg void OnToolLightAmbientcolor();
	afx_msg void OnToolLightDircontrol();
	afx_msg void OnToolLightShadowcolor();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeToolLightBrushSizeEdit();
	afx_msg void OnChangeToolLightBrushStrengthEdit();
	afx_msg void OnToolLightPaint();
	afx_msg void OnToolLightErase();
	afx_msg void OnToolLightShadowCurmap();
	afx_msg void OnToolLightShadowImport();
	afx_msg void OnToolLightShadowExport();
	afx_msg void OnToolLightDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLLIGHTEDITPAGE_H__B2C3A443_CEF9_41DC_8988_C1C9C0C64FF5__INCLUDED_)
