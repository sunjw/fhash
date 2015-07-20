//
//  MacUtils.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 15/7/20.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//
#import "MacUtils.h"

#include <string>
#import <Foundation/Foundation.h>

using namespace std;

namespace MacUtils {
    
    string ConvertNSStringToUTF8String(NSString *nsstr) {
        string strRet = string([nsstr UTF8String]);
        return strRet;
    }
    
    NSString *ConvertUTF8StringToNSString(string stdstrUtf8) {
        return [NSString stringWithUTF8String:stdstrUtf8.c_str()];
    }
    
}
