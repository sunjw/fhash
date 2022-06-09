/*
 * OsFile posix (darwin) implementation file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider basic open/close, read, write and 
 * attributes functions of file.
 */
#include "stdafx.h"

#include "OsFile.h"
#include "Windows.h"

using namespace sunjwbase;

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
		if (pFileExc != NULL)
		{
#if defined(UNICODE) || defined(_UNICODE)
			wcscpy_s(pFileExc, 1024, TEXT("Cannot open this file."));
#else
			strcpy_s(pFileExc, 1024, TEXT("Cannot open this file."));
#endif
		}
	}
	
	return (_osfileData != INVALID_HANDLE_VALUE);
}

bool OsFile::openRead(void *exception/* = NULL*/)
{
	bool ret = false;
	
	ret = this->open((void *)(O_RDONLY), exception);

	if (ret == true)
	{
		_fileStatus = OPEN_READ;
	}

	return ret;
}

bool OsFile::openReadScan(void *exception/* = NULL*/)
{
	return this->openRead(exception);
}

bool OsFile::openWrite(void *exception/* = NULL*/)
{
	bool ret = false;
	
	ret = this->open((void *)(O_RDWR | O_CREAT | O_SYNC), exception);

	if (ret == true)
	{
		_fileStatus = OPEN_WRITE;
	}

	return ret;
}

bool OsFile::openReadWrite(void *exception/* = NULL*/)
{
	return this->openWrite(exception);
}

int64_t OsFile::getLength()
{
	int64_t retLength = 0;
	string strFilePath = tstrtostr(_filePath);
	
	struct stat st;
	if (stat(strFilePath.c_str(), &st) == 0)
	{
		retLength = st.st_size;
	}

	return retLength;
}

bool OsFile::getModifiedTime(void *modifiedTime)
{
	struct timespec *darwinfileModTime = (struct timespec *)modifiedTime;
	
	string strFilePath = tstrtostr(_filePath);
	
	struct stat st;
	if (stat(strFilePath.c_str(), &st) == 0)
	{
		*darwinfileModTime = st.st_mtimespec;
		
		return true;
	}

	return false;
}

tstring OsFile::getModifiedTimeFormat(tstring timeFormat)
{
	tstring tstrLastModifiedTime;
	struct timespec ctModifedTime;
	if (this->getModifiedTime((void *)&ctModifedTime))
	{
		time_t ttModifiedTime;
		struct tm *tmModifiedTime;

		ttModifiedTime = ctModifedTime.tv_sec;
		tmModifiedTime = localtime(&ttModifiedTime);

		char szTmBuf[1024] = {0};
		strftime(szTmBuf, 1024, timeFormat.c_str(), tmModifiedTime);

		tstrLastModifiedTime = strtotstr(string(szTmBuf));
	}

	return tstrLastModifiedTime;
}

uint64_t OsFile::seek(uint64_t offset, OsFileSeekFrom from)
{
	int posixSeekFlag = SEEK_SET;
	switch(from)
	{
	case OF_SEEK_BEGIN:
		posixSeekFlag = SEEK_SET;
		break;
	case OF_SEEK_CUR:
		posixSeekFlag = SEEK_CUR;
		break;
	case OF_SEEK_END:
		posixSeekFlag = SEEK_END;
		break;
	}

	// Open first, we don't check here.
	int *fd = GET_FD_FROM_POINTER(_osfileData);

	return ::lseek(*fd, offset, posixSeekFlag);
}

int64_t OsFile::read(void *readBuffer, uint32_t bytes)
{
	// Open first, we don't check here.
	int *fd = GET_FD_FROM_POINTER(_osfileData);

	return ::read(*fd, readBuffer, bytes);
}

int64_t OsFile::write(void *writeBuffer, uint32_t bytes)
{
	// Open first, we don't check here.
	int *fd = GET_FD_FROM_POINTER(_osfileData);

	return ::write(*fd, writeBuffer, bytes);
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
