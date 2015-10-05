#include "stdafx.h"

#include "UIBridgeMFC.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <Windows.h>

#include "strhelper.h"
#include "Global.h"
#include "Utils.h"
#include "WindowsUtils.h"
#include "WindowsStrings.h"

using namespace std;
using namespace sunjwbase;

UIBridgeMFC::UIBridgeMFC(HWND hWnd, 
						tstring *tstrUIAll,
						OsMutex *mainMtx)
:m_hWnd(hWnd), m_uiTstrAll(tstrUIAll), m_mainMtx(mainMtx)
{
}

UIBridgeMFC::~UIBridgeMFC()
{
}

void UIBridgeMFC::lockData()
{
	m_mainMtx->lock();
}

void UIBridgeMFC::unlockData()
{
	m_mainMtx->unlock();
}

void UIBridgeMFC::preparingCalc()
{
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_WORKING, 0);
	
	lockData();
	{
		m_tstrNoPreparing = *m_uiTstrAll;
		m_uiTstrAll->append(GetStringByKey(MAINDLG_WAITING_START));
		m_uiTstrAll->append(_T("\r\n"));
	}
	unlockData();
	
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::removePreparingCalc()
{
	lockData();
	{
		// restore and remove MAINDLG_WAITING_START
		*m_uiTstrAll = m_tstrNoPreparing;
	}
	unlockData();
}

void UIBridgeMFC::calcStop()
{
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_STOPPED, 0);
}

void UIBridgeMFC::calcFinish()
{
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_FINISHED, 0);
}

void UIBridgeMFC::showFileName(const ResultData& result)
{
	lockData();
	{
		WindowsUtils::AppendFileNameToTstring(result, m_uiTstrAll);
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileMeta(const ResultData& result)
{
	lockData();
	{
		WindowsUtils::AppendFileMetaToTstring(result, m_uiTstrAll);
	}
	unlockData();
	
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileHash(const ResultData& result, bool uppercase)
{
	lockData();
	{
		WindowsUtils::AppendFileHashToTstring(result, uppercase, m_uiTstrAll);
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileErr(const ResultData& result)
{
	lockData();
	{
		WindowsUtils::AppendFileErrToTstring(result, m_uiTstrAll);
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

int UIBridgeMFC::getProgMax()
{
	return 100;
}

void UIBridgeMFC::updateProg(int value)
{
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_PROG, value);
}

void UIBridgeMFC::updateProgWhole(int value)
{
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_PROG_WHOLE, value);
}

void UIBridgeMFC::fileCalcFinish()
{
}

void UIBridgeMFC::fileFinish()
{
}
