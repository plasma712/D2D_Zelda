#include "stdafx.h"
#include "Stage02.h"
#include "Terrain.h"
#include "Player.h"
#include "ObjectTerrain.h"


CStage02::CStage02()
{
}


CStage02::~CStage02()
{
	Release();
}


void CStage02::Update()
{
	m_pObjectMgr->Update();
}

void CStage02::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CStage02::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CStage02::Initialize()
{
	HRESULT hr = 0;

	hr = m_pTextureMgr->LoadFromImgPathFile(L"../Data/ImgPath.txt");
	FAILED_CHECK_MSG_RETURN(hr, L"LoadFromImgPathFile Failed", E_FAIL);

	// Terrain
	hr = m_pObjectMgr->AddObject(TERRAIN, CTerrain::Create(Stage02));
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);

	// TerrainObject
	hr = m_pObjectMgr->AddObject(TERRAINOBJECT, ObjectTerrain::Create(Stage02));
	FAILED_CHECK_MSG_RETURN(hr, L"ObjectTerrain Create Failed", E_FAIL);

	// Player
	hr = m_pObjectMgr->AddObject(PLAYER, CPlayer::Create());
	FAILED_CHECK_MSG_RETURN(hr, L"Player Create Failed", E_FAIL);

	return S_OK;
}

void CStage02::Release()
{
}

CStage02 * CStage02::Create()
{
	CStage02* pInstance = new CStage02;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
