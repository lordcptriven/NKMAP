// NKMapTool.h : main header file for the NKMAPTOOL application
//

#if !defined(AFX_NKMAPTOOL_H__45FF0B5C_6F5E_464C_9F75_591E99538837__INCLUDED_)
#define AFX_NKMAPTOOL_H__45FF0B5C_6F5E_464C_9F75_591E99538837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNKMapToolApp:
// See NKMapTool.cpp for the implementation of this class
//

class CNKMapToolApp : public CWinApp
{
public:
	CNKMapToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNKMapToolApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNKMapToolApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NKMAPTOOL_H__45FF0B5C_6F5E_464C_9F75_591E99538837__INCLUDED_)
