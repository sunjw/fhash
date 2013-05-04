#pragma once
#include "stdafx.h"

#include "strhelper.h"

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
// GetProductInfo函数原型(Vista/2008)
typedef BOOL (WINAPI *PGPI)(DWORD dwOSMajorVersion, DWORD dwOSMinorVersion, DWORD dwSpMajorVersion, DWORD dwSpMinorVersion, PDWORD pdwReturnedProductType);
// 以上是获得 Windows 版本信息的定义内容

CString ConvertSizeToCStr(ULONGLONG size);

CString GetExeFileVersion(TCHAR* path);

BOOL GetWindowsVersion(OSVERSIONINFOEX& osvi, BOOL& bOsVersionInfoEx);

CString GetWindowsInfo();

bool AddContextMenu(void);
bool RemoveContextMenu(void);
bool ContextMenuExisted(void);
