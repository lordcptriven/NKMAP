// SkyPlane.h: interface for the CSkyPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKYPLANE_H__C27A21DA_251A_4245_8BB9_63DAAEF67187__INCLUDED_)
#define AFX_SKYPLANE_H__C27A21DA_251A_4245_8BB9_63DAAEF67187__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SKYPLANE_DIV	20
#define SKYPLANE_TIME	50000

class IndexedTexture;

class CSkyPlane  
{
public:
	CSkyPlane();
	CSkyPlane(IndexedTexture *pIndexedTexture);
	virtual ~CSkyPlane();

	LPDIRECT3DDEVICE7 m_pd3dDevice;
	IndexedTexture *m_pIndexedTexture;
	int m_textrSky[6];

	float m_PlanetRadius;
	float m_AtmosphereRadius;
	float m_HTileFactor;
	float m_VTileFactor;

	D3DLVERTEX m_Vertices[(SKYPLANE_DIV+1)*(SKYPLANE_DIV+1)];
	unsigned short m_Indices[SKYPLANE_DIV*SKYPLANE_DIV*6];
	float m_tU[(SKYPLANE_DIV+1)*(SKYPLANE_DIV+1)];

	BOOL Init(float planetRadius, float atmosphereRadius, float hTilefactor, float vTilefactor, float scale);
	void SetDevice(LPDIRECT3DDEVICE7 lpd3dDevice) { m_pd3dDevice = lpd3dDevice; };
	BOOL SetupVertices(float scale);
	BOOL SetupIndices();
	void Render(float x, float y, float z, BOOL bFog=TRUE);
	void SetFog(BOOL bFog);
};

#endif // !defined(AFX_SKYPLANE_H__C27A21DA_251A_4245_8BB9_63DAAEF67187__INCLUDED_)
