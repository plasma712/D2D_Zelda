// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MiniView.h"
#include "Terrain.h"
#include "FileInfo.h"
#include "afxdialogex.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrl);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedLoad)
	ON_STN_CLICKED(IDC_PICTURE, &CMapTool::OnStnClickedPicture)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.

// WM_DROPFILES �޽��� �߻��� ȣ��Ǵ� �Լ�.
void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialog::OnDropFiles(hDropInfo);

	//::AfxMessageBox(L"Drag and Drop!");	

	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szRemoveExt[MAX_STR] = L"";

	CString strRelativePath = L"";
	CString strFileName = L"";

	// �ι�° ���ڰ� -1�� ��� �巡�� �� ��ӵ� ����� ��ü ������ ��ȯ�Ѵ�.
	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	for (int i = 0; i < iCount; ++i)
	{
		// hDropInfo: �巡�׵� ������ ������ �Ǵ� �ڵ�.
		//::DragQueryFile(hDropInfo, index, FileName(out), BufferLength)
		::DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);

		// ����� ��ȯ.
		strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);

		// ���ϸ� ���ܺ���.
		// ���� ��ο��� ���ϸ� ��ȯ���ִ� �Լ�.
		strFileName = ::PathFindFileName(strRelativePath);

		// Ȯ���� ����
		lstrcpy(szRemoveExt, strFileName);
		::PathRemoveExtension(szRemoveExt);

		m_ListBox.AddString(szRemoveExt);
	}

	����ũ�Ѹ����();
}

void CMapTool::����ũ�Ѹ����()
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


void CMapTool::OnLbnSelchangeList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strSelectName = L"";
	m_ListBox.GetText(iIndex, strSelectName);

	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		// �������� �������� �Ǻ� �Լ�. ���ڶ�� true
		if (isdigit(strSelectName[i]))
			break;
	}

	// CString::Delete(index, count)
	// ���� ���ڿ� index���� count��ŭ ����
	strSelectName.Delete(0, i);

	// _ttoi: ���� -> ������ ��ȯ.
	m_iDrawID = _ttoi(strSelectName);

	// �̸����� ���
	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(
		L"Stage02", L"Tile", m_iDrawID);
	NULL_CHECK(pTexInfo);

	CDeviceMgr::GetInstance()->Render_Begin();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(WINCX) / TILECX, float(WINCY) / TILECY, 0.f);
	D3DXMatrixTranslation(&matTrans, 200.f, 150.f, 0.f);

	matWorld = matScale * matTrans;

	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDeviceMgr::GetInstance()->Render_End(m_PictureCtrl.m_hWnd);

	UpdateData(FALSE);
}


void CMapTool::OnBnClickedSave()
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


void CMapTool::OnBnClickedLoad()
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


void CMapTool::OnStnClickedPicture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
