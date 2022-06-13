#pragma once

namespace FilesHashUwp
{
	public delegate void OnHelloHandler(Platform::String^ strHello);

	public ref class UIBridgeDelegate sealed
	{
	public:
		UIBridgeDelegate();

		event OnHelloHandler^ OnHelloHandler;
	};
}
