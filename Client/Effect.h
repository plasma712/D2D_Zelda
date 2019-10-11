#pragma once
#include "GameObject.h"

// Ãß»óÃþ

class CEffectIMP;
class CEffect :
	public CGameObject
{
protected:
	CEffect();
	CEffect(CEffectIMP* pImp);

public:
	virtual ~CEffect();

public:
	void SetPos(const D3DXVECTOR3& vPos);
	void SetScale(const D3DXVECTOR3& vScale);

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual HRESULT LateInit() PURE;
	virtual void Release();

protected:
	CEffectIMP*	m_pImp;
	INFO		m_tInfo;
};

