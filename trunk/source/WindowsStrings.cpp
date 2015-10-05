#include "stdafx.h"

#include "WindowsStrings.h"

#include <Windows.h>
#include <tchar.h>
#include <memory>
#include "OsUtils/OsThread.h"
#include "strhelper.h"
#include "WindowsStringsInternal.h"

using namespace std;
using namespace sunjwbase;

namespace WindowsStrings
{
	static OsMutex s_initMtx;
	static int s_init = 0;
	static tr1::shared_ptr<WindowsStringsMgr> s_winStrsMgr;
	static int s_lcid = -1;

	static void InitStringsManager()
	{
		s_initMtx.lock();

		s_winStrsMgr.reset(WindowsStringsMgr::getInstance());
		LCID userDefLcid = GetUserDefaultLCID();
		s_lcid = userDefLcid & 0xFFFF; // lower 16bit

		s_init = 1;
		s_initMtx.unlock();
	}

	const TCHAR *GetStringByStringKey(const TCHAR *tzhKey)
	{
		if (s_init == 0)
			InitStringsManager();

		return s_winStrsMgr->getStringForLang(s_lcid, tzhKey);
	}
}
