// PathTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "FileInfo.h"
#include "afxdialogex.h"


// CPathTool 대화 상자입니다.

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


// CPathTool 메시지 처리기입니다.


void CPathTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
		// 정수 -> 문자
		_itow_s(pPathInfo->iImgCount, szImgCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
			L"|" + szImgCount + L"|" + pPathInfo->wstrImgPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	수평스크롤만들기();
}

void CPathTool::수평스크롤만들기()
{
	CDC* pDC = m_ListBox.GetDC();
	CSize stringPixel;
	int iCX = 0;

	CString strFileName = L"";

	// CListBox::GetCount
	// 현재 리스트박스에 들어있는 전체 데이터 개수 반환.
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strFileName);

		// GetTextExtent: 인자로 넘겨받은 문자열의 cx, cy 픽셀사이즈를 구해주는 함수.
		stringPixel = pDC->GetTextExtent(strFileName);

		if (iCX < stringPixel.cx)
			iCX = stringPixel.cx;
	}

	// 현재 리스트박스가 가지고 있는 수평 스크롤 최대 범위가 더 적으면 확장해라.
	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX); // 수평 스크롤 범위 재지정.

	m_ListBox.ReleaseDC(pDC);
}


void CPathTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(FALSE, L".txt", L"제목없음.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
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
		wofstream fout;

		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		wstring wstrCombined = L"";
		TCHAR szImgCount[MAX_STR] = L"";

		for (auto& pPathInfo : m_PathInfoLst)
		{
			// 정수 -> 문자
			_itow_s(pPathInfo->iImgCount, szImgCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
				L"|" + szImgCount + L"|" + pPathInfo->wstrImgPath;

			fout << wstrCombined << endl;
		}

		fout.close();
	}

	UpdateData(FALSE);

	// 윈도우즈 기본 응용프로그램 실행.
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(TRUE, L".txt", L"제목없음.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
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

	수평스크롤만들기();
}
