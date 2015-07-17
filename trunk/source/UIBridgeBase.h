#ifndef _UI_BRIDGE_BASE_H_
#define _UI_BRIDGE_BASE_H_

#include "Global.h"

class UIBridgeBase
{
public:
	UIBridgeBase() {}
	virtual ~UIBridgeBase() {}

	virtual void lockData() = 0;
	virtual void unlockData() = 0;

	virtual void preparingCalc() = 0;
	virtual void removePreparingCalc() = 0;
	virtual void calcStop() = 0;
	virtual void calcFinish() = 0;

	virtual void showFileName(const ResultData& result) = 0;
	virtual void showFileMeta(const ResultData& result) = 0;
	virtual void showFileHash(const ResultData& result, bool uppercase) = 0;
	virtual void showFileErr(const ResultData& result) = 0;

	virtual int getProgMax() = 0;
	virtual void updateProg(int value) = 0;
	virtual void updateProgWhole(int value) = 0;

	virtual void fileCalcFinish() = 0;
	virtual void fileFinish() = 0;

};

#endif
