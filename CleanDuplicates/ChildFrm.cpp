#include "pch.h"
using namespace MyViews;  // the RUNTIME_CLASS macro cannot handle namespaces, so we need to import all names

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

CChildFrame::CChildFrame() noexcept {}

CChildFrame::~CChildFrame() {}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
  CRect cr;
  GetClientRect(&cr);

  m_wndSplit1.CreateStatic(this, 2, 1); // number of rows, columns
  m_wndSplit1.CreateView(0, 0, RUNTIME_CLASS(ViewDirList), CSize(cr.Width(), 150), pContext);
  m_wndSplit2.CreateStatic(&m_wndSplit1, 1, 2, WS_CHILD | WS_VISIBLE, m_wndSplit1.IdFromRowCol(1, 0));
  m_wndSplit2.CreateView(0, 0, RUNTIME_CLASS(ViewFileTree), CSize(400, cr.Height() - 150), pContext);
  m_wndSplit2.CreateView(0, 1, RUNTIME_CLASS(ViewFileList), CSize(600, cr.Height() - 150), pContext);
  return TRUE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CMDIChildWndEx::PreCreateWindow(cs)) return FALSE;
  return TRUE;
}

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
  CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
  CMDIChildWndEx::Dump(dc);
}
#endif _DEBUG
