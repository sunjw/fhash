#include "stdafx.h"

#include "WindowsStringsInternal.h"

#include "OsUtils/OsThread.h"

using namespace sunjwbase;

namespace WindowsStrings
{
	const TCHAR *WindowsStringsMap::getStringByKey(const TCHAR *tzhKey)
	{
		StringMap::iterator itr = m_stringsMap.find(tstring(tzhKey));
		if (itr == m_stringsMap.end())
			return NULL;

		return itr->second.c_str();
	}

	OsMutex WindowsStringsMgr::s_instMtx;
	WindowsStringsMgr *WindowsStringsMgr::s_winStrsMgrInst = NULL;

	WindowsStringsMgr::WindowsStringsMgr()
	{
	}

	WindowsStringsMgr::~WindowsStringsMgr()
	{
		LangStringsMap::iterator itr = m_langStrsMap.begin();
		for (; itr != m_langStrsMap.end(); ++itr)
		{
			WindowsStringsMap *ptr = itr->second;
			delete ptr;
			itr->second = NULL;
		}
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

	int WindowsStringsMgr::registerStringsForLang(LANGID langId, WindowsStringsMap *stringsMap)
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

	const TCHAR *WindowsStringsMgr::getStringForLang(LANGID langId, const TCHAR *tzhKey)
	{
		const TCHAR *tzhRet = NULL;

		// Find string in lang id.
		LangStringsMap::iterator itrLang = m_langStrsMap.find(langId);
		if (itrLang != m_langStrsMap.end())
		{
			WindowsStringsMap *langStringsMap = itrLang->second;
			tzhRet = langStringsMap->getStringByKey(tzhKey);
		}

		// Find string in base.
		itrLang = m_langStrsMap.find(-1);
		if (tzhRet == NULL && itrLang != m_langStrsMap.end())
		{
			WindowsStringsMap *baseStringsMap = itrLang->second;
			tzhRet = baseStringsMap->getStringByKey(tzhKey);
		}

		if (tzhRet == NULL)
		{
			tzhRet = tzhKey;
		}

		return tzhRet;
	}

}
