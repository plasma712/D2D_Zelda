#pragma once
#include "afxwin.h"


// MapToolGameObject ��ȭ �����Դϴ�.

class MapToolGameObject : public CDialog
{
	DECLARE_DYNAMIC(MapToolGameObject)

public:
	MapToolGameObject(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MapToolGameObject();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOLGAMEOBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnStnClickedPicture();

public:
	CListBox m_ListBox;
	CStatic m_PictureCtrl1;
public:
	int m_iDrawID = 0;

};
