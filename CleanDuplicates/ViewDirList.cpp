#include "pch.h"

namespace MyViews
{

  IMPLEMENT_DYNCREATE(ViewDirList, CListView)
  BEGIN_MESSAGE_MAP(ViewDirList, CListView)
    ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &ViewDirList::OnFilePrintPreview)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_UPDATE_COMMAND_UI(ID_DIR_ADD, OnUpdateDirAdd)
    ON_UPDATE_COMMAND_UI(ID_DIR_DEL, OnUpdateDirDel)
    ON_UPDATE_COMMAND_UI(ID_DIR_EXECUTE, OnUpdateDirExecute)
  END_MESSAGE_MAP()


  BOOL ViewDirList::PreCreateWindow(CREATESTRUCT& cs)
  {
    cs.style |= LVS_REPORT;
    return CListView::PreCreateWindow(cs);
  }

  void ViewDirList::OnInitialUpdate() {
    CListView::OnInitialUpdate();

    GetDocument()->pDirList = &GetListCtrl();
    // define style and columns
    GetListCtrl().SetExtendedStyle(GetListCtrl().GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    GetListCtrl().InsertColumn(0, _T("Starting Path"), LVCFMT_LEFT, 500);
    GetListCtrl().InsertColumn(1, _T("# of Files"), LVCFMT_RIGHT, 100);
    GetListCtrl().InsertColumn(2, _T("# of Duplicates"), LVCFMT_RIGHT, 100);
    SetHeaderFont();
  }

  void ViewDirList::OnDraw(CDC* pDC)
  {
    CCleanDuplicatesDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
  }

  void ViewDirList::OnFilePrintPreview() { AFXPrintPreview(this); }
  BOOL ViewDirList::OnPreparePrinting(CPrintInfo* pInfo) { return DoPreparePrinting(pInfo); }
  void ViewDirList::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  void ViewDirList::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  
  void ViewDirList::OnRButtonUp(UINT nFlags, CPoint point)
  {
    ClientToScreen(&point);
    OnContextMenu(this, point);
  }

  void ViewDirList::OnContextMenu(CWnd* pWnd, CPoint point)
  {
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_DIRLIST_CONTEXT, point.x, point.y, this, TRUE);
  }

  void ViewDirList::SetHeaderFont()
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
    GetListCtrl().GetHeaderCtrl()->SetFont(&m_fntH);
  }

  void ViewDirList::OnUpdateDirAdd(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
  void ViewDirList::OnUpdateDirDel(CCmdUI* pCmdUI) { pCmdUI->Enable(GetListCtrl().GetFirstSelectedItemPosition() ? TRUE : FALSE); }
  void ViewDirList::OnUpdateDirExecute(CCmdUI* pCmdUI) { pCmdUI->Enable(GetListCtrl().GetItemCount() ? TRUE : FALSE); }
}
