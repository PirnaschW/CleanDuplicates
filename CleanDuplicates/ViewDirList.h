
namespace MyViews
{

  class ViewDirList : public CListView
  {
  protected:
    inline ViewDirList() noexcept {};
    DECLARE_DYNCREATE(ViewDirList)
  public:
    virtual ~ViewDirList() {}
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    CCleanDuplicatesDoc* GetDocument() const { return dynamic_cast<CCleanDuplicatesDoc*>(m_pDocument); }
  protected:
    virtual void OnInitialUpdate(); // called first time after construct
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  private:
    CFont m_fntH{};
    void SetHeaderFont();

  protected:
    // Generated message map functions
    // add functions that modify the document content in the CDoc
    // add functions that modify the view's display in the CView
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnUpdateDirAdd(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirDel(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDirDelAll(CCmdUI* pCmdUI);
    afx_msg void OnDirAdd();
    afx_msg void OnDirDel();
    afx_msg void OnDirDelAll();
    DECLARE_MESSAGE_MAP()
  };

}
