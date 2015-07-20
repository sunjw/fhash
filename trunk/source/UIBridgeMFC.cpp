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
#include "UIStrings.h"

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
		m_uiTstrAll->append(MAINDLG_WAITING_START);
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
		m_uiTstrAll->append(FILENAME_STRING);
		m_uiTstrAll->append(_T(" "));
		m_uiTstrAll->append(result.tstrPath);
		m_uiTstrAll->append(_T("\r\n"));
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileMeta(const ResultData& result)
{
	char chSizeBuff[1024] = {0};
	sprintf_s(chSizeBuff, 1024, "%I64u", result.ulSize);
	tstring tstrFileSize = strtotstr(string(chSizeBuff));

	tstring tstrShortSize = strtotstr(Utils::ConvertSizeToShortSizeStr(result.ulSize));

	lockData();
	{
		m_uiTstrAll->append(FILESIZE_STRING);
		m_uiTstrAll->append(_T(" "));
		m_uiTstrAll->append(tstrFileSize);
		m_uiTstrAll->append(_T(" "));
		m_uiTstrAll->append(BYTE_STRING);
		m_uiTstrAll->append(tstrShortSize);
		m_uiTstrAll->append(_T("\r\n"));
		m_uiTstrAll->append(MODIFYTIME_STRING);
		m_uiTstrAll->append(_T(" "));
		m_uiTstrAll->append(result.tstrMDate);

		if(result.tstrVersion != _T(""))
		{
			m_uiTstrAll->append(_T("\r\n"));
			m_uiTstrAll->append(VERSION_STRING);
			m_uiTstrAll->append(_T(" "));
			m_uiTstrAll->append(result.tstrVersion);
		}

		m_uiTstrAll->append(_T("\r\n"));
	}
	unlockData();
	
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileHash(const ResultData& result, bool uppercase)
{
	tstring tstrFileMD5, tstrFileSHA1, tstrFileSHA256, tstrFileCRC32;
	
	if (uppercase)
	{
		tstrFileMD5 = strtotstr(str_upper(tstrtostr(result.tstrMD5)));
		tstrFileSHA1 = strtotstr(str_upper(tstrtostr(result.tstrSHA1)));
		tstrFileSHA256 = strtotstr(str_upper(tstrtostr(result.tstrSHA256)));
		tstrFileCRC32 = strtotstr(str_upper(tstrtostr(result.tstrCRC32)));
	}
	else
	{
		tstrFileMD5 = strtotstr(str_lower(tstrtostr(result.tstrMD5)));
		tstrFileSHA1 = strtotstr(str_lower(tstrtostr(result.tstrSHA1)));
		tstrFileSHA256 = strtotstr(str_lower(tstrtostr(result.tstrSHA256)));
		tstrFileCRC32 = strtotstr(str_lower(tstrtostr(result.tstrCRC32)));
	}

	lockData();
	{
		m_uiTstrAll->append(_T("MD5: "));
		m_uiTstrAll->append(tstrFileMD5);
		m_uiTstrAll->append(_T("\r\nSHA1: "));
		m_uiTstrAll->append(tstrFileSHA1);
		m_uiTstrAll->append(_T("\r\nSHA256: "));
		m_uiTstrAll->append(tstrFileSHA256);
		m_uiTstrAll->append(_T("\r\nCRC32: "));
		m_uiTstrAll->append(tstrFileCRC32);
		m_uiTstrAll->append(_T("\r\n\r\n"));
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileErr(const ResultData& result)
{
	lockData();
	{
		m_uiTstrAll->append(result.tstrError);
		m_uiTstrAll->append(_T("\r\n\r\n"));
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
