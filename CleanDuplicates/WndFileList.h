
namespace MyWin
{

  class WndFileList : public CDockablePane
  {
  public:
    WndFileList() noexcept;

    void AdjustLayout();

  private:
    CListCtrl m_wndList;
    ToolBar m_wndToolBar;
    CFont m_fnt;
    CFont m_fntH;

  public:
    virtual ~WndFileList();

  public:
    bool CreatePane(CWnd* parent);
    void DirAdd(const std::filesystem::directory_entry& d); // add a single dir to bottom of display
    void DirDelSelected(DList& dl); // delete all selected from display and update given vector
    void DirSetTo(DList& dl); // set display to given vector (useful for serialization)

  protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnUpdateDirAdd(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirDel(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirExecute(CCmdUI* pCmdUI);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

    DECLARE_MESSAGE_MAP()

  private:
    void InitList();
    void SetFont();

  };

}

