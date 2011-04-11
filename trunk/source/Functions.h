#pragma once
#include "stdafx.h"

// 以下是获得 Windows 版本信息的定义内容
#define BUFSIZE 80

// ProductType常量
#define PRODUCT_UNDEFINED                            0x00000000    // An unknown product
#define PRODUCT_ULTIMATE                            0x00000001    // Ultimate Edition
#define PRODUCT_HOME_BASIC                            0x00000002    // Home Basic Edition
#define PRODUCT_HOME_PREMIUM                        0x00000003    // Home Premium Edition
#define PRODUCT_ENTERPRISE                            0x00000004    // Enterprise Edition
#define PRODUCT_HOME_BASIC_N                        0x00000005    // Home Basic Edition
#define PRODUCT_BUSINESS                            0x00000006    // Business Edition
#define PRODUCT_STANDARD_SERVER                        0x00000007    // Server Standard Edition (full installation)
#define PRODUCT_DATACENTER_SERVER                    0x00000008    // Server Datacenter Edition (full installation)
#define PRODUCT_SMALLBUSINESS_SERVER                0x00000009    // Small Business Server
#define PRODUCT_ENTERPRISE_SERVER                    0x0000000A    // Server Enterprise Edition (full installation)
#define PRODUCT_STARTER                                0x0000000B    // Starter Edition
#define PRODUCT_DATACENTER_SERVER_CORE                0x0000000C    // Server Datacenter Edition (core installation)
#define PRODUCT_STANDARD_SERVER_CORE                0x0000000D    // Server Standard Edition (core installation)
#define PRODUCT_ENTERPRISE_SERVER_CORE                0x0000000E    // Server Enterprise Edition (core installation)
#define PRODUCT_ENTERPRISE_SERVER_IA64                0x0000000F    // Server Enterprise Edition for Itanium-based Systems
#define PRODUCT_BUSINESS_N                            0x00000010    // Business Edition
#define PRODUCT_WEB_SERVER                            0x00000011    // Web Server Edition (full installation)
#define PRODUCT_CLUSTER_SERVER                        0x00000012    // Cluster Server Edition
#define PRODUCT_HOME_SERVER                            0x00000013    // Home Server Edition
#define PRODUCT_STORAGE_EXPRESS_SERVER                0x00000014    // Storage Server Express Edition
#define PRODUCT_STORAGE_STANDARD_SERVER                0x00000015    // Storage Server Standard Edition
#define PRODUCT_STORAGE_WORKGROUP_SERVER            0x00000016    // Storage Server Workgroup Edition
#define PRODUCT_STORAGE_ENTERPRISE_SERVER            0x00000017    // Storage Server Enterprise Edition
#define PRODUCT_SERVER_FOR_SMALLBUSINESS            0x00000018    // Server for Small Business Edition
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM        0x00000019    // Small Business Server Premium Edition
#define PRODUCT_HOME_PREMIUM_N                        0x0000001A    // Home Premium Edition
#define PRODUCT_ENTERPRISE_N                        0x0000001B    // Enterprise Edition
#define PRODUCT_ULTIMATE_N                            0x0000001C    // Ultimate Edition
#define PRODUCT_WEB_SERVER_CORE                        0x0000001D    // Web Server Edition (core installation)
#define PRODUCT_UNLICENSED                            0xABCDABCD    // Unlicensed edition

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
// GetProductInfo函数原型(Vista/2008)
typedef BOOL (WINAPI *PGPI)(DWORD dwOSMajorVersion, DWORD dwOSMinorVersion, DWORD dwSpMajorVersion, DWORD dwSpMinorVersion, PDWORD pdwReturnedProductType);
// 以上是获得 Windows 版本信息的定义内容

CString ConvertSizeToCStr(ULONGLONG size);

CString GetExeFileVersion(char* path);

BOOL GetWindowsVersion(OSVERSIONINFOEX& osvi, BOOL& bOsVersionInfoEx);

CString GetWindowsInfo();

bool AddContextMenu(void);
bool RemoveContextMenu(void);
bool IsContextMenuAdded(void);
