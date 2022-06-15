// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

// ExplorerCommand handlers are an inproc verb implementation method that can provide
// dynamic behavior including computing the name of the command, its icon and its visibility state.
// only use this verb implemetnation method if you are implementing a command handler on
// the commands module and need the same functionality on a context menu.
//
// each ExplorerCommand handler needs to have a unique COM object, run uuidgen to
// create new CLSID values for your handler. a handler can implement multiple
// different verbs using the information provided via IInitializeCommand (the verb name).
// your code can switch off those different verb names or the properties provided
// in the property bag

#include "Dll.h"
#include "Common/strhelper.h"
#include "WinCommon/WindowsStrings.h"
#include "UwpShellExtStringsBase.h"
#include "UwpShellExtStringsZHCN.h"

static WCHAR const c_szVerbDisplayNameBack[] = L"Hash with fHash";
static WCHAR const c_szProgID[] = L"*";
static WCHAR const c_szVerbName[] = L"fHashUwpShellExt";
#if defined (_DEBUG)
static WCHAR const c_szExecPath[] = L"fHashUwpDev.exe";
#else
static WCHAR const c_szExecPath[] = L"fHashUwp.exe";
#endif

class CExplorerCommandVerb : public IExplorerCommand,
                             public IInitializeCommand,
                             public IObjectWithSite
{
public:
    CExplorerCommandVerb() : _cRef(1), _punkSite(NULL), _hwnd(NULL), _pstmShellItemArray(NULL)
    {
        DllAddRef();
        RegisterStringsForLang(-1, new UwpShlExtStringsBase());
        RegisterStringsForLang(2052, new UwpShlExtStringsZHCN());
    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(CExplorerCommandVerb, IExplorerCommand),       // required
            QITABENT(CExplorerCommandVerb, IInitializeCommand),     // optional
            QITABENT(CExplorerCommandVerb, IObjectWithSite),        // optional
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        long cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
        {
            delete this;
        }
        return cRef;
    }

    // IExplorerCommand
    IFACEMETHODIMP GetTitle(IShellItemArray * /* psiItemArray */, LPWSTR *ppszName)
    {
        // the verb name can be computed here, in this example it is static
        LPCWSTR pszMenuItem = GetStringByKey(SHELL_EXT_DISPLAY_NAME);
        return SHStrDup(pszMenuItem, ppszName);
    }

    IFACEMETHODIMP GetIcon(IShellItemArray * /* psiItemArray */, LPWSTR *ppszIcon)
    {
        // the icon ref ("dll,-<resid>") is provied here, in this case none is provieded
        *ppszIcon = NULL;
        return E_NOTIMPL;
    }

    IFACEMETHODIMP GetToolTip(IShellItemArray * /* psiItemArray */, LPWSTR *ppszInfotip)
    {
        // tooltip provided here, in this case none is provieded
        *ppszInfotip = NULL;
        return E_NOTIMPL;
    }

    IFACEMETHODIMP GetCanonicalName(GUID* pguidCommandName)
    {
        *pguidCommandName = __uuidof(this);
        return S_OK;
    }

    // compute the visibility of the verb here, respect "fOkToBeSlow" if this is slow (does IO for example)
    // when called with fOkToBeSlow == FALSE return E_PENDING and this object will be called
    // back on a background thread with fOkToBeSlow == TRUE
    IFACEMETHODIMP GetState(IShellItemArray * /* psiItemArray */, BOOL /*fOkToBeSlow*/, EXPCMDSTATE* pCmdState)
    {
        *pCmdState = ECS_ENABLED;
        return S_OK;
    }

    IFACEMETHODIMP Invoke(IShellItemArray *psiItemArray, IBindCtx *pbc);

    IFACEMETHODIMP GetFlags(EXPCMDFLAGS *pFlags)
    {
        *pFlags = ECF_DEFAULT;
        return S_OK;
    }

    IFACEMETHODIMP EnumSubCommands(IEnumExplorerCommand **ppEnum)
    {
        *ppEnum = NULL;
        return E_NOTIMPL;
    }

    // IInitializeCommand
    IFACEMETHODIMP Initialize(PCWSTR /* pszCommandName */, IPropertyBag * /* ppb */)
    {
        // the verb name is in pszCommandName, this handler can vary its behavior
        // based on the command name (implementing different verbs) or the
        // data stored under that verb in the registry can be read via ppb
        return S_OK;
    }

    // IObjectWithSite
    IFACEMETHODIMP SetSite(IUnknown *punkSite)
    {
        SetInterface(&_punkSite, punkSite);
        return S_OK;
    }

    IFACEMETHODIMP GetSite(REFIID riid, void **ppv)
    {
        *ppv = NULL;
        return _punkSite ? _punkSite->QueryInterface(riid, ppv) : E_FAIL;
    }

private:
    ~CExplorerCommandVerb()
    {
        SafeRelease(&_punkSite);
        SafeRelease(&_pstmShellItemArray);
        DllRelease();
    }

    DWORD _ThreadProc();

    static DWORD __stdcall s_ThreadProc(void *pv)
    {
        CExplorerCommandVerb *pecv = (CExplorerCommandVerb *)pv;
        const DWORD ret = pecv->_ThreadProc();
        pecv->Release();
        return ret;
    }

    long _cRef;
    IUnknown *_punkSite;
    HWND _hwnd;
    IStream *_pstmShellItemArray;
};

__inline HRESULT ResolveWindowsAppExePath(LPCWSTR pszExecName, LPWSTR pszPath)
{
    WCHAR szUserPath[MAX_PATH];
    HRESULT hr = SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, szUserPath);
    if (FAILED(hr))
    {
        return hr;
    }

    StringCchPrintf(pszPath, MAX_PATH, L"%s\\AppData\\Local\\Microsoft\\WindowsApps\\%s",
        szUserPath, pszExecName);

    return 0;
}

DWORD CExplorerCommandVerb::_ThreadProc()
{
    IShellItemArray *psia;
    HRESULT hr = CoGetInterfaceAndReleaseStream(_pstmShellItemArray, IID_PPV_ARGS(&psia));
    _pstmShellItemArray = NULL;
    if (SUCCEEDED(hr))
    {
        WCHAR szExecPath[MAX_PATH];
        hr = ResolveWindowsAppExePath(c_szExecPath, szExecPath);
        if (SUCCEEDED(hr))
        {
            sunjwbase::tstring tstrExecPath(szExecPath); // executable path
            sunjwbase::tstring tstrExecCmd = L"\"" + tstrExecPath + L"\""; // full commandline

            DWORD shellItemCount;
            psia->GetCount(&shellItemCount);

            for (DWORD i = 0; i < shellItemCount; i++)
            {
                IShellItem2* psi;
                hr = GetItemAt(psia, i, IID_PPV_ARGS(&psi));
                if (SUCCEEDED(hr))
                {
                    PWSTR pszPath;
                    hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                    if (SUCCEEDED(hr))
                    {
                        sunjwbase::tstring tstrItemPath(pszPath);
                        CoTaskMemFree(pszPath);

                        tstrExecCmd.append(L" \"");
                        tstrExecCmd.append(tstrItemPath);
                        tstrExecCmd.append(L"\"");
                    }
                    psi->Release();
                }
            }

            size_t sizeExecCmdLen = tstrExecCmd.length() + 1;
            WCHAR* pszCmd = new WCHAR[sizeExecCmdLen];
            memset(pszCmd, 0, sizeof(WCHAR) * sizeExecCmdLen);
            wcscpy_s(pszCmd, sizeExecCmdLen, tstrExecCmd.c_str());

            // MessageBox(_hwnd, tstrExecCmd.c_str(), tstrExecPath.c_str(), MB_OK);

            STARTUPINFO sInfo = { 0 };
            sInfo.cb = sizeof(sInfo);
            PROCESS_INFORMATION pInfo = { 0 };

            CreateProcess(tstrExecPath.c_str(), pszCmd,
                0, 0, TRUE,
                NORMAL_PRIORITY_CLASS,
                0, 0, &sInfo, &pInfo);

            delete[] pszCmd;

            /*WCHAR szMsg[128];
            StringCchPrintf(szMsg, ARRAYSIZE(szMsg), L"%d item(s), first item is [%s]", count, pszPath);

            MessageBox(_hwnd, szMsg, L"ExplorerCommand Sample Verb", MB_OK);

            CoTaskMemFree(pszPath);*/
        }
        psia->Release();
    }

    return 0;
}

IFACEMETHODIMP CExplorerCommandVerb::Invoke(IShellItemArray *psia, IBindCtx * /* pbc */)
{
    IUnknown_GetWindow(_punkSite, &_hwnd);

    HRESULT hr = CoMarshalInterThreadInterfaceInStream(__uuidof(psia), psia, &_pstmShellItemArray);
    if (SUCCEEDED(hr))
    {
        AddRef();
        if (!SHCreateThread(s_ThreadProc, this, CTF_COINIT_STA | CTF_PROCESS_REF, NULL))
        {
            Release();
        }
    }
    return S_OK;
}

HRESULT CExplorerCommandVerb_RegisterUnRegister(bool fRegister)
{
    CRegisterExtension re(__uuidof(CExplorerCommandVerb));

    HRESULT hr;
    if (fRegister)
    {
        hr = re.RegisterInProcServer(c_szVerbDisplayNameBack, L"Apartment");
        if (SUCCEEDED(hr))
        {
            // register this verb on ProgID
            hr = re.RegisterExplorerCommandVerb(c_szProgID, c_szVerbName, c_szVerbDisplayNameBack);
            if (SUCCEEDED(hr))
            {
                hr = re.RegisterVerbAttribute(c_szProgID, c_szVerbName, L"NeverDefault");
            }
        }
    }
    else
    {
        // best effort
        hr = re.UnRegisterVerb(c_szProgID, c_szVerbName);
        hr = re.UnRegisterObject();
    }
    return hr;
}

HRESULT CExplorerCommandVerb_CreateInstance(REFIID riid, void **ppv)
{
    *ppv = NULL;
    CExplorerCommandVerb *pVerb = new (std::nothrow) CExplorerCommandVerb();
    HRESULT hr = pVerb ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr))
    {
        pVerb->QueryInterface(riid, ppv);
        pVerb->Release();
    }
    return hr;
}
