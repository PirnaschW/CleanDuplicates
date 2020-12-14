
namespace MyWin
{

  class WndFileTree : public CDockablePane
  {
  public:
    WndFileTree() noexcept;
    virtual ~WndFileTree();

    void AdjustLayout();
    void OnChangeVisualStyle();

  private:
  public:
    ToolBar m_wndToolBar;
    CTreeCtrl m_wndTree;
    CImageList m_Images;

  public:
    bool CreatePane(CWnd* parent);

    // Overrides
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    FMap FillFileTree(const DList& dl);

  private:
    void CollectFiles(const std::filesystem::directory_entry& s, HTREEITEM hScope);

  private:
    MD5::MD5 md5;
    std::vector<FileData> files_{};

  protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    DECLARE_MESSAGE_MAP()

  };

}