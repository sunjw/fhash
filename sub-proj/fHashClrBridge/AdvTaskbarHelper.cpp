#include "stdafx.h"
#include "AdvTaskbarHelper.h"

using namespace SunJWBase;

AdvTaskbarHelper::AdvTaskbarHelper(HWND hWnd):
	m_hWnd(hWnd), m_pTaskbarList3(NULL)
{
	ITaskbarList3 *pTaskbarListTemp = NULL;
	if (SUCCEEDED(WindowsComm::InitTaskbarList3(&pTaskbarListTemp)))
		m_pTaskbarList3 = pTaskbarListTemp;
}
