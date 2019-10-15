#include "stdafx.h"
#include "ObjectTerrain.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "ColliderMgr.h"


ObjectTerrain::ObjectTerrain()
{
}


ObjectTerrain::~ObjectTerrain()
{
	Release();
}

int ObjectTerrain::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (IsPicking(vPos, i))
			return (int)i;
	}

	return -1;
}

const vector<TILE_INFO*>& ObjectTerrain::GetVecTile() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecTile;
}

const vector<list<TILE_INFO*>>& ObjectTerrain::GetVecGraph() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecGraph;
}

int ObjectTerrain::Update()
{
	CGameObject::LateInit();
	return NO_EVENT;
}

void ObjectTerrain::LateUpdate()
{
}

void ObjectTerrain::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		D3DXMatrixScaling(&matScale,
			m_vecTile[i]->vSize.x,
			m_vecTile[i]->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x + TILECX / 2 - CScrollMgr::GetScrollPos().x,
			m_vecTile[i]->vPos.y - CScrollMgr::GetScrollPos().y,
			0.f);

		matWorld = matScale * matTrans;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"ObjectStage01", L"Tile", m_vecTile[i]->byDrawID);
		//pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

HRESULT ObjectTerrain::Initialize()
{
	m_vecTile.reserve(TILEX * TILEY);

	//HRESULT hr = LoadTile(L"../Data/MapData.dat");
	HRESULT hr = LoadTile(L"../Data/MapBlock01.dat");
	FAILED_CHECK_MSG_RETURN(hr, L"MapData Load Failed", E_FAIL);

	return S_OK;
}

HRESULT ObjectTerrain::LateInit()
{
	return S_OK;
}

void ObjectTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void ObjectTerrain::TileChange(const D3DXVECTOR3 & vPos, BYTE byDrawID, BYTE byOption)
{
}

bool ObjectTerrain::IsPicking(const D3DXVECTOR3 & vPos, size_t iIndex)
{
	return false;
}

HRESULT ObjectTerrain::LoadTile(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	if (!m_vecTile.empty())
	{
		for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
		m_vecTile.clear();
		m_vecTile.shrink_to_fit();
	}

	DWORD dwBytes = 0;
	TILE_INFO tInfo;
	TILE_INFO* pTile = nullptr;

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(TILE_INFO), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;

		pTile = new TILE_INFO(tInfo);
		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

	return S_OK;
}

void ObjectTerrain::CreateGraph()
{
}

ObjectTerrain * ObjectTerrain::Create()
{
	ObjectTerrain* pInstance = new ObjectTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
