#ifndef _WINDOWS_STRINGS_INTERNAL_H_
#define _WINDOWS_STRINGS_INTERNAL_H_

#include <map>
#include <Windows.h>
#include <tchar.h>
#include "OsUtils/OsThread.h"
#include "WindowsStrings.h"

namespace WindowsStrings
{
	typedef std::map<LANGID, WindowsStringsMap *> LangStringsMap;

	class WindowsStringsMgr
	{
	public:
		virtual ~WindowsStringsMgr();

		static WindowsStringsMgr *getInstance(void);
		static void resetInstance(void);

		int registerStringsForLang(LANGID langId, WindowsStringsMap *stringsMap);

		const TCHAR *getStringForLang(LANGID langId, const TCHAR *tzhKey);

	private:
		WindowsStringsMgr();
		WindowsStringsMgr(const WindowsStringsMgr& other) {}

		static sunjwbase::OsMutex s_instMtx;
		static WindowsStringsMgr *s_winStrsMgrInst;
		
		LangStringsMap m_langStrsMap;
	};
}

#endif