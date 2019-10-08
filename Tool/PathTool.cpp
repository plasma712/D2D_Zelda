// PathTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "FileInfo.h"
#include "afxdialogex.h"


// CPathTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathTool, CDialog)

CPathTool::CPathTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHTOOL, pParent)
{

}

CPathTool::~CPathTool()
{
	for_each(m_PathInfoLst.begin(), m_PathInfoLst.end(), SafeDelete<PATH_INFO*>);
	m_PathInfoLst.clear();
}

void CPathTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathTool, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPathTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CPathTool �޽��� ó�����Դϴ�.


void CPathTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialog::OnDropFiles(hDropInfo);

	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFilePath[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);
		CFileInfo::ExtractPathInfo(szFilePath, m_PathInfoLst);		
	}

	wstring wstrCombined = L"";
	TCHAR szImgCount[MAX_STR] = L"";

	for (auto& pPathInfo : m_PathInfoLst)
	{
		// ���� -> ����
		_itow_s(pPathInfo->iImgCount, szImgCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
			L"|" + szImgCount + L"|" + pPathInfo->wstrImgPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	����ũ�Ѹ����();
}

void CPathTool::����ũ�Ѹ����()
{
	CDC* pDC = m_ListBox.GetDC();
	CSize stringPixel;
	int iCX = 0;

	CString strFileName = L"";

	// CListBox::GetCount
	// ���� ����Ʈ�ڽ��� ����ִ� ��ü ������ ���� ��ȯ.
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strFileName);

		// GetTextExtent: ���ڷ� �Ѱܹ��� ���ڿ��� cx, cy �ȼ������ �����ִ� �Լ�.
		stringPixel = pDC->GetTextExtent(strFileName);

		if (iCX < stringPixel.cx)
			iCX = stringPixel.cx;
	}

	// ���� ����Ʈ�ڽ��� ������ �ִ� ���� ��ũ�� �ִ� ������ �� ������ Ȯ���ض�.
	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX); // ���� ��ũ�� ���� ������.

	m_ListBox.ReleaseDC(pDC);
}


void CPathTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(FALSE, L".txt", L"�������.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
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
		wofstream fout;

		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		wstring wstrCombined = L"";
		TCHAR szImgCount[MAX_STR] = L"";

		for (auto& pPathInfo : m_PathInfoLst)
		{
			// ���� -> ����
			_itow_s(pPathInfo->iImgCount, szImgCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
				L"|" + szImgCount + L"|" + pPathInfo->wstrImgPath;

			fout << wstrCombined << endl;
		}

		fout.close();
	}

	UpdateData(FALSE);

	// �������� �⺻ �������α׷� ����.
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(TRUE, L".txt", L"�������.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
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
		wifstream fin;

		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		if (!m_PathInfoLst.empty())
		{
			for_each(m_PathInfoLst.begin(), m_PathInfoLst.end(), SafeDelete<PATH_INFO*>);
			m_PathInfoLst.clear();
		}

		m_ListBox.ResetContent();

		TCHAR szObjectKey[MAX_STR] = L"";
		TCHAR szStateKey[MAX_STR] = L"";
		TCHAR szImgCount[MAX_STR] = L"";
		TCHAR szImgPath[MAX_STR] = L"";

		while (true)
		{
			fin.getline(szObjectKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szImgCount, MAX_STR, '|');
			fin.getline(szImgPath, MAX_STR);

			if (fin.eof())
				break;

			PATH_INFO* pPathInfo = new PATH_INFO;
			pPathInfo->wstrObjectKey = szObjectKey;
			pPathInfo->wstrStateKey = szStateKey;
			pPathInfo->iImgCount = _ttoi(szImgCount);
			pPathInfo->wstrImgPath = szImgPath;

			m_PathInfoLst.push_back(pPathInfo);

			wstring wstrCombined = wstring(szObjectKey) + L"|" + szStateKey + L"|"
				+ szImgCount + L"|" + szImgPath;

			m_ListBox.AddString(wstrCombined.c_str());
		}

		fin.close();		
	}

	UpdateData(FALSE);

	����ũ�Ѹ����();
}
