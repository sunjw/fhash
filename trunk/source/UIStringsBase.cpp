#include "stdafx.h"

#include "UIStringsBase.h"

#include <tchar.h>

UIStringsBase::UIStringsBase()
{
	// Global Strings
	m_stringsMap[_T("FILE_STRING")] = _T("File");
	m_stringsMap[_T("BYTE_STRING")] = _T("Byte(s)");
	m_stringsMap[_T("HASHVALUE_STRING")] = _T("Hash:");
	m_stringsMap[_T("FILENAME_STRING")] = _T("Name:");
	m_stringsMap[_T("FILESIZE_STRING")] = _T("File Size:");
	m_stringsMap[_T("MODIFYTIME_STRING")] = _T("Modified Date:");
	m_stringsMap[_T("VERSION_STRING")] = _T("Version:");
	m_stringsMap[_T("SECOND_STRING")] = _T("s");
	m_stringsMap[_T("BUTTON_OK")] = _T("OK");
	m_stringsMap[_T("BUTTON_CANCEL")] = _T("Cancel");

	// Main Dialog Strings
	m_stringsMap[_T("MAINDLG_INITINFO")] = _T("Drag files here or click open to start calculate.");
	m_stringsMap[_T("MAINDLG_WAITING_START")] = _T("Prepare to start calculation.");
	m_stringsMap[_T("MAINDLG_CONTEXT_INIT")] = _T("Need Administrator");
	m_stringsMap[_T("MAINDLG_ADD_SUCCEEDED")] = _T("Add Succeeded");
	m_stringsMap[_T("MAINDLG_ADD_FAILED")] = _T("Add Failed");
	m_stringsMap[_T("MAINDLG_REMOVE_SUCCEEDED")] = _T("Remove Succeeded");
	m_stringsMap[_T("MAINDLG_REMOVE_FAILED")] = _T("Remove Failed");
	m_stringsMap[_T("MAINDLG_REMOVE_CONTEXT_MENU")] = _T("Remove Context Menu");
	m_stringsMap[_T("MAINDLG_ADD_CONTEXT_MENU")] = _T("Add to Context Menu");
	m_stringsMap[_T("MAINDLG_CLEAR")] = _T("Clea&r");
	m_stringsMap[_T("MAINDLG_CLEAR_VERIFY")] = _T("Clea&r Verify");
	m_stringsMap[_T("MAINDLG_CALCU_TERMINAL")] = _T("Terminated");
	m_stringsMap[_T("MAINDLG_FIND_IN_RESULT")] = _T("Verify");
	m_stringsMap[_T("MAINDLG_RESULT")] = _T("Result:");
	m_stringsMap[_T("MAINDLG_NORESULT")] = _T("Nothing Found");
	m_stringsMap[_T("MAINDLG_FILE_PROGRESS")] = _T("File");
	m_stringsMap[_T("MAINDLG_TOTAL_PROGRESS")] = _T("Total");
	m_stringsMap[_T("MAINDLG_UPPER_HASH")] = _T("Uppercase");
	m_stringsMap[_T("MAINDLG_TIME_TITLE")] = _T("Time Used:");
	m_stringsMap[_T("MAINDLG_OPEN")] = _T("&Open...");
	m_stringsMap[_T("MAINDLG_STOP")] = _T("&Stop");
	m_stringsMap[_T("MAINDLG_COPY")] = _T("&Copy");
	m_stringsMap[_T("MAINDLG_VERIFY")] = _T("&Verify");
	m_stringsMap[_T("MAINDLG_ABOUT")] = _T("&About");
	m_stringsMap[_T("MAINDLG_EXIT")] = _T("E&xit");
	m_stringsMap[_T("MAINDLG_HYPEREDIT_MENU_COPY")] = _T("Copy hash value");
	m_stringsMap[_T("MAINDLG_HYPEREDIT_MENU_SERACHGOOGLE")] = _T("Search Google");
	m_stringsMap[_T("MAINDLG_HYPEREDIT_MENU_SERACHVIRUSTOTAL")] = _T("Search VirusTotal");

	// Find Dialog Strings
	m_stringsMap[_T("FINDDLG_TITLE")] = _T("Verify");

	// About Dialog Strings
	m_stringsMap[_T("ABOUTDLG_TITLE")] = _T("About fHash");
	m_stringsMap[_T("ABOUTDLG_INFO_TITLE")] = _T("fHash: Files' Hash Calculator ");
	m_stringsMap[_T("ABOUTDLG_INFO_RIGHT")] = _T("Copyright (C) 2007-2016 SUN Junwen.");
	m_stringsMap[_T("ABOUTDLG_INFO_MD5")] = _T("MD5 Algorithm is writtend by RSA Data Security, Inc. Copyright (C) RSA Data Security, Inc.");
	m_stringsMap[_T("ABOUTDLG_INFO_SHA256")] = _T("SHA256 Algorithm is writtend by Niels Moller. Copyright (C) Niels Moller");
	m_stringsMap[_T("ABOUTDLG_INFO_CRC32")] = _T("CRC32 Algorithm is writtend by Dominik Reichl. Copyright (C) Dominik Reichl");
	m_stringsMap[_T("ABOUTDLG_INFO_RIGHTDETAIL")] = _T("More details are on Project Site.");
	m_stringsMap[_T("ABOUTDLG_INFO_OSTITLE")] = _T("Operating System:");
	m_stringsMap[_T("ABOUTDLG_PROJECT_SITE")] = _T("<a>Hosted on GitHub</a>");
	m_stringsMap[_T("ABOUTDLG_PROJECT_URL")] = _T("https://github.com/sunjw/fhash");

}
