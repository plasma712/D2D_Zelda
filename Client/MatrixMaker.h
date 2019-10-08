#pragma once

// ������� ������ Ǯ���ֱ����� Ŭ����.

// �����Ϻ�ȯ���.
// ȸ����ȯ���.
// �̵���ȯ���.

class CMatrixMaker final 
{
public:
	explicit CMatrixMaker();
	virtual ~CMatrixMaker() = default;
public:
	// ���ϰ� : �Լ����� ���� �������� �ּ�.
	static D3DXMATRIX* MatrixScaling(D3DXMATRIX* pMatrix, float fX, float fY, float fZ);
	static D3DXMATRIX* MatrixRotationZ(D3DXMATRIX* pMatrix, float fRadian);
	static D3DXMATRIX* MatrixTranslation(D3DXMATRIX* pMatrix, float fX, float fY, float fZ);

};

