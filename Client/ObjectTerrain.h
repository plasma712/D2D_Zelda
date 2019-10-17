#pragma once
#include "GameObject.h"
class ObjectTerrain :
	public CGameObject
{
public:
	ObjectTerrain();
	~ObjectTerrain();

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
	static ObjectTerrain* Create(int _StageNumbering);

public:
	vector<TILE_INFO*>			m_vecTile;
	vector<list<TILE_INFO*>>	m_vecGraph;

	vector<ObjectTerrain*>			m_vecTile2;
	///////////////////////////////////////
	int StageNumbering = Stage01;
	HRESULT StageSelect(int _StageNumbering);

	void GetObjectKey(int _StageNumbering);
	void GetStateKey(int _StageNumbering);

	wstring ObjectKey;
	wstring StateKey;


};

