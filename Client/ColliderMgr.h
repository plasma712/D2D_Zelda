#pragma once
class CGameObject;
class CTerrain;
class CPlayer;

class CColliderMgr
{
	DECLARE_SINGLETON(CColliderMgr)
public:
	CColliderMgr();
	~CColliderMgr();
	//vector<CTerrain*>			m_vecTile;

	//TerrainPair TileCollision(vector<CTerrain*>* vtile, CPlayer* player);
	static void CollisionRect(const CObjectMgr& dstLst, const CObjectMgr& srcLst);
	CObjectMgr*		m_pObjectMgr;
};

