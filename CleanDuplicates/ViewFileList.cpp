#include "pch.h"

namespace MyViews
{

  IMPLEMENT_DYNCREATE(ViewFileList, CListView)
  BEGIN_MESSAGE_MAP(ViewFileList, CListView)
    ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &ViewFileList::OnFilePrintPreview)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_UPDATE_COMMAND_UI(ID_DIR_ADD, OnUpdateDirAdd)
    ON_UPDATE_COMMAND_UI(ID_DIR_DEL, OnUpdateDirDel)
    ON_UPDATE_COMMAND_UI(ID_DIR_EXECUTE, OnUpdateDirExecute)
  END_MESSAGE_MAP()


  BOOL ViewFileList::PreCreateWindow(CREATESTRUCT& cs)
  {
    cs.style |= LVS_REPORT;
    return CListView::PreCreateWindow(cs);
  }

  void ViewFileList::OnInitialUpdate() {
    CListView::OnInitialUpdate();
    GetDocument()->pFileList = &GetListCtrl();

    // define style and columns
    GetListCtrl().SetExtendedStyle(GetListCtrl().GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    GetListCtrl().InsertColumn(0, _T("Path"), LVCFMT_LEFT, 400);
    GetListCtrl().InsertColumn(1, _T("Filename"), LVCFMT_RIGHT, 150);

    // temporary: Add a dummy line
    GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), L"Test Entry File List");
  }

  void ViewFileList::OnDraw(CDC* pDC)
  {
    CCleanDuplicatesDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
  }

  void ViewFileList::OnFilePrintPreview() { AFXPrintPreview(this); }
  BOOL ViewFileList::OnPreparePrinting(CPrintInfo* pInfo) { return DoPreparePrinting(pInfo); }
  void ViewFileList::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  void ViewFileList::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  
  void ViewFileList::OnRButtonUp(UINT nFlags, CPoint point)
  {
    ClientToScreen(&point);
    OnContextMenu(this, point);
  }

  void ViewFileList::OnContextMenu(CWnd* pWnd, CPoint point)
  {
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
  }

  void ViewFileList::OnUpdateDirAdd(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
  void ViewFileList::OnUpdateDirDel(CCmdUI* pCmdUI) { pCmdUI->Enable(GetListCtrl().GetFirstSelectedItemPosition() ? TRUE : FALSE); }
  void ViewFileList::OnUpdateDirExecute(CCmdUI* pCmdUI) { pCmdUI->Enable(GetListCtrl().GetItemCount() ? TRUE : FALSE); }

}