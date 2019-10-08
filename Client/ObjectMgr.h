#pragma once

class CGameObject;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)

public:
	enum OBJECT_TYPE { TERRAIN, PLAYER, MONSTER, EFFECT, UI, END };

private:
	CObjectMgr();
	~CObjectMgr();

public:
	CGameObject* GetTerrain();
	CGameObject* GetPlayer();

public:
	HRESULT AddObject(OBJECT_TYPE eType, CGameObject* pObject);
	void Update();
	void LateUpdate();
	void Render();

private:
	void Release();

private:
	typedef list<CGameObject*>	OBJLIST;
	OBJLIST	m_ObjectLst[END];
};

