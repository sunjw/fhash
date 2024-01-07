/*
 * OsFile Windows UWP implementation file
 * Author: Sun Junwen
 * Version: 0.5
 * Provider basic open/close, read, write and
 * attributes functions of file.
 */
#include "stdafx.h"

#include "OsFile.h"

#include <Windows.h>
#include <strsafe.h>

// IStorageItemHandleAccess copied from https://github.com/microsoft/HoloJS/blob/master/windows/src/uwp-component/file-handle-access.h

// These are copied from WindowsStorageCOM.h
// You can remove this header file once the real file has been updated
// to fix the WINAPI_PARTITION_DESKTOP block

typedef interface IOplockBreakingHandler IOplockBreakingHandler;
typedef interface IStorageItemHandleAccess IStorageItemHandleAccess;
typedef interface IStorageFolderHandleAccess IStorageFolderHandleAccess;

#ifdef __cplusplus
extern "C" {
#endif

	typedef /* [v1_enum] */
		enum HANDLE_OPTIONS {
		HO_NONE = 0,
		HO_OPEN_REQUIRING_OPLOCK = 0x40000,
		HO_DELETE_ON_CLOSE = 0x4000000,
		HO_SEQUENTIAL_SCAN = 0x8000000,
		HO_RANDOM_ACCESS = 0x10000000,
		HO_NO_BUFFERING = 0x20000000,
		HO_OVERLAPPED = 0x40000000,
		HO_WRITE_THROUGH = 0x80000000
	} HANDLE_OPTIONS;

	DEFINE_ENUM_FLAG_OPERATORS(HANDLE_OPTIONS);
	typedef /* [v1_enum] */
		enum HANDLE_ACCESS_OPTIONS {
		HAO_NONE = 0,
		HAO_READ_ATTRIBUTES = 0x80,
		HAO_READ = 0x120089,
		HAO_WRITE = 0x120116,
		HAO_DELETE = 0x10000
	} HANDLE_ACCESS_OPTIONS;

	DEFINE_ENUM_FLAG_OPERATORS(HANDLE_ACCESS_OPTIONS);
	typedef /* [v1_enum] */
		enum HANDLE_SHARING_OPTIONS {
		HSO_SHARE_NONE = 0,
		HSO_SHARE_READ = 0x1,
		HSO_SHARE_WRITE = 0x2,
		HSO_SHARE_DELETE = 0x4
	} HANDLE_SHARING_OPTIONS;

	DEFINE_ENUM_FLAG_OPERATORS(HANDLE_SHARING_OPTIONS);
	typedef /* [v1_enum] */
		enum HANDLE_CREATION_OPTIONS {
		HCO_CREATE_NEW = 0x1,
		HCO_CREATE_ALWAYS = 0x2,
		HCO_OPEN_EXISTING = 0x3,
		HCO_OPEN_ALWAYS = 0x4,
		HCO_TRUNCATE_EXISTING = 0x5
	} HANDLE_CREATION_OPTIONS;

	EXTERN_C const IID IID_IOplockBreakingHandler;
	MIDL_INTERFACE("826ABE3D-3ACD-47D3-84F2-88AAEDCF6304")
		IOplockBreakingHandler : public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE OplockBreaking(void) = 0;
	};

	EXTERN_C const IID IID_IStorageItemHandleAccess;
	MIDL_INTERFACE("5CA296B2-2C25-4D22-B785-B885C8201E6A")
		IStorageItemHandleAccess : public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE Create(
			/* [in] */ HANDLE_ACCESS_OPTIONS accessOptions,
			/* [in] */ HANDLE_SHARING_OPTIONS sharingOptions,
			/* [in] */ HANDLE_OPTIONS options,
			/* [optional][in] */ __RPC__in_opt IOplockBreakingHandler * oplockBreakingHandler,
			/* [system_handle][retval][out] */ __RPC__deref_out_opt HANDLE * interopHandle) = 0;
	};

	EXTERN_C const IID IID_IStorageFolderHandleAccess;
	MIDL_INTERFACE("DF19938F-5462-48A0-BE65-D2A3271A08D6")
		IStorageFolderHandleAccess : public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE Create(
			/* [string][in] */ __RPC__in_string LPCWSTR fileName,
			/* [in] */ HANDLE_CREATION_OPTIONS creationOptions,
			/* [in] */ HANDLE_ACCESS_OPTIONS accessOptions,
			/* [in] */ HANDLE_SHARING_OPTIONS sharingOptions,
			/* [in] */ HANDLE_OPTIONS options,
			/* [optional][in] */ __RPC__in_opt IOplockBreakingHandler * oplockBreakingHandler,
			/* [system_handle][retval][out] */ __RPC__deref_out_opt HANDLE * interopHandle) = 0;
	};
#ifdef __cplusplus
}
#endif

#include <wrl\client.h>
#include <wrl\wrappers\corewrappers.h>
#include <ppl.h>
#include <ppltasks.h>

#include "CxHelper.h"

using namespace std;
using namespace Platform;
using namespace Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace Concurrency;
using namespace sunjwbase;

#define WINBOOL_2_CBOOL(winbool_var) bool((winbool_var) == TRUE)

struct CreateFileFlag
{
	DWORD dwDesiredAccess;
	DWORD dwShareMode;
	DWORD dwCreationDisposition;
	DWORD dwFlagsAndAttributes;
};

struct UwpCreateFlag
{
	HANDLE_ACCESS_OPTIONS accessOptions;
	HANDLE_SHARING_OPTIONS sharingOptions;
	HANDLE_OPTIONS handleOptions;
};

struct CombinedCreateFlag
{
	CreateFileFlag createFileFlag;
	UwpCreateFlag uwpCreateFlag;
};

static DWORD Win32ErrCodeFromHResult(HRESULT hr)
{
	if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0))
	{
		return HRESULT_CODE(hr);
	}

	if (hr == S_OK)
	{
		return ERROR_SUCCESS;
	}

	// Not a Win32 HRESULT so return a generic error code.
	return ERROR_CAN_NOT_COMPLETE;
}

static tstring LongPathFix(const tstring& tstrPath)
{
	tstring tstrFixPath;
	size_t pathLen = tstrPath.size();
	if (pathLen < MAX_PATH - 12)
		return tstrPath;

	if (tstrPath[0] == TEXT('\\') && tstrPath[1] == TEXT('\\'))
	{
		if (tstrPath[2] == TEXT('?')) // Already formatted
			return tstrPath;
		tstrFixPath = TEXT("\\\\?\\UNC\\");
		tstrFixPath += (tstrPath.c_str() + 2);
	}
	else
	{
		tstrFixPath = TEXT("\\\\?\\");
		tstrFixPath += tstrPath;
	}
	return tstrFixPath;
}

static DWORD GetHandleByCreateFileFromApp(const tstring& tstrPath, const CreateFileFlag *pUwpCreateFlag, LPHANDLE pFileHandle)
{
	DWORD dwRet = ERROR_SUCCESS;
	*pFileHandle = CreateFileFromAppW(tstrPath.c_str(), // file to open
		pUwpCreateFlag->dwDesiredAccess, // open for reading
		pUwpCreateFlag->dwShareMode, // share for reading
		NULL, // default security
		pUwpCreateFlag->dwCreationDisposition, // existing file only
		pUwpCreateFlag->dwFlagsAndAttributes, // normal file
		NULL); // no attr. template

	if (*pFileHandle == INVALID_HANDLE_VALUE)
		dwRet = GetLastError();

	return dwRet;
}

static DWORD GetHandleByStorageFile(const tstring& tstrPath, const UwpCreateFlag *pUwpCreateFlag, LPHANDLE pFileHandle)
{
	StorageFile^ storageFile = nullptr;
	HRESULT hr = S_OK;

	try
	{
		auto storageFileAction = StorageFile::GetFileFromPathAsync(ConvertToPlatStr(tstrPath.c_str()));
		// Make a blocking call to get hold of the StorageFile
		create_task(storageFileAction).then([&storageFile](StorageFile^ file)
		{
			storageFile = file;
		}).wait();
	}
	catch (Exception^ ex)
	{
		hr = ex->HResult;
	}

	if (storageFile != nullptr)
	{
		// Retrieve the IStorageItemHandleAccess interface from the StorageFile
		ComPtr<IUnknown> unknown(reinterpret_cast<IUnknown*>(storageFile));
		ComPtr<IStorageItemHandleAccess> fileAccessor;
		HRESULT hr = unknown.As(&fileAccessor);
		if (SUCCEEDED(hr))
		{
			hr = fileAccessor->Create(pUwpCreateFlag->accessOptions,
				pUwpCreateFlag->sharingOptions,
				pUwpCreateFlag->handleOptions,
				nullptr,
				pFileHandle);
		}
	}

	return Win32ErrCodeFromHResult(hr);
}

OsFile::OsFile(tstring filePath):
	_filePath(LongPathFix(filePath)),
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
	CombinedCreateFlag *fileFlag = (CombinedCreateFlag *)flag;
	TCHAR *pFileExc = (TCHAR *)exception;
	_osfileData = INVALID_HANDLE_VALUE;

	DWORD dwResult = GetHandleByStorageFile(_filePath, &(fileFlag->uwpCreateFlag), &_osfileData);
	if (_osfileData == INVALID_HANDLE_VALUE)
	{
		// Fallback to use CreateFileFromApp
		dwResult = GetHandleByCreateFileFromApp(_filePath, &(fileFlag->createFileFlag), &_osfileData);
	}

	if (_osfileData == INVALID_HANDLE_VALUE)
	{
		if (pFileExc != NULL)
		{
			if (dwResult == ERROR_SUCCESS)
				dwResult = ERROR_ACCESS_DENIED; // Why?

			LPVOID lpMsgBuf = NULL;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dwResult,
				0, //MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH),
				(LPTSTR)&lpMsgBuf,
				0, NULL);

			tstring tstrErrMsg(TEXT("Cannot open this file."));
			if (lpMsgBuf != NULL)
			{
				tstrErrMsg = (LPCTSTR)lpMsgBuf;
				LocalFree(lpMsgBuf);
			}
			tstrErrMsg = strtrim(tstrErrMsg);

#if defined(UNICODE) || defined(_UNICODE)
			wcscpy_s(pFileExc, OsFile::ERR_MSG_BUFFER_LEN, tstrErrMsg.c_str());
#else
			strcpy_s(pFileExc, OsFile::ERR_MSG_BUFFER_LEN, tstrErrMsg.c_str());
#endif
		}
	}

	return (_osfileData != INVALID_HANDLE_VALUE);
}

bool OsFile::openRead(void *exception/* = NULL*/)
{
	bool ret = false;

	CombinedCreateFlag fileFlag;
	fileFlag.createFileFlag.dwDesiredAccess = GENERIC_READ;
	fileFlag.createFileFlag.dwShareMode = FILE_SHARE_READ;
	fileFlag.createFileFlag.dwCreationDisposition = OPEN_EXISTING;
	fileFlag.createFileFlag.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	fileFlag.uwpCreateFlag.accessOptions = HANDLE_ACCESS_OPTIONS::HAO_READ | HANDLE_ACCESS_OPTIONS::HAO_READ_ATTRIBUTES;
	fileFlag.uwpCreateFlag.sharingOptions = HANDLE_SHARING_OPTIONS::HSO_SHARE_READ;
	fileFlag.uwpCreateFlag.handleOptions = HO_RANDOM_ACCESS;
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

	CombinedCreateFlag fileFlag;
	fileFlag.createFileFlag.dwDesiredAccess = GENERIC_READ;
	fileFlag.createFileFlag.dwShareMode = FILE_SHARE_READ;
	fileFlag.createFileFlag.dwCreationDisposition = OPEN_EXISTING;
	fileFlag.createFileFlag.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN;
	fileFlag.uwpCreateFlag.accessOptions = HANDLE_ACCESS_OPTIONS::HAO_READ | HANDLE_ACCESS_OPTIONS::HAO_READ_ATTRIBUTES;
	fileFlag.uwpCreateFlag.sharingOptions = HANDLE_SHARING_OPTIONS::HSO_SHARE_READ;
	fileFlag.uwpCreateFlag.handleOptions = HO_SEQUENTIAL_SCAN;
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

	CombinedCreateFlag fileFlag;
	fileFlag.createFileFlag.dwDesiredAccess = GENERIC_WRITE;
	fileFlag.createFileFlag.dwShareMode = 0;
	fileFlag.createFileFlag.dwCreationDisposition = CREATE_NEW;
	fileFlag.createFileFlag.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	fileFlag.uwpCreateFlag.accessOptions = HANDLE_ACCESS_OPTIONS::HAO_WRITE;
	fileFlag.uwpCreateFlag.sharingOptions = HANDLE_SHARING_OPTIONS::HSO_SHARE_NONE;
	fileFlag.uwpCreateFlag.handleOptions = HO_RANDOM_ACCESS;
	ret = this->open((void *)&fileFlag, exception);

	if (ret == true)
	{
		_fileStatus = OPEN_WRITE;
	}

	return ret;
}

bool OsFile::openReadWrite(void *exception/* = NULL*/)
{
	bool ret = false;

	CombinedCreateFlag fileFlag;
	fileFlag.createFileFlag.dwDesiredAccess = GENERIC_WRITE | GENERIC_READ;
	fileFlag.createFileFlag.dwShareMode = 0;
	fileFlag.createFileFlag.dwCreationDisposition = CREATE_NEW;
	fileFlag.createFileFlag.dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	fileFlag.uwpCreateFlag.accessOptions = HANDLE_ACCESS_OPTIONS::HAO_WRITE | HANDLE_ACCESS_OPTIONS::HAO_READ | HANDLE_ACCESS_OPTIONS::HAO_READ_ATTRIBUTES;
	fileFlag.uwpCreateFlag.sharingOptions = HANDLE_SHARING_OPTIONS::HSO_SHARE_NONE;
	fileFlag.uwpCreateFlag.handleOptions = HO_RANDOM_ACCESS;
	ret = this->open((void *)&fileFlag, exception);

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
