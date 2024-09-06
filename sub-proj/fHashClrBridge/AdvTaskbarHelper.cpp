#include "stdafx.h"
#include "AdvTaskbarHelper.h"

using namespace System;
using namespace SunJWBase;

AdvTaskbarHelper::AdvTaskbarHelper(IntPtr hWnd)
	:m_hWnd((HWND)hWnd.ToPointer()), m_pTaskbarList3(NULL)
{
	ITaskbarList3 *pTaskbarListTemp = NULL;
	if (SUCCEEDED(WindowsComm::InitTaskbarList3(&pTaskbarListTemp)))
		m_pTaskbarList3 = pTaskbarListTemp;
}

void AdvTaskbarHelper::SetTaskbarProg(ULONGLONG ullValue)
{
	if (m_hWnd == NULL || m_pTaskbarList3 == NULL)
		return;
	WindowsComm::SetTaskbarList3Value(m_pTaskbarList3, m_hWnd, ullValue);
}
