#include "stdafx.h"
#include "Terrain.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include "Player.h"
#include "ColliderMgr.h"

CTerrain::CTerrain()
	:m_pPlayer(nullptr)
{
}


CTerrain::~CTerrain()
{
	Release();
}

int CTerrain::GetTileIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (IsPicking(vPos, i))
			return (int)i;
	}

	return -1;
}

const vector<TILE_INFO*>& CTerrain::GetVecTile() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecTile;
}

const vector<list<TILE_INFO*>>& CTerrain::GetVecGraph() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecGraph;
}

int CTerrain::Update()
{
	CGameObject::LateInit();

	// 1초에 500씩 움직여라.
	float fSpeed = 500.f * m_pTimeMgr->GetDeltaTime();


	// 마우스 위치에 따르는 스크롤 
	//if (0.f > CMouse::GetMouse().x)
	//	CScrollMgr::SetScrollPos(D3DXVECTOR3(-fSpeed, 0.f, 0.f));
	//if (0.f > CMouse::GetMouse().y)
	//	CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, -fSpeed, 0.f));
	//if ((float)WINCX < CMouse::GetMouse().x)
	//	CScrollMgr::SetScrollPos(D3DXVECTOR3(fSpeed, 0.f, 0.f));
	//if ((float)WINCY < CMouse::GetMouse().y)
	//	CScrollMgr::SetScrollPos(D3DXVECTOR3(0.f, fSpeed, 0.f));

	TerrainPair temp = CollMgr->TileCollision(&m_vecTile2, m_pPlayer);
	
	if (temp.second != nullptr)
	{
		m_pPlayer->SetColData(temp);
		//temp.second->
	}

	return NO_EVENT;
}

void CTerrain::LateUpdate()
{
}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		D3DXMatrixScaling(&matScale,
			m_vecTile[i]->vSize.x,
			m_vecTile[i]->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x - CScrollMgr::GetScrollPos().x,
			m_vecTile[i]->vPos.y - CScrollMgr::GetScrollPos().y,
			0.f);

		matWorld = matScale * matTrans;

		//pTexInfo = m_pTextureMgr->GetTexInfo(L"Dungen", L"Tile", m_vecTile[i]->byDrawID);
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

HRESULT CTerrain::Initialize()
{
	m_vecTile.reserve(TILEX * TILEY);

	//HRESULT hr = LoadTile(L"../Data/MapData.dat");
	HRESULT hr = LoadTile(L"../Data/MapBlock.dat");
	FAILED_CHECK_MSG_RETURN(hr, L"MapData Load Failed", E_FAIL);

	return S_OK;
}

HRESULT CTerrain::LateInit()
{
	//CreateGraph();
	return S_OK;
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTerrain::TileChange(
	const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption)
{
	int iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = byOption;
}

bool CTerrain::IsPicking(const D3DXVECTOR3 & vPos, size_t iIndex)
{
	// 내적 (데카르트 기준)

	// 마름모의 꼭지점 시계방향으로 12시, 3시, 6시, 9시를 구한다.
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
	};

	// 시계방향으로 4개의 방향벡터를 구한다.
	D3DXVECTOR3 vDir[4] = 
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 위에서 구한 방향벡터의 법선벡터들을 구한다.
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDir[i]);

	// 각 꼭지점에서 마우스 위치로 향하는 벡터 4개 구한다.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vNormal와 vMouseDir을 내적해서 모두 음수(둔각)가 나오면 true
	for (int i = 0; i < 4; ++i)
	{
		// 하나라도 양수(예각)가 나오면 false
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;



}

HRESULT CTerrain::LoadTile(const TCHAR* pFilePath)
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

//void CTerrain::CreateGraph()
//{
//	m_vecGraph.resize(TILEX * TILEY);
//
//	for (int i = 0; i < TILEY; ++i)
//	{
//		for (int j = 0; j < TILEX; ++j)
//		{
//			int iIndex = i * TILEX + j;
//
//			// 좌상단
//			if ((0 != i) && (0 != iIndex % (TILEX * 2)))
//			{
//				if (!(i % 2) && (0 == m_vecTile[iIndex - (TILEX + 1)]->byOption))
//					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
//				else if((i % 2) && (0 == m_vecTile[iIndex - TILEX]->byOption))
//					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TILEX]);
//			}
//
//			// 우상단
//			if ((0 != i) && (TILEX * 2 - 1 != iIndex % (TILEX * 2)))
//			{
//				if (!(i % 2) && (0 == m_vecTile[iIndex - TILEX]->byOption))
//					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TILEX]);
//				else if ((i % 2) && (0 == m_vecTile[iIndex - (TILEX - 1)]->byOption))
//					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
//			}
//
//			// 좌하단
//			if ((TILEY - 1 != i) && (0 != iIndex % (TILEX * 2)))
//			{
//				if (!(i % 2) && (0 == m_vecTile[iIndex + (TILEX - 1)]->byOption))
//					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
//				else if ((i % 2) && (0 == m_vecTile[iIndex + TILEX]->byOption))
//					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + TILEX]);
//			}
//
//			// 우하단
//			if ((TILEY - 1 != i) && (TILEX * 2 - 1 != iIndex % (TILEX * 2)))
//			{
//				if (!(i % 2) && (0 == m_vecTile[iIndex + TILEX]->byOption))
//					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + TILEX]);
//				else if ((i % 2) && (0 == m_vecTile[iIndex + (TILEX + 1)]->byOption))
//					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);
//			}
//		}
//	}
//}

CTerrain* CTerrain::Create()
{
	CTerrain* pInstance = new CTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTerrain::SetPlayerPointer(CPlayer * _player)
{
	m_pPlayer = _player;
}

void CTerrain::Collider(const D3DXVECTOR3 & vPos, BYTE byDrawID, BYTE byOption)
{

}
