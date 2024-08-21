#include "stdafx.h"

#include "ClrHelper.h"
#include <string>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

std::string TestCLRBridge::ConvertToStdString(System::String^ mstr)
{
    return msclr::interop::marshal_as<std::string>(mstr);
}

std::wstring TestCLRBridge::ConvertToStdWstring(System::String^ mstr)
{
    return msclr::interop::marshal_as<std::wstring>(mstr);
}

System::String^ TestCLRBridge::ConvertStrToSystemString(LPCSTR lpStrSource)
{
    return msclr::interop::marshal_as<System::String^>(lpStrSource);
}

System::String^ TestCLRBridge::ConvertWstrToSystemString(LPCWSTR lpWstrSource)
{
    return msclr::interop::marshal_as<System::String^>(lpWstrSource);
}
