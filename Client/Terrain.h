#pragma once
#include "GameObject.h"

class CColliderMgr;
class CPlayer;

class CTerrain :
	public CGameObject
{
private:
	CTerrain();

public:
	virtual ~CTerrain();

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);
	const vector<TILE_INFO*>& GetVecTile() const;
	const vector<list<TILE_INFO*>>& GetVecGraph() const;

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	void TileChange(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption = 0);

private:
	bool IsPicking(const D3DXVECTOR3& vPos, size_t iIndex);
	HRESULT LoadTile(const TCHAR* pFilePath);
	void CreateGraph();

public:
	static CTerrain* Create();

private:
	vector<TILE_INFO*>			m_vecTile;
	vector<list<TILE_INFO*>>	m_vecGraph;	

	vector<CTerrain*>			m_vecTile2;
///////////////////////////////////////

public:
	void SetPlayerPointer(CPlayer * _player);

	void Collider(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption);
	CColliderMgr * CollMgr;
	CPlayer * m_pPlayer;

};

