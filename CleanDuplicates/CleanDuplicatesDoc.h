
constexpr char HashExtension[]{ ".md5" };

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
  FileMap::FMap::iterator GetPrimary(const FileMap::FileKey& fk);

public:
  std::vector<std::wstring> dirlist_{};
  CTreeCtrl* pFileTree{ nullptr };
  CListCtrl* pFileList{ nullptr };
  CToolBarCtrl* pToolBar{ nullptr };
  FileMap::FMap fmap_{};
  bool duplicatesOnly{ false };
  bool checkDuplicates{ false };
  bool saveHash{ true };
  size_t sortBy{ 0 };

protected:
  // Generated message map functions
  // add functions that modify the document content in the CDoc
  // add functions that modify the view's display in the CView
  afx_msg void OnTreeSelChanged(NMHDR*, LRESULT*);
  afx_msg void OnUpdateDirSaveHash(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileSortPath(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileSortSize(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileDupl(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileMark(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileMove(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileDel(CCmdUI* pCmdUI);
  afx_msg void OnDirSaveHash();  // Save Hashes
  afx_msg void OnFileSort();     // sort file display by size / path
  afx_msg void OnFileDupl();     // show only non-unique files
  afx_msg void OnFileMark();     // Mark non-primary files
  afx_msg void OnFileMove();     // Rename primary file to name of selected
  afx_msg void OnFileDel();      // Delete selected files
  DECLARE_MESSAGE_MAP()

};
