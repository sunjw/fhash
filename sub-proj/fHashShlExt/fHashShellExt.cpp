// fHashShellExt.cpp : CfHashShellExt µÄÊµÏÖ

#include "stdafx.h"
#include "fHashShellExt.h"

#include <string>
#include <atlconv.h>

#include "Psapi.h"

#include "strhelper.h"
#include "ShellExtComm.h"
#include "WindowsStrings.h"
#include "fHashShlExtStringsBase.h"
#include "fHashShlExtStringsZHCN.h"

using namespace std;
using namespace sunjwbase;
using namespace WindowsStrings;

#define MAX_FILE_CMD 32768

// CfHashShellExt
CfHashShellExt::CfHashShellExt()
{
	RegisterStringsForLang(-1, new fHashShlExtStringsBase());
	RegisterStringsForLang(2052, new fHashShlExtStringsZHCN());
}

// CfHashShellExt
HRESULT CfHashShellExt::Initialize(LPCITEMIDLIST pidlFolder,
								  LPDATAOBJECT pDataObj,
								  HKEY hProgID)
{
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT,
					  -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;
 
	// Look for CF_HDROP data in the data object. If there
	// is no such data, return an error back to Explorer.
	if(FAILED(pDataObj->GetData(&fmt, &stg)))
		return E_INVALIDARG;
	 
	// Get a pointer to the actual data.
	hDrop = (HDROP)GlobalLock(stg.hGlobal);
	 
	// Make sure it worked.
	if(NULL == hDrop)
		return E_INVALIDARG;

	// Sanity check ¨C make sure there is at least one filename.
	UINT uNumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	HRESULT hr = S_OK;
  
	if(0 == uNumFiles)
    {
		GlobalUnlock(stg.hGlobal);
		ReleaseStgMedium(&stg);
		return E_INVALIDARG;
    }
 
	// Get the name of files
	TCHAR szDragFilename[MAX_PATH];
	for(UINT i = 0; i < uNumFiles; i++)
	{
		if(0 == DragQueryFile(hDrop, i, 
							szDragFilename, sizeof(szDragFilename)))
			hr = E_INVALIDARG; 
		
		m_pathList.push_back(szDragFilename);
	}
 
	GlobalUnlock(stg.hGlobal);
	ReleaseStgMedium(&stg);

	// Try to find fHash
	CRegKey key;
	LPCTSTR lpszKeyName = SHELL_EXT_REGESTRY;
	LONG lResult;
	lResult = key.Open(HKEY_CLASSES_ROOT, lpszKeyName, KEY_READ);
	if(lResult != ERROR_SUCCESS)
		return E_INVALIDARG;

	TCHAR szPath[MAX_PATH + 1] = { L'0' };
	ULONG nChars = MAX_PATH;
	key.QueryStringValue(SHELL_EXT_EXEPATH, szPath, &nChars);
	key.Close();

	m_fHashPath = szPath;
	if(m_fHashPath == _T(""))
		hr = E_INVALIDARG;
 
	return hr;
}

HRESULT CfHashShellExt::QueryContextMenu(
						HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd,
						UINT uidLastCmd, UINT uFlags)
{
	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
	if (uFlags & CMF_DEFAULTONLY)
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

	LPCTSTR pszMenuItem = GetStringByKey(SHELL_EXT_ITEM);

	InsertMenu(hmenu, uMenuIndex, MF_BYPOSITION,
               uidFirstCmd, pszMenuItem);
 
	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 1);
}

HRESULT CfHashShellExt::GetCommandString(
						  UINT_PTR idCmd, UINT uFlags, UINT* pwReserved,
						  LPSTR pszName, UINT cchMax)
{
	USES_CONVERSION;
 
	// Check idCmd, it must be 0 since we have only one menu item.
	if(0 != idCmd)
		return E_INVALIDARG;
 
	// If Explorer is asking for a help string, copy our string into the
	// supplied buffer.
	if(uFlags & GCS_HELPTEXT)
    {
		LPCTSTR szText = _T("Using fHash to hash selected file(s).");
 
		if(uFlags & GCS_UNICODE)
		{
			// We need to cast pszName to a Unicode string, and then use the
			// Unicode string copy API.
			lstrcpynW((LPWSTR)pszName, T2CW(szText), cchMax);
		}
		else
		{
			// Use the ANSI string copy API to return the help string.
			lstrcpynA(pszName, T2CA(szText), cchMax);
		}
 
		return S_OK;
    }
 
  return E_INVALIDARG;
}

HRESULT CfHashShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo)
{
	// If lpVerb really points to a string, ignore this function call and bail out.
	if(0 != HIWORD(pCmdInfo->lpVerb))
		return E_INVALIDARG;
 
	// Get the command index - the only valid one is 0.
	switch (LOWORD(pCmdInfo->lpVerb))
    {
    case 0:
		{
			HWND hWndfHash = FindfHashWindow();
			if (hWndfHash == NULL) // Launch and calculate...
				return LaunchfHashByCommandLine(pCmdInfo, TRUE);

			// Found fHash, send message.
			SendFilesTofHash(pCmdInfo, hWndfHash);
			return S_OK;
		}
		break;
 
    default:
		return E_INVALIDARG;
		break;
    }

	return S_OK;
}

HRESULT CfHashShellExt::LaunchfHashByCommandLine(LPCMINVOKECOMMANDINFO pCmdInfo, BOOL bWithFiles)
{
	tstring tstrfHashPath = m_fHashPath;
	// fHash.exe
	tstring tstrCmd = _T("\"") + tstrfHashPath + _T("\"");

	if (bWithFiles == TRUE)
	{
		// Files...
		for(TstrList::const_iterator itr = m_pathList.begin();
			itr != m_pathList.end();
			++itr)
		{
			tstrCmd.append(_T(" "));
			tstrCmd.append(_T("\""));
			tstrCmd.append(*itr);
			tstrCmd.append(_T("\""));
		}
	}

	size_t cmdLen = tstrCmd.length() + 1;
	if(cmdLen > MAX_FILE_CMD)
	{
		MessageBox(pCmdInfo->hwnd, 
			GetStringByKey(SHELL_EXT_TOO_MANY_FILES), 
			GetStringByKey(SHELL_EXT_TOO_MANY_FILES), 
			MB_OK | MB_ICONWARNING);
		return S_OK;
	}

	TCHAR *pszCmd = new TCHAR[cmdLen];
	memset(pszCmd, 0, cmdLen);
#if defined(UNICODE) || defined(_UNICODE)
	wcscpy_s(pszCmd, cmdLen, tstrCmd.c_str());
#else
	strcpy_s(pszCmd, cmdLen, tstrCmd.c_str());
#endif

	STARTUPINFO sInfo = {0};
	sInfo.cb = sizeof(sInfo);
	PROCESS_INFORMATION pInfo = {0};

	CreateProcess(tstrfHashPath.c_str(), pszCmd,
		0, 0, TRUE,
		NORMAL_PRIORITY_CLASS,
		0, 0, &sInfo, &pInfo);

	delete [] pszCmd;

	return S_OK;
}

HWND CfHashShellExt::FindfHashWindow()
{
	HWND hWndfHash = NULL;
	hWndfHash = FindWindow(_T("#32770"), _T("fHash"));
	if (hWndfHash == NULL)
		return NULL;

	DWORD dwPidfHash = 0;
	GetWindowThreadProcessId(hWndfHash, &dwPidfHash);
	HANDLE hProcfHash = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwPidfHash);
	if (hProcfHash == NULL)
		return NULL;

	TCHAR szExecutable[MAX_PATH + 1] = {0};
	if (GetModuleFileNameEx(hProcfHash, NULL, szExecutable, MAX_PATH) <= 0)
		return NULL;

	tstring tstrProcfHashPath(szExecutable);
	if (tstrProcfHashPath == m_fHashPath)
		return hWndfHash;

	return NULL;
}

void CfHashShellExt::SendFilesTofHash(LPCMINVOKECOMMANDINFO pCmdInfo, HWND hWndfHash)
{
	if (hWndfHash == NULL)
		return;

	tstring tstrFiles;
	for(TstrList::const_iterator itr = m_pathList.begin();
		itr != m_pathList.end();
		++itr)
	{
		tstrFiles.append(_T(" "));
		tstrFiles.append(_T("\""));
		tstrFiles.append(*itr);
		tstrFiles.append(_T("\""));
	}

	size_t cmdLen = tstrFiles.length() + 1;
	if(cmdLen > MAX_FILE_CMD)
	{
		MessageBox(pCmdInfo->hwnd, 
			GetStringByKey(SHELL_EXT_TOO_MANY_FILES), 
			GetStringByKey(SHELL_EXT_TOO_MANY_FILES), 
			MB_OK | MB_ICONWARNING);
		return;
	}

	COPYDATASTRUCT cdFiles;
	cdFiles.dwData = 0;
	cdFiles.cbData = (DWORD)(cmdLen * sizeof(TCHAR));
	cdFiles.lpData = (PVOID)(tstrFiles.c_str());

	SendMessage(hWndfHash,
				WM_COPYDATA,
				(WPARAM)pCmdInfo->hwnd,
				(LPARAM)&cdFiles);
}
