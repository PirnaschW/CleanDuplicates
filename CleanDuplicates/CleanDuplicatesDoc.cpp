
#include "pch.h"

IMPLEMENT_DYNCREATE(CCleanDuplicatesDoc, CDocument)
BEGIN_MESSAGE_MAP(CCleanDuplicatesDoc, CDocument)
  ON_COMMAND(ID_DIR_ADD, OnDirAdd)
  ON_COMMAND(ID_DIR_DEL, OnDirDel)
  ON_COMMAND(ID_DIR_EXECUTE, OnDirExecute)
  ON_NOTIFY(TVN_SELCHANGED, ID_VIEW_FILETREE, OnTreeSelChanged)
END_MESSAGE_MAP()

BOOL CCleanDuplicatesDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument()) return FALSE;

  // bind CDocument's function to allow updating windows while working - saved at App to be callable from everywhere
  theApp.SetUpdateCallBack(std::bind(&CCleanDuplicatesDoc::UpdateAllViewsNow, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  return TRUE;
}

void CCleanDuplicatesDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
  // TODO: add storing code here
  }
  else
  {
  // TODO: add loading code here
  }
}


void CCleanDuplicatesDoc::UpdateAllViewsNow(CView* pSender, LPARAM lHint, CObject* pHint)
{
  UpdateAllViews(pSender, lHint, pHint);
  CView* v{ nullptr };
  for (POSITION p = GetFirstViewPosition(); (v = GetNextView(p)) != nullptr; )
    v->UpdateWindow();
  //GetMainFrame()->GetFileTree().Invalidate();
  //GetMainFrame()->GetFileTree().UpdateWindow();
  //GetMainFrame()->GetFileTree().m_wndTree.Invalidate();
  //GetMainFrame()->GetFileTree().m_wndTree.UpdateWindow();
  //GetMainFrame()->GetFileList().Invalidate();
  //GetMainFrame()->GetFileList().UpdateWindow();
}

void CCleanDuplicatesDoc::OnDirAdd()
{
  CFolderPickerDialog dlg{};
  dlg.m_ofn.lpstrTitle = _T("Select Path");
  dlg.m_ofn.lpstrInitialDir = _T("D:\\Projects\\");
  if (dlg.DoModal() == IDOK)
  {
    std::filesystem::directory_entry d(dlg.GetPathName().GetString());
    dlist_.push_back(d);

    pDirList->InsertItem(pDirList->GetItemCount(), d.path().c_str());

    assert(pDirList->GetItemCount() == dlist_.size());
    UpdateAllViews(nullptr);
  }
}

void CCleanDuplicatesDoc::OnDirDel()
{
  while (POSITION pos = pDirList->GetFirstSelectedItemPosition())
  {
    int i = pDirList->GetNextSelectedItem(pos);
    pDirList->DeleteItem(i);
    dlist_.erase(dlist_.begin() + i);
  }
  assert(pDirList->GetItemCount() == dlist_.size());
  UpdateAllViews(nullptr);
}

void CCleanDuplicatesDoc::OnDirExecute()
{
  //fmap_ = GetMainFrame()->GetFileTree().FillFileTree(dlist_);
  //GetMainFrame()->GetFileList().FillList(fmap_);
}

void CCleanDuplicatesDoc::OnTreeSelChanged(NMHDR* n, LRESULT* l)
{
  LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) n;
  switch (pnmtv->action)
  {
    case TVC_BYKEYBOARD:
      break;
    case TVC_BYMOUSE:
      break;
    default:
      break;
  }
}
