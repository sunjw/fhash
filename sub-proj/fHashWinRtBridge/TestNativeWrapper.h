#pragma once
#include <memory>
#include "TestDelegate.h"
#include "SomeNative.h"

namespace fHashUwp
{
    public ref class TestNativeWrapper sealed
    {
    public:
        TestNativeWrapper(TestDelegate^ testDelegate);

        void GetHello();

    private:
        TestDelegate^ m_testDelegate;
        std::shared_ptr<SomeNative> m_spSomeNative;
        SomeNative m_spSomeNative2;
    };
}
