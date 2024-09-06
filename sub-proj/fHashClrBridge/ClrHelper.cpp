#include "stdafx.h"

#include "ClrHelper.h"
#include <string>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

using namespace std;
using namespace msclr::interop;
using namespace System;

std::string sunjwbase::ConvertSystemStringToStdString(System::String^ mstr)
{
	return marshal_as<std::string>(mstr);
}

std::wstring sunjwbase::ConvertSystemStringToStdWstring(System::String^ mstr)
{
	return marshal_as<std::wstring>(mstr);
}

System::String^ sunjwbase::ConvertStrToSystemString(LPCSTR lpStrSource)
{
	return marshal_as<System::String^>(lpStrSource);
}

System::String^ sunjwbase::ConvertWstrToSystemString(LPCWSTR lpWstrSource)
{
	return marshal_as<System::String^>(lpWstrSource);
}
