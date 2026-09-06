// MyFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NKMapTool.h"
#include "MyFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog

IMPLEMENT_DYNAMIC(CMyFileDialog, CFileDialog)

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.Flags |= OFN_ENABLETEMPLATE;
	m_ofn.lpTemplateName = MAKEINTRESOURCE( IDD_MYFILEDLG );
}


BEGIN_MESSAGE_MAP(CMyFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CMyFileDialog)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMyFileDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bmDefault.LoadBitmap( IDB_FILE_NONEPREVIEW );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMyFileDialog::OnDestroy() 
{
	CFileDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_bmDefault.DeleteObject();
}


void CMyFileDialog::OnFileNameChange()
{
	CFileDialog::OnFileNameChange();
	
	CString	strPath = GetPathName();
	CString	strExt = strPath.Right( 4 );
	if( strExt.Compare( _T(".bmp" ) ) == 0 ) DrawBMP();			// bmp
	else if( strExt.Compare( _T(".tga" ) ) == 0 ) {}			// tga
	else if( strExt.Compare( _T(".ntx" ) ) == 0 ) DrawNTX();	// nako texture file
	else if( strExt.Compare( _T(".nmf" ) ) == 0 ) {}			// nako maptool file
	else if( strExt.Compare( _T(".map" ) ) == 0 ) {}			// map file
	else DrawNonePreview();
}


void CMyFileDialog::DrawNonePreview()
{
	if( (HBITMAP)m_bmDefault )
	{
		CRect		rc;
		CClientDC	dc( GetDlgItem( IDC_FILE_PREVIEW ) );
		GetDlgItem( IDC_FILE_PREVIEW )->GetClientRect( rc );

		HDC			hdcBitmap = NULL;
		hdcBitmap = CreateCompatibleDC( dc.m_hDC );

		if( hdcBitmap == NULL )
			return ;

		SelectObject( hdcBitmap, (HBITMAP)m_bmDefault );

		dc.SetStretchBltMode( COLORONCOLOR );
		StretchBlt( dc.m_hDC, 0, 0, rc.Width(), rc.Height(), hdcBitmap, 0, 0, 128, 128, SRCCOPY );
	
		DeleteDC( hdcBitmap );

		GetDlgItem( IDC_FILE_PREVIEW_INFO )->SetWindowText( _T("") );
	}

	return;
}


void CMyFileDialog::DrawBMP()
{
	HBITMAP	hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage( NULL, GetPathName(), IMAGE_BITMAP, 0, 0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	
	if( hBitmap )
	{
		CRect		rc;
		CClientDC	dc( GetDlgItem( IDC_FILE_PREVIEW ) );		
		GetDlgItem( IDC_FILE_PREVIEW )->GetClientRect( rc );

		HDC			hdcBitmap = NULL;
		hdcBitmap = CreateCompatibleDC( dc.m_hDC );

		if( hdcBitmap == NULL )
		{
			DeleteObject( hBitmap );
			DrawNonePreview();
			return ;
		}

		BITMAP	bm;
		GetObject( hBitmap, sizeof(BITMAP), &bm );
		SelectObject( hdcBitmap, hBitmap );

		dc.SetStretchBltMode( COLORONCOLOR );
		StretchBlt( dc.m_hDC, 0, 0, rc.Width(), rc.Height(), hdcBitmap, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY );
		
		DeleteDC( hdcBitmap );

		CString	strInfo;
		strInfo.Format( _T("%dx%d, %d bits"), bm.bmWidth, bm.bmHeight, bm.bmBitsPixel );
		GetDlgItem( IDC_FILE_PREVIEW_INFO )->SetWindowText( strInfo );		

		DeleteObject( hBitmap );
	}
	else
	{
		DrawNonePreview();
	}
	
	return;
}


void CMyFileDialog::DrawNTX()
{
	FILE*	fp = fopen( GetPathName(), "rb" );
	if( fp == NULL )
	{
		DrawNonePreview();
		return ;
	}

	// Magic
	DWORD	dwMagic;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( 'N' << 24 | 'T' << 16 | 'X' << 8 | ' ' ) )
	{
		fclose( fp );
		DrawNonePreview();
		return ;
	}

	// Width, Height, Mipmap Count
	DWORD	dwWidth, dwHeight, dwBpp, dwMipLevels;
	fread( &dwWidth, sizeof(DWORD), 1, fp );
	fread( &dwHeight, sizeof(DWORD), 1, fp );
	fread( &dwBpp, sizeof(DWORD), 1, fp );
	fread( &dwMipLevels, sizeof(DWORD), 1, fp );

	// Allocate memory
	BYTE*	pBits;
	pBits = new BYTE[sizeof(BYTE) * 3 * dwWidth * dwHeight];
	if( pBits == NULL )
	{
		fclose( fp );
		DrawNonePreview();
		return ;
	}

	// Bit data
	if( fread( pBits, sizeof(BYTE), 3 * dwWidth * dwHeight, fp ) < 3 * dwWidth * dwHeight )
	{
		delete [] pBits;			
		fclose( fp );
		DrawNonePreview();
		return ;
	}

	// Swap r & b channel
	BYTE*	pSrcBits = pBits;
	BYTE	temp;
	DWORD	dwX, dwY;
	for( dwY = 0; dwY < dwHeight; dwY++ )
	{
		for( dwX = 0; dwX < dwWidth; dwX++ )
		{
			temp = *( pSrcBits );
			*( pSrcBits ) = *( pSrcBits + 2 );
			*( pSrcBits + 2 ) = temp;
			pSrcBits += 3;
		}
	}

	fclose( fp );

	// Create DIBitmap
	DWORD	dwImageSize = dwWidth * dwHeight * dwBpp / 8;
	BITMAPINFO bh;
	memset( &bh, 0, sizeof(bh) );
	bh.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bh.bmiHeader.biWidth = dwWidth;
	bh.bmiHeader.biHeight = dwHeight;
	bh.bmiHeader.biPlanes = 1;
	bh.bmiHeader.biBitCount = dwBpp;
	bh.bmiHeader.biCompression = BI_RGB;
	bh.bmiHeader.biSizeImage = dwImageSize;

	CClientDC	dc( GetDlgItem( IDC_FILE_PREVIEW ) );
	HBITMAP hBitmap = CreateDIBitmap( dc.m_hDC, &bh.bmiHeader, CBM_INIT, pBits, &bh, DIB_RGB_COLORS );
	delete [] pBits;

	if( hBitmap )
	{
		CRect	rc;
		GetDlgItem( IDC_FILE_PREVIEW )->GetClientRect( rc );

		HDC		hdcBitmap = CreateCompatibleDC( dc.m_hDC );	
		if( hdcBitmap == NULL )
		{
			DeleteObject( hBitmap );
			DrawNonePreview();
			return ;
		}

		SelectObject( hdcBitmap, hBitmap );
		dc.SetStretchBltMode( COLORONCOLOR );
		StretchBlt( dc.m_hDC, 0, 0, rc.Width(), rc.Height(), hdcBitmap, 0, 0, dwWidth, dwHeight, SRCCOPY );
		DeleteDC( hdcBitmap );

		CString	strInfo;
		strInfo.Format( _T("%dx%d, %d bits, %d mips"), dwWidth, dwHeight, dwBpp, dwMipLevels );
		GetDlgItem( IDC_FILE_PREVIEW_INFO )->SetWindowText( strInfo );		

		DeleteObject( hBitmap );
	}
	else
	{
		DrawNonePreview();
	}	
}


