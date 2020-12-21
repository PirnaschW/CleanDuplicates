#include "pch.h"

namespace MyViews
{

  IMPLEMENT_DYNCREATE(ViewDirList, CListView)
  BEGIN_MESSAGE_MAP(ViewDirList, CListView)
    ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &ViewDirList::OnFilePrintPreview)
    ON_UPDATE_COMMAND_UI(ID_DIR_ADD, OnUpdateDirAdd)
    ON_UPDATE_COMMAND_UI(ID_DIR_DEL, OnUpdateDirDel)
    ON_UPDATE_COMMAND_UI(ID_DIR_DELALL, OnUpdateDirDelAll)
    ON_COMMAND(ID_DIR_ADD, OnDirAdd)
    ON_COMMAND(ID_DIR_DEL, OnDirDel)
    ON_COMMAND(ID_DIR_DELALL, OnDirDelAll)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
  END_MESSAGE_MAP()


  BOOL ViewDirList::PreCreateWindow(CREATESTRUCT& cs)
  {
    cs.style |= LVS_REPORT;
    return CListView::PreCreateWindow(cs);
  }

  void ViewDirList::OnInitialUpdate()
  {
    CListView::OnInitialUpdate();

    // define style and columns
    GetListCtrl().SetExtendedStyle(GetListCtrl().GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    GetListCtrl().InsertColumn(0, _T("Starting Path"), LVCFMT_LEFT, 500);
    GetListCtrl().InsertColumn(1, _T("# of Files"), LVCFMT_RIGHT, 100);
    GetListCtrl().InsertColumn(2, _T("# of Duplicates"), LVCFMT_RIGHT, 100);
    SetHeaderFont();

    // if this is a document loaded from file, we need to update the control with the data - Serialization doesn't do that
    for (size_t i = 0; i < GetDocument()->dirlist_.size(); ++i)
      GetListCtrl().InsertItem(i, GetDocument()->dirlist_[i].c_str());         // add to List Ctrl
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
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_DIRLIST, point.x, point.y, this, TRUE);
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

  // can always ADD a dir
  void ViewDirList::OnUpdateDirAdd(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
  // add a DIR
  void ViewDirList::OnDirAdd()
  {
    CFolderPickerDialog dlg{};
    dlg.m_ofn.lpstrTitle = _T("Select Path");
    dlg.m_ofn.lpstrInitialDir = _T("D:\\Projects\\");
    if (dlg.DoModal() == IDOK)
    {
      const std::wstring s = dlg.GetPathName().GetString();
      if (GetDocument()->DirAdd(s))                                            // add to Document's data
      {
        GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), s.c_str());     // add to List Ctrl
        GetDocument()->TreeAdd(s);                                             // add to File Tree
        GetDocument()->ListRebuild();                                          // rebuild File List
      }
    }
  }

  // can only DEL if there is a selection
  void ViewDirList::OnUpdateDirDel(CCmdUI* pCmdUI) { pCmdUI->Enable(GetListCtrl().GetFirstSelectedItemPosition() ? TRUE : FALSE); }
  // DEL selected dir(s)
  void ViewDirList::OnDirDel()
  {
    while (POSITION pos = GetListCtrl().GetFirstSelectedItemPosition())        // loop for all selected items (can delete multiples!)
    {
      int i = GetListCtrl().GetNextSelectedItem(pos);
      std::wstring s = GetListCtrl().GetItemText(i,0).GetString();
      if (GetDocument()->DirDel(s))                                            // delete from Document's data
      {
        GetListCtrl().DeleteItem(i);                                           // delete from ListCtrl
        GetDocument()->TreeDel(s);                                             // delete from File Tree
        GetDocument()->ListRebuild();                                          // rebuild File List
      }
    }
  }

  // can only DELETE ALL if there are any
  void ViewDirList::OnUpdateDirDelAll(CCmdUI* pCmdUI) { pCmdUI->Enable(GetListCtrl().GetItemCount() > 0 ? TRUE : FALSE); }
  // DEL ALL directories
  void ViewDirList::OnDirDelAll()
  {
    for (int i = 0; i < GetListCtrl().GetItemCount(); ++i)                     // loop for all items
    {
      std::wstring s = GetListCtrl().GetItemText(i, 0).GetString();
      if (!GetDocument()->DirDel(s)) throw;                                    // delete from Document's data
    }
    GetListCtrl().DeleteAllItems();                                            // delete all from ListCtrl
    GetDocument()->pFileTree->DeleteAllItems();                                // delete all from File Tree
    GetDocument()->ListRebuild();                                              // rebuild File List
  }

}
