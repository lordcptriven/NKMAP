#if !defined(AFX_TOOLOBJEDITPAGE_H__C1A0F8F5_3A72_48CE_8401_08A4026F2F93__INCLUDED_)
#define AFX_TOOLOBJEDITPAGE_H__C1A0F8F5_3A72_48CE_8401_08A4026F2F93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolObjEditPage.h : header file
//


#include "PageCtrl.h"


class CDecoObj;


/////////////////////////////////////////////////////////////////////////////
// CToolObjEditPage dialog

class CToolObjEditPage : public CPageCtrl
{
// Construction
public:
	void	UpdateReferenceCount( int nIndex );
	void	AddAllObjects();
	void	RemoveAllObjects();
	void	UpdateParameters( CDecoObj* pDecoObj );

	void	UpdateControls();
	BOOL	Create( CWnd* pParent );
	CToolObjEditPage(CWnd* pParent = NULL);   // standard constructor

//////////////////////////////////////////////////////////////////////////////
// 수정자 : 이원석
// 수정일 : 05-02-22
// 수정내용 : Q,W,E,R 의 단축키 필요 멤버함수 추가
	BOOL PreTranslateMessage( MSG* pMsg ); // 메시지를 받는 함수를 오버로딩.
	void HK_ObjectSelect()
	{
		m_nWorkType = 0; // 단순 버튼이 아니라 라디오 버튼이라서 직접 셋팅하고 적용시킨다.
		UpdateData( FALSE ); // 다이얼 로그에도 적용.
		OnToolObjectSelect(); // 여기선 그래픽적인 선택만 바꿔주고 내적으로 적용시킬 뿐 실제 m_nWorkType이 바뀌지는 않는다.
	}
	void HK_ToolObjectPlace()
	{
		m_nWorkType = 1; // 단순 버튼이 아니라 라디오 버튼이라서 직접 셋팅하고 적용시킨다.
		UpdateData( FALSE ); // 다이얼 로그에도 적용.
		OnToolObjectPlace(); // 여기선 그래픽적인 선택만 바꿔주고 내적으로 적용시킬 뿐 실제 m_nWorkType이 바뀌지는 않는다.
	}
	void HK_ToolObjectMove()
	{
		m_nWorkType = 2; // 단순 버튼이 아니라 라디오 버튼이라서 직접 셋팅하고 적용시킨다.
		UpdateData( FALSE ); // 다이얼 로그에도 적용.
		OnToolObjectMove(); // 여기선 그래픽적인 선택만 바꿔주고 내적으로 적용시킬 뿐 실제 m_nWorkType이 바뀌지는 않는다.
	}
	void HK_ToolObjectRotate()
	{
		m_nWorkType = 3; // 단순 버튼이 아니라 라디오 버튼이라서 직접 셋팅하고 적용시킨다.
		UpdateData( FALSE ); // 다이얼 로그에도 적용.
		OnToolObjectRotate(); // 여기선 그래픽적인 선택만 바꿔주고 내적으로 적용시킬 뿐 실제 m_nWorkType이 바뀌지는 않는다.
	}
//////////////////////////////////////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CToolObjEditPage)
	enum { IDD = IDD_TOOL_OBJECT_EDIT };
	CComboBox	m_ctrlType;
	CListBox	m_ctrlObjList;
	CString	m_strLolo;
	int		m_nIndex;
	CString	m_strName;
	CString	m_strType;
	int		m_nWorkType;
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	float	m_fRotate;
	BOOL	m_bRandomRotation;
	float	m_fOffset;
	BOOL	m_bShowNodeBound;
	int		m_nAxis;
	int		m_nCount;
	int		m_nReferenceCount;
	BOOL	m_bLocalAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolObjEditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBitmap	m_bmSelect, m_bmSelect_Check;
	CBitmap	m_bmPlace, m_bmPlace_Check;
	CBitmap	m_bmMove, m_bmMove_Check;
	CBitmap	m_bmRotate, m_bmRotate_Check;
	CBitmap	m_bmAlign, m_bmArray;

	// Generated message map functions
	//{{AFX_MSG(CToolObjEditPage)
	afx_msg void OnToolObjectInsert();
	afx_msg void OnToolObjectModify();
	afx_msg void OnToolObjectRemove();
	afx_msg void OnSelchangeToolObjectListbox();
	virtual BOOL OnInitDialog();
	afx_msg void OnToolObjectOpenlolo();
	afx_msg void OnToolObjectSelect();
	afx_msg void OnToolObjectPlace();
	afx_msg void OnToolObjectMove();
	afx_msg void OnToolObjectRotate();
	afx_msg void OnKillfocusToolObjectXEdit();
	afx_msg void OnKillfocusToolObjectYEdit();
	afx_msg void OnKillfocusToolObjectZEdit();
	afx_msg void OnKillfocusToolObjectRotateEdit();
	afx_msg void OnToolObjectShowNodebound();
	afx_msg void OnToolObjectNodebound();
	afx_msg void OnToolObjectRandom();
	afx_msg void OnToolObjectAlign();
	afx_msg void OnToolObjectArray();
	afx_msg void OnToolObjectImport();
	afx_msg void OnCheckLocalaxis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLOBJEDITPAGE_H__C1A0F8F5_3A72_48CE_8401_08A4026F2F93__INCLUDED_)
