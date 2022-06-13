#include "stdafx.h"
#include "SomeNative.h"
#include "TestDelegate.h"
#include "Common/strhelper.h"
#include "WinCommon/WindowsComm.h"

using namespace Platform;
using namespace sunjwbase;
using namespace FilesHashUwp;

int WINAPI TestThreadFunc(void* param);

SomeNative::SomeNative():
	SomeNative(WeakReference(nullptr))
{

}

SomeNative::SomeNative(WeakReference wrSomeDelegate):
	m_wrTestDelegate(wrSomeDelegate), m_thrdHandle(NULL)
{

}

SomeNative::~SomeNative()
{
	if (m_thrdHandle)
	{
		CloseHandle(m_thrdHandle);
	}
}

void SomeNative::getHello()
{
	DWORD thredID;
	m_thrdHandle = (HANDLE)_beginthreadex(NULL,
		0,
		(unsigned int (WINAPI*)(void*))TestThreadFunc,
		this,
		0,
		(unsigned int*)&thredID);
}

void SomeNative::doGetHello()
{
	TestDelegate^ testDelegate = m_wrTestDelegate.Resolve<TestDelegate>();
	if (testDelegate == nullptr)
	{
		return;
	}

	tstring tstrWinInfo = WindowsComm::GetWindowsInfo();
	tstring tstrHello(L"将文件拖入或点击打开，开始计算。");
	tstrHello.append(tstrWinInfo);
	String^ strHello = ref new String(tstrHello.c_str());
	testDelegate->OnGetHello(strHello);
}

int WINAPI TestThreadFunc(void* param)
{
	SomeNative* ptrSomeNative = (SomeNative*)param;
	SleepEx(5000, TRUE);
	ptrSomeNative->doGetHello();
	return 0;
}
