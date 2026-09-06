// ShadowRect.h: interface for the CShadowRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADOWRECT_H__EBA7CD83_41EF_4108_9696_070863140957__INCLUDED_)
#define AFX_SHADOWRECT_H__EBA7CD83_41EF_4108_9696_070863140957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3d.h>
#include <d3dx.h>

class CNKTerrain;
class IndexedTexture;

class CShadowRect  
{
	D3DLVERTEX		m_TileRectVert[36];
	unsigned short	m_TileIndex[54];
	CNKTerrain*		m_pTerrain;								// 지형에 대한 정보를 얻기 위한 포인터. 지우지 말것.
	D3DXVECTOR3		m_vSelSpot;								// 선택된 지점의 월드 좌표
	IndexedTexture*	m_pIndexedTexture;						// 텍스처 배열에 대한 포인터. 지우지 말것.
	int				m_tileIdx;								// 선택 지점의 타일 인덱스
	int				m_ShadowTexIdx;							// 그림자로 쓰일 텍스처의 인덱스.
	int				m_TileNum;
	int				m_ShadowXSize;
	int				m_ShadowZSize;
	int				m_Sign;

protected:
	void		SetupTileRect();
	
public:
	CShadowRect();
	virtual ~CShadowRect();

	void		Init( CNKTerrain* pTerrain, IndexedTexture *pIndexedTexture );
	void		SetTileCoord( D3DXVECTOR3& rVecWorld, int size_or_hour, BOOL bIsHour );
	void		SetTileCoord( float wx, float wy, float wz, int size_or_hour, BOOL bIsHour );

	HRESULT		Render( LPDIRECT3DDEVICE7 pd3dDevice, LPDIRECTDRAWSURFACE7 lpDDsurface = NULL, BOOL bReal = FALSE );	
};

#endif // !defined(AFX_SHADOWRECT_H__EBA7CD83_41EF_4108_9696_070863140957__INCLUDED_)
