#include "stdafx.h"
#include "NoAnimateIMP.h"


CNoAnimateIMP::CNoAnimateIMP()
	: m_fLifeTime(0.f), m_fTimeCount(0.f)
{
}


CNoAnimateIMP::~CNoAnimateIMP()
{
}

void CNoAnimateIMP::SetLifeTime(float fTime)
{
	m_fLifeTime = fTime;
}

int CNoAnimateIMP::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDeltaTime();

	if (m_fLifeTime < m_fTimeCount)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CNoAnimateIMP::LateUpdate()
{
}

void CNoAnimateIMP::Render(const D3DXMATRIX * pWorld)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Effect", m_wstrStateKey, 3);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(pWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));	
}

CNoAnimateIMP* CNoAnimateIMP::Create(float fLifeTime, const wstring& wstrStateKey)
{
	CNoAnimateIMP* pInstance = new CNoAnimateIMP;
	pInstance->SetLifeTime(fLifeTime);
	pInstance->SetStateKey(wstrStateKey);

	return pInstance;
}
