#include "stdafx.h"

#include "HashEngine.h"

#include <stdlib.h>

#if defined (__APPLE__) || defined (__unix)
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "strhelper.h"
#include "UIBridgeBase.h"

#if defined (WIN32)
#include "WindowsUtils.h"
#endif

#include "OsUtils/OsFile.h"
#include "OsUtils/OsThread.h"

#include "Algorithms/md5.h"
#include "Algorithms/SHA1.h"
#include "Algorithms/sha256.h"
#include "Algorithms/CRC32.h"

using namespace std;
using namespace sunjwbase;

class DataBuffer
{
public:
	DataBuffer():datalen(0), data(NULL)
	{ data = new unsigned char[DataBuffer::preflen]; }

	~DataBuffer()
	{ delete[] data; datalen = 0; }

	static unsigned int preflen;

	unsigned int datalen;
	unsigned char *data;
};

unsigned int DataBuffer::preflen = 1048576; // 2^20

//工作者线程
int WINAPI HashThreadFunc(void *param)
{
	ThreadData *thrdData = (ThreadData *)param;
	UIBridgeBase *uiBridge = thrdData->uiBridge;

	thrdData->threadWorking = true;

	uint32_t i;
	thrdData->totalSize = 0;
	uint64_t finishedSize = 0;
	uint64_t finishedSizeWhole = 0;
	bool isSizeCaled = false;
	//ULONGLONG* fSizes = NULL;
	ULLongVector fSizes(thrdData->nFiles);
	int positionWhole = 0; // 全局进度条位置

	tstring tstrFileSize;
	tstring tstrFileVersion;
	tstring tstrFileMD5;
	tstring tstrFileSHA1;
	tstring tstrFileSHA256;
	tstring tstrFileCRC32;

	uiBridge->preparingCalc();

	// 获得文件总大小
	if(thrdData->nFiles < 200) // 文件太多就不预先计算了
	{
		isSizeCaled = true;
		for(i = 0; i < (thrdData->nFiles); i++)
		{
			if(thrdData->stop)
			{
				thrdData->threadWorking = false;
				uiBridge->calcStop();
				return 0;
			}
			uint64_t fSize = 0;
		
			const TCHAR* path;
			path = thrdData->fullPaths[i].c_str();
			OsFile osFile(path);
			if(osFile.openRead())
			{
				fSize = osFile.getLength();//fsize=status.m_size; // Fix 4GB file
				osFile.close();
			}

			fSizes[i] = fSize;
			thrdData->totalSize += fSize;
		}
	}

	uiBridge->removePreparingCalc();

	// 计算循环
	for(i = 0; i < (thrdData->nFiles); i++)
	{
		if(thrdData->stop)
		{
			thrdData->threadWorking = false;
			uiBridge->calcStop();
			return 0;
		}

#if defined (WIN32)
		Sleep(50);
#else
		usleep(50 * 1000);
#endif

		// Declaration for calculator
		const TCHAR* path;
		uint64_t fsize, times, t = 0;
		finishedSize = 0;
		//unsigned int len;
		//unsigned char buffer[65534];
		DataBuffer databuf;
		
		MD5_CTX mdContext; // MD5 context

		CSHA1 sha1; // SHA1 object
		char strSHA1[256];

		SHA256_CTX sha256Ctx; // SHA256 context
		string strSHA256;

		uint32_t ulCRC32; // CRC32 context
		// Declaration for calculator
		
		ResultData resultNew;
        thrdData->resultList.push_back(resultNew);
        ResultData& result = thrdData->resultList.back();
        
        result.enumState = RESULT_NONE;
        
		path = thrdData->fullPaths[i].c_str();
		result.tstrPath = thrdData->fullPaths[i];

		int position = 0; // 进度条位置

        result.enumState = RESULT_PATH;
        
		uiBridge->showFileName(result);

		//Calculating begins
#if defined (WIN32)
		CFileException fExc;
#else
		char fExc[1024] = {0};
#endif
		OsFile osFile(path);
		if(osFile.openRead((void *)&fExc)) 
		{
			MD5Init(&mdContext, 0); // MD5开始
			sha1.Reset(); // SHA1开始
			sha256_init(&sha256Ctx); // SHA256开始
			crc32Init(&ulCRC32); // CRC32开始

			uiBridge->updateProg(0);

			/*
			// get file size - start //
			fsize=thrdData->File.GetLength();
			tsize+=fsize;
			// get file size - end //
			*/

			// get file status //
			tstring tstrLastModifiedTime;
#if defined (WIN32)
			CTime ctModifedTime;
#else
			struct timespec ctModifedTime;
#endif
			if(osFile.getModifiedTime((void *)&ctModifedTime))
			{
#if defined (WIN32)
				tstrLastModifiedTime = ctModifedTime.Format("%Y-%m-%d %H:%M").GetString();
#else
				time_t ttModifiedTime;
				struct tm *tmModifiedTime;
				
				ttModifiedTime = ctModifedTime.tv_sec;
				tmModifiedTime = localtime(&ttModifiedTime);
				
				char szTmBuf[1024] = {0};
				strftime(szTmBuf, 1024, "%Y-%m-%d %H:%M", tmModifiedTime);
				
				tstrLastModifiedTime = strtotstr(string(szTmBuf));
#endif
				result.tstrMDate = tstrLastModifiedTime;
			}
			
			fsize = osFile.getLength(); // Fix 4GB file
			result.ulSize = fsize;
			
			if(!isSizeCaled) // 如果没有计算过大小
			{
				thrdData->totalSize += fsize;
			}
			else
			{
				thrdData->totalSize = thrdData->totalSize + fsize - fSizes[i]; // 修正总大小
				fSizes[i] = fsize; // 修正文件大小
			}

#if defined (WIN32)
			// get file version //
			CString cstrVer = WindowsUtils::GetExeFileVersion((TCHAR *)path);
			tstrFileVersion = cstrVer.GetString();
			result.tstrVersion = tstrFileVersion;
#endif
            
            result.enumState = RESULT_META;

			uiBridge->showFileMeta(result);
			
			// get calculating times //
			times = fsize / DataBuffer::preflen + 1;
			
			//UINT bufferSize = sizeof(buffer);
			do 
			{	
				if(thrdData->stop)
				{
					osFile.close();

					thrdData->threadWorking = false;
					uiBridge->calcStop();
					return 0;
				}
				int64_t readRet = osFile.read(databuf.data, DataBuffer::preflen);
				if(readRet >= 0)
					databuf.datalen = (unsigned int)readRet;
				else
					databuf.datalen = 0;
				
				t++;

				MD5Update(&mdContext, databuf.data, databuf.datalen); // MD5更新
				sha1.Update(databuf.data, databuf.datalen); // SHA1更新
				sha256_update(&sha256Ctx, databuf.data, databuf.datalen); // SHA256更新
				crc32Update(&ulCRC32, databuf.data, databuf.datalen); // CRC32更新
				
				finishedSize += databuf.datalen;
				
				int progressMax = uiBridge->getProgMax();
				
				int positionNew;
				if(fsize == 0)
					positionNew = progressMax; // 注意除0错误
				else
					positionNew = (int)(progressMax * finishedSize / fsize);
				
				if(positionNew > position)
				{
					uiBridge->updateProg(positionNew);
					position = positionNew;
				}

				finishedSizeWhole += databuf.datalen;
				int positionWholeNew;
				if(thrdData->totalSize == 0)
					positionWholeNew = progressMax; // 注意除0错误
				else
					positionWholeNew = (int)(progressMax * finishedSizeWhole / thrdData->totalSize);
				if(isSizeCaled && positionWholeNew > positionWhole)
				{
					positionWhole = positionWholeNew;
					uiBridge->updateProgWhole(positionWhole);
				}

			} 
			while(databuf.datalen >= DataBuffer::preflen);
			
			uiBridge->fileCalcFinish();
			
			MD5Final (&mdContext); // MD5完成
			sha1.Final(); // SHA1完成
			sha256_final(&sha256Ctx); // SHA256完成
			crc32Finish(&ulCRC32); //CRC32完成

			if(!isSizeCaled)
			{
				if(thrdData->nFiles == 0)
				{
					uiBridge->updateProgWhole(0);
				}
				else
				{
					uiBridge->updateProgWhole((i + 1) * 100 / (thrdData->nFiles));
				}
			}

			osFile.close();
			//Calculating ends

			char chHashBuff[1024] = {0};

			// MD5
#if defined (WIN32)
			sprintf_s(chHashBuff, 1024, 
								"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
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
#else
			sprintf(chHashBuff,
					  "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
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
#endif
			tstrFileMD5 = strtotstr(string(chHashBuff));

			// SHA1
			sha1.ReportHash(strSHA1, CSHA1::REPORT_HEX);
			tstrFileSHA1 = strtotstr(string(strSHA1));

			// SHA256
			sha256_digest(&sha256Ctx, &strSHA256);
			tstrFileSHA256 = strtotstr(string(strSHA256));

			// CRC32
#if defined (WIN32)
			sprintf_s(chHashBuff, 1024, "%08X", ulCRC32);
#else
			sprintf(chHashBuff, "%08X", ulCRC32);
#endif
			tstrFileCRC32 = strtotstr(string(chHashBuff));
			
			// 在没做转换前，结果都是大写的
			result.tstrMD5 = tstrFileMD5;
			result.tstrSHA1 = tstrFileSHA1;
			result.tstrSHA256 = tstrFileSHA256;
			result.tstrCRC32 = tstrFileCRC32;
            
            result.enumState = RESULT_ALL;
			
			uiBridge->showFileHash(result, thrdData->uppercase);
		} // end if(File.Open(path, CFile::modeRead|CFile::shareDenyWrite, &ex)) 
		else
		{
#if defined (WIN32)
			TCHAR szError[1024] = {0};
			fExc.GetErrorMessage(szError, 1024);
			result.tstrError = szError;
#else
			result.tstrError = strtotstr(string(fExc));
#endif

            result.enumState = RESULT_ERROR;

			uiBridge->showFileErr(result);
		}
		
		uiBridge->fileFinish();
	} // end for(i = 0; i < (thrdData->nFiles); i++)

	uiBridge->calcFinish();
	
	thrdData->threadWorking = false;

	return 0;
}
