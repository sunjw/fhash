#include "stdafx.h"

#include "UIStringsZHCN.h"

#include <tchar.h>

UIStringsZHCN::UIStringsZHCN()
{
	// Global Strings
	m_stringsMap[_T("FILE_STRING")] = _T("文件");
	m_stringsMap[_T("BYTE_STRING")] = _T("字节");
	m_stringsMap[_T("HASHVALUE_STRING")] = _T("Hash 值:");
	m_stringsMap[_T("FILENAME_STRING")] = _T("文件名:");
	m_stringsMap[_T("FILESIZE_STRING")] = _T("文件大小:");
	m_stringsMap[_T("MODIFYTIME_STRING")] = _T("修改日期:");
	m_stringsMap[_T("VERSION_STRING")] = _T("版本:");
	m_stringsMap[_T("SECOND_STRING")] = _T("秒");
	m_stringsMap[_T("BUTTON_OK")] = _T("确定");
	m_stringsMap[_T("BUTTON_CANCEL")] = _T("取消");

	// Main Dialog Strings
	m_stringsMap[_T("MAINDLG_INITINFO")] = _T("将文件拖入或点击打开，开始计算。");
	m_stringsMap[_T("MAINDLG_WAITING_START")] = _T("准备开始计算。");
	m_stringsMap[_T("MAINDLG_CONTEXT_INIT")] = _T("需要管理员权限");
	m_stringsMap[_T("MAINDLG_ADD_SUCCEEDED")] = _T("添加成功");
	m_stringsMap[_T("MAINDLG_ADD_FAILED")] = _T("添加失败");
	m_stringsMap[_T("MAINDLG_REMOVE_SUCCEEDED")] = _T("移除成功");
	m_stringsMap[_T("MAINDLG_REMOVE_FAILED")] = _T("移除失败");
	m_stringsMap[_T("MAINDLG_REMOVE_CONTEXT_MENU")] = _T("移除右键菜单");
	m_stringsMap[_T("MAINDLG_ADD_CONTEXT_MENU")] = _T("添加右键菜单");
	m_stringsMap[_T("MAINDLG_CLEAR")] = _T("清除(&R)");
	m_stringsMap[_T("MAINDLG_CLEAR_VERIFY")] = _T("清除验证(&R)");
	m_stringsMap[_T("MAINDLG_CALCU_TERMINAL")] = _T("计算终止");
	m_stringsMap[_T("MAINDLG_FIND_IN_RESULT")] = _T("在结果中搜索");
	m_stringsMap[_T("MAINDLG_RESULT")] = _T("匹配的结果:");
	m_stringsMap[_T("MAINDLG_NORESULT")] = _T("无匹配结果");
	m_stringsMap[_T("MAINDLG_FILE_PROGRESS")] = _T("文件进度");
	m_stringsMap[_T("MAINDLG_TOTAL_PROGRESS")] = _T("总体进度");
	m_stringsMap[_T("MAINDLG_UPPER_HASH")] = _T("大写 Hash");
	m_stringsMap[_T("MAINDLG_TIME_TITLE")] = _T("计算时间:");
	m_stringsMap[_T("MAINDLG_OPEN")] = _T("打开(&O)...");
	m_stringsMap[_T("MAINDLG_STOP")] = _T("停止(&S)");
	m_stringsMap[_T("MAINDLG_COPY")] = _T("全部复制(&C)");
	m_stringsMap[_T("MAINDLG_VERIFY")] = _T("验证(&V)");
	m_stringsMap[_T("MAINDLG_ABOUT")] = _T("关于(&A)");
	m_stringsMap[_T("MAINDLG_EXIT")] = _T("退出(&X)");
	m_stringsMap[_T("MAINDLG_HYPEREDIT_MENU_COPY")] = _T("复制哈希值");
	m_stringsMap[_T("MAINDLG_HYPEREDIT_MENU_SERACHGOOGLE")] = _T("搜索 Google");
	m_stringsMap[_T("MAINDLG_HYPEREDIT_MENU_SERACHVIRUSTOTAL")] = _T("搜索 VirusTotal");

	// Find Dialog Strings
	m_stringsMap[_T("FINDDLG_TITLE")] = _T("验证");

	// About Dialog Strings
	m_stringsMap[_T("ABOUTDLG_TITLE")] = _T("关于 fHash");
	m_stringsMap[_T("ABOUTDLG_INFO_TITLE")] = _T("fHash: 文件 Hash 计算器 ");
	m_stringsMap[_T("ABOUTDLG_INFO_RIGHT")] = _T("Copyright (C) 2007-2016 SUN Junwen.");
	m_stringsMap[_T("ABOUTDLG_INFO_MD5")] = _T("MD5 算法是由 RSA Data Security, Inc.编写。Copyright (C) RSA Data Security, Inc.");
	m_stringsMap[_T("ABOUTDLG_INFO_SHA256")] = _T("SHA256 算法是由 Niels Moller 编写。Copyright (C) Niels Moller");
	m_stringsMap[_T("ABOUTDLG_INFO_CRC32")] = _T("CRC32 算法是由 Dominik Reichl 编写。Copyright (C) Dominik Reichl");
	m_stringsMap[_T("ABOUTDLG_INFO_RIGHTDETAIL")] = _T("详细授权信息见开发者网站。");
	m_stringsMap[_T("ABOUTDLG_INFO_OSTITLE")] = _T("当前操作系统:");
	m_stringsMap[_T("ABOUTDLG_PROJECT_SITE")] = _T("<a>Hosted on GitHub</a>");
	m_stringsMap[_T("ABOUTDLG_PROJECT_URL")] = _T("https://github.com/sunjw/fhash");

}
