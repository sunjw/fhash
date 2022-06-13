#include "stdafx.h"

#include <memory>
#include "TestNativeWrapper.h"
#include "TestDelegate.h"
#include "Common/strhelper.h"
#include "Common/HashEngine.h"

using namespace Platform;
using namespace sunjwbase;
using namespace FilesHashUwp;

TestNativeWrapper::TestNativeWrapper(TestDelegate^ testDelegate)
{
	m_testDelegate = testDelegate;
	WeakReference wrTestDelegate(m_testDelegate);
	m_spSomeNative.reset(new SomeNative(wrTestDelegate));
}

void TestNativeWrapper::GetHello()
{
	void* pHashThreadFunc = HashThreadFunc;
	m_spSomeNative->getHello();
}
