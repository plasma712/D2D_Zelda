#include "stdafx.h"
#include "NormalEffect.h"
#include "EffectIMP.h"
#include "ScrollMgr.h"

CNormalEffect::CNormalEffect()
{
}

CNormalEffect::CNormalEffect(CEffectIMP* pImp)
	: CEffect(pImp)
{
}


CNormalEffect::~CNormalEffect()
{
	Release();
}

int CNormalEffect::Update()
{
	CGameObject::LateInit();

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x - CScrollMgr::GetScrollPos().x, 
		m_tInfo.vPos.y - CScrollMgr::GetScrollPos().y,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;

	return m_pImp->Update();
}

void CNormalEffect::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CNormalEffect::Render()
{
	m_pImp->Render(&m_tInfo.matWorld);
}

HRESULT CNormalEffect::Initialize()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	return S_OK;
}

HRESULT CNormalEffect::LateInit()
{
	return S_OK;
}

void CNormalEffect::Release()
{
}

CNormalEffect* CNormalEffect::Create(CEffectIMP* pImp, const D3DXVECTOR3& vPos, const D3DXVECTOR3& Scale)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CNormalEffect* pInstance = new CNormalEffect(pImp);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetPos(vPos);
	pInstance->SetScale(Scale);

	return pInstance;
}
