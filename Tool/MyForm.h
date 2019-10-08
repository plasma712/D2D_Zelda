#pragma once
// CMyForm �� ���Դϴ�.
#include "UnitTool.h"
#include "MapTool.h"
#include "PathTool.h"
#include "MySheet.h"
#include "MapToolGameObject.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()

public:	// Message Functions
	afx_msg void OnBnClickedUnitTool();
	afx_msg void OnBnClickedMapTool();
	afx_msg void OnBnClickedPathTool();
	afx_msg void OnBnClickedMySheet();
	afx_msg void OnBnClickedButton8();

public:
	// MFC ���� ��Ʈ
	CFont		m_Font;
	CUnitTool	m_UnitTool;
	CMapTool	m_MapTool;	
	CPathTool	m_PathTool;
	CMySheet	m_MySheet;	
	MapToolGameObject m_MapToolGameObject;


};


