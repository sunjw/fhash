#ifndef _WINDOWS_STRINGS_H_
#define _WINDOWS_STRINGS_H_

#include <tchar.h>

#define GetStringByKey(key) WindowsStrings::GetStringByStringKey(_T( #key ))

namespace WindowsStrings
{
	const TCHAR *GetStringByStringKey(const TCHAR *tzhKey);
}

#endif
