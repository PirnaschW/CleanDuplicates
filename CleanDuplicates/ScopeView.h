
class  CScopeToolBar : public  CMFCToolBar
{
  virtual  void  OnUpdateCmdUI(CFrameWnd*  /*pTarget*/, BOOL  bDisableIfNoHndler)
  {
    CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
  }

  virtual  BOOL  AllowShowOnList()  const { return  FALSE; }
};

class  CScopeView : public  CDockablePane
{
public:
  CScopeView()  noexcept;
  virtual  ~CScopeView();

  void  AdjustLayout();
  void  OnChangeVisualStyle();

protected:
  CScopeToolBar  m_wndToolBar;
  CViewTree  m_wndScopeView;
  CImageList  m_ScopeViewImages;
  UINT  m_nCurrSort;

  void  FillScopeView();

//  Overrides
public:
  virtual  BOOL  PreTranslateMessage(MSG* pMsg);

protected:
  afx_msg  int  OnCreate(LPCREATESTRUCT  lpCreateStruct);
  afx_msg  void  OnSize(UINT  nType, int  cx, int  cy);
  afx_msg  void  OnContextMenu(CWnd* pWnd, CPoint  point);
  afx_msg  void  OnScopeAddMemberFunction();
  afx_msg  void  OnScopeAddMemberVariable();
  afx_msg  void  OnScopeDefinition();
  afx_msg  void  OnScopeProperties();
  afx_msg  void  OnNewFolder();
  afx_msg  void  OnPaint();
  afx_msg  void  OnSetFocus(CWnd* pOldWnd);
  afx_msg  LRESULT  OnChangeActiveTab(WPARAM, LPARAM);
  afx_msg  void  OnSort(UINT  id);
  afx_msg  void  OnUpdateSort(CCmdUI* pCmdUI);

  DECLARE_MESSAGE_MAP()


  void  CollectFiles(const  std::filesystem::directory_entry& s, HTREEITEM  hScope);
private:
  struct  FileData
  {
    std::wstring  path;
    bool  directory;
    std::filesystem::file_time_type  LastWriteTime;
  };
  std::vector<FileData>  files_{};
};
