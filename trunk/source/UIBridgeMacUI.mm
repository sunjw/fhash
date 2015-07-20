#import "UIBridgeMacUI.h"

#include <stdlib.h>
#include <string>
#include <dispatch/dispatch.h>

#include "strhelper.h"
#include "Global.h"
#include "Utils.h"
#include "MacUtils.h"

#import "MainViewController.h"

using namespace std;
using namespace sunjwbase;

UIBridgeMacUI::UIBridgeMacUI(MainViewController *mainViewController)
:_mainViewControllerPtr(mainViewController),
_oldProgWholeValue(0)
{
}

UIBridgeMacUI::~UIBridgeMacUI()
{
}

void UIBridgeMacUI::lockData()
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    mainViewController.mainMtx->lock();
}

void UIBridgeMacUI::unlockData()
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    mainViewController.mainMtx->unlock();
}

void UIBridgeMacUI::preparingCalc()
{
    
}

void UIBridgeMacUI::removePreparingCalc()
{
}

void UIBridgeMacUI::calcStop()
{
}

void UIBridgeMacUI::calcFinish()
{
}

void UIBridgeMacUI::showFileName(const ResultData& result)
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();

    lockData();
    {
        MacUtils::AppendFileNameToNSMutableString(result, mainViewController.mainText);
    }
    unlockData();
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [mainViewController updateMainTextView];
    });
}

void UIBridgeMacUI::showFileMeta(const ResultData& result)
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    
    lockData();
    {
        MacUtils::AppendFileMetaToNSMutableString(result, mainViewController.mainText);
    }
    unlockData();
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [mainViewController updateMainTextView];
    });
}

void UIBridgeMacUI::showFileHash(const ResultData& result, bool uppercase)
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    
    lockData();
    {
        MacUtils::AppendFileHashToNSMutableString(result,
                                                  uppercase,
                                                  mainViewController.mainText);
    }
    unlockData();
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [mainViewController updateMainTextView];
    });
}

void UIBridgeMacUI::showFileErr(const ResultData& result)
{
    
}

int UIBridgeMacUI::getProgMax()
{
    return 100;
}

void UIBridgeMacUI::updateProg(int value)
{
    
}

void UIBridgeMacUI::updateProgWhole(int value)
{
    if (value == _oldProgWholeValue)
        return;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        MainViewController *mainViewController = _mainViewControllerPtr.get();
        [mainViewController.mainProgressIndicator setDoubleValue:(double)value];
        _oldProgWholeValue = value;
    });
}

void UIBridgeMacUI::fileCalcFinish()
{
    
}

void UIBridgeMacUI::fileFinish()
{
    
}
