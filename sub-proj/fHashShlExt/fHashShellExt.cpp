// fHashShellExt.cpp : CfHashShellExt µÄÊµÏÖ

#include "stdafx.h"
#include "fHashShellExt.h"
#include <string>
#include <atlconv.h>
#include "strhelper.h"
#include "UIStrings.h"

using namespace std;
using namespace sunjwbase;

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

#ifdef ZH_CN
	LPCTSTR pszMenuItem = SHELL_EXT_ITEM_ZH_CN;
#else
	LPCTSTR pszMenuItem = SHELL_EXT_ITEM_EN_US;
#endif

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
			tstring tstrfHashPath = m_fHashPath;
			tstring tstrCmd = _T("\"") + tstrfHashPath + _T("\"");

			for(TstrList::const_iterator itr = m_pathList.begin();
				itr != m_pathList.end();
				++itr)
			{
				tstrCmd.append(_T(" "));
				tstrCmd.append(_T("\""));
				tstrCmd.append(*itr);
				tstrCmd.append(_T("\""));
			}

			TCHAR *pszCmd = new TCHAR[tstrCmd.length() + 1];
			memset(pszCmd, 0, sizeof(pszCmd));
#if defined(UNICODE) || defined(_UNICODE)
			wcscpy(pszCmd, tstrCmd.c_str());
#else
			strcpy(pszCmd, tstrCmd.c_str());
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
		break;
 
    default:
		return E_INVALIDARG;
		break;
    }
}
