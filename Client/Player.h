#pragma once
#include "GameObject.h"

class CAstarMgr;
class CPlayer :
	public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

private:
	void MoveFrame();
	void KeyInput();
	void MovePath();

public:
	static CPlayer* Create();

private:
	INFO	m_tInfo;
	FRAME	m_tFrame;

	CAstarMgr*	m_pAstar;

public:
	float m_fSpeedX;
	float m_fSpeedY;

	static const D3DXVECTOR3& GetPlayerPos();
	static D3DXVECTOR3 m_PlayerPos;

	bool CollCheck;
	TerrainPair m_collpair;
	void SetColData(TerrainPair _cp);
	void Physics();


///////////////////
public:
	
	const TEX_INFO* pTexInfo;
	void AnimationPicture();
	FsmPair m_FsmPair;
	FsmPair FSM();
	void FSMANI(FsmPair _m_FsmPair);

	const TEX_INFO* TextureCall
	(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		int iIndex
	);

	wstring m_wstrObjectKey;
	wstring m_wstrStateKey;
///////////////////////////////////////////////////

	void BeIdle();
	void BeWalk();
	void BeAttack();
	void BeAttackEffect();

	// ����Ʈ ����.

///////////////////////////////////////////////////
//m_FsmPair ���� �Լ�.
	void Arrow();
	void BeHaviorFsm(eBehavior _enum,int _FrameMax);
	void ArrowMove(FsmPair _m_FsmPair);
#pragma region �߿�������
	bool PushPullCheck = false;
	bool NotBeHavior = false;
	bool AttackMotion = false;
	bool MotionSkip = false;

	bool FrameMotionCheck = false;
	float fAniMationSpeedControl;
	//float TimeCheck = 0.f;

#pragma endregion

};

