
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
    void FillList(FMap& fmap);

  protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

    DECLARE_MESSAGE_MAP()

  private:
    void InitList();
    void SetFont();

  };

}

