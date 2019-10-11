#pragma once
class CPlayer;
class CTerrain;
class CTile;
class CGameObject;

class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
public:
	CCollisionMgr();
	~CCollisionMgr();

	//���� Ÿ��, �÷��̾�, �浹���̵�

	TerrainPair TileCollision(vector<CTerrain*>* vtile, CPlayer * player);

	CollPair TileCollision2(vector<CTile*>* vtile, CPlayer * player);

};

