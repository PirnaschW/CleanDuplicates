#include "pch.h"

namespace MyViews
{

  IMPLEMENT_DYNCREATE(ViewFileList0, CListView)
  BEGIN_MESSAGE_MAP(ViewFileList0, CListView)
    ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &ViewFileList0::OnFilePrintPreview)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
  END_MESSAGE_MAP()


  BOOL ViewFileList0::PreCreateWindow(CREATESTRUCT& cs)
  {


    cs.style |= LVS_REPORT;
    return CListView::PreCreateWindow(cs);
  }

  void ViewFileList0::OnInitialUpdate()
  {
    CListView::OnInitialUpdate();

    // define style and columns
    GetListCtrl().SetExtendedStyle(GetListCtrl().GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    GetListCtrl().InsertColumn(0, _T("Path"), LVCFMT_LEFT, 400);
    GetListCtrl().InsertColumn(1, _T("Filename"), LVCFMT_LEFT, 150);
    GetListCtrl().InsertColumn(2, _T("Size"), LVCFMT_RIGHT, 80);
    GetListCtrl().InsertColumn(3, _T("MD5 Hash"), LVCFMT_CENTER, 100);
    GetListCtrl().InsertColumn(4, _T("Duplicate"), LVCFMT_CENTER, 80);
    SetHeaderFont();

    // tell the document about the list control, so it can directly fill it
    assert(GetDocument()->pFileList == nullptr);
    GetDocument()->pFileList = &GetListCtrl();

    // if this is a document loaded from file, we need to update the control with the data - Serialization doesn't do that
    GetDocument()->ListRebuild();     // add data to List Ctrl

  }

  void ViewFileList0::OnFilePrintPreview() { AFXPrintPreview(this); }
  BOOL ViewFileList0::OnPreparePrinting(CPrintInfo* pInfo) { return DoPreparePrinting(pInfo); }
  void ViewFileList0::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  void ViewFileList0::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {}
  
  void ViewFileList0::OnRButtonUp(UINT nFlags, CPoint point)
  {
    ClientToScreen(&point);
    OnContextMenu(this, point);
  }

  void ViewFileList0::OnContextMenu(CWnd* pWnd, CPoint point)
  {
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
  }

  void ViewFileList0::SetHeaderFont()
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

  void ViewFileList0::OnUpdateDirAdd(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
  void ViewFileList0::OnUpdateDirDel(CCmdUI* pCmdUI) { pCmdUI->Enable(GetListCtrl().GetFirstSelectedItemPosition() ? TRUE : FALSE); }
  void ViewFileList0::OnUpdateDirExecute(CCmdUI* pCmdUI) { pCmdUI->Enable(GetListCtrl().GetItemCount() ? TRUE : FALSE); }

}
