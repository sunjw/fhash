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
#define ConvertTstrToSystemString ConvertWstrToSystemString
#else
#define ConvertTstrToSystemString ConvertStrToSystemString
#endif
}
