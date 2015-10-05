#include "stdafx.h"

#include "WindowsStringsInternal.h"

#include "WindowsStringsBase.h"
#include "OsUtils/OsThread.h"

using namespace sunjwbase;

namespace WindowsStrings
{
	OsMutex WindowsStringsMgr::s_instMtx;
	WindowsStringsMgr *WindowsStringsMgr::s_winStrsMgrInst = NULL;

	WindowsStringsMgr::WindowsStringsMgr()
	{
	}
	
	WindowsStringsMgr *WindowsStringsMgr::getInstance(void)
	{
		s_instMtx.lock();
		if (s_winStrsMgrInst == NULL)
		{
			s_winStrsMgrInst = new WindowsStringsMgr();
		}
		s_instMtx.unlock();

		return s_winStrsMgrInst;
	}

	int WindowsStringsMgr::registerStringsForLang(int langId, WindowsStringsMap *stringsMap)
	{
		m_langStrsMap[langId] = stringsMap;

		return 0;
	}

	void WindowsStringsMgr::resetInstance(void)
	{
		s_instMtx.lock();
		if (s_winStrsMgrInst != NULL)
		{
			delete s_winStrsMgrInst;
			s_winStrsMgrInst = NULL;
		}
		s_instMtx.unlock();
	}

	const TCHAR *WindowsStringsMgr::getStringForLang(int langId, const TCHAR *tzhKey)
	{
		const TCHAR *tzhRet = NULL;
		LangStringsMap::iterator itrLang = m_langStrsMap.find(langId);
		if (itrLang != m_langStrsMap.end())
		{
			WindowsStringsMap *langStringsMap = itrLang->second;
			tzhRet = langStringsMap->getStringByKey(tzhKey);
		}

		if (tzhRet == NULL)
		{
			tzhRet = m_stringsBase.getStringByKey(tzhKey);
		}

		if (tzhRet == NULL)
		{
			tzhRet = tzhKey;
		}

		return tzhRet;
	}

}
