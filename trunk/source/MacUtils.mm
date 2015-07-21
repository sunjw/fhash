//
//  MacUtils.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 15/7/20.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//
#import "MacUtils.h"

#include <string>
#include "strhelper.h"
#include "Utils.h"
#include "Global.h"
#include "UIStrings.h"

#import <Foundation/Foundation.h>

using namespace std;
using namespace sunjwbase;

namespace MacUtils {
    
    string ConvertNSStringToUTF8String(const NSString *nsstr) {
        string strRet = string([nsstr UTF8String]);
        return strRet;
    }
    
    NSString *ConvertUTF8StringToNSString(const string stdstrUtf8) {
        return [NSString stringWithUTF8String:stdstrUtf8.c_str()];
    }
    
    string GetStringFromRes(const TCHAR *tzhName) {
        string strResString = tstrtostr(tzhName);
        string strUtf8 = utf8conv(strResString);
        strUtf8 = strreplace(strUtf8, "&", ""); // Remove "&"
        return strUtf8;
    }
    
    NSString *GetNSStringFromRes(const TCHAR *tzhName) {
        return ConvertUTF8StringToNSString(GetStringFromRes(tzhName));
    }
    
    void AppendFileNameToNSMutableString(const ResultData& result,
                                         NSMutableString *nsmutString) {
        string strAppend = MacUtils::GetStringFromRes(FILENAME_STRING);
        strAppend.append(" ");
        strAppend.append(tstrtostr(result.tstrPath));
        strAppend.append("\n");
        
        NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
        [nsmutString appendString:nsstrAppend];
    }
    
    void AppendFileMetaToNSMutableString(const ResultData& result,
                                         NSMutableString *nsmutString) {
        char chSizeBuff[1024] = {0};
        sprintf(chSizeBuff, "%llu", result.ulSize);
        string strShortSize = Utils::ConvertSizeToShortSizeStr(result.ulSize);
        
        string strAppend = MacUtils::GetStringFromRes(FILESIZE_STRING);
        strAppend.append(" ");
        strAppend.append(chSizeBuff);
        strAppend.append(" ");
        strAppend.append(MacUtils::GetStringFromRes(BYTE_STRING));
        if (strShortSize != "") {
            strAppend.append(" (");
            strAppend.append(strShortSize);
            strAppend.append(")");
        }
        strAppend.append("\n");
        strAppend.append(MacUtils::GetStringFromRes(MODIFYTIME_STRING));
        strAppend.append(" ");
        strAppend.append(tstrtostr(result.tstrMDate));
        strAppend.append("\n");
        
        NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
        [nsmutString appendString:nsstrAppend];
    }
    
    void AppendFileHashToNSMutableString(const ResultData& result,
                                         bool uppercase,
                                         NSMutableString *nsmutString) {
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
        
        string strAppend;
        strAppend.append("MD5: ");
        strAppend.append(strFileMD5);
        strAppend.append("\nSHA1: ");
        strAppend.append(strFileSHA1);
        strAppend.append("\nSHA256: ");
        strAppend.append(strFileSHA256);
        strAppend.append("\nCRC32: ");
        strAppend.append(strFileCRC32);
        strAppend.append("\n\n");
        
        NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
        [nsmutString appendString:nsstrAppend];
    }
    
    void AppendFileErrToNSMutableString(const ResultData& result,
                                        NSMutableString *nsmutString) {
        string strAppend;
        strAppend.append(tstrtostr(result.tstrError));
        strAppend.append("\n\n");
        
        NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
        [nsmutString appendString:nsstrAppend];
    }
    
}
