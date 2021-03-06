#include "pch.h"

using namespace MyViews;  // MFC macros cannot handle namespace qualifications, so we need to import

BEGIN_MESSAGE_MAP(CCleanDuplicatesApp, CWinAppEx)
  ON_COMMAND(ID_APP_ABOUT, &CCleanDuplicatesApp::OnAppAbout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

CCleanDuplicatesApp::CCleanDuplicatesApp() noexcept
{
  m_bHiColorIcons = TRUE;

  // TODO: replace application ID string below with unique ID string; recommended
  // format for string is CompanyName.ProductName.SubProduct.VersionInformation
  SetAppID(_T("PiSoft.CleanDuplicates.1.000"));

  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

// The one and only CCleanDuplicatesApp object

CCleanDuplicatesApp theApp;


// CCleanDuplicatesApp initialization

BOOL CCleanDuplicatesApp::InitInstance()
{
 // InitCommonControlsEx() is required on Windows XP if an application
 // manifest specifies use of ComCtl32.dll version 6 or later to enable
 // visual styles. Otherwise, any window creation will fail.
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  // Set this to include all the common control classes you want to use
  // in your application.
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  CWinAppEx::InitInstance();


  EnableTaskbarInteraction();

  // AfxInitRichEdit2() is required to use RichEdit control
  // AfxInitRichEdit2();

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need
  // Change the registry key under which our settings are stored
  // TODO: You should modify this string to be something appropriate
  // such as the name of your company or organization
  SetRegistryKey(_T("PiSoft.CleanDuplicates"));
  LoadStdProfileSettings(16); // Load standard INI file options (including MRU)


  InitContextMenuManager();

  InitKeyboardManager();

  InitTooltipManager();
  CMFCToolTipInfo ttParams;
  ttParams.m_bVislManagerTheme = TRUE;
  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
    RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

    // Register the application's document templates. Document templates
    // serve as the connection between documents, frame windows and views
  CMultiDocTemplate* pDocTemplate;
  pDocTemplate = new CMultiDocTemplate(IDR_CleanDuplicatesTYPE,
    RUNTIME_CLASS(CCleanDuplicatesDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
   // RUNTIME_CLASS(CCleanDuplicatesView));
    RUNTIME_CLASS(ViewDirList));
//  RUNTIME_CLASS(ViewFileTree);
//  RUNTIME_CLASS(ViewFileList);
  if (!pDocTemplate)
    return FALSE;
  AddDocTemplate(pDocTemplate);

  // create main MDI Frame window
  CMainFrame* pMainFrame = new CMainFrame;
  if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    delete pMainFrame;
    return FALSE;
  }
  m_pMainWnd = pMainFrame;

  // call DragAcceptFiles only if there's a suffix
  // In an MDI app, this should occur immediately after setting m_pMainWnd
  // Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();

  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // Enable DDE Execute open
  EnableShellOpen();
  RegisterShellFileTypes(TRUE);


  // Dispatch commands specified on the command line. Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;
    // The main window has been initialized, so show and update it
   // pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
  pMainFrame->UpdateWindow();

  return TRUE;
}

int CCleanDuplicatesApp::ExitInstance()
{
 //TODO: handle additional resources you may have added
  return CWinAppEx::ExitInstance();
}

// CCleanDuplicatesApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
  CAboutDlg() noexcept;

 // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_ABOUTBOX };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

 // Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CCleanDuplicatesApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

// CCleanDuplicatesApp customization load/save methods

void CCleanDuplicatesApp::PreLoadState()
{
  BOOL bNameValid;
  CString strName;

  bNameValid = strName.LoadString(IDS_EDIT_MENU);
  ASSERT(bNameValid);
  GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);

  bNameValid = strName.LoadString(IDS_DIRLIST);
  ASSERT(bNameValid);
  GetContextMenuManager()->AddMenu(strName, IDR_DIRLIST);

  bNameValid = strName.LoadString(IDS_EXPLORER);
  ASSERT(bNameValid);
  GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CCleanDuplicatesApp::LoadCustomState()
{
}

void CCleanDuplicatesApp::SaveCustomState()
{
}

// CCleanDuplicatesApp message handlers
