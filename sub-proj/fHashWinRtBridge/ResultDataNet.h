#pragma once

namespace FilesHashUwp
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
		Platform::String^ Path; // Path
		uint64 Size; // Size
		Platform::String^ ModifiedDate; // Modified date
		Platform::String^ Version; // Version
		Platform::String^ MD5; // MD5
		Platform::String^ SHA1; // SHA1
		Platform::String^ SHA256; // SHA256
		Platform::String^ SHA512; // SHA512
		Platform::String^ Error; // Error string
	};
}
