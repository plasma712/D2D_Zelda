#pragma once

class CMaingame
{
private:
	CMaingame();

public:
	~CMaingame();

public:	
	void Update();
	void LateUpdate();
	void Render();
	void RenderLine();

private:
	HRESULT Initialize();
	void Release();

public:
	static CMaingame* Create();

private:
	CDeviceMgr*			m_pDeviceMgr;
	CTextureMgr*		m_pTextureMgr;
	CKeyMgr*			m_pKeyMgr;
	CTimeMgr*			m_pTimeMgr;
	CFrameMgr*			m_pFrameMgr;
	CObjectMgr*			m_pObjectMgr;
	CSceneMgr*			m_pSceneMgr;
};

