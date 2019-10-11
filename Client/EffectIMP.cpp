#include "stdafx.h"
#include "EffectIMP.h"


CEffectIMP::CEffectIMP()
	: m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_wstrStateKey(L""),
	m_wstrObjectKey(L"")

{
}


CEffectIMP::~CEffectIMP()
{
}

void CEffectIMP::SetStateKey(const wstring& wstrStateKey)
{
	m_wstrStateKey = wstrStateKey;
}

void CEffectIMP::SetObjectStateKey(const wstring & wstrObjectKey, const wstring & wstrStateKey)
{
	m_wstrObjectKey = wstrObjectKey;
	m_wstrStateKey = wstrStateKey;
}
