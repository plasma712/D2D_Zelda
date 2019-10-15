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


	D3DXMATRIX matScale, matWorld, matTrans;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"MainTitle");

	float fX = pTexInfo->tImgInfo.Width*0.5f;
	float fY = pTexInfo->tImgInfo.Height*0.5f;

	D3DXMatrixScaling(&matScale,
		3.5f,
		3.75f,
		0.f);

	D3DXMatrixTranslation(
		&matTrans,
		WINCX*0.5f,
		WINCY*0.5f,
		0.f);

	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(
		pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fX, fY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));


}

HRESULT CLogo::Initialize()
{	
	m_pTextureMgr->LoadTexture(
		m_pTextureMgr->SINGLE_TEXTURE,
		L"../Texture/Title/TemTitle.png",
		L"MainTitle");


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

const TEX_INFO * CLogo::TextureCall(const wstring & wstrObjectKey, const wstring & wstrStateKey, int iIndex)
{
	return nullptr;
}

void CLogo::SetTextureKey(const wstring & wstrObjectKey, const wstring & wstrStateKey, int _Max)
{
}
