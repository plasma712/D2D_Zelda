#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_bIsInit(false)
{
	m_tRECT = {};
}


CGameObject::~CGameObject()
{
}

// 모든 오브젝트들의 Initialize를 호출하고나서 호출되는 또다른 초기화 함수.
HRESULT CGameObject::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}

	return S_OK;
}

const INFO & CGameObject::GetInfo()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pInfo;
}

TILE_INFO & CGameObject::GetTileInfo()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return tileInfo;
}

void CGameObject::UpdateRECT()
{
	m_tRECT.left = tileInfo.vPos.x - TILECX / 2;
	m_tRECT.right = tileInfo.vPos.y - TILECY / 2;
	m_tRECT.top = tileInfo.vPos.x + TILECX / 2;
	m_tRECT.bottom = tileInfo.vPos.y + TILECY / 2;
}

RECT CGameObject::GetRect()
{
	RECT rc = {
		tileInfo.vPos.x - TILECX / 2,
		tileInfo.vPos.y - TILECY / 2,
		tileInfo.vPos.x + TILECX / 2,
		tileInfo.vPos.y + TILECY / 2
	};
	return rc;

}
