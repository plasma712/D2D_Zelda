#pragma once
#include "EffectIMP.h"
class CAnimateIMP :
	public CEffectIMP
{
private:
	CAnimateIMP();

public:
	virtual ~CAnimateIMP();

public:
	void SetFrame(const FRAME& frame);
	void SetAniMationSpeed(float _fAniMationSpeedControl);
public:
	// CEffectIMP을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const D3DXMATRIX* pWorld) override;

public:
	static CAnimateIMP* Create(const FRAME& frame, const wstring& wstrStateKey);
	static CAnimateIMP* Create(const FRAME& frame, const wstring& wstrObjectKey, const wstring& wstrStateKey, float _fAniMationSpeedControl);

	float fAniMationSpeedControl;

private:
	FRAME	m_tFrame;
	
};

