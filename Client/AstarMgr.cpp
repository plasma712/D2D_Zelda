#include "stdafx.h"
#include "AstarMgr.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
	: m_pTerrain(nullptr), m_iStartIndex(0)
{
}


CAstarMgr::~CAstarMgr()
{
}

list<TILE_INFO*>& CAstarMgr::GetBestPath()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_BestPath;
}

void CAstarMgr::StartAstar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal)
{
	m_pTerrain = dynamic_cast<CTerrain*>(CObjectMgr::GetInstance()->GetTerrain());
	NULL_CHECK(m_pTerrain);

	m_OpenLst.clear();
	m_CloseLst.clear();

#ifdef _DEBUG
	for (auto& pTile : m_BestPath)
		pTile->byDrawID = 2;
#endif
	m_BestPath.clear();

	m_iStartIndex = m_pTerrain->GetTileIndex(vStart);
	int iGoalIndex = m_pTerrain->GetTileIndex(vGoal);

	if (-1 == m_iStartIndex || -1 == iGoalIndex)
		return;

	if (m_iStartIndex == iGoalIndex)
		return;

	if (1 == m_pTerrain->GetVecTile()[iGoalIndex]->byOption)
		return;

	if (true == PathFinding(m_iStartIndex, iGoalIndex))
		MakePath(m_iStartIndex, iGoalIndex);
}

bool CAstarMgr::PathFinding(int iStartIndex, int iGoalIndex)
{
	const vector<TILE_INFO*>& vecTile = m_pTerrain->GetVecTile();
	const vector<list<TILE_INFO*>>& vecGraph = m_pTerrain->GetVecGraph();

	if(!m_OpenLst.empty())
		m_OpenLst.pop_front();

	m_CloseLst.push_back(iStartIndex);

	// 너비탐색
	for (auto& pTile : vecGraph[iStartIndex])
	{
		if (iGoalIndex == pTile->iMyIndex)
		{
			pTile->iParentIndex = iStartIndex;
			return true;
		}

		if (!IsInOpenlist(pTile->iMyIndex) && !IsInCloselist(pTile->iMyIndex))
		{
			pTile->iParentIndex = iStartIndex;
			m_OpenLst.push_back(pTile->iMyIndex);
		}		
	}

	// 더이상 갈 길이 없을 때
	if (m_OpenLst.empty())
		return false;

	// 휴리스틱 비용 조사
	// 휴리스틱 = S + G
	// S: 최초 출발지와 현재 노드의 거리
	// G: 도착지와 현재 노드의 거리
	int iOriginIndex = m_iStartIndex;

	m_OpenLst.sort(
		[&vecTile, &iOriginIndex, &iGoalIndex](int iPreIndex, int iNextIndex)
	{
		D3DXVECTOR3 v1 = vecTile[iOriginIndex]->vPos - vecTile[iPreIndex]->vPos;
		D3DXVECTOR3 v2 = vecTile[iGoalIndex]->vPos - vecTile[iPreIndex]->vPos;
		float fHeuristic1 = D3DXVec3Length(&v1) + D3DXVec3Length(&v2);

		v1 = vecTile[iOriginIndex]->vPos - vecTile[iNextIndex]->vPos;
		v2 = vecTile[iGoalIndex]->vPos - vecTile[iNextIndex]->vPos;
		float fHeuristic2 = D3DXVec3Length(&v1) + D3DXVec3Length(&v2);

		return fHeuristic1 < fHeuristic2; // 오름차순
	});	

	return PathFinding(m_OpenLst.front(), iGoalIndex);
}

void CAstarMgr::MakePath(int iStartIndex, int iGoalIndex)
{
	const vector<TILE_INFO*>& vecTile = m_pTerrain->GetVecTile();

	m_BestPath.push_front(vecTile[iGoalIndex]);
	int iParentIndex = vecTile[iGoalIndex]->iParentIndex;

	while (true)
	{
		if (iStartIndex == iParentIndex)
			break;

		m_BestPath.push_front(vecTile[iParentIndex]);
		iParentIndex = vecTile[iParentIndex]->iParentIndex;
	}

#ifdef _DEBUG
	for (auto& pTile : m_BestPath)
		pTile->byDrawID = 36;
#endif
}

bool CAstarMgr::IsInOpenlist(int iIndex)
{
	// <algorithm>
	auto iter_find = find(m_OpenLst.begin(), m_OpenLst.end(), iIndex);

	if (m_OpenLst.end() == iter_find)
		return false;

	return true;
}

bool CAstarMgr::IsInCloselist(int iIndex)
{
	// <algorithm>
	auto iter_find = find(m_CloseLst.begin(), m_CloseLst.end(), iIndex);

	if (m_CloseLst.end() == iter_find)
		return false;

	return true;
}
