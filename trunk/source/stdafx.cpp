// stdafx.cpp : 只包括标准包含文件的源文件
// MD5SUM03.pch 将是预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// Fix so big binary
// this is our own local copy of the AfxLoadSystemLibraryUsingFullPath function
HMODULE AfxLoadSystemLibraryUsingFullPath(const WCHAR *pszLibrary)
{
	WCHAR wszLoadPath[MAX_PATH+1];
	if (::GetSystemDirectoryW(wszLoadPath, _countof(wszLoadPath)) == 0)
	{
		return NULL;
	}

	if (wszLoadPath[wcslen(wszLoadPath)-1] != L'\\')
	{
		if (wcscat_s(wszLoadPath, _countof(wszLoadPath), L"\\") != 0)
		{
			return NULL;
		}
	}

	if (wcscat_s(wszLoadPath, _countof(wszLoadPath), pszLibrary) != 0)
	{
		return NULL;
	}

	return(::AfxCtxLoadLibraryW(wszLoadPath));
}


