#import "UIBridgeMacUI.h"

#include <stdlib.h>
#include <string>
#include <dispatch/dispatch.h>

#include "strhelper.h"
#include "Utils.h"
#include "MacUtils.h"
#include "Global.h"
#include "UIStrings.h"

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
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    
    lockData();
    {
        // Copy old string.
        _nsstrNoPreparing = [NSString stringWithString:mainViewController.mainText];
        
        string strAppend = MacUtils::GetStringFromRes(MAINDLG_WAITING_START);
        strAppend.append("\n");
        NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
        [mainViewController.mainText appendString:nsstrAppend];
    }
    unlockData();
    
    dispatch_async(dispatch_get_main_queue(), ^{
       [mainViewController updateMainTextView];
    });
}

void UIBridgeMacUI::removePreparingCalc()
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    
    lockData();
    {
        // Reset old string.
        [mainViewController.mainText setString:_nsstrNoPreparing];
    }
    unlockData();
}

void UIBridgeMacUI::calcStop()
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [mainViewController calculateStopped];
    });
}

void UIBridgeMacUI::calcFinish()
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [mainViewController calculateFinished];
    });
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
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    
    lockData();
    {
        MacUtils::AppendFileErrToNSMutableString(result, mainViewController.mainText);
    }
    unlockData();
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [mainViewController updateMainTextView];
    });
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
