// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strInName(_T(""))
	, m_strOutName(_T(""))
	, m_strName(_T(""))
	, m_iAtt(0)
	, m_iDef(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	/*for_each(m_mapUnitData.begin(), m_mapUnitData.end(),
		[](auto& MyPair)
	{
		SafeDelete(MyPair.second);
	});*/

	for(auto& MyPair : m_mapUnitData)
		SafeDelete(MyPair.second);

	m_mapUnitData.clear();
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strInName);
	DDX_Text(pDX, IDC_EDIT2, m_strOutName);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAtt);
	DDX_Text(pDX, IDC_EDIT5, m_iDef);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedPush)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnBnClickedAdd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelchangeList)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnEnChangeFindName)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.


void CUnitTool::OnBnClickedPush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// UpdateData함수를 호출하면 DoDataExchange함수가 실행됨.
	// TRUE일 때 컨트롤 -> 변수로 데이터 전달.
	UpdateData(TRUE);

	m_strOutName = m_strInName;

	// FALSE일 때 변수 -> 컨트롤로 데이터 전달.
	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	auto iter_find = m_mapUnitData.find(m_strName);

	if (m_mapUnitData.end() != iter_find)
		return;

	UNIT_DATA* pUnitData = new UNIT_DATA;
	pUnitData->strName = m_strName;
	pUnitData->iAtt = m_iAtt;
	pUnitData->iDef = m_iDef;

	for (int i = 0; i < 3; ++i)
	{
		// 현재 버튼에 체크되어 있으면 TRUE 리턴
		if (m_Radio[i].GetCheck())
		{
			pUnitData->iJob = i;
			break;
		}
	}

	/*if (m_Check[0].GetCheck())
		pUnitData->iItem |= RUBY;
	if (m_Check[1].GetCheck())
		pUnitData->iItem |= SAPPHIRE;
	if (m_Check[2].GetCheck())
		pUnitData->iItem |= DIAMOND;*/

	for (int i = 0; i < 3; ++i)
	{
		if (m_Check[i].GetCheck())
			pUnitData->iItem |= iItemArr[i];
	}

	m_mapUnitData.insert({ m_strName, pUnitData });

	// CListBox::AddString
	// 리스트박스에 데이터 목록을 추가하는 함수.
	m_ListBox.AddString(m_strName);

	UpdateData(FALSE);
}


void CUnitTool::OnLbnSelchangeList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// 현재 리스트박스 안에 있는 목록 중 선택된 대상의 인덱스를 반환한다.
	int iIndex = m_ListBox.GetCurSel();

	// GetCurSel은 잘못된 대상을 선택했을 때 -1 리턴
	if (-1 == iIndex)
		return;

	CString strSelectName = L"";

	// CListBox::GetText(Index(in), String(out))
	// 리스트목록 중 Index에 해당하는 String을 반환.
	m_ListBox.GetText(iIndex, strSelectName);

	auto iter_find = m_mapUnitData.find(strSelectName);

	if (m_mapUnitData.end() == iter_find)
		return;

	m_strName = iter_find->second->strName;
	m_iAtt = iter_find->second->iAtt;
	m_iDef = iter_find->second->iDef;

	// 모든 버튼 초기화
	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE); // 체크 해제
		m_Check[i].SetCheck(FALSE);
	}
	
	m_Radio[iter_find->second->iJob].SetCheck(TRUE); // 체크

	/*if (RUBY & iter_find->second->iItem)
		m_Check[0].SetCheck(TRUE);
	if (SAPPHIRE & iter_find->second->iItem)
		m_Check[1].SetCheck(TRUE);
	if (DIAMOND & iter_find->second->iItem)
		m_Check[2].SetCheck(TRUE);*/

	for (int i = 0; i < 3; ++i)
	{
		if (iItemArr[i] & iter_find->second->iItem)
			m_Check[i].SetCheck(TRUE);
	}

	UpdateData(FALSE);
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Radio[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUnitTool::OnEnChangeFindName()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	auto iter_find = m_mapUnitData.find(m_strFindName);

	if (m_mapUnitData.end() == iter_find)
		return;

	// CListBox::FindString(StartAfter, FindString)
	// 현재 리스트박스 목록 중에 StartAfter부터 FindString이 있는지 조사하는 함수.
	// 존재하면 해당 인덱스를 반환.
	int iIndex = m_ListBox.FindString(-1, m_strFindName);

	// 찾지 못했을 때 -1 리턴
	if (-1 == iIndex)
		return;

	// 해당 목록을 선택하게 만듦.
	m_ListBox.SetCurSel(iIndex);
	OnLbnSelchangeList();

	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strDeleteName = L"";
	m_ListBox.GetText(iIndex, strDeleteName);

	auto iter_find = m_mapUnitData.find(strDeleteName);

	if (m_mapUnitData.end() == iter_find)
		return;

	SafeDelete(iter_find->second);
	m_mapUnitData.erase(strDeleteName);
	m_ListBox.DeleteString(iIndex);

	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(FALSE, L".dat", L"제목없음.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	// Dlg.Create(): 모달리스 형식
	// Dlg.DoModal(): 모달 형식
	if (IDOK == Dlg.DoModal())
	{
		//::AfxMessageBox(Dlg.GetPathName());

		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		TCHAR szName[MAX_STR] = L"";
		int iAtt = 0, iDef = 0, iJob = 0, iItem = 0;

		DWORD dwBytes = 0;

		for (auto& MyPair : m_mapUnitData)
		{
			lstrcpy(szName, MyPair.second->strName);
			iAtt = MyPair.second->iAtt;
			iDef = MyPair.second->iDef;
			iJob = MyPair.second->iJob;
			iItem = MyPair.second->iItem;

			WriteFile(hFile, szName, sizeof(TCHAR) * MAX_STR, &dwBytes, nullptr);
			WriteFile(hFile, &iAtt, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &iDef, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &iJob, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &iItem, sizeof(int), &dwBytes, nullptr);
		}

		CloseHandle(hFile);
	}	

	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(TRUE, L".dat", L"제목없음.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if (IDOK == Dlg.DoModal())
	{
		//::AfxMessageBox(Dlg.GetPathName());

		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		if (!m_mapUnitData.empty())
		{
			for (auto& MyPair : m_mapUnitData)
				SafeDelete(MyPair.second);

			m_mapUnitData.clear();
		}

		// 리스트박스 초기화.
		m_ListBox.ResetContent();

		TCHAR szName[MAX_STR] = L"";
		int iAtt = 0, iDef = 0, iJob = 0, iItem = 0;

		DWORD dwBytes = 0;

		while (true)
		{
			ReadFile(hFile, szName, sizeof(TCHAR) * MAX_STR, &dwBytes, nullptr);
			ReadFile(hFile, &iAtt, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iDef, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iJob, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iItem, sizeof(int), &dwBytes, nullptr);

			if (0 == dwBytes)
				break;

			UNIT_DATA* pUnitData = new UNIT_DATA;
			pUnitData->strName = szName;
			pUnitData->iAtt = iAtt;
			pUnitData->iDef = iDef;
			pUnitData->iJob = iJob;
			pUnitData->iItem = iItem;

			m_mapUnitData.insert({ szName, pUnitData });
			m_ListBox.AddString(szName);
		}

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}
