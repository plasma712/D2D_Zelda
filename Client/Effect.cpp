#include "stdafx.h"
#include "Effect.h"
#include "EffectIMP.h"

CEffect::CEffect()
	: m_pImp(nullptr)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
}

CEffect::CEffect(CEffectIMP* pImp)
	: m_pImp(pImp)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
}


CEffect::~CEffect()
{
	Release();
}

void CEffect::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tInfo.vPos = vPos;
}

void CEffect::SetScale(const D3DXVECTOR3 & vScale)
{
	m_tInfo.vSize = vScale;

}

void CEffect::Release()
{
	SafeDelete(m_pImp);
}
