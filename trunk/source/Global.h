#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdint.h>

#include <vector>
#include <list>

#include <atlbase.h>
#include <WinUser.h>
#include <WinDef.h>
#include <WinNT.h>

#include "strhelper.h"

struct ResultData;

typedef std::vector<sunjwbase::tstring> TStrVector;
typedef std::vector<uint64_t> ULLongVector;
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
	bool bDone; // Done
	sunjwbase::tstring tstrPath; // 路径
	uint64_t ulSize; // 大小
	sunjwbase::tstring tstrMDate; // 修改日期
	sunjwbase::tstring tstrVersion; // 版本
	sunjwbase::tstring tstrMD5; // MD5
	sunjwbase::tstring tstrSHA1; // SHA1
	sunjwbase::tstring tstrSHA256; // SHA256
	sunjwbase::tstring tstrCRC32; // CRC32
	sunjwbase::tstring tstrError; // Error string
};

struct ThreadData // 传向计算线程的信息
{
	bool threadWorking; // 线程是否在工作

	HWND hWnd; // 界面句柄
	bool uppercase; // 是否大写

	uint32_t nFiles; // 文件个数
	uint64_t totalSize; // 所有文件大小
	bool stop; // 主界面要求停止计算

	TStrVector fullPaths; // 待计算的所有文件路径

	sunjwbase::tstring tstrAll; // 全部计算结果，它应该和下面的resultList 同步
	ResultList resultList;
};

#endif
