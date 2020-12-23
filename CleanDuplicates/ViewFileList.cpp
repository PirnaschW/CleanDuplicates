#include "pch.h"

namespace MyViews
{

  IMPLEMENT_DYNCREATE(ViewFileList, CView)
  BEGIN_MESSAGE_MAP(ViewFileList, CView)
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &ViewFileList::OnFilePrintPreview)
    ON_UPDATE_COMMAND_UI(ID_LIST_SORT, OnUpdateFileSort)
    ON_UPDATE_COMMAND_UI(ID_LIST_DUPL, OnUpdateFileDupl)
    ON_UPDATE_COMMAND_UI(ID_LIST_MARK, OnUpdateFileMark)
    ON_UPDATE_COMMAND_UI(ID_LIST_DEL, OnUpdateFileDel)
    ON_COMMAND(ID_LIST_SORT, OnFileSort)
    ON_COMMAND(ID_LIST_DUPL, OnFileDupl)
    ON_COMMAND(ID_LIST_MARK, OnFileMark)
    ON_COMMAND(ID_LIST_DEL, OnFileDel)
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
  END_MESSAGE_MAP()


  BOOL ViewFileList::PreCreateWindow(CREATESTRUCT& cs)
  {
    cs.style |= LVS_REPORT;
    return CView::PreCreateWindow(cs);
  }

  void ViewFileList::OnInitialUpdate()
  {
    CView::OnInitialUpdate();

    CRect r;
    GetClientRect(r);

    // create ToolBarCtrl, define style and buttons
    m_ToolBar.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, r.Width(), 24), this, IDR_FILELIST);
    
    // need to replace the original ImageList with a custom built, as the standard is limited to 16 colors
    HBITMAP hBitmap = (HBITMAP) ::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_FILELIST), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
    CBitmap bm;
    bm.Attach(hBitmap);
    m_ImageList.Create(16, 15, ILC_COLOR8, 4, 4);
    m_ImageList.Add(&bm, (CBitmap*) NULL);
    m_ToolBar.SetImageList(&m_ImageList);

    constexpr TBBUTTON b[]{
      { 0, ID_LIST_SORT, TBSTATE_ENABLED, BTNS_BUTTON, {0,0}, NULL, -1 },
      { 1, ID_LIST_DUPL, TBSTATE_ENABLED, BTNS_BUTTON, {0,0}, NULL, -1 },
      { 2, ID_LIST_MARK, TBSTATE_ENABLED, BTNS_BUTTON, {0,0}, NULL, -1 },
      { 3, ID_LIST_DEL,  TBSTATE_ENABLED, BTNS_BUTTON, {0,0}, NULL, -1 },
    };
    m_ToolBar.AddButtons(sizeof b / sizeof b[0], const_cast<TBBUTTON*>(b));

    // create ListCtrl, define style and columns
    m_List.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, CRect(0, 24, r.Width(), r.Height()-24), this, IDC_FILELIST);
    m_List.SetExtendedStyle(m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    m_List.InsertColumn(0, _T("Path"), LVCFMT_LEFT, 400);
    m_List.InsertColumn(1, _T("Filename"), LVCFMT_LEFT, 150);
    m_List.InsertColumn(2, _T("Size"), LVCFMT_RIGHT, 80);
    m_List.InsertColumn(3, _T("MD5 Hash"), LVCFMT_CENTER, 100);
    m_List.InsertColumn(4, _T("Duplicate"), LVCFMT_CENTER, 80);
    SetHeaderFont();

    // tell the document about the list control, so it can directly fill it
    assert(GetDocument()->pFileList == nullptr);
    GetDocument()->pFileList = &m_List;

    // if this is a document loaded from file, we need to update the control with the data - Serialization doesn't do that
    GetDocument()->ListRebuild();     // add data to List Ctrl
  }

  void ViewFileList::OnDraw(CDC* pDC)
  {
  }

  void ViewFileList::OnFilePrintPreview() { AFXPrintPreview(this); }
  BOOL ViewFileList::OnPreparePrinting(CPrintInfo* pInfo) { return DoPreparePrinting(pInfo); }
  void ViewFileList::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  void ViewFileList::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {}

  void ViewFileList::OnSize(UINT nType, int cx, int cy)
  {
    if (nType != SIZE_RESTORED || m_ToolBar.m_hWnd == nullptr || m_List.m_hWnd == nullptr) return;
    CRect r;
    GetClientRect(r);
    DWORD dw = m_ToolBar.GetButtonSize();
    m_ToolBar.SetWindowPos(nullptr, 0, 0, r.Width(), HIWORD(dw) + 2, SWP_NOACTIVATE | SWP_NOZORDER);
    m_List.SetWindowPos(nullptr, 0, HIWORD(dw) + 2, r.Width(), r.Height() - HIWORD(dw) - 2, SWP_NOACTIVATE | SWP_NOZORDER);
  }

  void ViewFileList::OnRButtonUp(UINT nFlags, CPoint point)
  {
    ClientToScreen(&point);
    OnContextMenu(this, point);
  }

  void ViewFileList::OnContextMenu(CWnd* pWnd, CPoint point)
  {
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_FILELIST, point.x, point.y, this, TRUE);
  }

  void ViewFileList::SetHeaderFont()
  {
    ::DeleteObject(m_fntH.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info{};
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);
    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight * 2;
    lf.lfItalic = info.lfMenuFont.lfItalic;
    m_fntH.CreateFontIndirect(&lf);
    m_List.GetHeaderCtrl()->SetFont(&m_fntH);
  }


  void ViewFileList::OnUpdateFileSort(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
  void ViewFileList::OnUpdateFileDupl(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
  void ViewFileList::OnUpdateFileMark(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
  void ViewFileList::OnUpdateFileDel(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }

  void ViewFileList::OnFileSort() {}
  void ViewFileList::OnFileDupl() {}
  void ViewFileList::OnFileMark() {}
  void ViewFileList::OnFileDel()  {}

}
