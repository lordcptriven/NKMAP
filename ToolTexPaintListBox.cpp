// ToolTexPaintListBox.cpp : implementation file
// See ToolTexPaintListBoxt.h for details on how to use this class
//

#include "stdafx.h"
#include "ToolTexPaintListBox.h"
#include "MyNKApp.h"
#include "Config.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// Global variables
extern CMyNKApp*	g_pMyNKApp;


/////////////////////////////////////////////////////////////////////////////
// CToolTexPaintListBox

CToolTexPaintListBox::CToolTexPaintListBox()
{	
	m_nCurSelTexture = -1;
	m_vecBitmaps.reserve( TEXTURE_MAX_COUNT );
	m_vecTextureInfos.reserve( TEXTURE_MAX_COUNT );
}

CToolTexPaintListBox::~CToolTexPaintListBox()
{
}


BEGIN_MESSAGE_MAP(CToolTexPaintListBox, CListBox)
	//{{AFX_MSG_MAP(CToolTexPaintListBox)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolTexPaintListBox message handlers

void CToolTexPaintListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if( m_vecBitmaps.size() > 0 )
	{
		CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC );

		// Item region
		CRect rcItem(lpDrawItemStruct->rcItem );
		
		// Draw bitmap
		HDC	hdcBitmap = NULL;
		hdcBitmap = CreateCompatibleDC( pDC->m_hDC );
		if( hdcBitmap )
		{
			SelectObject( hdcBitmap, m_vecBitmaps[lpDrawItemStruct->itemID] );
			pDC->SetStretchBltMode( COLORONCOLOR );
			BITMAP	bm;
			GetObject( m_vecBitmaps[lpDrawItemStruct->itemID], sizeof(BITMAP), &bm );
			StretchBlt( pDC->m_hDC, rcItem.left, rcItem.top, 48, 48,
						hdcBitmap, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY );
			DeleteObject( hdcBitmap );
		}

		rcItem.left += 48;
		if( lpDrawItemStruct->itemAction & ODA_FOCUS )	// Focus rect
		{
			CBrush brush( RGB(41, 69, 132) );
			pDC->FillRect( rcItem, &brush );
			pDC->SetTextColor( (COLORREF)RGB(255, 255, 255) );
			pDC->SetBkColor( (COLORREF)RGB(41, 69, 132) );
		}
		else
		{
			CBrush brush( RGB(255, 255, 255) );
			pDC->FillRect( rcItem, &brush );
			pDC->SetTextColor( (COLORREF)RGB(0, 0, 0) );
			pDC->SetBkColor( (COLORREF)RGB(255, 255, 255) );
		}

		// Draw text
		pDC->TextOut( 50, rcItem.top + 30, m_vecTextureInfos[lpDrawItemStruct->itemID].szName );
		//pDC->TextOut( 50, rcItem.top + 16, m_vecTextureInfos[lpDrawItemStruct->itemID].szWH );
		//pDC->TextOut( 50, rcItem.top + 30, m_vecTextureInfos[lpDrawItemStruct->itemID].szBpp );
	}
}


void CToolTexPaintListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 48;
}


void CToolTexPaintListBox::AddListItem( const TCHAR* szFilename, int nIndex )
{
	TextureInfos	Infos;

	// Store a texture filename
	_tcscpy( Infos.szName, szFilename );
	Infos.szName[_tcslen( szFilename )] = _T('\0');

	// Create a bitmap for a thumnail
	HBITMAP	hBitmap = NULL;
	CString	strFilename = szFilename;
	CString	strExt = strFilename.Right( 4 );
	CString strFullPath;
	strFullPath.Format( _T("%s\\map\\%s\\%s"), g_pMyNKApp->GetWorkingPath(),
						g_pMyNKApp->GetTerrain()->GetTerrainName(), strFilename );
	if( strExt.Compare( _T(".bmp" ) ) == 0 )
	{		
		hBitmap = (HBITMAP)LoadImage( NULL, strFullPath, IMAGE_BITMAP, 48, 48,
										LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	}
	else if( strExt.Compare( _T(".ntx" ) ) == 0 )
	{
		FILE*	fp = fopen( strFullPath, "rb" );
		if( fp )
		{
			// Magic
			DWORD	dwMagic;
			fread( &dwMagic, sizeof(DWORD), 1, fp );
			if( dwMagic == ( 'N' << 24 | 'T' << 16 | 'X' << 8 | ' ' ) )
			{
				// Width, Height, Mipmap Count
				DWORD	dwWidth, dwHeight, dwBpp, dwMipLevels;
				fread( &dwWidth, sizeof(DWORD), 1, fp );
				fread( &dwHeight, sizeof(DWORD), 1, fp );
				fread( &dwBpp, sizeof(DWORD), 1, fp );
				fread( &dwMipLevels, sizeof(DWORD), 1, fp );

				// Allocate memory
				BYTE*	pBits = NULL;
				pBits = new BYTE[sizeof(BYTE) * 3 * dwWidth * dwHeight];
				if( pBits )
				{
					// Bit data
					if( fread( pBits, sizeof(BYTE), 3 * dwWidth * dwHeight, fp ) == 3 * dwWidth * dwHeight )
					{
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

						HDC	hDC = ::GetDC( GetSafeHwnd() );
						hBitmap = CreateDIBitmap( hDC, &bh.bmiHeader, CBM_INIT, pBits, &bh, DIB_RGB_COLORS );
						delete [] pBits;
						::ReleaseDC( GetSafeHwnd(), hDC );
					}
				}
			}

			fclose( fp );
		}	
	}

	if( nIndex == -1 )
	{
		// Add a item to a list box
		m_vecTextureInfos.push_back( Infos );
		m_vecBitmaps.push_back( hBitmap );
		AddString( szFilename );
	}
	else
	{
		// Insert a item to a list box
		m_vecTextureInfos[nIndex] = Infos;
		DeleteObject( m_vecBitmaps[nIndex] );
		m_vecBitmaps[nIndex] = hBitmap;
		DeleteString( nIndex );
		InsertString( nIndex, szFilename );
	}
}


void CToolTexPaintListBox::RemoveListItem( int nIndex )
{
	vector<HBITMAP>::iterator it = m_vecBitmaps.begin();
	for( int i = 0; i < nIndex; i++ ) it++;
	
	if( m_vecBitmaps[nIndex] )
		DeleteObject( m_vecBitmaps[nIndex] );
	m_vecBitmaps.erase( it );

	vector<TextureInfos>::iterator it2 = m_vecTextureInfos.begin();
	for( int i = 0; i < nIndex; i++ ) it2++;
	m_vecTextureInfos.erase( it2 );

	DeleteString( nIndex );
}


void CToolTexPaintListBox::RemoveAllListItems()
{
	for( int i = 0; i < m_vecBitmaps.size(); i++ )
	{
		if( m_vecBitmaps[i] )
			DeleteObject( m_vecBitmaps[i] );
	}
	m_vecBitmaps.erase( m_vecBitmaps.begin(), m_vecBitmaps.end() );

	m_vecTextureInfos.erase( m_vecTextureInfos.begin(), m_vecTextureInfos.end() );

	ResetContent();
}


void CToolTexPaintListBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	int nIndex = GetCurSel();
	if( m_nCurSelTexture != nIndex )
	{
		m_nCurSelTexture = nIndex;
		g_pMyNKApp->SetCurSelTexture( nIndex );
	}

	Invalidate();
}
