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

	//벡터 타일, 플레이어, 충돌아이디

	TerrainPair TileCollision(vector<CTerrain*>* vtile, CPlayer * player);

	CollPair TileCollision2(vector<CTile*>* vtile, CPlayer * player);

};

