// fileshash.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "fileshash.h"
#include "fileshashdlg.h"
#include "WindowsStrings.h"
#include "UIStringsBase.h"
#include "UIStringsZHCN.h"

//#define MEMORY_LEAK_TEST
#undef MEMORY_LEAK_TEST

// Memory leak test
#if defined (MEMORY_LEAK_TEST)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
// Memory leak test


using namespace WindowsStrings;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFILESHASHApp

BEGIN_MESSAGE_MAP(CFILESHASHApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFILESHASHApp 构造

CFILESHASHApp::CFILESHASHApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFILESHASHApp 对象

CFILESHASHApp theApp;


// CFILESHASHApp 初始化

BOOL CFILESHASHApp::InitInstance()
{
	// Memory leak test
#if defined (MEMORY_LEAK_TEST)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	// Memory leak test

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。

	InitCommonControls();

	CWinApp::InitInstance();

	// Register multi-lang.
	RegisterStringsForLang(-1, new UIStringsBase());
	RegisterStringsForLang(2052, new UIStringsZHCN());

	CFilesHashDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO：在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO：在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	return FALSE;
}
