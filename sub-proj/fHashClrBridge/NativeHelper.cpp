#include "stdafx.h"

#include "NativeHelper.h"
#include "WinCommon/WindowsComm.h"
#include "ClrHelper.h"
#include "Common/strhelper.h"

using namespace std;
using namespace System;
using namespace FilesHashWUI;
using namespace sunjwbase;

NativeHelper::NativeHelper()
{
}

String^ NativeHelper::GetTargetArch()
{
	tstring tstrTarget;
#if defined (_M_AMD64)
	tstrTarget = TEXT("x64");
#elif defined (_M_ARM64)
	tstrTarget = TEXT("arm64");
#endif
	String^ pstrTarget = ConvertTstrToSystemString(tstrTarget.c_str());
	return pstrTarget;
}

String^ NativeHelper::GetWindowsInfo()
{
	tstring tstrWinInfo = WindowsComm::GetWindowsInfo();
	String^ pstrWinInfo = ConvertTstrToSystemString(tstrWinInfo.c_str());
	return pstrWinInfo;
}
