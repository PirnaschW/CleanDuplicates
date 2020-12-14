
#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCleanDuplicatesDoc
IMPLEMENT_DYNCREATE(CCleanDuplicatesDoc, CDocument)

BEGIN_MESSAGE_MAP(CCleanDuplicatesDoc, CDocument)
  ON_COMMAND(ID_DIR_ADD, OnDirAdd)
  ON_COMMAND(ID_DIR_DEL, OnDirDel)
  ON_COMMAND(ID_DIR_EXECUTE, OnDirExecute)
  ON_NOTIFY(TVN_SELCHANGED, ID_VIEW_FILETREE, OnTreeSelChanged)

END_MESSAGE_MAP()


CCleanDuplicatesDoc::CCleanDuplicatesDoc() noexcept
{
 // TODO: add one-time construction code here

}

CCleanDuplicatesDoc::~CCleanDuplicatesDoc()
{
}

BOOL CCleanDuplicatesDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

  return TRUE;
}


CMainFrame* CCleanDuplicatesDoc::GetMainFrame()
{
  CMainFrame* p = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
  if (p && p->GetSafeHwnd()) return p;
  else throw;
}


// CCleanDuplicatesDoc serialization

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

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CCleanDuplicatesDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
 // Modify this code to draw the document's data
  dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

  CString strText = _T("TODO: implement thumbnail drawing here");
  LOGFONT lf;

  CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
  pDefaultGUIFont->GetLogFont(&lf);
  lf.lfHeight = 36;

  CFont fontDraw;
  fontDraw.CreateFontIndirect(&lf);

  CFont* pOldFont = dc.SelectObject(&fontDraw);
  dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
  dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCleanDuplicatesDoc::InitializeSearchContent()
{
  CString strSearchContent;
  // Set search contents from document's data.
  // The content parts should be separated by ";"

  // For example: strSearchContent = _T("point;rectangle;circle;ole object;");
  SetSearchContent(strSearchContent);
}

void CCleanDuplicatesDoc::SetSearchContent(const CString& value)
{
  if (value.IsEmpty())
  {
    RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
  }
  else
  {
    CMFCFilterChunkValueImpl* pChunk = nullptr;
    ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
    if (pChunk != nullptr)
    {
      pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
      SetChunkValue(pChunk);
    }
  }
}

#endif // SHARED_HANDLERS

// CCleanDuplicatesDoc diagnostics

#ifdef _DEBUG
void CCleanDuplicatesDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CCleanDuplicatesDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG


// CCleanDuplicatesDoc commands
void CCleanDuplicatesDoc::OnDirAdd()
{
  CFolderPickerDialog dlg{};
  dlg.m_ofn.lpstrTitle = _T("Select Path");
  dlg.m_ofn.lpstrInitialDir = _T("D:\\Projects\\");
  if (dlg.DoModal() == IDOK)
  {
    std::filesystem::directory_entry d(dlg.GetPathName().GetString());
    dlist_.push_back(d);
    GetMainFrame()->GetDirList().DirAdd(d);
  }
}

void CCleanDuplicatesDoc::OnDirDel() { GetMainFrame()->GetDirList().DirDelSelected(dlist_); }

void CCleanDuplicatesDoc::OnDirExecute()
{
  fmap_ = GetMainFrame()->GetFileTree().FillFileTree(dlist_);
}

void CCleanDuplicatesDoc::OnTreeSelChanged(NMHDR* n, LRESULT* l)
{

}
