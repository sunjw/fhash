#include "stdafx.h"

#include "fHashShlExtStringsBase.h"

#include <tchar.h>

fHashShlExtStringsBase::fHashShlExtStringsBase()
{
	// Shell ext
	m_stringsMap[_T("SHELL_EXT_ITEM")] = _T("Hash with fHash");
	m_stringsMap[_T("SHELL_EXT_TOO_MANY_FILES")] = _T("Selected too many files");
}
