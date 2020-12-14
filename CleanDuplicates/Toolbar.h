namespace MyWin
{

  class ToolBar : public CMFCToolBar
  {
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
    virtual BOOL AllowShowOnList() const;
  };

}
