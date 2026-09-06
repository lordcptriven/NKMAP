#ifndef __theodoric_undo_object_200710251828__
#define __theodoric_undo_object_200710251828__

// [2007/10/29 theodoric] 무식하게 undo 기능을 추가해보자 ㅠㅠ

#include "d3dapp.h"
#include "DecoObj.h"

#define UNDO_OBJ_MAX 200

typedef enum UNDO_OBJECT_TYPE
{ 
	UNDO_OBJ_NONE = 0, 
	UNDO_OBJ_MOVE, 
	UNDO_OBJ_ARRAY 
};

typedef struct UNDO_OBJ
{
	UNDO_OBJECT_TYPE	type;
	D3DVECTOR			pos;
	float				dir;	
	CDecoObj*			pDecObject;
	
	UNDO_OBJ*			pParent;
	
	UNDO_OBJ()
	{
		type		= UNDO_OBJ_NONE;
		pos.x		= 0.0f;
		pos.y		= 0.0f;
		pos.z		= 0.0f;
		dir			= 0.0f;
		pDecObject	= NULL;
		pParent		= NULL;
	}

	UNDO_OBJ operator = (UNDO_OBJ r)
	{UNDO_OBJ s; s.dir=r.dir; s.pos.x=r.pos.x; s.pos.y=r.pos.y;s.pos.z=r.pos.z; s.pDecObject=r.pDecObject; s.pParent=r.pParent;s.type=r.type;  return s; }

}_undo_object_data;


class CUndoObject
{
protected:

	int				m_UndoCount;
	UNDO_OBJ*		m_pUndoObj;
	list<UNDO_OBJ*>	m_SelObjBakList;
	
public:

	CUndoObject();
	~CUndoObject();

	void init();
	void Destory();

	// 선택된 오브젝트들의 정보를 답는다.
	void OnSelObjListBackup();
	
	// 선택된 오브젝트들의 정보를 Undo에 추가한다.
	void OnSelObjListUndoSet();

	// 선택된 오브젝트 정볼르 백업한 정보를 삭제한다.
	void DeleteSelBackupList();

	void AddUndoObject(UNDO_OBJ* pUndoObj);

	// 차례대로 하나씩 삭제한다.
	void DeleteUndo();
	
	void DeleteUndo(CDecoObj* pDecObject);	
	
	// undo 기능을 실행한다.
	void OnUndo();

	// undo에 추가된 오브젝트를 지워준다.
	void DeleteObject();

};


#endif