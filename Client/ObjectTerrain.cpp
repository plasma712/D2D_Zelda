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

	RECT ColTemp;

	D3DXVECTOR2 vPoint[5] = {};



	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (m_vecTile[i]->byOption == MOVEOBJECT || m_vecTile[i]->byOption == Door)
		{
			ColTemp = ObjectTerrainGet(m_vecTile[i]);
			vPoint[0] = { (float)ColTemp.left, (float)ColTemp.top };
			vPoint[1] = { (float)ColTemp.right,(float)ColTemp.top };
			vPoint[2] = { (float)ColTemp.right,(float)ColTemp.bottom };
			vPoint[3] = { (float)ColTemp.left, (float)ColTemp.bottom };
			vPoint[4] = { (float)ColTemp.left, (float)ColTemp.top };

			D3DXMatrixScaling(&matScale,
				m_vecTile[i]->vSize.x,
				m_vecTile[i]->vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[i]->vPos.x /*+ TILECX / 2*/ - CScrollMgr::GetScrollPos().x,
				m_vecTile[i]->vPos.y - CScrollMgr::GetScrollPos().y,
				0.f);

			matWorld = matScale * matTrans;

			pTexInfo = m_pTextureMgr->GetTexInfo(L"MainStage01", L"Tile", m_vecTile[i]->byDrawID);
			//pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
			NULL_CHECK(pTexInfo);

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(140, 255, 255, 255));

			m_pDeviceMgr->GetLine()->SetWidth(10.f);
			m_pDeviceMgr->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 10, 100));

		}
	}
}

HRESULT ObjectTerrain::Initialize()
{
	m_vecTile.reserve(TILEX * TILEY);

	//HRESULT hr = LoadTile(L"../Data/MapBlock01.dat");
	//FAILED_CHECK_MSG_RETURN(hr, L"MapData Load Failed", E_FAIL);
	StageSelect(StageNumbering);

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

ObjectTerrain * ObjectTerrain::Create(int _StageNumbering)
{
	ObjectTerrain* pInstance = new ObjectTerrain;
	pInstance->StageNumbering = _StageNumbering;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT ObjectTerrain::StageSelect(int _StageNumbering)
{
	HRESULT hr;

	switch (_StageNumbering)
	{
	case Stage01:
		hr = LoadTile(L"../Data/MainStageObject01.dat");
		FAILED_CHECK_MSG_RETURN(hr, L"MapBlock01 Load Failed", E_FAIL);
		break;
	case Stage02:
		hr = LoadTile(L"../Data/MapObjectTemp.dat");
		FAILED_CHECK_MSG_RETURN(hr, L"MapObjectTemp Load Failed", E_FAIL);
		break;
	}

	return S_OK;
}

void ObjectTerrain::GetObjectKey(int _StageNumbering)
{
	// 추후
}

void ObjectTerrain::GetStateKey(int _StageNumbering)
{
	//추후 Terrain보면서 하기
}

RECT ObjectTerrain::ObjectTerrainGet(TILE_INFO * _vecTile)
{
	RECT rc =
	{
		_vecTile->vPos.x - TILECX - CScrollMgr::GetScrollPos().x,
		_vecTile->vPos.y - TILECY - CScrollMgr::GetScrollPos().y,
		_vecTile->vPos.x + TILECX - CScrollMgr::GetScrollPos().x,
		_vecTile->vPos.y + TILECY - CScrollMgr::GetScrollPos().y
	};
	return rc;
}
