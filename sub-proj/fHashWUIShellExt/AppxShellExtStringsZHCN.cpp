#include "AppxShellExtStringsZHCN.h"

#include <tchar.h>

AppxShlExtStringsZHCN::AppxShlExtStringsZHCN()
{
	// Shell ext
#if defined (_DEBUG)
	m_stringsMap[_T("SHELL_EXT_DISPLAY_NAME")] = _T("使用 fNext DEV 计算哈希");
#else
	m_stringsMap[_T("SHELL_EXT_DISPLAY_NAME")] = _T("使用 fNext 计算哈希");
#endif
}
