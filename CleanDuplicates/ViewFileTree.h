
namespace MyViews
{

  class ViewFileTree : public CTreeView
  {
  protected:
    ViewFileTree() noexcept {};
    DECLARE_DYNCREATE(ViewFileTree)
  public:
    virtual ~ViewFileTree() {}
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    CCleanDuplicatesDoc* GetDocument() const { return dynamic_cast<CCleanDuplicatesDoc*>(m_pDocument); }
    bool DirAdd(const std::wstring& s);
    bool DirDel(const std::wstring& s);

  protected:
    virtual void OnInitialUpdate(); // called first time after construct
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
  protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
  };

}
