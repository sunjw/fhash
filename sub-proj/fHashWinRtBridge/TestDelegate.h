#pragma once

namespace FilesHashUwp
{
	public delegate void OnHelloHandler(Platform::String^ strHello);

    public ref class TestDelegate sealed
    {
    public:
        TestDelegate();

        void OnGetHello(Platform::String^ strHello);

        event OnHelloHandler^ OnHelloHandler;
    };
}
