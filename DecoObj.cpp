// DecoObj.cpp: implementation of the CDecoObjSet class.
//
//////////////////////////////////////////////////////////////////////
#define D3D_OVERLOADS
#include "StdAfx.h"
#include <d3d.h>
#include <assert.h>
#include "MyNKApp.h"
#include "DecoObj.h"
#include "D3DMath.h"
#include "IndexedTexture.h"

#include <list>
#include <iterator>
using namespace std;

CDecoObjSet*			g_pDecoObjSet = NULL;
extern CNKObjQuadNode*	g_pObjTree;
extern list<CDecoObj*>	g_lstWalkObj[64][64];
extern CMyNKApp*		g_pMyNKApp;

#define DECO_FLY_FRAME		40
float	g_DecoFlyHeight[DECO_FLY_FRAME];

void DeleteDecoObjData();

void DecoObjInitialLoading( const char *strFilePath, const char *strFilename )
{
	FILE *fp = NULL;
//	int listNum;
	CDecoObj *pDecoObj = NULL;
	char *strFilePathName = NULL;

	// 기존에 로딩되어 있는 것들을 모두 지운다.
	DeleteDecoObjData();

	strFilePathName = new char[255];

	//현재 건물들의 텍스처는 모두 한 곳에.
	g_pMyNKApp->GetDecoTexture()->SetTexturePath("data//building//textr//");

	SAFE_DELETE(g_pDecoObjSet);
	g_pDecoObjSet = new CDecoObjSet(g_pMyNKApp->GetDecoTexture());

	// 롤로 디렉토리 설정.
	strcpy(strFilePathName, strFilePath);
	strcat(strFilePathName, "lolos/");
	g_pDecoObjSet->SetLolosDir(strFilePathName);

	// bld 화일 설정.
	if( strFilename == NULL )
	{
		strcpy(strFilePathName, strFilePath);
		strcat(strFilePathName, "bld.bld");
	}
	else
	{
		if( g_pMyNKApp->IsLoadedGameMap() )
		{
			strcpy(strFilePathName, strFilePath);
			strcat( strFilePathName, strFilename );
			strcat( strFilePathName, ".bld" );
		}
		else
		{
			strcpy(strFilePathName, strFilePath);
			strcat( strFilePathName, strFilename );
			strcat( strFilePathName, ".nbl" );
		}
	}
	g_pDecoObjSet->ReadDataFile( strFilePathName, TRUE );

	// Initialize a object quadtree
	if( strFilename == NULL )		// DecoObjList.lst
	{
		strcpy( strFilePathName, strFilePath );
		strcat( strFilePathName, "DecoObjList.lst" );
		CNKObjQuadNode::LoadLst( strFilePathName, g_pMyNKApp->GetDecoTexture() );
	}
	else
	{
		if( g_pMyNKApp->IsLoadedGameMap() )		// Object file ( for game )
			CNKObjQuadNode::LoadObj( strFilePath, strFilename, g_pMyNKApp->GetDecoTexture() );
		else									// Nako object tree file ( for maptool )
		{
			strcpy( strFilePathName, strFilePath );
			strcat( strFilePathName, strFilename );
			strcat( strFilePathName, ".not");
			CNKObjQuadNode::LoadNot( strFilePathName, g_pMyNKApp->GetDecoTexture() );
		}
	}

	/*
	// 데코오브젝트 셋을 읽었으면 월드상에 위치한 정보를 읽는다.
	strcpy(strFilePathName, strFilePath);
	strcat(strFilePathName, "DecoObjList.lst");
	fp = fopen(strFilePathName, "r");
	if (fp == NULL) {
		fclose(fp);
		return;
	}

	// 목록의 갯수를 읽어온다.
	fscanf(fp, "%d", &listNum);

	for (int i=0; i < listNum; i++) {
		pDecoObj = new CDecoObj(g_ClrAlloc);
		// id, x, z, y, dir
		fscanf(fp, "%d %f %f %f %f", &(pDecoObj->m_nLoloId), &(pDecoObj->m_wx), &(pDecoObj->m_wz), \
			&(pDecoObj->m_wy), &(pDecoObj->m_dir));
		pDecoObj->m_pLolo = &(g_pDecoObjSet->m_pLoloSet[pDecoObj->m_nLoloId]);

		InsertDecoObjToMaplist(pDecoObj);
		pDecoObj->CalBoundBox();					
	}
	
	fclose(fp);
	*/

	delete [] strFilePathName;
}


void DeleteDecoObjData()
{
	CNKObjQuadNode::Destroy();

	int							w, h;
	CDecoObj*					pDecoObj;
	list<CDecoObj *>::iterator	pr;
	
	for( h = 0; h < 64; h++ )
	{
		for( w = 0; w < 64; w++)
		{
			for( pr = g_lstWalkObj[h][w].begin(); pr != g_lstWalkObj[h][w].end(); )
			{
				pDecoObj = *pr;
				delete pDecoObj;
				pr = g_lstWalkObj[h][w].erase(pr);
				if( pr == g_lstWalkObj[h][w].end() )
					break;
			}
		}
	}

	SAFE_DELETE( g_pDecoObjSet );
}

//////////////////////////////////////////////////////////////////////
// CDecoObj Construction/Destruction
//////////////////////////////////////////////////////////////////////
D3DMATRIX CDecoObj::m_matWorld; 
D3DMATRIX CDecoObj::m_matTrans;
D3DMATRIX CDecoObj::m_matRotate;
float	  CDecoObj::m_fAniRate = 0.1f;

CDecoObj::CDecoObj()
{
	m_pLolo = NULL;
	m_nCurFrame = 0;
	m_fTimeElapsed = 0.0f;

	m_FloorVect = NULL;
	m_InDoorVect = NULL;
	m_OutDoorVect = NULL;
	m_WalkPlaneVect = NULL;
	m_bOpenedRoof = FALSE;

	m_nFlyFrame = rand() % DECO_FLY_FRAME;
}

CDecoObj::~CDecoObj()
{
	if (m_FloorVect != NULL) {
		delete [] m_FloorVect;
		m_FloorVect = NULL;
	}

	if (m_InDoorVect != NULL) {
		delete [] m_InDoorVect;
		m_InDoorVect = NULL;
	}

	if (m_OutDoorVect != NULL) {
		delete [] m_OutDoorVect;
		m_OutDoorVect = NULL;
	}

	if( m_WalkPlaneVect != NULL )
	{
		delete [] m_WalkPlaneVect;
		m_WalkPlaneVect = NULL;
	}
}

void CDecoObj::CalBoundBox()
{
	D3DUtil_SetTranslateMatrix(m_matTrans, m_wx, m_wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix(m_matRotate, m_dir);
	D3DMath_MatrixMultiply( m_matWorld, m_matRotate, m_matTrans );

	int i = 0;
	for (i=0; i < 8; i++) {
		D3DMath_VectorMatrixMultiply(m_vBoundBox[i], m_pLolo->m_BoundVect[i] , m_matWorld);
	}

	if (m_pLolo->m_FloorCount > 0) {
		if( m_FloorVect == NULL )
			m_FloorVect = new D3DVECTOR[m_pLolo->m_FloorCount * 4];
		for (i=0; i < m_pLolo->m_FloorCount * 4; i++) {
			D3DMath_VectorMatrixMultiply(m_FloorVect[i], m_pLolo->m_FloorVect[i], m_matWorld);
		}
	}

	if (m_pLolo->m_InDoorCount > 0) {
		if( m_InDoorVect == NULL )
			m_InDoorVect = new D3DVECTOR[m_pLolo->m_InDoorCount * 4];
		for (i=0; i < m_pLolo->m_InDoorCount * 4; i++) {
			D3DMath_VectorMatrixMultiply(m_InDoorVect[i], m_pLolo->m_InDoorVect[i], m_matWorld);
		}
	}

	if (m_pLolo->m_OutDoorCount > 0) {
		if( m_OutDoorVect == NULL )
			m_OutDoorVect = new D3DVECTOR[m_pLolo->m_OutDoorCount * 4];
		for (i=0; i < m_pLolo->m_OutDoorCount * 4; i++) {
			D3DMath_VectorMatrixMultiply(m_OutDoorVect[i], m_pLolo->m_OutDoorVect[i], m_matWorld);
		}
	}

	if( m_pLolo->m_WalkPlaneCount > 0 )
	{
		if( m_WalkPlaneVect == NULL )
			m_WalkPlaneVect = new D3DVECTOR[m_pLolo->m_WalkPlaneCount * 4];
		for( i = 0; i < m_pLolo->m_WalkPlaneCount * 4; i++ )
		{
			D3DMath_VectorMatrixMultiply( m_WalkPlaneVect[i], m_pLolo->m_WalkPlaneVect[i], m_matWorld );
		}
	}
}

// ----------------------------------------------------------------
// Name : 
// Desc : 월드 좌표 x, z가 오브젝트 내에 포함되어 있는지 확인한다.
// ----------------------------------------------------------------
BOOL CDecoObj::IsInside(float wx, float wz)
{
	int bCCW[4];
	// (AB) 와 (AC) 벡터의 외적을 구하여 방향을 세 점의 위치 관계를 판다.
	bCCW[0] = Nk3DMath_CCW(m_vBoundBox[0].x, m_vBoundBox[0].z, m_vBoundBox[1].x, m_vBoundBox[1].z, wx, wz);			
	bCCW[1] = Nk3DMath_CCW(m_vBoundBox[1].x, m_vBoundBox[1].z, m_vBoundBox[2].x, m_vBoundBox[2].z,  wx,  wz);			
	bCCW[2] = Nk3DMath_CCW(m_vBoundBox[2].x, m_vBoundBox[2].z, m_vBoundBox[3].x, m_vBoundBox[3].z,  wx,  wz);				
	bCCW[3] = Nk3DMath_CCW(m_vBoundBox[3].x, m_vBoundBox[3].z, m_vBoundBox[0].x, m_vBoundBox[0].z,  wx,  wz);

	if ( abs(bCCW[0] = (bCCW[0] + bCCW[1] + bCCW[2] + bCCW[3])) == 4) 
		return TRUE;
	else 
		return FALSE;
}

HRESULT CDecoObj::Render(LPDIRECT3DDEVICE7 lpd3dDevice, BOOL bNoRoof, int whichAlpha)
{
	m_fTimeElapsed += g_pMyNKApp->GetElapsedTime();
	while (m_fTimeElapsed >= m_fAniRate) {
		m_fTimeElapsed -= m_fAniRate;
		m_nCurFrame++;
		m_nFlyFrame++;
	}
	m_nFlyFrame %= DECO_FLY_FRAME;

	//if (m_pLolo->m_AnimCount > 1) {
	//	while (m_nCurFrame >= m_pLolo->m_AnimCount ) {
	//		m_nCurFrame -= m_pLolo->m_AnimCount;
	//	}
	//} else {
	//	m_nCurFrame = 0;
	//}

	static DWORD alphaTest, alphaBlend;

/*
	DWORD						dwZWriteEnable;

	if ((whichAlpha == DECO_ALPHA_SEE || whichAlpha == DECO_ALPHA_FAR) &&
			g_pDecoObjSet->m_vecObjType[m_nLoloId] != BLD_STAR) 
	{
		lpd3dDevice->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWriteEnable );
		lpd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
	}
*/
	float wy = m_wy;
	if (g_pDecoObjSet->m_vecObjType[m_nLoloId] == BLD_FLY) {
//		if (pCMyApp->GetCurWorld() == WORLD_SKY) {
//			wy += g_DecoFlyHeight[m_nFlyFrame];
//		} else {
			wy += g_DecoFlyHeight[m_nFlyFrame] / 4.0f;
//		}
	}

	D3DUtil_SetTranslateMatrix( m_matTrans, m_wx, wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix( m_matRotate, m_dir);
	D3DMath_MatrixMultiply( m_matWorld, m_matRotate, m_matTrans );
	lpd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
	
	// View Matrix 도 가져와보자.
	D3DMATRIX matView;
	g_pMyNKApp->GetD3DDevice()->GetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

	// 상태값 보존.
	lpd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	lpd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );

	lpd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// 알파 테스팅

	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000030);
	//lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x0000001);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);

	if ((whichAlpha == DECO_ALPHA_SEE || whichAlpha == DECO_ALPHA_FAR) &&
			g_pDecoObjSet->m_vecObjType[m_nLoloId] != BLD_STAR) {
	//if (FALSE) {
		DWORD alpha;
		if (whichAlpha == DECO_ALPHA_FAR) {
			alpha = (DWORD)(0xff * m_farFactor) << 24;
		} else {
			alpha = 0x55000000;
		}
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);

		DWORD alphaArg1, alphaArg2, alphaRef, alphaOp;
		lpd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &alphaArg1);
		lpd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG2, &alphaArg2);
		lpd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP,  &alphaOp);
		lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE);
		lpd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000001);

		m_pLolo->Render(lpd3dDevice, m_nCurFrame, FALSE, FALSE, FALSE, &m_matWorld, &matView,
			FALSE, FALSE, alpha);

		lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, alphaArg1);
		lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, alphaArg2);
		lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, alphaOp);
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF  , alphaRef);
	} else {
		lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);

		if (g_pDecoObjSet->m_vecObjType[m_nLoloId] == BLD_EVENT_1 ||
				g_pDecoObjSet->m_vecObjType[m_nLoloId] == BLD_EVENT_2) {
			
			m_pLolo->Render(lpd3dDevice, m_nCurFrame, FALSE, bNoRoof, FALSE, &m_matWorld, &matView);

/*			if( pCMyApp->m_pMouse->GetMouseType() == M_DOOR ) {
				DWORD dwLighting, dwZEnable, dwZWrite, dwAlpha, dwSrcRenState, dwDestRenState;
				DWORD dwAmbient;

				lpd3dDevice->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);
//				lpd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00181864);
				lpd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00646464);

//				lpd3dDevice->GetRenderState( D3DRENDERSTATE_ZENABLE, &dwZEnable );
//				lpd3dDevice->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
//				lpd3dDevice->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
				lpd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlpha );

//				lpd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
//				lpd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
//				lpd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );	
				lpd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

				lpd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcRenState );
				lpd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestRenState );

				lpd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
				lpd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

				m_pLolo->Render(lpd3dDevice, m_nCurFrame, FALSE, bNoRoof, FALSE, &m_matWorld, &matView);
				
				lpd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcRenState );
				lpd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestRenState );

//				lpd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
//				lpd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, dwZEnable );
//				lpd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);				
				lpd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, dwAlpha );

				lpd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, dwAmbient);
			}*/
		} else {
			m_pLolo->Render(lpd3dDevice, m_nCurFrame, FALSE, bNoRoof, FALSE, &m_matWorld, &matView);
		}
	}


	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, alphaTest);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend);
/*
	if ((whichAlpha == DECO_ALPHA_SEE || whichAlpha == DECO_ALPHA_FAR) &&
			g_pDecoObjSet->m_vecObjType[m_nLoloId] != BLD_STAR) 
	{
		lpd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWriteEnable );
	}
*/
	return S_OK;
}

/*
HRESULT CDecoObj::Render(LPDIRECT3DDEVICE7 lpd3dDevice, float wx, float wy, float wz, LPDIRECT3DDEVICE7 lpd3dDevPick, BOOL bNoRoof)
{
	D3DUtil_SetTranslateMatrix( m_matWorld, wx, wy, wz);
	lpd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );

	//lpd3dDevice->LightEnable(1, TRUE);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pLolo->Render(lpd3dDevice, 0);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	//lpd3dDevice->LightEnable(1, FALSE);

	// 픽킹 버퍼 쓰기.
	if (lpd3dDevPick) {
		lpd3dDevPick->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
		if( SUCCEEDED( lpd3dDevPick->BeginScene() ) ) {
			
			lpd3dDevPick->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			lpd3dDevPick->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick );
			m_pLolo->Render(lpd3dDevPick, 0, TRUE);

			lpd3dDevPick->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		}
		// End the scene.
		lpd3dDevPick->EndScene();
	}
	return S_OK;
}
*/




//////////////////////////////////////////////////////////////////////
// CDecoObjSet Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDecoObjSet::CDecoObjSet(IndexedTexture *pTexture)
{
	assert(pTexture != NULL);
	m_pIndexedTexture = pTexture;
	m_pLolosDir = NULL;
	m_vecLoloSet.reserve( 500 );
	m_vecObjIdx.reserve( 500 );
	m_vecObjType.reserve( 500 );
	m_vecFileName.reserve( 500 );
	m_vecNickName.reserve( 500 );
	m_vecRefCount.reserve( 500 );
	m_nLoloCount = 0;
	
	//assert(pTexture != NULL);
	//m_pIndexedTexture = pTexture;
	//m_pLoloSet = NULL;
	//m_pnObjIdx = NULL;
	//m_pnObjType = NULL;
	//m_pLolosDir = NULL;
	//m_FileName = NULL;
	//m_NickName = NULL;
}

CDecoObjSet::~CDecoObjSet()
{
	int	i;
		
	SAFE_DELETE_ARRAY( m_pLolosDir );
	m_vecObjIdx.erase( m_vecObjIdx.begin(), m_vecObjIdx.end() );
	m_vecObjType.erase( m_vecObjType.begin(), m_vecObjType.end() );
	m_vecRefCount.erase( m_vecRefCount.begin(), m_vecRefCount.end() );

	for( i = 0; i < m_vecLoloSet.size(); i++ )
		SAFE_DELETE( m_vecLoloSet[i] );
	m_vecLoloSet.erase( m_vecLoloSet.begin(), m_vecLoloSet.end() );

	for( i = 0; i < m_vecFileName.size(); i++ )
		SAFE_DELETE_ARRAY( m_vecFileName[i] );
	m_vecFileName.erase( m_vecFileName.begin(), m_vecFileName.end() );

	for( i = 0; i < m_vecNickName.size(); i++ )
		SAFE_DELETE_ARRAY( m_vecNickName[i] );
	m_vecNickName.erase( m_vecNickName.begin(), m_vecNickName.end() );
}

void CDecoObjSet::SetLolosDir(char *dir_name)
{
	if (m_pLolosDir != NULL) {
		delete [] m_pLolosDir;
		m_pLolosDir = NULL;
	}

	m_pLolosDir = new char[strlen(dir_name)+1];
	strcpy(m_pLolosDir, dir_name);
}

void CDecoObjSet::AddDecoObj( int nIndex, int nType, char* szNickName, char* szFileName, CLolos* pLolos )
{
	m_vecObjIdx.push_back( nIndex );
	m_vecObjType.push_back( nType );
	m_vecNickName.push_back( szNickName );
	m_vecFileName.push_back( szFileName );
	m_vecLoloSet.push_back( pLolos );
	m_vecRefCount.push_back( 0 );
	m_nLoloCount++;
}

void CDecoObjSet::InsertDecoObj( int nIndex, int nType, char* szNickName, char* szFileName, CLolos* pLolos )
{
	int	i;
	vector<int>::iterator		iterIdx = m_vecObjIdx.begin();
	vector<int>::iterator		iterType = m_vecObjType.begin();
	vector<char*>::iterator		iterNick = m_vecNickName.begin();
	vector<char*>::iterator		iterFile = m_vecFileName.begin();
	vector<CLolos*>::iterator	iterLolos = m_vecLoloSet.begin();
	vector<int>::iterator		iterRef = m_vecRefCount.begin();

	for( i = 0; i < nIndex; i++ )
	{
		iterIdx++;
		iterType++;
		iterNick++;
		iterFile++;
		iterLolos++;
		iterRef++;
	}

	m_vecObjIdx.insert( iterIdx, nIndex );
	m_vecObjType.insert( iterType, nType );
	m_vecNickName.insert( iterNick, szNickName );
	m_vecFileName.insert( iterFile, szFileName );
	m_vecLoloSet.insert( iterLolos, pLolos );
	m_vecRefCount.insert( iterRef, 0 );

	for( i = nIndex; i < m_vecObjIdx.size(); i++ )
		m_vecObjIdx[i] = i;

	m_nLoloCount++;
}

void CDecoObjSet::RemoveDecoObj( int nIndex )
{
	int	i;
	vector<int>::iterator		iterIdx = m_vecObjIdx.begin();
	vector<int>::iterator		iterType = m_vecObjType.begin();
	vector<char*>::iterator		iterNick = m_vecNickName.begin();
	vector<char*>::iterator		iterFile = m_vecFileName.begin();
	vector<CLolos*>::iterator	iterLolos = m_vecLoloSet.begin();
	vector<int>::iterator		iterRef = m_vecRefCount.begin();

	for( i = 0; i < nIndex; i++ )
	{
		iterIdx++;
		iterType++;
		iterNick++;
		iterFile++;
		iterLolos++;
		iterRef++;
	}

	m_vecObjIdx.erase( iterIdx );
	m_vecObjType.erase( iterType );
	SAFE_DELETE_ARRAY( m_vecNickName[nIndex] );
	m_vecNickName.erase( iterNick );
	SAFE_DELETE_ARRAY( m_vecFileName[nIndex] );
	m_vecFileName.erase( iterFile );
	SAFE_DELETE( m_vecLoloSet[nIndex] );
	m_vecLoloSet.erase( iterLolos );
	m_vecRefCount.erase( iterRef );

	for( i = nIndex; i < m_vecObjIdx.size(); i++ )
		m_vecObjIdx[i] = i;

	m_nLoloCount--;
}

/*
int rec_count; // 총 갯수

///////////////// 갯수 만큼...
int index;
int type;
int name_len; // name 의 문자열 길이
char *name;
int lolo_len; // lolo 의 문자열 길이
char *lolo;
//////////////////
*/
void CDecoObjSet::ReadDataFile( char *file_name, BOOL bLoadWalkPlane )
{
	FILE*	fp = NULL;
	FILE*	fpLolo = NULL;
	char	path_name[256];
	int		i, len;

	fp = fopen( file_name, "rb" );
	
	if( fp == NULL )
		return ;
	
	//assert( fp != NULL );

	fread( &m_nLoloCount, sizeof(int), 1, fp );
	if( m_nLoloCount == 0 )
		return ;

	int		nObjIdx;
	int		nObjType;
	char*	szFileName;
	char*	szNickName;
	for( i = 0; i < m_nLoloCount; i++ )
	{
		m_vecRefCount.push_back( 0 );

		fread( &nObjIdx, sizeof(int), 1, fp );
		m_vecObjIdx.push_back( nObjIdx );

		fread( &nObjType, sizeof(int), 1, fp );
		m_vecObjType.push_back( nObjType );

		fread( &len, sizeof(int), 1, fp );
		szNickName = new char[len+1];
		fread( szNickName, len, 1, fp );
		szNickName[len] = NULL;
		m_vecNickName.push_back( szNickName );

		fread( &len, sizeof(int), 1, fp );
		szFileName = new char[len+1];
		fread( szFileName, len, 1, fp );
		szFileName[len] = NULL;
		m_vecFileName.push_back( szFileName );

		fpLolo = NULL;
		strcpy( path_name, m_pLolosDir );
		strcat( path_name, m_vecFileName[i] );
		fpLolo = fopen( path_name, "rb" );
		if( fpLolo == NULL )
		{
			MessageBox( NULL, m_vecFileName[i], m_vecFileName[i], MB_OK );
		}
		assert( fpLolo != NULL );
		CLolos*	pLolos = new CLolos( m_pIndexedTexture );
		m_vecLoloSet.push_back( pLolos );
		m_vecLoloSet[i]->Read( fpLolo, FALSE, bLoadWalkPlane );
		fclose( fpLolo );
	}



	/*
	FILE *fp = NULL;
	FILE *fpLolo = NULL;
	//char lolo_name[256];
	char path_name[256];
	//char buf[256];
	int i, len;
	//int lolo_count;

	fp = fopen(file_name, "rb");
	assert(fp != NULL);

	fread(&m_nLoloCount, sizeof(int), 1, fp);

	assert(m_nLoloCount > 0);

	// 레코드 수만큼 메모리를 할당받는다.
	//m_pLoloSet = new CLolos[m_nLoloCount];		// 로로
	//m_pnObjIdx = new int[m_nLoloCount];
	//m_pnObjType = new int[m_nLoloCount];
	//m_FileName = new char *[m_nLoloCount];
	//m_NickName = new char *[m_nLoloCount];

	for (i=0; i < m_nLoloCount; i++) {
		m_pLoloSet[i].SetIndexedTexture(m_pIndexedTexture);
		fread(&m_pnObjIdx[i], sizeof(int), 1, fp); 
		fread(&m_pnObjType[i], sizeof(int), 1, fp);
		fread(&len, sizeof(int), 1, fp);

		m_NickName[i] = new char[len+1];
		fread(m_NickName[i], len, 1, fp);
		m_NickName[i][len] = NULL;

		fread(&len, sizeof(int), 1, fp);
		m_FileName[i] = new char[len+1];
		fread(m_FileName[i], len, 1, fp);
		m_FileName[i][len] = NULL;

		// 실제 로로를 읽는 부분.
		fpLolo = NULL;
		strcpy(path_name, m_pLolosDir);
		strcat(path_name, m_FileName[i]);
		fpLolo = fopen(path_name, "rb");
		if (fpLolo == NULL) {
			MessageBox(NULL, m_FileName[i], m_FileName[i], MB_OK);
		}
		assert(fpLolo != NULL);
		m_pLoloSet[i].Read(fpLolo, TRUE);
		fclose(fpLolo);
	}
	*/
}

///////////////////////////////////////////////////////////////////////////
// Name : GetSentence()
// Desc : DecoObjSet.set 화일을 읽기 위한 것.
//////////////////////////////////////////////////////////////////////////
char * CDecoObjSet::GetSentence(char *dest, int n, FILE *fp)
{	
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do {
		if ( (tmp = fgets(dest, n, fp)) == NULL)
			return tmp;

		//while ((*tmp == 0x20 || (*tmp >= 0x09 && *tmp <= 0x0D)) && *tmp != 0)
		while (iswspace(*tmp) && *tmp != 0)
			tmp++;
	} while (*tmp == '#' || *tmp == 0);

	return tmp;
}


/*
HRESULT CDecoObjSet::Render(LPDIRECT3DDEVICE7 lpd3dDevice, float wx, float wy, float wz)
{

	D3DUtil_SetTranslateMatrix( m_matWorld, wx, wy, wz);
	lpd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );

	m_pLolos->Render(lpd3dDevice, 0);
	return S_OK;
}
*/


bool CDecoObjSet::SaveDataFile( const TCHAR* szFilename )
{
	FILE*	fp = NULL;
	fp = fopen( szFilename, "wb" );
	
	if( fp == NULL )
		return false;
	
	fwrite( &m_nLoloCount, sizeof(int), 1, fp );

	int	nLength;
	for( int i = 0; i < m_nLoloCount; i++ )
	{
		fwrite( &m_vecObjIdx[i], sizeof(int), 1, fp );
		fwrite( &m_vecObjType[i], sizeof(int), 1, fp );

		nLength = _tcslen( m_vecNickName[i] );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( m_vecNickName[i], nLength, 1, fp );

		nLength = _tcslen( m_vecFileName[i] );
		fwrite( &nLength, sizeof(int), 1, fp );
		fwrite( m_vecFileName[i], nLength, 1, fp );
	}

	fclose( fp );

	return true;
}