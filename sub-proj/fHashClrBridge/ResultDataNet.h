#pragma once

namespace FilesHashWUI
{
	public enum class ResultStateNet
	{
		ResultNone = 0,
		ResultPath,
		ResultMeta,
		ResultAll,
		ResultError
	};

	public value struct ResultDataNet
	{
	public:
		ResultStateNet EnumState; // State
		System::String^ Path; // Path
		System::UInt64 Size; // Size
		System::String^ ModifiedDate; // Modified date
		System::String^ Version; // Version
		System::String^ MD5; // MD5
		System::String^ SHA1; // SHA1
		System::String^ SHA256; // SHA256
		System::String^ SHA512; // SHA512
		System::String^ Error; // Error string
	};
}
