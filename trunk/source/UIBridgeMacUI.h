#ifndef _UI_BRIDGE_MAC_UI_
#define _UI_BRIDGE_MAC_UI_

#include "OsUtils/OsThread.h"
#include "UIBridgeBase.h"

#import "MacUtils.h"
#import "MainViewController.h"

class UIBridgeMacUI: public UIBridgeBase
{
public:
    UIBridgeMacUI(MainViewController *mainViewController);
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

    /**
     * Append file name in ResultData to NSMutableAttributedString.
     */
    static void AppendFileNameToNSMutableAttributedString(const ResultData& result,
                                                          NSMutableAttributedString *nsmutString);

    /**
     * Append file meta in ResultData to NSMutableAttributedString.
     */
    static void AppendFileMetaToNSMutableAttributedString(const ResultData& result,
                                                          NSMutableAttributedString *nsmutString);

    /**
     * Append file hash result in ResultData to NSMutableAttributedString.
     */
    static void AppendFileHashToNSMutableAttributedString(const ResultData& result,
                                                          bool uppercase,
                                                          NSMutableAttributedString *nsmutString);

    /**
     * Append file error message in ResultData to NSMutableAttributedString.
     */
    static void AppendFileErrToNSMutableAttributedString(const ResultData& result,
                                                         NSMutableAttributedString *nsmutString);

    /**
     * Append whole ResultData to NSMutableAttributedString.
     */
    static void AppendResultToNSMutableAttributedString(const ResultData& result,
                                                        bool uppercase,
                                                        NSMutableAttributedString *nsmutString);

private:
    MacUtils::ObjcWeakPtr<MainViewController> _mainViewControllerPtr;
    
    NSAttributedString *_nsstrNoPreparing;
};

#endif
