#pragma once
#include <memory>
#include <Windows.h>
#include "TestDelegate.h"

class SomeNative
{
public:
	SomeNative();
	SomeNative(Platform::WeakReference wrSomeDelegate);
	virtual ~SomeNative();

	void getHello();
	void doGetHello();

private:
	Platform::WeakReference m_wrTestDelegate;
	HANDLE m_thrdHandle;
};
