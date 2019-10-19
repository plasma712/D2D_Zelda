#pragma once
#include "GameObject.h"
class ObjectTerrain;
class CTerrain;
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
	/////
	FsmPair PullPushArrow(FsmPair _m_FsmPair, TILE_INFO * _vecTile);
	D3DXVECTOR3 PullPushArrowMove(FsmPair _m_FsmPair);
	bool PullPushCheck = false;
	bool PullPushRunCheck = false;
	FsmPair m_PullPushFsmPair;



#pragma region �߿�������
	bool PushPullCheck = false;
	bool NotBeHavior = false;
	bool AttackMotion = false;
	bool MotionSkip = false;

	bool FrameMotionCheck = false;
	float fAniMationSpeedControl;
	//float TimeCheck = 0.f;


	// Terrain �ʵ����� ������ �ü��ִ��� Ȯ������.


	void Temp();
	CTerrain*			m_pTerrain;


	void PlayerCollider();
	float m_fSpeed;

	// �������� Rect�� �����.

	RECT TerrainGet(TILE_INFO* _vecTile);
	void PlayerCollider2(RECT _Player, RECT _Tile);
	RECT RectPlayer(INFO* m_tInfo);

	//bool Crush = false;

	void ColTerPlayer(FsmPair _m_FsmPair , FsmPair _IceBlockTempPair);

#pragma endregion

	////////////////////////

#pragma region ���浹 
	RECT ActionPlug;
	bool bAction = false;
	void vActionPlug(INFO * m_tInfo, FsmPair _m_FsmPair);

	void ActionPlugCollider(RECT _ActionPlug, RECT _Tile, TILE_INFO* _vecTile);
#pragma  endregion

#pragma region �ʿ�����Ʈ�浹 
	ObjectTerrain* m_pObjectTerrain;
	RECT ObjectActionPlug;
	bool ObjectbAction = false;
	void vObjectActionPlug(INFO * m_tInfo, FsmPair _m_FsmPair);

	void ObjectActionPlugCollider(RECT _ActionPlug, RECT _Tile, TILE_INFO* _vecTile);
#pragma  endregion

	//���Ǳ� �浹.
	void IceBlockAction(RECT _Player, RECT _Tile);
	void IceBlockCol(FsmPair _m_FsmPair);
	bool IceBlockCheck = false; // ��������Ʈ�� �浹�� true�� �����ؼ� ice�浹�ȵǰ�
	void RectImotalPlayer(INFO * m_tInfo, FsmPair _m_FsmPair);//���� �浹�� �� bool�� �������ַ��� ����°�,,, 
	RECT BlockCheckWall;
	void TempCrushCheck(RECT _BlockCheckWall, RECT _Tile);
	bool WallCol = false;

	FsmPair IceBlockTempPair;

	//�Ϲݺ����浹.

	void NormalWallCol(RECT _Player, RECT _Tile);
	bool bNomalWallCheck = false;

};

