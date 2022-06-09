#include "stdafx.h"

#include "WindowsUtils.h"

#include <stdio.h>
#include <string>

#include <atlbase.h>

#include "Common/strhelper.h"
#include "Common/Utils.h"
#include "WinCommon/WindowsStrings.h"
#include "ShellExtComm.h"

using namespace std;
using namespace sunjwbase;

typedef HRESULT(__stdcall *LPFN_DllRegisterServer)(void);
typedef HRESULT(__stdcall *LPFN_DllUnregisterServer)(void);

namespace WindowsUtils
{
	BOOL IsWindows64()
	{
		CRegKey key;
		LPCTSTR lpszEnvKeyName = _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
		LPCTSTR lpszArchKeyName = _T("PROCESSOR_ARCHITECTURE");

		LONG lResEnv;

		// 打开
		lResEnv = key.Open(HKEY_LOCAL_MACHINE, lpszEnvKeyName, KEY_READ);

		if(lResEnv != ERROR_SUCCESS)
			return false;
		
		TCHAR tszArch[100] = { L'0' };
		ULONG nChars = 100;
		key.QueryStringValue(lpszArchKeyName, tszArch, &nChars);
		key.Close();
		
		string strArch(tstrtostr(tszArch));
		strArch = str_upper(strArch);
		strArch = strtrim(strArch);

		return (strArch == "AMD64");
	}

	BOOL IsLimitedProc()
	{
		HRESULT hResult = E_FAIL; // assume an error occurred
		HANDLE hToken = NULL;
		TOKEN_ELEVATION_TYPE tet;

		if (!OpenProcessToken(GetCurrentProcess(), 
							TOKEN_QUERY, 
							&hToken))
		{
			return TRUE;
		}

		DWORD dwReturnLength = 0;

		if (GetTokenInformation(
					hToken,
					TokenElevationType,
					&tet,
					sizeof(tet),
					&dwReturnLength))
		{
				if(dwReturnLength != sizeof(tet))
					return TRUE;
				hResult = S_OK;
		}

		CloseHandle(hToken);

		if(hResult != S_OK)
			return TRUE;

		if(tet == TokenElevationTypeLimited)
		{
			return TRUE;
		}
		else if(tet == TokenElevationTypeDefault && 
				!IsUserAnAdmin())
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL ElevateProcess()
	{
		BOOL bRet = FALSE;

		TCHAR szExecutable[MAX_PATH + 1] = {0};
		if(GetModuleFileName(NULL, szExecutable, MAX_PATH) > 0)
		{
			SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
			sei.lpVerb = TEXT("runas");
			sei.lpFile = szExecutable;
			sei.lpParameters = NULL;
			sei.nShow = SW_SHOWNORMAL;

			return ShellExecuteEx(&sei);
		}

		return bRet;
	}

	void CopyCString(const CString& cstrToCopy)
	{
		HGLOBAL hMoveable;
		LPTSTR pszArr;

		size_t bytes = (cstrToCopy.GetLength() + 1)*sizeof(TCHAR);
		hMoveable = GlobalAlloc(GMEM_MOVEABLE, bytes);
		pszArr = (LPTSTR)GlobalLock(hMoveable);
		ZeroMemory(pszArr, bytes);
		_tcscpy_s(pszArr, cstrToCopy.GetLength() + 1, cstrToCopy);
		GlobalUnlock(hMoveable);

		::OpenClipboard(NULL);
		::EmptyClipboard();
		::SetClipboardData(CF_UNICODETEXT, hMoveable);
		::CloseClipboard();
	}

	HINSTANCE OpenURL(const TCHAR *pszURL)
	{
		return ShellExecute(NULL, _T("open"), pszURL, 
			NULL, NULL, SW_SHOW);
	}

	bool FindShlExtDll(TCHAR *pszExeFullPath, TCHAR *pszShlDllPath)
	{
		tstring tstrExeDirPath(pszExeFullPath);
		tstring::size_type idx = tstrExeDirPath.rfind(_T("\\"));
		tstrExeDirPath = tstrExeDirPath.substr(0, idx);

		tstring tstrShlExtDll = tstrExeDirPath;
		tstrShlExtDll.append(_T("\\fHashShlExt"));
		if(IsWindows64())
			tstrShlExtDll.append(_T("64"));
		tstrShlExtDll.append(_T(".dll"));

		WIN32_FIND_DATA ffData;
		HANDLE hFind = FindFirstFile(tstrShlExtDll.c_str(), &ffData);

		bool bRet = (hFind != INVALID_HANDLE_VALUE);

		if(bRet)
		{
#if defined(UNICODE) || defined(_UNICODE)
				wcscpy_s(pszShlDllPath, MAX_PATH, tstrShlExtDll.c_str());
#else
				strcpy_s(pszShlDllPath, MAX_PATH, tstrShlExtDll.c_str());
#endif

			FindClose(hFind);
		}

		return bRet;
	}

	bool RegShellExt(TCHAR *pszShlDllPath)
	{
		HMODULE hModule = LoadLibrary(pszShlDllPath);
		if(hModule)
		{
			LPFN_DllRegisterServer fnDllRegSvr = NULL;
			fnDllRegSvr = (LPFN_DllRegisterServer) GetProcAddress(hModule, "DllRegisterServer");
			if(fnDllRegSvr != NULL)
			{
				return (fnDllRegSvr() == S_OK);
			}

			FreeLibrary(hModule);
		}
		
		return false;
	}

	bool UnregShellExt(TCHAR *pszShlDllPath)
	{
		HMODULE hModule = LoadLibrary(pszShlDllPath);
		if(hModule)
		{
			LPFN_DllUnregisterServer fnDllUnregSvr = NULL;
			fnDllUnregSvr = (LPFN_DllUnregisterServer) GetProcAddress(hModule, "DllUnregisterServer");
			if(fnDllUnregSvr != NULL)
			{
				return (fnDllUnregSvr() == S_OK);
			}

			FreeLibrary(hModule);
		}
		
		return false;
	}


	bool AddContextMenu(TCHAR *pszExeFullPath)
	{
		CRegKey key;
		LONG lResult;
		LPCTSTR lpszKeyName = NULL;
		if (WindowsStrings::GetCurrentUILang() == 2052)
			lpszKeyName = CONTEXT_MENU_REGESTRY_ZH_CN;
		else
			lpszKeyName = CONTEXT_MENU_REGESTRY_EN_US;

		// 创建目录
		lResult = key.Create(HKEY_CLASSES_ROOT, lpszKeyName);
		if(lResult != ERROR_SUCCESS)
			return false; // 失败

		// 打开
		lResult = key.Open(HKEY_CLASSES_ROOT, lpszKeyName, KEY_ALL_ACCESS);
		if(lResult != ERROR_SUCCESS)
			return false;

		// 成功打开
		TCHAR pszCommand[270];

#if defined(UNICODE) || defined(_UNICODE)
		wcscpy_s(pszCommand, MAX_PATH + 10, pszExeFullPath);
		wcscat_s(pszCommand, MAX_PATH + 10, _T(" \"%1\""));
#else
		strcpy_s(pszCommand, MAX_PATH + 10, pszExeFullPath);
		strcat_s(pszCommand, MAX_PATH + 10, _T(" \"%1\""));
#endif
		
		lResult = key.SetStringValue(NULL, pszCommand);
		key.Close();

		if(lResult == ERROR_SUCCESS)
			return true;
		else
			return false;
	}

	bool AddShellExt(TCHAR *pszExeFullPath)
	{
		CRegKey key;
		LONG lResult;
		LPCTSTR lpszKeyName = SHELL_EXT_REGESTRY;

		// 创建目录
		lResult = key.Create(HKEY_CLASSES_ROOT, lpszKeyName);
		if(lResult != ERROR_SUCCESS)
			return false; // 失败

		// 打开
		lResult = key.Open(HKEY_CLASSES_ROOT, lpszKeyName, KEY_ALL_ACCESS);
		if(lResult != ERROR_SUCCESS)
			return false;

		// 成功打开
		LPCTSTR pszUuid = SHELL_EXT_UUID;
		LPCTSTR pszExePath = pszExeFullPath;
		
		lResult = key.SetStringValue(NULL, pszUuid);
		lResult |= key.SetStringValue(SHELL_EXT_EXEPATH, pszExePath);
		key.Close();

		if(lResult == ERROR_SUCCESS)
			return true;
		else
			return false;
	}

	bool AddContextMenu()
	{
		TCHAR pszExeFullPath[MAX_PATH + 10] = { L'0' };
		TCHAR pszShlDllPath[MAX_PATH + 10] = { L'0' };

		GetModuleFileName(NULL, pszExeFullPath, MAX_PATH); // 得到程序模块名称，全路径
		
		if(FindShlExtDll(pszExeFullPath, pszShlDllPath))
		{
			// We found shell extension dll
			if(RegShellExt(pszShlDllPath) &&
				AddShellExt(pszExeFullPath))
				return true;
			else
				return AddContextMenu(pszExeFullPath); // fallback
		}
		else
		{
			// No dll, just add context menu
			return AddContextMenu(pszExeFullPath);
		}
	}

	bool RemoveContextMenu()
	{
		TCHAR pszExeFullPath[MAX_PATH + 10] = { L'0' };
		TCHAR pszShlDllPath[MAX_PATH + 10] = { L'0' };

		GetModuleFileName(NULL, pszExeFullPath, MAX_PATH); // 得到程序模块名称，全路径
		if(FindShlExtDll(pszExeFullPath, pszShlDllPath))
		{
			UnregShellExt(pszShlDllPath);
		}

		CRegKey keyShell, keyShellEx;
		LPCTSTR lpszKeyShellName = _T("*\\shell\\");
		LPCTSTR lpszKeyShellExName = _T("*\\shellex\\ContextMenuHandlers\\");
		LONG lResShell, lResShellEx;

		// 打开
		lResShell = keyShell.Open(HKEY_CLASSES_ROOT, lpszKeyShellName, KEY_ALL_ACCESS);
		lResShellEx = keyShellEx.Open(HKEY_CLASSES_ROOT, lpszKeyShellExName, KEY_ALL_ACCESS);
		if(lResShell != ERROR_SUCCESS && 
			lResShellEx != ERROR_SUCCESS)
			return false;

		LONG lResult = 1L;

		// Try to delete context menu
		if(lResShell == ERROR_SUCCESS)
		{
			lResult &= keyShell.RecurseDeleteKey(CONTEXT_MENU_ITEM_EN_US);
			lResult &= keyShell.RecurseDeleteKey(CONTEXT_MENU_ITEM_ZH_CN);
			keyShell.Close();
		}

		// Try to delete shell extension
		if(lResShellEx == ERROR_SUCCESS)
		{
			lResult &= keyShellEx.RecurseDeleteKey(_T("fHashShellExt"));
			keyShellEx.Close();
		}

		if(lResult != ERROR_SUCCESS)
			return false;
		else
			return true;
	}

	bool ContextMenuExisted()
	{
		CRegKey keyCtxMenuBase, keyCtxMenuZh, keyShlExt;

		LPCTSTR lpszCtxMenuKeyNameBase = CONTEXT_MENU_REGESTRY_EN_US;
		LPCTSTR lpszCtxMenuKeyNameZh = CONTEXT_MENU_REGESTRY_ZH_CN;
		LPCTSTR lpszShlExtKeyName = SHELL_EXT_REGESTRY;

		LONG lResCtxMenuBase;
		LONG lResCtxMenuZh;
		LONG lResShlExt;

		// 打开
		lResCtxMenuBase = keyCtxMenuBase.Open(HKEY_CLASSES_ROOT, lpszCtxMenuKeyNameBase, KEY_READ);
		lResCtxMenuZh = keyCtxMenuZh.Open(HKEY_CLASSES_ROOT, lpszCtxMenuKeyNameZh, KEY_READ);
		lResShlExt = keyShlExt.Open(HKEY_CLASSES_ROOT, lpszShlExtKeyName, KEY_READ);

		if(lResCtxMenuBase != ERROR_SUCCESS && 
			lResCtxMenuZh != ERROR_SUCCESS && 
			lResShlExt != ERROR_SUCCESS)
			return false;
		
		if(lResCtxMenuBase == ERROR_SUCCESS)
			keyCtxMenuBase.Close();
		if(lResCtxMenuZh == ERROR_SUCCESS)
			keyCtxMenuZh.Close();
		if(lResShlExt == ERROR_SUCCESS)
			keyShlExt.Close();

		return true;
	}

}

