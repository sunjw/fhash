//
//  UIBridgeMacSwift.mm
//  fHash
//
//  Created by Sun Junwen on 2023/12/7.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

#import "UIBridgeMacSwift.h"

#include <stdlib.h>
#include <string>
#include <dispatch/dispatch.h>

#import <Cocoa/Cocoa.h>
#import "fHash-Swift.h"

#include "Common/strhelper.h"
#include "Common/Utils.h"
#include "MacUtils.h"
#include "Common/Global.h"

using namespace std;
using namespace sunjwbase;

UIBridgeMacSwift::UIBridgeMacSwift(MainViewControllerX *mainViewController)
:_mainViewControllerPtr(mainViewController)
{
}

UIBridgeMacSwift::~UIBridgeMacSwift()
{
}

void UIBridgeMacSwift::lockData()
{
    //MainViewController *mainViewController = _mainViewControllerPtr.get();
    //mainViewController.mainMtx->lock();
}

void UIBridgeMacSwift::unlockData()
{
    //MainViewController *mainViewController = _mainViewControllerPtr.get();
    //mainViewController.mainMtx->unlock();
}

void UIBridgeMacSwift::preparingCalc()
{
    MainViewControllerX *mainViewController = _mainViewControllerPtr.get();
    [mainViewController preparingCalc];
}

void UIBridgeMacSwift::removePreparingCalc()
{
    MainViewControllerX *mainViewController = _mainViewControllerPtr.get();

    //lockData();
    //{
    //    // Reset old string.
    //    mainViewController.mainText = [[NSMutableAttributedString alloc] initWithAttributedString:_nsstrNoPreparing];
    //}
    //unlockData();
}

void UIBridgeMacSwift::calcStop()
{
    MainViewControllerX *mainViewController = _mainViewControllerPtr.get();

    //dispatch_async(dispatch_get_main_queue(), ^{
    //    [mainViewController calculateStopped];
    //});
}

void UIBridgeMacSwift::calcFinish()
{
    MainViewControllerX *mainViewController = _mainViewControllerPtr.get();

    //dispatch_async(dispatch_get_main_queue(), ^{
    //    [mainViewController calculateFinished];
    //});
}

void UIBridgeMacSwift::showFileName(const ResultData& result)
{
    MainViewControllerX *mainViewController = _mainViewControllerPtr.get();

    //lockData();
    //{
    //    UIBridgeMacSwift::AppendFileNameToNSMutableAttributedString(result, mainViewController.mainText);
    //}
    //unlockData();
    //
    //dispatch_async(dispatch_get_main_queue(), ^{
    //    [mainViewController updateMainTextView];
    //});
}

void UIBridgeMacSwift::showFileMeta(const ResultData& result)
{
    MainViewControllerX *mainViewController = _mainViewControllerPtr.get();

    //lockData();
    //{
    //    UIBridgeMacSwift::AppendFileMetaToNSMutableAttributedString(result, mainViewController.mainText);
    //}
    //unlockData();
    //
    //dispatch_async(dispatch_get_main_queue(), ^{
    //    [mainViewController updateMainTextView];
    //});
}

void UIBridgeMacSwift::showFileHash(const ResultData& result, bool uppercase)
{
    MainViewControllerX *mainViewController = _mainViewControllerPtr.get();

    //lockData();
    //{
    //    UIBridgeMacSwift::AppendFileHashToNSMutableAttributedString(result,
    //                                                                uppercase,
    //                                                                mainViewController.mainText);
    //}
    //unlockData();
    //
    //dispatch_async(dispatch_get_main_queue(), ^{
    //    [mainViewController updateMainTextView];
    //});
}

void UIBridgeMacSwift::showFileErr(const ResultData& result)
{
    MainViewControllerX *mainViewController = _mainViewControllerPtr.get();

    //lockData();
    //{
    //    UIBridgeMacSwift::AppendFileErrToNSMutableAttributedString(result, mainViewController.mainText);
    //}
    //unlockData();
    //
    //dispatch_async(dispatch_get_main_queue(), ^{
    //    [mainViewController updateMainTextView];
    //});
}

int UIBridgeMacSwift::getProgMax()
{
    return 200;
}

void UIBridgeMacSwift::updateProg(int value)
{
}

void UIBridgeMacSwift::updateProgWhole(int value)
{
    //dispatch_async(dispatch_get_main_queue(), ^{
    //    MainViewController *mainViewController = _mainViewControllerPtr.get();
    //    int oldValue = (int)mainViewController.mainProgressIndicator.toValue;
    //    if (value == oldValue)
    //        return;
    //    [mainViewController.mainProgressIndicator animateToDoubleValue:(double)value];
    //});
}

void UIBridgeMacSwift::fileCalcFinish()
{
}

void UIBridgeMacSwift::fileFinish()
{
}

ResultDataSwift *UIBridgeMacSwift::ConvertResultDataToSwift(const ResultData& result)
{
    ResultDataSwift *resultDataSwift = [[ResultDataSwift alloc] init];
    switch (result.enumState)
    {
        case ResultState::RESULT_NONE:
            resultDataSwift.state = ResultDataSwift.RESULT_NONE;
            break;
        case ResultState::RESULT_PATH:
            resultDataSwift.state = ResultDataSwift.RESULT_PATH;
            break;
        case ResultState::RESULT_META:
            resultDataSwift.state = ResultDataSwift.RESULT_META;
            break;
        case ResultState::RESULT_ALL:
            resultDataSwift.state = ResultDataSwift.RESULT_ALL;
            break;
        case ResultState::RESULT_ERROR:
            resultDataSwift.state = ResultDataSwift.RESULT_ERROR;
            break;
    }
    resultDataSwift.strPath = MacUtils::ConvertUTF8StringToNSString(tstrtostr(result.tstrPath));
    resultDataSwift.ulSize = result.ulSize;
    resultDataSwift.strMDate = MacUtils::ConvertUTF8StringToNSString(tstrtostr(result.tstrPath));
    resultDataSwift.strVersion = MacUtils::ConvertUTF8StringToNSString(tstrtostr(result.tstrVersion));
    resultDataSwift.strMD5 = MacUtils::ConvertUTF8StringToNSString(tstrtostr(result.tstrMD5));
    resultDataSwift.strSHA1 = MacUtils::ConvertUTF8StringToNSString(tstrtostr(result.tstrSHA1));
    resultDataSwift.strSHA256 = MacUtils::ConvertUTF8StringToNSString(tstrtostr(result.tstrSHA256));
    resultDataSwift.strSHA512 = MacUtils::ConvertUTF8StringToNSString(tstrtostr(result.tstrSHA512));
    resultDataSwift.strError = MacUtils::ConvertUTF8StringToNSString(tstrtostr(result.tstrError));

    return resultDataSwift;
}

void UIBridgeMacSwift::AppendFileNameToNSMutableAttributedString(const ResultData& result,
                                                                 NSMutableAttributedString *nsmutString)
{
    string strAppend = GetStringFromResByKey(FILENAME_STRING);
    strAppend.append(" ");
    strAppend.append(tstrtostr(result.tstrPath));
    strAppend.append("\n");

    NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutString, nsstrAppend);
}

void UIBridgeMacSwift::AppendFileMetaToNSMutableAttributedString(const ResultData& result,
                                                                 NSMutableAttributedString *nsmutString)
{
    char chSizeBuff[1024] = {0};
    snprintf(chSizeBuff, 1024, "%llu", result.ulSize);
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

void UIBridgeMacSwift::AppendFileHashToNSMutableAttributedString(const ResultData& result,
                                                                 bool uppercase,
                                                                 NSMutableAttributedString *nsmutString)
{
    string strFileMD5, strFileSHA1, strFileSHA256, strFileSHA512;

    if (uppercase) {
        strFileMD5 = str_upper(tstrtostr(result.tstrMD5));
        strFileSHA1 = str_upper(tstrtostr(result.tstrSHA1));
        strFileSHA256 = str_upper(tstrtostr(result.tstrSHA256));
        strFileSHA512 = str_upper(tstrtostr(result.tstrSHA512));
    } else {
        strFileMD5 = str_lower(tstrtostr(result.tstrMD5));
        strFileSHA1 = str_lower(tstrtostr(result.tstrSHA1));
        strFileSHA256 = str_lower(tstrtostr(result.tstrSHA256));
        strFileSHA512 = str_lower(tstrtostr(result.tstrSHA512));
    }

    NSString *nsstrFileMD5 = MacUtils::ConvertUTF8StringToNSString(strFileMD5);
    NSString *nsstrFileSHA1 = MacUtils::ConvertUTF8StringToNSString(strFileSHA1);
    NSString *nsstrFileSHA256 = MacUtils::ConvertUTF8StringToNSString(strFileSHA256);
    NSString *nsstrFileSHA512 = MacUtils::ConvertUTF8StringToNSString(strFileSHA512);

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

    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, @"\nSHA512: ");
    oldLength = [nsmutStrHash length];
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, nsstrFileSHA512);
    [nsmutStrHash addAttribute:NSLinkAttributeName
                         value:nsstrFileSHA512
                         range:NSMakeRange(oldLength, [nsstrFileSHA512 length])];

    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutStrHash, @"\n\n");

    [nsmutStrHash endEditing];

    [nsmutString appendAttributedString:nsmutStrHash];
}

void UIBridgeMacSwift::AppendFileErrToNSMutableAttributedString(const ResultData& result,
                                                                NSMutableAttributedString *nsmutString)
{
    string strAppend;
    strAppend.append(tstrtostr(result.tstrError));
    strAppend.append("\n\n");

    NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
    MacUtils::AppendNSStringToNSMutableAttributedString(nsmutString, nsstrAppend);
}

void UIBridgeMacSwift::AppendResultToNSMutableAttributedString(const ResultData& result,
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
