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
		string strVer("");
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
			strVer = strappendformat(strVer, ("%d.%d.%d.%d"), MVer, SVer, LVer, BVer);
			// Ver.Format(_T("%d.%d.%d.%d"), MVer, SVer, LVer, BVer);
			// 将版本号转换为数字 //

			delete[] pver;
		}

		// get file version //
		return strtotstr(strVer);
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
		string strOsinfo("");

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
							strOsinfo.append("Windows 11 ");
						else
							strOsinfo.append("Windows 10 ");
					}
					else
					{
						strOsinfo = strappendformat(strOsinfo, "Windows Server %d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
					}
				}
				else
				{
					strOsinfo = strappendformat(strOsinfo, "Windows Version %d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
				}
			}

			if (osvi.dwMajorVersion == 6)
			{
				if (osvi.dwMinorVersion == 0)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						strOsinfo.append("Windows Vista ");
					else
						strOsinfo.append("Windows Server 2008 ");
				}
				else if (osvi.dwMinorVersion == 1)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						strOsinfo.append("Windows 7 ");
					else
						strOsinfo.append("Windows Server 2008 R2 ");
				}
				else if (osvi.dwMinorVersion == 2)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						strOsinfo.append("Windows 8 ");
					else
						strOsinfo.append("Windows Server 2012 ");
				}
				else if (osvi.dwMinorVersion == 3)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						strOsinfo.append("Windows 8.1 ");
					else
						strOsinfo.append("Windows Server 2012 R2 ");
				}
				else if (osvi.dwMinorVersion == 4)
				{
					if (osvi.wProductType == VER_NT_WORKSTATION)
						strOsinfo.append("Windows 10 ");
					else
						strOsinfo = strappendformat(strOsinfo, "Windows Server %d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
				}
				else
				{
					strOsinfo = strappendformat(strOsinfo, "Windows Version %d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
				}
					
			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
			{
				if (osvi.wProductType == VER_NT_WORKSTATION &&
					si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					strOsinfo.append("Microsoft Windows XP Professional x64 Edition ");
				}
				else
				{
					strOsinfo.append("Microsoft Windows Server 2003, ");
				}
			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
			{	
				strOsinfo.append("Microsoft Windows XP ");
			}

			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				strOsinfo.append("Microsoft Windows 2000 ");

			if (osvi.dwMajorVersion <= 4)
				strOsinfo.append("Microsoft Windows NT ");

			///osvi.dwMajorVersion = 6;

			if (osvi.dwMajorVersion < 6)
			{
				if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
					strOsinfo.append("Home Edition ");
				else
					strOsinfo.append("Professional ");
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
						strOsinfo.append("Ultimate ");
						break;
					case PRODUCT_PROFESSIONAL:
						strOsinfo.append("Professional ");
						break;
					case PRODUCT_CORE:
						strOsinfo.append("Home ");
						break;
					case PRODUCT_HOME_PREMIUM:
						strOsinfo.append("Home Premium ");
						break;
					case PRODUCT_HOME_BASIC:
						strOsinfo.append("Home Basic ");
						break;
					case PRODUCT_ENTERPRISE:
						strOsinfo.append("Enterprise ");
						break;
					case PRODUCT_BUSINESS:
						strOsinfo.append("Business ");
						break;
					case PRODUCT_STARTER:
						strOsinfo.append("Starter ");
						break;
					case PRODUCT_CLUSTER_SERVER:
						strOsinfo.append("HPC Edition ");
						break;
					case PRODUCT_DATACENTER_SERVER:
						strOsinfo.append("Datacenter ");
						break;
					case PRODUCT_DATACENTER_SERVER_CORE:
						strOsinfo.append("Datacenter (core installation) ");
						break;
					case PRODUCT_ENTERPRISE_SERVER:
						strOsinfo.append("Enterprise ");
						break;
					case PRODUCT_ENTERPRISE_SERVER_CORE:
						strOsinfo.append("Enterprise (core installation) ");
						break;
					case PRODUCT_ENTERPRISE_SERVER_IA64:
						strOsinfo.append("Enterprise for Itanium-based Systems ");
						break;
					case PRODUCT_SMALLBUSINESS_SERVER:
						strOsinfo.append("Small Business Server ");
						break;
					case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
						strOsinfo.append("Small Business Server Premium ");
						break;
					case PRODUCT_STANDARD_SERVER:
						strOsinfo.append("Standard ");
						break;
					case PRODUCT_STANDARD_SERVER_CORE:
						strOsinfo.append("Standard (core installation) ");
						break;
					case PRODUCT_WEB_SERVER:
						strOsinfo.append("Web Server ");
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
						strOsinfo.append("Workstation 4.0 ");
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
								strOsinfo.append("Datacenter Edition for Itanium-based Systems");
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								strOsinfo.append("Enterprise Edition for Itanium-based Systems");
						}

						else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
						{
							if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
								strOsinfo.append("Datacenter x64 Edition ");
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								strOsinfo.append("Enterprise x64 Edition ");
							else
								strOsinfo.append("Standard x64 Edition ");
						}

						else
						{
							if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
								strOsinfo.append("Datacenter Edition ");
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								strOsinfo.append("Enterprise Edition ");
							else if (osvi.wSuiteMask & VER_SUITE_BLADE)
								strOsinfo.append("Web Edition ");
							else
								strOsinfo.append("Standard Edition ");
						}
					}
					else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER )
							strOsinfo.append("Datacenter Server ");
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							strOsinfo.append("Advanced Server ");
						else
							strOsinfo.append("Server ");
					}
					else  // Windows NT 4.0 
					{
						if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							strOsinfo.append("Server 4.0, Enterprise Edition ");
						else
							strOsinfo.append("Server 4.0 ");
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
					strOsinfo.append("Workstation ");
				if (lstrcmpi(TEXT("LANMANNT"), szProductType) == 0)
					strOsinfo.append("Server ");
				if (lstrcmpi(TEXT("SERVERNT"), szProductType) == 0)
					strOsinfo.append("Advanced Server ");
				string verinfo;
				verinfo = strappendformat(verinfo, "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
				strOsinfo.append(verinfo);
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
					string spinfo;
					spinfo = strappendformat(spinfo, "Service Pack 6a (Build %d)\r\n",
									osvi.dwBuildNumber & 0xFFFF); 
					strOsinfo.append(spinfo);
				}
				else // Windows NT 4.0 prior to SP6a
				{
					string spinfo;
					tstring tstrCSDVersion(osvi.szCSDVersion);
					if (tstrCSDVersion.length() > 0)
					{
						string strCSDVersion = tstrtostr(tstrCSDVersion);
						spinfo = strappendformat(spinfo,
							"%s (Build %d)\r\n",
							strCSDVersion.c_str(),
							osvi.dwBuildNumber & 0xFFFF);
					}
					else
					{
						spinfo = strappendformat(spinfo,
							"(Build %d)\r\n", osvi.dwBuildNumber & 0xFFFF);
					}
					strOsinfo.append(spinfo);
				}

				RegCloseKey( hKey );
			}
			else // not Windows NT 4.0 
			{
				string spinfo;
				tstring tstrCSDVersion(osvi.szCSDVersion);
				if (tstrCSDVersion.length() > 0)
				{
					string strCSDVersion = tstrtostr(tstrCSDVersion);
					spinfo = strappendformat(spinfo,
						"%s (Build %d)\r\n",
						strCSDVersion.c_str(),
						osvi.dwBuildNumber & 0xFFFF);
				}
				else
				{
					spinfo = strappendformat(spinfo,
						"(Build %d)\r\n", osvi.dwBuildNumber & 0xFFFF);
				}
				strOsinfo.append(spinfo);
			}

			break;

			// Test for the Windows Me/98/95.
		case VER_PLATFORM_WIN32_WINDOWS:

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
			{
				strOsinfo.append("Microsoft Windows 95 ");
				if (osvi.szCSDVersion[1] == TEXT('C') || osvi.szCSDVersion[1] == TEXT('B'))
					strOsinfo.append("OSR2 ");
			} 

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
			{
				strOsinfo.append("Microsoft Windows 98 ");
				if (osvi.szCSDVersion[1] == TEXT('A') || osvi.szCSDVersion[1] == TEXT('B'))
					strOsinfo.append("SE ");
			} 

			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
			{
				strOsinfo.append("Microsoft Windows Millennium Edition\r\n");
			} 
			break;

		case VER_PLATFORM_WIN32s:

			strOsinfo.append("Microsoft Win32s\r\n");
			break;
		}

		return strtotstr(strOsinfo);
	}

}

