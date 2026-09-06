// NkCharacter.cpp: implementation of the CNkCharacter class.
//
//////////////////////////////////////////////////////////////////////
#define D3D_OVERLOADS
#include "stdafx.h"
#include <d3d.h>
#include "NkCharacter.h"
#include "MyNKApp.h"
#include "Capsyong.h"
#include "ShadowRect.h"
#include "D3DMath.h"


////////////////////////////////////////////////////////////////////////////////
// 전역 함수와 전역 변수 : 포팅할때 증요함.
////////////////////////////////////////////////////////////////////////////////
CBiped		*g_pBipedList[5];
CCapsyong	*g_pBodyList[50];
extern IndexedTexture	*g_pCapsyongTexture;
extern CMyNKApp			*g_pMyNKApp;

// 기타 장비

// 전역 초기화
void PlayerInitialLoading()
{
	int i = 0;
	for( i = 0; i < 5; i++ )
		g_pBipedList[i] = NULL;
	for( i = 0; i < 50; i++ )
		g_pBodyList[i] = NULL;

	//g_pCapsyongTexture = new IndexedTexture(1000);
	g_pCapsyongTexture->SetTexturePath("data/character/textr/");

	// Capsyong 읽기
	CCapsyong	*pCap = new CCapsyong(g_pCapsyongTexture);

	FILE *fp;
	fp = fopen("data/character/sm_body.csy", "rb");
	if (!fp)
		return;
	pCap->Read(fp);
	g_pBodyList[0] = pCap;
	fclose(fp);
/*
	fp = fopen("data/sm_body2.csy", "rb");
	if (!fp)
		return;
	pCap->Read(fp, pCMyApp->GetDevice());
	g_pBodyList[1] = pCap;
	fclose(fp);
*/
	// 바이페드 읽기
	CBiped *pBiped=NULL;

	pBiped = new CBiped;

	fp = fopen("data/character/sm_st_peace.bpd", "rb");
	pBiped->Read(fp);
	fclose(fp);
	g_pBipedList[0] = pBiped;

	/*
	pBiped = new CBiped;
	fp = fopen("data/character/sm_walk_peace.bpd", "rb");
	pBiped->Read(fp);
	g_pBipedList[1] = pBiped;
	fclose(fp);	

	pBiped = new CBiped;
	fp = fopen("data/character/sm_att_unarm1.bpd", "rb");
	pBiped->Read(fp);
	g_pBipedList[2] = pBiped;
	fclose(fp);

	pBiped = new CBiped;
	fp = fopen("data/character/sm_stand_sit.bpd", "rb");
	pBiped->Read(fp);
	g_pBipedList[3] = pBiped;
	fclose(fp);

	pBiped = new CBiped;
	fp = fopen("data/character/sm_sit_stand.bpd", "rb");
	pBiped->Read(fp);
	g_pBipedList[4] = pBiped;
	fclose(fp);

	pBiped = new CBiped;
	fp = fopen("data/character/sm_sitting1.bpd", "rb");
	pBiped->Read(fp);
	g_pBipedList[5] = pBiped;
	fclose(fp);

	pBiped = new CBiped;
	fp = fopen("data/character/sm_sitting2.bpd", "rb");
	pBiped->Read(fp);
	g_pBipedList[6] = pBiped;
	fclose(fp);

	pBiped = new CBiped;
	fp = fopen("data/character/sm_st_1hand.bpd", "rb");
	pBiped->Read(fp);
	g_pBipedList[6] = pBiped;
	fclose(fp);
	
	pBiped = new CBiped;
	fp = fopen("data/character/sm_st_2sword.bpd", "rb");
	pBiped->Read(fp);
	g_pBipedList[7] = pBiped;
	fclose(fp);
	*/
}

void DeletePlayerData()
{
	int i = 0;
	for( i = 0; i < 5; i++ )
	{
		if( g_pBipedList[i] )
		{
			delete g_pBipedList[i];
			g_pBipedList[i] = NULL;
		}
	}
	for( i = 0; i < 50; i++ )
	{
		if( g_pBodyList[i] )
		{
			delete g_pBodyList[i];
			g_pBodyList[i] = NULL;
		}
	}
	if (g_pCapsyongTexture) {
		delete g_pCapsyongTexture;
		g_pCapsyongTexture = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// CNkCharacter Class

// static member variables
float CNkCharacter::m_fWalkSpeed = 5.0f;
float CNkCharacter::m_fRunSpeed = 8.0f;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNkCharacter::CNkCharacter( CShadowRect* pShadowRect )
{
	m_Biped = NULL;

	int i = 0;
	for (i = 0; i < CAPSYONG_NUM; i++)
		m_CapSyong[i] = NULL;

	m_CurFrame = 0;

	for (i = 0; i < CAPSYONG_NUM + 2; i++)
		m_bShow[i] = TRUE;

	m_pShadowRect = pShadowRect;

	m_dwBaseTime = timeGetTime();
	m_AniRate = 0.05f;

	m_bMoving = FALSE;
	
	m_nVirId = -1;
	m_pstrName = NULL;

	m_wColorIdx = 0;
	m_dwColorPick = 0x00ffffff;
}

CNkCharacter::~CNkCharacter()
{
	m_pShadowRect = NULL;
	if (m_pstrName != NULL) {
		delete [] m_pstrName;
		m_pstrName = NULL;
	}
}

void CNkCharacter::FrameMove(unsigned int nInc)
{
	int i;

	// CapSyong들 Animation
	for (i = 0; i < CAPSYONG_NUM; i++) {
		if (m_bShow[i] && m_CapSyong[i]) {			
			if (m_bEddy && i == WEAPON) {
				//m_CapSyong[i]->Animate(NULL, m_EddyFrame, &matWorld, &m_ViewMat);
				m_CapSyong[i]->Animate(NULL, m_EddyFrame, NULL, NULL);
			} else {
				//m_CapSyong[i]->Animate(m_Biped, m_CurFrame, &matWorld, &m_ViewMat);
				m_CapSyong[i]->Animate(m_Biped, m_CurFrame, NULL, NULL);
			}
		}
	}

	// Frame 증가
	//m_CurFrame++;
	m_CurFrame += nInc;
	if (m_CurFrame >= m_Biped->m_AnimCount) {
		m_CurFrame = 0;
		// 모션을 복귀시키는 코드가 있으면 좋겠다.
	}
}

void CNkCharacter::SetBiped(CBiped *pBiped)
{
	m_dwBaseTime = timeGetTime();
	m_dwTimeCapPrev = m_dwBaseTime;
	m_Biped = pBiped;
}

void CNkCharacter::SetCapsyong(CCapsyong *pCapsyong, int capsyong_num)
{
	m_CapSyong[capsyong_num] = pCapsyong;
	m_dwBaseTime = timeGetTime();
}

void CNkCharacter::SetInitPos(float wx, float wz)
{
	m_wx = wx; m_wz = wz;
	m_wy = g_pMyNKApp->GetTerrain()->GetHeights( m_wx, m_wz );
}

void CNkCharacter::SetInitPos(float wx, float wy, float wz)
{
	m_wx = wx; m_wy = wy; m_wz = wz;
}

// --------------------------------------------------------------------
// SetInfo()
// Desc : 캐릭터에 대한 정보를 넣어준다.
// --------------------------------------------------------------------
void CNkCharacter::SetInfo(int id, const char *name)
{
	m_nVirId = id;
	int len = strlen(name);
	if (m_pstrName != NULL)
		delete [] m_pstrName;
	m_pstrName = new char[len+1];
	strcpy(m_pstrName, name);
}

// --------------------------------------------------------------------
// Move() 
// Desc : 월드 공간 wx, wz로 이동을 한다.
// --------------------------------------------------------------------
BOOL CNkCharacter::Move(float tx, float tz)
{
	float mag;
	// 현재의 상태를 저장한다.
	//SetMotionState(SM_WALK_PEACE);

	// 타겟지점 저정
	m_tx = tx; m_tz = tz;

	m_dx = m_tx - m_wx;	
	m_dz = m_tz - m_wz;

	m_dir = (float) atan2(m_dz, m_dx);

	mag = m_dx*m_dx + m_dz*m_dz;
	mag = (float) sqrt( (double) mag );

	if (mag < g_EPSILON) {
		m_bMoving = FALSE;
		return FALSE;
	}

	m_dx = m_dx / mag;
	m_dz = m_dz / mag;

	m_dx *= (m_fRunSpeed/4.0f);
	m_dz *= (m_fRunSpeed/4.0f);

	m_bMoving = TRUE;

	return TRUE;
}

// --------------------------------------------------------------------
// Attack() 
// Desc : (tx, tz)를 향하여 공격 모션을 취한다.
// --------------------------------------------------------------------
void CNkCharacter::Attack(float tx, float tz)
{
	if (m_bAttack)
		return;

	// 타겟지점 저정
	m_tx = tx; m_tz = tz;

	m_dx = m_tx - m_wx;	
	m_dz = m_tz - m_wz;

	m_dir = (float) atan2(m_dz, m_dx);

	m_bAttack = TRUE;
	m_CurFrame = 0;
	SetMotionState(SM_ATTACK_UNARMED);
}

void CNkCharacter::SetMotionState(unsigned short nState)
{
	m_nMotionState = nState;
	SetBiped(g_pBipedList[nState]);
}

void CNkCharacter::Render(LPDIRECT3DDEVICE7 lpd3dDevice, LPDIRECT3DDEVICE7 lpd3dDevPick)
{
	D3DMATRIX matTrans, matRotate, matWorld;
	DWORD dwCurtime;
	DWORD dwTimeCap;
	unsigned int frame_count;
	BOOL bArrived = FALSE;

	// 다음 프레임 이동.

	dwCurtime = timeGetTime();
	//ftime = (timeGetTime() - m_dwBaseTime) * 0.001f;

	frame_count = 0;
	if ( ((dwCurtime - m_dwBaseTime) * 0.001f) >= m_AniRate)
	{
		
		m_dwBaseTime += (DWORD) (m_AniRate*1000);
		frame_count = 1;
		while ( ((dwCurtime - m_dwBaseTime)*0.001f) >= m_AniRate )
		{
			frame_count++;
			m_dwBaseTime += (DWORD) (m_AniRate*1000);
		}

		// 이동 거리 계산.. 음.. 수정 필요...
		
	}	
	
	dwTimeCap = dwCurtime - m_dwTimeCapPrev;
	m_dwTimeCapPrev = dwCurtime;
	
		if (m_bMoving)////////
		{
			//float tx = m_dx / 2.0f;
			//float tz = m_dz / 2.0f;
			float tx = (m_dx * ((float)dwTimeCap / 50.0f));
			float tz = (m_dz * ((float)dwTimeCap / 50.0f));
			m_wx += tx;
			m_wz += tz;

			// 1/4
			if ( m_dx >= 0 && m_dz >= 0) {
				if (m_wx >= m_tx && m_wz >= m_tz) {
					bArrived = TRUE;
				}
			} // 2 / 4
			else if ( m_dx < 0 && m_dz >= 0) {
				if (m_wx <= m_tx && m_wz >= m_tz) {
					bArrived = TRUE;
				}
			} // 3/4
			else if ( m_dx <= 0 && m_dz <= 0) {
				if (m_wx <= m_tx && m_wz <= m_tz) {
					bArrived = TRUE;
				}
			} // 4/4
			else if ( m_dx >= 0 && m_dz <= 0) {
				if (m_wx >= m_tx && m_wz <= m_tz) {
					bArrived = TRUE;
				}
			} else
				;

			if (bArrived) {
				m_wx = m_tx;
				m_wz = m_tz;
				m_bMoving = FALSE;
			}

			if (m_bMoving == FALSE) {
				// 스테이트를 바꾸어 주어야 한다.
				SetMotionState(SM_ST_PEACE);
			}
			
			m_wy = g_pMyNKApp->GetTerrain()->GetHeights( m_wx, m_wz );
		}////////

		if (m_bAttack) {
			if ( (m_CurFrame+frame_count) >= m_Biped->m_AnimCount) {
				m_bAttack = FALSE;
				SetMotionState(SM_ST_PEACE);
			}
		}

	// 다음 프레임 계산
	FrameMove(frame_count);
	//FrameMove(0);

	if( m_pShadowRect )		// 그림자 그리는 옵션이 켜 있으면 그림자를 그려라.
	{
		//m_pShadowRect->SetWorldCoord(D3DVECTOR(m_wx, m_wy, m_wz));
		//m_pShadowRect->Render(lpd3dDevice);
		m_pShadowRect->SetTileCoord( D3DXVECTOR3( m_wx, m_wy, m_wz ), 15, FALSE );
		m_pShadowRect->Render( lpd3dDevice, NULL, FALSE );
	}

	// 월드 트랜스폼. - 캐릭터를 그리기 위한.
	D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
	lpd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	// CCapsyong들 출력
	//lpd3dDevice->LightEnable(1, TRUE);

	LPDIRECTDRAWSURFACE7 traceTexture = NULL, lightswordTexture = NULL;
	int i = 0;
	for (i = 0; i < CAPSYONG_NUM; i++) {
		if (m_bShow[i] && m_CapSyong[i]) {
			
			if (i == WEAPON) {
				traceTexture = m_TraceTexture->GetSurfaceByIdx(0);
				lightswordTexture = m_LightswordTexture->GetSurfaceByIdx(0);
			}

			//m_CapSyong[i]->Render(lpd3dDevice, bShowLow, traceTexture, lightswordTexture, m_LightswordFrame);			
			m_CapSyong[i]->Render(lpd3dDevice);			
		}
	}
	//lpd3dDevice->LightEnable(1, FALSE);

	// pick buffer 쓰기.
	if (lpd3dDevPick) {
		lpd3dDevPick->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		if( SUCCEEDED( lpd3dDevPick->BeginScene() ) ) {
			lpd3dDevPick->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			lpd3dDevPick->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick );
			for (i = 0; i < CAPSYONG_NUM; i++) {
				if (m_bShow[i] && m_CapSyong[i]) {			
					if (i == WEAPON) {
						traceTexture = m_TraceTexture->GetSurfaceByIdx(0);
						lightswordTexture = m_LightswordTexture->GetSurfaceByIdx(0);
					}

					//m_CapSyong[i]->Render(lpd3dDevice, bShowLow, traceTexture, lightswordTexture, m_LightswordFrame);			
					m_CapSyong[i]->Render(lpd3dDevPick, TRUE);			
				}
			}
			lpd3dDevPick->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		}
		// End the scene.
		lpd3dDevPick->EndScene();
	}
}
