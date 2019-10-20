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

	// ������ ��� ������Ʈ�� �� ���̿��� ������ ����.

	m_pSceneMgr->Render();
	m_pFrameMgr->RenderFPS();
	//RenderLine();

	m_pDeviceMgr->Render_End();
}

void CMaingame::RenderLine()
{
	// D3DXLINE�� ��������Ʈ�� �׸��°� �ƴϱ� ������
	// ��������Ʈ�� Begin, End �ܺο� �׸��� �ڵ尡 �����ؾ���.
	m_pDeviceMgr->GetSprite()->End();

	D3DXVECTOR2 vPoint[4] = 
	{
		{ 400.f, 300.f },
		{ 500.f, 400.f },
		{ 300.f, 400.f },
		{ 400.f, 300.f }
	};

	m_pDeviceMgr->GetLine()->SetWidth(5.f); // ���� ����
	m_pDeviceMgr->GetLine()->Draw(vPoint, 4, D3DCOLOR_ARGB(255, 0, 255, 0));

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

HRESULT CMaingame::Initialize()
{
	HRESULT hr = 0;

	hr = m_pTimeMgr->InitTime();
	FAILED_CHECK_MSG_RETURN(hr, L"InitTime ����", E_FAIL);

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG_RETURN(hr, L"��ġ�ʱ�ȭ ����", E_FAIL);

	hr = m_pSceneMgr->SceneChange(CSceneMgr::LOGO);
	FAILED_CHECK_MSG_RETURN(hr, L"SceneChange ����", E_FAIL);

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
