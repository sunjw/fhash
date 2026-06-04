#include "stdafx.h"

#include "WindowsComm.h"

#include <stdio.h>
#include <string>

#include "Common/strhelper.h"

#pragma warning(disable: 4996)

using namespace std;
using namespace sunjwbase;

#define BUFSIZE 80

// 以下是获得 Windows 版本信息的定义内容
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
// GetProductInfo 函数原型(Vista/2008)
typedef BOOL(WINAPI *PGPI)(DWORD, DWORD, DWORD,
	DWORD, PDWORD);
// 10 additional
#define PRODUCT_CORE 0x00000065
// 以上是获得 Windows 版本信息的定义内容

namespace WindowsComm
{
	/*
	 * GetExeFileVersion
	 * 获得指定路径文件版本
	 * 格式化为 主版本号.副版本号.低版本号.编译版本号
	 * 如果文件没有版本号，返回""
	 */
	tstring GetExeFileVersion(TCHAR* path)
	{
		// get file version //
		tstring tstrVer(TEXT(""));
		unsigned int MVer,SVer,LVer,BVer;
		VS_FIXEDFILEINFO pvsf;
		DWORD dwHandle;
		DWORD cchver = GetFileVersionInfoSize(path, &dwHandle);
		BYTE *pver = new BYTE[cchver];
		BOOL bret = GetFileVersionInfo(path, dwHandle, cchver, pver);
		if (bret)
		{
			UINT uLen;
			void *pbuf;
			bret = VerQueryValue(pver, TEXT("\\"), &pbuf, &uLen);
			memcpy(&pvsf, pbuf, sizeof(VS_FIXEDFILEINFO));

			// 将版本号转换为数字 //
			MVer = pvsf.dwFileVersionMS / 65536;
			SVer = pvsf.dwFileVersionMS - 65536 * MVer;
			LVer = pvsf.dwFileVersionLS / 65536;
			BVer = pvsf.dwFileVersionLS - 65536 * LVer;
			tstrVer = strappendformat(tstrVer, TEXT("%d.%d.%d.%d"), MVer, SVer, LVer, BVer);
			// Ver.Format(_T("%d.%d.%d.%d"), MVer, SVer, LVer, BVer);
			// 将版本号转换为数字 //

			delete[] pver;
		}

		// get file version //
		return tstrVer;
	}

	BOOL GetWindowsVersion(OSVERSIONINFOEX& osvi, BOOL& bOsVersionInfoEx)
	{
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
		// If that fails, try using the OSVERSIONINFO structure.

		if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi)))
		{
			osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
			if (!GetVersionEx((OSVERSIONINFO *)&osvi))
				return FALSE;
		}

		return TRUE;
	}

	/*
	 * GetWindowsInfo()
	 * 获得 Windows 版本信息
	 */
	tstring GetWindowsInfo()
	{
		tstring tstrOsinfo(TEXT(""));

		OSVERSIONINFOEX osvi;
		SYSTEM_INFO si;
		PGNSI pGNSI;
		BOOL bOsVersionInfoEx;

		ZeroMemory(&si, sizeof(SYSTEM_INFO));

		GetWindowsVersion(osvi, bOsVersionInfoEx);

		// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

		pGNSI = (PGNSI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
		if (NULL != pGNSI)
			pGNSI(&si);
		else
			GetSystemInfo(&si);

		switch (osvi.dwPlatformId)
		{
		// Test for the Windows NT product family.
		case VER_PLATFORM_WIN32_NT:

			// Test for the specific product.
			if (osvi.dwMajorVersion > 6)
			{
				if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
					{
						if (osvi.dwBuildNumber >= 22000) // Test for the Windows 11
							tstrOsinfo.append(TEXT("Windows 11 "));
						else
							tstrOsinfo.append(TEXT("Windows 10 "));
					}
					else
					{
						tstrOsinfo = strappendformat(tstrOsinfo, TEXT("Windows Server %d.%d "), osvi.dwMajorVersion, osvi.dwMinorVersion);
					}
				}
				else
				{
					tstrOsinfo = strappendformat(tstrOsinfo, TEXT("Windows Version %d.%d "), osvi.dwMajorVersion, osvi.dwMinorVersion);
				}
			}

			if (osvi.dwMajorVersion == 6)
			{
				if (osvi.dwMinorVersion == 0)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						tstrOsinfo.append(TEXT("Windows Vista "));
					else
						tstrOsinfo.append(TEXT("Windows Server 2008 "));
				}
				else if (osvi.dwMinorVersion == 1)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						tstrOsinfo.append(TEXT("Windows 7 "));
					else
						tstrOsinfo.append(TEXT("Windows Server 2008 R2 "));
				}
				else if (osvi.dwMinorVersion == 2)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						tstrOsinfo.append(TEXT("Windows 8 "));
					else
						tstrOsinfo.append(TEXT("Windows Server 2012 "));
				}
				else if (osvi.dwMinorVersion == 3)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						tstrOsinfo.append(TEXT("Windows 8.1 "));
					else
						tstrOsinfo.append(TEXT("Windows Server 2012 R2 "));
				}
				else if (osvi.dwMinorVersion == 4)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						tstrOsinfo.append(TEXT("Windows 10 "));
					else
						tstrOsinfo = strappendformat(tstrOsinfo, TEXT("Windows Server %d.%d "), osvi.dwMajorVersion, osvi.dwMinorVersion);
				}
				else
				{
					tstrOsinfo = strappendformat(tstrOsinfo, TEXT("Windows Version %d.%d "), osvi.dwMajorVersion, osvi.dwMinorVersion);
				}

			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
			{
				if (osvi.wProductType == VER_NT_WORKSTATION &&
					si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					tstrOsinfo.append(TEXT("Microsoft Windows XP Professional x64 Edition "));
				}
				else
				{
					tstrOsinfo.append(TEXT("Microsoft Windows Server 2003, "));
				}
			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
			{
				tstrOsinfo.append(TEXT("Microsoft Windows XP "));
			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				tstrOsinfo.append(TEXT("Microsoft Windows 2000 "));

			if (osvi.dwMajorVersion <= 4)
				tstrOsinfo.append(TEXT("Microsoft Windows NT "));

			///osvi.dwMajorVersion = 6;

			if (osvi.dwMajorVersion < 6)
			{
				if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
					tstrOsinfo.append(TEXT("Home Edition "));
				else
					tstrOsinfo.append(TEXT("Professional "));
			}
			else
			{
				// >= 6, Vista, 7, 8, 10...
				// 获得 GetProductInfo 函数原型
				PGPI pGetProductInfo  = NULL;
				pGetProductInfo = (PGPI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
				if (pGetProductInfo != NULL)
				{
					DWORD dwProductType = 0;
					(*pGetProductInfo)(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwProductType);
					switch (dwProductType)
					{
					case PRODUCT_ULTIMATE:
						tstrOsinfo.append(TEXT("Ultimate "));
						break;
					case PRODUCT_PROFESSIONAL:
						tstrOsinfo.append(TEXT("Professional "));
						break;
					case PRODUCT_CORE:
						tstrOsinfo.append(TEXT("Home "));
						break;
					case PRODUCT_HOME_PREMIUM:
						tstrOsinfo.append(TEXT("Home Premium "));
						break;
					case PRODUCT_HOME_BASIC:
						tstrOsinfo.append(TEXT("Home Basic "));
						break;
					case PRODUCT_ENTERPRISE:
						tstrOsinfo.append(TEXT("Enterprise "));
						break;
					case PRODUCT_BUSINESS:
						tstrOsinfo.append(TEXT("Business "));
						break;
					case PRODUCT_STARTER:
						tstrOsinfo.append(TEXT("Starter "));
						break;
					case PRODUCT_CLUSTER_SERVER:
						tstrOsinfo.append(TEXT("HPC Edition "));
						break;
					case PRODUCT_DATACENTER_SERVER:
						tstrOsinfo.append(TEXT("Datacenter "));
						break;
					case PRODUCT_DATACENTER_SERVER_CORE:
						tstrOsinfo.append(TEXT("Datacenter (core installation) "));
						break;
					case PRODUCT_ENTERPRISE_SERVER:
						tstrOsinfo.append(TEXT("Enterprise "));
						break;
					case PRODUCT_ENTERPRISE_SERVER_CORE:
						tstrOsinfo.append(TEXT("Enterprise (core installation) "));
						break;
					case PRODUCT_ENTERPRISE_SERVER_IA64:
						tstrOsinfo.append(TEXT("Enterprise for Itanium-based Systems "));
						break;
					case PRODUCT_SMALLBUSINESS_SERVER:
						tstrOsinfo.append(TEXT("Small Business Server "));
						break;
					case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
						tstrOsinfo.append(TEXT("Small Business Server Premium "));
						break;
					case PRODUCT_STANDARD_SERVER:
						tstrOsinfo.append(TEXT("Standard "));
						break;
					case PRODUCT_STANDARD_SERVER_CORE:
						tstrOsinfo.append(TEXT("Standard (core installation) "));
						break;
					case PRODUCT_WEB_SERVER:
						tstrOsinfo.append(TEXT("Web Server "));
						break;
					}
				}
			}

			// Test for specific product on Windows NT 4.0 SP6 and later.
			if (bOsVersionInfoEx)
			{
				// Test for the workstation type.
				if (osvi.wProductType == VER_NT_WORKSTATION &&
					si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64)
				{
					if (osvi.dwMajorVersion == 4)
						tstrOsinfo.append(TEXT("Workstation 4.0 "));
				}

				// Test for the server type.
				else if (osvi.wProductType == VER_NT_SERVER ||
					osvi.wProductType == VER_NT_DOMAIN_CONTROLLER)
				{
					if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
					{
						if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
						{
							if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
								tstrOsinfo.append(TEXT("Datacenter Edition for Itanium-based Systems"));
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								tstrOsinfo.append(TEXT("Enterprise Edition for Itanium-based Systems"));
						}

						else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
						{
							if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
								tstrOsinfo.append(TEXT("Datacenter x64 Edition "));
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								tstrOsinfo.append(TEXT("Enterprise x64 Edition "));
							else
								tstrOsinfo.append(TEXT("Standard x64 Edition "));
						}

						else
						{
							if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
								tstrOsinfo.append(TEXT("Datacenter Edition "));
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								tstrOsinfo.append(TEXT("Enterprise Edition "));
							else if (osvi.wSuiteMask & VER_SUITE_BLADE)
								tstrOsinfo.append(TEXT("Web Edition "));
							else
								tstrOsinfo.append(TEXT("Standard Edition "));
						}
					}
					else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER )
							tstrOsinfo.append(TEXT("Datacenter Server "));
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							tstrOsinfo.append(TEXT("Advanced Server "));
						else
							tstrOsinfo.append(TEXT("Server "));
					}
					else  // Windows NT 4.0
					{
						if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							tstrOsinfo.append(TEXT("Server 4.0, Enterprise Edition "));
						else
							tstrOsinfo.append(TEXT("Server 4.0 "));
					}
				}
			}
			// Test for specific product on Windows NT 4.0 SP5 and earlier
			else
			{
				HKEY hKey;
				TCHAR szProductType[BUFSIZE];
				DWORD dwBufLen = BUFSIZE*sizeof(TCHAR);
				LONG lRet;

				lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
					0, KEY_QUERY_VALUE, &hKey );
				if( lRet != ERROR_SUCCESS )
					return FALSE;

				lRet = RegQueryValueEx(hKey, TEXT("ProductType"), NULL, NULL,
					(LPBYTE) szProductType, &dwBufLen);
				RegCloseKey( hKey );

				if ((lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE*sizeof(TCHAR)))
					return FALSE;

				if (lstrcmpi(TEXT("WINNT"), szProductType) == 0)
					tstrOsinfo.append(TEXT("Workstation "));
				if (lstrcmpi(TEXT("LANMANNT"), szProductType) == 0)
					tstrOsinfo.append(TEXT("Server "));
				if (lstrcmpi(TEXT("SERVERNT"), szProductType) == 0)
					tstrOsinfo.append(TEXT("Advanced Server "));
				tstring verinfo;
				verinfo = strappendformat(verinfo, TEXT("%d.%d "), osvi.dwMajorVersion, osvi.dwMinorVersion);
				tstrOsinfo.append(verinfo);
			}

			// Display service pack (if any) and build number.
			if (osvi.dwMajorVersion == 4 &&
				lstrcmpi(osvi.szCSDVersion, TEXT("Service Pack 6")) == 0 )
			{
				HKEY hKey;
				LONG lRet;

				// Test for SP6 versus SP6a.
				lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
					0, KEY_QUERY_VALUE, &hKey);
				if (lRet == ERROR_SUCCESS)
				{
					tstring spinfo;
					spinfo = strappendformat(spinfo, TEXT("Service Pack 6a (Build %d)\r\n"),
									osvi.dwBuildNumber & 0xFFFF);
					tstrOsinfo.append(spinfo);
				}
				else // Windows NT 4.0 prior to SP6a
				{
					tstring spinfo;
					tstring tstrCSDVersion(osvi.szCSDVersion);
					if (tstrCSDVersion.length() > 0)
					{
						spinfo = strappendformat(spinfo,
							TEXT("%s (Build %d)\r\n"),
							tstrCSDVersion.c_str(),
							osvi.dwBuildNumber & 0xFFFF);
					}
					else
					{
						spinfo = strappendformat(spinfo,
							TEXT("(Build %d)\r\n"), osvi.dwBuildNumber & 0xFFFF);
					}
					tstrOsinfo.append(spinfo);
				}

				RegCloseKey( hKey );
			}
			else // not Windows NT 4.0
			{
				tstring spinfo;
				tstring tstrCSDVersion(osvi.szCSDVersion);
				if (tstrCSDVersion.length() > 0)
				{
					spinfo = strappendformat(spinfo,
						TEXT("%s (Build %d)\r\n"),
						tstrCSDVersion.c_str(),
						osvi.dwBuildNumber & 0xFFFF);
				}
				else
				{
					spinfo = strappendformat(spinfo,
						TEXT("(Build %d)\r\n"), osvi.dwBuildNumber & 0xFFFF);
				}
				tstrOsinfo.append(spinfo);
			}

			break;

			// Test for the Windows Me/98/95.
		case VER_PLATFORM_WIN32_WINDOWS:

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
			{
				tstrOsinfo.append(TEXT("Microsoft Windows 95 "));
				if (osvi.szCSDVersion[1] == TEXT('C') || osvi.szCSDVersion[1] == TEXT('B'))
					tstrOsinfo.append(TEXT("OSR2 "));
			}

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
			{
				tstrOsinfo.append(TEXT("Microsoft Windows 98 "));
				if (osvi.szCSDVersion[1] == TEXT('A') || osvi.szCSDVersion[1] == TEXT('B'))
					tstrOsinfo.append(TEXT("SE "));
			}

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
			{
				tstrOsinfo.append(TEXT("Microsoft Windows Millennium Edition\r\n"));
			}
			break;

		case VER_PLATFORM_WIN32s:

			tstrOsinfo.append(TEXT("Microsoft Win32s\r\n"));
			break;
		}

		return tstrOsinfo;
	}

}

