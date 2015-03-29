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

#include "strhelper.h"

using namespace std;
using namespace sunjwbase;

#define WINBOOL_2_CBOOL(winbool_var) bool((winbool_var) == TRUE)

#define GET_CFILE_FROM_POINTER(pointer) ((CFile *)(pointer))

OsFile::OsFile(tstring filePath):
	_filePath(filePath),
	_osfileData(new CFile())
{
	
}

OsFile::~OsFile()
{
	if (_osfileData != NULL)
	{
		delete GET_CFILE_FROM_POINTER(_osfileData);
	}
}

bool OsFile::openRead()
{
	BOOL ret = FALSE;

	CFileException ex;
	CFile* cfile = GET_CFILE_FROM_POINTER(_osfileData);
	ret = cfile->Open(_filePath.c_str(), CFile::modeRead|CFile::shareDenyWrite, &ex);

	return WINBOOL_2_CBOOL(ret);
}

void OsFile::close()
{
	CFile* cfile = GET_CFILE_FROM_POINTER(_osfileData);

	if (cfile->m_hFile != CFile::hFileNull)
	{
		cfile->Close();
	}
}
