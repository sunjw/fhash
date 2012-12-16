#include "stdafx.h"

#include <cstdlib>

#include "HashEngine.h"
#include "Functions.h"
#include "UIStrings.h"

#include "Algorithms/md5.h"
#include "Algorithms/sha1.h"
#include "Algorithms/sha256.h"
#include "Algorithms/crc32.h"

//工作者线程
DWORD WINAPI md5_file(LPVOID pParam)
{
	ThreadData* thrdData = (ThreadData*)pParam;

	thrdData->threadWorking = TRUE;

	unsigned int i;
	CFileException ex;
	CFile File;
	thrdData->totalSize = 0;
	ULONGLONG finishedSize = 0;
	bool isSizeCaled = false;
	//ULONGLONG* fSizes = NULL;
	ULLongVector fSizes(thrdData->nFiles);
	int positionWhole = 0; // 全局进度条位置

	CString strFileSize;
	CString strFileMD5;
	CString strFileSHA1;
	CString strFileSHA256;
	CString strFileCRC32;

	//界面设置 - 开始
	::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_WORKING, 0);
	//界面设置 - 结束

	// 获得文件总大小
	if(thrdData->nFiles < 200) // 文件太多就不预先计算了
	{
		isSizeCaled = true;
		for(i = 0; i < (thrdData->nFiles); i++)
		{
			if(thrdData->stop)
			{
				thrdData->threadWorking = FALSE;
				::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_STOPPED, 0);
				return 0;
			}
			ULONGLONG fSize = 0;
		
			char* path;
			path = (char *)thrdData->fullPaths[i].operator LPCSTR();
			if(File.Open(path, CFile::modeRead|CFile::shareDenyWrite, &ex))
			{
				fSize = File.GetLength();//fsize=status.m_size; // Fix 4GB file
				File.Close();
			}

			fSizes[i] = fSize;
			thrdData->totalSize += fSize;
		}
	}

	// 计算循环
	for(i = 0; i < (thrdData->nFiles); i++)
	{
		if(thrdData->stop)
		{
			thrdData->threadWorking = FALSE;
			::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_STOPPED, 0);
			return 0;
		}

		// Declaration for calculator
		char* path;
		ULONGLONG fsize, times, t = 0;
		unsigned int len;
		unsigned char buffer[65534];
		
		MD5_CTX mdContext; // MD5 context

		CSHA1 sha1; // SHA1 object
		char strSHA1[256];

		SHA256_CTX sha256Ctx; // SHA256 context
		CString strSHA256 = "";

		unsigned long ulCRC32; // CRC32 context
		// Declaration for calculator

		int position = 0; // 进度条位置

		// 显示文件名
		thrdData->strAll.Append(FILENAME_STRING);
		thrdData->strAll.Append(" ");
		thrdData->strAll.Append(thrdData->fullPaths[i]);
		thrdData->strAll.Append("\r\n"); 

		::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);

		path = (char *)thrdData->fullPaths[i].operator LPCSTR();
		// 显示文件名

		//Calculating begins
		if(File.Open(path, CFile::modeRead|CFile::shareDenyWrite, &ex)) 
		{
			MD5Init(&mdContext, 0); // MD5开始
			sha1.Reset(); // SHA1开始
			sha256_init(&sha256Ctx); // SHA256开始
			crc32Init(&ulCRC32); // CRC32开始

			::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_PROG, 0);

			/*
			// get file size - start //
			fsize=thrdData->File.GetLength();
			tsize+=fsize;
			// get file size - end //
			*/

			// get file status //
			CString lastModifiedTime;
			CFileStatus status;
			if(File.GetStatus(status))
			{
				lastModifiedTime = status.m_mtime.Format("%Y-%m-%d %H:%M");
				fsize = File.GetLength();//fsize=status.m_size; // Fix 4GB file
				if(!isSizeCaled) // 如果没有计算过大小
				{
					thrdData->totalSize += fsize;
				}
				else
				{
					thrdData->totalSize = thrdData->totalSize + fsize - fSizes[i]; // 修正总大小
					fSizes[i] = fsize; // 修正文件大小
				}
			}

			CString shortSize = ConvertSizeToCStr(fsize);

			strFileSize.Format("%I64u", fsize);

			thrdData->strAll.Append(FILESIZE_STRING);
			thrdData->strAll.Append(" ");
			thrdData->strAll.Append(strFileSize);
			thrdData->strAll.Append(" ");
			thrdData->strAll.Append(BYTE_STRING);
			thrdData->strAll.Append(shortSize);
			thrdData->strAll.Append("\r\n");
			thrdData->strAll.Append(MODIFYTIME_STRING);
			thrdData->strAll.Append(" ");
			thrdData->strAll.Append(lastModifiedTime);

			// get file version //
			CString Ver = GetExeFileVersion(path);
			if(Ver.Compare("") != 0)
			{
				thrdData->strAll.Append("\r\n");
				thrdData->strAll.Append(VERSION_STRING);
				thrdData->strAll.Append(" ");
				thrdData->strAll.Append(Ver);
				/*
				thrdData->strAll = thrdData->strAll + "文件大小: " + thrdData->Size + " 字节" + shortSize + "\r\n"
				+ "修改日期: " + lastModifiedTime + "\r\n" + "版本: " + Ver + "\r\n";
				*/
			}

			thrdData->strAll.Append("\r\n");
	
			::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
			
			// get calculating times //
			times = fsize / sizeof(buffer) + 1;
			
			UINT bufferSize = sizeof( buffer );
			do 
			{	
				if(thrdData->stop)
				{
					if(File.m_hFile != CFile::hFileNull)
						File.Close();//很重要，否则会出错

					thrdData->threadWorking = FALSE;
					::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_STOPPED, 0);
					return 0;
				}
				len = File.Read( buffer, bufferSize );
				t++;

				MD5Update (&mdContext, (unsigned char *)buffer, len); // MD5更新
				sha1.Update(buffer, len); // SHA1更新
				sha256_update(&sha256Ctx, buffer, len); // SHA256更新
				crc32Update(&ulCRC32, buffer, len); // CRC32更新
				
				if((int)(100 * t / times) > position)
				{
					position = (int)(100 * t / times);
	
					::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_PROG, position);
				}
				finishedSize += len;
				if(isSizeCaled && thrdData->totalSize > 0 && 
					(int)(100 * finishedSize / thrdData->totalSize) > positionWhole)
				{
					positionWhole = (int)(100 * finishedSize / thrdData->totalSize);

					::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_PROG_WHOLE, positionWhole);
				}

			} while(len >= bufferSize);

			MD5Final (&mdContext); // MD5完成
			sha1.Final(); // SHA1完成
			sha256_final(&sha256Ctx); // SHA256完成
			crc32Finish(&ulCRC32); //CRC32完成

			if(!isSizeCaled)
			{
				if(thrdData->nFiles == 0)
					::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_PROG_WHOLE, 0);
				else
					::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_PROG_WHOLE, (i + 1) * 100 / (thrdData->nFiles));
			}

			File.Close();
			//Calculating ends

			strFileMD5.Format("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
								mdContext.digest[0],
								mdContext.digest[1],
								mdContext.digest[2],
								mdContext.digest[3],
								mdContext.digest[4],
								mdContext.digest[5],
								mdContext.digest[6],
								mdContext.digest[7],
								mdContext.digest[8],
								mdContext.digest[9],
								mdContext.digest[10],
								mdContext.digest[11],
								mdContext.digest[12],
								mdContext.digest[13],
								mdContext.digest[14],
								mdContext.digest[15]);
			sha1.ReportHash(strSHA1, CSHA1::REPORT_HEX);
			strFileSHA1.Format("%s", strSHA1);
			sha256_digest(&sha256Ctx, &strSHA256);
			strFileSHA256 = strSHA256;
			strFileCRC32.Format("%08X", ulCRC32);

			// 在这里保存结果是因为只保存完全计算完毕的
			ResultData result;
			result.strPath = thrdData->fullPaths[i];
			result.ulSize = fsize;
			result.strMDate = lastModifiedTime;
			result.strVersion = Ver;
			// 在没做转换前，结果都是大写的
			result.strMD5 = strFileMD5;
			result.strSHA1 = strFileSHA1;
			result.strSHA256 = strFileSHA256;
			result.strCRC32 = strFileCRC32;

			thrdData->resultList.push_back(result); // 保存结果

			if(thrdData->uppercase)
			{
				strFileMD5.MakeUpper();
				strFileSHA1.MakeUpper();
				strFileSHA256.MakeUpper();
				strFileCRC32.MakeUpper();
			}
			else
			{
				strFileMD5.MakeLower();
				strFileSHA1.MakeLower();
				strFileSHA256.MakeLower();
				strFileCRC32.MakeLower();
			}
			
			// 显示结果
			thrdData->strAll.Append("MD5: ");
			thrdData->strAll.Append(strFileMD5);
			thrdData->strAll.Append("\r\nSHA1: ");
			thrdData->strAll.Append(strFileSHA1);
			thrdData->strAll.Append("\r\nSHA256: ");
			thrdData->strAll.Append(strFileSHA256);
			thrdData->strAll.Append("\r\nCRC32: ");
			thrdData->strAll.Append(strFileCRC32);
			thrdData->strAll.Append("\r\n\r\n");

			::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
			// 显示结果
		}
		else
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			// 显示结果
			thrdData->strAll.Append(szError);
			thrdData->strAll.Append("\r\n\r\n");

			::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_REFRESH_TEXT, 0);
			// 显示结果
		}
	}

	::PostMessage(thrdData->hWnd, WM_THREAD_INFO, WP_FINISHED, 0);
	
	thrdData->threadWorking=FALSE;

	return 0;
}
