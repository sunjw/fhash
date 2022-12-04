#include "stdafx.h"

#include "NativeHelper.h"
#include "WinCommon/WindowsComm.h"
#include "CxHelper.h"
#include "Common/strhelper.h"

using namespace std;
using namespace Platform;
using namespace FilesHashUwp;
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
	String^ pstrTarget = ConvertToPlatStr(tstrTarget.c_str());
	return pstrTarget;
}

String^ NativeHelper::GetWindowsInfo()
{
	tstring tstrWinInfo = WindowsComm::GetWindowsInfo();
	String^ pstrWinInfo = ConvertToPlatStr(tstrWinInfo.c_str());
	return pstrWinInfo;
}
