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
