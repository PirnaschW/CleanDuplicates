
class CCleanDuplicatesDoc : public CDocument
{
protected: // create from serialization only
  CCleanDuplicatesDoc() noexcept {};
  DECLARE_DYNCREATE(CCleanDuplicatesDoc)
public:
  virtual ~CCleanDuplicatesDoc() {};
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);

public:
  void UpdateAllViewsNow(CView* pSender = nullptr, LPARAM lHint = 0L, CObject* pHint = nullptr);  // function to update windows while working
  bool DirAdd(const std::wstring& s);
  bool DirDel(const std::wstring& s);
  void TreeAdd(const std::wstring& s);
  void TreeDel(const std::wstring& s);
  void ListRebuild();

private:
  void CollectFiles(const std::filesystem::directory_entry& s, HTREEITEM hScope);

public:
  std::vector<std::wstring> dirlist_{};
  CTreeCtrl* pFileTree{ nullptr };
  CListCtrl* pFileList{ nullptr };
  FileMap::FMap fmap_{};


protected:
  // Generated message map functions
  // add functions that modify the document content in the CDoc
  // add functions that modify the view's display in the CView
  afx_msg void OnTreeSelChanged(NMHDR*, LRESULT*);
  DECLARE_MESSAGE_MAP()

};
