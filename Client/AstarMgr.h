#pragma once

class CTerrain;
class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

public:
	list<TILE_INFO*>& GetBestPath();

public:
	void StartAstar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);

private:
	bool PathFinding(int iStartIndex, int iGoalIndex);
	void MakePath(int iStartIndex, int iGoalIndex);
	bool IsInOpenlist(int iIndex);
	bool IsInCloselist(int iIndex);

private:
	list<int>			m_OpenLst;
	list<int>			m_CloseLst;
	list<TILE_INFO*>	m_BestPath;

	CTerrain*			m_pTerrain;
	int					m_iStartIndex;
};

