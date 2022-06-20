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

String^ NativeHelper::GetWindowsInfo()
{
	tstring tstrWinInfo = WindowsComm::GetWindowsInfo();
	String^ pstrWinInfo = ConvertToPlatStr(tstrWinInfo.c_str());
	return pstrWinInfo;
}
