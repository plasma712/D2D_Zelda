#pragma once
#include "GameObject.h"
class CPlayer;

class CPlayerEffect :
	public CGameObject
{
public:
	CPlayerEffect();
	~CPlayerEffect();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	INFO	m_tInfo;
	FRAME	m_tFrame;


////////////////////////////////////////
	static CPlayerEffect* Create();

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

