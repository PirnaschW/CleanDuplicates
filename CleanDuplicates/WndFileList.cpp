#include "pch.h"

namespace MyWin
{

  WndFileList::WndFileList() noexcept {}
  WndFileList::~WndFileList() {}

  BEGIN_MESSAGE_MAP(WndFileList, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_UPDATE_COMMAND_UI(ID_DIR_ADD, OnUpdateDirAdd)
    ON_UPDATE_COMMAND_UI(ID_DIR_DEL, OnUpdateDirDel)
    ON_UPDATE_COMMAND_UI(ID_DIR_EXECUTE, OnUpdateDirExecute)
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // CResourceViewBar message handlers

  void WndFileList::AdjustLayout()
  {
    if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
    {
      return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
    m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndList.SetWindowPos(nullptr, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
  }

  bool WndFileList::CreatePane(CWnd* parent)
  {
    CString str;
    BOOL bNameValid = str.LoadString(IDS_FILELIST);
    ASSERT(bNameValid);
    if (!Create(str, parent, CRect(0, 0,600, 50), TRUE, ID_VIEW_FILELIST, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
      TRACE0("Failed to create File List window\n");
      return false; // failed to create
    }

    HICON hDirListIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_FILELIST_HC : IDI_FILELIST), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    SetIcon(hDirListIcon, FALSE);

    return true;

  }

  int WndFileList::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
    if (CDockablePane::OnCreate(lpCreateStruct) == -1) return -1;

    // create List Control
    CRect rectDummy;
    rectDummy.SetRectEmpty();
    if (!m_wndList.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rectDummy, this, ID_VIEW_FILELIST))
    {
      TRACE0("Failed to create Directory List\n");
      return -1; // fail to create
    }
    m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    SetFont();
    // define columns
    m_wndList.InsertColumn(0, _T("Path"), LVCFMT_LEFT, 300);
    m_wndList.InsertColumn(1, _T("Filename"), LVCFMT_RIGHT, 100);
    m_wndList.InsertColumn(2, _T("Size"), LVCFMT_RIGHT, 100);
    m_wndList.InsertColumn(3, _T("MD5 Hash"), LVCFMT_CENTER, 300);
    m_wndList.InsertColumn(4, _T("Duplicate"), LVCFMT_CENTER, 50);

    // set up tool bar
    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDB_FILELIST);
    m_wndToolBar.LoadToolBar(IDB_DIRLIST, 0, 0, TRUE /* Is locked */);
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_FILELIST_HC : IDB_FILELIST, 0, 0, TRUE /* Locked */);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // All commands will be routed via this control, not via the parent frame:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();
    return 0;
  }

  void WndFileList::OnSize(UINT nType, int cx, int cy)
  {
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
  }

  void WndFileList::InitList()
  {
  }

  void WndFileList::OnSetFocus(CWnd* pOldWnd)
  {
    CDockablePane::OnSetFocus(pOldWnd);
    m_wndList.SetFocus();
  }

  void WndFileList::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
  {
    CDockablePane::OnSettingChange(uFlags, lpszSection);
    SetFont();
  }

  void WndFileList::SetFont()
  {
    ::DeleteObject(m_fnt.Detach());
    ::DeleteObject(m_fntH.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info{};
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);
    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;
    m_fnt.CreateFontIndirect(&lf);
    m_wndList.SetFont(&m_fnt);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight * 2;
    lf.lfItalic = info.lfMenuFont.lfItalic;
    m_fntH.CreateFontIndirect(&lf);
    m_wndList.GetHeaderCtrl()->SetFont(&m_fntH);

  }

  void WndFileList::OnUpdateDirAdd(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
  void WndFileList::OnUpdateDirDel(CCmdUI* pCmdUI) { pCmdUI->Enable(m_wndList.GetFirstSelectedItemPosition() ? TRUE : FALSE); }
  void WndFileList::OnUpdateDirExecute(CCmdUI* pCmdUI) { pCmdUI->Enable(m_wndList.GetItemCount() ? TRUE : FALSE); }

  void WndFileList::DirAdd(const std::filesystem::directory_entry& d)
  {
    m_wndList.InsertItem(m_wndList.GetItemCount(), d.path().c_str());
  }

  void WndFileList::DirDelSelected(DList& dl)
  {
    while (POSITION pos = m_wndList.GetFirstSelectedItemPosition())
    {
      int i = m_wndList.GetNextSelectedItem(pos);
      m_wndList.DeleteItem(i);
      dl.erase(dl.begin() + i);
    }
  }

  void WndFileList::DirSetTo(DList& dl)
  {
    m_wndList.DeleteAllItems();
    for (auto& d : dl) DirAdd(d);
  }

}
