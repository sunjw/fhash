//
//  UIBridgeMacSwift.h
//  fHashMacUI
//
//  Created by Sun Junwen on 2023/12/7.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

#ifndef _UI_BRIDGE_MAC_SWIFT_
#define _UI_BRIDGE_MAC_SWIFT_

#include "OsUtils/OsThread.h"
#include "Common/UIBridgeBase.h"

#import "MacUtils.h"

@class ResultDataSwift;
@class MainViewControllerX;

class UIBridgeMacSwift: public UIBridgeBase
{
public:
    UIBridgeMacSwift(MainViewControllerX *mainViewController);
    virtual ~UIBridgeMacSwift();

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
     * Convert ResultData to ResultDataSwift
     */
    static ResultDataSwift *ConvertResultDataToSwift(const ResultData& result);

private:
    MacUtils::ObjcWeakPtr<MainViewControllerX> _mainViewControllerPtr;
};

#endif

