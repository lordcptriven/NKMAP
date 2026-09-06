// NkCharacter.h: interface for the CNkCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NKCHARACTER_H__AFA8B0D2_2EFF_49BC_A4B8_E77536B2F710__INCLUDED_)
#define AFX_NKCHARACTER_H__AFA8B0D2_2EFF_49BC_A4B8_E77536B2F710__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "IndexedTexture.h"
#include "ChaSta.h"


#define BODY			0
#define HELMET			1
#define ARMOR			2
#define WEAPON			3
#define SHOES			4
#define CAPSYONG_NUM	5

#define CHA_STAND		0
#define CHA_WALK		1
#define CHA_RUN			2
#define CHA_ATTACK		3
#define CHA_SWIM		4
#define CHA_SWIM_STAND	5

class CBiped;
class CAniMatrix;
class CCsyObj;
class CCapsyong;
class CShadowRect;


class CNkCharacter  
{
private:
	unsigned char	m_cMotionSort;				// 움직임, 걷기, 뛰기...
	unsigned char   m_cChaType; 				// 캐릭터 타입 (체형적인 타입)
	unsigned char	m_cSex;						// 성별

	CBiped *m_Biped;
	CCapsyong *m_CapSyong[CAPSYONG_NUM];
	
	int m_CurFrame;
	int m_EddyFrame;							// 무엇에 쓰는 물건인고..

	BOOL		m_bShow[ CAPSYONG_NUM + 2 ];
	BOOL		m_bEddy;

	//Motion State
	unsigned short m_nMotionState;

	// 텍스처
	IndexedTexture *m_CapsyongTexture;
	IndexedTexture *m_TraceTexture;
	IndexedTexture *m_LightswordTexture;

	int m_LightswordFrame;

	// 타임 체크.
	DWORD			m_dwBaseTime, m_dwTimeCapPrev;
	float			m_AniRate;

protected:
	static float	m_fRunSpeed;
	static float	m_fWalkSpeed;
	CShadowRect*	m_pShadowRect;

	// 모션에 관련된 변수
	BOOL			m_bMoving, m_bAttack;
	float			m_dx, m_dz;						// 이동 방향 벡터.
	float			m_tx, m_tz;						// target (x, z) 좌표.
	
public:
	CNkCharacter( CShadowRect* pShadowRect = NULL );
	~CNkCharacter();

	// 캐릭터의 특성값.
	int			m_nVirId;
	WORD		m_wColorIdx;						// 컬러 테이블 인덱스
	DWORD		m_dwColorPick;						// 컬러 패러미터.
	char		*m_pstrName;

	void		SetInfo(int id, const char *name);

	// 사용자 위치와 방향 정보 : world coordinate
	float		m_wx, m_wy, m_wz, m_dir;

	BOOL		Move(float tx, float tz);
	void		Attack(float tx, float tz);

	// 초기화 함수
	void	SetBiped(CBiped *pBiped);
	void	SetCapsyong(CCapsyong *pCapsyong, int capsyong_num);
	void	SetInitPos(float wx, float wy, float wz);
	void	SetInitPos(float wx, float wz);
	
	// 프레임 계산하는 함수
	void	FrameMove(unsigned int nInc);	

	// 렌더링 함수
	void	Render(LPDIRECT3DDEVICE7 lpd3dDevice, LPDIRECT3DDEVICE7 lpd3dDevPick = NULL);
	//void	CsyRender(LPDIRECT3DDEVICE7 lpd3dDevice, CsyObj *pCsy);

	// 캐릭터에 대한 명령 함수
	void	SetMotionState(unsigned short nState);	
	unsigned short GetMotionState() {return m_nMotionState; };
	BOOL	IsMoving() { return m_bMoving; };
};

#endif // !defined(AFX_NKCHARACTER_H__AFA8B0D2_2EFF_49BC_A4B8_E77536B2F710__INCLUDED_)
