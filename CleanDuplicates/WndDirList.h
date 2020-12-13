
namespace MyWin
{

  class WndDirListToolBar : public CMFCToolBar
  {
  public:
    virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
    {
      CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() const { return FALSE; }
  };

  class WndDirList : public CDockablePane
  {
  public:
    WndDirList() noexcept;

    void AdjustLayout();

  protected:
    CFont m_fntList;
    CFont m_fntListH;
    WndDirListToolBar m_wndToolBar;
    CListCtrl m_wndList;

  public:
    virtual ~WndDirList();

  protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDirAdd();
    afx_msg void OnDirDel();
    afx_msg void OnDirExecute();
    afx_msg void OnUpdateDirAdd(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
    afx_msg void OnUpdateDirDel(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
    afx_msg void OnUpdateDirExecute(CCmdUI* pCmdUI) { pCmdUI->Enable(TRUE); }
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

    DECLARE_MESSAGE_MAP()

    void InitList();
    void SetListFont();
  };

}
