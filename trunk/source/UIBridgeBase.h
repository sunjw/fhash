#ifndef _UI_BRIDGE_BASE_H_
#define _UI_BRIDGE_BASE_H_

#include "strhelper.h"

class UIBridgeBase
{
public:
	UIBridgeBase() {}
	virtual ~UIBridgeBase() {}

	virtual void preparingCalc() = 0;
	virtual void removePreparingCalc() = 0;
	virtual void calcStop() = 0;
	virtual void calcFinish() = 0;

	virtual void showFileName(const sunjwbase::tstring& tstrFileName) = 0;
	virtual void showFileMeta(const sunjwbase::tstring& tstrFileSize,
		const sunjwbase::tstring& tstrShortSize,
		const sunjwbase::tstring& tstrLastModifiedTime,
		const sunjwbase::tstring& tstrFileVersion) = 0;
	virtual void showFileHash(const sunjwbase::tstring& tstrFileMD5,
		const sunjwbase::tstring& tstrFileSHA1,
		const sunjwbase::tstring& tstrFileSHA256,
		const sunjwbase::tstring& tstrFileCRC32) = 0;
	virtual void showFileErr(const sunjwbase::tstring& tstrErr) = 0;

	virtual int getProgMax() = 0;
	virtual void updateProg(int value) = 0;
	virtual void updateProgWhole(int value) = 0;

	virtual void fileCalcFinish() = 0;
	virtual void fileFinish() = 0;

};

#endif
