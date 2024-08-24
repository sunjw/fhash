#include "stdafx.h"

#include "AdvTaskbar.h"

HRESULT WindowsComm::InitTaskbarList3(ITaskbarList3 **ppTaskbarList3)
{
    HRESULT hr = CoCreateInstance(
        CLSID_TaskbarList, NULL, CLSCTX_ALL,
        IID_ITaskbarList3, (void **)ppTaskbarList3);
    if (FAILED(hr))
    {
        *ppTaskbarList3 = NULL;
    }
    return hr;
}

HRESULT WindowsComm::SetTaskbarList3Value(ITaskbarList3 *pTaskbarList3, HWND hWnd, ULONGLONG ullValue)
{
    if (pTaskbarList3 && hWnd)
        return pTaskbarList3->SetProgressValue(hWnd, ullValue, 99);
    return E_FAIL;
}
