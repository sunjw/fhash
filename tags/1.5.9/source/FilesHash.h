// MD5SUM03.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号

// CMD5SUM03App:
// 有关此类的实现，请参阅 MD5SUM03.cpp
//

class CFILESHASHApp : public CWinApp
{
public:
	CFILESHASHApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CFILESHASHApp theApp;
