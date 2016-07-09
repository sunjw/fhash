#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdint.h>

#include <vector>
#include <list>

#if defined (WIN32)

#include <atlbase.h>
#include <WinUser.h>
#include <WinDef.h>
#include <WinNT.h>

#define WM_THREAD_INFO		WM_USER + 1 // 线程发出消息
#define WP_WORKING			WM_USER + 2 // 开始工作
#define WP_FINISHED			WM_USER + 3 // 线程完成
#define WP_STOPPED			WM_USER + 4 // 线程停止(未完成)
#define WP_REFRESH_TEXT		WM_USER + 5 // 刷新文本框
#define WP_PROG				WM_USER + 6 // 文件进度条
#define WP_PROG_WHOLE		WM_USER + 7 // 全局进度条

#define WM_CUSTOM_MSG		WM_USER + 16 // 自定义消息
#define WM_HYPEREDIT_MENU	WM_USER + 17 // HyperEdit 弹出菜单消息

#else

#define WINAPI

#endif

#include "strhelper.h"

class UIBridgeBase;

struct ResultData;

typedef std::vector<sunjwbase::tstring> TStrVector;
typedef std::vector<uint64_t> ULLongVector;
typedef std::list<ResultData> ResultList;

#define MAX_FILES_NUM 8192

enum ResultState
{
    RESULT_NONE = 0,
    RESULT_PATH,
    RESULT_META,
    RESULT_ALL,
    RESULT_ERROR
};

struct ResultData // 计算结果
{
	ResultState enumState; // State
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
	UIBridgeBase *uiBridge;

	bool threadWorking; // 线程是否在工作

	bool uppercase; // 是否大写

	uint32_t nFiles; // 文件个数
	uint64_t totalSize; // 所有文件大小
	bool stop; // 主界面要求停止计算

	TStrVector fullPaths; // 待计算的所有文件路径

	ResultList resultList;
};

#endif
