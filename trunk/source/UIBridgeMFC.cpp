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
						 OsMutex *mainMtx,
						 CHyperEditHash *hyperEdit)
:m_hWnd(hWnd), m_mainMtx(mainMtx), m_mainHyperEdit(hyperEdit)
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
		m_tstrNoPreparing = m_mainHyperEdit->GetTextBuffer().GetBuffer();
		// Save link offsets
		m_mainHyperEdit->CopyLinkOffsets(m_offsetsNoPreparing);
		if (m_tstrNoPreparing == tstring(GetStringByKey(MAINDLG_INITINFO)))
		{
			// Initial state
			m_tstrNoPreparing = _T("");
			m_offsetsNoPreparing.clear();
			m_mainHyperEdit->ClearTextBuffer();
		}

		m_mainHyperEdit->AppendTextToBuffer(GetStringByKey(MAINDLG_WAITING_START));
		m_mainHyperEdit->AppendTextToBuffer(_T("\r\n"));
	}
	unlockData();
	
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::removePreparingCalc()
{
	lockData();
	{
		// Restore and remove MAINDLG_WAITING_START
		m_mainHyperEdit->ClearTextBuffer();
		m_mainHyperEdit->AppendTextToBuffer(m_tstrNoPreparing.c_str());
		// Reset link offsets
		m_mainHyperEdit->SetLinkOffsets(m_offsetsNoPreparing);
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
		AppendFileNameToHyperEdit(result, m_mainHyperEdit);
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileMeta(const ResultData& result)
{
	lockData();
	{
		AppendFileMetaToHyperEdit(result, m_mainHyperEdit);
	}
	unlockData();
	
	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileHash(const ResultData& result, bool uppercase)
{
	lockData();
	{
		AppendFileHashToHyperEdit(result, uppercase, m_mainHyperEdit);
	}
	unlockData();

	::PostMessage(m_hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
}

void UIBridgeMFC::showFileErr(const ResultData& result)
{
	lockData();
	{
		AppendFileErrToHyperEdit(result, m_mainHyperEdit);
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

void UIBridgeMFC::AppendFileNameToHyperEdit(const ResultData& result,
											CHyperEditHash *hyerEdit)
{
	hyerEdit->AppendTextToBuffer(GetStringByKey(FILENAME_STRING));
	hyerEdit->AppendTextToBuffer(_T(" "));
	hyerEdit->AppendTextToBuffer(result.tstrPath.c_str());
	hyerEdit->AppendTextToBuffer(_T("\r\n"));
}

void UIBridgeMFC::AppendFileMetaToHyperEdit(const ResultData& result,
											CHyperEditHash *hyerEdit)
{
	char chSizeBuff[1024] = {0};
	sprintf_s(chSizeBuff, 1024, "%I64u", result.ulSize);
	tstring tstrFileSize = strtotstr(string(chSizeBuff));
	tstring tstrShortSize = strtotstr(Utils::ConvertSizeToShortSizeStr(result.ulSize));

	hyerEdit->AppendTextToBuffer(GetStringByKey(FILESIZE_STRING));
	hyerEdit->AppendTextToBuffer(_T(" "));
	hyerEdit->AppendTextToBuffer(tstrFileSize.c_str());
	hyerEdit->AppendTextToBuffer(_T(" "));
	hyerEdit->AppendTextToBuffer(GetStringByKey(BYTE_STRING));
	if (tstrShortSize.length() > 0)
	{
		hyerEdit->AppendTextToBuffer(_T(" ("));
		hyerEdit->AppendTextToBuffer(tstrShortSize.c_str());
		hyerEdit->AppendTextToBuffer(_T(")"));
	}
	hyerEdit->AppendTextToBuffer(_T("\r\n"));
	hyerEdit->AppendTextToBuffer(GetStringByKey(MODIFYTIME_STRING));
	hyerEdit->AppendTextToBuffer(_T(" "));
	hyerEdit->AppendTextToBuffer(result.tstrMDate.c_str());

	if (result.tstrVersion != _T(""))
	{
		hyerEdit->AppendTextToBuffer(_T("\r\n"));
		hyerEdit->AppendTextToBuffer(GetStringByKey(VERSION_STRING));
		hyerEdit->AppendTextToBuffer(_T(" "));
		hyerEdit->AppendTextToBuffer(result.tstrVersion.c_str());
	}

	hyerEdit->AppendTextToBuffer(_T("\r\n"));

}

void UIBridgeMFC::AppendFileHashToHyperEdit(const ResultData& result,
											bool uppercase,
											CHyperEditHash *hyerEdit)
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

	hyerEdit->AppendTextToBuffer(_T("MD5: "));
	hyerEdit->AppendLinkToBuffer(tstrFileMD5.c_str());
	hyerEdit->AppendTextToBuffer(_T("\r\nSHA1: "));
	hyerEdit->AppendLinkToBuffer(tstrFileSHA1.c_str());
	hyerEdit->AppendTextToBuffer(_T("\r\nSHA256: "));
	hyerEdit->AppendLinkToBuffer(tstrFileSHA256.c_str());
	hyerEdit->AppendTextToBuffer(_T("\r\nCRC32: "));
	hyerEdit->AppendLinkToBuffer(tstrFileCRC32.c_str());
	hyerEdit->AppendTextToBuffer(_T("\r\n\r\n"));
}

void UIBridgeMFC::AppendFileErrToHyperEdit(const ResultData& result,
											CHyperEditHash *hyerEdit)
{
	hyerEdit->AppendTextToBuffer(result.tstrError.c_str());
	hyerEdit->AppendTextToBuffer(_T("\r\n\r\n"));
}

void UIBridgeMFC::AppendResultToHyperEdit(const ResultData& result,
											bool uppercase,
											CHyperEditHash *hyerEdit)
{
	if (result.enumState == RESULT_NONE)
		return;

	if (result.enumState == RESULT_ALL ||
		result.enumState == RESULT_META ||
		result.enumState == RESULT_ERROR ||
		result.enumState == RESULT_PATH) 
	{
		AppendFileNameToHyperEdit(result, hyerEdit);
	}

	if (result.enumState == RESULT_ALL ||
		result.enumState == RESULT_META) 
	{
		AppendFileMetaToHyperEdit(result, hyerEdit);
	}

	if (result.enumState == RESULT_ALL) 
	{
		AppendFileHashToHyperEdit(result, uppercase, hyerEdit);
	}

	if (result.enumState == RESULT_ERROR) 
	{
		AppendFileErrToHyperEdit(result, hyerEdit);
	}

	if (result.enumState != RESULT_ALL &&
		result.enumState != RESULT_ERROR) 
	{
		tstring tstrAppend = _T("\r\n");
		hyerEdit->AppendTextToBuffer(tstrAppend.c_str());
	}
}
