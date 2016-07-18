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

#import <Foundation/Foundation.h>

using namespace std;
using namespace sunjwbase;

namespace MacUtils {
    
    NSString *GetSystemVersion() {
        NSDictionary *systemVersionDictionary = [NSDictionary dictionaryWithContentsOfFile:
                                                 @"/System/Library/CoreServices/SystemVersion.plist"];
        NSString *systemVersion = [systemVersionDictionary objectForKey:@"ProductVersion"];
        return systemVersion;
    }
    
    NSInteger GetSystemMajorVersion() {
        NSInteger nsiMajorVersion = 0;
        
        NSString *systemVersion = GetSystemVersion();
        NSArray *versionParts = [systemVersion componentsSeparatedByString: @"."];
        if (versionParts != nil && versionParts.count > 0) {
            NSString *nsstrMajorVersion = [versionParts objectAtIndex:0];
            nsiMajorVersion = [nsstrMajorVersion integerValue];
        }
        
        return nsiMajorVersion;
    }
    
    NSInteger GetSystemMinorVersion() {
        NSInteger nsiMinorVersion = 0;
        
        NSString *systemVersion = GetSystemVersion();
        NSArray *versionParts = [systemVersion componentsSeparatedByString: @"."];
        if (versionParts != nil && versionParts.count > 1) {
            NSString *nsstrMinorVersion = [versionParts objectAtIndex:1];
            nsiMinorVersion = [nsstrMinorVersion integerValue];
        }
        
        return nsiMinorVersion;
    }
    
    bool IsSystemEarlierThan(int majorVersion, int minorVersion) {
        NSInteger systemMajorVersion = MacUtils::GetSystemMajorVersion();
        NSInteger systemMinorVersion = MacUtils::GetSystemMinorVersion();
        return (systemMajorVersion < majorVersion ||
                (systemMajorVersion == majorVersion && systemMinorVersion < minorVersion));
    }
    
    bool IsSystemEarlierThan10_10() {
        return IsSystemEarlierThan(10, 10);
    }
    
    bool IsSystemEarlierThan10_11() {
        return IsSystemEarlierThan(10, 11);
    }
    
    NSString *GetSystemPreferredLanguage() {
        NSString* language = [[NSLocale preferredLanguages] objectAtIndex:0];
        return language;
    }
    
    string ConvertNSStringToUTF8String(const NSString *nsstr) {
        string strRet = string([nsstr UTF8String]);
        return strRet;
    }
    
    NSString *ConvertUTF8StringToNSString(const string stdstrUtf8) {
        return [NSString stringWithUTF8String:stdstrUtf8.c_str()];
    }
    
    string GetStringFromRes(NSString *nsstrKey) {
        NSString *nsstrLocalized = NSLocalizedString(nsstrKey, @"");
        string strLocalized = ConvertNSStringToUTF8String(nsstrLocalized);
        // Fix Windows UI strings.
        // Fix zh-cn.
        for (char chTest = 'A'; chTest <= 'Z'; ++chTest) {
            string strTest = "(&";
            strTest += chTest;
            strTest.append(")");
            strLocalized = strreplace(strLocalized, strTest, ""); // Remove "(&X)"
        }
        // Fix en-us
        strLocalized = strreplace(strLocalized, "&", ""); // Remove "&"
        
        return strLocalized;
    }
    
    NSString *GetNSStringFromRes(NSString *nsstrKey) {
        string strLocalized = GetStringFromRes(nsstrKey);
        NSString *nsstrLocalized = ConvertUTF8StringToNSString(strLocalized);
        return nsstrLocalized;
    }

    NSAttributedString *ConvertNSStringToNSAttributedString(NSString *str) {
        return [[NSAttributedString alloc] initWithString:str];
    }

    void AppendNSStringToNSMutableAttributedString(NSMutableAttributedString *base, NSString *str) {
        [base appendAttributedString:ConvertNSStringToNSAttributedString(str)];
    }
    
}
