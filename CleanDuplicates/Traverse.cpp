
#include "Traverse.h"
#include <iostream>

namespace Traverse
{

	void ProcessTree(const std::wstring& path, Callback f)
	{
		const std::wstring pattern = path + L"\\*.*";
		WIN32_FIND_DATA data;
		HANDLE h = ::FindFirstFile(pattern.c_str(), &data); // read the first file's data
		BOOL more = (h != INVALID_HANDLE_VALUE);            // if nothing, we are done
		while (more)
		{
			if (wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0)  // it's not itself or its parent
			{
				const bool children = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;   // is it a directory?
				const bool drilldown = (f)(path, data, children);      // callback to user to handle this file - user needs to tell if Drilldown requested
				if (drilldown && children) ProcessTree(path + L"\\" + data.cFileName, f);  // recursively get all lower level files now
			}
			more = FindNextFile(h, &data); // read next file
		};
		::FindClose(h);
	}

	bool TraverseTest(const std::wstring& path, const WIN32_FIND_DATA& data, bool children) // dummy for testing
	{
		OutputDebugString(std::wstring(path + L"\\" + data.cFileName + L"\n").c_str());
		return true;  // always drill down further
	}

}

//CString Path = path.c_str();
//CString OldName = Path + L"\\" + data.cFileName;
//CString NewName = Path + L"\\" + m_replace + (data.cFileName + wcslen(m_from.GetString()));
//CFile::Rename(OldName, NewName);
