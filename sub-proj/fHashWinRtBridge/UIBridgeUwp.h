#pragma once

#include "Common/UIBridgeBase.h"
#include "Common/Global.h"

#include "UIBridgeDelegate.h"
#include "ResultDataNet.h"

namespace FilesHashUwp
{
	class UIBridgeUwp : public UIBridgeBase
	{
	public:
		UIBridgeUwp(UIBridgeDelegate^ uiBridgeDelegate);
		virtual ~UIBridgeUwp();

		virtual void lockData();
		virtual void unlockData();

		virtual void preparingCalc();
		virtual void removePreparingCalc();
		virtual void calcStop();
		virtual void calcFinish();

		virtual void showFileName(const ResultData& result);
		virtual void showFileMeta(const ResultData& result);
		virtual void showFileHash(const ResultData& result, bool uppercase);
		virtual void showFileErr(const ResultData& result);

		virtual int getProgMax();
		virtual void updateProg(int value);
		virtual void updateProgWhole(int value);

		virtual void fileCalcFinish();
		virtual void fileFinish();

		static ResultDataNet ConvertResultDataToNet(const ResultData& result);

	private:
		UIBridgeDelegate^ m_uiBridgeDelegate;
	};
}
