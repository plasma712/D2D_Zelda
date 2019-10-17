#pragma once
#include "Scene.h"

class CStage02 :
	public CScene
{
public:
	CStage02();
	~CStage02();

	// CScene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CStage02* Create();
};

