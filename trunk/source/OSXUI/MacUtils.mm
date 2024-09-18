//
//  MacUtils.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 15/7/20.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//
#import "MacUtils.h"

#include <string>
#include "Common/strhelper.h"
#include "Common/Utils.h"
#include "Common/Global.h"

#import <Foundation/Foundation.h>

using namespace std;
using namespace sunjwbase;

namespace MacUtils {

    string ConvertNSStringToUTF8String(const NSString *nsstr) {
        string strRet = string([nsstr UTF8String]);
        return strRet;
    }

    NSString *ConvertUTF8StringToNSString(const string& stdstrUtf8) {
        return [NSString stringWithUTF8String:stdstrUtf8.c_str()];
    }

}
