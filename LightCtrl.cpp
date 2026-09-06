// LightCtrl.cpp: implementation of the CLightCtrl class.
//
//////////////////////////////////////////////////////////////////////
#define D3D_OVERLOADS
#include "StdAfx.h"
#include <windows.h>
#include "D3DUtil.h"
#include "LightCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLightCtrl::CLightCtrl()
{
	m_dwAmbiColor = 0x00777777;
	m_fCurRange = 0.0f;
}

CLightCtrl::~CLightCtrl()
{

}

void CLightCtrl::InitLight(int light_num, D3DLIGHTTYPE ltType, FLOAT x, FLOAT y, FLOAT z )
{
	if (light_num < 0 || light_num >= NKLIGHT_NUM) {
		MessageBox(NULL, "패러미터를 잘못 넘겨줬습니다", "Warning", MB_OK);
	}

	ZeroMemory( &m_d3dLight[light_num], sizeof(D3DLIGHT7) );
    m_d3dLight[light_num].dltType        = ltType;
    m_d3dLight[light_num].dcvDiffuse.r   = 1.0f;
    m_d3dLight[light_num].dcvDiffuse.g   = 1.0f;
    m_d3dLight[light_num].dcvDiffuse.b   = 1.0f;
    m_d3dLight[light_num].dcvSpecular    = m_d3dLight[light_num].dcvDiffuse;
    m_d3dLight[light_num].dvPosition.x   = x;
    m_d3dLight[light_num].dvPosition.y   = y;
    m_d3dLight[light_num].dvPosition.z   = z;
	m_d3dLight[light_num].dvDirection = Normalize(D3DVECTOR(x, y, z));
	/*
	m_d3dLight[light_num].dvDirection.x = x;
	m_d3dLight[light_num].dvDirection.y = y;
	m_d3dLight[light_num].dvDirection.z = z;
	*/
    m_d3dLight[light_num].dvAttenuation0 = 1.0f;
    m_d3dLight[light_num].dvRange        = D3DLIGHT_RANGE_MAX;
}

void CLightCtrl::InitLight( int light_num, D3DLIGHTTYPE ltType, FLOAT x, FLOAT y, FLOAT z,
						FLOAT r, FLOAT g, FLOAT b )
{
	if (light_num < 0 || light_num >= NKLIGHT_NUM) {
		MessageBox(NULL, "패러미터를 잘못 넘겨줬습니다", "Warning", MB_OK);
	}

	ZeroMemory( &m_d3dLight[light_num], sizeof(D3DLIGHT7) );
    m_d3dLight[light_num].dltType        = ltType;
    m_d3dLight[light_num].dcvDiffuse.r   = r;
    m_d3dLight[light_num].dcvDiffuse.g   = g;
    m_d3dLight[light_num].dcvDiffuse.b   = b;
    m_d3dLight[light_num].dcvSpecular    = m_d3dLight[light_num].dcvDiffuse;
    m_d3dLight[light_num].dvPosition.x   = x;
    m_d3dLight[light_num].dvPosition.y   = y;
    m_d3dLight[light_num].dvPosition.z   = z;
	m_d3dLight[light_num].dvDirection = Normalize(D3DVECTOR(x, y, z));
	/*
	m_d3dLight[light_num].dvDirection.x = x;
	m_d3dLight[light_num].dvDirection.y = y;
	m_d3dLight[light_num].dvDirection.z = z;
	*/
	m_d3dLight[light_num].dvAttenuation0 = 1.0f;
	m_d3dLight[light_num].dvRange        = D3DLIGHT_RANGE_MAX;
}

void CLightCtrl::LightingEnalbe(LPDIRECT3DDEVICE7 lpd3dDevice, BOOL bEnable)
{
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, bEnable);
}

void CLightCtrl::LightEnable(LPDIRECT3DDEVICE7 lpd3dDevice, int light_num, BOOL bEnable)
{
	if (light_num < 0 || light_num >= NKLIGHT_NUM) {
		MessageBox(NULL, "패러미터를 잘못 넘겨줬습니다", "Warning", MB_OK);
	}

	lpd3dDevice->LightEnable(light_num, bEnable);
}

void CLightCtrl::LightAllEnalbe(LPDIRECT3DDEVICE7 lpd3dDevice, BOOL bEnable)
{
	for (int i=0; i < NKLIGHT_NUM; i++) {
		lpd3dDevice->LightEnable(i, bEnable);
	}
}

void CLightCtrl::SetLight(LPDIRECT3DDEVICE7 lpd3dDevice, int light_num)
{
	if (light_num < 0 || light_num >= NKLIGHT_NUM) {
		MessageBox(NULL, "패러미터를 잘못 넘겨줬습니다", "Warning", MB_OK);
	}

	lpd3dDevice->SetLight(light_num, &m_d3dLight[light_num]);
}

void CLightCtrl::SetAmbientLight(LPDIRECT3DDEVICE7 lpd3dDevice, DWORD dwColor)
{
	lpd3dDevice->SetRenderState( D3DRENDERSTATE_AMBIENT,  dwColor);
}

void CLightCtrl::SetCurLandLight(LPDIRECT3DDEVICE7 lpd3dDevice)
{
	lpd3dDevice->SetRenderState( D3DRENDERSTATE_AMBIENT,  m_dwAmbiColor);
}

void CLightCtrl::IncAmbientLight(LPDIRECT3DDEVICE7 lpd3dDevice)
{
	m_dwAmbiColor += 0x00050505;

	if (m_dwAmbiColor >= 0x00EEEEEE)
		m_dwAmbiColor = 0x00EEEEEE;
}

void CLightCtrl::DecAmbientLight(LPDIRECT3DDEVICE7 lpd3dDevice)
{
	m_dwAmbiColor -= 0x00050505;

	if (m_dwAmbiColor <= 0x00101010)
		m_dwAmbiColor = 0x00101010;
}

void CLightCtrl::SetDayLight(LPDIRECT3DDEVICE7 lpd3dDevice)
{
	m_dwAmbiColor = 0x00777777;
	lpd3dDevice->SetRenderState( D3DRENDERSTATE_AMBIENT,  m_dwAmbiColor);
}

void CLightCtrl::SetNightLight(LPDIRECT3DDEVICE7 lpd3dDevice)
{
	m_dwAmbiColor = 0x00233081;
	lpd3dDevice->SetRenderState( D3DRENDERSTATE_AMBIENT,  m_dwAmbiColor);
}

BOOL CLightCtrl::IsEnabled(LPDIRECT3DDEVICE7 lpd3dDevice, int light_num)
{
	BOOL bEnabled;

	lpd3dDevice->GetLightEnable(light_num, &bEnabled);

	return bEnabled;
}