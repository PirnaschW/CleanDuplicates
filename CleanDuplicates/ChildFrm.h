
class CChildFrame : public CMDIChildWndEx
{
  DECLARE_DYNCREATE(CChildFrame)
public:
  CChildFrame() noexcept;
  virtual ~CChildFrame();
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

private:
  CSplitterWndEx m_wndSplit1;  // used to split off top for DirList
  CSplitterWndEx m_wndSplit2;  // used to split bottom, left is FileTree, right is FileList

// Generated message map functions
protected:
  DECLARE_MESSAGE_MAP()
};
