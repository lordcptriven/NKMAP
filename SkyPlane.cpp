// SkyPlane.cpp: implementation of the CSkyPlane class.
//
//////////////////////////////////////////////////////////////////////

#define D3D_OVERLOADS
#include "StdAfx.h"
#include <windows.h>
#include <d3d.h>

#include "IndexedTexture.h"
#include "SkyPlane.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "MyNKApp.h"
#include "NKTerrain.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern IndexedTexture*	g_pCapsyongTexture;
extern CMyNKApp*		g_pMyNKApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkyPlane::CSkyPlane()
{
	m_PlanetRadius = 0.0f;
	m_AtmosphereRadius = 0.0f;
	m_HTileFactor = 1.0f;
	m_VTileFactor = 1.0f;

	m_pIndexedTexture = g_pCapsyongTexture;

	m_pIndexedTexture->SetTexturePath( "data/testtex/" );
	m_textrSky[0] = m_pIndexedTexture->CreateTextureFromFile("sebuk_sky.bmp");
	m_textrSky[1] = m_pIndexedTexture->CreateTextureFromFile("day_sky.bmp");
	m_textrSky[2] = m_pIndexedTexture->CreateTextureFromFile("noul_sky.bmp");
	m_textrSky[3] = m_pIndexedTexture->CreateTextureFromFile("night_sky.bmp");
	m_textrSky[4] = m_pIndexedTexture->CreateTextureFromFile("flow_cloud3.bmp");
	m_textrSky[5] = m_pIndexedTexture->CreateTextureFromFile("mat_sky.bmp");	// matrix
}

CSkyPlane::CSkyPlane(IndexedTexture *pIndexedTexture)
{
	m_PlanetRadius = 0.0f;
	m_AtmosphereRadius = 0.0f;
	m_HTileFactor = 1.0f;
	m_VTileFactor = 1.0f;

	m_pIndexedTexture = pIndexedTexture;
	m_pIndexedTexture->SetTexturePath( "data/testtex/" );
	m_textrSky[0] = m_pIndexedTexture->CreateTextureFromFile("sky/flow_cloud3.bmp");
}

CSkyPlane::~CSkyPlane()
{
}

// Initialize
BOOL CSkyPlane::Init(float planetRadius,
                          float atmosphereRadius,
                          float hTilefactor,
                          float vTilefactor,
						  float scale)
{
	//atmosphereRadius = 295.0f;
	m_PlanetRadius = planetRadius;
	m_AtmosphereRadius = atmosphereRadius;
	m_HTileFactor = hTilefactor;
	m_VTileFactor = vTilefactor;

	if (!SetupVertices(scale))
	{
		OutputDebugString("unable to set up verts");
		return FALSE;
	}

	if (!SetupIndices())
	{
		OutputDebugString("unable to set up indices");
		return FALSE;
	}

	return TRUE;
}

// Generate vertices that describe sphere geometry
BOOL CSkyPlane::SetupVertices(float scale)
{
	float plane_size = 2.0f * (float)sqrt((m_AtmosphereRadius*m_AtmosphereRadius) - 
						(m_PlanetRadius*m_PlanetRadius));

	float delta = plane_size/(float)SKYPLANE_DIV;
	float tex_delta = 2.0f/(float)SKYPLANE_DIV;
	float x_dist   = 0.0f;
	float z_dist   = 0.0f;
	float x_height = 0.0f;
	float z_height = 0.0f;
	float height = 0.0f;

	D3DMATRIX matScale;
	D3DUtil_SetScaleMatrix(matScale, scale, scale, scale);

	for (int i=0;i<=SKYPLANE_DIV;i++)
	{
		for (int j=0;j<=SKYPLANE_DIV;j++)
		{
			x_dist = (-0.5f * plane_size) + ((float)j*delta);
			z_dist = (-0.5f * plane_size) + ((float)i*delta);

			x_height = (x_dist*x_dist) / m_AtmosphereRadius;
			z_height = (z_dist*z_dist) / m_AtmosphereRadius;
			height = x_height + z_height;

			D3DVECTOR v = D3DVECTOR( x_dist, 0.0f - height, z_dist);
			D3DMath_VectorMatrixMultiply(v, v, matScale);
			m_tU[i*(SKYPLANE_DIV+1)+j] = m_HTileFactor*((float)j * tex_delta*0.5f);
			m_Vertices[i*(SKYPLANE_DIV+1)+j] =
				D3DLVERTEX(v, 0x00FFFFFF, 0,
					m_tU[i*(SKYPLANE_DIV+1)+j], m_VTileFactor*(1.0f - (float)i * tex_delta*0.5f));
		}
	}

	return TRUE;
}


BOOL CSkyPlane::SetupIndices()
{
	int index = 0;

	for (int i=0;i<SKYPLANE_DIV;i++)
	{
		for (int j=0;j<SKYPLANE_DIV;j++)
		{
			int startvert = (i*(SKYPLANE_DIV+1) + j);

			// tri 1
			m_Indices[index++] = startvert;
			m_Indices[index++] = startvert+1;
			m_Indices[index++] = startvert+SKYPLANE_DIV+1;
			//m_Indices[index++] = startvert+SKYPLANE_DIV+1;
			//m_Indices[index++] = startvert+1;

			// tri 2
			m_Indices[index++] = startvert+1;
			m_Indices[index++] = startvert+SKYPLANE_DIV+2;
			m_Indices[index++] = startvert+SKYPLANE_DIV+1;
			//m_Indices[index++] = startvert+SKYPLANE_DIV+1;
			//m_Indices[index++] = startvert+SKYPLANE_DIV+2;
		}
	}

	return TRUE;
}

void CSkyPlane::Render(float x, float y, float z, BOOL bFog)
{
	D3DMATRIX matWorld;

	// 하늘을 이동시키고...
	//D3DUtil_SetTranslateMatrix( matWorld, x, 565.0f, z);
	D3DUtil_SetTranslateMatrix( matWorld, x, y, z);
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	//m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);

	// 텍스춰 좌표를 이동시켜주자!
	static float	fTime = 0.0f;
	fTime += g_pMyNKApp->GetElapsedTime();
	DWORD			dwTime = fTime * 2000.0f;
	for (int i=0;i<=SKYPLANE_DIV;i++)
	{
		for (int j=0;j<=SKYPLANE_DIV;j++)
		{
			m_Vertices[i*(SKYPLANE_DIV+1)+j].tu = m_tU[i*(SKYPLANE_DIV+1)+j] +
				dwTime % SKYPLANE_TIME * 1.0f / SKYPLANE_TIME;
		}
	}
	// 설정하고 그리자!
	SetFog(bFog);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING , FALSE);

	LPDIRECTDRAWSURFACE7 lpSurfaceSky;

	int	nWorldTime = g_pMyNKApp->GetWorldTime();
	if( nWorldTime >= 2 && nWorldTime <= 6 )			// 새벽
		lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[0]);
	else if( nWorldTime >= 7 && nWorldTime <= 21)		// 낮
		lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[1]);
	else if( nWorldTime >= 22 && nWorldTime <= 27 )		// 저녁
		lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[2]);
	else												// 밤
		lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[3]);

	m_pd3dDevice->SetTexture(0, lpSurfaceSky);
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
	m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX,
									 m_Vertices, (SKYPLANE_DIV+1)*(SKYPLANE_DIV+1),
									 m_Indices, SKYPLANE_DIV*SKYPLANE_DIV*6, 0 );

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING , TRUE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
}

extern float g_cS, g_cE, g_cD;

DWORD g_skyFogColor[30] = {
	RGBA_MAKE(25, 53, 115, 255), RGBA_MAKE(25, 53, 115, 255),	// 0, 1
	RGBA_MAKE(135, 198, 255, 255), RGBA_MAKE(135, 198, 255, 255),	// 새벽 2, 3
	RGBA_MAKE(135, 198, 255, 255), RGBA_MAKE(135, 198, 255, 255),	// 4, 5
	RGBA_MAKE(135, 198, 255, 255), RGBA_MAKE(134, 177, 210, 255),	// 6, 낮 7
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 8, 9
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 10, 11
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 12, 13
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 14, 15
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 16, 17
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 18, 19
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 20, 21
	RGBA_MAKE(255, 185, 140, 255), RGBA_MAKE(255, 185, 140, 255),	// 저녁 22, 23
	RGBA_MAKE(255, 185, 140, 255), RGBA_MAKE(255, 185, 140, 255),	// 24, 25
	RGBA_MAKE(255, 185, 140, 255), RGBA_MAKE(255, 185, 140, 255),	// 26, 27
	RGBA_MAKE(25, 53, 115, 255), RGBA_MAKE(25, 53, 115, 255),	// 밤 28, 29
};

void CSkyPlane::SetFog(BOOL bFog)
{
	if (bFog) {
		BOOL bUsingTableFog = FALSE;		// pixel fog가 table 포그이다.
		BOOL bRangeBasedFog = FALSE;		// 지원하는 하드웨어가 없다. 오직 vertexfog에서 cpu를 잡는다.
		
		float fFogStart = (1.16f * (6000.0f-20.0f)) + 20.0f; // ( fStart*(m_zFar-m_zNear) ) + m_zNear
		float fFogEnd = (1.4f * (6000.0f-20.0f)) + 20.0f; // ( fEnd*(m_zFar-m_zNear) ) + m_zNear;
		//float fFogStart = (g_cS * (6000.0f-20.0f)) + 20.0f; // ( fStart*(m_zFar-m_zNear) ) + m_zNear
		//float fFogEnd = (g_cE * (6000.0f-20.0f)) + 20.0f; // ( fEnd*(m_zFar-m_zNear) ) + m_zNear;
		float fFogDensity = 0.0f;
		//float fFogDensity = g_cD;
		//DWORD dwFogColor = RGBA_MAKE ( 163, 189, 255, 255 );
		DWORD dwFogColor;
//		if (pCMyApp->GetCurWorld() == WORLD_SKY) {
//			dwFogColor = RGBA_MAKE ( 57, 81, 156, 255 );
//		} else {
			dwFogColor = g_skyFogColor[g_pMyNKApp->GetWorldTime()];
//		}

		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, dwFogColor);
		
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&fFogStart)));
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&fFogEnd)));
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&fFogDensity)));
		
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  D3DFOG_LINEAR );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, bRangeBasedFog );
		
	} else {
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
	}
}