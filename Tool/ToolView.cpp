
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyForm.h"
#include "Terrain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�
HWND g_hWnd;

CToolView::CToolView()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTerrain(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CToolView::~CToolView()
{
	SafeDelete(m_pTerrain);
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

// WM_PAINT �޽��� �߻��� �� ȣ���.
void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	/*pDC->Rectangle(100, 100, 200, 200);	
	pDC->Ellipse(100, 100, 200, 200);*/

	m_pDeviceMgr->Render_Begin();

	m_pTerrain->Render();

	m_pDeviceMgr->Render_End();
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// ��ũ�� x, y ���� ����
	int cx = TILECX * TILEX; // ���� ��ũ�� ����
	int cy = TILECY / 2 * TILEY;

	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));


	// View ������ 800 * 600���� �����ϱ�.

	// ������ �����츦 ���´�.
	// AfxGetApp: CWinApp�� �����͸� ������ MFC ���� �Լ�.
	// GetMainWnd: MainFrame�� �����͸� ������ CWinApp�� ����Լ�.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	// MainFrame�� ũ�⸦ ���´�.
	// GetWindowRect: �������� ��ü �׵θ��� ������ �簢�� ũ�⸦ ������ �Լ�.
	// �� �� left, top, right, bottom�� ��ũ����ǥ �����̴�.
	RECT rcMain = {};
	pMainFrm->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);


	// View�� ũ�⸦ ���´�.
	// GetClientRect: �������� �׵θ��� �������� ���� ���� Ŭ���̾�Ʈ ������ �簢�� ũ�⸦ ������ �Լ�.
	// �� �� left, top, rigth, bottom�� Ŭ���̾�Ʈ ��ǥ �����̴�.
	// ��, left, top�� �׻� 0, 0�̴�.
	RECT rcView = {};
	GetClientRect(&rcView);

	// MainFrame�� View�� ������ ���.
	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	// MainFrame�� ������ ��ġ�� ũ�⸦ �ٽ� ����.
	pMainFrm->SetWindowPos(
		nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER);


	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;

	HRESULT hr = 0;

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	hr = m_pTextureMgr->LoadTexture(CTextureMgr::MULTI_TEXTURE,
		L"../Texture/Stage/Terrain/Tile/Tile%d.png",
		L"Terrain", L"Tile", 38);
	FAILED_CHECK_MSG(hr, L"Terrain Tile Texture Load Failed");

	m_pTerrain = CTerrain::Create(this);
	NULL_CHECK_MSG(m_pTerrain, L"Terrain Create Failed");
}

// WM_LBUTTONDOWN �޽��� �߻��� �� ȣ���.
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnLButtonDown(nFlags, point);

	D3DXVECTOR3 vMouse =
	{
		float(point.x) + GetScrollPos(0),
		float(point.y) + GetScrollPos(1),
		0.f
	};

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitter.GetPane(1, 0));
	NULL_CHECK(pMyForm);

	int iDrawID = pMyForm->m_MapTool.m_iDrawID;
	m_pTerrain->TileChange(vMouse, iDrawID, 1);

	// ȭ�� ���� �Լ�. WM_PAINT �޽��� �߻�.
	Invalidate(FALSE);

	// MiniView ����
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);
}

// WM_MOUSEMOVE �޽��� �߻��� �� ȣ���.
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnMouseMove(nFlags, point);

	if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		D3DXVECTOR3 vMouse =
		{
			float(point.x) + GetScrollPos(0),
			float(point.y) + GetScrollPos(1),
			0.f
		};

		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitter.GetPane(1, 0));
		NULL_CHECK(pMyForm);

		int iDrawID = pMyForm->m_MapTool.m_iDrawID;
		m_pTerrain->TileChange(vMouse, iDrawID, 1);

		// ȭ�� ���� �Լ�. WM_PAINT �޽��� �߻�.
		Invalidate(FALSE);

		// MiniView ����
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
		NULL_CHECK(pMiniView);

		pMiniView->Invalidate(FALSE);
	}
}
