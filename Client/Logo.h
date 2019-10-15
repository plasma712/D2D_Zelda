#pragma once
#include "Scene.h"
class CLogo :
	public CScene
{
private:
	CLogo();

public:
	virtual ~CLogo();

public:
	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;


public:
	static CLogo* Create();


	INFO	m_tInfo;
	FRAME	m_tFrame;

	const TEX_INFO* TextureCall
	(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		int iIndex
	);

	const TEX_INFO* pTexInfo;
	wstring m_wstrObjectKey;
	wstring m_wstrStateKey;

	void SetTextureKey(const wstring& wstrObjectKey, const wstring& wstrStateKey, int _Max);


};

