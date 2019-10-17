#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Player.h"
#include "ObjectTerrain.h"

CStage::CStage()
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Update()
{
	m_pObjectMgr->Update();
}

void CStage::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
}

void CStage::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CStage::Initialize()
{
	HRESULT hr = 0;

	hr = m_pTextureMgr->LoadFromImgPathFile(L"../Data/ImgPath.txt");
	FAILED_CHECK_MSG_RETURN(hr, L"LoadFromImgPathFile Failed", E_FAIL);

	// Terrain
	hr = m_pObjectMgr->AddObject(TERRAIN, CTerrain::Create(Stage01));
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);

	// TerrainObject
	hr = m_pObjectMgr->AddObject(TERRAINOBJECT, ObjectTerrain::Create(Stage01));
	FAILED_CHECK_MSG_RETURN(hr, L"ObjectTerrain Create Failed", E_FAIL);


	// Player
	hr = m_pObjectMgr->AddObject(PLAYER, CPlayer::Create());
	FAILED_CHECK_MSG_RETURN(hr, L"Player Create Failed", E_FAIL);
	
	dynamic_cast<CTerrain*>(m_pObjectMgr->GetTerrain())->SetPlayerPointer(dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer()));

	return S_OK;
}

void CStage::Release()
{
}

CStage* CStage::Create()
{
	CStage* pInstance = new CStage;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
