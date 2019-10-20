#include "stdafx.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "AstarMgr.h"
#include "Mouse.h"
#include "Terrain.h"
#include "PlayerEffect.h"
#include "ObjectTerrain.h"

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
	MoveFrame();
	//MovePath();
	//Physics();
	FSMANI(m_FsmPair);
	PlayerCollider();

	KeyInput();


	return NO_EVENT;
}

void CPlayer::LateUpdate()
{

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x -CScrollMgr::GetScrollPos().x,
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
	//Temp();
	//m_tInfo.vPos = { 430.f, 300.f, 0.f };

	m_tInfo.vPos = { 1600.f, 1500.f, 0.f };
	CScrollMgr::SetScrollPos(m_tInfo.vPos * 0.7);
	m_tInfo.vSize = { 2.5f, 2.5f, 0.f };

	m_tFrame.fStartFrame = 0.f;
	m_tFrame.fMaxFrameCnt = 4.f; // 계속 FSM함수에서 바꿔야함... 흠...
	m_fSpeed = 1.f;

	m_FsmPair.first = BehaviorIdle;
	m_FsmPair.second = ArrowDOWN;
	m_wstrObjectKey = L"Idle";
	m_wstrStateKey = L"Front";

	fAniMationSpeedControl = 1.f;

	/////
	IceBlockTempPair.first = BehaviorIdle;
	IceBlockTempPair.second = ArrowDOWN;

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
	//cout << "IceBlockCheck : " << IceBlockCheck << endl;
	// 임시 Rect 충돌보려고
	RECT ColTemp;

	D3DXVECTOR2 vPoint[5] = {};


	ColTemp = RectPlayer(&this->m_tInfo);
	vPoint[0] = { (float)ColTemp.left, (float)ColTemp.top };
	vPoint[1] = { (float)ColTemp.right,(float)ColTemp.top };
	vPoint[2] = { (float)ColTemp.right,(float)ColTemp.bottom };
	vPoint[3] = { (float)ColTemp.left, (float)ColTemp.bottom };
	vPoint[4] = { (float)ColTemp.left, (float)ColTemp.top };
	//

	//pTexInfo = m_pTextureMgr->GetTexInfo(L"Idle", L"Front", (int)m_tFrame.fStartFrame);
	pTexInfo = TextureCall(m_wstrObjectKey, m_wstrStateKey, (int)m_tFrame.fStartFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(TRANSPARENCY, 255, 255, 255));

	m_pDeviceMgr->GetLine()->SetWidth(10.f);
	m_pDeviceMgr->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 0, 255, 0));

}

FsmPair CPlayer::FSM()
{
	static float TimeCheck = 0.f;
	Arrow();
	/////////////////////////////////////////////////////////////////
	if (m_pKeyMgr->KeyPressing(KEY_UP) || m_pKeyMgr->KeyPressing(KEY_DOWN)
		|| m_pKeyMgr->KeyPressing(KEY_RIGHT) || m_pKeyMgr->KeyPressing(KEY_LEFT))
	{
		if (PullPushRunCheck == false && PullPushCheck == false)
		{
			TimeCheck += m_pTimeMgr->GetDeltaTime();
			if (TimeCheck > 0.01f)
			{
				BeHaviorFsm(BehaviorWalk, 6);
				ArrowMove(m_FsmPair);
			}
		}
		if (PullPushRunCheck == true && PullPushCheck == true)
		{
			TimeCheck += m_pTimeMgr->GetDeltaTime();
			if (TimeCheck > 0.01f)
			{
				if (m_PullPushFsmPair.second == m_FsmPair.second)
				{
					BeHaviorFsm(BehaviorPush, 9);
					ArrowMove(m_FsmPair);
				}
				else
				{
					BeHaviorFsm(BehaviorPull, 9);
					ArrowMove(m_FsmPair);
				}
			}
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
		fAniMationSpeedControl = 1.75f;
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
		fAniMationSpeedControl = 1.f;
	}


	if (m_pKeyMgr->KeyPressing(KEY_D))
	{
		bAction = true;
		vObjectActionPlug(&this->m_tInfo, m_FsmPair);
	}

	if (m_pKeyMgr->KeyUp(KEY_D))
	{
		bAction = false;
		PullPushCheck = false;
		PullPushRunCheck = false;
		BeHaviorFsm(BehaviorIdle, 4);
		TimeCheck = 0.f;
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
	case  BehaviorPull:
		m_wstrObjectKey = L"Pull";
		break;
	case BehaviorPush:
		m_wstrObjectKey = L"Push";
		break;
	case BehaviorIceSlide:
		m_wstrObjectKey = L"ICE_Slide";
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
		TempInfo.vPos = { 0,60.f,0 };
		break;
	case ArrowDOWN:
		TempInfo.vPos = { 0,-60.f,0 };
		break;
	case ArrowRIGHT:
		TempInfo.vPos = { 60.f,0,0 };
		break;
	case ArrowLEFT:
		TempInfo.vPos = { -60.f,0,0 };
		break;
	default:
		break;
	}

	CNormalEffect* pEffect = CNormalEffect::Create(pImp, (this->m_tInfo.vPos + TempInfo.vPos + CScrollMgr::GetScrollPos()), { 2.0f,2.0f,2.0f });
	//CBuffEffect* pEffect = CBuffEffect::Create(pImp);		
	m_pObjectMgr->AddObject(EFFECT, pEffect);

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
	
	if (MotionSkip != true && PullPushRunCheck == false)
	{
		if (bNomalWallCheck == true || WallCol == true)
		{
			if (m_pKeyMgr->KeyDown(KEY_UP))
			{
				m_FsmPair.second = ArrowDOWN;
				WallCol = false;
			}
			if (m_pKeyMgr->KeyDown(KEY_DOWN))
			{
				m_FsmPair.second = ArrowUP;
				WallCol = false;
			}
			if (m_pKeyMgr->KeyDown(KEY_RIGHT))
			{
				m_FsmPair.second = ArrowRIGHT;
				WallCol = false;
			}
			if (m_pKeyMgr->KeyDown(KEY_LEFT))
			{
				m_FsmPair.second = ArrowLEFT;
				WallCol = false;
			}
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
	if (MotionSkip != true && PullPushRunCheck == false)
	{
		if (bNomalWallCheck == true)
		{
			switch (_m_FsmPair.second)
			{
			case ArrowUP:
				m_tInfo.vPos += { 0, m_fSpeed, 0 };
				CScrollMgr::SetScrollPos(D3DXVECTOR3(0, m_fSpeed, 0));
				break;
			case ArrowDOWN:
				m_tInfo.vPos += { 0, -m_fSpeed, 0 };
				CScrollMgr::SetScrollPos(D3DXVECTOR3(0, -m_fSpeed, 0));
				break;
			case ArrowRIGHT:
				m_tInfo.vPos += { m_fSpeed, 0, 0 };
				CScrollMgr::SetScrollPos(D3DXVECTOR3(m_fSpeed, 0, 0));
				break;
			case ArrowLEFT:
				m_tInfo.vPos += { -m_fSpeed, 0, 0 };
				CScrollMgr::SetScrollPos(D3DXVECTOR3(-m_fSpeed, 0, 0));
				break;
			}
		}

	}
}

FsmPair CPlayer::PullPushArrow(FsmPair _m_FsmPair, TILE_INFO * _vecTile)
{
	m_PullPushFsmPair.first = BehaviorIdle;

	if (PullPushCheck == true)
	{
		if (m_pKeyMgr->KeyPressing(KEY_UP))
		{
			m_PullPushFsmPair.second = ArrowDOWN;
			PullPushRunCheck = true;
			_vecTile->vPos += PullPushArrowMove(_m_FsmPair);
		}
		if (m_pKeyMgr->KeyPressing(KEY_DOWN))
		{
			m_PullPushFsmPair.second = ArrowUP;
			PullPushRunCheck = true;
			_vecTile->vPos += PullPushArrowMove(_m_FsmPair);
		}
		if (m_pKeyMgr->KeyPressing(KEY_RIGHT))
		{
			m_PullPushFsmPair.second = ArrowRIGHT;
			PullPushRunCheck = true;
			_vecTile->vPos += PullPushArrowMove(_m_FsmPair);
		}
		if (m_pKeyMgr->KeyPressing(KEY_LEFT))
		{
			m_PullPushFsmPair.second = ArrowLEFT;
			PullPushRunCheck = true;
			_vecTile->vPos += PullPushArrowMove(_m_FsmPair);
		}


		if (m_pKeyMgr->KeyUp(KEY_UP))
		{
			m_PullPushFsmPair.second = ArrowDOWN;
			PullPushRunCheck = false;
		}
		if (m_pKeyMgr->KeyUp(KEY_DOWN))
		{
			m_PullPushFsmPair.second = ArrowUP;
			PullPushRunCheck = false;
		}
		if (m_pKeyMgr->KeyUp(KEY_RIGHT))
		{
			m_PullPushFsmPair.second = ArrowRIGHT;
			PullPushRunCheck = false;
		}
		if (m_pKeyMgr->KeyUp(KEY_LEFT))
		{
			m_PullPushFsmPair.second = ArrowLEFT;
			PullPushRunCheck = false;
		}

	}
	return m_PullPushFsmPair;
}

D3DXVECTOR3 CPlayer::PullPushArrowMove(FsmPair _m_FsmPair)
{
	D3DXVECTOR3 vPos = {};
	if (PullPushCheck == true)
	{
		if ((m_PullPushFsmPair.second == _m_FsmPair.second) && PullPushRunCheck == true)// Pull
		{
			switch (_m_FsmPair.second)
			{
			case ArrowUP:
				_m_FsmPair.first = BehaviorPull;
				vPos = { 0, m_fSpeed, 0 };
				m_tInfo.vPos += { 0, m_fSpeed, 0 };
				break;
			case ArrowDOWN:
				_m_FsmPair.first = BehaviorPull;
				vPos = { 0, -m_fSpeed, 0 };
				m_tInfo.vPos += { 0, -m_fSpeed, 0 };
				break;
			case ArrowRIGHT:
				_m_FsmPair.first = BehaviorPull;
				vPos = { m_fSpeed,0, 0 };
				m_tInfo.vPos += { m_fSpeed, 0, 0 };
				break;
			case ArrowLEFT:
				_m_FsmPair.first = BehaviorPull;
				vPos = { -m_fSpeed,0, 0 };
				m_tInfo.vPos += { -m_fSpeed, 0, 0 };
				break;
			}
			return vPos;
		}
		else // push
		{
			switch (_m_FsmPair.second)
			{
			case ArrowUP:
				_m_FsmPair.first = BehaviorPush;
				vPos = { 0, -m_fSpeed, 0 };
				m_tInfo.vPos += { 0, -m_fSpeed, 0 };
				break;
			case ArrowDOWN:
				_m_FsmPair.first = BehaviorPush;
				vPos = { 0, m_fSpeed, 0 };
				m_tInfo.vPos += { 0, m_fSpeed, 0 };
				break;
			case ArrowRIGHT:
				_m_FsmPair.first = BehaviorPush;
				vPos = { -m_fSpeed,0, 0 };
				m_tInfo.vPos += {-m_fSpeed, 0, 0 };
				break;
			case ArrowLEFT:
				_m_FsmPair.first = BehaviorPush;
				vPos = { m_fSpeed,0, 0 };
				m_tInfo.vPos += { m_fSpeed, 0, 0 };
				break;
			}
			return vPos;
		}
	}
}

void CPlayer::Temp()
{

	m_pTerrain = dynamic_cast<CTerrain*>(CObjectMgr::GetInstance()->GetTerrain());

	const vector<TILE_INFO*>& vecTile = m_pTerrain->GetVecTile();

	for (int k = 0; k < vecTile.size(); k++)
	{
		if (vecTile[k]->byOption == 1)
		{
			cout << "K :" << k << endl;
			cout << "byDrawID :" << vecTile[k]->byDrawID << endl;
			cout << "byOption :" << vecTile[k]->byOption << endl;
			cout << "iMyIndex :" << vecTile[k]->iMyIndex << endl;
			cout << "iParentIndex :" << vecTile[k]->iParentIndex << endl;
			cout << "vPos		:" << vecTile[k]->vPos << endl;

			RECT ColTemp = TerrainGet(vecTile[k]);
			cout << endl;
			cout << "Left: " << ColTemp.left << endl;
			cout << "Right: " << ColTemp.right << endl;
			cout << "Up: " << ColTemp.top << endl;
			cout << "Bottom: " << ColTemp.bottom << endl;
			cout << "--------------------------------------------------" << endl;
			cout << "--------------------------------------------------" << endl;
		}
	}


}

void CPlayer::PlayerCollider()
{
	m_pTerrain = dynamic_cast<CTerrain*>(CObjectMgr::GetInstance()->GetTerrain());
	const vector<TILE_INFO*>& vecTile = m_pTerrain->GetVecTile();
	RECT ColTemp;


	m_pObjectTerrain = dynamic_cast<ObjectTerrain*>(CObjectMgr::GetInstance()->GetObjectTerrain());
	const vector<TILE_INFO*>& ObjectvecTile = m_pObjectTerrain->GetVecTile();
	RECT ColObjectTemp;

	RectImotalPlayer(&this->m_tInfo, m_FsmPair);

	//기본 맵 충돌 타일.
	for (int k = 0; k < vecTile.size(); k++)
	{
		if (vecTile[k]->byOption == IMMORTALWALL)
		{
			ColTemp = TerrainGet(vecTile[k]);
			//cout << "가나다라" << endl;
			PlayerCollider2(RectPlayer(&this->m_tInfo), ColTemp);

			TempCrushCheck(BlockCheckWall, ColTemp);
		}

		if (vecTile[k]->byOption == ICEBLOCK)
		{
			ColTemp = TerrainGet(vecTile[k]);
			//PlayerCollider2(RectPlayer(&this->m_tInfo), ColTemp);
			IceBlockAction(RectPlayer(&this->m_tInfo), ColTemp);

		}

		if (/*IceBlockCheck==true &&*/ vecTile[k]->byOption == NormalWall)
		{
			ColTemp = TerrainGet(vecTile[k]);
			NormalWallCol(RectPlayer(&this->m_tInfo), ColTemp);
		}


	}

	// 움직이는 오브젝트쪽 관련
	for (int k = 0; k < ObjectvecTile.size(); k++)
	{
		if (ObjectvecTile[k]->byOption == MOVEOBJECT)
		{
			ColObjectTemp = TerrainGet(ObjectvecTile[k]);
			PlayerCollider2(RectPlayer(&this->m_tInfo), ColObjectTemp);
		}
		if (ObjectvecTile[k]->byOption == MOVEOBJECT && bAction == true)
		{
			ColObjectTemp = TerrainGet(ObjectvecTile[k]);
			ObjectActionPlugCollider(ObjectActionPlug, ColObjectTemp, ObjectvecTile[k]);
		}
	}

}

RECT CPlayer::TerrainGet(TILE_INFO* _vecTile)
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

void CPlayer::PlayerCollider2(RECT _Player, RECT _Tile)
{
	RECT rc = {};
	if (IntersectRect(&rc, &(_Player), &(_Tile)))
	{
		ColTerPlayer(m_FsmPair, IceBlockTempPair);


	}
	else
	{
		//Crush = false;
		//IceBlockCheck = false;
	}
}

RECT CPlayer::RectPlayer(INFO * m_tInfo)
{
	RECT rc =
	{
		m_tInfo->vPos.x - TILECX / 2 - 12 - CScrollMgr::GetScrollPos().x,
		m_tInfo->vPos.y - TILECY / 2 - 8 - CScrollMgr::GetScrollPos().y,
		m_tInfo->vPos.x + TILECX / 2 + 12 - CScrollMgr::GetScrollPos().x,
		m_tInfo->vPos.y + TILECY / 2 + 20 - CScrollMgr::GetScrollPos().y
	};
	return rc;
}

void CPlayer::ColTerPlayer(FsmPair _m_FsmPair, FsmPair _IceBlockTempPair) // 이쪽고치기
{
	if (IceBlockCheck == false)
	{
		switch (_m_FsmPair.second)
		{
		case ArrowUP:
			m_tInfo.vPos += { 0, -m_fSpeed, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0, -m_fSpeed, 0));
			break;
		case ArrowDOWN:
			m_tInfo.vPos += { 0, m_fSpeed, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0, m_fSpeed, 0));
			break;
		case ArrowRIGHT:
			m_tInfo.vPos += { -m_fSpeed, 0, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(-m_fSpeed, 0, 0));
			break;
		case ArrowLEFT:
			m_tInfo.vPos += { m_fSpeed, 0, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(m_fSpeed, 0, 0));
			break;
		}
	}
	else
	{
		switch (_IceBlockTempPair.second)
		{
		case ArrowUP:
			m_tInfo.vPos += { 0, -m_fSpeed, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0, -m_fSpeed, 0));
			break;
		case ArrowDOWN:
			m_tInfo.vPos += { 0, m_fSpeed, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0, m_fSpeed, 0));
			break;
		case ArrowRIGHT:
			m_tInfo.vPos += { -m_fSpeed, 0, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(-m_fSpeed, 0, 0));
			break;
		case ArrowLEFT:
			m_tInfo.vPos += { m_fSpeed, 0, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(m_fSpeed, 0, 0));
			break;
		}

	}

}

void CPlayer::vActionPlug(INFO * m_tInfo, FsmPair _m_FsmPair)
{
	float fX = 0;
	float fY = 0;

	switch (_m_FsmPair.second)
	{
	case ArrowUP:
		fX = 0;
		fY = TILECY;
		break;
	case ArrowDOWN:
		fX = 0;
		fY = -TILECY;
		break;
	case ArrowRIGHT:
		fX = TILECX;
		fY = 0;
		break;
	case ArrowLEFT:
		fX = -TILECX;
		fY = 0;
		break;
	}

	RECT rc =
	{
		m_tInfo->vPos.x - TILECX / 2,//+ fX,
		m_tInfo->vPos.y - TILECY / 2,//+ fY,
		m_tInfo->vPos.x + TILECX / 2,//+ fX,
		m_tInfo->vPos.y + TILECY / 2//+ fY
	};




	ActionPlug = rc;
}

void CPlayer::ActionPlugCollider(RECT _ActionPlug, RECT _Tile, TILE_INFO* _vecTile)
{
	RECT rc = {};
	if (IntersectRect(&rc, &(_ActionPlug), &(_Tile)))
	{
		//cout << "충돌" << endl;
		_vecTile->vPos.x -= 0.1f;
		PullPushCheck = true;
	}
	else
	{
		PullPushCheck = false;
		//Crush = false;
	}

}

void CPlayer::vObjectActionPlug(INFO * m_tInfo, FsmPair _m_FsmPair)
{
	float fX = 0;
	float fY = 0;

	switch (_m_FsmPair.second)
	{
	case ArrowUP:
		fX = 0;
		fY = TILECY;
		break;
	case ArrowDOWN:
		fX = 0;
		fY = -TILECY;
		break;
	case ArrowRIGHT:
		fX = TILECX;
		fY = 0;
		break;
	case ArrowLEFT:
		fX = -TILECX;
		fY = 0;
		break;
	}

	RECT rc =
	{
		m_tInfo->vPos.x - TILECX / 2 + fX,
		m_tInfo->vPos.y - TILECY / 2 + fY,
		m_tInfo->vPos.x + TILECX / 2 + fX,
		m_tInfo->vPos.y + TILECY / 2 + fY
	};




	ObjectActionPlug = rc;

}

void CPlayer::ObjectActionPlugCollider(RECT _ActionPlug, RECT _Tile, TILE_INFO * _vecTile)
{
	RECT rc = {};
	if (IntersectRect(&rc, &(_ActionPlug), &(_Tile)))
	{
		PullPushCheck = true;
		PullPushArrow(m_FsmPair,_vecTile);
		//cout << PullPushCheck << endl;
		//_vecTile->vPos += PullPushArrowMove(m_FsmPair);
	}
	else
	{
		//PullPushCheck = false;
		//cout << PullPushCheck << endl;
	}
}

void CPlayer::IceBlockAction(RECT _Player, RECT _Tile)
{
	RECT rc = {};
	if (IntersectRect(&rc, &(_Player), &(_Tile)))
	{
		IceBlockCheck = true;
		bNomalWallCheck = false; // 확인
		IceBlockCol(m_FsmPair);
		m_FsmPair.first = BehaviorIceSlide;
		m_tFrame.fMaxFrameCnt = 2;
	}
	else
	{
		//IceBlockCheck = false;
		//Crush = false;
	}
}

void CPlayer::IceBlockCol(FsmPair _m_FsmPair)
{
	if (IceBlockCheck == true)
	{
		switch (_m_FsmPair.second)
		{
		case ArrowUP:
			m_tInfo.vPos += { 0, m_fSpeed, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0, m_fSpeed, 0));
			break;
		case ArrowDOWN:
			m_tInfo.vPos += { 0, -m_fSpeed, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(0, -m_fSpeed, 0));
			break;
		case ArrowRIGHT:
			m_tInfo.vPos += { m_fSpeed, 0, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(m_fSpeed, 0, 0));
			break;
		case ArrowLEFT:
			m_tInfo.vPos += { -m_fSpeed, 0, 0 };
			CScrollMgr::SetScrollPos(D3DXVECTOR3(-m_fSpeed, 0, 0));
			break;
		}
	}
	//cout << _m_FsmPair.second << endl;

}

void CPlayer::RectImotalPlayer(INFO * m_tInfo, FsmPair _m_FsmPair)
{
	//static int k = 0;
	//cout << k++ << endl;

	float fX = 0;
	float fY = 0;

	switch (_m_FsmPair.second)
	{
	case ArrowUP:
		fX = 0;
		fY = TILECY;
		break;
	case ArrowDOWN:
		fX = 0;
		fY = -TILECY;
		break;
	case ArrowRIGHT:
		fX = TILECX;
		fY = 0;
		break;
	case ArrowLEFT:
		fX = -TILECX;
		fY = 0;
		break;
	}

	RECT rc =
	{
		m_tInfo->vPos.x - TILECX / 2 + fX / 2,
		m_tInfo->vPos.y - TILECY / 2 + fY / 2,
		m_tInfo->vPos.x + TILECX / 2 + fX / 2,
		m_tInfo->vPos.y + TILECY / 2 + fY / 2
	};

	BlockCheckWall = rc;
}

void CPlayer::TempCrushCheck(RECT _BlockCheckWall, RECT _Tile)
{
	RECT rc = {};
	if (IntersectRect(&rc, &(_BlockCheckWall), &(_Tile)))
	{
		WallCol = true;
		IceBlockTempPair.second = m_FsmPair.second;
	}
}

void CPlayer::NormalWallCol(RECT _Player, RECT _Tile)
{
	RECT rc = {};
	if (IntersectRect(&rc, &(_Player), &(_Tile)))
	{
		bNomalWallCheck = true;		
	}
	else
	{
		//bNomalWallCheck = false;
	}
}



