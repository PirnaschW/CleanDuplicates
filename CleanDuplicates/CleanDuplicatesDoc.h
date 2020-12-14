
class CCleanDuplicatesDoc : public CDocument
{
protected: // create from serialization only
  CCleanDuplicatesDoc() noexcept;
  DECLARE_DYNCREATE(CCleanDuplicatesDoc)

 // Attributes
public:

// Operations
public:

// Overrides
public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
  virtual void InitializeSearchContent();
  virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
  virtual ~CCleanDuplicatesDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

private:
  CMainFrame* GetMainFrame();

private:
  DList dlist_{};
  //std::vector<FileData> files_{};
  FMap fmap_{};

 // Generated message map functions
protected:
  afx_msg void OnDirAdd();
  afx_msg void OnDirDel();
  afx_msg void OnDirExecute();
  afx_msg void OnTreeSelChanged(NMHDR*, LRESULT*);
//  afx_msg void OnClick(NMHDR*, LRESULT*);
  DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
 // Helper function that sets search content for a Search Handler
  void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
