#include "UwpShellExtStringsZHCN.h"

#include <tchar.h>

UwpShlExtStringsZHCN::UwpShlExtStringsZHCN()
{
	// Shell ext
#if defined (_DEBUG)
	m_stringsMap[_T("SHELL_EXT_DISPLAY_NAME")] = _T("使用 fHash DEV 计算哈希");
#else
	m_stringsMap[_T("SHELL_EXT_DISPLAY_NAME")] = _T("使用 fHash 计算哈希");
#endif
}
