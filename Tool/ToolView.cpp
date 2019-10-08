
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸
HWND g_hWnd;

CToolView::CToolView()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTerrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
	SafeDelete(m_pTerrain);
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

// WM_PAINT 메시지 발생할 때 호출됨.
void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	/*pDC->Rectangle(100, 100, 200, 200);	
	pDC->Ellipse(100, 100, 200, 200);*/

	m_pDeviceMgr->Render_Begin();

	m_pTerrain->Render();

	m_pDeviceMgr->Render_End();
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// 스크롤 x, y 범위 지정
	int cx = TILECX * TILEX; // 가로 스크롤 범위
	int cy = TILECY / 2 * TILEY;

	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));


	// View 사이즈 800 * 600으로 조정하기.

	// 프레임 윈도우를 얻어온다.
	// AfxGetApp: CWinApp의 포인터를 얻어오는 MFC 전역 함수.
	// GetMainWnd: MainFrame의 포인터를 얻어오는 CWinApp의 멤버함수.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	// MainFrame의 크기를 얻어온다.
	// GetWindowRect: 윈도우의 전체 테두리를 포함한 사각형 크기를 얻어오는 함수.
	// 이 때 left, top, right, bottom은 스크린좌표 기준이다.
	RECT rcMain = {};
	pMainFrm->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);


	// View의 크기를 얻어온다.
	// GetClientRect: 윈도우의 테두리를 포함하지 않은 순수 클라이언트 영역의 사각형 크기를 얻어오는 함수.
	// 이 때 left, top, rigth, bottom은 클라이언트 좌표 기준이다.
	// 즉, left, top은 항상 0, 0이다.
	RECT rcView = {};
	GetClientRect(&rcView);

	// MainFrame과 View의 간격을 계산.
	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	// MainFrame의 윈도우 위치와 크기를 다시 조정.
	pMainFrm->SetWindowPos(
		nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER);


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

// WM_LBUTTONDOWN 메시지 발생할 때 호출됨.
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

	// 화면 갱신 함수. WM_PAINT 메시지 발생.
	Invalidate(FALSE);

	// MiniView 갱신
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	pMiniView->Invalidate(FALSE);
}

// WM_MOUSEMOVE 메시지 발생할 때 호출됨.
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

		// 화면 갱신 함수. WM_PAINT 메시지 발생.
		Invalidate(FALSE);

		// MiniView 갱신
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
		NULL_CHECK(pMiniView);

		pMiniView->Invalidate(FALSE);
	}
}
