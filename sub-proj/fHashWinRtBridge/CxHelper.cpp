#include "stdafx.h"
#include "CxHelper.h"

using namespace Platform;
using namespace sunjwbase;

String^ sunjwbase::tstrToPlatStr(const tstring& tstrSource)
{
	return (ref new String(tstrSource.c_str()));
}
