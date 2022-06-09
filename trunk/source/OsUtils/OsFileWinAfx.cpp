/*
 * OsFile Windows AFX implementation file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider basic open/close, read, write and 
 * attributes functions of file.
 */
#include "stdafx.h"

#include "OsFile.h"

#include <stdlib.h>
#include <stdint.h>

#include "Windows.h"
#include "afx.h"

#include "Common/strhelper.h"

using namespace std;
using namespace sunjwbase;

#define WINBOOL_2_CBOOL(winbool_var) bool((winbool_var) == TRUE)

#define GET_CFILE_FROM_POINTER(pointer) ((CFile *)(pointer))

OsFile::OsFile(tstring filePath):
	_filePath(filePath),
	_osfileData(new CFile()),
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

		delete GET_CFILE_FROM_POINTER(_osfileData);
	}
}

bool OsFile::open(void *flag, void *exception)
{
	BOOL ret = FALSE;

	CFileException fExc;
	CFileException *pFileExc = (CFileException *)exception;
	if (pFileExc == NULL)
	{
		pFileExc = &fExc; 
	}

	UINT cfFlag = (UINT)flag;
	CFile* cfile = GET_CFILE_FROM_POINTER(_osfileData);
	ret = cfile->Open(_filePath.c_str(), cfFlag, pFileExc);

	return WINBOOL_2_CBOOL(ret);
}

bool OsFile::openRead(void *exception/* = NULL*/)
{
	bool ret = false;
	
	ret = this->open((void *)(CFile::modeRead|CFile::shareDenyWrite), 
		exception);

	if (ret == true)
	{
		_fileStatus = OPEN_READ;
	}

	return ret;
}

bool OsFile::openReadScan(void *exception/* = NULL*/)
{
	bool ret = false;

	ret = this->open((void *)(CFile::modeRead | CFile::shareDenyWrite|CFile::osSequentialScan),
		exception);

	if (ret == true)
	{
		_fileStatus = OPEN_READ;
	}

	return ret;
}

bool OsFile::openWrite(void *exception/* = NULL*/)
{
	bool ret = false;
	
	ret = this->open((void *)(CFile::modeWrite|CFile::shareExclusive), 
		exception);

	if (ret == true)
	{
		_fileStatus = OPEN_WRITE;
	}

	return ret;
}

bool OsFile::openReadWrite(void *exception/* = NULL*/)
{
	bool ret = false;
	
	ret = this->open((void *)(CFile::modeReadWrite|CFile::shareExclusive), 
		exception);

	if (ret == true)
	{
		_fileStatus = OPEN_READWRITE;
	}

	return ret;
}

int64_t OsFile::getLength()
{
	int64_t retLength = 0;

	CFileException ex;
	CFile* cfile = GET_CFILE_FROM_POINTER(_osfileData);

	bool needClose = false;

	// Not opened, let's open it.
	if (_fileStatus == CLOSED && openRead())
	{
		needClose = true;
	}

	if (_fileStatus != CLOSED)
	{
		retLength = cfile->GetLength();
	}

	if (needClose)
	{
		close();
	}

	return retLength;
}

bool OsFile::getModifiedTime(void *modifiedTime)
{
	CTime *cfileModTime = (CTime *)modifiedTime;

	CFileStatus cfileStatus;

	if (CFile::GetStatus(_filePath.c_str(), cfileStatus))
	{
		*cfileModTime = cfileStatus.m_mtime;

		return true;
	}

	return false;
}

tstring OsFile::getModifiedTimeFormat()
{
	tstring tstrLastModifiedTime;
	CTime ctModifedTime;
	if (this->getModifiedTime((void *)&ctModifedTime))
	{
		tstrLastModifiedTime = ctModifedTime.Format("%Y-%m-%d %H:%M").GetString();
	}

	return tstrLastModifiedTime;
}

uint64_t OsFile::seek(uint64_t offset, OsFileSeekFrom from)
{
	UINT cfSeekFlag = CFile::begin;
	switch (from)
	{
	case OF_SEEK_BEGIN:
		cfSeekFlag = CFile::begin;
		break;
	case OF_SEEK_CUR:
		cfSeekFlag = CFile::current;
		break;
	case OF_SEEK_END:
		cfSeekFlag = CFile::end;
		break;
	}

	// Open first, we don't check here.
	CFile* cfile = GET_CFILE_FROM_POINTER(_osfileData);

	return cfile->Seek(offset, cfSeekFlag);
}

int64_t OsFile::read(void *readBuffer, uint32_t bytes)
{
	// Open first, we don't check here.
	CFile* cfile = GET_CFILE_FROM_POINTER(_osfileData);

	return cfile->Read(readBuffer, bytes);
}

int64_t OsFile::write(void *writeBuffer, uint32_t bytes)
{
	// Open first, we don't check here.
	CFile* cfile = GET_CFILE_FROM_POINTER(_osfileData);

	cfile->Write(writeBuffer, bytes);

	return bytes;
}

void OsFile::close()
{
	CFile* cfile = GET_CFILE_FROM_POINTER(_osfileData);

	if (_fileStatus != CLOSED)
	{
		cfile->Close();
		_fileStatus = CLOSED;
	}
}
