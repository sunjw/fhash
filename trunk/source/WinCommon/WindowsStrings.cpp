#include "stdafx.h"

#include "WindowsStrings.h"

#include <Windows.h>
#include <tchar.h>
#include <memory>
#include "OsUtils/OsThread.h"
#include "Common/strhelper.h"
#include "WindowsStringsInternal.h"

using namespace std;
using namespace sunjwbase;

namespace WindowsStrings
{
	static OsMutex s_initMtx;
	static int s_init = 0;
	static shared_ptr<WindowsStringsMgr> s_winStrsMgr;
	static LANGID s_uiLang = -1;

	static void InitStringsManager()
	{
		s_initMtx.lock();

		s_winStrsMgr.reset(WindowsStringsMgr::getInstance());
		s_uiLang = GetUserDefaultUILanguage();

		s_init = 1;
		s_initMtx.unlock();
	}

	int RegisterStringsForLang(LANGID langId, WindowsStringsMap *stringsMap)
	{
		if (s_init == 0)
			InitStringsManager();

		return s_winStrsMgr->registerStringsForLang(langId, stringsMap);
	}

	LANGID GetCurrentUILang()
	{
		if (s_init == 0)
			InitStringsManager();

		return s_uiLang;
	}

	const TCHAR *GetStringByStringKey(const TCHAR *tzhKey)
	{
		if (s_init == 0)
			InitStringsManager();

		return s_winStrsMgr->getStringForLang(s_uiLang, tzhKey);
	}
}
