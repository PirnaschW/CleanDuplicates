
//  CleanDuplicates.h  :  main  header  file  for  the  CleanDuplicates  application
//

#ifndef  __AFXWIN_H__
	#error  "include  'pch.h'  before  including  this  file  for  PCH"
#endif

#include  "resource.h"              //  main  symbols


//  CCleanDuplicatesApp:
//  See  CleanDuplicates.cpp  for  the  implementation  of  this  class
//

class  CCleanDuplicatesApp  :  public  CWinAppEx
{
public:
	CCleanDuplicatesApp()  noexcept;


//  Overrides
public:
	virtual  BOOL  InitInstance();
	virtual  int  ExitInstance();

//  Implementation
	UINT    m_nAppLook;
	BOOL    m_bHiColorIcons;

	virtual  void  PreLoadState();
	virtual  void  LoadCustomState();
	virtual  void  SaveCustomState();

	afx_msg  void  OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern  CCleanDuplicatesApp  theApp;
