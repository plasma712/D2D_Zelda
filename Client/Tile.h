#pragma once
#include "GameObject.h"
class CTile :
	public CGameObject
{
public:
	CTile();
	~CTile();

	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;
};

