#include "pch.h"
#include <sstream>

namespace MyWin
{

  class WndFileTreeMenuButton : public CMFCToolBarMenuButton
  {
    friend class WndFileTree;

    DECLARE_SERIAL(WndFileTreeMenuButton)

  public:
    WndFileTreeMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT) -1, hMenu, -1)
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

  IMPLEMENT_SERIAL(WndFileTreeMenuButton, CMFCToolBarMenuButton, 1)

  //////////////////////////////////////////////////////////////////////
  // Construction/Destruction
  //////////////////////////////////////////////////////////////////////

    WndFileTree::WndFileTree() noexcept {}

  WndFileTree::~WndFileTree() {}

  BEGIN_MESSAGE_MAP(WndFileTree, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // WndFileTree message handlers


  bool WndFileTree::CreatePane(CWnd* parent)
  {
    CString str;
    BOOL bNameValid = str.LoadString(IDS_FILETREE);
    ASSERT(bNameValid);
    if (!Create(str, parent, CRect(0, 0, 800, 50), TRUE, ID_VIEW_FILETREE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
      TRACE0("Failed to create File Tree window\n");
      return false; // failed to create
    }

    HICON hFileTreeIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_FILETREE_HC : IDI_FILETREE), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    SetIcon(hFileTreeIcon, FALSE);

    return true;
  }

  int WndFileTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
    if (CDockablePane::OnCreate(lpCreateStruct) == -1) return -1;

    // Create Tree Control
    CRect rectDummy;
    rectDummy.SetRectEmpty();
    if (!m_wndTree.Create(WS_CHILD | WS_VISIBLE | TVS_TRACKSELECT | TVS_FULLROWSELECT | TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rectDummy, this, ID_VIEW_FILETREE))
    {
      TRACE0("Failed to create File Tree\n");
      return -1; // fail to create
    }

    // set up tool bar:
    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDB_FILETREE);
    m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

    OnChangeVisualStyle();

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

    m_wndToolBar.SetOwner(this);

    // All commands will be routed via this control , not via the parent frame:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    CMenu menuSort;
    menuSort.LoadMenu(IDR_POPUP_SORT);

//    m_wndToolBar.ReplaceButton(ID_SORT_MENU, WndFileTreeMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

    WndFileTreeMenuButton* pButton = DYNAMIC_DOWNCAST(WndFileTreeMenuButton, m_wndToolBar.GetButton(0));

    if (pButton != nullptr)
    {
      pButton->m_bText = FALSE;
      pButton->m_bImage = TRUE;
      pButton->SetMessageWnd(this);
    }

    return 0;
  }

  void WndFileTree::OnSize(UINT nType, int cx, int cy)
  {
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
  }

  void WndFileTree::OnContextMenu(CWnd* pWnd, CPoint point)
  {
    CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndTree;
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

      if (!pPopupMenu->Create(this, point.x, point.y, (HMENU) pSumMenu->m_hMenu, FALSE, TRUE))
        return;

      ((CMDIFrameWndEx*) AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
      UpdateDialogControls(this, FALSE);
    }
  }

  void WndFileTree::AdjustLayout()
  {
    if (GetSafeHwnd() == nullptr)
    {
      return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndTree.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
  }

  BOOL WndFileTree::PreTranslateMessage(MSG* pMsg)
  {
    return CDockablePane::PreTranslateMessage(pMsg);
  }

  void WndFileTree::OnPaint()
  {
    CPaintDC dc(this); // device context for painting

    CRect rectTree;
    m_wndTree.GetWindowRect(rectTree);
    ScreenToClient(rectTree);

    rectTree.InflateRect(1, 1);
    dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
  }

  void WndFileTree::OnSetFocus(CWnd* pOldWnd)
  {
    CDockablePane::OnSetFocus(pOldWnd);

    m_wndTree.SetFocus();
  }

  void WndFileTree::OnChangeVisualStyle()
  {
    m_Images.DeleteImageList();

    UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILETREE_HC : IDB_FILETREE;

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

    m_Images.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
    m_Images.Add(&bmp, RGB(255, 0, 255));

    m_wndTree.SetImageList(&m_Images, TVSIL_NORMAL);

    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
  }

  FMap WndFileTree::FillFileTree(const DList& dl)
  {
  // clear everything out
    files_.clear();
    m_wndTree.DeleteAllItems();

    // set up root items
    HTREEITEM hRoot = m_wndTree.InsertItem(_T("File Tree"), 0, 0);
    m_wndTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
    m_wndTree.Expand(hRoot, TVE_EXPAND);

    // collect all files for each starting directory
    for (auto& d : dl)
    {
      HTREEITEM h = m_wndTree.InsertItem(d.path().c_str(), 0, 0, hRoot);
      CollectFiles(d, h);
      m_wndTree.Expand(hRoot, TVE_EXPAND);
    }

    FMap fmap{};
    for (auto& f : files_)
    {
      fmap.insert({ f.hash, f });
    }
    return fmap;
  }


  void WndFileTree::CollectFiles(const std::filesystem::directory_entry& s, HTREEITEM hScope)
  {
    for (auto& p : std::filesystem::directory_iterator(s, std::filesystem::directory_options::skip_permission_denied))
    {

      if (p.is_regular_file())  // collect data, but don't enter in tree
      {
        std::error_code ec{};

        std::wstringstream cls;
        cls << md5.digestFile(p.path().string().c_str());
        FileData f{ p, cls.str() };
        if (ec) continue; // ignore all files with errors
        files_.push_back(f);
      }

      if (p.is_directory()) // enter in tree, but don't collect data
      {
        HTREEITEM h = m_wndTree.InsertItem(p.path().filename().wstring().c_str(), 0, 0, hScope);
        CollectFiles(p, h);
        m_wndTree.Expand(h, TVE_EXPAND);
      }
    }
  }

}
