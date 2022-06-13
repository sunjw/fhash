#include "stdafx.h"
#include "HashMgmt.h"

using namespace std;
using namespace Platform;
using namespace FilesHashUwp;

HashMgmt::HashMgmt(UIBridgeDelegate^ uiBridgeDelegate)
{
	m_uiBridgeUwp = make_shared<UIBridgeUwp>(uiBridgeDelegate);
}
