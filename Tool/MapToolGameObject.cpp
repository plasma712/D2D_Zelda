// MapToolGameObject.cpp : ���� �����Դϴ�.
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


// MapToolGameObject ��ȭ �����Դϴ�.

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


// MapToolGameObject �޽��� ó�����Դϴ�.



// WM_DROPFILES �޽��� �߻��� ȣ��Ǵ� �Լ�.
void MapToolGameObject::OnDropFiles(HDROP hDropInfo)
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

}

void MapToolGameObject::OnLbnSelchangeList()
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


void MapToolGameObject::OnBnClickedLoad()
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


void MapToolGameObject::OnStnClickedPicture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
