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
        UIBridgeMacUI::AppendFileNameToNSMutableAttributedString(result, mainViewController.mainText);
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
        UIBridgeMacUI::AppendFileMetaToNSMutableAttributedString(result, mainViewController.mainText);
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
        UIBridgeMacUI::AppendFileHashToNSMutableAttributedString(result,
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
        UIBridgeMacUI::AppendFileErrToNSMutableAttributedString(result, mainViewController.mainText);
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

void UIBridgeMacUI::AppendFileNameToNSMutableAttributedString(const ResultData& result,
                                                              NSMutableAttributedString *nsmutString)
{
    string strAppend = GetStringFromResByKey(FILENAME_STRING);
    strAppend.append(" ");
    strAppend.append(tstrtostr(result.tstrPath));
    strAppend.append("\n");

    NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutString, nsstrAppend);
}

void UIBridgeMacUI::AppendFileMetaToNSMutableAttributedString(const ResultData& result,
                                                              NSMutableAttributedString *nsmutString)
{
    char chSizeBuff[1024] = {0};
    sprintf(chSizeBuff, "%llu", result.ulSize);
    string strShortSize = Utils::ConvertSizeToShortSizeStr(result.ulSize);

    string strAppend = GetStringFromResByKey(FILESIZE_STRING);
    strAppend.append(" ");
    strAppend.append(chSizeBuff);
    strAppend.append(" ");
    strAppend.append(GetStringFromResByKey(BYTE_STRING));
    if (strShortSize != "") {
        strAppend.append(" (");
        strAppend.append(strShortSize);
        strAppend.append(")");
    }
    strAppend.append("\n");
    strAppend.append(GetStringFromResByKey(MODIFYTIME_STRING));
    strAppend.append(" ");
    strAppend.append(tstrtostr(result.tstrMDate));
    strAppend.append("\n");

    NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutString, nsstrAppend);
}

void UIBridgeMacUI::AppendFileHashToNSMutableAttributedString(const ResultData& result,
                                                              bool uppercase,
                                                              NSMutableAttributedString *nsmutString)
{
    string strFileMD5, strFileSHA1, strFileSHA256, strFileCRC32;

    if (uppercase) {
        strFileMD5 = str_upper(tstrtostr(result.tstrMD5));
        strFileSHA1 = str_upper(tstrtostr(result.tstrSHA1));
        strFileSHA256 = str_upper(tstrtostr(result.tstrSHA256));
        strFileCRC32 = str_upper(tstrtostr(result.tstrCRC32));
    } else {
        strFileMD5 = str_lower(tstrtostr(result.tstrMD5));
        strFileSHA1 = str_lower(tstrtostr(result.tstrSHA1));
        strFileSHA256 = str_lower(tstrtostr(result.tstrSHA256));
        strFileCRC32 = str_lower(tstrtostr(result.tstrCRC32));
    }

    NSString *nsstrFileMD5 = MacUtils::ConvertUTF8StringToNSString(strFileMD5);
    NSString *nsstrFileSHA1 = MacUtils::ConvertUTF8StringToNSString(strFileSHA1);
    NSString *nsstrFileSHA256 = MacUtils::ConvertUTF8StringToNSString(strFileSHA256);
    NSString *nsstrFileCRC32 = MacUtils::ConvertUTF8StringToNSString(strFileCRC32);

    NSMutableAttributedString *nsmutStrHash = [[NSMutableAttributedString alloc] init];

    [nsmutStrHash beginEditing];

    NSUInteger oldLength;

    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, @"MD5: ");
    oldLength = [nsmutStrHash length];
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, nsstrFileMD5);
    [nsmutStrHash addAttribute:NSLinkAttributeName
                         value:nsstrFileMD5
                         range:NSMakeRange(oldLength, [nsstrFileMD5 length])];

    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, @"\nSHA1: ");
    oldLength = [nsmutStrHash length];
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, nsstrFileSHA1);
    [nsmutStrHash addAttribute:NSLinkAttributeName
                         value:nsstrFileSHA1
                         range:NSMakeRange(oldLength, [nsstrFileSHA1 length])];

    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, @"\nSHA256: ");
    oldLength = [nsmutStrHash length];
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, nsstrFileSHA256);
    [nsmutStrHash addAttribute:NSLinkAttributeName
                         value:nsstrFileSHA256
                         range:NSMakeRange(oldLength, [nsstrFileSHA256 length])];

    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, @"\nCRC32: ");
    oldLength = [nsmutStrHash length];
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, nsstrFileCRC32);
    [nsmutStrHash addAttribute:NSLinkAttributeName
                         value:nsstrFileCRC32
                         range:NSMakeRange(oldLength, [nsstrFileCRC32 length])];

    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, @"\n\n");

    [nsmutStrHash endEditing];

    [nsmutString appendAttributedString:nsmutStrHash];
}

void UIBridgeMacUI::AppendFileErrToNSMutableAttributedString(const ResultData& result,
                                                             NSMutableAttributedString *nsmutString)
{
    string strAppend;
    strAppend.append(tstrtostr(result.tstrError));
    strAppend.append("\n\n");

    NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutString, nsstrAppend);
}

void UIBridgeMacUI::AppendResultToNSMutableAttributedString(const ResultData& result,
                                                            bool uppercase,
                                                            NSMutableAttributedString *nsmutString)
{
    if (result.enumState == ResultState::RESULT_NONE)
        return;

    if (result.enumState == ResultState::RESULT_ALL ||
        result.enumState == ResultState::RESULT_META ||
        result.enumState == ResultState::RESULT_ERROR ||
        result.enumState == ResultState::RESULT_PATH) {
        AppendFileNameToNSMutableAttributedString(result, nsmutString);
    }

    if (result.enumState == ResultState::RESULT_ALL ||
        result.enumState == ResultState::RESULT_META) {
        AppendFileMetaToNSMutableAttributedString(result, nsmutString);
    }

    if (result.enumState == ResultState::RESULT_ALL) {
        AppendFileHashToNSMutableAttributedString(result, uppercase, nsmutString);
    }

    if (result.enumState == ResultState::RESULT_ERROR) {
        AppendFileErrToNSMutableAttributedString(result, nsmutString);
    }

    if (result.enumState != ResultState::RESULT_ALL &&
        result.enumState != ResultState::RESULT_ERROR) {
        string strAppend = "\n";
        NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
        MacUtils::AppendNSStringToNSMutableAttributedString(nsmutString, nsstrAppend);
    }
}
