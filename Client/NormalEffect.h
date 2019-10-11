#pragma once
#include "Effect.h"
class CNormalEffect :
	public CEffect
{
private:
	CNormalEffect();
	CNormalEffect(CEffectIMP* pImp);

public:
	virtual ~CNormalEffect();

public:
	// CEffect을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	static CNormalEffect* Create(CEffectIMP* pImp, const D3DXVECTOR3& vPos, const D3DXVECTOR3& Scale);
};

