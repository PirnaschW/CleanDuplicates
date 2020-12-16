
class CCleanDuplicatesDoc : public CDocument
{
protected: // create from serialization only
  CCleanDuplicatesDoc() noexcept {};
  DECLARE_DYNCREATE(CCleanDuplicatesDoc)
public:
  virtual ~CCleanDuplicatesDoc() {};
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
  virtual void AssertValid() const { CDocument::AssertValid(); }
  virtual void Dump(CDumpContext& dc) const { CDocument::Dump(dc); }

public:
  void UpdateAllViewsNow(CView* pSender = nullptr, LPARAM lHint = 0L, CObject* pHint = nullptr);  // function to update windows while working

private:
  CMainFrame* GetMainFrame() { CMainFrame* p = dynamic_cast<CMainFrame*>(AfxGetMainWnd()); if (p && p->GetSafeHwnd()) return p; else throw; }
  //void DirAdd(const std::filesystem::directory_entry& d);  // add a single dir to bottom of display
  //void DirDelSelected(DList& dl);                          // delete all selected from display and update given vector
  //void DirSetTo(DList& dl);                                // set display to given vector (useful for serialization)
  void FillFileTree(const std::filesystem::directory_entry& d);
  void CollectFiles(const std::filesystem::directory_entry& s, HTREEITEM hScope);

public:
  CListCtrl* pDirList{ nullptr };
  CTreeCtrl* pFileTree{ nullptr };
  CListCtrl* pFileList{ nullptr };

private:
  MD5::MD5 md5;
  DList dlist_{};
  FMap fmap_{};

 // Generated message map functions
protected:
  afx_msg void OnDirAdd();
  afx_msg void OnDirDel();
  afx_msg void OnDirExecute();
  afx_msg void OnTreeSelChanged(NMHDR*, LRESULT*);
  DECLARE_MESSAGE_MAP()

};
