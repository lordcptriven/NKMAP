//-----------------------------------------------------------------------------
// File: Triangle.cpp
//
// Desc: Simple tutorial code to show the rendering of one triangle.
//
// Copyright (c) 1998-1999 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#define D3D_OVERLOADS
#include "StdAfx.h"
#include <windows.h>
#include <math.h>
#include <d3d.h>
#include "d3dxmath.h"

#include "CapSyong.h"
#include "IndexedTexture.h"
//#include "VirtualFile.h"

#include "d3dmath.h"
#include "d3dutil.h"
#include <tchar.h>

BOOL bShowLow = FALSE;

D3DMATRIX matEddy[EDDY_FRAME];
BOOL bCalced_matEddy = FALSE;
void Calc_matEddy();
D3DVERTEX loloVert[MAX_LOLO_VERT];
extern DWORD	g_dwObjTriangles;


CAniMatrix::CAniMatrix()
{
	//m_FaceCount = 0;
	//m_Vertices = NULL;
	//m_Vertices0 = NULL;
	m_Mats = NULL;
}

CAniMatrix::~CAniMatrix()
{
	/*if (m_Vertices)
		delete[] m_Vertices;
	if (m_Vertices0)
		delete[] m_Vertices0;*/
	if (m_Mats)
		delete[] m_Mats;
}

CBiped::CBiped()
{
	m_AniMat = NULL;
}

CBiped::~CBiped()
{
	if (m_AniMat)
		delete[] m_AniMat;
}

CCsyObj::CCsyObj()
{
	m_TextureNum = -1;
	m_CsyEffect = CSY_EFF_NONE;
	m_VertexCount = 0;
	m_FaceCount = 0;
	m_FaceCountLow = 0;
	m_Vertices = NULL;
	m_Vertices0 = NULL;
	m_BipedIdx = NULL;
	m_Indexed = NULL;
	m_IndexedLow = NULL;
}

CCsyObj::~CCsyObj()
{
	if (m_Vertices0)
		delete[] m_Vertices0;
	if (m_Vertices)
		delete[] m_Vertices;
	if (m_BipedIdx)
		delete[] m_BipedIdx;
	if (m_Indexed)
		delete[] m_Indexed;
	if (m_IndexedLow)
		delete[] m_IndexedLow;
}

CCapsyong::CCapsyong(IndexedTexture *pTexture)
{
	m_Csy = NULL;
	m_pTexture = pTexture;
	m_LineVector0 = NULL;
	m_LineVector = NULL;
	m_LineTextureNum = -1;
	m_bLightEffect = FALSE;

	m_BoundNum = 0;
}


CCapsyong::~CCapsyong()
{
	if (m_Csy)
		delete[] m_Csy;
	if (m_LineVector)
		delete[] m_LineVector;
	if (m_LineVector0)
		delete[] m_LineVector0;
	if (m_BoundNum > 0) {
		delete[] m_BoundVert;
		delete[] m_BoundIndex;
	}
}

CLoloObj::CLoloObj()
{
	m_bAlphablend = FALSE;

	m_AnimCount = 0;
	m_TextureNum = -1;
	m_FaceCount = 0;
	m_Vertices = NULL;
	m_Indexed = NULL;
	m_CenterPoint = NULL;

	m_FaceCountLow = 0;
	m_IndexedLow = NULL;
}

CLoloObj::~CLoloObj()
{
	if (m_Vertices) {
		for (int idx = 0; idx < m_AnimCount; idx++) {
			delete[] m_Vertices[idx];
		}
		delete[] m_Vertices;
	}
	if (m_CenterPoint)
		delete[] m_CenterPoint;
	if (m_Indexed)
		delete[] m_Indexed;

	if (m_IndexedLow)
		delete[] m_IndexedLow;
}

CLolos::CLolos()
{
	m_pTexture = NULL;

	m_LoloObjCount = 0;
	m_LoloObjs = NULL;
	m_LoloRoofCount = 0;
	m_LoloRoofs = NULL;
	m_LineVector0 = NULL;
	m_LineType = LINE_TYPE_BRIGHT;
	m_bMustRenderLast = FALSE;

	m_FloorVect = NULL;
	m_InDoorVect = NULL;
	m_OutDoorVect = NULL;
	m_WalkPlaneVect = NULL;
	m_WalkPlaneCount = 0;
}
 
CLolos::CLolos(IndexedTexture *pTexture)
{
	m_pTexture = pTexture;

	m_LoloObjCount = 0;
	m_LoloObjs = NULL;
	m_LoloRoofCount = 0;
	m_LoloRoofs = NULL;
	m_LineVector0 = NULL;
	m_LineType = LINE_TYPE_BRIGHT;
	m_bMustRenderLast = FALSE;

	m_FloorVect = NULL;
	m_InDoorVect = NULL;
	m_OutDoorVect = NULL;
	m_WalkPlaneVect = NULL;
	m_WalkPlaneCount = 0;
}

CLolos::~CLolos()
{
	if (m_LoloObjs)
		delete[] m_LoloObjs;
	if (m_LoloRoofs)
		delete[] m_LoloRoofs;
	/*if (m_CenterPoint)
		delete[] m_CenterPoint;*/
	if (m_LineVector0)
		delete[] m_LineVector0;
	if (m_FloorVect)
		delete[] m_FloorVect;
	if (m_InDoorVect)
		delete[] m_InDoorVect;
	if (m_OutDoorVect)
		delete[] m_OutDoorVect;
	if( m_WalkPlaneVect )
		delete [] m_WalkPlaneVect;
}

void CLolos::SetIndexedTexture(IndexedTexture *pTexture)
{
	m_pTexture = pTexture;
}

void GetHalfMat(D3DMATRIX &mat, CBiped *pBiped, int bpd_idx, int CurFrame)
{
	mat._11 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._11 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._11) / 2;
	mat._12 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._12 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._12) / 2;
	mat._13 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._13 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._13) / 2;
	mat._14 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._14 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._14) / 2;
	mat._21 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._21 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._21) / 2;
	mat._22 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._22 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._22) / 2;
	mat._23 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._23 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._23) / 2;
	mat._24 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._24 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._24) / 2;
	mat._31 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._31 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._31) / 2;
	mat._32 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._32 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._32) / 2;
	mat._33 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._33 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._33) / 2;
	mat._34 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._34 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._34) / 2;
	mat._41 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._41 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._41) / 2;
	mat._42 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._42 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._42) / 2;
	mat._43 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._43 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._43) / 2;
	mat._44 = 1.0f;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._44 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._44) / 2;
}


#define EDDY_HEIGHT	15.0f
#define EDDY_SCALE		(-0.5f)

#define LIGHTSWORD_SCALE	(0.5f)
#define LIGHTSWORD_Z		(3.0f)

#define LASERGUN_SCALE		(0.1f)

//#define BPD_ASSERT(pBpd, idx)	(idx >= pBpd->
D3DMATRIX g_TempMat[50];
int CCapsyong::Animate(CBiped *pBiped, int CurFrame, BOOL bTrace, int whatEff, D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat,
					   D3DVECTOR *pTargetVector, int csyEff)
{
	//CAniMatrix *pAniMat;
	D3DMATRIX *pLineMat, *pLineMat2;
	//D3DMATRIX LineMat, LineMat2;
	D3DVECTOR lineVector, rectVector;
	//BOOL bHalf = (pBiped->m_bHalfFrame && CurFrame % 2);
	//D3DMATRIX *pTempMat = NULL;
	//BOOL bMadeBiped = FALSE;
	int oldCurFrame = CurFrame;
	int bpd_idx;

	// Biped�� �̿��� Animation, Biped�� ���� ���� ȸ���� Į�̶�� ���ǿɴϴ�....
	if (pBiped) {
		CurFrame %= pBiped->m_AnimCount;
		// �߰� ������ ����µ�, Frame�� Ȧ���̰� ������ ���ڸ� �ƴϸ� �߰� ������ ����ؼ� ����.
		if (pBiped->m_bHalfFrame && CurFrame % 2) {
			CurFrame /= 2;
			//pTempMat = new D3DMATRIX[pBiped->m_BipedNum];
			//bMadeBiped = TRUE;

			// �߰� �������� ���� Matrix�� �����.
			for (int bpd_idx = 0; bpd_idx < pBiped->m_BipedNum; bpd_idx++)
			{
				GetHalfMat(g_TempMat[bpd_idx], pBiped, bpd_idx, CurFrame);
			}
			// Vertex���� Animation ��Ų��.
			for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++)
			{
				for (int i = 0; i < m_Csy[csy_idx].m_VertexCount; i++)
				{
					int bpd_idx = m_Csy[csy_idx].m_BipedIdx[i];
					if (bpd_idx >= pBiped->m_BipedNum)
						bpd_idx = pBiped->m_BipedNum - 1;
					NKMath_LagVertexMatrixMultiply(m_Csy[csy_idx].m_Vertices[i],
								m_Csy[csy_idx].m_Vertices0[i],
								g_TempMat[bpd_idx]);
				}
			}
			// Line ���...
			if (m_LineVector0) {
				NKMath_VectorMatrixMultiply(m_LineVector[0],
							m_LineVector0[0], g_TempMat[m_LineBiped[0]]);
				NKMath_VectorMatrixMultiply(m_LineVector[1],
							m_LineVector0[1], g_TempMat[m_LineBiped[1]]);
			}
		} else {
			if (pBiped->m_bHalfFrame)
				CurFrame /= 2;

			for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++) {
				//CCsyObj *pCsy = &(m_Csy[csy_idx]);

				// Vertex���� Animation ��Ų��.
				for (int i = 0; i < m_Csy[csy_idx].m_VertexCount; i++) {
					//pAniMat = &(pBiped->m_AniMat[m_Csy[csy_idx].m_BipedIdx[i]]);
					bpd_idx = m_Csy[csy_idx].m_BipedIdx[i];
					if (bpd_idx >= pBiped->m_BipedNum)
						bpd_idx = pBiped->m_BipedNum - 1;

					NKMath_LagVertexMatrixMultiply(m_Csy[csy_idx].m_Vertices[i],
								m_Csy[csy_idx].m_Vertices0[i],
								pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]);
				}
			}
			// Line ���...
			if (m_LineVector0) {
				NKMath_VectorMatrixMultiply(m_LineVector[0],
							m_LineVector0[0], pBiped->m_AniMat[m_LineBiped[0]].m_Mats[CurFrame]);
				NKMath_VectorMatrixMultiply(m_LineVector[1],
							m_LineVector0[1], pBiped->m_AniMat[m_LineBiped[1]].m_Mats[CurFrame]);
			}
		}

		if (bTrace && m_LineVector0) {
			// �� Trace Line ���
			oldCurFrame--;
			if (oldCurFrame < 0) {
				//oldCurFrame = 0;
				m_LineVector[2] = m_LineVector[0];
				m_LineVector[3] = m_LineVector[1];
			} else {
				if (pBiped->m_bHalfFrame) {
					if (oldCurFrame % 2) {
						GetHalfMat(g_TempMat[0], pBiped, m_LineBiped[0], oldCurFrame / 2);
						GetHalfMat(g_TempMat[1], pBiped, m_LineBiped[1], oldCurFrame / 2);
						pLineMat = &g_TempMat[0];
						pLineMat2 = &g_TempMat[1];
					} else {
						pLineMat = &(pBiped->m_AniMat[m_LineBiped[0]].m_Mats[oldCurFrame / 2]);
						pLineMat2 = &(pBiped->m_AniMat[m_LineBiped[1]].m_Mats[oldCurFrame / 2]);
					}
				} else {
					pLineMat = &(pBiped->m_AniMat[m_LineBiped[0]].m_Mats[oldCurFrame]);
					pLineMat2 = &(pBiped->m_AniMat[m_LineBiped[1]].m_Mats[oldCurFrame]);
				}
				//pAniMat = &(pBiped->m_AniMat[m_LineBiped[0]]);
				NKMath_VectorMatrixMultiply(m_LineVector[2],
							m_LineVector0[0], *pLineMat/*->m_Mats[CurFrame]*/);
				//pAniMat = &(pBiped->m_AniMat[m_LineBiped[1]]);
				NKMath_VectorMatrixMultiply(m_LineVector[3],
							m_LineVector0[1], *pLineMat2/*->m_Mats[CurFrame]*/);
			}

			oldCurFrame--;
			if (oldCurFrame < 0) {
				//oldCurFrame = 0;
				m_LineVector[4] = m_LineVector[2];
				m_LineVector[5] = m_LineVector[3];
			} else {
				if (pBiped->m_bHalfFrame) {
					if (oldCurFrame % 2) {
						GetHalfMat(g_TempMat[0], pBiped, m_LineBiped[0], oldCurFrame / 2);
						GetHalfMat(g_TempMat[1], pBiped, m_LineBiped[1], oldCurFrame / 2);
						pLineMat = &g_TempMat[0];
						pLineMat2 = &g_TempMat[1];
					} else {
						pLineMat = &(pBiped->m_AniMat[m_LineBiped[0]].m_Mats[oldCurFrame / 2]);
						pLineMat2 = &(pBiped->m_AniMat[m_LineBiped[1]].m_Mats[oldCurFrame / 2]);
					}
				} else {
					pLineMat = &(pBiped->m_AniMat[m_LineBiped[0]].m_Mats[oldCurFrame]);
					pLineMat2 = &(pBiped->m_AniMat[m_LineBiped[1]].m_Mats[oldCurFrame]);
				}
				//pAniMat = &(pBiped->m_AniMat[m_LineBiped[0]]);
				NKMath_VectorMatrixMultiply(m_LineVector[4],
							m_LineVector0[0], *pLineMat/*->m_Mats[CurFrame]*/);
				//pAniMat = &(pBiped->m_AniMat[m_LineBiped[1]]);
				NKMath_VectorMatrixMultiply(m_LineVector[5],
							m_LineVector0[1], *pLineMat2/*->m_Mats[CurFrame]*/);
			}

		}
	} else {
		// ȸ���� ���� Animation
		CurFrame %= EDDY_FRAME;
		if (!bCalced_matEddy)
			Calc_matEddy();

		for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++) {
			CCsyObj *pCsy = &(m_Csy[csy_idx]);

			// ���⸦ ȸ���� Matrix�� Animation ��Ų��.
			for (int i = 0; i < pCsy->m_VertexCount; i++) {
				NKMath_LagVertexMatrixMultiply(pCsy->m_Vertices[i],
							pCsy->m_Vertices0[i], matEddy[CurFrame]);
			}
		}

		if (m_LineVector0) {
			// �� Trace Line ���
			NKMath_VectorMatrixMultiply(m_LineVector[0],
						m_LineVector0[0], matEddy[CurFrame]);
			NKMath_VectorMatrixMultiply(m_LineVector[1],
						m_LineVector0[1], matEddy[CurFrame]);

			CurFrame--;
			if (CurFrame < 0)
				CurFrame = 0;
			NKMath_VectorMatrixMultiply(m_LineVector[2],
						m_LineVector0[0], matEddy[CurFrame]);
			NKMath_VectorMatrixMultiply(m_LineVector[3],
						m_LineVector0[1], matEddy[CurFrame]);

			CurFrame--;
			if (CurFrame < 0)
				CurFrame = 0;
			NKMath_VectorMatrixMultiply(m_LineVector[4],
						m_LineVector0[0], matEddy[CurFrame]);
			NKMath_VectorMatrixMultiply(m_LineVector[5],
						m_LineVector0[1], matEddy[CurFrame]);
		}
	}

	// ������ �ʹ� ������ �� �Ƚ�!!
	if (pTargetVector && pTargetVector->x < 30) {
		pViewMat = NULL;
	}

	m_bLightEffect = FALSE;
	if (m_LineVector && pWorldMat && pViewMat) {
		D3DMATRIX matView, matInv;
		if (pTargetVector) {
		// ������ ���� ���� Rect ���
			// View Matrix�� ���� �� View�� Inv�� ���Ѵ�.
			D3DMATRIX matView, matInv;
			D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
			D3DMath_MatrixInvert(matInv, matView);

			m_LineVector[1].x = pTargetVector->x;
			m_LineVector[1].y = pTargetVector->y;
			m_LineVector[1].z = pTargetVector->z;

			// LineVector�� View ��ǥ��..
			D3DVECTOR lineVector[2];
			D3DMath_VectorMatrixMultiply(lineVector[0], m_LineVector[0], matView);
			D3DMath_VectorMatrixMultiply(lineVector[1], m_LineVector[1], matView);

			// line�� ������ ���͸� ���Ѵ�.
			D3DVECTOR lineInv;
			lineInv = lineVector[0];
			lineInv -= lineVector[1];

			FLOAT x_temp = lineInv.x;
			lineInv.x = lineInv.y;
			lineInv.y = x_temp;
			lineInv.z = 0;
			lineInv = Normalize(lineInv);
			lineInv *= LASERGUN_SCALE;

			// View ��ǥ�� �簢��
			m_RectVector[0] = D3DVECTOR(lineVector[0].x + lineInv.x,
									lineVector[0].y - lineInv.y,
									lineVector[0].z - LIGHTSWORD_Z);
			m_RectVector[1] = D3DVECTOR(lineVector[1].x + lineInv.x,
									lineVector[1].y - lineInv.y,
									lineVector[1].z - LIGHTSWORD_Z);
			m_RectVector[2] = D3DVECTOR(lineVector[0].x - lineInv.x,
									lineVector[0].y + lineInv.y,
									lineVector[0].z - LIGHTSWORD_Z);
			m_RectVector[3] = D3DVECTOR(lineVector[1].x - lineInv.x,
									lineVector[1].y + lineInv.y,
									lineVector[1].z - LIGHTSWORD_Z);

			// �簢���� World ��ǥ��...
			D3DMath_VectorMatrixMultiply(m_RectVector[0], m_RectVector[0], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[1], m_RectVector[1], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[2], m_RectVector[2], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[3], m_RectVector[3], matInv);
		} else if (whatEff == LINE_EFF_LIGHTSWORD) {
		// �������̳� �Ұ˵��� ���� Rect ���

			// View Matrix�� ���� �� View�� Inv�� ���Ѵ�.
			D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
			D3DMath_MatrixInvert(matInv, matView);

			// LineVector�� View ��ǥ��..
			D3DVECTOR lineVector[2];
			D3DMath_VectorMatrixMultiply(lineVector[0], m_LineVector[0], matView);
			D3DMath_VectorMatrixMultiply(lineVector[1], m_LineVector[1], matView);

			// line�� ������ ���͸� ���Ѵ�.
			D3DVECTOR lineInv;
			lineInv = lineVector[0];
			lineInv -= lineVector[1];

			FLOAT x_temp = lineInv.x;
			lineInv.x = lineInv.y;
			lineInv.y = x_temp;
			lineInv.z = 0;
			lineInv = Normalize(lineInv);
			lineInv *= LIGHTSWORD_SCALE;

			// View ��ǥ�� �簢��
			m_RectVector[0] = D3DVECTOR(lineVector[0].x + lineInv.x,
									lineVector[0].y - lineInv.y,
									lineVector[0].z - LIGHTSWORD_Z);
			m_RectVector[1] = D3DVECTOR(lineVector[1].x + lineInv.x,
									lineVector[1].y - lineInv.y,
									lineVector[1].z - LIGHTSWORD_Z);
			m_RectVector[2] = D3DVECTOR(lineVector[0].x - lineInv.x,
									lineVector[0].y + lineInv.y,
									lineVector[0].z - LIGHTSWORD_Z);
			m_RectVector[3] = D3DVECTOR(lineVector[1].x - lineInv.x,
									lineVector[1].y + lineInv.y,
									lineVector[1].z - LIGHTSWORD_Z);

			// �簢���� World ��ǥ��...
			D3DMath_VectorMatrixMultiply(m_RectVector[0], m_RectVector[0], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[1], m_RectVector[1], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[2], m_RectVector[2], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[3], m_RectVector[3], matInv);
		} else if (whatEff == LINE_EFF_BRIGHT) {
			// View Matrix�� ���� �� View�� Inv�� ���Ѵ�.
			D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
			D3DMath_MatrixInvert(matInv, matView);

			// LineVector�� View ��ǥ��..
			//D3DVECTOR lineVector[2];
			//D3DMath_VectorMatrixMultiply(lineVector[0], m_LineVector[0], matView);
			//D3DMath_VectorMatrixMultiply(lineVector[1], m_LineVector[1], matView);

			D3DVECTOR lineCenter;

			// Line�� ������ ������ ���Ѵ�. (�ٻ簪)
			float lineLen = MAX(PLUS(m_LineVector[0].x - m_LineVector[1].x), 
							MAX(PLUS(m_LineVector[0].y - m_LineVector[1].y),
								PLUS(m_LineVector[0].z - m_LineVector[1].z))) / 2;

			// ������ �߰����� ���Ѵ�.
			lineCenter.x = (m_LineVector[0].x + m_LineVector[1].x) / 2;
			lineCenter.y = (m_LineVector[0].y + m_LineVector[1].y) / 2;
			lineCenter.z = (m_LineVector[0].z + m_LineVector[1].z) / 2;
			D3DMath_VectorMatrixMultiply(lineCenter, lineCenter, matView);

			// View ��ǥ�� �簢��
			m_RectVector[0] = D3DVECTOR(lineCenter.x - lineLen,
											lineCenter.y - lineLen,
											lineCenter.z - LIGHTSWORD_Z);
			m_RectVector[1] = D3DVECTOR(lineCenter.x - lineLen,
											lineCenter.y + lineLen,
											lineCenter.z - LIGHTSWORD_Z);
			m_RectVector[2] = D3DVECTOR(lineCenter.x + lineLen,
											lineCenter.y - lineLen,
											lineCenter.z - LIGHTSWORD_Z);
			m_RectVector[3] = D3DVECTOR(lineCenter.x + lineLen,
											lineCenter.y + lineLen,
											lineCenter.z - LIGHTSWORD_Z);

			// �簢���� World ��ǥ��...
			D3DMath_VectorMatrixMultiply(m_RectVector[0], m_RectVector[0], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[1], m_RectVector[1], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[2], m_RectVector[2], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[3], m_RectVector[3], matInv);
		}
		m_bLightEffect = TRUE;
	} else {
	}

	//if (pTempMat)
	//	delete[] pTempMat;

	return S_OK;
}

void Calc_matEddy()
{
	for (int eddy_idx = 0; eddy_idx < EDDY_FRAME; eddy_idx++) {
		D3DMATRIX mat_pos, mat_rot;
		// Frame�� �°� matrix�� �����.
		D3DUtil_SetRotateZMatrix(mat_pos, (g_2_PI / 12) * eddy_idx);
		mat_pos._43 = (float) eddy_idx * EDDY_SCALE;
		mat_pos._42 = EDDY_HEIGHT;
		D3DUtil_SetRotateYMatrix(mat_rot, (g_2_PI / 12) * eddy_idx);
		D3DMath_MatrixMultiply(matEddy[eddy_idx], mat_pos, mat_rot);
	}

	bCalced_matEddy = TRUE;
}

unsigned short g_TraceIndex[24] = {0, 2, 1, 1, 2, 3,
									4, 6, 5, 5, 6, 7,
									0, 1, 2, 1, 3, 2,
									4, 5, 6, 5, 7, 6};
//#define EFF_NUM		10
void CCapsyong::Render( LPDIRECT3DDEVICE7 pd3dDevice, int textureNum, BOOL bPicking, BOOL bShowLow, LPDIRECTDRAWSURFACE7 traceTexture,
							/*LPDIRECTDRAWSURFACE7 lightswordTexture,*/ int effFrame)
{
	CCsyObj *pCsy;
	LPDIRECTDRAWSURFACE7 lpTexture;
	DWORD srcBlend, destBlend, srcBlend2, destBlend2;

	pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlend2);
	pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlend2 );
	// Capsyong Rendering �ϴ� �κ�
	for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++) {
		pCsy = &(m_Csy[csy_idx]);
		int tNum;
		if (textureNum >= 0)
			tNum = textureNum;
		else
			tNum = pCsy->m_TextureNum;
		// Texture�� ������ Texture ���
		if (tNum >= 0 && !bPicking) {
			if ((lpTexture = m_pTexture->GetSurfaceByIdx(tNum)) != NULL) {
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK) {
					pd3dDevice->SetTexture(0, NULL);
				}
			} else {
				pd3dDevice->SetTexture(0, NULL);
			}
		} else {
			pd3dDevice->SetTexture(0, NULL);
		}
		float uv_add = (1.0f / 40) * (effFrame % 40);
		if (textureNum >= 0 && effFrame >= 0) {
			for (int i = 0; i < pCsy->m_VertexCount; i++) {
				pCsy->m_Vertices[i].tu += uv_add;
				pCsy->m_Vertices[i].tv += uv_add;
			}
		}

		DWORD blendEnable;
		if (pCsy->m_CsyEffect == CSY_EFF_ONEONE) {
			pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &blendEnable);
			pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlend);
			pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlend );

			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
		}

		// �׷���!
		if (bShowLow && pCsy->m_IndexedLow) {
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
						pCsy->m_Vertices, pCsy->m_VertexCount,
						pCsy->m_IndexedLow, pCsy->m_FaceCountLow * 3, NULL );
		} else {
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
						pCsy->m_Vertices, pCsy->m_VertexCount,
						pCsy->m_Indexed, pCsy->m_FaceCount * 3, NULL );
		}
		if (pCsy->m_CsyEffect == CSY_EFF_ONEONE) {
			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, blendEnable);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlend);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlend );
		}

		if (textureNum >= 0 && effFrame >= 0) {
			for (int i = 0; i < pCsy->m_VertexCount; i++) {
				pCsy->m_Vertices[i].tu = pCsy->m_Vertices0[i].tu;
				pCsy->m_Vertices[i].tv = pCsy->m_Vertices0[i].tv;
			}
		}
	}

	if (m_LineVector && !bPicking) {
		DWORD alphaBlend, alphaTest, alphaRef;
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF   , 0x0000001);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		// ���� ���븦 ǥ���ϴ� �κ�...
		if (traceTexture) {
			D3DLVERTEX traceVertex[8];
			//D3DVERTEX lineVertex[6];

			traceVertex[0] = D3DLVERTEX(m_LineVector[0], 0x55ffffff, 0L, 0, 1);
			traceVertex[1] = D3DLVERTEX(m_LineVector[1], 0x55ffffff, 0L, 0, 0);
			traceVertex[2] = D3DLVERTEX(m_LineVector[2], 0x55ffffff, 0L, 1, 1);
			traceVertex[3] = D3DLVERTEX(m_LineVector[3], 0x55ffffff, 0L, 1, 0);
			traceVertex[4] = D3DLVERTEX(m_LineVector[2], 0x55ffffff, 0L, 0, 1);
			traceVertex[5] = D3DLVERTEX(m_LineVector[3], 0x55ffffff, 0L, 0, 0);
			traceVertex[6] = D3DLVERTEX(m_LineVector[4], 0x55ffffff, 0L, 1, 1);
			traceVertex[7] = D3DLVERTEX(m_LineVector[5], 0x55ffffff, 0L, 1, 0);
			/*lineVertex[0] = D3DVERTEX(m_LineVector0[0], D3DVECTOR(0, 0, 1), 0, 0);
			lineVertex[1] = D3DVERTEX(m_LineVector0[1], D3DVECTOR(0, 0, 1), 0, 1);
			lineVertex[2] = D3DVERTEX(m_LineVector1[0], D3DVECTOR(0, 0, 1), 1, 0);
			lineVertex[3] = D3DVERTEX(m_LineVector1[1], D3DVECTOR(0, 0, 1), 1, 1);
			lineVertex[4] = D3DVERTEX(m_LineVector2[0], D3DVECTOR(0, 0, 1), 0, 0);
			lineVertex[5] = D3DVERTEX(m_LineVector2[1], D3DVECTOR(0, 0, 1), 0, 1);*/

			pd3dDevice->SetTexture(0, traceTexture);
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
						traceVertex, 8, g_TraceIndex, 24, NULL );
			//pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX ,
			//			lineVertex, 6, lineIndex, 24, NULL );
		}

		// �������̳�, �Ұ�, �Ǵ� ��¦�̸� ǥ���ϴ� �κ�...
		if (m_bLightEffect) {
			pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
			pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlend);
			pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlend );
			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

			D3DLVERTEX rectVertex[4];

			// �� 16Frame ���� 8Frame�� �����ش�.
			int lightswordFrame = effFrame % 16 / 2;
			// 5->3, 6->2, 7->1
			if (lightswordFrame > 4)
				lightswordFrame = 8 - lightswordFrame;

			float textureU = lightswordFrame / 5.0f;

			// ������ ȿ�� ?			rectVertex[0] = D3DLVERTEX(m_RectVector[0], 0x00ffffff, 0L, textureU, 1);
			rectVertex[1] = D3DLVERTEX(m_RectVector[1], 0x00ffffff, 0L, textureU, 0);
			rectVertex[2] = D3DLVERTEX(m_RectVector[2], 0x00ffffff, 0L, textureU + 0.2f, 1);
			rectVertex[3] = D3DLVERTEX(m_RectVector[3], 0x00ffffff, 0L, textureU + 0.2f, 0);
			//rectVertex[0] = D3DLVERTEX(m_RectVector[0], 0x00ffffff, 0L, 0, 1);
			//rectVertex[1] = D3DLVERTEX(m_RectVector[1], 0x00ffffff, 0L, 0, 0);
			//rectVertex[2] = D3DLVERTEX(m_RectVector[2], 0x00ffffff, 0L, 1, 1);
			//rectVertex[3] = D3DLVERTEX(m_RectVector[3], 0x00ffffff, 0L, 1, 0);

			if ((lpTexture = m_pTexture->GetSurfaceByIdx(m_LineTextureNum)) != NULL) {
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK) {
					pd3dDevice->SetTexture(0, NULL);
				}
			} else {
				pd3dDevice->SetTexture(0, NULL);
			}
			//pd3dDevice->SetTexture(0, lightswordTexture);
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX ,
						rectVertex, 4, NULL );

			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlend);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlend );
			pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
		}

		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF   , alphaRef);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	}
	pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlend2);
	pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlend2 );
	pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
}

void CCapsyong::RenderBound( LPDIRECT3DDEVICE7 pd3dDevice )
{
	if (m_BoundNum <= 0)
		return;
	pd3dDevice->SetTexture(0, NULL);

	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX ,
				m_BoundVert, m_BoundVertNum, m_BoundIndex, m_BoundFaceNum*3, NULL );
}
int loloZcompare( const void *arg1, const void *arg2 )
{
	if ((*(CLoloObj **)arg1)->m_CenterWorld.z < (*(CLoloObj **)arg2)->m_CenterWorld.z)
		return 1;
	else
		return -1;
}

void CLolos::Render( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, BOOL bPicking, BOOL bNoRoof, BOOL bShowLow,
						D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, BOOL bSort, BOOL bNoLight, DWORD dwAlpha )
{
	int lolo_idx;
	D3DMATRIX matView;

	int oldFrame = CurFrame;

	CurFrame %= m_AnimCount;

	if (pWorldMat && pViewMat) {
		D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
		//matView = *pViewMat;
	}

	if (bSort && pWorldMat && pViewMat) {
		int lolo_count;
		if (bNoRoof) {
			lolo_count = m_LoloObjCount;
		} else {
			lolo_count = m_LoloObjCount + m_LoloRoofCount;
		}
		CLoloObj **pLoloList = new CLoloObj*[lolo_count];

		// �߽� ��ǥ ȸ��!
		for (lolo_idx = 0; lolo_idx < lolo_count; lolo_idx++) {
			int nFrame = CurFrame;
			CLoloObj *pLoloObj;
			if (lolo_idx < m_LoloObjCount)
				pLoloObj = &m_LoloObjs[lolo_idx];
			else
				pLoloObj = &m_LoloRoofs[lolo_idx];

			if (pLoloObj->m_AnimCount == 1)
				nFrame = 0;

			D3DMath_VectorMatrixMultiply(pLoloObj->m_CenterWorld,
										pLoloObj->m_CenterPoint[nFrame], matView);
			pLoloList[lolo_idx] = pLoloObj;
		}
		// �߽� ��ǥ�� ����!
		qsort((void *) pLoloList, (size_t) lolo_count, sizeof(CLoloObj *), loloZcompare);

		for (lolo_idx = 0; lolo_idx < lolo_count; lolo_idx++) {
			pLoloList[lolo_idx]->Render(pd3dDevice, CurFrame,
					m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
		}

		delete[] pLoloList;

	} else {
		for (lolo_idx = 0; lolo_idx < m_LoloObjCount; lolo_idx++) {
			int nFrame = CurFrame;
			if (m_LoloObjs[lolo_idx].m_AnimCount == 1)
				nFrame = 0;

			m_LoloObjs[lolo_idx].Render(pd3dDevice, nFrame,
					m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
		}
		if (!bNoRoof) {
			for (lolo_idx = 0; lolo_idx < m_LoloRoofCount; lolo_idx++) {
				int nFrame = CurFrame;
				if (m_LoloRoofs[lolo_idx].m_AnimCount == 1)
					nFrame = 0;

				m_LoloRoofs[lolo_idx].Render(pd3dDevice, nFrame,
						m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
			}
		}
	}

	if (!bPicking && m_LineVector0 && pWorldMat && pViewMat && m_LineType == LINE_TYPE_BRIGHT) {
		D3DLVERTEX lvertex[4];
		//D3DVECTOR lineVector[2];
		D3DVECTOR lineCenter;

		D3DMATRIX matInv;
		D3DMath_MatrixInvert(matInv, matView);

		// Line�� View ��ǥ�� �ٲ۴�.
		//D3DMath_VectorMatrixMultiply(lineVector[0], m_LineVector0[0], matView);
		//D3DMath_VectorMatrixMultiply(lineVector[1], m_LineVector0[1], matView);

		// Line�� ������ ������ ���Ѵ�. (�ٻ簪)
		float lineLen = MAX(PLUS(m_LineVector0[0].x - m_LineVector0[1].x), 
						MAX(PLUS(m_LineVector0[0].y - m_LineVector0[1].y),
							PLUS(m_LineVector0[0].z - m_LineVector0[1].z))) / 2;

		// ������ �߰����� ���Ѵ�.
		lineCenter.x = (m_LineVector0[0].x + m_LineVector0[1].x) / 2;
		lineCenter.y = (m_LineVector0[0].y + m_LineVector0[1].y) / 2;
		lineCenter.z = (m_LineVector0[0].z + m_LineVector0[1].z) / 2;
		D3DMath_VectorMatrixMultiply(lineCenter, lineCenter, matView);

			// �� 16Frame ���� 8Frame�� �����ش�.
		int brightFrame = oldFrame % 16 / 2;
		// 5->3, 6->2, 7->1
		if (brightFrame > 4)
			brightFrame = 8 - brightFrame;

		float textureU = brightFrame / 5.0f;

		// View ��ǥ�� �簢��
		lvertex[0] = D3DLVERTEX(D3DVECTOR(lineCenter.x - lineLen,
										lineCenter.y - lineLen,
										lineCenter.z - LIGHTSWORD_Z),
										0x55ffffff, 0L, textureU, 1);
		lvertex[1] = D3DLVERTEX(D3DVECTOR(lineCenter.x - lineLen,
										lineCenter.y + lineLen,
										lineCenter.z - LIGHTSWORD_Z),
										0x55ffffff, 0L, textureU, 0);
		lvertex[2] = D3DLVERTEX(D3DVECTOR(lineCenter.x + lineLen,
										lineCenter.y - lineLen,
										lineCenter.z - LIGHTSWORD_Z),
										0x55ffffff, 0L, textureU + 0.2f, 1);
		lvertex[3] = D3DLVERTEX(D3DVECTOR(lineCenter.x + lineLen,
										lineCenter.y + lineLen,
										lineCenter.z - LIGHTSWORD_Z),
										0x55ffffff, 0L, textureU + 0.2f, 0);

		// �簢���� World ��ǥ��...
		NKMath_LVertexMatrixMultiply(lvertex[0], lvertex[0], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[1], lvertex[1], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[2], lvertex[2], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[3], lvertex[3], matInv);

		unsigned short lineIndex[6];
		lineIndex[0] = 0;
		lineIndex[1] = 1;
		lineIndex[2] = 2;
		lineIndex[3] = 1;
		lineIndex[4] = 3;
		lineIndex[5] = 2;

		DWORD alphaBlend;
		DWORD alphaTest;
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		//pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
		// �׷���!
		LPDIRECTDRAWSURFACE7 lpTexture;
		if (m_LineTextureNum >= 0) {
			if ((lpTexture = m_pTexture->GetSurfaceByIdx(m_LineTextureNum)) != NULL) {
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK) {
					pd3dDevice->SetTexture(0, NULL);
				}
			} else {
				pd3dDevice->SetTexture(0, NULL);
			}
		} else {
			pd3dDevice->SetTexture(0, NULL);
		}
		pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX ,
						lvertex, 4, NULL );
		//pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
		//				lvertex, 4, lineIndex, 6, NULL );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		//pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	}
}

void CLoloObj::Render( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, float height, BOOL bPicking, 
					  IndexedTexture *pTexture, BOOL bShowLow, DWORD dwAlpha )
{
	if (m_VertexCount == 0 || m_FaceCount == 0)
		return;

	CurFrame %= m_AnimCount;

	LPDIRECTDRAWSURFACE7 lpTexture;
	// Texture�� ������ Texture ���
	if (m_TextureNum >= 0 && !bPicking/* && pTexture*/) {
		if ((lpTexture = pTexture->GetSurfaceByIdx(m_TextureNum)) != NULL) {
			if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK) {
				pd3dDevice->SetTexture(0, NULL);
			}
		} else {
			pd3dDevice->SetTexture(0, NULL);
		}
	} else {
		pd3dDevice->SetTexture(0, NULL);
	}

	LAGVERTEX *pVert;
	pVert = m_Vertices[CurFrame];

	// �׷���!
	DWORD alphaBlend;
	DWORD alphaTest;
	DWORD alphaRef;
	DWORD alphaFunc;
	if (m_bAlphablend) {
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHAFUNC, &alphaFunc);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x0000001);
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
		//pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
	}
	if (dwAlpha != 0xff) {
		DWORD diffuse = 0x00ffffff | dwAlpha;
		for (int idx = 0; idx < m_VertexCount; idx++) {
			pVert[idx].diffuse = diffuse;
			//pVert[idx].diffuse = 0xffffffff;
		}
	}
	if (bShowLow && m_IndexedLow) {
		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
					pVert, m_VertexCount,
					m_IndexedLow, m_FaceCountLow * 3, NULL);
		g_dwObjTriangles += m_FaceCountLow;
	} else {
		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
					pVert, m_VertexCount,
					m_Indexed, m_FaceCount * 3, NULL);
		g_dwObjTriangles += m_FaceCount;
	}
	if (m_bAlphablend) {
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF   , alphaRef);
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, alphaFunc);
		//pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	}
}

void CBiped::Read(FILE *fp)
{
	int read_start = 1;
	int i;
	D3DMATRIX mat_temp;

	// Half Frame������ �д´�..
	fread(&m_bHalfFrame, sizeof(BOOL), 1, fp);
	// Biped �� Object ������ �д´�.
	fread(&m_BipedNum, sizeof(int), 1, fp);
	/*if (m_BipedNum >= 35) {
		MessageBox(NULL, "�����е� ������ �� ������!", "����!", MB_OK);
	}*/
	m_AniMat = new CAniMatrix[m_BipedNum];
	// Biped �� Animation ������ �д´�.
	fread(&(m_AnimCount), sizeof(int), 1, fp);

	for (int bi = 0; bi < m_BipedNum; bi++) {
		// Read Objects - �̰� ���߿� �ʿ� ������ ����! Test��!

		// Read Vertices
		/*int vertex_num;

		D3DVECTOR *VectorList;
		fread(&vertex_num, sizeof(int), 1, fp);
		VectorList = new D3DVECTOR[vertex_num];

		for (i = 0; i < vertex_num; i++) {
			fread(&(VectorList[i].x), sizeof(float), 1, fp);
			fread(&(VectorList[i].y), sizeof(float), 1, fp);
			fread(&(VectorList[i].z), sizeof(float), 1, fp);
		}

		// Face�� ������ �ҷ��ͼ� Vertices�� �ʱ�ȭ ��Ų��.
		fread(&(m_AniMat[bi].m_FaceCount), sizeof(int), 1, fp);
		m_AniMat[bi].m_Vertices0 = new D3DVERTEX[m_AniMat[bi].m_FaceCount * 3];
		m_AniMat[bi].m_Vertices = new D3DVERTEX[m_AniMat[bi].m_FaceCount * 3];

		// Vertex�� ��ȣ�� �о�ͼ� ������ ������ Vertex������ ������ Face���� �����.
		for (i = 0; i < m_AniMat[bi].m_FaceCount * 3; i++) {
			int vert_idx;

			fread(&vert_idx, sizeof(int), 1, fp);
			m_AniMat[bi].m_Vertices0[i] = D3DVERTEX(VectorList[vert_idx], D3DVECTOR(0, 0, 0), 0, 0);
			m_AniMat[bi].m_Vertices[i] = m_AniMat[bi].m_Vertices0[i];
		}

		delete[] VectorList;*/

		// Animation Matrix �б�.
		int anim_count;
		if (m_bHalfFrame) {
			anim_count = (m_AnimCount+1)/2;
		} else {
			anim_count = m_AnimCount;
		}

		m_AniMat[bi].m_Mats = new D3DMATRIX[anim_count];
		mat_temp._14 = mat_temp._24 = mat_temp._34 = 0.0f;
		mat_temp._44 = 1.0f;
		for (i = 0; i < anim_count; i++) {
			fread(&(mat_temp._11), sizeof(float), 1, fp);
			fread(&(mat_temp._12), sizeof(float), 1, fp);
			fread(&(mat_temp._13), sizeof(float), 1, fp);
			fread(&(mat_temp._21), sizeof(float), 1, fp);
			fread(&(mat_temp._22), sizeof(float), 1, fp);
			fread(&(mat_temp._23), sizeof(float), 1, fp);
			fread(&(mat_temp._31), sizeof(float), 1, fp);
			fread(&(mat_temp._32), sizeof(float), 1, fp);
			fread(&(mat_temp._33), sizeof(float), 1, fp);
			fread(&(mat_temp._41), sizeof(float), 1, fp);
			fread(&(mat_temp._42), sizeof(float), 1, fp);
			fread(&(mat_temp._43), sizeof(float), 1, fp);

			memcpy(&(m_AniMat[bi].m_Mats[i]), &mat_temp, sizeof(D3DMATRIX));
		}
	}
	return;
}

//#define CSY_ALPHA	(0x55ffffff)
#define CSY_ALPHA	(0xffffffff)
int CCapsyong::Read(FILE *fp)
{
	int i;

	char name[100];
	int name_len;

	int object_num;
	int all_face_count = 0;
	int low_face_count = 0;

	CCsyObj *pCsy;

	fread(&object_num, sizeof(int), 1, fp);
	if (object_num < 1) {
		return FALSE;
	}

	if (object_num == 181818) {
		int i_temp;
		for (i = 0; i < 8; i++) {
			fread(&i_temp, sizeof(int), 1, fp);
		}
		fread(&object_num, sizeof(int), 1, fp);
	}

	m_ObjectCount = object_num;
	m_Csy = new CCsyObj[object_num];

	// Object�� ������ŭ �о�´�.
	for (int obj_idx = 0; obj_idx < object_num; obj_idx++) {
		pCsy = &(m_Csy[obj_idx]);

		// Material�� Texture���� �̸��� �д´�.
		fread(&name_len, sizeof(int), 1, fp);
		if (name_len) {
			fread(name, name_len, 1, fp);
			name[name_len] = '\0';

			if (!strncmp(name, "oo_", 3)) {
				pCsy->m_CsyEffect = CSY_EFF_ONEONE;
			}

			// Texture �о����.
			int TextureNum;
			//if ((TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xff)) < 0) {
			if ((TextureNum = m_pTexture->CreateTextureFromFile(name)) < 0) {
				/*CString errMsg;
				errMsg.Format("Create Texture Error!!: '%s'", name);
				AfxMessageBox(errMsg);*/
				pCsy->m_TextureNum = -1;
			} else {
				pCsy->m_TextureNum = TextureNum;
			}
		}

		// Read Vertices
		int vertex_num;

		fread(&vertex_num, sizeof(int), 1, fp);

		pCsy->m_Vertices0 = new LAGVERTEX[vertex_num];
		pCsy->m_Vertices = new LAGVERTEX[vertex_num];
		pCsy->m_BipedIdx = new short[vertex_num];

		pCsy->m_VertexCount = vertex_num;

		for (i = 0; i < vertex_num; i++) {
			//fread(&(pCsy->m_Vertices0[i]), sizeof(D3DVERTEX), 1, fp);
			fread(&(pCsy->m_Vertices0[i].x), sizeof(float), 1, fp);
			fread(&(pCsy->m_Vertices0[i].y), sizeof(float), 1, fp);
			fread(&(pCsy->m_Vertices0[i].z), sizeof(float), 1, fp);

			fread(&(pCsy->m_Vertices0[i].nx), sizeof(float), 1, fp);
			fread(&(pCsy->m_Vertices0[i].ny), sizeof(float), 1, fp);
			fread(&(pCsy->m_Vertices0[i].nz), sizeof(float), 1, fp);

			fread(&(pCsy->m_Vertices0[i].tu), sizeof(float), 1, fp);
			fread(&(pCsy->m_Vertices0[i].tv), sizeof(float), 1, fp);

			pCsy->m_Vertices0[i].diffuse = CSY_ALPHA;

			fread(&(pCsy->m_BipedIdx[i]), sizeof(short), 1, fp);
		}

		// Read Faces
		int face_num;
		fread(&(face_num), sizeof(int), 1, fp);
		pCsy->m_FaceCount = face_num;
		all_face_count += face_num;

		pCsy->m_Indexed = new unsigned short[face_num*3];
		int vert_idx;

		for (i = 0; i < face_num * 3; i++) {
			fread(&vert_idx, sizeof(int), 1, fp);
			pCsy->m_Indexed[i] = (unsigned short) vert_idx;
		}

		// Read Faces Low
		fread(&(face_num), sizeof(int), 1, fp);
		pCsy->m_FaceCountLow = face_num;
		low_face_count += face_num;

		if (face_num > 0) {
			pCsy->m_IndexedLow = new unsigned short[face_num*3];

			for (i = 0; i < face_num * 3; i++) {
				fread(&vert_idx, sizeof(int), 1, fp);
				pCsy->m_IndexedLow[i] = (unsigned short) vert_idx;
			}
		}

		// Vertex�� Copy
		for (i = 0; i < vertex_num; i++) {
			pCsy->m_Vertices[i] = pCsy->m_Vertices0[i];
		}
	}

	int hasLine;
	fread(&hasLine, sizeof(int), 1, fp);
	if (hasLine > 0) {
		m_LineVector0 = new D3DVECTOR[2];
		m_LineVector = new D3DVECTOR[6];

		fread(&(m_LineVector0[0].x), sizeof(float), 1, fp);
		fread(&(m_LineVector0[0].y), sizeof(float), 1, fp);
		fread(&(m_LineVector0[0].z), sizeof(float), 1, fp);
		fread(&(m_LineBiped[0]), sizeof(int), 1, fp);

		fread(&(m_LineVector0[1].x), sizeof(float), 1, fp);
		fread(&(m_LineVector0[1].y), sizeof(float), 1, fp);
		fread(&(m_LineVector0[1].z), sizeof(float), 1, fp);
		fread(&(m_LineBiped[1]), sizeof(int), 1, fp);

		fread(&name_len, sizeof(int), 1, fp);
		if (name_len) {
			fread(name, name_len, 1, fp);
			name[name_len] = '\0';

			// Texture �о����.
			int TextureNum;
			if ((TextureNum = m_pTexture->CreateTextureFromFile(name)) < 0) {
				m_LineTextureNum = -1;
			} else {
				m_LineTextureNum = TextureNum;
			}
		}
	}

	fread(&m_BoundNum, sizeof(int), 1, fp);
	if (m_BoundNum > 0) {
		fread(&m_BoundVertNum, sizeof(int), 1, fp);
		m_BoundVert = new D3DVERTEX[m_BoundVertNum];
		for (int vert = 0; vert < m_BoundVertNum; vert++) {
			float x, y, z;
			fread(&x, sizeof(float), 1, fp);
			fread(&y, sizeof(float), 1, fp);
			fread(&z, sizeof(float), 1, fp);
			m_BoundVert[vert] = D3DVERTEX(D3DVECTOR(x, y, z), D3DVECTOR(0, 0, 0), 0, 0);
		}
		fread(&m_BoundFaceNum, sizeof(int), 1, fp);
		m_BoundIndex = new unsigned short[m_BoundFaceNum*3];
		for (int idx = 0; idx < m_BoundFaceNum*3; idx++) {
			int indexed;
			fread(&indexed, sizeof(int), 1, fp);
			m_BoundIndex[idx] = indexed;
		}
	}
	//m_pTexture->RestoreAllTextures(pd3dDevice);

	return all_face_count;
}


//#define LOLO_ALPHA	(0x55)
#define LOLO_ALPHA	(0xFE)
int CLolos::Read( FILE *fp/*, LPDIRECT3DDEVICE7 pd3dDevice*/, BOOL bAlpha, BOOL bLoadWalkPlane )
{
	int i;

	char name[100];
	int name_len;

	int normal_obj_num;
	int ani_obj_num;
	int normal_roof_num;
	int ani_roof_num;
	int anim_num;
	int obj_idx;
	CLoloObj *pLoloObj;
	int TextureNum;
	int vertex_num;
	int vert_idx;

	int all_face_count = 0;
	int low_face_count = 0;

	D3DVERTEX vertTemp;

	// ���� Ȯ��!
	unsigned int version;
	fread(&version, sizeof(unsigned int), 1, fp);
	if (version != 100) {
		//AfxMessageBox("������ Ʋ��������!");
		return 0;
	}

	fread(&anim_num, sizeof(int), 1, fp);
	fread(&normal_obj_num, sizeof(int), 1, fp);
	fread(&ani_obj_num, sizeof(int), 1, fp);
	fread(&normal_roof_num, sizeof(int), 1, fp);
	fread(&ani_roof_num, sizeof(int), 1, fp);
	if (normal_obj_num + ani_obj_num + normal_roof_num + ani_roof_num < 1) {
		//AfxMessageBox("Object�� ������ 1���� �۽��ϴ�.");
		return FALSE;
	}

	m_AnimCount = anim_num;
	m_LoloObjCount = normal_obj_num + ani_obj_num;
	m_LoloRoofCount = normal_roof_num + ani_roof_num;

	m_LoloObjs = new CLoloObj[m_LoloObjCount];
	// Normal Object�� ������ŭ �о�´�.
	for (obj_idx = 0; obj_idx < normal_obj_num; obj_idx++) {
		// ������Ʈ �ʱ�ȭ
		pLoloObj = &(m_LoloObjs[obj_idx]);

		pLoloObj->m_AnimCount = 1;
		pLoloObj->m_Vertices = new LAGVERTEX*[1];
		pLoloObj->m_CenterPoint = new D3DVECTOR[1];

		// Material�� Texture���� �̸��� �д´�.
		fread(&name_len, sizeof(int), 1, fp);
		if (name_len) {
			fread(name, name_len, 1, fp);
			name[name_len] = '\0';

			if (!strncmp(name, "ap_", 3)) {
				pLoloObj->m_bAlphablend = TRUE;
				m_bMustRenderLast = TRUE;
			}
			// Texture �о����.
			if (bAlpha)
				TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, LOLO_ALPHA);
			else
				TextureNum = m_pTexture->CreateTextureFromFile(name);

			if (TextureNum < 0) {
				/*CString errMsg;
				errMsg.Format("Create Texture Error!!: '%s'", name);
				AfxMessageBox(errMsg);*/
				pLoloObj->m_TextureNum = -1;
			} else {
				pLoloObj->m_TextureNum = TextureNum;
			}
		}

		// Read Vertices
		fread(&vertex_num, sizeof(int), 1, fp);
		pLoloObj->m_VertexCount = vertex_num;

		pLoloObj->m_Vertices[0] = new LAGVERTEX[vertex_num];

		for (i = 0; i < vertex_num; i++) {
			fread(&vertTemp, sizeof(D3DVERTEX), 1, fp);
			pLoloObj->m_Vertices[0][i].x = vertTemp.x;
			pLoloObj->m_Vertices[0][i].y = vertTemp.y;
			pLoloObj->m_Vertices[0][i].z = vertTemp.z;
			pLoloObj->m_Vertices[0][i].nx = vertTemp.nx;
			pLoloObj->m_Vertices[0][i].ny = vertTemp.ny;
			pLoloObj->m_Vertices[0][i].nz = vertTemp.nz;
			pLoloObj->m_Vertices[0][i].tu = vertTemp.tu;
			pLoloObj->m_Vertices[0][i].tv = vertTemp.tv;
			pLoloObj->m_Vertices[0][i].diffuse = 0xffffffff;
		}
		// Read CenterPoint
		fread(&(pLoloObj->m_CenterPoint[0]), sizeof(D3DVECTOR), 1, fp);

		// Read Faces
		fread(&(pLoloObj->m_FaceCount), sizeof(int), 1, fp);
		pLoloObj->m_Indexed = new unsigned short[pLoloObj->m_FaceCount * 3];
		all_face_count += pLoloObj->m_FaceCount;

		for (i = 0; i < pLoloObj->m_FaceCount * 3; i++) {
			fread(&vert_idx, sizeof(int), 1, fp);
			pLoloObj->m_Indexed[i] = vert_idx;
		}

		// Read Low Faces
		fread(&(pLoloObj->m_FaceCountLow), sizeof(int), 1, fp);
		if (pLoloObj->m_FaceCountLow > 0) {
			pLoloObj->m_IndexedLow = new unsigned short[pLoloObj->m_FaceCountLow * 3];
			low_face_count += pLoloObj->m_FaceCountLow;

			//int vert_idx;
			for (i = 0; i < pLoloObj->m_FaceCountLow * 3; i++) {
				fread(&vert_idx, sizeof(int), 1, fp);
				pLoloObj->m_IndexedLow[i] = vert_idx;
			}
		}
	}

	// Animation Object�� ������ŭ �о�´�.
	for (obj_idx = normal_obj_num; obj_idx < m_LoloObjCount; obj_idx++) {
		// ������Ʈ �ʱ�ȭ
		pLoloObj = &(m_LoloObjs[obj_idx]);

		pLoloObj->m_AnimCount = anim_num;
		pLoloObj->m_Vertices = new LAGVERTEX*[anim_num];
		pLoloObj->m_CenterPoint = new D3DVECTOR[anim_num];

		// Material�� Texture���� �̸��� �д´�.
		fread(&name_len, sizeof(int), 1, fp);
		if (name_len) {
			fread(name, name_len, 1, fp);
			name[name_len] = '\0';

			if (!strncmp(name, "ap_", 3)) {
				pLoloObj->m_bAlphablend = TRUE;
				m_bMustRenderLast = TRUE;
			}
			// Texture �о����.
			if (bAlpha)
				TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, LOLO_ALPHA);
			else
				TextureNum = m_pTexture->CreateTextureFromFile(name);
			if (TextureNum < 0) {
				/*CString errMsg;
				errMsg.Format("Create Texture Error!!: '%s'", name);
				AfxMessageBox(errMsg);*/
				pLoloObj->m_TextureNum = -1;
			} else {
				pLoloObj->m_TextureNum = TextureNum;
			}
		}

		// Read Vertices
		fread(&vertex_num, sizeof(int), 1, fp);
		pLoloObj->m_VertexCount = vertex_num;

		for (int anim_idx = 0; anim_idx < anim_num; anim_idx++) {

			pLoloObj->m_Vertices[anim_idx] = new LAGVERTEX[vertex_num];

			for (i = 0; i < vertex_num; i++) {
				fread(&vertTemp, sizeof(D3DVERTEX), 1, fp);
				pLoloObj->m_Vertices[anim_idx][i].x = vertTemp.x;
				pLoloObj->m_Vertices[anim_idx][i].y = vertTemp.y;
				pLoloObj->m_Vertices[anim_idx][i].z = vertTemp.z;
				pLoloObj->m_Vertices[anim_idx][i].nx = vertTemp.nx;
				pLoloObj->m_Vertices[anim_idx][i].ny = vertTemp.ny;
				pLoloObj->m_Vertices[anim_idx][i].nz = vertTemp.nz;
				pLoloObj->m_Vertices[anim_idx][i].tu = vertTemp.tu;
				pLoloObj->m_Vertices[anim_idx][i].tv = vertTemp.tv;
				pLoloObj->m_Vertices[anim_idx][i].diffuse = 0xffffffff;
			}
			// Read CenterPoint
			fread(&(pLoloObj->m_CenterPoint[anim_idx]), sizeof(D3DVECTOR), 1, fp);
		}

		// Read Faces
		fread(&(pLoloObj->m_FaceCount), sizeof(int), 1, fp);
		pLoloObj->m_Indexed = new unsigned short[pLoloObj->m_FaceCount * 3];
		all_face_count += pLoloObj->m_FaceCount;

		for (i = 0; i < pLoloObj->m_FaceCount * 3; i++) {
			fread(&vert_idx, sizeof(int), 1, fp);
			pLoloObj->m_Indexed[i] = vert_idx;
		}

		fread(&(pLoloObj->m_FaceCountLow), sizeof(int), 1, fp);
		if (pLoloObj->m_FaceCountLow > 0) {
			pLoloObj->m_IndexedLow = new unsigned short[pLoloObj->m_FaceCountLow * 3];
			low_face_count += pLoloObj->m_FaceCountLow;

			//int vert_idx;
			for (i = 0; i < pLoloObj->m_FaceCountLow * 3; i++) {
				fread(&vert_idx, sizeof(int), 1, fp);
				pLoloObj->m_IndexedLow[i] = vert_idx;
			}
		}
	}

	m_LoloRoofs = new CLoloObj[m_LoloRoofCount];
	// ���� Object�� ������ŭ �о�´�.
	for (obj_idx = 0; obj_idx < normal_roof_num; obj_idx++) {
		// ������Ʈ �ʱ�ȭ
		pLoloObj = &(m_LoloRoofs[obj_idx]);

		pLoloObj->m_AnimCount = 1;
		pLoloObj->m_Vertices = new LAGVERTEX*[1];
		pLoloObj->m_CenterPoint = new D3DVECTOR[1];

		// Material�� Texture���� �̸��� �д´�.
		fread(&name_len, sizeof(int), 1, fp);
		if (name_len) {
			fread(name, name_len, 1, fp);
			name[name_len] = '\0';

			if (!strncmp(name, "ap_", 3)) {
				pLoloObj->m_bAlphablend = TRUE;
			}
			// Texture �о����.
			if (bAlpha)
				TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, LOLO_ALPHA);
			else
				TextureNum = m_pTexture->CreateTextureFromFile(name);
			if (TextureNum < 0) {
				/*CString errMsg;
				errMsg.Format("Create Texture Error!!: '%s'", name);
				AfxMessageBox(errMsg);*/
				pLoloObj->m_TextureNum = -1;
			} else {
				pLoloObj->m_TextureNum = TextureNum;
			}
		}

		// Read Vertices
		fread(&vertex_num, sizeof(int), 1, fp);
		pLoloObj->m_VertexCount = vertex_num;

		pLoloObj->m_Vertices[0] = new LAGVERTEX[vertex_num];

		for (i = 0; i < vertex_num; i++) {
			fread(&vertTemp, sizeof(D3DVERTEX), 1, fp);
			pLoloObj->m_Vertices[0][i].x = vertTemp.x;
			pLoloObj->m_Vertices[0][i].y = vertTemp.y;
			pLoloObj->m_Vertices[0][i].z = vertTemp.z;
			pLoloObj->m_Vertices[0][i].nx = vertTemp.nx;
			pLoloObj->m_Vertices[0][i].ny = vertTemp.ny;
			pLoloObj->m_Vertices[0][i].nz = vertTemp.nz;
			pLoloObj->m_Vertices[0][i].tu = vertTemp.tu;
			pLoloObj->m_Vertices[0][i].tv = vertTemp.tv;
			pLoloObj->m_Vertices[0][i].diffuse = 0xffffffff;
			//pLoloObj->m_Vertices[0][i].diffuse = 0x0;
		}
		// Read CenterPoint
		fread(&(pLoloObj->m_CenterPoint[0]), sizeof(D3DVECTOR), 1, fp);

		// Read Faces
		fread(&(pLoloObj->m_FaceCount), sizeof(int), 1, fp);
		pLoloObj->m_Indexed = new unsigned short[pLoloObj->m_FaceCount * 3];
		all_face_count += pLoloObj->m_FaceCount;

		for (i = 0; i < pLoloObj->m_FaceCount * 3; i++) {
			fread(&vert_idx, sizeof(int), 1, fp);
			pLoloObj->m_Indexed[i] = vert_idx;
		}

		// Read Low Faces
		fread(&(pLoloObj->m_FaceCountLow), sizeof(int), 1, fp);
		if (pLoloObj->m_FaceCountLow > 0) {
			pLoloObj->m_IndexedLow = new unsigned short[pLoloObj->m_FaceCountLow * 3];
			low_face_count += pLoloObj->m_FaceCountLow;

			//int vert_idx;
			for (i = 0; i < pLoloObj->m_FaceCountLow * 3; i++) {
				fread(&vert_idx, sizeof(int), 1, fp);
				pLoloObj->m_IndexedLow[i] = vert_idx;
			}
		}
	}

	// ���� Animation Object�� ������ŭ �о�´�.
	for (obj_idx = normal_roof_num; obj_idx < m_LoloRoofCount; obj_idx++) {
		// ������Ʈ �ʱ�ȭ
		pLoloObj = &(m_LoloRoofs[obj_idx]);

		pLoloObj->m_AnimCount = anim_num;
		pLoloObj->m_Vertices = new LAGVERTEX*[anim_num];
		pLoloObj->m_CenterPoint = new D3DVECTOR[anim_num];

		// Material�� Texture���� �̸��� �д´�.
		fread(&name_len, sizeof(int), 1, fp);
		if (name_len) {
			fread(name, name_len, 1, fp);
			name[name_len] = '\0';

			if (!strncmp(name, "ap_", 3)) {
				pLoloObj->m_bAlphablend = TRUE;
			}
			// Texture �о����.
			if (bAlpha)
				TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, LOLO_ALPHA);
			else
				TextureNum = m_pTexture->CreateTextureFromFile(name);
			if (TextureNum < 0) {
				/*CString errMsg;
				errMsg.Format("Create Texture Error!!: '%s'", name);
				AfxMessageBox(errMsg);*/
				pLoloObj->m_TextureNum = -1;
			} else {
				pLoloObj->m_TextureNum = TextureNum;
			}
		}

		// Read Vertices
		fread(&vertex_num, sizeof(int), 1, fp);
		pLoloObj->m_VertexCount = vertex_num;

		for (int anim_idx = 0; anim_idx < anim_num; anim_idx++) {

			pLoloObj->m_Vertices[anim_idx] = new LAGVERTEX[vertex_num];

			for (i = 0; i < vertex_num; i++) {
				fread(&vertTemp, sizeof(D3DVERTEX), 1, fp);
				pLoloObj->m_Vertices[anim_idx][i].x = vertTemp.x;
				pLoloObj->m_Vertices[anim_idx][i].y = vertTemp.y;
				pLoloObj->m_Vertices[anim_idx][i].z = vertTemp.z;
				pLoloObj->m_Vertices[anim_idx][i].nx = vertTemp.nx;
				pLoloObj->m_Vertices[anim_idx][i].ny = vertTemp.ny;
				pLoloObj->m_Vertices[anim_idx][i].nz = vertTemp.nz;
				pLoloObj->m_Vertices[anim_idx][i].tu = vertTemp.tu;
				pLoloObj->m_Vertices[anim_idx][i].tv = vertTemp.tv;
				pLoloObj->m_Vertices[anim_idx][i].diffuse = 0xffffffff;
			}
			// Read CenterPoint
			fread(&(pLoloObj->m_CenterPoint[anim_idx]), sizeof(D3DVECTOR), 1, fp);
		}

		// Read Faces
		fread(&(pLoloObj->m_FaceCount), sizeof(int), 1, fp);
		pLoloObj->m_Indexed = new unsigned short[pLoloObj->m_FaceCount * 3];
		all_face_count += pLoloObj->m_FaceCount;

		for (i = 0; i < pLoloObj->m_FaceCount * 3; i++) {
			fread(&vert_idx, sizeof(int), 1, fp);
			pLoloObj->m_Indexed[i] = vert_idx;
		}

		fread(&(pLoloObj->m_FaceCountLow), sizeof(int), 1, fp);
		if (pLoloObj->m_FaceCountLow > 0) {
			pLoloObj->m_IndexedLow = new unsigned short[pLoloObj->m_FaceCountLow * 3];
			low_face_count += pLoloObj->m_FaceCountLow;

			//int vert_idx;
			for (i = 0; i < pLoloObj->m_FaceCountLow * 3; i++) {
				fread(&vert_idx, sizeof(int), 1, fp);
				pLoloObj->m_IndexedLow[i] = vert_idx;
			}
		}
	}

	//m_CenterPoint = new D3DVECTOR[m_AnimCount];
	//for (i = 0; i < m_AnimCount; i++) {
		//fread(&(m_CenterPoint[i]), sizeof(D3DVECTOR), 1, fp);
	//}
	fread(&m_CenterPoint, sizeof(D3DVECTOR), 1, fp);
	float MinX, MaxX, MinY, MaxY, MinZ, MaxZ;
	fread(&MinX, sizeof(float), 1, fp);
	fread(&MaxX, sizeof(float), 1, fp);
	fread(&MinY, sizeof(float), 1, fp);
	fread(&MaxY, sizeof(float), 1, fp);
	fread(&MinZ, sizeof(float), 1, fp);
	fread(&MaxZ, sizeof(float), 1, fp);

	// Line Export
	int hasLine;
	fread(&hasLine, sizeof(int), 1, fp);
	if (hasLine > 0) {
		fread(&name_len, sizeof(int), 1, fp);
		if (name_len) {
			fread(name, name_len, 1, fp);
			name[name_len] = '\0';

			if (!strncmp(name, "hb_", 3)) {
				m_LineType = LINE_TYPE_FIRE;
			} else if (!strncmp(name, "jk_", 3)) {
				m_LineType = LINE_TYPE_SMOKE;
			} else if (!strncmp(name, "yn_", 3)) {
				m_LineType = LINE_TYPE_BLUEFIRE;
			} else if (!strncmp(name, "kr_", 3)) {
				m_LineType = LINE_TYPE_VOLCANO;
			} else if (!strncmp(name, "os_", 3)) {
				m_LineType = LINE_TYPE_POISON;
			} else if (!strncmp(name, "se_", 3)) {
				m_LineType = LINE_TYPE_GAS;
			}
			// Texture �о����.
			//if ((TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xff/*m_LoloAlpha*/)) < 0) {
			if ((TextureNum = m_pTexture->CreateTextureFromFile(name)) < 0) {
				/*CString errMsg;
				errMsg.Format("Create Texture Error!!: '%s'", name);
				AfxMessageBox(errMsg);*/
				m_LineTextureNum = -1;
			} else {
				m_LineTextureNum = TextureNum;
			}
		}

		m_LineVector0 = new D3DVECTOR[2];

		fread(&(m_LineVector0[0].x), sizeof(float), 1, fp);
		fread(&(m_LineVector0[0].y), sizeof(float), 1, fp);
		fread(&(m_LineVector0[0].z), sizeof(float), 1, fp);

		fread(&(m_LineVector0[1].x), sizeof(float), 1, fp);
		fread(&(m_LineVector0[1].y), sizeof(float), 1, fp);
		fread(&(m_LineVector0[1].z), sizeof(float), 1, fp);

		if (m_LineVector0[0].x < MinX)
			MinX = m_LineVector0[0].x;
		if (m_LineVector0[0].y < MinY)
			MinY = m_LineVector0[0].y;
		if (m_LineVector0[0].z < MinZ)
			MinZ = m_LineVector0[0].z;
		if (m_LineVector0[0].x > MaxX)
			MaxX = m_LineVector0[0].x;
		if (m_LineVector0[0].y > MaxY)
			MaxY = m_LineVector0[0].y;
		if (m_LineVector0[0].z > MaxZ)
			MaxZ = m_LineVector0[0].z;

		if (m_LineVector0[1].x < MinX)
			MinX = m_LineVector0[1].x;
		if (m_LineVector0[1].y < MinY)
			MinY = m_LineVector0[1].y;
		if (m_LineVector0[1].z < MinZ)
			MinZ = m_LineVector0[1].z;
		if (m_LineVector0[1].x > MaxX)
			MaxX = m_LineVector0[1].x;
		if (m_LineVector0[1].y > MaxY)
			MaxY = m_LineVector0[1].y;
		if (m_LineVector0[1].z > MaxZ)
			MaxZ = m_LineVector0[1].z;
	}
	m_BoundVect[0] = D3DVECTOR(MinX, MinY, MinZ);
	m_BoundVect[1] = D3DVECTOR(MinX, MinY, MaxZ);
	m_BoundVect[2] = D3DVECTOR(MaxX, MinY, MaxZ);
	m_BoundVect[3] = D3DVECTOR(MaxX, MinY, MinZ);
	m_BoundVect[4] = D3DVECTOR(MinX, MaxY, MinZ);
	m_BoundVect[5] = D3DVECTOR(MinX, MaxY, MaxZ);
	m_BoundVect[6] = D3DVECTOR(MaxX, MaxY, MaxZ);
	m_BoundVect[7] = D3DVECTOR(MaxX, MaxY, MinZ);


	// Floor �б�
	fread(&m_FloorCount, sizeof(int), 1, fp);
	if( m_FloorCount > 0 )
	{
		m_FloorVect = new D3DVECTOR[m_FloorCount * 4];
		for (i = 0; i < m_FloorCount; i++) {
			fread(&MinX, sizeof(float), 1, fp);
			fread(&MaxX, sizeof(float), 1, fp);
			fread(&MinZ, sizeof(float), 1, fp);
			fread(&MaxZ, sizeof(float), 1, fp);

			m_FloorVect[i*4+0] = D3DVECTOR(MinX, 0, MinZ);
			m_FloorVect[i*4+1] = D3DVECTOR(MinX, 0, MaxZ);
			m_FloorVect[i*4+2] = D3DVECTOR(MaxX, 0, MaxZ);
			m_FloorVect[i*4+3] = D3DVECTOR(MaxX, 0, MinZ);
		}
	}
	// InDoor �б�
	fread(&m_InDoorCount, sizeof(int), 1, fp);
	if( m_InDoorCount > 0 )
	{
		m_InDoorVect = new D3DVECTOR[m_InDoorCount * 4];
		for (i = 0; i < m_InDoorCount; i++) {
			fread(&MinX, sizeof(float), 1, fp);
			fread(&MaxX, sizeof(float), 1, fp);
			fread(&MinZ, sizeof(float), 1, fp);
			fread(&MaxZ, sizeof(float), 1, fp);

			m_InDoorVect[i*4+0] = D3DVECTOR(MinX, 0, MinZ);
			m_InDoorVect[i*4+1] = D3DVECTOR(MinX, 0, MaxZ);
			m_InDoorVect[i*4+2] = D3DVECTOR(MaxX, 0, MaxZ);
			m_InDoorVect[i*4+3] = D3DVECTOR(MaxX, 0, MinZ);
		}
	}
	// OutDoor �б�
	fread(&m_OutDoorCount, sizeof(int), 1, fp);
	if( m_OutDoorCount > 0 )
	{
		m_OutDoorVect = new D3DVECTOR[m_OutDoorCount * 4];
		for (i = 0; i < m_OutDoorCount; i++) {
			fread(&MinX, sizeof(float), 1, fp);
			fread(&MaxX, sizeof(float), 1, fp);
			fread(&MinZ, sizeof(float), 1, fp);
			fread(&MaxZ, sizeof(float), 1, fp);

			m_OutDoorVect[i*4+0] = D3DVECTOR(MinX, 0, MinZ);
			m_OutDoorVect[i*4+1] = D3DVECTOR(MinX, 0, MaxZ);
			m_OutDoorVect[i*4+2] = D3DVECTOR(MaxX, 0, MaxZ);
			m_OutDoorVect[i*4+3] = D3DVECTOR(MaxX, 0, MinZ);
		}
	}

	if( bLoadWalkPlane )
	{
		fread( &m_WalkPlaneCount, sizeof(int), 1, fp );
		if( m_WalkPlaneCount > 0 )
		{
			D3DVECTOR	vTemp;
			m_WalkPlaneVect = new D3DVECTOR[m_WalkPlaneCount * 4];
			for( i = 0; i < m_WalkPlaneCount * 4; i++ )
			{
				fread( &vTemp, sizeof(D3DVECTOR), 1, fp );
				m_WalkPlaneVect[i] = vTemp;
			}
		}
	}


	//m_pTexture->RestoreAllTextures(pd3dDevice);

	/*CString str;
	str.Format("%d, %d", all_face_count, low_face_count);
	AfxMessageBox(str);*/
	return all_face_count;
}



HRESULT NKMath_VectorMatrixMultiply( D3DVECTOR& vDest, D3DVECTOR& vSrc,
                                      D3DMATRIX& mat)
{
    vDest.x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
    vDest.y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
    vDest.z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
    //FLOAT w = 1.0f;//vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;
    
    //vDest.x = x;///w;
    //vDest.y = y;///w;
    //vDest.z = z;///w;

    return S_OK;
}

HRESULT NKMath_VertexMatrixMultiply( D3DVERTEX& vDest, D3DVERTEX& vSrc,
                                      D3DMATRIX& mat)
{
    vDest.x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
    vDest.y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
    vDest.z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
    //FLOAT w = 1.0f;//vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;
    
    //vDest.x = x;///w;
    //vDest.y = y;///w;
    //vDest.z = z;///w;

    vDest.nx = vSrc.nx*mat._11 + vSrc.ny*mat._21 + vSrc.nz* mat._31;// + mat._41;
    vDest.ny = vSrc.nx*mat._12 + vSrc.ny*mat._22 + vSrc.nz* mat._32;// + mat._42;
    vDest.nz = vSrc.nx*mat._13 + vSrc.ny*mat._23 + vSrc.nz* mat._33;// + mat._43;
    //w = 1.0f;//vSrc.nx*mat._14 + vSrc.ny*mat._24 + vSrc.nz* mat._34 + mat._44;
    
    //vDest.nx = x;///w;
    //vDest.ny = y;///w;
    //vDest.nz = z;///w;

    return S_OK;

}

HRESULT NKMath_LagVertexMatrixMultiply( LAGVERTEX& vDest, LAGVERTEX& vSrc,
                                      D3DMATRIX& mat)
{
    vDest.x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
    vDest.y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
    vDest.z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
    //FLOAT w = 1.0f;//vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;
    
    //vDest.x = x;///w;
    //vDest.y = y;///w;
    //vDest.z = z;///w;

    vDest.nx = vSrc.nx*mat._11 + vSrc.ny*mat._21 + vSrc.nz* mat._31;// + mat._41;
    vDest.ny = vSrc.nx*mat._12 + vSrc.ny*mat._22 + vSrc.nz* mat._32;// + mat._42;
    vDest.nz = vSrc.nx*mat._13 + vSrc.ny*mat._23 + vSrc.nz* mat._33;// + mat._43;
    //w = 1.0f;//vSrc.nx*mat._14 + vSrc.ny*mat._24 + vSrc.nz* mat._34 + mat._44;
    
    //vDest.nx = x;///w;
    //vDest.ny = y;///w;
    //vDest.nz = z;///w;

    return S_OK;

}

HRESULT NKMath_LVertexMatrixMultiply( D3DLVERTEX& vDest, D3DLVERTEX& vSrc,
                                      D3DMATRIX& mat)
{
    FLOAT x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
    FLOAT y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
    FLOAT z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
    FLOAT w = vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;
    
    if( fabs( w ) < g_EPSILON )
        return E_INVALIDARG;

    vDest.x = x/w;
    vDest.y = y/w;
    vDest.z = z/w;

	return S_OK;
}
