#pragma once
#include "GameObject.h"
class CTile :
	public CGameObject
{
public:
	CTile();
	~CTile();

	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;
};

