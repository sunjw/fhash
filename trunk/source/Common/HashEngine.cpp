#include "stdafx.h"

#include "HashEngine.h"

#include <stdlib.h>

#if defined (FHASH_THREAD_HASH_UPDATE)
#include "Common/ThreadPool.h"
#endif

#if defined (__APPLE__) || defined (__unix)
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "Common/strhelper.h"
#include "Common/UIBridgeBase.h"

#if defined (WIN32)
#include "WinCommon/WindowsComm.h"
#if defined (FHASH_UWP_LIB)
#include "WinCommon/FileVersionHelper.h"
#endif
#endif

#include "OsUtils/OsFile.h"
#include "OsUtils/OsThread.h"

#include "Algorithms/MD5.h"
#include "Algorithms/SHA1.h"
#include "Algorithms/sha256.h"
#include "Algorithms/sha512.h"

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

static void MD5UpdateWrapper(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen)
{
	MD5Update(mdContext, inBuf, inLen); // MD5 update
}

static void SHA1UpdateWrapper(CSHA1 *sha1, unsigned char *data, unsigned int len)
{
	sha1->Update(data, len); // SHA1 update
}

static void SHA256UpdateWrapper(struct sha256_ctx *ctx, const unsigned char *buffer, uint32_t length)
{
	sha256_update(ctx, buffer, length); // SHA256 update
}

static void SHA512UpdateWrapper(SHA512_CTX *context, void *datain, size_t len)
{
	SHA512_Update(context, datain, len); // SHA512 update
}

// working thread
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
	int positionWhole = 0; // whole position

	tstring tstrFileSize;
	tstring tstrFileVersion;
	tstring tstrFileMD5;
	tstring tstrFileSHA1;
	tstring tstrFileSHA256;
	tstring tstrFileSHA512;

#if defined (FHASH_THREAD_HASH_UPDATE)
	// Create thread pool with 4 threads
	ThreadPool threadPool(4);
	// Initialize thread pool
	threadPool.init();
#endif

	uiBridge->preparingCalc();

	// get total files size
	if (thrdData->nFiles < 200) // not too many
	{
		isSizeCaled = true;
		for (i = 0; i < (thrdData->nFiles); i++)
		{
			if (thrdData->stop)
			{
#if defined (FHASH_THREAD_HASH_UPDATE)
				threadPool.shutdown();
#endif
				thrdData->threadWorking = false;

				uiBridge->calcStop();
				return 0;
			}
			uint64_t fSize = 0;

			const TCHAR* path;
			path = thrdData->fullPaths[i].c_str();
			OsFile osFile(path);
			if (osFile.openRead())
			{
				fSize = osFile.getLength();//fsize=status.m_size; // Fix 4GB file
				osFile.close();
			}

			fSizes[i] = fSize;
			thrdData->totalSize += fSize;
		}
	}

	uiBridge->removePreparingCalc();

	// loop all files
	for (i = 0; i < (thrdData->nFiles); i++)
	{
		if (thrdData->stop)
		{
#if defined (FHASH_THREAD_HASH_UPDATE)
			threadPool.shutdown();
#endif
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

		SHA512_CTX sha512Ctx; // SHA512 context
		uint8_t digestSHA512[SHA512_DIGEST_LENGTH];
		string strSHA512;
		// Declaration for calculator

		ResultData resultNew;
		thrdData->resultList.push_back(resultNew);
		ResultData& result = thrdData->resultList.back();

		result.enumState = RESULT_NONE;

		path = thrdData->fullPaths[i].c_str();
		result.tstrPath = thrdData->fullPaths[i];

		int position = 0; // progress bar position

		result.enumState = RESULT_PATH;

		uiBridge->showFileName(result);

		//Calculating begins
#if defined (WIN32)
		// CFileException fExc;
		TCHAR fExc[OsFile::ERR_MSG_BUFFER_LEN] = { 0 };
#else
		char fExc[OsFile::ERR_MSG_BUFFER_LEN] = { 0 };
#endif
		OsFile osFile(path);
		if (osFile.openReadScan((void *)&fExc)) 
		{
			MD5Init(&mdContext, 0); // MD5 init
			sha1.Reset(); // SHA1 init
			sha256_init(&sha256Ctx); // SHA256 init
			SHA512_Init(&sha512Ctx); // SHA512 init

			uiBridge->updateProg(0);

			/*
			// get file size - start //
			fsize=thrdData->File.GetLength();
			tsize+=fsize;
			// get file size - end //
			*/

			// get file status //
			tstring tstrLastModifiedTime = osFile.getModifiedTimeFormat();
			result.tstrMDate = tstrLastModifiedTime;
			
			fsize = osFile.getLength(); // fix 4GB file
			result.ulSize = fsize;

			if (!isSizeCaled) // not calculated size
			{
				thrdData->totalSize += fsize;
			}
			else
			{
				thrdData->totalSize = thrdData->totalSize + fsize - fSizes[i]; // fix total size
				fSizes[i] = fsize; // fix file size
			}

#if defined (WIN32)
			// get file version //
#if defined (FHASH_UWP_LIB)
			WindowsComm::FileVersionHelper fvHelper(osFile);
			tstrFileVersion = fvHelper.Find();
			result.tstrVersion = tstrFileVersion;
			osFile.seek(0, OsFile::OsFileSeekFrom::OF_SEEK_BEGIN); // reset offset
#else
			tstrFileVersion = WindowsComm::GetExeFileVersion((TCHAR *)path);
			result.tstrVersion = tstrFileVersion;
#endif
#endif

			result.enumState = RESULT_META;

			uiBridge->showFileMeta(result);

			// get calculating times //
			times = fsize / DataBuffer::preflen + 1;
			
			//UINT bufferSize = sizeof(buffer);
			do 
			{	
				if (thrdData->stop)
				{
					osFile.close();
#if defined (FHASH_THREAD_HASH_UPDATE)
					threadPool.shutdown();
#endif
					thrdData->threadWorking = false;

					uiBridge->calcStop();
					return 0;
				}
				int64_t readRet = osFile.read(databuf.data, DataBuffer::preflen);
				if (readRet >= 0)
				{
					databuf.datalen = (unsigned int)readRet;
				}
				else
				{
					databuf.datalen = 0;
				}

				t++;

#if defined (FHASH_THREAD_HASH_UPDATE)
				// multi threads
				future<void> taskSHA512Update = threadPool.submit(SHA512UpdateWrapper, &sha512Ctx, databuf.data, databuf.datalen);
				future<void> taskSHA256Update = threadPool.submit(SHA256UpdateWrapper, &sha256Ctx, databuf.data, databuf.datalen);
				future<void> taskSHA1Update = threadPool.submit(SHA1UpdateWrapper, &sha1, databuf.data, databuf.datalen);
				future<void> taskMD5Update = threadPool.submit(MD5UpdateWrapper, &mdContext, databuf.data, databuf.datalen);

				taskSHA512Update.wait();
				taskSHA256Update.wait();
				taskSHA1Update.wait();
				taskMD5Update.wait();
#else
				// single thread
				//MD5Update(&mdContext, databuf.data, databuf.datalen); // MD5 update
				MD5UpdateWrapper(&mdContext, databuf.data, databuf.datalen);
				//sha1.Update(databuf.data, databuf.datalen); // SHA1 update
				SHA1UpdateWrapper(&sha1, databuf.data, databuf.datalen);
				//sha256_update(&sha256Ctx, databuf.data, databuf.datalen); // SHA256 update
				SHA256UpdateWrapper(&sha256Ctx, databuf.data, databuf.datalen);
				//SHA512_Update(&sha512Ctx, databuf.data, databuf.datalen); // SHA512 update
				SHA512UpdateWrapper(&sha512Ctx, databuf.data, databuf.datalen);
#endif

				finishedSize += databuf.datalen;

				int progressMax = uiBridge->getProgMax();

				int positionNew;
				if (fsize == 0)
				{
					positionNew = progressMax;
				}
				else
				{
					positionNew = (int)(progressMax * finishedSize / fsize);
				}

				if (positionNew > position)
				{
					uiBridge->updateProg(positionNew);
					position = positionNew;
				}

				finishedSizeWhole += databuf.datalen;
				int positionWholeNew;
				if (thrdData->totalSize == 0)
				{
					positionWholeNew = progressMax;
				}
				else
				{
					positionWholeNew = (int)(progressMax * finishedSizeWhole / thrdData->totalSize);
				}
				if (isSizeCaled && positionWholeNew > positionWhole)
				{
					positionWhole = positionWholeNew;
					uiBridge->updateProgWhole(positionWhole);
				}

			} 
			while (databuf.datalen >= DataBuffer::preflen);

			uiBridge->fileCalcFinish();

			MD5Final(&mdContext); // MD5 final
			sha1.Final(); // SHA1 final
			sha256_final(&sha256Ctx); // SHA256 final
			SHA512_Final(digestSHA512, &sha512Ctx); // SHA256 final

			if (!isSizeCaled)
			{
				if (thrdData->nFiles == 0)
				{
					uiBridge->updateProgWhole(0);
				}
				else
				{
					int progressMax = uiBridge->getProgMax();
					uiBridge->updateProgWhole((i + 1) * progressMax / (thrdData->nFiles));
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
			tstrFileSHA256 = strtotstr(strSHA256);

			// SHA512
			for (int p = 0; p < SHA512_DIGEST_LENGTH; p++)
			{
				char buf[8] = { 0 };
#if defined (WIN32)
				sprintf_s(buf, 8, "%02X", digestSHA512[p]);
#else
				sprintf(buf, "%02X", digestSHA512[p]);
#endif
				strSHA512.append(std::string(buf));
			}
			tstrFileSHA512 = strtotstr(strSHA512);

			// all upper case
			result.tstrMD5 = tstrFileMD5;
			result.tstrSHA1 = tstrFileSHA1;
			result.tstrSHA256 = tstrFileSHA256;
			result.tstrSHA512 = tstrFileSHA512;

			result.enumState = RESULT_ALL;

			uiBridge->showFileHash(result, thrdData->uppercase);
		} // end if(File.Open(path, CFile::modeRead|CFile::shareDenyWrite, &ex)) 
		else
		{
#if defined (WIN32)
			/*TCHAR szError[1024] = {0};
			fExc.GetErrorMessage(szError, 1024);
			result.tstrError = szError;*/
			result.tstrError = fExc;
#else
			result.tstrError = strtotstr(string(fExc));
#endif

			result.enumState = RESULT_ERROR;

			uiBridge->showFileErr(result);
		}

		uiBridge->fileFinish();
	} // end for(i = 0; i < (thrdData->nFiles); i++)

	uiBridge->calcFinish();

#if defined (FHASH_THREAD_HASH_UPDATE)
	threadPool.shutdown();
#endif
	thrdData->threadWorking = false;

	return 0;
}
