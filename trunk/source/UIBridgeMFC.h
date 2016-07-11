#ifndef _UI_BRIDGE_MFC_
#define _UI_BRIDGE_MFC_

#include "UIBridgeBase.h"

#include <Windows.h>

#include "strhelper.h"
#include "OsUtils/OsThread.h"
#include "Global.h"
#include "HyperEditHash.h"

class UIBridgeMFC: public UIBridgeBase
{
public:
	UIBridgeMFC(HWND hWnd,
				sunjwbase::OsMutex *mainMtx,
				CHyperEditHash *hyperEdit);
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

	static void AppendFileNameToHyperEdit(const ResultData& result,
											CHyperEditHash *hyerEdit);
	static void AppendFileMetaToHyperEdit(const ResultData& result,
											CHyperEditHash *hyerEdit);
	static void AppendFileHashToHyperEdit(const ResultData& result,
											bool uppercase,
											CHyperEditHash *hyerEdit);
	static void AppendFileErrToHyperEdit(const ResultData& result,
											CHyperEditHash *hyerEdit);
	static void AppendResultToHyperEdit(const ResultData& result,
										bool uppercase,
										CHyperEditHash *hyerEdit);

private:
	HWND m_hWnd;
	sunjwbase::OsMutex *m_mainMtx;
	CHyperEditHash *m_mainHyperEdit;

	OFFSETS m_offsetsNoPreparing;
	sunjwbase::tstring m_tstrNoPreparing;
};

#endif
