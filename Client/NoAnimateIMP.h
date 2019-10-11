#pragma once
#include "EffectIMP.h"
class CNoAnimateIMP :
	public CEffectIMP
{
public:
	CNoAnimateIMP();
	virtual ~CNoAnimateIMP();

public:
	void SetLifeTime(float fTime);

public:
	// CEffectIMP을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX * pWorld) override;

public:
	static CNoAnimateIMP* Create(float fLifeTime, const wstring& wstrStateKey);

private:
	float m_fLifeTime;
	float m_fTimeCount;
};

