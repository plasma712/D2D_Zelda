#include "stdafx.h"
#include "Logo.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Update()
{
	if (m_pKeyMgr->KeyUp(KEY_RETURN))
	{
		m_pSceneMgr->SceneChange(CSceneMgr::STAGE);
		return;
	}
}

void CLogo::LateUpdate()
{
	
}

void CLogo::Render()
{
	
}

HRESULT CLogo::Initialize()
{
	return S_OK;
}

void CLogo::Release()
{
}

CLogo* CLogo::Create()
{
	CLogo* pInstance = new CLogo;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
