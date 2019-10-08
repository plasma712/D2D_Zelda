// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM), m_MySheet(L"����")
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


// CMyForm �����Դϴ�.

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


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_Font.CreatePointFont(200, L"consolas"); // (��Ʈũ��, ��ü)

	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON6)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);

	// ���̾�αװ� �����Ǳ� ���� ������ �ڵ��� null�̴�.
	if(nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL); // ���̾�α� ����.

	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL); // ���̾�α� ����.

	if (nullptr == m_PathTool.GetSafeHwnd())
		m_PathTool.Create(IDD_PATHTOOL); // ���̾�α� ����.

	if (nullptr == m_MySheet.GetSafeHwnd())
		m_MySheet.Create(0, WS_OVERLAPPEDWINDOW); 

	if (nullptr == m_MapToolGameObject.GetSafeHwnd())
		m_MapToolGameObject.Create(IDD_MAPTOOLGAMEOBJECT);

}


void CMyForm::OnBnClickedUnitTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//::AfxMessageBox(L"Unit Tool ����");
	m_UnitTool.ShowWindow(SW_SHOW); // SW_SHOW: â������� ���
}


void CMyForm::OnBnClickedMapTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedPathTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_PathTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedMySheet()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_MySheet.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedButton8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_MapToolGameObject.ShowWindow(SW_SHOW);
}
