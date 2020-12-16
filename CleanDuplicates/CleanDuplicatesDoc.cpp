
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
    pDirList->InsertItem(pDirList->GetItemCount(), d.path().c_str());    // add to Directory List
    FillFileTree(d);                                                     // recursively add all sub directories to File Tree

    UpdateAllViews(nullptr);
  }
}

void CCleanDuplicatesDoc::OnDirDel()
{
  while (POSITION pos = pDirList->GetFirstSelectedItemPosition())
  {
    int i = pDirList->GetNextSelectedItem(pos);
    pDirList->DeleteItem(i);
 //   dlist_.erase(dlist_.begin() + i);
  }
//  assert(pDirList->GetItemCount() == dlist_.size());
  UpdateAllViews(nullptr);
}


void CCleanDuplicatesDoc::FillFileTree(const std::filesystem::directory_entry& d)
{
//// clear everything out - NO!
//  files_.clear();
//  m_wndTree.DeleteAllItems();

  // collect all files for each starting directory
  HTREEITEM h = pFileTree->InsertItem(d.path().c_str(), 0, 0, pFileTree->GetRootItem());
  pFileTree->Expand(pFileTree->GetRootItem(), TVE_EXPAND);
  CollectFiles(d, h);

// only when all files are collected can the List be populated, as the 'Unique' qualifiers change while more files are read
  for (const auto& it : fmap_)
  {
    auto z = pFileList->InsertItem(pFileList->GetItemCount(), it.second.d.path().parent_path().wstring().c_str());
    pFileList->SetItemText(z, 1, it.second.d.path().filename().wstring().c_str());
    static wchar_t buffer[32];
    _ui64tow_s(it.first.size, buffer, 32, 10);
    pFileList->SetItemText(z, 2, buffer);
    pFileList->SetItemText(z, 3, it.first.hash.c_str());
    pFileList->SetItemText(z, 4, fmap_.count(it.first) > 1 ? L"Duplicate" : L"Unique");
  }

}


void CCleanDuplicatesDoc::CollectFiles(const std::filesystem::directory_entry& s, HTREEITEM hScope)
{
  for (auto& d : std::filesystem::directory_iterator(s, std::filesystem::directory_options::skip_permission_denied))
  {

    if (d.is_regular_file())  // collect data, but don't enter in tree
    {
      FileMap::FMap::const_iterator it = FileMap::Insert(fmap_, d);
      if (fmap_.size() % 16 == 0) UpdateAllViewsNow(nullptr);  // refresh screen NOW
    }

    if (d.is_directory()) // enter in tree, but don't collect the directory's data
    {
      HTREEITEM h = pFileTree->InsertItem(d.path().filename().wstring().c_str(), 0, 0, hScope);
      pFileTree->Expand(hScope, TVE_EXPAND);
      CollectFiles(d, h);
    }
  }
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
