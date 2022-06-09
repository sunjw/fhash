#ifndef _WINDOWS_COMM_H_
#define _WINDOWS_COMM_H_

#include <string>
#include <Windows.h>
#include <stdint.h>

#include "Common/strhelper.h"

namespace WindowsComm
{
	sunjwbase::tstring GetExeFileVersion(TCHAR* path);

	BOOL GetWindowsVersion(OSVERSIONINFOEX& osvi, BOOL& bOsVersionInfoEx);
	sunjwbase::tstring GetWindowsInfo();
}

#endif
