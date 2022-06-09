#ifndef _WINDOWS_UTILS_H_
#define _WINDOWS_UTILS_H_

#include <string>
#include <Windows.h>
#include <atlbase.h>
#include <stdint.h>

#include "Common/strhelper.h"
#include "Common/Global.h"

namespace WindowsUtils
{
	inline sunjwbase::tstring cstrtotstr(const CString& cstring)
	{
		return sunjwbase::tstring(cstring.GetString());
	}

	inline CString tstrtocstr(const sunjwbase::tstring& tstr)
	{
		CString temp;
		temp.SetString(tstr.c_str());
		return temp;
	}

	BOOL IsWindows64();
	BOOL IsLimitedProc();
	BOOL ElevateProcess();

	void CopyCString(const CString& cstrToCopy);

	HINSTANCE OpenURL(const TCHAR *pszURL);

	bool FindShlExtDll(TCHAR *pszExeFullPath, TCHAR *pszShlDllPath);

	bool RegShellExt(TCHAR *pszShlDllPath);
	bool UnregShellExt(TCHAR *pszShlDllPath);

	bool AddContextMenu(TCHAR *pszExeFullPath);
	bool AddShellExt(TCHAR *pszExeFullPath);

	bool AddContextMenu();
	bool RemoveContextMenu();
	bool ContextMenuExisted();

}

#endif
