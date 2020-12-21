
#include "pch.h"

IMPLEMENT_DYNCREATE(CCleanDuplicatesDoc, CDocument)
BEGIN_MESSAGE_MAP(CCleanDuplicatesDoc, CDocument)
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
    ar << dirlist_;
  }
  else
  {
    ar >> dirlist_;
  }
}


void CCleanDuplicatesDoc::UpdateAllViewsNow(CView* pSender, LPARAM lHint, CObject* pHint)
{
  UpdateAllViews(pSender, lHint, pHint);
  CView* v{ nullptr };
  for (POSITION p = GetFirstViewPosition(); (v = GetNextView(p)) != nullptr; )
    v->UpdateWindow();
}


bool CCleanDuplicatesDoc::DirAdd(const std::wstring& s)
{
  if (std::find(dirlist_.cbegin(), dirlist_.cend(), s) != dirlist_.cend())
  {
    AfxMessageBox(L"already in the list!");
    return false;
  }
  dirlist_.push_back(s);
  return true;
}

bool CCleanDuplicatesDoc::DirDel(const std::wstring& s)
{
  auto it = std::find(dirlist_.cbegin(), dirlist_.cend(), s);
  if (it == dirlist_.cend())
  {
    AfxMessageBox(L"not in the list!");
    return false;
  }
  dirlist_.erase(it);
  return true;
}

void CCleanDuplicatesDoc::TreeAdd(const std::wstring& s)
{
  // add a new main item, and collect all files below the the new starting directory
  assert(pFileTree != nullptr);
  HTREEITEM h = pFileTree->InsertItem(s.c_str(), 0, 0, pFileTree->GetRootItem());
  pFileTree->Expand(pFileTree->GetRootItem(), TVE_EXPAND);
  CWaitCursor CouldTakeAWhile;
  CollectFiles(std::filesystem::directory_entry(s), h);
}

void CCleanDuplicatesDoc::TreeDel(const std::wstring& s)
{
  // remove starting directory and all below
  assert(pFileTree != nullptr);

  HTREEITEM hRoot = pFileTree->GetRootItem();
  for (HTREEITEM hItem = pFileTree->GetChildItem(hRoot); hItem != NULL; hItem = pFileTree->GetNextSiblingItem(hItem))
  {
    // check whether the current item is the searched one
    if (pFileTree->GetItemText(hItem).GetString() == s)
      pFileTree->DeleteItem(hItem);
  }

  size_t len = s.length();
  for (FileMap::FMap::iterator it = fmap_.begin(); it != fmap_.end();)
  {
    if (it->second.d.path().wstring().substr(0,len) == s)
      it = fmap_.erase(it);
    else it++;
  }
}

// recursive function to collect content of one directory (calls itself for all sub-directories)
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

void CCleanDuplicatesDoc::ListRebuild()
{
  pFileList->DeleteAllItems();
  for (const auto& it : fmap_)
  {
    size_t n = fmap_.count(it.first);
    if (n == 1) continue; // don't show unique files
    auto z = pFileList->InsertItem(pFileList->GetItemCount(), it.second.d.path().parent_path().wstring().c_str());
    pFileList->SetItemText(z, 1, it.second.d.path().filename().wstring().c_str());
    static wchar_t buffer[32];
    _ui64tow_s(it.first.size, buffer, 32, 10);
    pFileList->SetItemText(z, 2, buffer);
    pFileList->SetItemText(z, 3, it.first.hash.c_str());
    pFileList->SetItemText(z, 4, n == 2 ? L"Duplicate" : L"Multiples");
  }
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

