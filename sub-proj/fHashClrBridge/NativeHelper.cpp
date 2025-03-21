#include "stdafx.h"

#include "NativeHelper.h"
#include "WinCommon/WindowsComm.h"
#include "WinCommon/ClipboardHelper.h"
#include "ClrHelper.h"
#include "Common/strhelper.h"

using namespace std;
using namespace System;
using namespace SunJWBase;
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
	String^ sstrTarget = ConvertTstrToSystemString(tstrTarget.c_str());
	return sstrTarget;
}

String^ NativeHelper::GetWindowsInfo()
{
	tstring tstrWinInfo = WindowsComm::GetWindowsInfo();
	String^ sstrWinInfo = ConvertTstrToSystemString(tstrWinInfo.c_str());
	return sstrWinInfo;
}

Boolean NativeHelper::SetClipboardText(String^ sstrCopyText)
{
	tstring tstrCopyText = ConvertSystemStringToTstr(sstrCopyText);
	return WindowsComm::SetClipboardText(tstrCopyText);
}
