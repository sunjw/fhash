#pragma once
#include <memory>
#include "TestDelegate.h"
#include "SomeNative.h"

#include "Common/Global.h"

namespace FilesHashUwp
{
    public ref class TestNativeWrapper sealed
    {
    public:
        TestNativeWrapper(TestDelegate^ testDelegate);

        void GetHello();

    private:
        TestDelegate^ m_testDelegate;
        std::shared_ptr<SomeNative> m_spSomeNative;
        SomeNative m_someNative2;
        ThreadData m_threadData;
    };
}
