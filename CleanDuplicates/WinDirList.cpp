#include "pch.h"

namespace MyWin
{

  WndDirList::WndDirList() noexcept {}
  WndDirList::~WndDirList() {}

  BEGIN_MESSAGE_MAP(WndDirList, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_DIR_ADD, OnDirAdd)
    ON_COMMAND(ID_DIR_DEL, OnDirDel)
    ON_COMMAND(ID_DIR_EXECUTE, OnDirExecute)
    ON_UPDATE_COMMAND_UI(ID_DIR_ADD, OnUpdateDirAdd)
    ON_UPDATE_COMMAND_UI(ID_DIR_DEL, OnUpdateDirDel)
    ON_UPDATE_COMMAND_UI(ID_DIR_EXECUTE, OnUpdateDirExecute)
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // CResourceViewBar message handlers

  void WndDirList::AdjustLayout()
  {
    if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
    {
      return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
    m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndList.SetWindowPos(nullptr, rectClient.left, rectClient.top  + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
  }

  int WndDirList::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
    if (CDockablePane::OnCreate(lpCreateStruct) == -1) return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();
    if (!m_wndList.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rectDummy, this, 2))
      {
      TRACE0("Failed to create Directory List\n");
      return -1; // fail to create
    }
    m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    InitList();

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDB_DIRLIST);
    m_wndToolBar.LoadToolBar(IDB_DIRLIST, 0, 0, TRUE /* Is locked */);
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_DIRLIST_HC : IDB_DIRLIST, 0, 0, TRUE /* Locked */);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // All commands will be routed via this control, not via the parent frame:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();
    return 0;
  }

  void WndDirList::OnSize(UINT nType, int cx, int cy)
  {
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
  }

  void WndDirList::InitList()
  {
    m_wndList.InsertColumn(0, _T("Starting Path"), LVCFMT_LEFT, 500);
    m_wndList.InsertColumn(1, _T("# of Files"), LVCFMT_RIGHT, 100);
    m_wndList.InsertColumn(2, _T("# of Duplicates"), LVCFMT_RIGHT, 100);
    m_wndList.InsertItem(1, L"First Line");
    m_wndList.InsertItem(2, L"Second Line");
    m_wndList.InsertItem(3, L"Third Line");

    SetListFont();
  }

  void WndDirList::OnSetFocus(CWnd* pOldWnd)
  {
    CDockablePane::OnSetFocus(pOldWnd);
    m_wndList.SetFocus();
  }

  void WndDirList::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
  {
    CDockablePane::OnSettingChange(uFlags, lpszSection);
    SetListFont();
  }

  void WndDirList::SetListFont()
  {
    ::DeleteObject(m_fntList.Detach());
    ::DeleteObject(m_fntListH.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info;
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);
    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;
    m_fntList.CreateFontIndirect(&lf);
    m_wndList.SetFont(&m_fntList);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight*2;
    lf.lfItalic = info.lfMenuFont.lfItalic;
    m_fntListH.CreateFontIndirect(&lf);
    m_wndList.GetHeaderCtrl()->SetFont(&m_fntListH);

  }

  void WndDirList::OnDirAdd() {}
  void WndDirList::OnDirDel() {}
  void WndDirList::OnDirExecute() {}

}