#include "stdafx.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "AstarMgr.h"
#include "Mouse.h"
#include "Terrain.h"
#include "PlayerEffect.h"

#include "NormalEffect.h"
#include "BuffEffect.h"
#include "AnimateIMP.h"
#include "NoAnimateIMP.h"


D3DXVECTOR3 CPlayer::m_PlayerPos = { 0.f, 0.f, 0.f };

CPlayer::CPlayer()
	: m_pAstar(CAstarMgr::GetInstance())
	//m_wstrObjectKey(0), m_wstrStateKey(L"Front")
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
	//MovePath();
	//Physics();
	FSMANI(m_FsmPair);


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
}

void CPlayer::Render()
{
	AnimationPicture();
}

HRESULT CPlayer::Initialize()
{
	m_tInfo.vPos = { 430.f, 300.f, 0.f };
	m_tInfo.vSize = { 2.5f, 2.5f, 0.f };

	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fMaxFrameCnt = 4.f; // 계속 FSM함수에서 바꿔야함... 흠...
	m_fSpeedX = 1.f;

	m_FsmPair.first = BehaviorIdle;
	m_FsmPair.second = ArrowDOWN;
	m_wstrObjectKey = L"Idle";
	m_wstrStateKey = L"Front";

	fAniMationSpeedControl = 1.f;
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
	FrameMotionCheck = true;
	// 1초에 fMaxFrameCnt만큼 재생되고 있다.
	m_tFrame.fStartFrame += m_tFrame.fMaxFrameCnt * m_pTimeMgr->GetDeltaTime() * fAniMationSpeedControl;

	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrameCnt)
	{
		m_tFrame.fStartFrame = 0.f;
		FrameMotionCheck = false;
	}
}

void CPlayer::KeyInput()
{
#pragma region 주석
	//if (m_pKeyMgr->KeyUp(KEY_LBUTTON))
	//{
	//	D3DXVECTOR3 vMouse = CMouse::GetMouse();
	//	vMouse += CScrollMgr::GetScrollPos();

	//	m_pAstar->StartAstar(m_tInfo.vPos, vMouse);
	//}

	/*if (m_pKeyMgr->KeyPressing(KEY_RIGHT))
	{
		m_tInfo.vPos += D3DXVECTOR3(m_fSpeedX, 0.f, 0.f);
		m_wstrObjectKey = L"Move";
		m_wstrStateKey = L"Right";
		m_tFrame.fMaxFrameCnt = 6;
	}
	if (m_pKeyMgr->KeyUp(KEY_RIGHT))
	{
		m_wstrObjectKey = L"Idle";
		m_tFrame.fMaxFrameCnt = 4;
	}
	if (m_pKeyMgr->KeyPressing(KEY_LEFT))
	{
		m_tInfo.vPos -= D3DXVECTOR3(m_fSpeedX, 0.f, 0.f);
	}
	*/

#pragma endregion
	FSM();

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
	//if(m_collpair.second->GetTileInfo().byDrawID== IMMORTALWALL)
	//{
	//	if (m_collpair.first == Left ||
	//		m_collpair.first == Right)
	//	{
	//		m_fSpeedX *= -1;	// 벽에 충돌시 X속도 반전
	//	}
	//	else if (m_collpair.first == Up || m_collpair.first == Down)
	//	{
	//		m_fSpeedY = -1;	//바닥에 부딫혔을때..
	//	}
	//}


}

void CPlayer::AnimationPicture()
{
	//pTexInfo = m_pTextureMgr->GetTexInfo(L"Idle", L"Front", (int)m_tFrame.fStartFrame);
	pTexInfo = TextureCall(m_wstrObjectKey, m_wstrStateKey, (int)m_tFrame.fStartFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

FsmPair CPlayer::FSM()
{
	static float TimeCheck = 0.f;
	Arrow();
	/////////////////////////////////////////////////////////////////
	if (m_pKeyMgr->KeyPressing(KEY_UP) || m_pKeyMgr->KeyPressing(KEY_DOWN)
		|| m_pKeyMgr->KeyPressing(KEY_RIGHT) || m_pKeyMgr->KeyPressing(KEY_LEFT))
	{
		TimeCheck += m_pTimeMgr->GetDeltaTime();
		if (TimeCheck > 0.01f)
		{
			BeHaviorFsm(BehaviorWalk, 6);
			ArrowMove(m_FsmPair);
		}
	}

	/////////////////////////////////////////////////////////////////
	if (m_pKeyMgr->KeyUp(KEY_UP) || m_pKeyMgr->KeyUp(KEY_DOWN)
		|| m_pKeyMgr->KeyUp(KEY_RIGHT) || m_pKeyMgr->KeyUp(KEY_LEFT))
	{
		BeHaviorFsm(BehaviorIdle, 4);
		TimeCheck = 0.f;
	}
	/////////////////////////////////////////////////////////////////
	if (m_pKeyMgr->KeyPressing(KEY_A))
	{
		m_FsmPair.first = BehaviorAttack;
		TimeCheck += m_pTimeMgr->GetDeltaTime();
		if (m_tFrame.fStartFrame > 0 && MotionSkip == false)
		{
			m_tFrame.fStartFrame = 0;
			MotionSkip = true;
		}
		if ((int)m_tFrame.fStartFrame > 2 && AttackMotion == false)
		{
			BeAttackEffect();
			AttackMotion = true;
		}

		if (FrameMotionCheck == true && AttackMotion == true)
		{
			AttackMotion = false;
			TimeCheck = 0.f;
		}

	}
	if (m_pKeyMgr->KeyUp(KEY_A))
	{
		m_FsmPair.first = BehaviorIdle;
		TimeCheck = 0.f;
		AttackMotion = false;
		MotionSkip = false;
	}


	return m_FsmPair;
}

void CPlayer::FSMANI(FsmPair _m_FsmPair)
{
	switch (_m_FsmPair.first)
	{
	case BehaviorIdle:
		m_wstrObjectKey = L"Idle";
		break;
	case BehaviorWalk:
		m_wstrObjectKey = L"Move";
		break;
	case BehaviorAttack:
		m_wstrObjectKey = L"AttackMotion";
		break;
	}

	switch (_m_FsmPair.second)
	{
	case ArrowUP:
		m_wstrStateKey = L"Front";
		break;
	case ArrowDOWN:
		m_wstrStateKey = L"Back";
		break;
	case ArrowRIGHT:
		m_wstrStateKey = L"Right";
		break;
	case ArrowLEFT:
		m_wstrStateKey = L"Left";
		break;
	}
}


const TEX_INFO * CPlayer::TextureCall(const wstring & wstrObjectKey, const wstring & wstrStateKey, int iIndex)
{
	return m_pTextureMgr->GetTexInfo(wstrObjectKey, wstrStateKey, (int)m_tFrame.fStartFrame);
}

void CPlayer::BeIdle()
{
}

void CPlayer::BeWalk()
{
}

void CPlayer::BeAttack()
{
}

void CPlayer::BeAttackEffect()
{
	// 구현층
	FRAME tFrame = { 0.f, 1.f };
	CAnimateIMP* pImp = CAnimateIMP::Create(tFrame, L"AttackEffect", m_wstrStateKey, 3.5f);
	//CNoAnimateIMP* pImp = CNoAnimateIMP::Create(10.f, L"Crash");

	// 추상층
	INFO TempInfo;

	switch (m_FsmPair.second)
	{
	case ArrowUP:
		TempInfo.vPos = { 0,70.f,0 };
		break;
	case ArrowDOWN:
		TempInfo.vPos = { 0,-70.f,0 };
		break;
	case ArrowRIGHT:
		TempInfo.vPos = { 70.f,0,0 };
		break;
	case ArrowLEFT:
		TempInfo.vPos = { -70.f,0,0 };
		break;
	default:
		break;
	}

	CNormalEffect* pEffect = CNormalEffect::Create(pImp, (this->m_tInfo.vPos + TempInfo.vPos), { 2.0f,2.0f,2.0f });
	//CBuffEffect* pEffect = CBuffEffect::Create(pImp);		
	m_pObjectMgr->AddObject(CObjectMgr::EFFECT, pEffect);

	////////////////////

	// 옆에 검
	//// 구현층
	//FRAME tFrame1 = { 0.f, 3.f };
	//CAnimateIMP* pImp1 = CAnimateIMP::Create(tFrame1, L"Attack", m_wstrStateKey);
	////CNoAnimateIMP* pImp = CNoAnimateIMP::Create(10.f, L"Crash");

	//// 추상층
	//INFO TempInfo1;

	//switch (m_FsmPair.second)
	//{
	//case ArrowUP:
	//	TempInfo1.vPos = { 50.f,0,0 };
	//	break;
	//case ArrowDOWN:
	//	TempInfo1.vPos = { -50.f,0,0 };
	//	break;
	//case ArrowRIGHT:
	//	TempInfo1.vPos = { 0,-50.f,0 };
	//	break;
	//case ArrowLEFT:
	//	TempInfo1.vPos = { 0,50.f,0 };
	//	break;
	//}

	//CNormalEffect* pEffect1 = CNormalEffect::Create(pImp1, (this->m_tInfo.vPos + TempInfo1.vPos), { 2.0f,2.0f,2.0f });
	//m_pObjectMgr->AddObject(CObjectMgr::EFFECT, pEffect1);
}

void CPlayer::Arrow()
{
	if (MotionSkip != true)
	{
		if (m_pKeyMgr->KeyDown(KEY_UP))
		{
			m_FsmPair.second = ArrowDOWN;
		}
		if (m_pKeyMgr->KeyDown(KEY_DOWN))
		{
			m_FsmPair.second = ArrowUP;
		}
		if (m_pKeyMgr->KeyDown(KEY_RIGHT))
		{
			m_FsmPair.second = ArrowRIGHT;
		}
		if (m_pKeyMgr->KeyDown(KEY_LEFT))
		{
			m_FsmPair.second = ArrowLEFT;
		}
	}
}

void CPlayer::BeHaviorFsm(eBehavior _enum, int _FrameMax)
{
	m_FsmPair.first = _enum;
	//m_tFrame.fStartFrame = 0.f;
	m_tFrame.fMaxFrameCnt = _FrameMax;

}

void CPlayer::ArrowMove(FsmPair _m_FsmPair)
{
	switch (_m_FsmPair.first)
	{
	case ArrowUP:
		m_tInfo.vPos += { 1,0,0 };
		break;
	case ArrowDOWN:
		m_tInfo.vPos += { -1,0,0 };
		break;
	case ArrowRIGHT:
		m_tInfo.vPos += { 0,-1,0 };
		break;
	case ArrowLEFT:
		m_tInfo.vPos += { 0, 1,0 };
		break;
	}
}

