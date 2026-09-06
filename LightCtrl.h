// LightCtrl.h: interface for the CLightCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCTRL_H__06C06D32_A2C5_4B3F_9ED9_840E66F6451A__INCLUDED_)
#define AFX_LIGHTCTRL_H__06C06D32_A2C5_4B3F_9ED9_840E66F6451A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NKLIGHT_DIRECTIONAL 0
#define NKLIGHT_POINT		1
#define NKLIGHT_NUM			4

// 게임내 실시간 라이트를 조절하기 위한 랩퍼 클래스
// 특별한 기능은 없다.

// 낮에는 directional light를 뿌리고
// 밤에는 point light를 뿌린다.

class CLightCtrl  
{
public:
	CLightCtrl();
	virtual ~CLightCtrl();

	D3DLIGHT7 m_d3dLight[NKLIGHT_NUM];
	DWORD	m_dwAmbiColor;
	
	void	InitLight(int light_num, D3DLIGHTTYPE ltType, FLOAT x=0.0f, FLOAT y=0.0f, FLOAT z=0.0f );
	void	InitLight(int light_num, D3DLIGHTTYPE ltType, FLOAT x=0.0f, FLOAT y=0.0f, FLOAT z=0.0f,
						FLOAT r=1.0f, FLOAT g=1.0f, FLOAT b=1.0f );
	void	LightingEnalbe(LPDIRECT3DDEVICE7 lpd3dDevice, BOOL bEnable);
	void	LightEnable(LPDIRECT3DDEVICE7 lpd3dDevice, int light_num, BOOL bEnable);
	void	LightAllEnalbe(LPDIRECT3DDEVICE7 lpd3dDevice, BOOL bEnable);
	void	SetLight(LPDIRECT3DDEVICE7 lpd3dDevice, int light_num);
	BOOL	IsEnabled(LPDIRECT3DDEVICE7 lpd3dDevice, int light_num);

	void	SetAmbientLight(LPDIRECT3DDEVICE7 lpd3dDevice, DWORD dwColor);
	void	SetCurLandLight(LPDIRECT3DDEVICE7 lpd3dDevice);
	void	IncAmbientLight(LPDIRECT3DDEVICE7 lpd3dDevice);
	void	DecAmbientLight(LPDIRECT3DDEVICE7 lpd3dDevice);
	void	SetDayLight(LPDIRECT3DDEVICE7 lpd3dDevice);
	void	SetNightLight(LPDIRECT3DDEVICE7 lpd3dDevice);
	void    SetPointLightRange(float range){ m_fCurRange = range;}
	float   GetPointLightRange() { return m_fCurRange;}
	float     m_fCurRange;

	// 5월 2일에 추가하는 것.
	/*
	void	SetDayAmbientLight(LPDIRECT3DDEVICE7 lpd3dDevice);
	void	SetDayDirectionalLight(LPDIRECT3DDEVICE7 lpd3dDevice);
	void	SetNightAmbientLight(LPDIRECT3DDEVICE7 lpd3dDevice);
	void	SetNightDirectionalLight(LPDIRECT3DDEVICE7 lpd3dDevice);	
	*/
};

#endif // !defined(AFX_LIGHTCTRL_H__06C06D32_A2C5_4B3F_9ED9_840E66F6451A__INCLUDED_)
