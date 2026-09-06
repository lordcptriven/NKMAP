
#ifndef __CAPSYONG_H__
#define __CAPSYONG_H__

#define E_VERTEX_MATRIX_MULTIPLY	(-1)
#define E_NO_MATRIX				(-2)

#define MAX_LOLO_VERT		1000
#define SWING_FRAME			80

#define LINE_EFF_NONE			0
#define LINE_EFF_LIGHTSWORD		1
#define LINE_EFF_GUN			2
#define LINE_EFF_BRIGHT			3

#define LINE_TYPE_BRIGHT		0
#define LINE_TYPE_FIRE			1
#define LINE_TYPE_SMOKE			2
#define LINE_TYPE_BLUEFIRE		3
#define LINE_TYPE_VOLCANO		4
#define LINE_TYPE_POISON		5
#define LINE_TYPE_GAS			6

#define CSY_EFF_NONE			0
#define CSY_EFF_SKIN			1	// ������� ����.
#define CSY_EFF_ONEONE			2

#include <stdio.h>

#define FVF_LAGHAIM (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct CVF {
	D3DVALUE x;
	D3DVALUE y;
	D3DVALUE z;

	D3DVALUE nx;
	D3DVALUE ny;
	D3DVALUE nz;

	D3DCOLOR  diffuse;
	D3DVALUE tu, tv;
} LAGVERTEX;

class IndexedTexture;
//class CVirtualFile;

typedef struct CVertexTemp {
	D3DVERTEX	m_Vert;
	short	m_Int;
} VertexTemp;

class CAniMatrix {
public:
	CAniMatrix();
	~CAniMatrix();

	//int m_FaceCount;
	//D3DVERTEX *m_Vertices0;
	//D3DVERTEX *m_Vertices;

	D3DMATRIX *m_Mats;
};

class CBiped {
public:
	CBiped();
	~CBiped();

	void Read(FILE *fp);

	int m_AnimCount;
	int m_BipedNum;
	BOOL m_bHalfFrame;

	CAniMatrix *m_AniMat;
};

class CCsyObj {
public:
	CCsyObj();
	~CCsyObj();

	int m_TextureNum;
	int m_CsyEffect;

	int m_VertexCount;
	int m_FaceCount;
	int m_FaceCountLow;
	LAGVERTEX *m_Vertices0;
	LAGVERTEX *m_Vertices;
	short *m_BipedIdx;
	unsigned short *m_Indexed;
	unsigned short *m_IndexedLow;

};

class CCapsyong {
public:
	CCapsyong(IndexedTexture *pTexture);
	~CCapsyong();

	int Read(FILE *fp/*, LPDIRECT3DDEVICE7 pd3dDevice*/);
	//int Read(CVirtualFile *pvf/*, LPDIRECT3DDEVICE7 pd3dDevice*/);
	int Animate(CBiped *pBiped, int CurFrame, BOOL bTrace = FALSE, int whatEff = LINE_EFF_NONE, D3DMATRIX *pWorldMat = NULL, D3DMATRIX *pViewMat = NULL,
					D3DVECTOR *pTargetVector = NULL, int csyEff = CSY_EFF_NONE);
	void Render( LPDIRECT3DDEVICE7 pd3dDevice, int textureNum = -1, BOOL bPicking = FALSE, BOOL bShowLow = FALSE, LPDIRECTDRAWSURFACE7 traceTexture = NULL,
					/*LPDIRECTDRAWSURFACE7 lightswordTexture = NULL,*/ int effFrame = -1);
	void RenderBound( LPDIRECT3DDEVICE7 pd3dDevice );

	IndexedTexture *m_pTexture;
	int m_ObjectCount;
	CCsyObj *m_Csy;
	BOOL m_bLightEffect;

	int m_LineBiped[2];
	D3DVECTOR *m_LineVector0;
	D3DVECTOR *m_LineVector;
	int m_LineTextureNum;

	D3DVECTOR m_RectVector[4];

	int m_BoundNum;
	int m_BoundVertNum;
	D3DVERTEX *m_BoundVert;
	int m_BoundFaceNum;
	unsigned short *m_BoundIndex;
};

class CLoloObj {
public:
	CLoloObj();
	~CLoloObj();

	void Render( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, float height, BOOL bPicking, IndexedTexture *pTexture, BOOL bShowLow, DWORD dwAlpha);

	int m_bAlphablend;
	int m_AnimCount;
	int m_TextureNum;

	int m_VertexCount;
	int m_FaceCount;
	int m_FaceCountLow;
	LAGVERTEX **m_Vertices;
	unsigned short *m_Indexed;
	unsigned short *m_IndexedLow;

	D3DVECTOR *m_CenterPoint;
	D3DVECTOR m_CenterWorld;
};

class CLolos {
public:
	CLolos();
	CLolos(IndexedTexture *pTexture);
	~CLolos();

	void SetIndexedTexture(IndexedTexture *pTexture);
	int Read( FILE *fp/*, LPDIRECT3DDEVICE7 pd3dDevice*/, BOOL bAlpha = FALSE, BOOL bLoadWalkPlane = FALSE );
	//int Read(CVirtualFile *pvf, BOOL bAlpha = FALSE);
	void Render( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame = 0, /*int swingFrame = -1, */BOOL bPicking = FALSE, BOOL bNoRoof = FALSE, BOOL bShowLow = FALSE, D3DMATRIX *pWorldMat = NULL,
					D3DMATRIX *pViewMat = NULL, BOOL bSort = FALSE, BOOL bNoLight = FALSE, DWORD dwAlpha = 0xff );

	BOOL m_bMustRenderLast;
	BOOL MustRenderLast() { return (m_bMustRenderLast || (m_LineVector0 && (m_LineType == LINE_TYPE_BRIGHT))); }
	IndexedTexture *m_pTexture;

	int m_AnimCount;

	int m_LoloObjCount;
	CLoloObj *m_LoloObjs;
	int m_LoloRoofCount;
	CLoloObj *m_LoloRoofs;

	int m_LineType;
	int m_LineTextureNum;
	D3DVECTOR *m_LineVector0;

	//D3DVECTOR *m_CenterPoint;
	D3DVECTOR m_CenterPoint;
	D3DVECTOR m_BoundVect[8];

	int m_FloorCount;
	D3DVECTOR *m_FloorVect;
	int m_InDoorCount;
	D3DVECTOR *m_InDoorVect;
	int m_OutDoorCount;
	D3DVECTOR *m_OutDoorVect;
	int	m_WalkPlaneCount;
	D3DVECTOR*	m_WalkPlaneVect;
};

HRESULT NKMath_VectorMatrixMultiply( D3DVECTOR& vDest, D3DVECTOR& vSrc,
                                      D3DMATRIX& mat);
HRESULT NKMath_LVertexMatrixMultiply( D3DLVERTEX& vDest, D3DLVERTEX& vSrc,
                                      D3DMATRIX& mat);
HRESULT NKMath_VertexMatrixMultiply( D3DVERTEX& vDest, D3DVERTEX& vSrc,
                                      D3DMATRIX& mat);
HRESULT NKMath_LagVertexMatrixMultiply( LAGVERTEX& vDest, LAGVERTEX& vSrc,
                                      D3DMATRIX& mat);

#define EDDY_FRAME		40

#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define PLUS(a)		((a) > 0 ? (a) : (-(a)))

#endif
