
//  CleanDuplicatesView.cpp  :  implementation  of  the  CCleanDuplicatesView  class
//

#include  "pch.h"

#ifdef  _DEBUG
#define  new  DEBUG_NEW
#endif


//  CCleanDuplicatesView

IMPLEMENT_DYNCREATE(CCleanDuplicatesView,  CView)

BEGIN_MESSAGE_MAP(CCleanDuplicatesView,  CView)
	//  Standard  printing  commands
	ON_COMMAND(ID_FILE_PRINT,  &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT,  &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW,  &CCleanDuplicatesView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

//  CCleanDuplicatesView  construction/destruction

CCleanDuplicatesView::CCleanDuplicatesView()  noexcept
{
	//  TODO:  add  construction  code  here

}

CCleanDuplicatesView::~CCleanDuplicatesView()
{
}

BOOL  CCleanDuplicatesView::PreCreateWindow(CREATESTRUCT&  cs)
{
	//  TODO:  Modify  the  Window  class  or  styles  here  by  modifying
	//    the  CREATESTRUCT  cs

	return  CView::PreCreateWindow(cs);
}

//  CCleanDuplicatesView  drawing

void  CCleanDuplicatesView::OnDraw(CDC*  /*pDC*/)
{
	CCleanDuplicatesDoc*  pDoc  =  GetDocument();
	ASSERT_VALID(pDoc);
	if  (!pDoc)
		return;

	//  TODO:  add  draw  code  for  native  data  here
}


//  CCleanDuplicatesView  printing


void  CCleanDuplicatesView::OnFilePrintPreview()
{
#ifndef  SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL  CCleanDuplicatesView::OnPreparePrinting(CPrintInfo*  pInfo)
{
	//  default  preparation
	return  DoPreparePrinting(pInfo);
}

void  CCleanDuplicatesView::OnBeginPrinting(CDC*  /*pDC*/,  CPrintInfo*  /*pInfo*/)
{
	//  TODO:  add  extra  initialization  before  printing
}

void  CCleanDuplicatesView::OnEndPrinting(CDC*  /*pDC*/,  CPrintInfo*  /*pInfo*/)
{
	//  TODO:  add  cleanup  after  printing
}

void  CCleanDuplicatesView::OnRButtonUp(UINT  /*  nFlags  */,  CPoint  point)
{
	ClientToScreen(&point);
	OnContextMenu(this,  point);
}

void  CCleanDuplicatesView::OnContextMenu(CWnd*  /*  pWnd  */,  CPoint  point)
{
#ifndef  SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT,  point.x,  point.y,  this,  TRUE);
#endif
}


//  CCleanDuplicatesView  diagnostics

#ifdef  _DEBUG
void  CCleanDuplicatesView::AssertValid()  const
{
	CView::AssertValid();
}

void  CCleanDuplicatesView::Dump(CDumpContext&  dc)  const
{
	CView::Dump(dc);
}

CCleanDuplicatesDoc*  CCleanDuplicatesView::GetDocument()  const  //  non-debug  version  is  inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCleanDuplicatesDoc)));
	return  (CCleanDuplicatesDoc*)m_pDocument;
}
#endif  //_DEBUG


//  CCleanDuplicatesView  message  handlers
