#include "stdafx.h"

#include "fHashShlExtStringsZHCN.h"

#include <tchar.h>

fHashShlExtStringsZHCN::fHashShlExtStringsZHCN()
{
	// Shell ext
	m_stringsMap[_T("SHELL_EXT_ITEM")] = _T("使用 fHash 计算哈希");
	m_stringsMap[_T("SHELL_EXT_TOO_MANY_FILES")] = _T("选择的文件太多");
}
