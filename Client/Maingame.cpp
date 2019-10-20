#include "stdafx.h"
#include "Maingame.h"

CMaingame::CMaingame()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pFrameMgr(CFrameMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance())
{
}


CMaingame::~CMaingame()
{
	Release();
}

void CMaingame::Update()
{
	m_pTimeMgr->UpdateTime();
	m_pKeyMgr->Update();
	m_pSceneMgr->Update();
}

void CMaingame::LateUpdate()
{
	m_pSceneMgr->LateUpdate();
}

void CMaingame::Render()
{
	m_pDeviceMgr->Render_Begin();

	// 앞으로 모든 오브젝트는 이 사이에서 렌더링 수행.

	m_pSceneMgr->Render();
	m_pFrameMgr->RenderFPS();
	//RenderLine();

	m_pDeviceMgr->Render_End();
}

void CMaingame::RenderLine()
{
	// D3DXLINE은 스프라이트로 그리는게 아니기 때문에
	// 스프라이트의 Begin, End 외부에 그리는 코드가 존재해야함.
	m_pDeviceMgr->GetSprite()->End();

	D3DXVECTOR2 vPoint[4] = 
	{
		{ 400.f, 300.f },
		{ 500.f, 400.f },
		{ 300.f, 400.f },
		{ 400.f, 300.f }
	};

	m_pDeviceMgr->GetLine()->SetWidth(5.f); // 선의 굵기
	m_pDeviceMgr->GetLine()->Draw(vPoint, 4, D3DCOLOR_ARGB(255, 0, 255, 0));

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

HRESULT CMaingame::Initialize()
{
	HRESULT hr = 0;

	hr = m_pTimeMgr->InitTime();
	FAILED_CHECK_MSG_RETURN(hr, L"InitTime 실패", E_FAIL);

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG_RETURN(hr, L"장치초기화 실패", E_FAIL);

	hr = m_pSceneMgr->SceneChange(CSceneMgr::LOGO);
	FAILED_CHECK_MSG_RETURN(hr, L"SceneChange 실패", E_FAIL);

	return S_OK;
}

void CMaingame::Release()
{	
	m_pSceneMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	m_pFrameMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pKeyMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

CMaingame* CMaingame::Create()
{
	CMaingame* pInstance = new CMaingame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
