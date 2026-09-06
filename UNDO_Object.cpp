#include "StdAfx.h"
#include "MyNKApp.h"
#include "NKVertexType.h"
#include "NKCamera.h"
#include "NKTerrain.h"
#include "NKObjQuadNode.h"
#include "DecoObj.h"
#include "Undo_Object.h"

extern CMyNKApp*		g_pMyNKApp;
extern CNKObjQuadNode*	g_pObjTree;
extern CDecoObjSet*		g_pDecoObjSet;


CUndoObject::CUndoObject()
{
	init();
}

CUndoObject::~CUndoObject()
{
	Destory();
}


void CUndoObject::init()
{
	m_UndoCount = 0;
	m_pUndoObj	= NULL;

}

void CUndoObject::Destory()
{
	if( m_pUndoObj != NULL )
	{
		UNDO_OBJ* pTemp;
		while(1)
		{
			pTemp = m_pUndoObj->pParent;
			delete m_pUndoObj;
			m_pUndoObj = pTemp;

			if( m_pUndoObj == NULL)
				break;
		}
	}

	DeleteSelBackupList();
}


// 선택된 오브젝트 정보를 백업한다.
void CUndoObject::OnSelObjListBackup()
{
	if( g_pMyNKApp->m_lstSelDecoObjs.size() == 0 )
		return;

	DeleteSelBackupList();

	CDecoObj*					pDecoObj;
	list<CDecoObj*>::iterator	iterDeco;

	for( iterDeco =g_pMyNKApp-> m_lstSelDecoObjs.begin(); iterDeco != g_pMyNKApp->m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj = *iterDeco;
		UNDO_OBJ* temp;
		temp = new UNDO_OBJ;

		temp->type	= UNDO_OBJ_MOVE;
		temp->dir	= pDecoObj->m_dir;
		temp->pos.x = pDecoObj->m_wx;
		temp->pos.y = pDecoObj->m_wy;
		temp->pos.z = pDecoObj->m_wz;
		temp->pDecObject = pDecoObj;
		m_SelObjBakList.push_back(temp);
	}
}

// 선택된 오브젝트를 언도에 추가해준다.
// 무싲하지만 어쩔 수 없다. ㅜㅜ
void CUndoObject::OnSelObjListUndoSet()
{
	CDecoObj*					pDecoObj;
	UNDO_OBJ*					pDecoObjBak;
	list<CDecoObj*>::iterator	iterDeco;
	list<UNDO_OBJ*>::iterator	iterDecoBak;
	iterDecoBak = m_SelObjBakList.begin();

	if( m_SelObjBakList.size() == 0)
		return;

	for( iterDeco = g_pMyNKApp->m_lstSelDecoObjs.begin(); iterDeco != g_pMyNKApp->m_lstSelDecoObjs.end(); iterDeco++ )
	{
		pDecoObj	= *iterDeco;
		
		// 다중 선택시의 문제 때문에 이중 for문으로 검색한다. ㅜㅜ
		for( iterDecoBak = m_SelObjBakList.begin(); iterDecoBak != m_SelObjBakList.end(); iterDecoBak ++)
		{
			pDecoObjBak	= *iterDecoBak;
		
			if(		pDecoObjBak->pDecObject == pDecoObj &&
				(	pDecoObj->m_wx != pDecoObjBak->pos.x ||	pDecoObj->m_wy	!= pDecoObjBak->pos.y
 				||	pDecoObj->m_wz != pDecoObjBak->pos.z ||	pDecoObj->m_dir	!= pDecoObjBak->dir))
			{
				AddUndoObject(pDecoObjBak);
			}
		}
	}

	pDecoObj = NULL;
	pDecoObjBak = NULL;

	DeleteSelBackupList();
}



// Undo를 추가한다.
void CUndoObject::AddUndoObject(UNDO_OBJ* pUndoObj)
{
	UNDO_OBJ* undoObj;
	undoObj = new UNDO_OBJ;


	switch(pUndoObj->type)
	{
	case UNDO_OBJ_MOVE:		
		undoObj->type = pUndoObj->type;
		undoObj->pos.x		= pUndoObj->pos.x;
		undoObj->pos.y		= pUndoObj->pos.y;
		undoObj->pos.z		= pUndoObj->pos.z;
		undoObj->dir		= pUndoObj->dir;
		undoObj->pDecObject = pUndoObj->pDecObject;
		break;

	case UNDO_OBJ_ARRAY:
		undoObj->type = pUndoObj->type; 
		undoObj->pos.x		= 0.0f;
		undoObj->pos.y		= 0.0f;
		undoObj->pos.z		= 0.0f;
		undoObj->dir		= 0.0f;
		undoObj->pDecObject = pUndoObj->pDecObject;
		break;
	}

	// 처음 시작할때
	if( m_UndoCount <= 0 && m_pUndoObj == NULL )
	{
		m_pUndoObj = undoObj;

		m_UndoCount ++;
	}
	// Undo max를 초과 할때나 같을 때 
	else if( m_UndoCount >= UNDO_OBJ_MAX ) 
	{
		undoObj->pParent = m_pUndoObj;
		m_pUndoObj = undoObj;

		UNDO_OBJ* tempUndo = m_pUndoObj;

		while(1)
		{
			if( tempUndo->pParent->pParent == NULL )
			{
				delete tempUndo->pParent;
				tempUndo->pParent = NULL;
				break;
			}
			tempUndo = tempUndo->pParent;
		}
		tempUndo = NULL;		
	}
	else
	{
		undoObj->pParent = m_pUndoObj;
		m_pUndoObj = undoObj;

		m_UndoCount++;
	}
}


// 차례대로 하나씩 삭제한다.
void CUndoObject::DeleteUndo()
{
	if( m_pUndoObj == NULL)
		return;

	if(m_pUndoObj->pParent != NULL )
	{
		UNDO_OBJ* tempUndo = m_pUndoObj->pParent ;
		delete [] m_pUndoObj;
		m_pUndoObj = tempUndo;
		tempUndo = NULL;
	}
	else
	{
		delete [] m_pUndoObj;
		m_pUndoObj = NULL;
	}

	m_UndoCount --;

}	

void CUndoObject::DeleteUndo(CDecoObj* pDecObject)
{
	if( m_pUndoObj == NULL )
		return;

	UNDO_OBJ* pTemp;

	// 꼭 이렇게 해줘야 하는지 모르겠다... ㅜㅜ
	// 머리 아파서 쉽게 쉽게 가기로 했다.
	while(1)
	{
		if( m_pUndoObj->pDecObject == pDecObject )
		{
			pTemp = m_pUndoObj;
			m_pUndoObj = m_pUndoObj->pParent;
			delete [] pTemp;
			pTemp = NULL;
			
			m_UndoCount --;
		}
		else
		{
			if( m_pUndoObj == NULL || m_pUndoObj->pParent == NULL )
				return;
			else
				break;
		}

		if( m_pUndoObj == NULL )
			return;
	}

	pTemp = m_pUndoObj;

	while(1)
	{	
		if( pTemp->pParent->pDecObject == pDecObject )
		{
			UNDO_OBJ* pDeltemp = pTemp->pParent;
			pTemp->pParent = pTemp->pParent->pParent;
			delete [] pDeltemp;

			pDeltemp = NULL;
			m_UndoCount --;
		}
		else			
			pTemp = pTemp->pParent;

		if( pTemp == NULL || pTemp->pParent == NULL)
			break;
	}
	pTemp = NULL;	
}

// undo 기능을 실행한다.
void CUndoObject::OnUndo()
{

	if( m_pUndoObj == NULL && g_pMyNKApp->GetWorkingState() == OBJECT_EDIT ) 
		return;

	switch( m_pUndoObj->type )
	{
		case UNDO_OBJ_MOVE: // 이동과 회전 Undo

			g_pMyNKApp->MoveAndRotateSelDecoObjs(m_pUndoObj->pDecObject, m_pUndoObj->pos.x, m_pUndoObj->pos.y, m_pUndoObj->pos.z, m_pUndoObj->dir);
			g_pMyNKApp->UpdateChangedDecoObjs();
			DeleteUndo();

			break;

		case UNDO_OBJ_ARRAY: // 생성 된걸 사제하자

			int nX = m_pUndoObj->pDecObject->m_wx / CNKObjQuadNode::GetCellSize();
			int nZ = m_pUndoObj->pDecObject->m_wz / CNKObjQuadNode::GetCellSize();
			
			CNKObjQuadNode* pObjQuadNode;
			pObjQuadNode = CNKObjQuadNode::GetQuadNode( nZ, nX );
				
			if( !pObjQuadNode->HasThisDecoObj( m_pUndoObj->pDecObject ) )
			{
				bool	bFind = false;
				pObjQuadNode = NULL;
				g_pObjTree->FindQuadNode( m_pUndoObj->pDecObject, &pObjQuadNode, bFind );
			}
			
			CDecoObj	*ptemp = m_pUndoObj->pDecObject;
			DeleteUndo(m_pUndoObj->pDecObject);
			
			//선택 리스트에서 삭제
			// 에러가 나는데 왜 버그가 나는지 찾기가 싫당 ㅡㅡ;;
			if(g_pMyNKApp->m_lstSelDecoObjs.size() > 0)
			{
				list<CDecoObj*>::iterator	iterDeco = find( g_pMyNKApp->m_lstSelDecoObjs.begin(), g_pMyNKApp->m_lstSelDecoObjs.end(), ptemp );
				g_pMyNKApp->m_lstSelDecoObjs.erase( iterDeco );
			}

			g_pDecoObjSet->m_vecRefCount[ptemp->m_nLoloId]--;	 
			pObjQuadNode->RemoveDecoObj( ptemp );// , false );

			g_pMyNKApp->MakeDecoBoxMesh();
			g_pMyNKApp->MakeNodeBoundMesh();
			g_pMyNKApp->CalcGizmoPosition();
			
			g_pMyNKApp->UpdateObjEditReferenceCount();			
			
			break;
	}	
}


void CUndoObject::DeleteSelBackupList()
{
	if( m_SelObjBakList.size() == 0)
		return;
	
	UNDO_OBJ*			pTemp;
	list<UNDO_OBJ*>::iterator iter;

	for( iter = m_SelObjBakList.begin(); iter != m_SelObjBakList.end(); iter++ )
	{
		pTemp = *iter;
		delete pTemp;
		pTemp = NULL;	
	}

	m_SelObjBakList.clear();

}


void CUndoObject::DeleteObject()
{

}