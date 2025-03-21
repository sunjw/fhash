#include "stdafx.h"

#include "ClipboardHelper.h"

using namespace std;
using namespace sunjwbase;

BOOL WindowsComm::SetClipboardText(const sunjwbase::tstring tstrCopyText)
{
    if (tstrCopyText.empty())
        return FALSE;

    if (!OpenClipboard(NULL))
        return FALSE;

    EmptyClipboard();

    HGLOBAL hClipboardMem = GlobalAlloc(GMEM_MOVEABLE, (tstrCopyText.length() + 1) * sizeof(TCHAR));
    if (hClipboardMem == NULL)
    {
        CloseClipboard();
        return FALSE;
    }

    LPTSTR ptstrData = static_cast<LPTSTR>(GlobalLock(hClipboardMem));
    if (ptstrData == NULL)
    {
        GlobalFree(hClipboardMem);
        CloseClipboard();
        return FALSE;
    }

    memcpy(ptstrData, tstrCopyText.c_str(), (tstrCopyText.length() + 1) * sizeof(TCHAR));
    GlobalUnlock(hClipboardMem);

    if (SetClipboardData(CF_UNICODETEXT, hClipboardMem) == NULL)
    {
        GlobalFree(hClipboardMem);
        CloseClipboard();
        return FALSE;
    }

    CloseClipboard();

    return TRUE;
}
