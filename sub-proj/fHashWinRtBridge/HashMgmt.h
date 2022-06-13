#pragma once

#include <memory>
#include "UIBridgeDelegate.h"
#include "UIBridgeUwp.h"

namespace FilesHashUwp
{
	public ref class HashMgmt sealed
	{
	public:
		HashMgmt(UIBridgeDelegate^ uiBridgeDelegate);

	private:
		std::shared_ptr<UIBridgeUwp> m_uiBridgeUwp;
	};
}
