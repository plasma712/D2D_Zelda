#include "stdafx.h"
#include "BuffEffect.h"
#include "EffectIMP.h"
#include "Player.h"

CBuffEffect::CBuffEffect()
	: m_fAngle(0.f)
{
}

CBuffEffect::CBuffEffect(CEffectIMP* pImp)
	: CEffect(pImp),
	m_fAngle(0.f)
{
}


CBuffEffect::~CBuffEffect()
{
	Release();
}

int CBuffEffect::Update()
{
	CGameObject::LateInit();

	D3DXMATRIX matScale, matRotZ, matTrans, matRevZ;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRevZ, D3DXToRadian(-m_fAngle));

	CPlayer* pParent = dynamic_cast<CPlayer*>(m_pObjectMgr->GetPlayer());
	NULL_CHECK_RETURN(pParent, DEAD_OBJ);

	m_tInfo.matWorld = matScale * matRotZ * matTrans * matRevZ * pParent->GetInfo().matWorld;

	m_fAngle += 360.f * m_pTimeMgr->GetDeltaTime();

	return m_pImp->Update();
}

void CBuffEffect::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CBuffEffect::Render()
{
	m_pImp->Render(&m_tInfo.matWorld);
}

HRESULT CBuffEffect::Initialize()
{
	m_tInfo.vPos = { 200.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	return S_OK;
}

HRESULT CBuffEffect::LateInit()
{
	return S_OK;
}

void CBuffEffect::Release()
{
}

CBuffEffect* CBuffEffect::Create(CEffectIMP* pImp)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CBuffEffect* pInstance = new CBuffEffect(pImp);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
