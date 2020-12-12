
//  CleanDuplicatesDoc.cpp  :  implementation  of  the  CCleanDuplicatesDoc  class
//

#include  "pch.h"
//  SHARED_HANDLERS  can  be  defined  in  an  ATL  project  implementing  preview,  thumbnail
//  and  search  filter  handlers  and  allows  sharing  of  document  code  with  that  project.

#ifdef  _DEBUG
#define  new  DEBUG_NEW
#endif

//  CCleanDuplicatesDoc

IMPLEMENT_DYNCREATE(CCleanDuplicatesDoc,  CDocument)

BEGIN_MESSAGE_MAP(CCleanDuplicatesDoc,  CDocument)
END_MESSAGE_MAP()


//  CCleanDuplicatesDoc  construction/destruction

CCleanDuplicatesDoc::CCleanDuplicatesDoc()  noexcept
{
	//  TODO:  add  one-time  construction  code  here

}

CCleanDuplicatesDoc::~CCleanDuplicatesDoc()
{
}

BOOL  CCleanDuplicatesDoc::OnNewDocument()
{
	if  (!CDocument::OnNewDocument())
		return  FALSE;

	//  TODO:  add  reinitialization  code  here
	//  (SDI  documents  will  reuse  this  document)

	return  TRUE;
}




//  CCleanDuplicatesDoc  serialization

void  CCleanDuplicatesDoc::Serialize(CArchive&  ar)
{
	if  (ar.IsStoring())
	{
		//  TODO:  add  storing  code  here
	}
	else
	{
		//  TODO:  add  loading  code  here
	}
}

#ifdef  SHARED_HANDLERS

//  Support  for  thumbnails
void  CCleanDuplicatesDoc::OnDrawThumbnail(CDC&  dc,  LPRECT  lprcBounds)
{
	//  Modify  this  code  to  draw  the  document's  data
	dc.FillSolidRect(lprcBounds,  RGB(255,  255,  255));

	CString  strText  =  _T("TODO:  implement  thumbnail  drawing  here");
	LOGFONT  lf;

	CFont*  pDefaultGUIFont  =  CFont::FromHandle((HFONT)  GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight  =  36;

	CFont  fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont*  pOldFont  =  dc.SelectObject(&fontDraw);
	dc.DrawText(strText,  lprcBounds,  DT_CENTER  |  DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

//  Support  for  Search  Handlers
void  CCleanDuplicatesDoc::InitializeSearchContent()
{
	CString  strSearchContent;
	//  Set  search  contents  from  document's  data.
	//  The  content  parts  should  be  separated  by  ";"

	//  For  example:    strSearchContent  =  _T("point;rectangle;circle;ole  object;");
	SetSearchContent(strSearchContent);
}

void  CCleanDuplicatesDoc::SetSearchContent(const  CString&  value)
{
	if  (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid,  PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl  *pChunk  =  nullptr;
		ATLTRY(pChunk  =  new  CMFCFilterChunkValueImpl);
		if  (pChunk  !=  nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents,  value,  CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif  //  SHARED_HANDLERS

//  CCleanDuplicatesDoc  diagnostics

#ifdef  _DEBUG
void  CCleanDuplicatesDoc::AssertValid()  const
{
	CDocument::AssertValid();
}

void  CCleanDuplicatesDoc::Dump(CDumpContext&  dc)  const
{
	CDocument::Dump(dc);
}
#endif  //_DEBUG


//  CCleanDuplicatesDoc  commands
