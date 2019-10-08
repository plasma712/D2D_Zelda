#pragma once
#include "GameObject.h"

class CToolView;
class CTerrain :
	public CGameObject
{
private:
	CTerrain();

public:
	virtual ~CTerrain();

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);

public:
	void SetView(CToolView* pView);

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
	void MiniRender();
	void SaveTile(const TCHAR* pFilePath);
	void LoadTile(const TCHAR* pFilePath);

private:
	bool IsPicking(const D3DXVECTOR3& vPos, size_t iIndex);

public:
	static CTerrain* Create(CToolView* pView);

private:
	vector<TILE_INFO*>	m_vecTile;
	CToolView*			m_pView;
public:

	bool CheckInOut(const POINT& pt, const int iIndex);


};

