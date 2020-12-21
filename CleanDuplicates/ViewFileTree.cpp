#include "pch.h"

namespace MyViews
{

  IMPLEMENT_DYNCREATE(ViewFileTree, CTreeView)
  BEGIN_MESSAGE_MAP(ViewFileTree, CTreeView)
    ON_COMMAND(ID_FILE_PRINT, &CTreeView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CTreeView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &ViewFileTree::OnFilePrintPreview)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
  END_MESSAGE_MAP()


  BOOL ViewFileTree::PreCreateWindow(CREATESTRUCT& cs)
  {
    cs.style |= TVS_FULLROWSELECT | TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
    //TVS_SHOWSELALWAYS | TVS_TRACKSELECT | 
    return CTreeView::PreCreateWindow(cs);
  }

  void ViewFileTree::OnInitialUpdate()
  {
    CTreeView::OnInitialUpdate();

    // define tree root
    HTREEITEM hRoot = GetTreeCtrl().InsertItem(_T("File Tree"), 0, 0);
    GetTreeCtrl().SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
    GetTreeCtrl().Expand(hRoot, TVE_EXPAND);

    // tell the document about the tree contorl, so it can directly fill it
    assert(GetDocument()->pFileTree == nullptr);
    GetDocument()->pFileTree = &GetTreeCtrl();

    // if this is a document loaded from file, we need to update the control with the data - Serialization doesn't do that
    for (size_t i = 0; i < GetDocument()->dirlist_.size(); ++i)
      GetDocument()->TreeAdd(GetDocument()->dirlist_[i].c_str());     // add to Tree Ctrl
  }

  void ViewFileTree::OnFilePrintPreview() { AFXPrintPreview(this); }
  BOOL ViewFileTree::OnPreparePrinting(CPrintInfo* pInfo) { return DoPreparePrinting(pInfo); }
  void ViewFileTree::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  void ViewFileTree::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  
  void ViewFileTree::OnRButtonUp(UINT nFlags, CPoint point)
  {
    ClientToScreen(&point);
    OnContextMenu(this, point);
  }

  void ViewFileTree::OnContextMenu(CWnd* pWnd, CPoint point)
  {
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
  }

}
