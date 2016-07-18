#import "UIBridgeMacUI.h"

#include <stdlib.h>
#include <string>
#include <dispatch/dispatch.h>

#include "strhelper.h"
#include "Utils.h"
#include "MacUtils.h"
#include "Global.h"

#import "ESSProgressIndicator.h"
#import "MainViewController.h"

using namespace std;
using namespace sunjwbase;

UIBridgeMacUI::UIBridgeMacUI(MainViewController *mainViewController)
:_mainViewControllerPtr(mainViewController)
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
        _nsstrNoPreparing = [[NSMutableAttributedString alloc] initWithAttributedString:mainViewController.mainText];
        
        string strAppend = GetStringFromResByKey(MAINDLG_WAITING_START);
        strAppend.append("\n");
        NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
        MacUtils::AppendNSStringToNSMutableAttributedString(mainViewController.mainText, nsstrAppend);
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
        mainViewController.mainText = [[NSMutableAttributedString alloc] initWithAttributedString:_nsstrNoPreparing];
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
        MacUtils::AppendFileNameToNSMutableAttributedString(result, mainViewController.mainText);
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
        MacUtils::AppendFileMetaToNSMutableAttributedString(result, mainViewController.mainText);
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
        MacUtils::AppendFileHashToNSMutableAttributedString(result,
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
        MacUtils::AppendFileErrToNSMutableAttributedString(result, mainViewController.mainText);
    }
    unlockData();
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [mainViewController updateMainTextView];
    });
}

int UIBridgeMacUI::getProgMax()
{
    return 200;
}

void UIBridgeMacUI::updateProg(int value)
{
}

void UIBridgeMacUI::updateProgWhole(int value)
{
    MainViewController *mainViewController = _mainViewControllerPtr.get();
    int oldValue = (int)[mainViewController.mainProgressIndicator doubleValue];
    
    if (value == oldValue)
        return;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [mainViewController.mainProgressIndicator animateToDoubleValue:(double)value];
    });
}

void UIBridgeMacUI::fileCalcFinish()
{
}

void UIBridgeMacUI::fileFinish()
{
}
