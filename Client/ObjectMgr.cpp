#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	Release();
}

CGameObject* CObjectMgr::GetTerrain()
{
	if(m_ObjectLst[TERRAIN].empty())
		return nullptr;

	return m_ObjectLst[TERRAIN].front();
}

CGameObject * CObjectMgr::GetObjectTerrain()
{
	if (m_ObjectLst[TERRAINOBJECT].empty())
		return nullptr;

	return m_ObjectLst[TERRAINOBJECT].front();
}

CGameObject * CObjectMgr::GetPlayer()
{
	if (m_ObjectLst[PLAYER].empty())
		return nullptr;

	return m_ObjectLst[PLAYER].front();
}

HRESULT CObjectMgr::AddObject(OBJECT_TYPE eType, CGameObject* pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_ObjectLst[eType].push_back(pObject);

	return S_OK;
}

void CObjectMgr::Update()
{
	OBJLIST::iterator iter_begin;
	OBJLIST::iterator iter_end;

	for (int i = 0; i < END; ++i)
	{
		iter_begin = m_ObjectLst[i].begin();
		iter_end = m_ObjectLst[i].end();

		for (; iter_begin != iter_end; )
		{
			int iEvent = (*iter_begin)->Update();

			if (DEAD_OBJ == iEvent)
			{
				SafeDelete(*iter_begin);
				iter_begin = m_ObjectLst[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
}

void CObjectMgr::LateUpdate()
{
	for (int i = 0; i < END; ++i)
	{
		for (auto& pObject : m_ObjectLst[i])
			pObject->LateUpdate();
	}
}

void CObjectMgr::Render()
{
	for (int i = 0; i < END; ++i)
	{
		for (auto& pObject : m_ObjectLst[i])
			pObject->Render();
	}
}

void CObjectMgr::Release()
{
	for (int i = 0; i < END; ++i)
	{
		for_each(m_ObjectLst[i].begin(), m_ObjectLst[i].end(), SafeDelete<CGameObject*>);
		m_ObjectLst[i].clear();
	}
}
