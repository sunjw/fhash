#ifndef _ADV_TASKBAR_H_
#define _ADV_TASKBAR_H_

#include <Windows.h>
#include <Shobjidl.h>

namespace WindowsComm
{
    HRESULT InitTaskbarList3(ITaskbarList3 **ppTaskbarList3);
    HRESULT SetTaskbarList3Value(ITaskbarList3 *pTaskbarList3, HWND hWnd, ULONGLONG ullValue);
}

#endif
