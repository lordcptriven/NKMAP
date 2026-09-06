//-----------------------------------------------------------------------------
// File: D3DTextr.h
//
// Desc: Functions to manage textures, including creating (loading from a
//       file), restoring lost surfaces, invalidating, and destroying.
//
//       Note: the implementation of these fucntions maintain an internal list
//       of loaded textures. After creation, individual textures are referenced
//       via their ASCII names.
//
// Copyright (c) 1997-1999 Microsoft Corporation. All rights reserved
//-----------------------------------------------------------------------------
#ifndef D3DTEXTR_H
#define D3DTEXTR_H
#include <ddraw.h>
#include <d3d.h>


//-----------------------------------------------------------------------------
// Name: TextureContainer
// Desc: Linked list structure to hold info per texture
//-----------------------------------------------------------------------------
class TextureContainer
{
public: //
    //TextureContainer* m_pNext; // Linked list ptr

    TCHAR   m_strName[80];     // Name of texture (doubles as image filename)
	TCHAR	m_strPathName[_MAX_FNAME + _MAX_EXT];
    DWORD   m_dwWidth;
    DWORD   m_dwHeight;
    DWORD   m_dwStage;         // Texture stage (for multitexture devices)
    DWORD   m_dwBPP;
    DWORD   m_dwFlags;
    BOOL    m_bHasAlpha;

    LPDIRECTDRAWSURFACE7 m_pddsSurface; // Surface of the texture
    HBITMAP m_hbmBitmap;       // Bitmap containing texture image
    DWORD*  m_pRGBAData;
	BYTE**	m_ppNTXData;
	DWORD	m_transAlpha;

	DWORD	m_dwMipmapCount;

public:
    HRESULT LoadImageData(TCHAR* strTexturePath, TCHAR* strTexturePath2, TCHAR* strTexturePath3);
	HRESULT LoadImageData();
	HRESULT LoadBitmapFile2( TCHAR* strPathname );
    HRESULT LoadBitmapFile( TCHAR* strPathname );
	HRESULT LoadWxyFile( TCHAR* strPathname );
    HRESULT LoadTargaFile( TCHAR* strPathname );
	HRESULT LoadSekFile( TCHAR* strPathname );
	HRESULT	LoadNTXFile( TCHAR* strPathname );
    HRESULT Restore( LPDIRECT3DDEVICE7 pd3dDevice );
    HRESULT CopyBitmapToSurface();
    HRESULT CopyRGBADataToSurface();
	HRESULT CopyNTXDataToSurface();

    TextureContainer( TCHAR* strName, DWORD dwStage, DWORD dwFlags, DWORD transAlpha );
    ~TextureContainer();

	HRESULT SetAlpha(DWORD transAlpha);
};

//-----------------------------------------------------------------------------
// Texture creation and deletion functions
//-----------------------------------------------------------------------------
#define D3DTEXTR_TRANSPARENTWHITE 0x00000001
#define D3DTEXTR_TRANSPARENTBLACK 0x00000002
#define D3DTEXTR_TRANSPARENTALPHA 0x00000003

#define D3DTEXTR_32BITSPERPIXEL   0x00000004
#define D3DTEXTR_16BITSPERPIXEL   0x00000008
#define D3DTEXTR_CREATEWITHALPHA  0x00000010

#define CREATE_ADD		(-1)
#define CREATE_INSERT	(-2)

class IndexedTexture {
public:

	IndexedTexture();
	IndexedTexture(int size);
	~IndexedTexture();

	TCHAR  m_TexturePath[512]; // Path for files
	TCHAR  *m_TexturePath2;
	TCHAR  *m_TexturePath3;
	
	// Local list of textures
	TextureContainer** m_TextureList;

	//-----------------------------------------------------------------------------
	// Access functions for loaded textures. Note: these functions search
	// an internal list of the textures, and use the texture associated with the
	// ASCII name.
	//-----------------------------------------------------------------------------
	LPDIRECTDRAWSURFACE7 GetSurface( TCHAR* strName );
	LPDIRECTDRAWSURFACE7 GetSurfaceByIdx( int Idx );

	//-----------------------------------------------------------------------------
	// Texture invalidation and restoration functions
	//-----------------------------------------------------------------------------
	HRESULT Invalidate( TCHAR* strName );
	HRESULT Restore( TCHAR* strName, LPDIRECT3DDEVICE7 pd3dDevice );
	HRESULT RestoreByIdx( int Idx, LPDIRECT3DDEVICE7 pd3dDevice );
	HRESULT InvalidateAllTextures();
	HRESULT RestoreAllTextures( LPDIRECT3DDEVICE7 pd3dDevice );


	/*HRESULT CreateTextureFromFile( TCHAR* strName, DWORD dwStage=0L,
											DWORD dwFlags=0L );*/
	int CreateTextureFromFile( TCHAR* strName, int Idx = CREATE_ADD, DWORD dwStage=0L,
											DWORD dwFlags=0L, DWORD transAlpha = 0xff );
	int CreateEmptyTexture( TCHAR* strName, DWORD dwWidth,
										 DWORD dwHeight, int Idx, DWORD dwStage,
										 DWORD dwFlags, DWORD transAlpha = 0xff );
	HRESULT DestroyTexture( TCHAR* strName );
	HRESULT DestroyTextureByIdx( int Idx );
	HRESULT DestroyTextureAll(void);
	VOID    SetTexturePath( LPCTSTR strTexturePath );
	VOID    SetTexturePath2( LPCTSTR strTexturePath );
	VOID    SetTexturePath3( LPCTSTR strTexturePath );

	int FindTexture( TCHAR* strTextureName );
	void ReleaseTextureList();

	void SetAlphaAll(DWORD transAlpha = 0xff);
	BOOL SetAlpha(int Idx, DWORD transAlpha);

	BOOL SetSize(int size);
	int m_Size;
	int m_Idx;

	TCHAR*	GetTextureName( int nIndex ) const { return m_TextureList[nIndex]->m_strName; }
};

// 타일용 서피스를 위한 클래스
// tga를 읽는 루틴은 생략하였다.
class TileTextures
{
protected: 
    TCHAR   m_strName[80];		// Name of texture (doubles as image filename)
    DWORD   m_dwWidth;			// bitmap의 Width
    DWORD   m_dwHeight;			// bitmap의 height
	DWORD	m_dwTileWidth;		// tile 하나의 width
	DWORD	m_dwTileHeight;		// tile 하나의 height
	DWORD	m_dwWidthNum;		// 서피스의 가로 갯수
	DWORD	m_dwHeightNum;		// 서피스의 세로 갯수
    DWORD   m_dwStage;			// Texture stage (for multitexture devices)
    DWORD   m_dwBPP;
    DWORD   m_dwFlags;
    BOOL    m_bHasAlpha;

    LPDIRECTDRAWSURFACE7	*m_ppddsSurface; // Surface of the texture
    HBITMAP m_hbmBitmap;       // Bitmap containing texture image
	DWORD	m_transAlpha;

	DWORD	m_dwMipmapCount;

public:
	TCHAR  m_TexturePath[512]; // Path for files

protected:
	HRESULT LoadImageData(TCHAR* strTexturePath);
    HRESULT LoadBitmapFile( TCHAR* strPathname );
    
    HRESULT CopyBitmapToMultipleSurface();

public:
	VOID    SetTexturePath( LPCTSTR strTexturePath );
	HRESULT CreateTextureFromFile(TCHAR* strName, DWORD dwTileWidth, DWORD dwTileHeight, 
					DWORD dwStage=0L, DWORD dwFlags=0L, DWORD transAlpha = 0xff );

	//-----------------------------------------------------------------------------
	// Texture invalidation and restoration functions
	//-----------------------------------------------------------------------------
	HRESULT Invalidate();
	HRESULT Restore(LPDIRECT3DDEVICE7 pd3dDevice);

	// Texture Access functions
	LPDIRECTDRAWSURFACE7 GetSurfaceByIdx( int Idx);

    TileTextures(DWORD dwStage=0L, DWORD dwFlags=0L, DWORD transAlpha = 0xff );
    ~TileTextures();

	void DeleteTextureSurfaces();
};

#endif // D3DTEXTR_H
