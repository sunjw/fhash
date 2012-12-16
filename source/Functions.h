#pragma once
#include "stdafx.h"

// 以下是获得 Windows 版本信息的定义内容
#define BUFSIZE 80

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
// GetProductInfo函数原型(Vista/2008)
typedef BOOL (WINAPI *PGPI)(DWORD dwOSMajorVersion, DWORD dwOSMinorVersion, DWORD dwSpMajorVersion, DWORD dwSpMinorVersion, PDWORD pdwReturnedProductType);
// 以上是获得 Windows 版本信息的定义内容

CString ConvertSizeToCStr(ULONGLONG size);

CString GetExeFileVersion(char* path);

BOOL GetWindowsVersion(OSVERSIONINFOEX& osvi, BOOL& bOsVersionInfoEx);

CString GetWindowsInfo();

bool AddContextMenu(void);
bool RemoveContextMenu(void);
bool IsContextMenuAdded(void);
