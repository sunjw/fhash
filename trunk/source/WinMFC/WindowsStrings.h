#ifndef _WINDOWS_STRINGS_H_
#define _WINDOWS_STRINGS_H_

#include <tchar.h>
#include <Windows.h>
#include <map>
#include "Common/strhelper.h"

#define GetStringByKey(key) WindowsStrings::GetStringByStringKey(_T( #key ))

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

	int RegisterStringsForLang(LANGID langId, WindowsStringsMap *stringsMap);

	LANGID GetCurrentUILang();

	const TCHAR *GetStringByStringKey(const TCHAR *tzhKey);
}

#endif
