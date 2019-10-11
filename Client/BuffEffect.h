#pragma once
#include "Effect.h"
class CBuffEffect :
	public CEffect
{
private:
	CBuffEffect();
	CBuffEffect(CEffectIMP* pImp);

public:
	virtual ~CBuffEffect();

public:
	// CEffect��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	static CBuffEffect* Create(CEffectIMP* pImp);

private:
	float m_fAngle;
};

