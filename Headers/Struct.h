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
	// IDirect3DTexture9�� ������
	// IDirect3DTexture9: �ҷ��� �ؽ�ó�� �����ϴ� Com��ü.
	LPDIRECT3DTEXTURE9	pTexture;

	// ����� �ؽ�ó�� ������ �����ִ� ����ü.
	D3DXIMAGE_INFO tImgInfo;
}TEX_INFO;

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// ��ġ����
	D3DXVECTOR3 vDir;	// ���⺤��
	D3DXVECTOR3 vLook;	// ���غ���
	D3DXVECTOR3 vSize;
	D3DXMATRIX matWorld; // ������� = ũ����� * ������� * �̵���� * ������� * �θ����
}INFO;

class CTerrain;
typedef pair<eColDir, CTerrain*> TerrainPair;


//////////////////////////////////////////////////

typedef pair<eBehavior, eArrow> FsmPair;

#define __STRUCT_H__
#endif