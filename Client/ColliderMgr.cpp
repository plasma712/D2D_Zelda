#include "stdafx.h"
#include "ColliderMgr.h"
#include "GameObject.h"
#include "Terrain.h"
#include "TextureMgr.h"
#include "Player.h"

CColliderMgr::CColliderMgr()
{
}


CColliderMgr::~CColliderMgr()
{
}

TerrainPair CColliderMgr::TileCollision(vector<CTerrain*>* vtile, CPlayer * player)
{
	eColDir CollDir = ColDirNONE;
	vector<CTerrain*> ColObjvec;
	ColObjvec.reserve(50);
	vector<RECT>ColRectvec;
	ColRectvec.reserve(50);

	for (auto& obj : *vtile)
	{
		RECT objRect = obj->GetRect();
		objRect.left = obj->GetRect().left;
		RECT thisRect = player->GetRect();
		RECT tempRect;// = { 0,0,0,0 };

		if (IntersectRect(&tempRect, &player->GetRect(), &obj->GetRect()))	//this와 obj가 충돌했는지 판단.
		{

			SetRect(&tempRect, 0, 0
				, tempRect.right - tempRect.left
				, tempRect.bottom - tempRect.top);

			ColObjvec.push_back(obj);
			ColRectvec.push_back(tempRect);

			if (obj->GetTileInfo().byDrawID == 7 ||
				obj->GetTileInfo().byDrawID == 0)
			{

				ColObjvec.pop_back();
				ColRectvec.pop_back();
			}
		}



	}

	if (ColObjvec.size() != 0)
	{
		//cout << "ColObjvec.size() : " << ColObjvec.size() << endl;
		int count = 0;
		//좌우, 상하 판정 계산 for문
		for (size_t i = 0; i < ColObjvec.size(); i++)
		{
			//if (ColRectvec[i].right == ColRectvec[i].bottom)
			//	continue;
			if (ColRectvec[i].right < ColRectvec[i].bottom)
				count++;
			else
				count--;
		}

		if (count > 0)	//좌우충돌 판정!  
		{
			if (player->GetTileInfo().vPos.x < ColObjvec.back()->GetTileInfo().vPos.x)
			{
				player->GetTileInfo().vPos.x -= ColRectvec.back().right;
				CollDir = Right;
			}
			else
			{
				player->GetTileInfo().vPos.x += ColRectvec.back().right;
				CollDir = Left;
			}
			//cout << " X 충돌 판정 " << endl;
			//m_fSpeedX = 0;

		}
		else
		{
			if (player->GetTileInfo().vPos.y < ColObjvec.back()->GetTileInfo().vPos.y)
			{
				player->GetTileInfo().vPos.y -= ColRectvec.back().bottom;		//player가 더 위에 있을때는 위로 민다
				CollDir = Down;
			}
			else
			{
				CollDir = Up;
				player->GetTileInfo().vPos.y += ColRectvec.back().bottom;		//player가 더 아래 있을때는 아래로 민다/

			}
			//player-> = 0;
		}
	}
	TerrainPair temp = { ColDirNONE,nullptr };

	if (CollDir != ColDirNONE)
	{
		temp.first = CollDir;
		temp.second = ColObjvec.back();
	}

	return temp;
}
