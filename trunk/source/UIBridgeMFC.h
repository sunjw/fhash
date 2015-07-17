#ifndef _UI_BRIDGE_MFC_
#define _UI_BRIDGE_MFC_

#include "UIBridgeBase.h"

#include <Windows.h>

#include "strhelper.h"
#include "OsUtils/OsThread.h"
#include "Global.h"

class UIBridgeMFC: public UIBridgeBase
{
public:
	UIBridgeMFC(HWND hWnd, 
				sunjwbase::tstring *tstrUIAll,
				sunjwbase::OsMutex *mainMtx);
	virtual ~UIBridgeMFC();

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
	HWND m_hWnd;
	sunjwbase::tstring *m_uiTstrAll;
	sunjwbase::OsMutex *m_mainMtx;

	sunjwbase::tstring m_tstrNoPreparing;
};

#endif
