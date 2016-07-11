#ifndef _WINDOWS_UTILS_H_
#define _WINDOWS_UTILS_H_

#include <stdint.h>
#include <string>
#include <Windows.h>
#include <atlbase.h>

#include "strhelper.h"
#include "Global.h"

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

	// 以下是获得 Windows 版本信息的定义内容
#define BUFSIZE 80

	typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
	// GetProductInfo 函数原型(Vista/2008)
	typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, 
								DWORD, PDWORD );
	// 以上是获得 Windows 版本信息的定义内容
	typedef HRESULT (__stdcall *LPFN_DllRegisterServer)(void);
	typedef HRESULT (__stdcall *LPFN_DllUnregisterServer)(void);

	CString GetExeFileVersion(TCHAR* path);

	BOOL GetWindowsVersion(OSVERSIONINFOEX& osvi, BOOL& bOsVersionInfoEx);
	CString GetWindowsInfo();
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
