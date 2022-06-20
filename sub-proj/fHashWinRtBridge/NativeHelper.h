#pragma once

namespace FilesHashUwp
{
	public ref class NativeHelper sealed
	{
	public:
		NativeHelper();

		Platform::String^ GetWindowsInfo();
	};
}
