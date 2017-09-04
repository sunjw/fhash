// fHashShellExt.h : CfHashShellExt 的声明

#pragma once
#include "resource.h"       // 主符号

#include "fHashShlExt_i.h"
#include <string>
#include <list>
#include <shlobj.h>
#include "strhelper.h"

typedef std::list<sunjwbase::tstring> TstrList;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CfHashShellExt

class ATL_NO_VTABLE CfHashShellExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CfHashShellExt, &CLSID_fHashShellExt>,
	//public IDispatchImpl<IfHashShellExt, &IID_IfHashShellExt, &LIBID_fHashShlExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit, public IContextMenu 
{
public:
	CfHashShellExt();

DECLARE_REGISTRY_RESOURCEID(IDR_FHASHSHELLEXT)


BEGIN_COM_MAP(CfHashShellExt)
	//COM_INTERFACE_ENTRY(IfHashShellExt)
	//COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// IShellExtInit
	STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	// IContextMenu
	STDMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT);
	STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
	STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);

private:
	HRESULT LaunchfHashByCommandLine(LPCMINVOKECOMMANDINFO, BOOL);
	HWND FindfHashWindow();
	void SendFilesTofHash(LPCMINVOKECOMMANDINFO, HWND);

	sunjwbase::tstring m_fHashPath;
	TstrList m_pathList;

};

OBJECT_ENTRY_AUTO(__uuidof(fHashShellExt), CfHashShellExt)
