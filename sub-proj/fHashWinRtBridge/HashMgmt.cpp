#include "stdafx.h"

#include "HashMgmt.h"
#include "CxHelper.h"
#include "Common/strhelper.h"
#include "Common/HashEngine.h"

using namespace std;
using namespace Platform;
using namespace FilesHashUwp;
using namespace sunjwbase;

HashMgmt::HashMgmt(UIBridgeDelegate^ uiBridgeDelegate)
	:m_hWorkThread(NULL)
{
	m_spUiBridgeUwp = make_shared<UIBridgeUwp>(uiBridgeDelegate);
}

void HashMgmt::Init()
{
	m_threadData.uiBridge = m_spUiBridgeUwp.get();
}

void HashMgmt::Clear()
{
	m_threadData.threadWorking = false;
	m_threadData.stop = false;

	m_threadData.uppercase = false;
	m_threadData.totalSize = 0;

	m_threadData.nFiles = 0;
	m_threadData.fullPaths.clear();

	m_threadData.resultList.clear();
}

void HashMgmt::SetStop(Boolean val)
{
	m_threadData.stop = val;
}

void HashMgmt::SetUppercase(Boolean val)
{
	m_threadData.uppercase = val;
}

void HashMgmt::AddFiles(const Array<String^>^ filePaths)
{
	m_threadData.fullPaths.clear();
	m_threadData.nFiles = filePaths->Length;
	for (String^ pstrFile : filePaths)
	{
		tstring tstrFile(pstrFile->Data());
		m_threadData.fullPaths.push_back(tstrFile);
	}
}

void HashMgmt::StartHashThread()
{
	if (m_hWorkThread)
	{
		CloseHandle(m_hWorkThread);
	}
	DWORD thredID;
	m_hWorkThread = (HANDLE)_beginthreadex(NULL,
										0,
										(unsigned int (WINAPI*)(void*))HashThreadFunc,
										&m_threadData,
										0,
										(unsigned int*)&thredID);
}
