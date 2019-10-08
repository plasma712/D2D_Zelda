// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM), m_MySheet(L"줍줍")
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedUnitTool)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyForm::OnBnClickedPathTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnBnClickedMySheet)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnBnClickedButton8)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Font.CreatePointFont(200, L"consolas"); // (폰트크기, 서체)

	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON6)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);

	// 다이얼로그가 생성되기 전의 윈도우 핸들은 null이다.
	if(nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL); // 다이얼로그 생성.

	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL); // 다이얼로그 생성.

	if (nullptr == m_PathTool.GetSafeHwnd())
		m_PathTool.Create(IDD_PATHTOOL); // 다이얼로그 생성.

	if (nullptr == m_MySheet.GetSafeHwnd())
		m_MySheet.Create(0, WS_OVERLAPPEDWINDOW); 

	if (nullptr == m_MapToolGameObject.GetSafeHwnd())
		m_MapToolGameObject.Create(IDD_MAPTOOLGAMEOBJECT);

}


void CMyForm::OnBnClickedUnitTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//::AfxMessageBox(L"Unit Tool 실행");
	m_UnitTool.ShowWindow(SW_SHOW); // SW_SHOW: 창모양으로 출력
}


void CMyForm::OnBnClickedMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedPathTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_PathTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedMySheet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MySheet.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MapToolGameObject.ShowWindow(SW_SHOW);
}
