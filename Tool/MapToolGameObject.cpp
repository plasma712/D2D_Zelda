// MapToolGameObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MiniView.h"
#include "Terrain.h"
#include "FileInfo.h"
#include "MapToolGameObject.h"
#include "afxdialogex.h"


// MapToolGameObject 대화 상자입니다.

IMPLEMENT_DYNAMIC(MapToolGameObject, CDialog)

MapToolGameObject::MapToolGameObject(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOLGAMEOBJECT, pParent)
{

}

MapToolGameObject::~MapToolGameObject()
{
}

void MapToolGameObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrl1);
}


BEGIN_MESSAGE_MAP(MapToolGameObject, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &MapToolGameObject::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON1, &MapToolGameObject::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &MapToolGameObject::OnBnClickedLoad)
	ON_STN_CLICKED(IDC_PICTURE, &MapToolGameObject::OnStnClickedPicture)
END_MESSAGE_MAP()


// MapToolGameObject 메시지 처리기입니다.



// WM_DROPFILES 메시지 발생시 호출되는 함수.
void MapToolGameObject::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialog::OnDropFiles(hDropInfo);

	//::AfxMessageBox(L"Drag and Drop!");	

	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szRemoveExt[MAX_STR] = L"";

	CString strRelativePath = L"";
	CString strFileName = L"";

	// 두번째 인자가 -1일 경우 드래그 앤 드롭된 대상의 전체 개수를 반환한다.
	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	for (int i = 0; i < iCount; ++i)
	{
		// hDropInfo: 드래그된 대상들의 기준이 되는 핸들.
		//::DragQueryFile(hDropInfo, index, FileName(out), BufferLength)
		::DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);

		// 상대경로 변환.
		strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);

		// 파일명만 남겨보기.
		// 현재 경로에서 파일명만 반환해주는 함수.
		strFileName = ::PathFindFileName(strRelativePath);

		// 확장자 제거
		lstrcpy(szRemoveExt, strFileName);
		::PathRemoveExtension(szRemoveExt);

		m_ListBox.AddString(szRemoveExt);
	}

}

void MapToolGameObject::OnLbnSelchangeList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strSelectName = L"";
	m_ListBox.GetText(iIndex, strSelectName);

	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		// 글자인지 숫자인지 판별 함수. 숫자라면 true
		if (isdigit(strSelectName[i]))
			break;
	}

	// CString::Delete(index, count)
	// 현재 문자열 index부터 count만큼 제거
	strSelectName.Delete(0, i);

	// _ttoi: 문자 -> 정수로 변환.
	m_iDrawID = _ttoi(strSelectName);

	// 미리보기 기능
	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
		L"Terrain", L"Object", m_iDrawID);
	NULL_CHECK(pTexInfo);

	CDeviceMgr::GetInstance()->Render_Begin();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(WINCX) / TILECX, float(WINCY) / TILECY, 0.f);
	D3DXMatrixTranslation(&matTrans, 200.f, 150.f, 0.f);

	matWorld = matScale * matTrans;

	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDeviceMgr::GetInstance()->Render_End(m_PictureCtrl1.m_hWnd);

	UpdateData(FALSE);
}


void MapToolGameObject::OnBnClickedSave()
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

	if (IDOK == Dlg.DoModal())
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pToolView);

		CTerrain* pTerrain = pToolView->m_pTerrain;
		NULL_CHECK(pTerrain);

		pTerrain->SaveTile(Dlg.GetPathName());
	}

	UpdateData(FALSE);
}


void MapToolGameObject::OnBnClickedLoad()
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
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pToolView);

		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
		NULL_CHECK(pMiniView);

		CTerrain* pTerrain = pToolView->m_pTerrain;
		NULL_CHECK(pTerrain);

		pTerrain->LoadTile(Dlg.GetPathName());
		pToolView->Invalidate(FALSE);
		pMiniView->Invalidate(FALSE);
	}

	UpdateData(FALSE);
}


void MapToolGameObject::OnStnClickedPicture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
