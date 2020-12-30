
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
  static const std::wstring GetText(size_t n);
  static int __stdcall CompareFunc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort);

public:
  std::vector<std::wstring> dirlist_{};
  CTreeCtrl* pFileTree{ nullptr };
  CListCtrl* pFileList{ nullptr };
  CToolBarCtrl* pToolBar{ nullptr };
  FileMap::FMap fmap_{};
  bool duplicatesOnly{ false };
  bool checkDuplicates{ false };
  size_t sortBy{ 0 };

protected:
  // Generated message map functions
  // add functions that modify the document content in the CDoc
  // add functions that modify the view's display in the CView
  afx_msg void OnTreeSelChanged(NMHDR*, LRESULT*);
  afx_msg void OnUpdateFileSortPath(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileSortSize(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileDupl(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileMark(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileMove(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileDel(CCmdUI* pCmdUI);
  afx_msg void OnFileSort();
  afx_msg void OnFileDupl();
  afx_msg void OnFileMark();
  afx_msg void OnFileMove();
  afx_msg void OnFileDel();
  DECLARE_MESSAGE_MAP()

};
