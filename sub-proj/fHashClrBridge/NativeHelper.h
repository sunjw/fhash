#pragma once

namespace SunJWBase
{
	public ref class NativeHelper sealed
	{
	public:
		NativeHelper();

		System::String^ GetTargetArch();
		System::String^ GetWindowsInfo();

		System::Boolean SetClipboardText(System::String^ sstrCopyText);
	};
}
