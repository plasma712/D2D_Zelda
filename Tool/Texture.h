#pragma once
class CTexture
{
protected:
	CTexture();

public:
	virtual ~CTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", /* ���� Ű*/
		int iIndex = 0	/* �̹��� ��ȣ */) PURE;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath, /* �ҷ��� �̹��� ��� */
		const wstring& wstrStateKey = L"", /* ���� Ű*/
		int iImgCount = 0 /* �̹��� ���� */) PURE;

protected:
	virtual void Release() PURE;

protected:
	CDeviceMgr*	m_pDeviceMgr;
};

