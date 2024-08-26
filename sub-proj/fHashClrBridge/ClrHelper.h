#pragma once

#include <string>
#include <Windows.h>

namespace sunjwbase
{
	std::string ConvertSystemStringToStdString(System::String^ mstr);

	std::wstring ConvertSystemStringToStdWstring(System::String^ mstr);

	System::String^ ConvertStrToSystemString(LPCSTR lpStrSource);

	System::String^ ConvertWstrToSystemString(LPCWSTR lpWstrSource);

#if defined(WIN32) && (defined(UNICODE) || defined(_UNICODE))
#define ConvertSystemStringToTstr ConvertSystemStringToStdWstring
#define ConvertTstrToSystemString ConvertWstrToSystemString
#else
#define ConvertSystemStringToTstr ConvertSystemStringToStdString
#define ConvertTstrToSystemString ConvertStrToSystemString
#endif
}
