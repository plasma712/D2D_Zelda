#pragma once

// ±¸ÇöÃþ

class CEffectIMP
{
protected:
	CEffectIMP();

public:
	virtual ~CEffectIMP();

public:
	void SetStateKey(const wstring& wstrStateKey);
	void SetObjectStateKey(const wstring& wstrObjectKey,const wstring& wstrStateKey);

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(const D3DXMATRIX* pWorld) PURE;

protected:
	CTimeMgr*		m_pTimeMgr;
	CTextureMgr*	m_pTextureMgr;
	CDeviceMgr*		m_pDeviceMgr;

	wstring			m_wstrStateKey;
	wstring			m_wstrObjectKey;

};

