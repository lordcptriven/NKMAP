#if !defined(AFX_TOOLTEXPAINTPAGE_H__BF3691EF_4E26_4172_AD68_C7F432208252__INCLUDED_)
#define AFX_TOOLTEXPAINTPAGE_H__BF3691EF_4E26_4172_AD68_C7F432208252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTexPaintPage.h : header file
//


#include "PageCtrl.h"
#include "ToolTexPaintListBox.h"


/////////////////////////////////////////////////////////////////////////////
// CToolTexPaintPage dialog

class CToolTexPaintPage : public CPageCtrl
{
// Construction
public:
	void	RemoveAllTextures();
	void	AddTexture( const TCHAR* szFilename );
	void	ShowLayers();
	void	ShowAllLayers();
	
	void	UpdateControls();
	BOOL	Create( CWnd* pParent );
	CToolTexPaintPage(CWnd* pParent = NULL);   // standard constructor

//////////////////////////////////////////////////////////////////////////////
// 수정자 : 이원석
// 수정일 : 05-02-22
// 수정내용 : Q,W,E,R 의 단축키 필요 멤버함수 추가
	BOOL PreTranslateMessage( MSG* pMsg ); // 메시지를 받는 함수를 오버로딩.
	void HK_PaintTypeP()
	{
		m_nEditType = 0; // 단순 버튼이 아니라 라디오 버튼이라서 직접 셋팅하고 적용시킨다.
		UpdateData( FALSE ); // 다이얼 로그에도 적용.
		OnToolPaintTypeP(); // 여기선 그래픽적인 선택만 바꿔주고 내적으로 적용시킬 뿐 실제값이 바뀌지는 않는다.
	}
	void HK_PaintTypeE()
	{
		m_nEditType = 1; // 단순 버튼이 아니라 라디오 버튼이라서 직접 셋팅하고 적용시킨다.
		UpdateData( FALSE ); // 다이얼 로그에도 적용.
		OnToolPaintTypeE(); // 여기선 그래픽적인 선택만 바꿔주고 내적으로 적용시킬 뿐 실제값이 바뀌지는 않는다.
	}
//////////////////////////////////////////////////////////////////////////////

	// Dialog Data
	//{{AFX_DATA(CToolTexPaintPage)
	enum { IDD = IDD_TOOL_TEXTURE_PAINT };
	CButton	m_ctrlToolType;
	CButton	m_ctrlShowLayer3;
	CButton	m_ctrlShowLayer2;
	CButton	m_ctrlShowLayer1;
	CSliderCtrl	m_ctrlStrength;
	CSliderCtrl	m_ctrlSize;
	int		m_nSize;
	int		m_nStrength;
	BOOL	m_bFilter;
	BOOL	m_bShowLayer1;
	BOOL	m_bShowLayer2;
	BOOL	m_bShowLayer3;
	int		m_nEditLayer;
	BOOL	m_bBrushTool;
	int		m_nEditType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTexPaintPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBitmap					m_bmShowIcon, m_bmHideIcon;
	CBitmap					m_bmLayer1, m_bmLayer1_Check;
	CBitmap					m_bmLayer2, m_bmLayer2_Check;
	CBitmap					m_bmLayer3, m_bmLayer3_Check;
	CBitmap					m_bmPaint, m_bmPaint_Check;
	CBitmap					m_bmErase, m_bmErase_Check;
	CBitmap					m_bmPencil, m_bmBrush;
	BOOL					m_bLayer23Brush;
	CToolTexPaintListBox	m_ListBox;

	// Generated message map functions
	//{{AFX_MSG(CToolTexPaintPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnToolPaintNew();
	afx_msg void OnToolPaintDelete();
	afx_msg void OnToolPaintTypeP();
	afx_msg void OnToolPaintTypeE();
	afx_msg void OnChangeToolPaintSizeEdit();
	afx_msg void OnChangeToolPaintStrengthEdit();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnToolPaintFilter();
	afx_msg void OnToolPaintLayer1();
	afx_msg void OnToolPaintLayer2();
	afx_msg void OnToolPaintLayer3();
	afx_msg void OnToolPaintShowLayer1();
	afx_msg void OnToolPaintShowLayer2();
	afx_msg void OnToolPaintShowLayer3();
	afx_msg void OnToolPaintToolType();
	afx_msg void OnToolPaintReplace();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTEXPAINTPAGE_H__BF3691EF_4E26_4172_AD68_C7F432208252__INCLUDED_)
