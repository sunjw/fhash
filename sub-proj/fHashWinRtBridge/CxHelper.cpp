#include "stdafx.h"
#include "CxHelper.h"

using namespace Platform;
using namespace sunjwbase;

String^ sunjwbase::ConvertToPlatStr(LPCWSTR lpStrSource)
{
	return (ref new String(lpStrSource));
}
