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
		AppendFileNameToTstring(result, m_uiTstrAll);
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileMeta(const ResultData& result)
{
	lockData();
	{
		AppendFileMetaToTstring(result, m_uiTstrAll);
	}
	unlockData();
	
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileHash(const ResultData& result, bool uppercase)
{
	lockData();
	{
		AppendFileHashToTstring(result, uppercase, m_uiTstrAll);
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileErr(const ResultData& result)
{
	lockData();
	{
		AppendFileErrToTstring(result, m_uiTstrAll);
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
	//::PostMessage(m_hWnd, WM_THREAD_INFO, WP_PROG, value);
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

void UIBridgeMFC::AppendFileNameToTstring(const ResultData& result,
										tstring *tString)
{
	tString->append(GetStringByKey(FILENAME_STRING));
	tString->append(_T(" "));
	tString->append(result.tstrPath);
	tString->append(_T("\r\n"));
}

void UIBridgeMFC::AppendFileMetaToTstring(const ResultData& result,
										tstring *tString)
{
	char chSizeBuff[1024] = {0};
	sprintf_s(chSizeBuff, 1024, "%I64u", result.ulSize);
	tstring tstrFileSize = strtotstr(string(chSizeBuff));
	tstring tstrShortSize = strtotstr(Utils::ConvertSizeToShortSizeStr(result.ulSize));

	tString->append(GetStringByKey(FILESIZE_STRING));
	tString->append(_T(" "));
	tString->append(tstrFileSize);
	tString->append(_T(" "));
	tString->append(GetStringByKey(BYTE_STRING));
	if (tstrShortSize.length() > 0)
	{
		tString->append(_T(" ("));
		tString->append(tstrShortSize);
		tString->append(_T(")"));
	}
	tString->append(_T("\r\n"));
	tString->append(GetStringByKey(MODIFYTIME_STRING));
	tString->append(_T(" "));
	tString->append(result.tstrMDate);

	if (result.tstrVersion != _T(""))
	{
		tString->append(_T("\r\n"));
		tString->append(GetStringByKey(VERSION_STRING));
		tString->append(_T(" "));
		tString->append(result.tstrVersion);
	}

	tString->append(_T("\r\n"));

}

void UIBridgeMFC::AppendFileHashToTstring(const ResultData& result,
										bool uppercase,
										tstring *tString)
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

	tString->append(_T("MD5: "));
	tString->append(tstrFileMD5);
	tString->append(_T(" http://baidu.com"));
	tString->append(_T("\r\nSHA1: "));
	tString->append(tstrFileSHA1);
	tString->append(_T(" http://baidu.com"));
	tString->append(_T("\r\nSHA256: "));
	tString->append(tstrFileSHA256);
	tString->append(_T(" http://baidu.com"));
	tString->append(_T("\r\nCRC32: "));
	tString->append(tstrFileCRC32);
	tString->append(_T(" http://baidu.com"));
	tString->append(_T("\r\n\r\n"));
}

void UIBridgeMFC::AppendFileErrToTstring(const ResultData& result,
										tstring *tString)
{
	tString->append(result.tstrError);
	tString->append(_T("\r\n\r\n"));
}

void UIBridgeMFC::AppendResultToTstring(const ResultData& result,
										bool uppercase,
										tstring *tString)
{
	if (result.enumState == RESULT_NONE)
		return;

	if (result.enumState == RESULT_ALL ||
		result.enumState == RESULT_META ||
		result.enumState == RESULT_ERROR ||
		result.enumState == RESULT_PATH) 
	{
			AppendFileNameToTstring(result, tString);
	}

	if (result.enumState == RESULT_ALL ||
		result.enumState == RESULT_META) 
	{
			AppendFileMetaToTstring(result, tString);
	}

	if (result.enumState == RESULT_ALL) 
	{
		AppendFileHashToTstring(result, uppercase, tString);
	}

	if (result.enumState == RESULT_ERROR) 
	{
		AppendFileErrToTstring(result, tString);
	}

	if (result.enumState != RESULT_ALL &&
		result.enumState != RESULT_ERROR) 
	{
			tstring tstrAppend = _T("\r\n");
			tString->append(tstrAppend);
	}
}
