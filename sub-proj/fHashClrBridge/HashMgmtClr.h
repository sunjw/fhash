#pragma once

#include "Common/Global.h"
#include "UIBridgeDelegates.h"
#include "UIBridgeWUI.h"

namespace FilesHashWUI
{
	public ref class HashMgmtClr sealed
	{
	public:
		HashMgmtClr(UIBridgeDelegates^ uiBridgeDelegates);

		virtual ~HashMgmtClr()
		{
			// clean up code to release managed resource
			// ...
			// to avoid code duplication,
			// call finalizer to release unmanaged resources
			this->!HashMgmtClr();
		}

		// finalizer cleans up unmanaged resources
		// destructor or garbage collector will
		// clean up managed resources
		!HashMgmtClr();

		void Init();
		void Clear();

		void SetStop(bool val);
		void SetUppercase(bool val);
		System::UInt64 GetTotalSize();

		void AddFiles(cli::array<System::String^>^ filePaths);
		void StartHashThread();

		cli::array<ResultDataNet>^ FindResult(System::String^ sstrHashToFind);
		System::UInt64 GetResultCount();

	private:
		UIBridgeWUI *m_pUiBridgeWUI;
		ThreadData *m_pThreadData;
		HANDLE m_hWorkThread;
	};
}
