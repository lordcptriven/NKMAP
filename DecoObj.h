// DecoObj.h: interface for the CDecoObjSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECOOBJ_H__F7833635_8B59_487B_B758_32C17B732EEA__INCLUDED_)
#define AFX_DECOOBJ_H__F7833635_8B59_487B_B758_32C17B732EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CapSyong.h"
class IndexedTexture;

// 건물의 종류에 대한 분류.
#define BLD_HOUSE		0
#define BLD_BLD			1
#define BLD_TREE		2
#define BLD_TABLE		3
#define BLD_CHAIR		4
#define BLD_ETC			5
#define BLD_EVENT_1		6
#define BLD_EVENT_2		7
#define BLD_SHOP		8
#define BLD_FLY			9
#define BLD_STAR		11
#define BLD_FLOOR		12 // [2007/11/5 theodoric] 바닥 속성 추가
#define BLD_TYPE_NUM	13

#define DECO_ALPHA_NONE	0
#define DECO_ALPHA_SEE	1
#define DECO_ALPHA_FAR	2

class CDecoObj
{
protected:
	BOOL		m_bOpenedRoof;
public:
	CDecoObj();
	~CDecoObj();

	CLolos *m_pLolo;						// object 하나.
	int		m_nLoloId;

	float m_wx, m_wy, m_wz, m_dir;			// 월드공간 좌표와 방향
	D3DVECTOR	m_vBoundBox[8];
	D3DVECTOR *m_FloorVect;
	D3DVECTOR *m_InDoorVect;
	D3DVECTOR *m_OutDoorVect;
	D3DVECTOR *m_WalkPlaneVect;

	WORD		m_wColorIdx;
	DWORD		m_dwColorPick;
	int			m_nCurFrame;
	float		m_fTimeElapsed;
	int			m_nFlyFrame;

	static D3DMATRIX	m_matWorld, m_matTrans, m_matRotate;
	static float		m_fAniRate;

	void		CalBoundBox();

	void		OpenRoof() { m_bOpenedRoof = TRUE; };
	void		CloseRoof() { m_bOpenedRoof = FALSE; };
	BOOL		IsRoofOpend() { return m_bOpenedRoof; };
	BOOL		IsInside(float wx, float wz);
	BOOL		MustRenderLast() { return (m_pLolo->MustRenderLast()); }

	//HRESULT Render(LPDIRECT3DDEVICE7 lpd3dDevice, LPDIRECT3DDEVICE7 lpd3dDevPick = NULL, BOOL bNoRoof = FALSE, BOOL bAlpha=FALSE);
	HRESULT Render(LPDIRECT3DDEVICE7 lpd3dDevice, BOOL bNoRoof = FALSE, int whichAlpha=DECO_ALPHA_NONE);
	//HRESULT Render(LPDIRECT3DDEVICE7 lpd3dDevice, float wx, float wy, float wz, LPDIRECT3DDEVICE7 lpd3dDevPick = NULL, BOOL bNoRoof = FALSE);

	// 멀리 있는 것 알파 블렌딩을 위한 Factor, 0.0f: 먼거, 1.0f: 가까운거
	float m_farFactor;
};

class CDecoObjSet
{
protected:
	IndexedTexture	*m_pIndexedTexture;
	char	*m_pLolosDir;

public:
	vector<CLolos*>	m_vecLoloSet;
	vector<char*>	m_vecFileName;
	vector<char*>	m_vecNickName;
	vector<int>		m_vecObjIdx;
	vector<int>		m_vecObjType;
	vector<int>		m_vecRefCount;
	int		m_nLoloCount;
	
	//CLolos	*m_pLoloSet;					// 배열 ,,, 이중포인터가 아니다.
	//char	**m_FileName;
	//char	**m_NickName;
	//int		*m_pnObjIdx;						// object 고유 넘버.
	//int		m_nLoloCount;
	//int		*m_pnObjType;						// 오브젝트의 종류.

	CDecoObjSet(IndexedTexture *pTexture);
	virtual ~CDecoObjSet();

	void SetLolosDir(char *dir_name);
	void ReadDataFile( char *file_name, BOOL bLoadWalkPlane = FALSE );
	char *GetSentence(char *dest, int n, FILE *fp);

	void	AddDecoObj( int nIndex, int nType, char* szNickName, char* szFileName, CLolos* pLolos );
	void	InsertDecoObj( int nIndex, int nType, char* szNickName, char* szFileName, CLolos* pLolos );
	void	RemoveDecoObj( int nIndex );

	bool	SaveDataFile( const TCHAR* szFilename );
};

#endif // !defined(AFX_DECOOBJ_H__F7833635_8B59_487B_B758_32C17B732EEA__INCLUDED_)
