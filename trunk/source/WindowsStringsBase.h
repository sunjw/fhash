#ifndef _WINDOWS_STRINGS_BASE_H_
#define _WINDOWS_STRINGS_BASE_H_

#include <tchar.h>
#include <map>
#include "WindowsStringsInternal.h"

namespace WindowsStrings
{
	class WindowsStringsBase: public WindowsStringsMap
	{
	public:
		WindowsStringsBase();
	};
}

#endif
