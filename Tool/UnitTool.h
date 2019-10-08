#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
	// 첫번째 에딧컨트롤에 문자열을 입력 받을 변수
	CString m_strInName;
	// 두번째 에딧컨트롤에 문자열을 출력
	CString m_strOutName;
	
	// 캐릭터 이름
	CString m_strName;
	// 캐릭터 공격력
	int m_iAtt;
	// 캐릭터 방어력
	int m_iDef;		

	// 캐릭터 검색
	CString m_strFindName;

public:	// Control Variable
	CListBox m_ListBox;

	// 직업 선택
	CButton m_Radio[3];

	// 아이템 선택
	CButton m_Check[3];

public:	// User Variable
	map<CString, UNIT_DATA*>	m_mapUnitData;	
	int iItemArr[3] = { RUBY, SAPPHIRE, DIAMOND };		
};
