#pragma once

#ifndef __STRUCT_H__

typedef struct tagPathInfo
{
	wstring wstrObjectKey = L"";
	wstring wstrStateKey = L"";
	wstring wstrImgPath = L"";
	int iImgCount = 0;
}PATH_INFO;

typedef struct tagUnit
{
#ifdef TOOL
	CString strName = L"";
#elif CLIENT
	wstring strName = L"";
#endif
	int iAtt = 0;
	int iDef = 0;
	int iJob = 0;
	int iItem = 0; 
}UNIT_DATA;

typedef struct tagFrame
{
	float fStartFrame;
	float fMaxFrameCnt;
}FRAME;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	BYTE byDrawID;
	BYTE byOption;
	int iMyIndex;
	int iParentIndex;
}TILE_INFO;

typedef struct tagTexture
{
	// IDirect3DTexture9의 포인터
	// IDirect3DTexture9: 불러온 텍스처를 조작하는 Com객체.
	LPDIRECT3DTEXTURE9	pTexture;

	// 사용할 텍스처의 정보를 갖고있는 구조체.
	D3DXIMAGE_INFO tImgInfo;
}TEX_INFO;

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// 위치벡터
	D3DXVECTOR3 vDir;	// 방향벡터
	D3DXVECTOR3 vLook;	// 기준벡터
	D3DXVECTOR3 vSize;
	D3DXMATRIX matWorld; // 월드행렬 = 크기행렬 * 자전행렬 * 이동행렬 * 공전행렬 * 부모행렬
}INFO;

class CTerrain;
typedef pair<eColDir, CTerrain*> TerrainPair;


//////////////////////////////////////////////////

typedef pair<eBehavior, eArrow> FsmPair;

#define __STRUCT_H__
#endif