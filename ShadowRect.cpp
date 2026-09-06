// ShadowRect.cpp: implementation of the CShadowRect class.
//
//////////////////////////////////////////////////////////////////////
#define STRICT
#define D3D_OVERLOADS

#include "StdAfx.h"
#include "ShadowRect.h"
#include "NKTerrain.h"
#include "IndexedTexture.h"
#include "D3dUtil.h"
#include "NKMath.h"

/*
// 외부 전역변수
extern int		gMapWidth;							// 맵의 x축 넓이
extern int		gMapHeight;
extern int		gHFWidth;
extern int		gHFHeight;
extern int		gMapExtention;					// 스케일 팩터 - 정수형
extern float	gfMapExtention;				// 스케일 팩터 - 실수형	
*/
//#define TILE_SIZE_F		(50.0f)
//#define TILE_SIZE		50
//#define SHADOW_SIZE		60
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShadowRect::CShadowRect()
{
	m_pTerrain = NULL;
	m_pIndexedTexture = NULL;

	int i = 0;
	for( i = 0; i < 9; i++ )
	{
		m_TileIndex[i*6+0] = i*4+0;
		m_TileIndex[i*6+1] = i*4+1;
		m_TileIndex[i*6+2] = i*4+2;
		m_TileIndex[i*6+3] = i*4+2;
		m_TileIndex[i*6+4] = i*4+3;
		m_TileIndex[i*6+5] = i*4+0;
	}
	for( i = 0; i < 36; i++ )
	{
		m_TileRectVert[i].dcColor = 0xffffffff;
		m_TileRectVert[i].dcSpecular = 1;
	}
	m_TileNum = 0;
}

CShadowRect::~CShadowRect()
{
	m_pTerrain = NULL;
	m_pIndexedTexture = NULL;
	m_ShadowTexIdx = -1;
}

// -------------------------------------------------------------------
// Init ()
// Desc : 지형에 대한 정보를 알기 위해서 필요한 클래스 포인터 연결
// -------------------------------------------------------------------
void CShadowRect::Init( CNKTerrain *pTerrain, IndexedTexture *pIndexedTexture )
{
	m_pTerrain = pTerrain;
	m_pIndexedTexture = pIndexedTexture;

	m_ShadowTexIdx = m_pIndexedTexture->CreateTextureFromFile( "shadow.tga" );
}


void CShadowRect::SetTileCoord( float wx, float wy, float wz, int size_or_hour, BOOL bIsHour )
{
	D3DXVECTOR3	vecWorld;
	vecWorld = D3DVECTOR( wx, wy, wz );

	SetTileCoord( vecWorld, size_or_hour, bIsHour );
}

#define SHADOW_MIN_SIZE		40
#define SHADOW_MAX_SIZE		90
void CShadowRect::SetTileCoord( D3DXVECTOR3& rVecWorld, int size_or_hour, BOOL bIsHour )
{
	m_vSelSpot = rVecWorld;
	m_tileIdx = m_pTerrain->GetMapIndex( m_vSelSpot );

	if( bIsHour )
	{
		m_ShadowZSize = SHADOW_MIN_SIZE;
		if( size_or_hour < 5 || size_or_hour > 24 )
		{
			m_ShadowXSize = SHADOW_MIN_SIZE;
		}
		else if( size_or_hour < 15 )
		{
			// SHADOW_MAX_SIZE - (SHADOW_MAX_SIZE-SHADOW_MIN_SIZE)*(cur_hour-5)/10
			m_ShadowXSize = 90 - (50 * (size_or_hour-5) / 10);
		}
		else
		{
			// SHADOW_MIN_SIZE + (SHADOW_MAX_SIZE-SHADOW_MIN_SIZE)*(cur_hour-15)/9
			m_ShadowXSize = 40 + (50 * (size_or_hour-15) / 9);
		}

		if( size_or_hour < 15 )
			m_Sign = 1;
		else
			m_Sign = -1;
	}
	else
	{
		m_ShadowXSize = size_or_hour;
		m_ShadowZSize = size_or_hour;
		m_Sign = 1;
	}

	SetupTileRect();
}


HRESULT CShadowRect::Render( LPDIRECT3DDEVICE7 pd3dDevice, LPDIRECTDRAWSURFACE7 lpDDsurface, BOOL bReal )
{
	HRESULT		res;
	//LPDIRECTDRAWSURFACE7 lpDDsurface;
	D3DMATRIX	matWorldOld;
	DWORD		lightingOld, alphaEnableOld, srcBlendOld, destBlendOld, tssAddressOld;

	if( !lpDDsurface )
	{
		if(m_ShadowTexIdx == -1 )
			lpDDsurface = NULL;
		else
			lpDDsurface = m_pIndexedTexture->GetSurfaceByIdx( m_ShadowTexIdx );
	}
	res = pd3dDevice->SetTexture( 0, lpDDsurface );

	pd3dDevice->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorldOld );
	pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, (D3DMATRIX*)(&NKMAT_IDENTITY) );

	// 알파테스팅
	pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, FALSE );

	// 라이팅 끄고...
	pd3dDevice->GetRenderState( D3DRENDERSTATE_LIGHTING, &lightingOld );
	pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	// 알파블렌딩
	pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaEnableOld );
	pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld ); // Test 용...
	pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );

	pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	//pd3dDevice->SetRenderState(D3DRENDERSTATE_COLORKEYBLENDENABLE, TRUE);
 
	if( bReal )
	{
		pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO ); 
		pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR );
		//pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR); 
		//pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO );
	}
	else
	{
		pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}


	// 여기서 부터 스테이지 설정
	pd3dDevice->GetTextureStageState( 0 ,D3DTSS_ADDRESS, &tssAddressOld );
	pd3dDevice->SetTextureStageState( 0 ,D3DTSS_ADDRESS, D3DTADDRESS_CLAMP );
/*
	pd3dDevice->SetRenderState(D3DRENDERSTATE_WRAP0 , 0);
	
	pd3dDevice->SetRenderState( D3DRENDERSTATE_WRAP0, 0);
	pd3dDevice->SetRenderState( D3DRENDERSTATE_WRAP1, 0);
	pd3dDevice->SetRenderState( D3DRENDERSTATE_WRAP2, 0);
	

	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
*/
	res = pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX,
											m_TileRectVert, m_TileNum*4,
											m_TileIndex, m_TileNum*6, 0 );

	// 디폴트 설정으로 되돌리기
	//pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, lightingOld );
	pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaEnableOld );
	pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld );
	pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld );
	pd3dDevice->SetTextureStageState( 0 ,D3DTSS_ADDRESS, tssAddressOld );
	pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorldOld );

	return res;
}


void CShadowRect::SetupTileRect()
{
	int		tx, tz;//, tileIdx;
	float	wx, wz, tu, tv;
	int		nMapWidth = m_pTerrain->GetHMWidth() - 1;
	int		nMapPatchSize = m_pTerrain->GetPatchSize();
	float	fMapPatchSize = (float)nMapPatchSize;

	// 넘겨온 좌표 부분 그리기.		
	wx = m_vSelSpot.x;
	wz = m_vSelSpot.z;

	// 텍스처 좌표 넣기
	// tv 값 계산시 맵 좌표는 위로 증가 이고 택스처 좌표는 아래로 증가이다.	
	tx = (int)wx % nMapPatchSize;
	tz = (int)wz % nMapPatchSize;
			
	//tu = ((float)tx) / SHADOW_SIZE;				// 맵 좌표를 타일 한칸의 폭으로 나누어 몫을 구한다.
	//tv = ((float)tz) / SHADOW_SIZE;
	tu = (float)tz / m_ShadowZSize;				// 맵 좌표를 타일 한칸의 폭으로 나누어 몫을 구한다.
	tv = (float)tx / m_ShadowXSize;

	// z값의 나머지가 클수록 타일의 윗쪽으로 이동하는 것이지만 텍스처 좌표는 아래쪽으로 이동한다.
	// 따라서 값을 보정해 주어야 한다.

	// 현재까지의 tu와 tv는 텍스처 상단 위치. 원점 이동을 통한 보정.
	//tu = 0.5f - tu;
	//tv = 0.5f - ((TILE_SIZE_F/SHADOW_SIZE) - tv);
	tu = 0.5f - ( m_Sign * tu );
	tv = 0.5f - ( m_Sign * tv );

	float	tile_shadow_x_num = m_Sign * ( fMapPatchSize / m_ShadowXSize );
	float	tile_shadow_z_num = m_Sign * ( fMapPatchSize / m_ShadowZSize );

	float	x = (float)( m_tileIdx % nMapWidth );
	float	z = (float)( m_tileIdx / nMapWidth );	

	m_TileRectVert[0].tu = tu;
	m_TileRectVert[0].tv = tv;
	m_TileRectVert[0].x = x;
	m_TileRectVert[0].z = z;

	m_TileNum = 1;
	if( tz < m_ShadowZSize / 2 && z > 0 )
	{
		// 하
		m_TileRectVert[m_TileNum*4].tu = tu - tile_shadow_z_num;
		m_TileRectVert[m_TileNum*4].tv = tv;
		m_TileRectVert[m_TileNum*4].x = x;
		m_TileRectVert[m_TileNum*4].z = z-1;
		m_TileNum++;

		if( tx < m_ShadowXSize / 2 && x > 0 )
		{
			// 좌하
			m_TileRectVert[m_TileNum*4].tu = tu - tile_shadow_z_num;
			m_TileRectVert[m_TileNum*4].tv = tv - tile_shadow_x_num;
			m_TileRectVert[m_TileNum*4].x = x-1;
			m_TileRectVert[m_TileNum*4].z = z-1;
			m_TileNum++;
		}
		if( tx > nMapPatchSize - ( m_ShadowXSize / 2 ) && x < nMapWidth - 1 )
		{
			// 우하
			m_TileRectVert[m_TileNum*4].tu = tu - tile_shadow_z_num;
			m_TileRectVert[m_TileNum*4].tv = tv + tile_shadow_x_num;
			m_TileRectVert[m_TileNum*4].x = x+1;
			m_TileRectVert[m_TileNum*4].z = z-1;
			m_TileNum++;
		}
	}

	if( tx < m_ShadowXSize / 2 && x > 0 )
	{
		// 좌
		m_TileRectVert[m_TileNum*4].tu = tu;
		m_TileRectVert[m_TileNum*4].tv = tv - tile_shadow_x_num;
		m_TileRectVert[m_TileNum*4].x = x-1;
		m_TileRectVert[m_TileNum*4].z = z;
		m_TileNum++;
	}
	if( tx > nMapPatchSize - ( m_ShadowXSize / 2 ) && x < nMapWidth - 1 )
	{
		// 우
		m_TileRectVert[m_TileNum*4].tu = tu;
		m_TileRectVert[m_TileNum*4].tv = tv + tile_shadow_x_num;
		m_TileRectVert[m_TileNum*4].x = x+1;
		m_TileRectVert[m_TileNum*4].z = z;
		m_TileNum++;
	}

	if( tz > nMapPatchSize - ( m_ShadowZSize / 2 ) && z < nMapWidth - 1 )
	{
		// 상
		m_TileRectVert[m_TileNum*4].tu = tu + tile_shadow_z_num;
		m_TileRectVert[m_TileNum*4].tv = tv;
		m_TileRectVert[m_TileNum*4].x = x;
		m_TileRectVert[m_TileNum*4].z = z+1;
		m_TileNum++;

		if( tx < m_ShadowXSize / 2 && x > 0 )
		{
			// 좌상
			m_TileRectVert[m_TileNum*4].tu = tu + tile_shadow_z_num;
			m_TileRectVert[m_TileNum*4].tv = tv - tile_shadow_x_num;
			m_TileRectVert[m_TileNum*4].x = x-1;
			m_TileRectVert[m_TileNum*4].z = z+1;
			m_TileNum++;
		}
		if( tx > nMapPatchSize - ( m_ShadowXSize / 2 ) &&  x < nMapWidth - 1 )
		{
			// 우상
			m_TileRectVert[m_TileNum*4].tu = tu + tile_shadow_z_num;
			m_TileRectVert[m_TileNum*4].tv = tv + tile_shadow_x_num;
			m_TileRectVert[m_TileNum*4].x = x+1;
			m_TileRectVert[m_TileNum*4].z = z+1;
			m_TileNum++;
		}
	}

	// 기본 정보를 사용하여 vertex들을 만든다.
	for( int i = 0; i < m_TileNum; i++ )
	{
		int	x = (int)m_TileRectVert[i*4].x;
		int	z = (int)m_TileRectVert[i*4].z;

		m_TileRectVert[i*4].x = x * fMapPatchSize;
		m_TileRectVert[i*4].y = (float)m_pTerrain->GetHeights( z, x ) + 0.1f;
		//m_TileRectVert[i*4].y = (float) m_pLand->GetHF(x, z);
		m_TileRectVert[i*4].z = z * fMapPatchSize;

		m_TileRectVert[i*4+1].tu = m_TileRectVert[i*4].tu + tile_shadow_z_num;
		m_TileRectVert[i*4+1].tv = m_TileRectVert[i*4].tv;
		m_TileRectVert[i*4+1].x = x * fMapPatchSize;
		m_TileRectVert[i*4+1].y = (float)m_pTerrain->GetHeights( z+1, x ) + 0.1f;
		//m_TileRectVert[i*4+1].y = (float) m_pLand->GetHF(x, z+1);
		m_TileRectVert[i*4+1].z = (z+1) * fMapPatchSize;

		m_TileRectVert[i*4+2].tu = m_TileRectVert[i*4].tu + tile_shadow_z_num;
		m_TileRectVert[i*4+2].tv = m_TileRectVert[i*4].tv + tile_shadow_x_num;
		m_TileRectVert[i*4+2].x = (x+1) * fMapPatchSize;
		m_TileRectVert[i*4+2].y = (float)m_pTerrain->GetHeights( z+1, x+1 ) + 0.1f;
		//m_TileRectVert[i*4+2].y = (float) m_pLand->GetHF(x+1, z+1);
		m_TileRectVert[i*4+2].z = (z+1) * fMapPatchSize;

		m_TileRectVert[i*4+3].tu = m_TileRectVert[i*4].tu;
		m_TileRectVert[i*4+3].tv = m_TileRectVert[i*4].tv + tile_shadow_x_num;
		m_TileRectVert[i*4+3].x = (x+1) * fMapPatchSize;
		m_TileRectVert[i*4+3].y = (float)m_pTerrain->GetHeights( z, x+1 ) + 0.1f;
		//m_TileRectVert[i*4+3].y = (float) m_pLand->GetHF(x+1, z);
		m_TileRectVert[i*4+3].z = z * fMapPatchSize;
	}
}