#pragma once
class CGameObject
{
protected:
	CGameObject();

public:
	virtual ~CGameObject();

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual HRESULT LateInit();
	virtual void Release() PURE;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CKeyMgr*		m_pKeyMgr;
	CTimeMgr*		m_pTimeMgr;

	bool			m_bIsInit;	


public:
	const INFO& GetInfo();
	TILE_INFO & GetTileInfo();
	void UpdateRECT();
	RECT  GetRect();
	TILE_INFO		tileInfo;
	INFO			m_pInfo;
	RECT			m_tRECT;

};

