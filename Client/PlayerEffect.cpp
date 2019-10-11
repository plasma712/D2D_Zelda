#include "stdafx.h"
#include "PlayerEffect.h"
#include "Player.h"


CPlayerEffect::CPlayerEffect()
{
}


CPlayerEffect::~CPlayerEffect()
{
}

int CPlayerEffect::Update()
{
	m_tFrame.fStartFrame += m_tFrame.fMaxFrameCnt * m_pTimeMgr->GetDeltaTime();

	//if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrameCnt)
	//	//SafeDelete(this);
	//{

	//}
	return NO_EVENT;
}

void CPlayerEffect::LateUpdate()
{
}

void CPlayerEffect::Render()
{
	pTexInfo = TextureCall(m_wstrObjectKey, m_wstrStateKey, (int)m_tFrame.fStartFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CPlayerEffect::Initialize()
{
	m_tInfo.vSize = { 5.f, 5.f, 0.f };
	m_tInfo.vPos = CPlayer::m_PlayerPos;
	m_wstrObjectKey = L"A";
	m_wstrStateKey = L"Front";

	return E_NOTIMPL;
}

void CPlayerEffect::Release()
{
}

const TEX_INFO * CPlayerEffect::TextureCall(const wstring & wstrObjectKey, const wstring & wstrStateKey, int iIndex)
{
	return m_pTextureMgr->GetTexInfo(wstrObjectKey, wstrStateKey, (int)m_tFrame.fStartFrame);
}

void CPlayerEffect::SetTextureKey(const wstring & wstrObjectKey, const wstring & wstrStateKey, int _Max)
{
	m_wstrObjectKey = wstrObjectKey;
	m_wstrStateKey = wstrStateKey;
	m_tFrame.fMaxFrameCnt = _Max;
}

CPlayerEffect * CPlayerEffect::Create()
{
	CPlayerEffect* pInstance = new CPlayerEffect;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
