#include "pch.h"

namespace MyWin
{

  void ToolBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) { CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler); }
  BOOL ToolBar::AllowShowOnList() const { return FALSE; }

}
