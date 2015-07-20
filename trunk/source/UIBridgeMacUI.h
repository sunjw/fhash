#ifndef _UI_BRIDGE_MAC_UI_
#define _UI_BRIDGE_MAC_UI_

#include "UIBridgeBase.h"

class UIBridgeMacUI: public UIBridgeBase
{
public:
    UIBridgeMacUI();
	virtual ~UIBridgeMacUI();

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

private:
    
};

#endif
