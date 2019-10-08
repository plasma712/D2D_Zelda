#pragma once
#include "afxwin.h"


// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:	// Message Function
	afx_msg void OnBnClickedPush();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnEnChangeFindName();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

public:	// Value Variable
	// ù��° ������Ʈ�ѿ� ���ڿ��� �Է� ���� ����
	CString m_strInName;
	// �ι�° ������Ʈ�ѿ� ���ڿ��� ���
	CString m_strOutName;
	
	// ĳ���� �̸�
	CString m_strName;
	// ĳ���� ���ݷ�
	int m_iAtt;
	// ĳ���� ����
	int m_iDef;		

	// ĳ���� �˻�
	CString m_strFindName;

public:	// Control Variable
	CListBox m_ListBox;

	// ���� ����
	CButton m_Radio[3];

	// ������ ����
	CButton m_Check[3];

public:	// User Variable
	map<CString, UNIT_DATA*>	m_mapUnitData;	
	int iItemArr[3] = { RUBY, SAPPHIRE, DIAMOND };		
};
