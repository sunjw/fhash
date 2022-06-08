#include "stdafx.h"
#include <memory>
#include "TestNativeWrapper.h"
#include "Common/strhelper.h"
#include "TestDelegate.h"

using namespace Platform;
using namespace sunjwbase;
using namespace fHashUwp;

TestNativeWrapper::TestNativeWrapper(TestDelegate^ testDelegate)
{
	m_testDelegate = testDelegate;
	WeakReference wrTestDelegate(m_testDelegate);
	m_spSomeNative.reset(new SomeNative(wrTestDelegate));
}

void TestNativeWrapper::GetHello()
{
	m_spSomeNative->getHello();
}
