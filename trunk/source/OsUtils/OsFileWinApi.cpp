/*
 * OsFile posix (darwin) implementation file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider basic open/close, read, write and 
 * attributes functions of file.
 */
#include "stdafx.h"

#include "OsFile.h"

#include <Windows.h>
#include <strsafe.h>

using namespace sunjwbase;

#define WINBOOL_2_CBOOL(winbool_var) bool((winbool_var) == TRUE)

// HANDLE == void *

struct CreateFileFlag
{
	DWORD dwDesiredAccess;
	DWORD dwShareMode;
	DWORD dwCreationDisposition;
	DWORD dwFlagsAndAttributes;
};

OsFile::OsFile(tstring filePath):
	_filePath(filePath),
	_osfileData(NULL),
	_fileStatus(CLOSED)
{

}

OsFile::~OsFile()
{
	if (_osfileData != NULL)
	{
		if (_fileStatus != CLOSED)
		{
			close();
		}
	}
}

bool OsFile::open(void *flag, void *exception)
{
	CreateFileFlag* fileFlag = (CreateFileFlag*)flag;
	TCHAR *pFileExc = (TCHAR *)exception;

	_osfileData = CreateFile(_filePath.c_str(), // file to open
		fileFlag->dwDesiredAccess, // open for reading
		fileFlag->dwShareMode, // share for reading
		NULL, // default security
		fileFlag->dwCreationDisposition, // existing file only
		fileFlag->dwFlagsAndAttributes, // normal file
		NULL); // no attr. template

	if (_osfileData == INVALID_HANDLE_VALUE)
	{
		DWORD dw = GetLastError();
		if (pFileExc != NULL)
		{
#if defined(UNICODE) || defined(_UNICODE)
			wcscpy_s(pFileExc, OsFile::ERR_MSG_BUFFER_LEN, TEXT("Cannot open this file."));
#else
			strcpy_s(pFileExc, OsFile::ERR_MSG_BUFFER_LEN, TEXT("Cannot open this file."));
#endif
		}
	}
	
	return (_osfileData != INVALID_HANDLE_VALUE);
}

bool OsFile::openRead(void *exception/* = NULL*/)
{
	bool ret = false;
	
	CreateFileFlag fileFlag;
	fileFlag.dwDesiredAccess = GENERIC_READ;
	fileFlag.dwShareMode = FILE_SHARE_READ;
	fileFlag.dwCreationDisposition = OPEN_EXISTING;
	fileFlag.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	ret = this->open((void *)&fileFlag, exception);

	if (ret == true)
	{
		_fileStatus = OPEN_READ;
	}

	return ret;
}

bool OsFile::openReadScan(void *exception/* = NULL*/)
{
	bool ret = false;

	CreateFileFlag fileFlag;
	fileFlag.dwDesiredAccess = GENERIC_READ;
	fileFlag.dwShareMode = FILE_SHARE_READ;
	fileFlag.dwCreationDisposition = OPEN_EXISTING;
	fileFlag.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN;
	ret = this->open((void*)&fileFlag, exception);

	if (ret == true)
	{
		_fileStatus = OPEN_READ;
	}

	return ret;
}

bool OsFile::openWrite(void *exception/* = NULL*/)
{
	bool ret = false;

	CreateFileFlag fileFlag;
	fileFlag.dwDesiredAccess = GENERIC_WRITE;
	fileFlag.dwShareMode = 0;
	fileFlag.dwCreationDisposition = CREATE_NEW;
	fileFlag.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	ret = this->open((void*)&fileFlag, exception);

	if (ret == true)
	{
		_fileStatus = OPEN_READ;
	}

	return ret;
}

bool OsFile::openReadWrite(void *exception/* = NULL*/)
{
	bool ret = false;

	CreateFileFlag fileFlag;
	fileFlag.dwDesiredAccess = GENERIC_WRITE | GENERIC_READ;
	fileFlag.dwShareMode = 0;
	fileFlag.dwCreationDisposition = CREATE_NEW;
	fileFlag.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	ret = this->open((void*)&fileFlag, exception);

	if (ret == true)
	{
		_fileStatus = OPEN_READWRITE;
	}

	return ret;
}

int64_t OsFile::getLength()
{
	int64_t retLength = 0;

	bool needClose = false;

	// Not opened, let's open it.
	if (_fileStatus == CLOSED && openRead())
	{
		needClose = true;
	}

	if (_fileStatus != CLOSED)
	{
		LARGE_INTEGER liSize = { 0 };
		if (GetFileSizeEx(_osfileData, &liSize))
		{
			retLength = liSize.QuadPart;
		}
	}

	if (needClose)
	{
		close();
	}

	return retLength;
}

bool OsFile::getModifiedTime(void *modifiedTime)
{
	FILETIME *lpFtWrite = (FILETIME *)modifiedTime;

	bool needClose = false;

	// Not opened, let's open it.
	if (_fileStatus == CLOSED && openRead())
	{
		needClose = true;
	}

	BOOL ret = FALSE;
	if (_fileStatus != CLOSED)
	{
		ret = GetFileTime(_osfileData, NULL, NULL, lpFtWrite);
	}

	if (needClose)
	{
		close();
	}

	return WINBOOL_2_CBOOL(ret);
}

tstring OsFile::getModifiedTimeFormat()
{
	tstring tstrLastModifiedTime;

	FILETIME ftWrite;
	if (this->getModifiedTime((void*)&ftWrite))
	{
		// Convert the last-write time to local time.
		SYSTEMTIME stUTC, stLocal;
		FileTimeToSystemTime(&ftWrite, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

		TCHAR tzTmBuf[1024] = { 0 };
		StringCchPrintf(tzTmBuf, 1024,
			TEXT("%d-%02d-%02d %02d:%02d"),
			stLocal.wYear, stLocal.wMonth, stLocal.wDay,
			stLocal.wHour, stLocal.wMinute);
		tstrLastModifiedTime = tzTmBuf;
	}

	return tstrLastModifiedTime;
}

uint64_t OsFile::seek(uint64_t offset, OsFileSeekFrom from)
{
	DWORD dwMoveMethod = FILE_BEGIN;
	switch (from)
	{
	case OF_SEEK_BEGIN:
		dwMoveMethod = FILE_BEGIN;
		break;
	case OF_SEEK_CUR:
		dwMoveMethod = FILE_CURRENT;
		break;
	case OF_SEEK_END:
		dwMoveMethod = FILE_END;
		break;
	}

	// Open first, we don't check here.
	LARGE_INTEGER liDistance = { 0 };
	liDistance.QuadPart = offset;
	LARGE_INTEGER liNewPos = { 0 };
	SetFilePointerEx(_osfileData, liDistance, &liNewPos, dwMoveMethod);

	return liNewPos.QuadPart;
}

int64_t OsFile::read(void *readBuffer, uint32_t bytes)
{
	// Open first, we don't check here.
	DWORD dwNumberOfBytesRead = 0;
	ReadFile(_osfileData, readBuffer, bytes, &dwNumberOfBytesRead, NULL);

	return dwNumberOfBytesRead;
}

int64_t OsFile::write(void *writeBuffer, uint32_t bytes)
{
	// Open first, we don't check here.
	DWORD lpNumberOfBytesWritten = 0;
	WriteFile(_osfileData, writeBuffer, bytes, &lpNumberOfBytesWritten, NULL);

	return lpNumberOfBytesWritten;
}

void OsFile::close()
{
	if (_fileStatus != CLOSED)
	{
		CloseHandle(_osfileData);
		_osfileData = NULL;
		_fileStatus = CLOSED;
	}
}
