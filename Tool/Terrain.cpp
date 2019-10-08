#include "stdafx.h"
#include "Terrain.h"
#include "ToolView.h"

CTerrain::CTerrain()
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

void CTerrain::SetView(CToolView* pView)
{
	m_pView = pView;
}

int CTerrain::Update()
{
	CGameObject::LateInit();

	return NO_EVENT;
}

void CTerrain::LateUpdate()
{
}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	TCHAR szIndex[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		D3DXMatrixScaling(&matScale,
			m_vecTile[i]->vSize.x,
			m_vecTile[i]->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x - m_pView->GetScrollPos(0),
			m_vecTile[i]->vPos.y - m_pView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 다이렉트 폰트 출력
		swprintf_s(szIndex, L"%d", i);

		m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(),
			szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

HRESULT CTerrain::Initialize()
{
	m_vecTile.reserve(TILEX * TILEY);

	TILE_INFO* pTile = nullptr;
	float fX = 0.f, fY = 0.f;

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			//fX = j * TILECX + (i % 2) * (TILECX * 0.5f);
			//fY = i * (TILECY * 0.5f);
			fX = j * TILECX * 2 + TILECX;
			fY = i * TILECY * 2 + TILECY;

			pTile = new TILE_INFO;
			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { 4.f, 4.f, 0.f };
			pTile->byDrawID = 4;
			pTile->byOption = 0;
			pTile->iMyIndex = i * TILEX + j;
			pTile->iParentIndex = 0;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

HRESULT CTerrain::LateInit()
{
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

void CTerrain::MiniRender()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	TCHAR szIndex[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		D3DXMatrixScaling(&matScale,
			m_vecTile[i]->vSize.x * 0.3f,
			m_vecTile[i]->vSize.y * 0.3f,
			0.f);
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x * 0.3f,
			m_vecTile[i]->vPos.y * 0.3f,
			0.f);

		matWorld = matScale * matTrans;

		pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain::SaveTile(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwBytes = 0;

	for (auto& pTile : m_vecTile)
		WriteFile(hFile, pTile, sizeof(TILE_INFO), &dwBytes, nullptr);

	CloseHandle(hFile);
}

void CTerrain::LoadTile(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

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


	//////////////////////////////////////////////////////////////////////


	//// 직선의 방정식 (데카르트 기준)
	//// 마름모를 이루는 직선 4개에 대한 기울기를 구한다.
	//float fGradient[4] = 
	//{
	//	-(TILECY * 0.5f) / (TILECX * 0.5f),
	//	(TILECY * 0.5f) / (TILECX * 0.5f),
	//	-(TILECY * 0.5f) / (TILECX * 0.5f),
	//	(TILECY * 0.5f) / (TILECX * 0.5f)
	//};

	//// 마름모의 꼭지점 시계방향으로 12시, 3시, 6시, 9시를 구한다.
	//D3DXVECTOR3 vPoint[4] =
	//{
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f),
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f),
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
	//};

	//// Y절편 구한다.
	//// 0 = ax + b - y
	//// b = y - ax
	//float fB[4] = 
	//{
	//	vPoint[0].y - fGradient[0] * vPoint[0].x,
	//	vPoint[1].y - fGradient[1] * vPoint[1].x,
	//	vPoint[2].y - fGradient[2] * vPoint[2].x,
	//	vPoint[3].y - fGradient[3] * vPoint[3].x
	//};

	//// 0 = ax + b - y: 현재 점은 직선과 같은 선상에 있다. (직선이 점을 통과한다)
	//// 0 > ax + b - y: 현재 점은 직선보다 위에 있다.
	//// 0 < ax + b - y: 현재 점은 직선보다 아래에 있다.
	//return (0 < fGradient[0] * vPos.x + fB[0] - vPos.y &&
	//	0 > fGradient[1] * vPos.x + fB[1] - vPos.y &&
	//	0 > fGradient[2] * vPos.x + fB[2] - vPos.y &&
	//	0 < fGradient[3] * vPos.x + fB[3] - vPos.y);
}

CTerrain* CTerrain::Create(CToolView* pView)
{
	NULL_CHECK_RETURN(pView, nullptr);

	CTerrain* pInstance = new CTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetView(pView);

	return pInstance;
}

bool CTerrain::CheckInOut(const POINT & pt, const int iIndex)
{
	const TEX_INFO* pTexInfo =
		m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

	float left = m_vecTile[iIndex]->vPos.x - pTexInfo->tImgInfo.Width*0.5f;
	float right = m_vecTile[iIndex]->vPos.x + pTexInfo->tImgInfo.Width*0.5f;
	float top = m_vecTile[iIndex]->vPos.y - pTexInfo->tImgInfo.Height*0.5f;
	float bottom = m_vecTile[iIndex]->vPos.y + pTexInfo->tImgInfo.Height*0.5f;


	if (pt.x >= left &&
		pt.x <= right &&
		pt.y >= top &&
		pt.y <= bottom)
		return true;
	else
		return false;
}
