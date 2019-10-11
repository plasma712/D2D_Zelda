#include "stdafx.h"
#include "AnimateIMP.h"


CAnimateIMP::CAnimateIMP()
{
}


CAnimateIMP::~CAnimateIMP()
{
}

void CAnimateIMP::SetFrame(const FRAME& frame)
{
	m_tFrame = frame;
}

void CAnimateIMP::SetAniMationSpeed(float _fAniMationSpeedControl)
{
	fAniMationSpeedControl = _fAniMationSpeedControl;
}



int CAnimateIMP::Update()
{
	m_tFrame.fStartFrame += m_tFrame.fMaxFrameCnt * m_pTimeMgr->GetDeltaTime() * fAniMationSpeedControl;

	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrameCnt)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CAnimateIMP::LateUpdate()
{
}

void CAnimateIMP::Render(const D3DXMATRIX* pWorld)
{
	// 알파 블렌딩

	// 여기까지가 DESTBLEND이다.
	m_pDeviceMgr->GetSprite()->End(); 

	// 여기서부터 SCRBLEND 시작.
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	// pixel1 = D3DRS_SRCBLEND * D3DBLEND_SRCCOLOR
	// D3DRS_SRCBLEND: 이제 그려야할 이미지의 픽셀 (RGBA)
	// D3DBLEND_SRCCOLOR: D3DRS_SRCBLEND의 RGBA를 0~1범위로 환산한 값.
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);

	// pixel2 = D3DRS_DESTBLEND * D3DBLEND_INVSRCCOLOR
	// D3DRS_DESTBLEND: 이미 후면버퍼에 그려진 이미지의 픽셀 (RGBA)
	// D3DBLEND_INVSRCCOLOR: D3DBLEND_SRCCOLOR의 반전 값.
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	// pixel1 + pixel2
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_wstrObjectKey, m_wstrStateKey, (int)m_tFrame.fStartFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(pWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 여기까지 SRCBLEND 끝.
	m_pDeviceMgr->GetSprite()->End();

	// 이제부터 그려지는 스프라이트는 위의 알파블렌딩과 연관 없다.
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

CAnimateIMP* CAnimateIMP::Create(
	const FRAME& frame, const wstring& wstrStateKey)
{
	CAnimateIMP* pInstance = new CAnimateIMP;
	pInstance->SetFrame(frame);
	pInstance->SetStateKey(wstrStateKey);

	return pInstance;
}

CAnimateIMP * CAnimateIMP::Create(const FRAME & frame, const wstring & wstrObjectKey, const wstring & wstrStateKey, float _fAniMationSpeedControl)
{
	CAnimateIMP* pInstance = new CAnimateIMP;
	pInstance->SetFrame(frame);
	pInstance->SetObjectStateKey(wstrObjectKey,wstrStateKey);
	pInstance->SetAniMationSpeed(_fAniMationSpeedControl);

	return pInstance;
}
