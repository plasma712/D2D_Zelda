#include "stdafx.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "AstarMgr.h"
#include "Mouse.h"
#include "Terrain.h"

D3DXVECTOR3 CPlayer::m_PlayerPos = { 0.f, 0.f, 0.f };

CPlayer::CPlayer()
	: m_pAstar(CAstarMgr::GetInstance())
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
}


CPlayer::~CPlayer()
{
	Release();
}

int CPlayer::Update()
{
	CGameObject::LateInit();

	KeyInput();
	MoveFrame();
	MovePath();
	Physics();

	return NO_EVENT;
}

void CPlayer::LateUpdate()
{

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, 
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;


	//Physics();



}

void CPlayer::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Idle", L"Front", (int)m_tFrame.fStartFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, 
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CPlayer::Initialize()
{
	m_tInfo.vPos = { 430.f, 300.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };

	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fMaxFrameCnt = 4.f;
	m_fSpeedX = 5.f;

	return S_OK;
}

HRESULT CPlayer::LateInit()
{
	return S_OK;
}

void CPlayer::Release()
{
	m_pAstar->DestroyInstance();
}

void CPlayer::MoveFrame()
{
	// 1초에 fMaxFrameCnt만큼 재생되고 있다.
	m_tFrame.fStartFrame += m_tFrame.fMaxFrameCnt * m_pTimeMgr->GetDeltaTime();

	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrameCnt)
		m_tFrame.fStartFrame = 0.f;
}

void CPlayer::KeyInput()
{
	//if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
	//{
	//	D3DXVECTOR3 vMouse = CMouse::GetMouse();
	//	vMouse += CScrollMgr::GetScrollPos();

	//	m_pAstar->StartAstar(m_tInfo.vPos, vMouse);
	//}

	if (m_pKeyMgr->KeyPressing(KEY_RIGHT))
	{
		m_tInfo.vPos += D3DXVECTOR3(m_fSpeedX, 0.f, 0.f);
	}
	if (m_pKeyMgr->KeyPressing(KEY_LEFT))
	{
		m_tInfo.vPos -= D3DXVECTOR3(m_fSpeedX, 0.f, 0.f);
	}

}

void CPlayer::MovePath()
{
	list<TILE_INFO*>& BestPath = m_pAstar->GetBestPath();

	if (BestPath.empty())
		return;

	m_tInfo.vDir = BestPath.front()->vPos - m_tInfo.vPos;
	float fLength = D3DXVec3Length(&m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	if (3.f > fLength)
	{
#ifdef _DEBUG
		BestPath.front()->byDrawID = 2;
#endif
		BestPath.pop_front();
	}

	m_tInfo.vPos += m_tInfo.vDir * 300.f * m_pTimeMgr->GetDeltaTime();
}

CPlayer* CPlayer::Create()
{
	CPlayer* pInstance = new CPlayer;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}


const D3DXVECTOR3 & CPlayer::GetPlayerPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_PlayerPos;
}

void CPlayer::SetColData(TerrainPair _cp)
{
	CollCheck = true;
	m_collpair.first = _cp.first;
	m_collpair.second = _cp.second;
}

void CPlayer::Physics()
{
	if(m_collpair.second->GetTileInfo().byDrawID== IMMORTALWALL)
	{
		if (m_collpair.first == Left ||
			m_collpair.first == Right)
		{
			m_fSpeedX *= -1;	// 벽에 충돌시 X속도 반전
		}
		else if (m_collpair.first == Up || m_collpair.first == Down)
		{
			m_fSpeedY = -1;	//바닥에 부딫혔을때..
		}
	}

}
