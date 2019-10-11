#include "stdafx.h"
#include "Tile.h"


CTile::CTile()
{
	tileInfo.vPos = { 0.f,0.f,0.f };
	tileInfo.vSize = { 1.f,1.f,0.f };
}


CTile::~CTile()
{
}

int CTile::Update()
{
	return 0;
}

void CTile::LateUpdate()
{
}

void CTile::Render()
{
}

HRESULT CTile::Initialize()
{
	UpdateRECT();
	return E_NOTIMPL;
}

void CTile::Release()
{
}
