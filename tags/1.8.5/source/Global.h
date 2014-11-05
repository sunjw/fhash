#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <vector>
#include <list>

#include <atlbase.h>
#include <WinUser.h>
#include <WinDef.h>
#include <WinNT.h>

struct ResultData;

typedef std::vector<CString> CStrVector;
typedef std::vector<ULONGLONG> ULLongVector;
typedef std::list<ResultData> ResultList;

#define WM_THREAD_INFO		WM_USER + 1 // 线程发出消息
#define WP_WORKING			WM_USER + 2 // 开始工作
#define WP_FINISHED			WM_USER + 3 // 线程完成
#define WP_STOPPED			WM_USER + 4 // 线程停止(未完成)
#define WP_REFRESH_TEXT		WM_USER + 5 // 刷新文本框
#define WP_PROG				WM_USER + 6 // 文件进度条
#define WP_PROG_WHOLE		WM_USER + 7 // 全局进度条

#define MAX_FILES_NUM 8192

struct ResultData // 计算结果
{
	BOOL bDone; // Done
	CString strPath; // 路径
	ULONGLONG ulSize; // 大小
	CString strMDate; // 修改日期
	CString strVersion; // 版本
	CString strMD5; // MD5
	CString strSHA1; // SHA1
	CString strSHA256; // SHA256
	CString strCRC32; // CRC32
	CString strError; // Error string
};

struct ThreadData // 传向计算线程的信息
{
	BOOL threadWorking; // 线程是否在工作

	HWND hWnd; // 界面句柄
	BOOL uppercase; // 是否大写

	unsigned int nFiles; // 文件个数
	ULONGLONG totalSize; // 所有文件大小
	BOOL stop; // 主界面要求停止计算

	CStrVector fullPaths; // 待计算的所有文件路径

	CString strAll; // 全部计算结果，它应该和下面的resultList 同步
	ResultList resultList;
};

#endif
