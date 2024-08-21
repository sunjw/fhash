#pragma once
#include <string>
#include <Windows.h>

namespace TestCLRBridge
{
    std::string ConvertToStdString(System::String^ mstr);

    std::wstring ConvertToStdWstring(System::String^ mstr);

    System::String^ ConvertStrToSystemString(LPCSTR lpStrSource);

    System::String^ ConvertWstrToSystemString(LPCWSTR lpWstrSource);

#if defined(WIN32) && (defined(UNICODE) || defined(_UNICODE))
#define ConvertTstrToSystemString ConvertWstrToSystemString
#else
#define ConvertTstrToSystemString ConvertStrToSystemString
#endif
}
