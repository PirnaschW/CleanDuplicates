
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"
#include "ScopeView.h"
#include "Resource.h"
#include "CleanDuplicates.h"
#include "Traverse.h"

class CScopeViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CScopeView;

	DECLARE_SERIAL(CScopeViewMenuButton)

public:
	CScopeViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CScopeViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScopeView::CScopeView() noexcept
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CScopeView::~CScopeView()
{
}

BEGIN_MESSAGE_MAP(CScopeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnScopeAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnScopeAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnScopeDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnScopeProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScopeView message handlers

int CScopeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndScopeView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Scope View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CScopeViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CScopeViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CScopeViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillScopeView();

	return 0;
}

void CScopeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


bool CScopeView::CollectFiles(const std::wstring& path, const WIN32_FIND_DATA& data, bool children)
{
	FileData f{ std::wstring(path + L"\\" + data.cFileName + L"\n") , children,  data.ftCreationTime, data.ftLastWriteTime };
	files_.push_back(f);

	m_wndScopeView.Expand(hScope, TVE_EXPAND);
	if (children) hScope = m_wndScopeView.InsertItem(data.cFileName, 0, 0, hScope);
	else m_wndScopeView.InsertItem(data.cFileName, 2, 2, hScope);
	m_wndScopeView.Expand(hScope, TVE_EXPAND);

	return true;  // always drill down further
}

void CScopeView::FillScopeView()
{
  // clear everything out
	files_.clear();
	hRoot = {};
	hScope = {};
	m_wndScopeView.DeleteAllItems();

  // set up root items
	hRoot = m_wndScopeView.InsertItem(_T("Scope"), 0, 0);
	m_wndScopeView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	hScope = m_wndScopeView.InsertItem(LR"(D:\Projects\Test1)", 0, 0, hRoot);
	m_wndScopeView.InsertItem(LR"(D:\Projects\Test1)", 2, 2, hScope);
	m_wndScopeView.Expand(hScope, TVE_EXPAND);


  // add all other items
	Traverse::ProcessTree(LR"(D:\Projects\Test1)", std::bind(&CScopeView::CollectFiles, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));



	//hScope = m_wndScopeView.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	//m_wndScopeView.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hScope);

	m_wndScopeView.Expand(hRoot, TVE_EXPAND);

	hScope = m_wndScopeView.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	m_wndScopeView.InsertItem(_T("CFakeAppFrame()"), 2, 2, hScope);
	m_wndScopeView.InsertItem(_T("~CFakeAppFrame()"), 2, 2, hScope);
	m_wndScopeView.InsertItem(_T("m_wndMenuBar"), 2, 2, hScope);
	m_wndScopeView.InsertItem(_T("m_wndToolBar"), 2, 2, hScope);
	m_wndScopeView.InsertItem(_T("m_wndStatusBar"), 2, 2, hScope);

	hScope = m_wndScopeView.InsertItem(_T("Globals"), 2, 2, hRoot);
	m_wndScopeView.InsertItem(_T("theFakeApp"), 2, 2, hScope);
	m_wndScopeView.Expand(hScope, TVE_EXPAND);
}

void CScopeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndScopeView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CScopeView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndScopeView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CScopeView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CScopeView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CScopeViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CScopeViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CScopeView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CScopeView::OnScopeAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void CScopeView::OnScopeAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CScopeView::OnScopeDefinition()
{
	// TODO: Add your command handler code here
}

void CScopeView::OnScopeProperties()
{
	// TODO: Add your command handler code here
}

void CScopeView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CScopeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndScopeView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CScopeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndScopeView.SetFocus();
}

void CScopeView::OnChangeVisualStyle()
{
	m_ScopeViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ScopeViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ScopeViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndScopeView.SetImageList(&m_ScopeViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}
