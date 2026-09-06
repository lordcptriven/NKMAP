//-----------------------------------------------------------------------------
// File: IndexedTexture.cpp
//
// Desc: Functions to manage textures, including creating (loading from a
//       file), restoring lost surfaces, invalidating, and destroying.
//
//       Note: the implementation of these fucntions maintain an internal list
//       of loaded textures. After creation, individual textures are referenced
//       via their ASCII names.
//
// Copyright . All rights reserved
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <tchar.h>
#include <stdio.h>
#include "IndexedTexture.h"
#include "D3DUtil.h"

BOOL	g_bWXY_FORMAT = TRUE;
char	fileName[256];

IndexedTexture::IndexedTexture()
{
	m_TexturePath[0] = '\0';
	m_TextureList = NULL;

	m_Size = 0;
	m_Idx = 0;
}


IndexedTexture::IndexedTexture(int size)
{
	m_TexturePath[0] = '\0';
	m_TexturePath2 = NULL;
	m_TexturePath3 = NULL;
	m_TextureList = NULL;

	SetSize(size);
}


IndexedTexture::~IndexedTexture()
{
	if (m_TexturePath2)
		delete[] m_TexturePath2;
	if (m_TexturePath3)
		delete[] m_TexturePath3;

	InvalidateAllTextures();
	ReleaseTextureList();
}



//-----------------------------------------------------------------------------
// Name: CD3DTextureManager
// Desc: Class used to automatically construct and destruct the static
//       texture engine class.
//-----------------------------------------------------------------------------
/*class CD3DTextureManager
{
public:
    CD3DTextureManager() {}
    ~CD3DTextureManager() { if( g_ptcTextureList ) delete g_ptcTextureList; }
};

// Global instance
CD3DTextureManager g_StaticTextureEngine;*/




//-----------------------------------------------------------------------------
// Name: struct TEXTURESEARCHINFO
// Desc: Structure used to search for texture formats
//-----------------------------------------------------------------------------
struct TEXTURESEARCHINFO
{
    DWORD dwDesiredBPP;   // Input for texture format search
	BOOL  bSetAlpha;
    BOOL  bUseAlpha;
    BOOL  bUsePalette;
    BOOL  bFoundGoodFormat;

    DDPIXELFORMAT* pddpf; // Output of texture format search
};




//-----------------------------------------------------------------------------
// Name: TextureSearchCallback()
// Desc: Enumeration callback routine to find a best-matching texture format.
//       The param data is the DDPIXELFORMAT of the best-so-far matching
//       texture. Note: the desired BPP is passed in the dwSize field, and the
//       default BPP is passed in the dwFlags field.
//-----------------------------------------------------------------------------
static HRESULT CALLBACK TextureSearchCallback( DDPIXELFORMAT* pddpf,
                                               VOID* param )
{
    if( NULL==pddpf || NULL==param )
        return DDENUMRET_OK;

    TEXTURESEARCHINFO* ptsi = (TEXTURESEARCHINFO*)param;

    // Skip any funky modes
    if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
        return DDENUMRET_OK;

    // Check for palettized formats
    if( ptsi->bUsePalette )
    {
        if( !( pddpf->dwFlags & DDPF_PALETTEINDEXED8 ) )
            return DDENUMRET_OK;

        // Accept the first 8-bit palettized format we get
        memcpy( ptsi->pddpf, pddpf, sizeof(DDPIXELFORMAT) );
        ptsi->bFoundGoodFormat = TRUE;
        return DDENUMRET_CANCEL;
    }

    // Else, skip any paletized formats (all modes under 16bpp)
    if( pddpf->dwRGBBitCount < 16 )
        return DDENUMRET_OK;

    // Skip any FourCC formats
    if( pddpf->dwFourCC != 0 )
        return DDENUMRET_OK;

    // Skip any ARGB 4444 formats (which are best used for pre-authored
    // content designed speciafically for an ARGB 4444 format).
    if( pddpf->dwRGBAlphaBitMask == 0x0000f000 )
        return DDENUMRET_OK;

	if( ( (ptsi->bUseAlpha) && (pddpf->dwRGBBitCount == 16) ) ) {//||
			//( (!(ptsi->bSetAlpha) && ptsi->bUseAlpha) && pddpf->dwRGBAlphaBitMask == 0x00008000 && pddpf->dwRBitMask == 0x00007c00 && 
			//	pddpf->dwGBitMask == 0x000003e0 && pddpf->dwBBitMask == 0x0000001f) ) {
		pddpf->dwRGBAlphaBitMask = 0x0000f000;
		pddpf->dwRBitMask = 0x00000f00;
		pddpf->dwGBitMask = 0x000000f0;
		pddpf->dwBBitMask = 0x0000000f;
		pddpf->dwFlags |= DDPF_ALPHAPIXELS;
	}

    // Make sure current alpha format agrees with requested format type
    if( (ptsi->bUseAlpha==TRUE) && !(pddpf->dwFlags&DDPF_ALPHAPIXELS) )
        return DDENUMRET_OK;
    if( (ptsi->bUseAlpha==FALSE) && (pddpf->dwFlags&DDPF_ALPHAPIXELS) )
        return DDENUMRET_OK;

    // Check if we found a good match
    if( pddpf->dwRGBBitCount == ptsi->dwDesiredBPP )
    {
        memcpy( ptsi->pddpf, pddpf, sizeof(DDPIXELFORMAT) );
        ptsi->bFoundGoodFormat = TRUE;
        return DDENUMRET_CANCEL;
    }

    return DDENUMRET_OK;
}




//-----------------------------------------------------------------------------
// Name: FindTexture()
// Desc: Searches the internal list of textures for a texture specified by
//       its name. Returns the structure associated with that texture.
//-----------------------------------------------------------------------------
int IndexedTexture::FindTexture( TCHAR* strTextureName )
{
	for (int i = 0; i < m_Idx; i++) {
		if (m_TextureList[i]) {
			if( !lstrcmpi( strTextureName, m_TextureList[i]->m_strName ) )
				return i;
		}
	}

	return -1;

    /*TextureContainer* ptcTexture = g_ptcTextureList;

    while( ptcTexture )
    {
        if( !lstrcmpi( strTextureName, ptcTexture->m_strName ) )
            return ptcTexture;
        ptcTexture = ptcTexture->m_pNext;
    }

    return NULL;*/
}




//-----------------------------------------------------------------------------
// Name: TextureContainer()
// Desc: Constructor for a texture object
//-----------------------------------------------------------------------------
TextureContainer::TextureContainer( TCHAR* strName, DWORD dwStage,
                                    DWORD dwFlags, DWORD transAlpha )
{
    lstrcpy( m_strName, strName );
    m_dwWidth     = 0;
    m_dwHeight    = 0;
    m_dwStage     = dwStage;
    m_dwBPP       = 0;
    m_dwFlags     = dwFlags;
    m_bHasAlpha   = 0;

    m_pddsSurface = NULL;
    m_hbmBitmap   = NULL;
    m_pRGBAData   = NULL;
	m_ppNTXData = NULL;

	m_transAlpha  = transAlpha;
	m_dwMipmapCount = 4;

    // Add the texture to the head of the global texture list
    //m_pNext = g_ptcTextureList;
    //g_ptcTextureList = this;

}




//-----------------------------------------------------------------------------
// Name: ~TextureContainer()
// Desc: Destructs the contents of the texture container
//-----------------------------------------------------------------------------
TextureContainer::~TextureContainer()
{
    SAFE_RELEASE( m_pddsSurface );
    SAFE_DELETE_ARRAY( m_pRGBAData );	// DELETE가 아니라 DELETE_ARRAY가 아닐까??
    DeleteObject( m_hbmBitmap );

	if( m_ppNTXData )
	{
		for( DWORD i = 0; i < m_dwMipmapCount; i++ )
			SAFE_DELETE_ARRAY( m_ppNTXData[i] );
		delete [] m_ppNTXData;
	}

    // Remove the texture container from the global list
    /*if( g_ptcTextureList == this )
        g_ptcTextureList = m_pNext;
    else
    {
        for( TextureContainer* ptc=g_ptcTextureList; ptc; ptc=ptc->m_pNext )
            if( ptc->m_pNext == this )
                ptc->m_pNext = m_pNext;
    }*/
}




//-----------------------------------------------------------------------------
// Name: LoadImageData()
// Desc: Loads the texture map's image data
//-----------------------------------------------------------------------------
HRESULT TextureContainer::LoadImageData(TCHAR* strTexturePath, TCHAR* strTexturePath2, TCHAR* strTexturePath3)
{
    TCHAR* strExtension;
    TCHAR  strPathname[256];
    FILE*  file;

    // Check the executable's resource. If it's there, we're done!
    m_hbmBitmap = (HBITMAP)LoadImage( GetModuleHandle(NULL), m_strName,
                                      IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
    if( m_hbmBitmap )
        return S_OK;

    // First check if the file exists in the global texture path
    lstrcpy( strPathname, strTexturePath );
    lstrcat( strPathname, m_strName );
    if( NULL == ( file = fopen( strPathname, "rb" ) ) )
    {
        // Then check if the file exists in the DirectX SDK media path
        //lstrcpy( strPathname, D3DUtil_GetDXSDKMediaPath() );
        //lstrcat( strPathname, m_strName );
		if (strTexturePath2) {
			lstrcpy( strPathname, strTexturePath2 );
			lstrcat( strPathname, m_strName );
			if( NULL == ( file = fopen( strPathname, "rb" ) ) ) {
				if (strTexturePath3) {
					lstrcpy( strPathname, strTexturePath3 );
					lstrcat( strPathname, m_strName );
					if( NULL == ( file = fopen( strPathname, "rb" ) ) ) {
						return DDERR_NOTFOUND;
					}
				} else {
					return DDERR_NOTFOUND;
				}
			}
		} else {
			return DDERR_NOTFOUND;
		}
    }
    fclose( file );

    // Get the filename extension
    if( NULL == ( strExtension = _tcsrchr( m_strName, _T('.') ) ) )
        return DDERR_UNSUPPORTED;

	// 텍스처 파일의 경로와 이름을 보관하여 restore를 할때 사용한다.
	lstrcpy(m_strPathName, strPathname);

	return LoadImageData();

	/*
	return S_OK;

    // Load bitmap files
    if( !lstrcmpi( strExtension, _T(".bmp") ) )
        return LoadBitmapFile( strPathname );

    // Load targa files
    if( !lstrcmpi( strExtension, _T(".tga") ) )
	{
		m_dwMipmapCount = 1;
        return LoadTargaFile( strPathname );
	}

    // Can add code here to check for other file formats before failing
    return DDERR_UNSUPPORTED;
	*/
}


HRESULT TextureContainer::LoadImageData()
{
	TCHAR* strExtension;

	// Get the filename extension
    if( NULL == ( strExtension = _tcsrchr( m_strPathName, _T('.') ) ) )
        return DDERR_UNSUPPORTED;

	// Load bitmap files
    if( !lstrcmpi( strExtension, _T(".bmp") ) )
       return LoadBitmapFile( m_strPathName );

    // Load targa files
    if( !lstrcmpi( strExtension, _T(".tga") ) ) {
		// tga는 밉맵을 사용하지 않는다.
		m_dwMipmapCount = 1;
        return LoadTargaFile( m_strPathName );
	}

	// Load NTX files
	if( !lstrcmpi( strExtension, _T(".ntx") ) )
		return LoadNTXFile( m_strPathName );

	if( !lstrcmpi( strExtension, _T(".wxy") ) )
		return LoadWxyFile( m_strPathName );

	if( !lstrcmpi( strExtension, _T(".sek") ) )
		m_dwMipmapCount = 1;
		return LoadSekFile( m_strPathName );

    // Can add code here to check for other file formats before failing
    return DDERR_UNSUPPORTED;
}



//-----------------------------------------------------------------------------
// Name: LoadBitmapFile()
// Desc: Loads data from a .bmp file, and stores it in a bitmap structure.
//-----------------------------------------------------------------------------
HRESULT TextureContainer::LoadBitmapFile2( TCHAR* strPathname )
{
    // Try to load the bitmap as a file
    m_hbmBitmap = (HBITMAP)LoadImage( NULL, strPathname, IMAGE_BITMAP, 0, 0,
                                      LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	if( m_hbmBitmap )
		return S_OK;
   
    return DDERR_NOTFOUND;
}

HRESULT TextureContainer::LoadBitmapFile( TCHAR* strPathname )
{
	LPVOID lpBits;
	FILE *fp;

	if( (fp = fopen(strPathname, "rb")) == NULL )
		return DDERR_NOTFOUND;
	
	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (fread((LPSTR)&bmfHeader, sizeof(bmfHeader), 1, fp) <= 0)
		return E_FAIL;

		
	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return E_FAIL;

	BITMAPINFO *pbmInfo;
	pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, 
				sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256 );
	if (pbmInfo == NULL)
		return NULL;
		
	fread(pbmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256, 1, fp );
		
	BITMAPINFO &bmInfo = *pbmInfo ;
		
	m_hbmBitmap = CreateDIBSection( NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, 
						NULL, 0 );

	fseek(fp, bmfHeader.bfOffBits, SEEK_SET);

	int bytes = (bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount + 15) / 16 * 2;
	bytes *= bmInfo.bmiHeader.biHeight;
	fread(lpBits, bytes, 1, fp);

	::GlobalFree(pbmInfo);

	fclose(fp);

	return S_OK;
}


HRESULT TextureContainer::LoadWxyFile( TCHAR* strPathname )
{
	LPVOID lpBits;
	FILE *fp;
	if( (fp = fopen(strPathname, "rb")) == NULL )
		return DDERR_NOTFOUND;
	
	BITMAPFILEHEADER bmfHeader;

	char dummy[10];
	// Read Dummy header
	if (fread(dummy, 6, 1, fp) <= 0)
		return E_FAIL;

	// Read file header
	if (fread((LPSTR)&bmfHeader, sizeof(bmfHeader), 1, fp) <= 0)
		return E_FAIL;
	
	// File type should be 'BM'
	//if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
	//	return E_FAIL;

	BITMAPINFO *pbmInfo;
	pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, 
				sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256 );
	if (pbmInfo == NULL)
		return NULL;
	
	fread(pbmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256, 1, fp );
	
	BITMAPINFO &bmInfo = *pbmInfo ;
	
	m_hbmBitmap = CreateDIBSection( NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, 
						NULL, 0 );

	fseek(fp, bmfHeader.bfOffBits+6, SEEK_SET);
	int bytes = (bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount + 15) / 16 * 2;
	bytes *= bmInfo.bmiHeader.biHeight;
	fread(lpBits, bytes, 1, fp);

	::GlobalFree(pbmInfo);

	fclose(fp);

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: LoadTargaFile()
// Desc: Loads RGBA data from a .tga file, and stores it in allocated memory
//       for the specified texture container
//-----------------------------------------------------------------------------
HRESULT TextureContainer::LoadTargaFile( TCHAR* strPathname )
{
    FILE* file = fopen( strPathname, "rb" );
    if( NULL == file )
        return E_FAIL;

    struct TargaHeader
    {
        BYTE IDLength;
        BYTE ColormapType;
        BYTE ImageType;
        BYTE ColormapSpecification[5];
        WORD XOrigin;
        WORD YOrigin;
        WORD ImageWidth;
        WORD ImageHeight;
        BYTE PixelDepth;
        BYTE ImageDescriptor;
    } tga;

    fread( &tga, sizeof(TargaHeader), 1, file );

    // Only true color, non-mapped images are supported
    if( ( 0 != tga.ColormapType ) || 
        ( tga.ImageType != 10 && tga.ImageType != 2 ) )
    {
        fclose( file );
        return E_FAIL;
    }

    // Skip the ID field. The first byte of the header is the length of this field
    if( tga.IDLength )
        fseek( file, tga.IDLength, SEEK_CUR );

    m_dwWidth   = tga.ImageWidth;
    m_dwHeight  = tga.ImageHeight;
    m_dwBPP     = tga.PixelDepth;
    m_pRGBAData = new DWORD[m_dwWidth*m_dwHeight];

    if( m_pRGBAData == NULL )
    {
        fclose(file);
        return E_FAIL;
    }

    for( DWORD y=0; y<m_dwHeight; y++ )
    {
        DWORD dwOffset = y*m_dwWidth;

        if( 0 == ( tga.ImageDescriptor & 0x0010 ) )
            dwOffset = (m_dwHeight-y-1)*m_dwWidth;

        for( DWORD x=0; x<m_dwWidth; x )
        {
            if( tga.ImageType == 10 )
            {
                BYTE PacketInfo = getc( file );
                WORD PacketType = 0x80 & PacketInfo;
                WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

                if( PacketType )
                {
                    DWORD b = getc( file );
                    DWORD g = getc( file );
                    DWORD r = getc( file );
                    DWORD a = 0xff;
                    if( m_dwBPP == 32 )
                        a = getc( file );

                    while( PixelCount-- )
                    {
                        m_pRGBAData[dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
                        x++;
                    }
                }
                else
                {
                    while( PixelCount-- )
                    {
                        BYTE b = getc( file );
                        BYTE g = getc( file );
                        BYTE r = getc( file );
                        BYTE a = 0xff;
                        if( m_dwBPP == 32 )
                            a = getc( file );

                        m_pRGBAData[dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
                        x++;
                    }
                }
            }
            else
            {
                BYTE b = getc( file );
                BYTE g = getc( file );
                BYTE r = getc( file );
                BYTE a = 0xff;
                if( m_dwBPP == 32 )
                    a = getc( file );

                m_pRGBAData[dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
                x++;
            }
        }
    }

    fclose( file );

	// sek 파일은 알파값을 가지고 있다.
	m_bHasAlpha = TRUE;

	/*
    // Check for alpha content
    for( DWORD i=0; i<(m_dwWidth*m_dwHeight); i++ )
    {
        if( (m_pRGBAData[i] & 0x000000ff) != 0xff )
        {
            m_bHasAlpha = TRUE;
            break;
        }
    }
	*/
    
    return S_OK;
}


HRESULT TextureContainer::LoadSekFile( TCHAR* strPathname )
{
    FILE* file = fopen( strPathname, "rb" );
    if( NULL == file )
        return E_FAIL;

	char dummy[10];
	fread( dummy, 8, 1, file );

    struct TargaHeader
    {
        BYTE IDLength;
        BYTE ColormapType;
        BYTE ImageType;
        BYTE ColormapSpecification[5];
        WORD XOrigin;
        WORD YOrigin;
        WORD ImageWidth;
        WORD ImageHeight;
        BYTE PixelDepth;
        BYTE ImageDescriptor;
    } tga;

    fread( &tga, sizeof(TargaHeader), 1, file );

    // Only true color, non-mapped images are supported
    if( ( 0 != tga.ColormapType ) || 
        ( tga.ImageType != 10 && tga.ImageType != 2 ) )
    {
        fclose( file );
        return E_FAIL;
    }

    // Skip the ID field. The first byte of the header is the length of this field
    if( tga.IDLength )
        fseek( file, tga.IDLength+8, SEEK_CUR );

    m_dwWidth   = tga.ImageWidth;
    m_dwHeight  = tga.ImageHeight;
    m_dwBPP     = tga.PixelDepth;
    m_pRGBAData = new DWORD[m_dwWidth*m_dwHeight];

    if( m_pRGBAData == NULL )
    {
        fclose(file);
        return E_FAIL;
    }

    for( DWORD y=0; y<m_dwHeight; y++ )
    {
        DWORD dwOffset = y*m_dwWidth;

        if( 0 == ( tga.ImageDescriptor & 0x0010 ) )
            dwOffset = (m_dwHeight-y-1)*m_dwWidth;

        for( DWORD x=0; x<m_dwWidth; x )
        {
            if( tga.ImageType == 10 )
            {
                BYTE PacketInfo = getc( file );
                WORD PacketType = 0x80 & PacketInfo;
                WORD PixelCount = ( 0x007f & PacketInfo ) + 1;

                if( PacketType )
                {
                    DWORD b = getc( file );
                    DWORD g = getc( file );
                    DWORD r = getc( file );
                    DWORD a = 0xff;
                    if( m_dwBPP == 32 )
                        a = getc( file );

                    while( PixelCount-- )
                    {
                        m_pRGBAData[dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
                        x++;
                    }
                }
                else
                {
                    while( PixelCount-- )
                    {
                        BYTE b = getc( file );
                        BYTE g = getc( file );
                        BYTE r = getc( file );
                        BYTE a = 0xff;
                        if( m_dwBPP == 32 )
                            a = getc( file );

                        m_pRGBAData[dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
                        x++;
                    }
                }
            }
            else
            {
                BYTE b = getc( file );
                BYTE g = getc( file );
                BYTE r = getc( file );
                BYTE a = 0xff;
                if( m_dwBPP == 32 )
                    a = getc( file );

                m_pRGBAData[dwOffset+x] = (r<<24L)+(g<<16L)+(b<<8L)+(a);
                x++;
            }
        }
    }

    fclose( file );

	// sek 파일은 알파값을 가지고 있다.
	m_bHasAlpha = TRUE;

	/*
    // Check for alpha content
    for( DWORD i=0; i<(m_dwWidth*m_dwHeight); i++ )
    {
        if( (m_pRGBAData[i] & 0x000000ff) != 0xff )
        {
            m_bHasAlpha = TRUE;
            break;
        }
    }*/
    
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: LoadNTXFile()
// Desc: Loads data from a .ntx file, and stores it
//-----------------------------------------------------------------------------
HRESULT TextureContainer::LoadNTXFile( TCHAR* strPathname )
{
	FILE*	fp = fopen( strPathname, "rb" );
	if( fp == NULL )
		return E_FAIL;

	// Magic
	DWORD	dwMagic;
	fread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( 'N' << 24 | 'T' << 16 | 'X' << 8 | ' ' ) )
	{
		fclose( fp );
		return E_FAIL;
	}

	// Width, Height, Mipmap Count
	fread( &m_dwWidth, sizeof(DWORD), 1, fp );
	fread( &m_dwHeight, sizeof(DWORD), 1, fp );
	fread( &m_dwBPP, sizeof(DWORD), 1, fp );
	fread( &m_dwMipmapCount, sizeof(DWORD), 1, fp );

	DWORD	i, j;
	DWORD	dwWidth, dwHeight;

	// Allocate memory
	m_ppNTXData = new BYTE*[m_dwMipmapCount];
	if( m_ppNTXData == NULL )
	{
		fclose( fp );
		return E_FAIL;
	}

	for( i = 0; i < m_dwMipmapCount; i++ )
	{
		dwWidth = m_dwWidth >> i;
		dwHeight = m_dwHeight >> i;
		if( dwWidth < 1 ) dwWidth = 1;
		if( dwHeight < 1 ) dwHeight = 1;

		m_ppNTXData[i] = new BYTE[sizeof(BYTE) * 3 * dwWidth * dwHeight];
		if( m_ppNTXData[i] == NULL )
		{			
			for( j = 0; j < i; j++ )
				delete [] m_ppNTXData[j];
			delete [] m_ppNTXData;

			fclose( fp );
			return E_FAIL;
		}
	}

	// Bit data
	for( i = 0; i < m_dwMipmapCount; i++ )
	{
		dwWidth = m_dwWidth >> i;
		dwHeight = m_dwHeight >> i;
		if( dwWidth < 1 ) dwWidth = 1;
		if( dwHeight < 1 ) dwHeight = 1;

		if( fread( m_ppNTXData[i], sizeof(BYTE), 3 * dwWidth * dwHeight, fp ) < 3 * dwWidth * dwHeight )
		{
			for( j = 0; j < m_dwMipmapCount; j++ )
				delete [] m_ppNTXData[j];
			delete [] m_ppNTXData;

			fclose( fp );
			return E_FAIL;
		}
	}

	fclose( fp );

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: Restore()
// Desc: Rebuilds the texture surface using the new device.
//-----------------------------------------------------------------------------
HRESULT TextureContainer::Restore( LPDIRECT3DDEVICE7 pd3dDevice)
{
    // Release any previously created objects
    SAFE_RELEASE( m_pddsSurface );

    // Check params
    if( NULL == pd3dDevice )
        return DDERR_INVALIDPARAMS;

    // Get the device caps
    D3DDEVICEDESC7 ddDesc;
    if( FAILED( pd3dDevice->GetCaps( &ddDesc) ) )
        return E_FAIL;

    // Setup the new surface desc
    DDSURFACEDESC2 ddsd;
	if (m_dwMipmapCount > 1) {
		D3DUtil_InitSurfaceDesc( ddsd );
		ddsd.dwFlags         = DDSD_CAPS|DDSD_MIPMAPCOUNT|DDSD_HEIGHT|DDSD_WIDTH|
							   DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE|DDSCAPS_MIPMAP|DDSCAPS_COMPLEX;
		ddsd.dwMipMapCount   = m_dwMipmapCount;
		ddsd.dwTextureStage  = m_dwStage;
		ddsd.dwWidth         = m_dwWidth;
		ddsd.dwHeight        = m_dwHeight;
	} else {
		D3DUtil_InitSurfaceDesc( ddsd );
		ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|
							   DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		ddsd.dwTextureStage  = m_dwStage;
		ddsd.dwWidth         = m_dwWidth;
		ddsd.dwHeight        = m_dwHeight;
	}

    // Turn on texture management for hardware devices
    if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
        ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
    else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
        ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
    else
        ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

    // Adjust width and height to be powers of 2, if the device requires it
    if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2 )
    {
        for( ddsd.dwWidth=1;  m_dwWidth>ddsd.dwWidth;   ddsd.dwWidth<<=1 );
        for( ddsd.dwHeight=1; m_dwHeight>ddsd.dwHeight; ddsd.dwHeight<<=1 );
    }

    // Limit max texture sizes, if the driver can't handle large textures
    DWORD dwMaxWidth  = ddDesc.dwMaxTextureWidth;
    DWORD dwMaxHeight = ddDesc.dwMaxTextureHeight;
    ddsd.dwWidth  = min( ddsd.dwWidth,  ( dwMaxWidth  ? dwMaxWidth  : 256 ) );
    ddsd.dwHeight = min( ddsd.dwHeight, ( dwMaxHeight ? dwMaxHeight : 256 ) );

    // Make the texture square, if the driver requires it
    if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
    {
        if( ddsd.dwWidth > ddsd.dwHeight ) ddsd.dwHeight = ddsd.dwWidth;
        else                               ddsd.dwWidth  = ddsd.dwHeight;
    }

    // Setup the structure to be used for texture enumration.
    TEXTURESEARCHINFO tsi;
    tsi.bFoundGoodFormat = FALSE;
    tsi.pddpf            = &ddsd.ddpfPixelFormat;
    tsi.dwDesiredBPP     = m_dwBPP;
    //tsi.bUsePalette      = ( m_dwBPP <= 8 );
	tsi.bUsePalette      = FALSE;
    tsi.bUseAlpha        = m_bHasAlpha;
	tsi.bSetAlpha		 = (m_dwFlags & D3DTEXTR_TRANSPARENTALPHA);
	if (tsi.bSetAlpha)
		tsi.bUseAlpha =	TRUE;
    if( m_dwFlags & D3DTEXTR_16BITSPERPIXEL )
        tsi.dwDesiredBPP = 16;
    else if( m_dwFlags & D3DTEXTR_32BITSPERPIXEL )
        tsi.dwDesiredBPP = 32;

    if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
    {
        if( tsi.bUsePalette )
        {
            if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
            {
                tsi.bUseAlpha   = TRUE;
                tsi.bUsePalette = TRUE;
            }
            else
            {
                tsi.bUseAlpha   = TRUE;
                tsi.bUsePalette = FALSE;
            }
        }
    }

    // Enumerate the texture formats, and find the closest device-supported
    // texture pixel format
    pd3dDevice->EnumTextureFormats( TextureSearchCallback, &tsi );

    // If we couldn't find a format, let's try a default format
    if( FALSE == tsi.bFoundGoodFormat )
    {
        tsi.bUsePalette  = FALSE;
        tsi.dwDesiredBPP = 16;
        pd3dDevice->EnumTextureFormats( TextureSearchCallback, &tsi );

        // If we still fail, we cannot create this texture
        if( FALSE == tsi.bFoundGoodFormat )
            return E_FAIL;
    }

    // Get the DirectDraw interface for creating surfaces
    LPDIRECTDRAW7        pDD;
    LPDIRECTDRAWSURFACE7 pddsRender;
    pd3dDevice->GetRenderTarget( &pddsRender );
    pddsRender->GetDDInterface( (VOID**)&pDD );
    pddsRender->Release();

    // Create a new surface for the texture
    HRESULT hr = pDD->CreateSurface( &ddsd, &m_pddsSurface, NULL );

    // Done with DDraw
    pDD->Release();

    if( FAILED(hr) )
        return hr;

	// 실제 데이타 로딩.
	if (!m_hbmBitmap && !m_pRGBAData && !m_ppNTXData)
		LoadImageData();

	// For bitmap-based textures, copy the bitmap image.
	if( m_hbmBitmap ) {
        hr = CopyBitmapToSurface();
		DeleteObject(m_hbmBitmap);
		m_hbmBitmap = NULL;
		return hr;
	}	

    if( m_pRGBAData ) {
		hr = CopyRGBADataToSurface();
		SAFE_DELETE_ARRAY( m_pRGBAData );		// DELETE가 아니라 DELETE_ARRAY가 아닐까??
        return hr;
	}

	if( m_ppNTXData ) {
		hr = CopyNTXDataToSurface();
		for( DWORD i = 0; i < m_dwMipmapCount; i++ )
			SAFE_DELETE_ARRAY( m_ppNTXData[i] );
		SAFE_DELETE_ARRAY( m_ppNTXData );
		return hr;
	}

    // At this point, code can be added to handle other file formats (such as
    // .dds files, .jpg files, etc.).
    return S_OK;
}


HRESULT TextureContainer::SetAlpha(DWORD transAlpha)
{
	if ( m_dwFlags & D3DTEXTR_TRANSPARENTALPHA ) {
		DDSURFACEDESC2 ddsd;
		ddsd.dwSize = sizeof(ddsd);
		while( m_pddsSurface->Lock( NULL, &ddsd, 0, NULL ) ==
											DDERR_WASSTILLDRAWING );

		DWORD dwAlphaMask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;
		DWORD dwRGBMask   = ( ddsd.ddpfPixelFormat.dwRBitMask |
								ddsd.ddpfPixelFormat.dwGBitMask |
								ddsd.ddpfPixelFormat.dwBBitMask );

		if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 &&
				ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xf000 ) {
			dwAlphaMask = (m_transAlpha >> 4) << 12;
		} else if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 &&
				ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff000000 ) {
			dwAlphaMask = m_transAlpha << 24;
		} else {
			return E_FAIL;
		}

		m_transAlpha = transAlpha;

		// Add an opaque alpha value to each non-colorkeyed pixel
		for( DWORD y=0; y<ddsd.dwHeight; y++ )
		{
			WORD*  p16 =  (WORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);
			DWORD* p32 = (DWORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);

			for( DWORD x=0; x<ddsd.dwWidth; x++ )
			{
				if( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 )
				{
					*p16 &= dwRGBMask;
					*p16 |= dwAlphaMask;

					p16++;
				}
				if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
				{
					*p32 &= dwRGBMask;
					*p32 |= dwAlphaMask;

					p32++;
				}
			}
		}
		m_pddsSurface->Unlock( NULL );
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CopyBitmapToSurface()
// Desc: Copies the image of a bitmap into a surface
//-----------------------------------------------------------------------------
HRESULT TextureContainer::CopyBitmapToSurface()
{
	DWORD i;
    // Get a DDraw object to create a temporary surface
    LPDIRECTDRAW7 pDD;
    m_pddsSurface->GetDDInterface( (VOID**)&pDD );

    // Get the bitmap structure (to extract width, height, and bpp)
    BITMAP bm;
    GetObject( m_hbmBitmap, sizeof(BITMAP), &bm );

    // Setup the new surface desc
    DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    m_pddsSurface->GetSurfaceDesc( &ddsd );
    ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
                            DDSD_TEXTURESTAGE;
    ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
    ddsd.ddsCaps.dwCaps2  = 0L;
    ddsd.dwWidth          = bm.bmWidth;
    ddsd.dwHeight         = bm.bmHeight;

	// 일단 1555 포맷으로 바꿔서 Temp 서피스를 만든다.
	BOOL bSet4444 = FALSE;
	if (ddsd.ddpfPixelFormat.dwRGBBitCount == 16 &&
			ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xf000 &&
			(m_dwFlags & D3DTEXTR_TRANSPARENTALPHA) ) {
		ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0x8000;
		ddsd.ddpfPixelFormat.dwRBitMask = 0x00007c00;
		ddsd.ddpfPixelFormat.dwGBitMask = 0x000003e0;
		ddsd.ddpfPixelFormat.dwBBitMask = 0x0000001f;
		bSet4444 = TRUE;
	}

    // Create a new surface for the texture
    LPDIRECTDRAWSURFACE7 pddsTempSurface[4];
    HRESULT hr;
    for (i=0; i < m_dwMipmapCount; i++) {
		if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface[i], NULL ) ) )
		{
			pDD->Release();
			return hr;
		}

		ddsd.dwWidth /= 2;			// dwWidth와 dwHeight가 0이 되지 않게 한다.
		ddsd.dwHeight /= 2;
		if (ddsd.dwWidth < 1)
			ddsd.dwWidth = 1;
		if (ddsd.dwHeight < 1)
			ddsd.dwHeight = 1;
	}

    // Get a DC for the bitmap
    HDC hdcBitmap = CreateCompatibleDC( NULL );

    if( NULL == hdcBitmap )
    {
        for (i=0; i < m_dwMipmapCount; i++) {
			pddsTempSurface[i]->Release();
		}
        pDD->Release();
        return hr;
    }
    SelectObject( hdcBitmap, m_hbmBitmap );
	
    // Handle palettized textures. Need to attach a palette
    if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 )
    {
        LPDIRECTDRAWPALETTE  pPalette;
        DWORD dwPaletteFlags = DDPCAPS_8BIT|DDPCAPS_ALLOW256;
        DWORD pe[256];
        WORD  wNumColors     = GetDIBColorTable( hdcBitmap, 0, 256, (RGBQUAD*)pe );

        // Create the color table
        for( i=0; i<wNumColors; i++ )
        {
            pe[i] = RGB( GetBValue(pe[i]), GetGValue(pe[i]), GetRValue(pe[i]) );

            // Handle textures with transparent pixels
            if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
            {
                // Set alpha for opaque pixels
                if( m_dwFlags & D3DTEXTR_TRANSPARENTBLACK )
                {
                    if( pe[i] != 0x00000000 )
                        pe[i] |= 0xff000000;
                }
                else if( m_dwFlags & D3DTEXTR_TRANSPARENTWHITE )
                {
                    if( pe[i] != 0x00ffffff )
                        pe[i] |= 0xff000000;
                }
                else if( m_dwFlags & D3DTEXTR_TRANSPARENTALPHA )
                {
                    pe[i] |= (m_transAlpha << 24);
                }
            }
        }
        // Add DDPCAPS_ALPHA flag for textures with transparent pixels
        if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
            dwPaletteFlags |= DDPCAPS_ALPHA;

        // Create & attach a palette
        pDD->CreatePalette( dwPaletteFlags, (PALETTEENTRY*)pe, &pPalette, NULL );
        for (i=0; i < m_dwMipmapCount; i++) {
			pddsTempSurface[i]->SetPalette( pPalette );
		}
        m_pddsSurface->SetPalette( pPalette );
        SAFE_RELEASE( pPalette );
    }

    // Copy the bitmap image to the surface.
    HDC hdcSurface[4];
	int width = bm.bmWidth;
	int height = bm.bmHeight;
	for (i=0; i < m_dwMipmapCount; i++) {
		if( SUCCEEDED( pddsTempSurface[i]->GetDC( &hdcSurface[i] ) ) )
		{
			SetStretchBltMode(hdcSurface[i], COLORONCOLOR);
			StretchBlt( hdcSurface[i], 0, 0, width, height, hdcBitmap, 0, 0, bm.bmWidth, bm.bmHeight, SRCPAINT );
			pddsTempSurface[i]->ReleaseDC( hdcSurface[i] );
		}
		width /= 2;
		height /= 2;
		if (width < 1)
			width = 1;
		if (height < 1)
			height = 1;	
	}

    DeleteDC( hdcBitmap );
	
    // Copy the temp surface to the real texture surface
	if (!bSet4444) {
		LPDIRECTDRAWSURFACE7 pddsDest = m_pddsSurface;
		for (i=0; i < m_dwMipmapCount; i++) {
			pddsDest->Blt( NULL, pddsTempSurface[i], NULL, DDBLT_WAIT, NULL );

			DDSCAPS2 ddsCaps;
			ddsCaps.dwCaps  = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
			ddsCaps.dwCaps2 = 0;
			ddsCaps.dwCaps3 = 0;
			ddsCaps.dwCaps4 = 0;
			if( SUCCEEDED( pddsDest->GetAttachedSurface( &ddsCaps, &pddsDest ) ) )
				pddsDest->Release();
		}
	}

    // For textures with real alpha (not palettized), set transparent bits
    if( ddsd.ddpfPixelFormat.dwRGBAlphaBitMask )
    {
        if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
        {
			DWORD mipmapCount = m_dwMipmapCount;

			LPDIRECTDRAWSURFACE7 pddsDest = m_pddsSurface;
			for (i=0; i < mipmapCount; i++) {
				// Lock the texture surface
				DDSURFACEDESC2 ddsd, ddsd_t;
				ddsd.dwSize = sizeof(ddsd);
				ddsd_t.dwSize = sizeof(ddsd_t);
				while( pddsDest->Lock( NULL, &ddsd, 0, NULL ) ==
					   DDERR_WASSTILLDRAWING );
				while( pddsTempSurface[i]->Lock( NULL, &ddsd_t, 0, NULL ) ==
					   DDERR_WASSTILLDRAWING );

				DWORD dwAlphaMask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;
				DWORD dwRGBMask   = ( ddsd.ddpfPixelFormat.dwRBitMask |
									  ddsd.ddpfPixelFormat.dwGBitMask |
									  ddsd.ddpfPixelFormat.dwBBitMask );
				DWORD dwColorkey  = 0x00000000; // Colorkey on black
				if( m_dwFlags & D3DTEXTR_TRANSPARENTWHITE )
					dwColorkey = dwRGBMask;     // Colorkey on white
            
				if ( m_dwFlags & D3DTEXTR_TRANSPARENTALPHA ) {
					dwColorkey = 0xffffffff;
					if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 &&
							ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xf000 ) {
						dwAlphaMask = (m_transAlpha >> 4) << 12;
					} else if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 &&
							ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xff000000 ) {
						dwAlphaMask = m_transAlpha << 24;
					}	
				}

		        // Add an opaque alpha value to each non-colorkeyed pixel
	            for( DWORD y=0; y<ddsd.dwHeight; y++ )
			    {
				    WORD*  p16 =  (WORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);
					WORD*  p16_t =  (WORD*)((BYTE*)ddsd_t.lpSurface + y*ddsd_t.lPitch);
					DWORD* p32 = (DWORD*)((BYTE*)ddsd.lpSurface + y*ddsd.lPitch);

					for( DWORD x=0; x<ddsd.dwWidth; x++ )
					{
						if( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 )
						{
							// 1555 로 저장했던 것을 4444 로 바꿔서 재저장한다.
							if (bSet4444) {
								WORD r = ((*p16_t & 0x7c00) >> 11) << 8;
								WORD g = ((*p16_t & 0x03e0) >> 6) << 4;
								WORD b = ((*p16_t & 0x001f) >> 1);
								*p16 = r | g | b;
							}

							if( ( *p16 &= dwRGBMask ) != dwColorkey )
								*p16 |= dwAlphaMask;
							p16++;
							p16_t++;
						}
						if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
						{
							if( ( *p32 &= dwRGBMask ) != dwColorkey )
								*p32 |= dwAlphaMask;
							p32++;
						}
					}
				}
				pddsDest->Unlock( NULL );
				pddsTempSurface[i]->Unlock( NULL );

				DDSCAPS2 ddsCaps;
				ddsCaps.dwCaps  = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
				ddsCaps.dwCaps2 = 0;
				ddsCaps.dwCaps3 = 0;
				ddsCaps.dwCaps4 = 0;
				if( SUCCEEDED( pddsDest->GetAttachedSurface( &ddsCaps, &pddsDest ) ) )
					pddsDest->Release();
			}
        }
    }

	for (i=0; i < m_dwMipmapCount; i++) {
	    pddsTempSurface[i]->Release();
	}

    pDD->Release();

    return S_OK;;
}




//-----------------------------------------------------------------------------
// Name: CopyRGBADataToSurface()
// Desc: Invalidates the current texture objects and rebuilds new ones
//       using the new device.
//-----------------------------------------------------------------------------
HRESULT TextureContainer::CopyRGBADataToSurface()
{
    // Get a DDraw object to create a temporary surface
    LPDIRECTDRAW7 pDD;
    m_pddsSurface->GetDDInterface( (VOID**)&pDD );

    // Setup the new surface desc
    DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    m_pddsSurface->GetSurfaceDesc( &ddsd );
    ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
                           DDSD_TEXTURESTAGE;
    ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
    ddsd.ddsCaps.dwCaps2 = 0L;
    ddsd.dwWidth         = m_dwWidth;
    ddsd.dwHeight        = m_dwHeight;

    // Create a new surface for the texture
    LPDIRECTDRAWSURFACE7 pddsTempSurface;
    HRESULT hr;
    if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface, NULL ) ) )
    {
        pDD->Release();
        return NULL;
    }

    while( pddsTempSurface->Lock( NULL, &ddsd, 0, 0 ) == DDERR_WASSTILLDRAWING );
    DWORD lPitch = ddsd.lPitch;
    BYTE* pBytes = (BYTE*)ddsd.lpSurface;

    DWORD dwRMask = ddsd.ddpfPixelFormat.dwRBitMask;
    DWORD dwGMask = ddsd.ddpfPixelFormat.dwGBitMask;
    DWORD dwBMask = ddsd.ddpfPixelFormat.dwBBitMask;
    DWORD dwAMask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;

    DWORD dwRShiftL = 8, dwRShiftR = 0;
    DWORD dwGShiftL = 8, dwGShiftR = 0;
    DWORD dwBShiftL = 8, dwBShiftR = 0;
    DWORD dwAShiftL = 8, dwAShiftR = 0;

    DWORD dwMask;
    for( dwMask=dwRMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwRShiftR++;
    for( ; dwMask; dwMask>>=1 ) dwRShiftL--;

    for( dwMask=dwGMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwGShiftR++;
    for( ; dwMask; dwMask>>=1 ) dwGShiftL--;

    for( dwMask=dwBMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwBShiftR++;
    for( ; dwMask; dwMask>>=1 ) dwBShiftL--;

    for( dwMask=dwAMask; dwMask && !(dwMask&0x1); dwMask>>=1 ) dwAShiftR++;
    for( ; dwMask; dwMask>>=1 ) dwAShiftL--;

    for( DWORD y=0; y<ddsd.dwHeight; y++ )
    {
        DWORD* pDstData32 = (DWORD*)pBytes;
        WORD*  pDstData16 = (WORD*)pBytes;

        for( DWORD x=0; x<ddsd.dwWidth; x++ )
        {
            DWORD dwPixel = m_pRGBAData[y*ddsd.dwWidth+x];

            BYTE r = (BYTE)((dwPixel>>24)&0x000000ff);
            BYTE g = (BYTE)((dwPixel>>16)&0x000000ff);
            BYTE b = (BYTE)((dwPixel>> 8)&0x000000ff);
            BYTE a = (BYTE)((dwPixel>> 0)&0x000000ff);

            DWORD dr = ((r>>(dwRShiftL))<<dwRShiftR)&dwRMask;
            DWORD dg = ((g>>(dwGShiftL))<<dwGShiftR)&dwGMask;
            DWORD db = ((b>>(dwBShiftL))<<dwBShiftR)&dwBMask;
            DWORD da = ((a>>(dwAShiftL))<<dwAShiftR)&dwAMask;

            if( 32 == ddsd.ddpfPixelFormat.dwRGBBitCount )
                pDstData32[x] = (DWORD)(dr+dg+db+da);
            else
                pDstData16[x] = (WORD)(dr+dg+db+da);
        }

        pBytes += ddsd.lPitch;
    }

    pddsTempSurface->Unlock(0);

    // Copy the temp surface to the real texture surface
    m_pddsSurface->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );

    // Done with the temp objects
    pddsTempSurface->Release();
    pDD->Release();

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CopyNTXDataToSurface()
// Desc: Copies the image of a ntx file into a surface
//-----------------------------------------------------------------------------
HRESULT TextureContainer::CopyNTXDataToSurface()
{
	// Get a DDraw object to create a temporary surface
	LPDIRECTDRAW7	pDD;
	m_pddsSurface->GetDDInterface( (VOID**)&pDD );

	// Setup the new surface desc
	DDSURFACEDESC2	ddsd;
	ddsd.dwSize = sizeof(ddsd);
	m_pddsSurface->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps2 = 0L;
	ddsd.dwWidth = m_dwWidth;
	ddsd.dwHeight = m_dwHeight;

	// Create a new surface for the texture
	LPDIRECTDRAWSURFACE7 pddsTempSurface[9];
	HRESULT hr;
	DWORD i = 0;
	for( i = 0; i < m_dwMipmapCount; i++ )
	{
		if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface[i], NULL ) ) )
		{
			pDD->Release();
			return hr;
		}

		ddsd.dwWidth >>= 1;
		ddsd.dwHeight >>= 1;
		if( ddsd.dwWidth < 1 ) ddsd.dwWidth = 1;
		if( ddsd.dwHeight < 1 ) ddsd.dwHeight = 1;
	}

	DWORD	dwX, dwY, dwWidth, dwHeight;
	DWORD	r, g, b;

	pddsTempSurface[0]->GetSurfaceDesc( &ddsd );
	if( 16 == ddsd.ddpfPixelFormat.dwRGBBitCount )		// 16 bit
	{
		DWORD	dwRMask = ddsd.ddpfPixelFormat.dwRBitMask;
		DWORD	dwGMask = ddsd.ddpfPixelFormat.dwGBitMask;
		DWORD	dwBMask = ddsd.ddpfPixelFormat.dwBBitMask;
		DWORD	dwRShiftL = 8, dwRShiftR = 0;
	    DWORD	dwGShiftL = 8, dwGShiftR = 0;
		DWORD	dwBShiftL = 8, dwBShiftR = 0;
		DWORD	dwMask;

		for( dwMask = dwRMask; dwMask && !(dwMask & 0x1); dwMask >>= 1 ) dwRShiftR++;
		for( ; dwMask; dwMask >>= 1 ) dwRShiftL--;
		for( dwMask = dwGMask; dwMask && !(dwMask & 0x1); dwMask >>= 1 ) dwGShiftR++;
		for( ; dwMask; dwMask >>= 1 ) dwGShiftL--;
		for( dwMask = dwBMask; dwMask && !(dwMask & 0x1); dwMask >>= 1 ) dwBShiftR++;
		for( ; dwMask; dwMask >>= 1 ) dwBShiftL--;

		for( i = 0; i < m_dwMipmapCount; i++ )
		{
			dwWidth = m_dwWidth >> i;
			dwHeight = m_dwHeight >> i;
			if( dwWidth < 1 ) dwWidth = 1;
			if( dwHeight < 1 ) dwHeight = 1;

			while( pddsTempSurface[i]->Lock( NULL, &ddsd, 0, 0 ) == DDERR_WASSTILLDRAWING );
			BYTE*	pBytes = (BYTE*)ddsd.lpSurface;

			WORD*	pDest;
			BYTE*	pSrc = m_ppNTXData[i];
			BYTE*	pSrcRow = pSrc;
			
			for( dwY = 0; dwY < ddsd.dwHeight; dwY++ )
			{
				pDest = (WORD*)pBytes;
				for( dwX = 0; dwX < ddsd.dwWidth; dwX++ )
				{
					r = *( pSrc );		g = *( pSrc + 1 );		b = *( pSrc + 2 );
					r = ( ( r >> dwRShiftL ) << dwRShiftR ) & dwRMask;
					g = ( ( g >> dwGShiftL ) << dwGShiftR ) & dwGMask;
					b = ( ( b >> dwBShiftL ) << dwBShiftR ) & dwBMask;

					pDest[dwX] = (WORD)( r + g + b );
					pSrc += 3;
				}

				pBytes += ddsd.lPitch;
				pSrcRow += dwWidth * 3;
				pSrc = pSrcRow;
			}

			pddsTempSurface[i]->Unlock( 0 );
		}
	}
	else		// 32bit
	{
		for( i = 0; i < m_dwMipmapCount; i++ )
		{
			dwWidth = m_dwWidth >> i;
			dwHeight = m_dwHeight >> i;
			if( dwWidth < 1 ) dwWidth = 1;
			if( dwHeight < 1 ) dwHeight = 1;

			while( pddsTempSurface[i]->Lock( NULL, &ddsd, 0, 0 ) == DDERR_WASSTILLDRAWING );
			BYTE*	pBytes = (BYTE*)ddsd.lpSurface;

			DWORD*	pDest;
			BYTE*	pSrc = m_ppNTXData[i];
			BYTE*	pSrcRow = pSrc;
			
			for( dwY = 0; dwY < ddsd.dwHeight; dwY++ )
			{
				pDest = (DWORD*)pBytes;
				for( dwX = 0; dwX < ddsd.dwWidth; dwX++ )
				{
					r = *( pSrc );		g = *( pSrc + 1 );		b = *( pSrc + 2 );

					pDest[dwX] = ( r << 16 ) + ( g << 8 ) + b;
					pSrc += 3;
				}

				pBytes += ddsd.lPitch;
				pSrcRow += dwWidth * 3;
				pSrc = pSrcRow;
			}

			pddsTempSurface[i]->Unlock( 0 );
		}
	}

	// Copy the temp surface to the real texture surface
	LPDIRECTDRAWSURFACE7	pddsDest = m_pddsSurface;
	for( i = 0; i < m_dwMipmapCount; i++ )
	{
		pddsDest->Blt( NULL, pddsTempSurface[i], NULL, DDBLT_WAIT, NULL );

		DDSCAPS2	ddsCaps;
		ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;
		ddsCaps.dwCaps2 = 0;
		ddsCaps.dwCaps3 = 0;
		ddsCaps.dwCaps4 = 0;
		if( SUCCEEDED( pddsDest->GetAttachedSurface( &ddsCaps, &pddsDest ) ) )
			pddsDest->Release();
	}

    // Done with the temp objects
	for( i = 0; i < m_dwMipmapCount; i++ )
		pddsTempSurface[i]->Release();

    pDD->Release();

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: SetTexturePath()
// Desc: Enumeration callback routine to find a best-matching texture format.
//-----------------------------------------------------------------------------
VOID IndexedTexture::SetTexturePath( LPCTSTR strTexturePath )
{
    if( NULL == strTexturePath )
        strTexturePath = _T("");
    lstrcpy( m_TexturePath, strTexturePath );
}

VOID IndexedTexture::SetTexturePath2( LPCTSTR strTexturePath )
{
    if( NULL == strTexturePath )
        return;

	int len = strlen(strTexturePath);

	if (m_TexturePath2)
		delete[] m_TexturePath2;
	m_TexturePath2 = new char[len+1];

    lstrcpy( m_TexturePath2, strTexturePath );
}

VOID IndexedTexture::SetTexturePath3( LPCTSTR strTexturePath )
{
    if( NULL == strTexturePath )
        return;

	int len = strlen(strTexturePath);

	if (m_TexturePath3)
		delete[] m_TexturePath3;
	m_TexturePath3 = new char[len+1];

    lstrcpy( m_TexturePath3, strTexturePath );
}



//-----------------------------------------------------------------------------
// Name: CreateTextureFromFile()
// Desc: Is passed a filename and creates a local Bitmap from that file.
//       The texture can not be used until it is restored, however.
//-----------------------------------------------------------------------------
int IndexedTexture::CreateTextureFromFile( TCHAR* strName, int Idx, DWORD dwStage,
                                        DWORD dwFlags, DWORD transAlpha )
{
	// Check parameters
    if( NULL == strName )
        //return E_INVALIDARG;
		return -1;

	int len = strlen(strName);
	if (len < 3 || len > 255) {
		//MessageBox(NULL, "Texture File 이름의 길이가 너무 길거나 짧습니다.", "오류!", MB_OK);
		return -1;
	}

	strcpy(fileName, strName);
	if (g_bWXY_FORMAT) {
		TCHAR* strExtension;
		if( NULL != ( strExtension = _tcsrchr( fileName, _T('.') ) ) ) {
			if( !lstrcmpi( strExtension, _T(".bmp") ) ) {
				len = strlen(fileName);
				strcpy(fileName+len-3, "wxy");
			} else if ( !lstrcmpi( strExtension, _T(".tga") ) ) {
				len = strlen(fileName);
				strcpy(fileName+len-3, "sek");
			}
		}
	}

    // Check first to see if the texture is already loaded
    //if( NULL != FindTexture( strName ) )
	int TextureNum;
	if( (TextureNum = FindTexture( fileName )) >= 0)
		return TextureNum;
        //return S_OK;

	if (Idx >= m_Size || (Idx == CREATE_ADD && m_Idx >= m_Size))
		//return E_FAIL;
		return -1;

	if (Idx == CREATE_INSERT) {
		for (int i = 0; i < m_Size; i++)
			if (!m_TextureList[i]) {
				Idx = i;
				break;
			}
	}
	if (Idx == CREATE_INSERT)
		//return E_FAIL;
		return -1;

    // Allocate and add the texture to the linked list of textures;
    TextureContainer* ptcTexture = new TextureContainer( fileName, dwStage,
                                                         dwFlags, transAlpha );
    if( NULL == ptcTexture )
        //return E_OUTOFMEMORY;
		return -1;

    // Create a bitmap and load the texture file into it,
    if( FAILED( ptcTexture->LoadImageData(m_TexturePath, m_TexturePath2, m_TexturePath3) ) )
    {
        delete ptcTexture;
        //return E_FAIL;
		return -1;
    }

    // Save the image's dimensions
    if( ptcTexture->m_hbmBitmap )
    {
        BITMAP bm;
        GetObject( ptcTexture->m_hbmBitmap, sizeof(BITMAP), &bm );
        ptcTexture->m_dwWidth  = (DWORD)bm.bmWidth;
        ptcTexture->m_dwHeight = (DWORD)bm.bmHeight;
        ptcTexture->m_dwBPP    = (DWORD)bm.bmBitsPixel;
    }

	if (Idx >= 0) {
		if (m_TextureList[Idx])
			delete m_TextureList[Idx];
		m_TextureList[Idx] = ptcTexture;
		if (Idx >= m_Idx)
			m_Idx = Idx+1;

		return Idx;
	} else {
		m_TextureList[m_Idx++] = ptcTexture;
		return (m_Idx-1);
	}

    //return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CreateEmptyTexture()
// Desc: Creates an empty texture.
//-----------------------------------------------------------------------------
int IndexedTexture::CreateEmptyTexture( TCHAR* strName, DWORD dwWidth,
                                     DWORD dwHeight, int Idx, DWORD dwStage, 
                                     DWORD dwFlags, DWORD transAlpha )
{
    // Check parameters
    if( NULL == strName )
        //return E_INVALIDARG;
		return -1;

    // Check first to see if the texture is already loaded
    //if( NULL != FindTexture( strName ) )
	if( FindTexture( strName ) >= 0)
        //return E_FAIL;
		return -1;

	if (Idx >= m_Size || (Idx < 0 && m_Idx >= m_Size))
		//return E_FAIL;
		return -1;

    // Allocate and add the texture to the indexed list of textures;
    TextureContainer* ptcTexture = new TextureContainer( strName, dwStage,
                                                         dwFlags, transAlpha );
    if( NULL == ptcTexture )
        //return E_OUTOFMEMORY;
		return -1;

    // Save dimensions
    ptcTexture->m_dwWidth  = dwWidth;
    ptcTexture->m_dwHeight = dwHeight;
    ptcTexture->m_dwBPP    = 16;
    if( ptcTexture->m_dwFlags & D3DTEXTR_32BITSPERPIXEL )
        ptcTexture->m_dwBPP = 32;

    // Save alpha usage flag
    if( dwFlags & D3DTEXTR_CREATEWITHALPHA )
        ptcTexture->m_bHasAlpha = TRUE;

	if (Idx >= 0) {
		if (m_TextureList[Idx])
			delete m_TextureList[Idx];
		m_TextureList[Idx] = ptcTexture;

		if (Idx >= m_Idx)
			m_Idx = Idx+1;

		return Idx;
	} else {
		m_TextureList[m_Idx++] = ptcTexture;

		return (m_Idx-1);
	}

    //return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Restore()
// Desc: Invalidates the current texture objects and rebuilds new ones
//       using the new device.
//-----------------------------------------------------------------------------
HRESULT IndexedTexture::Restore( TCHAR* strName, LPDIRECT3DDEVICE7 pd3dDevice )
{
    /*TextureContainer* ptcTexture = FindTexture( strName );
    if( NULL == ptcTexture )
        return DDERR_NOTFOUND;

    // Restore the texture (this recreates the new surface for this device).
    return ptcTexture->Restore( pd3dDevice );*/
    int TextureNum = FindTexture( strName );
    if( TextureNum < 0 )
        return DDERR_NOTFOUND;

    // Restore the texture (this recreates the new surface for this device).
    return m_TextureList[TextureNum]->Restore( pd3dDevice );
}


HRESULT IndexedTexture::RestoreByIdx( int Idx, LPDIRECT3DDEVICE7 pd3dDevice )
{
    if( Idx < 0 || Idx >= m_Idx )
        return DDERR_NOTFOUND;

    // Restore the texture (this recreates the new surface for this device).
    return m_TextureList[Idx]->Restore( pd3dDevice );
}


//-----------------------------------------------------------------------------
// Name: RestoreAllTextures()
// Desc: This function is called when a mode is changed. It updates all
//       texture objects to be valid with the new device.
//-----------------------------------------------------------------------------
HRESULT IndexedTexture::RestoreAllTextures( LPDIRECT3DDEVICE7 pd3dDevice )
{
    /*TextureContainer* ptcTexture = g_ptcTextureList;

    while( ptcTexture )
    {
        Restore( ptcTexture->m_strName, pd3dDevice );
        ptcTexture = ptcTexture->m_pNext;
    }*/

	for (int i = 0; i < m_Idx; i++) {
		if (m_TextureList[i])
	        //Restore( m_TextureList[i]->m_strName, pd3dDevice );
			RestoreByIdx(i, pd3dDevice );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Invalidate()
// Desc: Used to bump a texture out of (video) memory, this function
//       actually destroys the d3dtexture and ddsurface of the texture
//-----------------------------------------------------------------------------
HRESULT IndexedTexture::Invalidate( TCHAR* strName )
{
    /*TextureContainer* ptcTexture = FindTexture( strName );
    if( NULL == ptcTexture )
        return DDERR_NOTFOUND;

    SAFE_RELEASE( ptcTexture->m_pddsSurface );

    return S_OK;*/
    int TextureNum = FindTexture( strName );
    if( TextureNum < 0 )
        return DDERR_NOTFOUND;

    SAFE_RELEASE( m_TextureList[TextureNum]->m_pddsSurface );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateAllTextures()
// Desc: This function is called when a mode is changed. It invalidates
//       all texture objects so their device can be safely released.
//-----------------------------------------------------------------------------
HRESULT IndexedTexture::InvalidateAllTextures()
{
	for (int i = 0; i < m_Idx; i++) {
		if (m_TextureList[i])
			SAFE_RELEASE( m_TextureList[i]->m_pddsSurface );
	}
    /*TextureContainer* ptcTexture = g_ptcTextureList;

    while( ptcTexture )
    {
        SAFE_RELEASE( ptcTexture->m_pddsSurface );
        ptcTexture = ptcTexture->m_pNext;
    }*/

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DestroyTexture()
// Desc: Frees the resources for the specified texture container
//-----------------------------------------------------------------------------
HRESULT IndexedTexture::DestroyTexture( TCHAR* strName )
{
    /*TextureContainer* ptcTexture = FindTexture( strName );

    SAFE_DELETE( ptcTexture );*/
    int TextureNum = FindTexture( strName );

	if (TextureNum >=0) {
		SAFE_DELETE( m_TextureList[TextureNum] );
		m_TextureList[TextureNum] = NULL;
	}

    return S_OK;
}

HRESULT IndexedTexture::DestroyTextureByIdx( int Idx )
{
	if (Idx >= m_Idx)
		return E_FAIL;

	SAFE_DELETE( m_TextureList[Idx] );
	m_TextureList[Idx] = NULL;

    return S_OK;
}

HRESULT IndexedTexture::DestroyTextureAll()
{
	for (int i = 0; i < m_Idx; i++) {
		SAFE_DELETE( m_TextureList[i] );
		m_TextureList[i] = NULL;
	}
	m_Idx = 0;

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: GetSurface()
// Desc: Returns a pointer to a d3dSurface from the name of the texture
//-----------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 IndexedTexture::GetSurface( TCHAR* strName )
{
    /*TextureContainer* ptcTexture = FindTexture( strName );

    return ptcTexture ? ptcTexture->m_pddsSurface : NULL;*/
	int TextureNum = FindTexture( strName );

	return (TextureNum >= 0) ? m_TextureList[TextureNum]->m_pddsSurface : NULL;
}

LPDIRECTDRAWSURFACE7 IndexedTexture::GetSurfaceByIdx( int Idx )
{
    /*TextureContainer* ptcTexture = FindTexture( strName );

    return ptcTexture ? ptcTexture->m_pddsSurface : NULL;*/
	if (Idx >= 0 && Idx < m_Size && m_TextureList[Idx])
		return m_TextureList[Idx]->m_pddsSurface;

	return NULL;
}

BOOL IndexedTexture::SetSize(int size)
{
	ReleaseTextureList();

	m_TextureList = new TextureContainer*[size];
	ZeroMemory(m_TextureList, sizeof(TextureContainer*) * size);

	m_Size = size;
	m_Idx = 0;

	return TRUE;
}

void IndexedTexture::ReleaseTextureList()
{
	if (m_TextureList) {
		for (int i = 0; i < m_Idx; i++) {
			if (m_TextureList[i])
				delete m_TextureList[i];
		}
		delete[] m_TextureList;
	}
}

void IndexedTexture::SetAlphaAll(DWORD transAlpha)
{
	for (int i = 0; i < m_Size; i++) {
		if (m_TextureList[i])
			m_TextureList[i]->SetAlpha(transAlpha);
	}
}

BOOL IndexedTexture::SetAlpha(int Idx, DWORD transAlpha)
{
	if (Idx < m_Idx && m_TextureList[Idx]) {
		m_TextureList[Idx]->SetAlpha(transAlpha);

		return TRUE;
	} else {
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------
// Name : TileTextures Class
// Desc : 하나의 비트맵에서 여러 장의 텍스처 서피스를 생성한 후에 인덱스를 이용하여 접근을 한다.
//------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////

// 생성자
TileTextures::TileTextures(DWORD dwStage, DWORD dwFlags, DWORD transAlpha)
{
	m_TexturePath[0] = '\0';

    m_dwWidth     = 0;
    m_dwHeight    = 0;
    m_dwStage     = dwStage;
    m_dwBPP       = 0;
    m_dwFlags     = dwFlags;
    m_bHasAlpha   = 0;

    m_ppddsSurface = NULL;
    m_hbmBitmap   = NULL;

	m_transAlpha  = transAlpha;
}

TileTextures::~TileTextures()
{
	m_TexturePath[0] = '\0';

	Invalidate();								// 여러개의 서피스를 invalidate 시킨다.
	DeleteTextureSurfaces();

	DeleteObject( m_hbmBitmap );				// 원본 비트맵을 제거.
}

// --------------------------------------------------------------------------
// Name : SetTexturePath()
// Desc : 텍스처 파일이 있는 경로를 설정한다.
// --------------------------------------------------------------------------
VOID TileTextures::SetTexturePath( LPCTSTR strTexturePath )
{
	if( NULL == strTexturePath )
        strTexturePath = _T("");
    lstrcpy( m_TexturePath, strTexturePath );
}

// --------------------------------------------------------------------------
// Name : CreateTextureFromFile()
// Desc : 파일로부터 원하는 데이타를 로딩하여 비트맵 핸들을 보관한다.
// --------------------------------------------------------------------------
HRESULT TileTextures::CreateTextureFromFile(TCHAR* strName, DWORD dwTileWidth, DWORD dwTileHeight, DWORD dwStage, DWORD dwFlags, DWORD transAlpha)
{
	// Check parameters
    if( NULL == strName )
        return E_INVALIDARG;

	// 로딩할 텍스처 이름을 보관
	lstrcpy( m_strName, strName );

	// Create a bitmap and load the texture file into it,
	if (FAILED( LoadImageData(m_TexturePath) )) 
		return E_FAIL;
	
	// Save the image's dimensions
	if ( m_hbmBitmap )
	{
		BITMAP bm;
        GetObject( m_hbmBitmap, sizeof(BITMAP), &bm );
        m_dwWidth  = (DWORD)bm.bmWidth;
        m_dwHeight = (DWORD)bm.bmHeight;
        m_dwBPP    = (DWORD)bm.bmBitsPixel;
	}

	m_dwTileWidth = dwTileWidth;
	m_dwTileHeight = dwTileHeight;

	m_dwWidthNum = m_dwWidth / m_dwTileWidth;
	m_dwHeightNum = m_dwHeight / m_dwTileHeight;

	return S_OK;
}

// --------------------------------------------------------------------------
// Name : LoadImageData()
// Desc : 실제적으로 비트맵을 읽어 생성하는 루틴.
// --------------------------------------------------------------------------
HRESULT TileTextures::LoadImageData(TCHAR* strTexturePath)
{
	TCHAR* strExtension;
    TCHAR  strPathname[256];
    FILE*  file;

    // Check the executable's resource. If it's there, we're done!
    m_hbmBitmap = (HBITMAP)LoadImage( GetModuleHandle(NULL), m_strName,
                                      IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
    if( m_hbmBitmap )
        return S_OK;

	// First check if the file exists in the global texture path
    lstrcpy( strPathname, strTexturePath );
    lstrcat( strPathname, m_strName );
	if( NULL == ( file = fopen( strPathname, "rb" ) ) )
            return DDERR_NOTFOUND;
    fclose(file);

	// Get the filename extension
    if( NULL == ( strExtension = _tcsrchr( m_strName, _T('.') ) ) )
        return DDERR_UNSUPPORTED;

	// Load bitmap files
    if( !lstrcmpi( strExtension, _T(".bmp") ) )
        return LoadBitmapFile( strPathname );

	// Can add code here to check for other file formats before failing
    return DDERR_UNSUPPORTED;
}

// -------------------------------------------------------------------------------
//
//
// -------------------------------------------------------------------------------
HRESULT TileTextures::LoadBitmapFile( TCHAR* strPathname )
{
	// Try to load the bitmap as a file
    m_hbmBitmap = (HBITMAP)LoadImage( NULL, strPathname, IMAGE_BITMAP, 0, 0,
                                      LR_LOADFROMFILE|LR_CREATEDIBSECTION );
    if( m_hbmBitmap )
        return S_OK;
    
    return DDERR_NOTFOUND;
}

// -------------------------------------------------------------------------------
// Name: Restore(LPDIRECT3DDEVICE7 pd3dDevice);
// Desc:
// -------------------------------------------------------------------------------
HRESULT TileTextures::Restore(LPDIRECT3DDEVICE7 pd3dDevice)
{
	// Release any previously created objects
	if (m_ppddsSurface != NULL) 
	{
		Invalidate();
		DeleteTextureSurfaces();
	}

	// Check params
    if( NULL == pd3dDevice )
        return DDERR_INVALIDPARAMS;

	// Get the device caps
    D3DDEVICEDESC7 ddDesc;
    if( FAILED( pd3dDevice->GetCaps( &ddDesc) ) )
        return E_FAIL;

	// Setup the new surface desc  : 타일 크기의 서피스에 대한 설정
	// 타일 텍스처는 비디오 메모리에 생성을 한다.
    DDSURFACEDESC2 ddsd;
    D3DUtil_InitSurfaceDesc( ddsd );
    ddsd.dwFlags         = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|
                           DDSD_PIXELFORMAT|DDSD_TEXTURESTAGE;
    //ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE | DDSCAPS_VIDEOMEMORY;
	ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
    ddsd.dwTextureStage  = m_dwStage;
    ddsd.dwWidth         = m_dwTileWidth;
    ddsd.dwHeight        = m_dwTileHeight;


    // Turn on texture management for hardware devices
	/*
    if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
        ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
    else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
        ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
    else
        ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	*/

	// 맵 타일 서피스는 하드웨어 디바이스라면, 비디오 메모리에 생성을 한다.
	if( ddDesc.deviceGUID == IID_IDirect3DHALDevice || ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice ) {
		ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
	} else {
        ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	


	// Adjust width and height to be powers of 2, if the device requires it
    if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2 )
    {
        for( ddsd.dwWidth=1;  m_dwTileWidth>ddsd.dwWidth;   ddsd.dwWidth<<=1 );
        for( ddsd.dwHeight=1; m_dwTileHeight>ddsd.dwHeight; ddsd.dwHeight<<=1 );
    }

	// Limit max texture sizes, if the driver can't handle large textures
    DWORD dwMaxWidth  = ddDesc.dwMaxTextureWidth;
    DWORD dwMaxHeight = ddDesc.dwMaxTextureHeight;
    ddsd.dwWidth  = min( ddsd.dwWidth,  ( dwMaxWidth  ? dwMaxWidth  : 256 ) );
    ddsd.dwHeight = min( ddsd.dwHeight, ( dwMaxHeight ? dwMaxHeight : 256 ) );

	// Make the texture square, if the driver requires it
    if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
    {
        if( ddsd.dwWidth > ddsd.dwHeight ) ddsd.dwHeight = ddsd.dwWidth;
        else                               ddsd.dwWidth  = ddsd.dwHeight;
    }

	// Setup the structure to be used for texture enumration.
    TEXTURESEARCHINFO tsi;
    tsi.bFoundGoodFormat = FALSE;
    tsi.pddpf            = &ddsd.ddpfPixelFormat;
    tsi.dwDesiredBPP     = m_dwBPP;
    //tsi.bUsePalette      = ( m_dwBPP <= 8 );
	tsi.bUsePalette      = 0;
    tsi.bUseAlpha        = m_bHasAlpha;
	tsi.bSetAlpha		 = FALSE;
    if( m_dwFlags & D3DTEXTR_16BITSPERPIXEL )
        tsi.dwDesiredBPP = 16;
    else if( m_dwFlags & D3DTEXTR_32BITSPERPIXEL )
        tsi.dwDesiredBPP = 32;

    if( m_dwFlags & (D3DTEXTR_TRANSPARENTWHITE|D3DTEXTR_TRANSPARENTBLACK|D3DTEXTR_TRANSPARENTALPHA) )
    {
        if( tsi.bUsePalette )
        {
            if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
            {
                tsi.bUseAlpha   = TRUE;
                tsi.bUsePalette = TRUE;
            }
            else
            {
                tsi.bUseAlpha   = TRUE;
                tsi.bUsePalette = FALSE;
            }
        }
    }

    // Enumerate the texture formats, and find the closest device-supported
    // texture pixel format
    pd3dDevice->EnumTextureFormats( TextureSearchCallback, &tsi );

    // If we couldn't find a format, let's try a default format
    if( FALSE == tsi.bFoundGoodFormat )
    {
        tsi.bUsePalette  = FALSE;
        tsi.dwDesiredBPP = 16;
        pd3dDevice->EnumTextureFormats( TextureSearchCallback, &tsi );

        // If we still fail, we cannot create this texture
        if( FALSE == tsi.bFoundGoodFormat )
            return E_FAIL;
    }

	// Get the DirectDraw interface for creating surfaces
    LPDIRECTDRAW7        pDD;
    LPDIRECTDRAWSURFACE7 pddsRender;
    pd3dDevice->GetRenderTarget( &pddsRender );
    pddsRender->GetDDInterface( (VOID**)&pDD );
    pddsRender->Release();

	// 타일의 갯수만큼 서피스를 생성.
	m_ppddsSurface = new LPDIRECTDRAWSURFACE7[m_dwWidthNum*m_dwHeightNum];

	for (int i=0; i < (int) (m_dwWidthNum*m_dwHeightNum); i++)
	{
		HRESULT hr = pDD->CreateSurface(&ddsd, &m_ppddsSurface[i], NULL);
		if( FAILED(hr) )
			return hr;
	}
    
    // Done with DDraw
    pDD->Release();    

    // For bitmap-based textures, copy the bitmap image.
    if( m_hbmBitmap )
        return CopyBitmapToMultipleSurface();

    return S_OK;
}

// -------------------------------------------------------------------------------
// Name: DeleteTextureSurfaces()
// Desc:
// -------------------------------------------------------------------------------
void TileTextures::DeleteTextureSurfaces()
{
	if (m_ppddsSurface != NULL)
		delete [] m_ppddsSurface;
	m_ppddsSurface = NULL;
}


// -------------------------------------------------------------------------------
// Name: CopyBitmapToMultipleSurface()
// Desc:
// -------------------------------------------------------------------------------
HRESULT TileTextures::CopyBitmapToMultipleSurface()
{
	unsigned int i;

	// Get a DDraw object to create a temporary surface
    LPDIRECTDRAW7 pDD;
    m_ppddsSurface[0]->GetDDInterface( (VOID**)&pDD );

	// Get the bitmap structure (to extract width, height, and bpp)
    BITMAP bm;
    GetObject( m_hbmBitmap, sizeof(BITMAP), &bm );

	// Setup the new surface desc
    DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    m_ppddsSurface[0]->GetSurfaceDesc( &ddsd );
    ddsd.dwFlags          = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT|
                            DDSD_TEXTURESTAGE;
    ddsd.ddsCaps.dwCaps   = DDSCAPS_TEXTURE|DDSCAPS_SYSTEMMEMORY;
    ddsd.ddsCaps.dwCaps2  = 0L;
    ddsd.dwWidth          = m_dwTileWidth;
    ddsd.dwHeight         = m_dwTileHeight;

	// Create a new surface for the texture
    LPDIRECTDRAWSURFACE7 pddsTempSurface;
    HRESULT hr;
    if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface, NULL ) ) )
    {
        pDD->Release();
        return hr;
    }

	// Get a DC for the bitmap
    HDC hdcBitmap = CreateCompatibleDC( NULL );
    if( NULL == hdcBitmap )
    {
        pddsTempSurface->Release();
        pDD->Release();
        return hr;
    }
    SelectObject( hdcBitmap, m_hbmBitmap );

	// Handle palettized textures. Need to attach a palette
	// 아래 내용중에서 알파 부분은 삭제.
    if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 )
    {
        LPDIRECTDRAWPALETTE  pPalette;
        DWORD dwPaletteFlags = DDPCAPS_8BIT|DDPCAPS_ALLOW256;
        DWORD pe[256];
        WORD  wNumColors     = GetDIBColorTable( hdcBitmap, 0, 256, (RGBQUAD*)pe );

        // Create the color table
        for( WORD i=0; i<wNumColors; i++ )
        {
            pe[i] = RGB( GetBValue(pe[i]), GetGValue(pe[i]), GetRValue(pe[i]) );
		}

		// Create & attach a palette
        pDD->CreatePalette( dwPaletteFlags, (PALETTEENTRY*)pe, &pPalette, NULL );
        pddsTempSurface->SetPalette( pPalette );
		for (i = 0; i < m_dwTileWidth*m_dwTileHeight; i++)
			m_ppddsSurface[0]->SetPalette( pPalette );
        SAFE_RELEASE( pPalette );
    }

	// Copy the bitmap image to the surface.
    HDC hdcSurface;

	for (i=0; i < m_dwWidthNum*m_dwHeightNum; i++) 
	{
		if( SUCCEEDED( pddsTempSurface->GetDC( &hdcSurface ) ) )
		{
			//BitBlt( hdcSurface, 0, 0, m_dwTileWidth, m_dwTileHeight, hdcBitmap, 
			//	(i % m_dwWidthNum)*m_dwTileWidth, ((m_dwHeightNum-1) * m_dwTileHeight)-(i/m_dwWidthNum)*m_dwTileHeight,
			//	SRCCOPY );
			BitBlt( hdcSurface, 0, 0, m_dwTileWidth, m_dwTileHeight, hdcBitmap, 
				(i % m_dwWidthNum)*m_dwTileWidth, (i/m_dwWidthNum)*m_dwTileHeight,
				SRCCOPY );
			pddsTempSurface->ReleaseDC( hdcSurface );
		}	

		// Copy the temp surface to the real texture surface
		m_ppddsSurface[i]->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );
	}

	DeleteDC( hdcBitmap );    
    // Done with the temp surface
    pddsTempSurface->Release();
	pDD->Release();

	return S_OK;
}

// -------------------------------------------------------------------------------
// Name: Invalidate()
// Desc:
// -------------------------------------------------------------------------------
HRESULT TileTextures::Invalidate()
{
	if (m_ppddsSurface != NULL)
	{
		// m_dwTileWidth * m_dwTileHeight 갯수만큼의 서피스를 release 한다.
		for (unsigned int i = 0; i < m_dwWidthNum*m_dwHeightNum; i++)
			SAFE_RELEASE( m_ppddsSurface[i] );
	}

	return S_OK;
}

LPDIRECTDRAWSURFACE7 TileTextures::GetSurfaceByIdx( int Idx)
{
	if (m_ppddsSurface != NULL)
		return m_ppddsSurface[Idx];

	return NULL;
}