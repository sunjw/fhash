/*
 * OsFile posix (darwin) implementation file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider basic open/close, read, write and 
 * attributes functions of file.
 */
#include "stdafx.h"

#include "OsFile.h"

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

#include "Common/strhelper.h"

using namespace std;
using namespace sunjwbase;

#define GET_FD_FROM_POINTER(pointer) ((int *)(pointer))

OsFile::OsFile(tstring filePath):
    _filePath(filePath),
    _osfileData(new int(-1)),
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

        delete GET_FD_FROM_POINTER(_osfileData);
    }
}

bool OsFile::open(void *flag, void *exception)
{
    string strFilePath = tstrtostr(_filePath);
    char *pFileExc = (char *)exception;
    int *fd = GET_FD_FROM_POINTER(_osfileData);
    *fd = -1;

    int statRet = -1;
    struct stat st;
    if ((statRet = stat(strFilePath.c_str(), &st)) == 0 &&
        (st.st_mode & S_IFREG))
    {
        
        int posixFlag = (int)(uint64_t)flag;
        
        *fd = ::open(strFilePath.c_str(), posixFlag);
        
        if (*fd == -1 && pFileExc != NULL)
        {
            strlcpy(pFileExc, "Cannot open this file.", OsFile::ERR_MSG_BUFFER_LEN);
        }
    }
    else if (statRet == 0 && (st.st_mode & S_IFDIR))
    {
        if (pFileExc != NULL)
            strlcpy(pFileExc, "Cannot open a directory.", OsFile::ERR_MSG_BUFFER_LEN);
    }
    else
    {
        if (pFileExc != NULL)
            strlcpy(pFileExc, "File is missing.", OsFile::ERR_MSG_BUFFER_LEN);
    }

    return (*fd != -1);
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

tstring OsFile::getModifiedTimeFormat()
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
        strftime(szTmBuf, 1024, "%Y-%m-%d %H:%M", tmModifiedTime);

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
    int *fd = GET_FD_FROM_POINTER(_osfileData);

    if (_fileStatus != CLOSED)
    {
        ::close(*fd);
        *fd = -1;
        _fileStatus = CLOSED;
    }
}
