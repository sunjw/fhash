#ifndef _WINDOWS_STRINGS_BASE_H_
#define _WINDOWS_STRINGS_BASE_H_

#include <tchar.h>
#include <map>
#include "strhelper.h"

namespace WindowsStrings
{
	typedef std::map<sunjwbase::tstring, sunjwbase::tstring> StringMap;

	class WindowsStringsMap
	{
	public:
		WindowsStringsMap() {}
		virtual ~WindowsStringsMap() {}

		const TCHAR *getStringByKey(const TCHAR *tzhKey);

	protected:
		StringMap m_stringsMap;
	};

	// WindowsStringsBase is a special StringsMap.
	class WindowsStringsBase: public WindowsStringsMap
	{
	public:
		WindowsStringsBase();
	};
}

#endif
