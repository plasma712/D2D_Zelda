#pragma once
#include "afxwin.h"


// MapToolGameObject 대화 상자입니다.

class MapToolGameObject : public CDialog
{
	DECLARE_DYNAMIC(MapToolGameObject)

public:
	MapToolGameObject(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MapToolGameObject();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOLGAMEOBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
