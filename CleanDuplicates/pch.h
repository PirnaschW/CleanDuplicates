// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

// add headers that you want to pre-compile here

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"


#define _CRT_SECURE_NO_WARNINGS

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h> // MFC core and standard components
#include <afxext.h> // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h> // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h> // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h> // MFC support for ribbons and control bars

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "afxwinappex.h"
#include "afxdialogex.h"
#include "afxcview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <filesystem>
#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <propkey.h>

#include "Archive.h"
#include "FileMap.h"
#include "MD5.h"
#include "ToolBar.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "MainFrm.h"
#include "CleanDuplicatesDoc.h"
#include "ViewDirList.h"
#include "ViewFileTree.h"
#include "ViewFileList.h"
#include "ChildFrm.h"
#include "Resource.h"
#include "CleanDuplicates.h"
#include "CleanDuplicatesView.h"
