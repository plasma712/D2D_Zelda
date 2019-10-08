// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

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


// CUnitTool �޽��� ó�����Դϴ�.


void CUnitTool::OnBnClickedPush()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// UpdateData�Լ��� ȣ���ϸ� DoDataExchange�Լ��� �����.
	// TRUE�� �� ��Ʈ�� -> ������ ������ ����.
	UpdateData(TRUE);

	m_strOutName = m_strInName;

	// FALSE�� �� ���� -> ��Ʈ�ѷ� ������ ����.
	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		// ���� ��ư�� üũ�Ǿ� ������ TRUE ����
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
	// ����Ʈ�ڽ��� ������ ����� �߰��ϴ� �Լ�.
	m_ListBox.AddString(m_strName);

	UpdateData(FALSE);
}


void CUnitTool::OnLbnSelchangeList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	// ���� ����Ʈ�ڽ� �ȿ� �ִ� ��� �� ���õ� ����� �ε����� ��ȯ�Ѵ�.
	int iIndex = m_ListBox.GetCurSel();

	// GetCurSel�� �߸��� ����� �������� �� -1 ����
	if (-1 == iIndex)
		return;

	CString strSelectName = L"";

	// CListBox::GetText(Index(in), String(out))
	// ����Ʈ��� �� Index�� �ش��ϴ� String�� ��ȯ.
	m_ListBox.GetText(iIndex, strSelectName);

	auto iter_find = m_mapUnitData.find(strSelectName);

	if (m_mapUnitData.end() == iter_find)
		return;

	m_strName = iter_find->second->strName;
	m_iAtt = iter_find->second->iAtt;
	m_iDef = iter_find->second->iDef;

	// ��� ��ư �ʱ�ȭ
	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE); // üũ ����
		m_Check[i].SetCheck(FALSE);
	}
	
	m_Radio[iter_find->second->iJob].SetCheck(TRUE); // üũ

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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Radio[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUnitTool::OnEnChangeFindName()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	auto iter_find = m_mapUnitData.find(m_strFindName);

	if (m_mapUnitData.end() == iter_find)
		return;

	// CListBox::FindString(StartAfter, FindString)
	// ���� ����Ʈ�ڽ� ��� �߿� StartAfter���� FindString�� �ִ��� �����ϴ� �Լ�.
	// �����ϸ� �ش� �ε����� ��ȯ.
	int iIndex = m_ListBox.FindString(-1, m_strFindName);

	// ã�� ������ �� -1 ����
	if (-1 == iIndex)
		return;

	// �ش� ����� �����ϰ� ����.
	m_ListBox.SetCurSel(iIndex);
	OnLbnSelchangeList();

	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(FALSE, L".dat", L"�������.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	// Dlg.Create(): ��޸��� ����
	// Dlg.DoModal(): ��� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(TRUE, L".dat", L"�������.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

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

		// ����Ʈ�ڽ� �ʱ�ȭ.
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
