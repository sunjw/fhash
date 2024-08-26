#include "stdafx.h"

#include "HashMgmtClr.h"
#include "ClrHelper.h"
#include "Common/strhelper.h"
#include "Common/HashEngine.h"

using namespace std;
using namespace System;
using namespace FilesHashWUI;
using namespace sunjwbase;

HashMgmtClr::HashMgmtClr(UIBridgeDelegates^ uiBridgeDelegates)
	:m_pUiBridgeWUI(NULL), m_pThreadData(NULL), m_hWorkThread(NULL)
{
	m_pUiBridgeWUI = new UIBridgeWUI(uiBridgeDelegates);
	m_pThreadData = new ThreadData();
	m_pThreadData->uiBridge = m_pUiBridgeWUI;
}

HashMgmtClr::!HashMgmtClr()
{
	if (m_pUiBridgeWUI)
		delete m_pUiBridgeWUI;
	if (m_pThreadData)
		delete m_pThreadData;
	if (m_hWorkThread)
		CloseHandle(m_hWorkThread);
}

void HashMgmtClr::Init()
{
}

void HashMgmtClr::Clear()
{
	m_pThreadData->threadWorking = false;
	m_pThreadData->stop = false;

	m_pThreadData->uppercase = false;
	m_pThreadData->totalSize = 0;

	m_pThreadData->nFiles = 0;
	m_pThreadData->fullPaths.clear();

	m_pThreadData->resultList.clear();
}

void HashMgmtClr::SetStop(bool val)
{
	m_pThreadData->stop = val;
}

void HashMgmtClr::SetUppercase(bool val)
{
	m_pThreadData->uppercase = val;
}

UInt64 HashMgmtClr::GetTotalSize()
{
	return m_pThreadData->totalSize;
}

void HashMgmtClr::AddFiles(cli::array<String^>^ filePaths)
{
	m_pThreadData->fullPaths.clear();
	m_pThreadData->nFiles = filePaths->Length;
	for each (String^ sstrFile in filePaths)
	{
		tstring tstrFile(ConvertSystemStringToTstr(sstrFile));
		m_pThreadData->fullPaths.push_back(tstrFile);
	}
}

void HashMgmtClr::StartHashThread()
{
	if (m_hWorkThread)
	{
		CloseHandle(m_hWorkThread);
		m_hWorkThread = NULL;
	}
	DWORD thredID;
	m_hWorkThread = (HANDLE)_beginthreadex(NULL,
										0,
										(unsigned int (WINAPI*)(void*))HashThreadFunc,
										m_pThreadData,
										0,
										(unsigned int*)&thredID);
}

cli::array<ResultDataNet>^ HashMgmtClr::FindResult(String^ sstrHashToFind)
{
	tstring tstrHashToFind(ConvertSystemStringToTstr(sstrHashToFind));
	tstrHashToFind = strtotstr(str_upper(tstrtostr(tstrHashToFind))); // Upper
	tstrHashToFind = strtrim(tstrHashToFind);

	ResultList findResultList;
	ResultList::iterator itr;
	if (tstrHashToFind.size() > 0)
	{
		itr = m_pThreadData->resultList.begin();
		for (; itr != m_pThreadData->resultList.end(); ++itr)
		{
			if (itr->tstrMD5.find(tstrHashToFind) != tstring::npos ||
				itr->tstrSHA1.find(tstrHashToFind) != tstring::npos ||
				itr->tstrSHA256.find(tstrHashToFind) != tstring::npos ||
				itr->tstrSHA512.find(tstrHashToFind) != tstring::npos)
			{
				findResultList.push_back(*itr);
			}
		}
	}

	cli::array<ResultDataNet>^ findResultNetArray =
		gcnew cli::array<ResultDataNet>(findResultList.size());

	size_t i = 0;
	itr = findResultList.begin();
	for (; itr != findResultList.end(); ++itr)
	{
		ResultDataNet resultDataNet = ConvertResultDataToNet(*itr);
		findResultNetArray[i] = resultDataNet;
		i++;
	}

	return findResultNetArray;
}
