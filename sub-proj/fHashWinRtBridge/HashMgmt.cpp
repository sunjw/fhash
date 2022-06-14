#include "stdafx.h"
#include "HashMgmt.h"

using namespace std;
using namespace Platform;
using namespace FilesHashUwp;

HashMgmt::HashMgmt(UIBridgeDelegate^ uiBridgeDelegate)
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

void HashMgmt::SetStop(Platform::Boolean val)
{
	m_threadData.stop = val;
}
