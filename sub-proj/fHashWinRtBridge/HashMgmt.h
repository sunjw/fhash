#pragma once

#include <memory>
#include "Common/Global.h"
#include "UIBridgeDelegate.h"
#include "UIBridgeUwp.h"

namespace FilesHashUwp
{
	public ref class HashMgmt sealed
	{
	public:
		HashMgmt(UIBridgeDelegate^ uiBridgeDelegate);

		void Init();
		void Clear();

	private:
		std::shared_ptr<UIBridgeUwp> m_spUiBridgeUwp;
		ThreadData m_threadData;
	};
}
