#include "AppxShellExtStringsBase.h"

#include <tchar.h>

AppxShlExtStringsBase::AppxShlExtStringsBase()
{
	// Shell ext
#if defined (_DEBUG)
	m_stringsMap[_T("SHELL_EXT_DISPLAY_NAME")] = _T("Hash with fNext DEV");
#else
	m_stringsMap[_T("SHELL_EXT_DISPLAY_NAME")] = _T("Hash with fNext");
#endif
}
