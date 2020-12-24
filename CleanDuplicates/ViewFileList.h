
namespace MyViews
{

  class ViewFileList : public CView
  {
  protected:
    inline ViewFileList() noexcept {};
    DECLARE_DYNCREATE(ViewFileList)
  public:
    virtual ~ViewFileList() {}
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnDraw(CDC* pDC);
    CCleanDuplicatesDoc* GetDocument() const { return dynamic_cast<CCleanDuplicatesDoc*>(m_pDocument); }
  protected:
    virtual void OnInitialUpdate(); // called first time after construct
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  private:
    CFont m_fntH{};
    void SetHeaderFont();
    CListCtrl m_List;
    CImageList m_ImageList;
    CToolBarCtrl m_ToolBar;

// Generated message map functions
  protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
  };

}
