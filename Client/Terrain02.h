#pragma once
#include "GameObject.h"
class CTerrain02 :
	public CGameObject
{
public:
	CTerrain02();
	~CTerrain02();

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);
	const vector<TILE_INFO*>& GetVecTile() const;
	const vector<list<TILE_INFO*>>& GetVecGraph() const;

public:
	// CGameObject��(��) ���� ��ӵ�
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

public:
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

