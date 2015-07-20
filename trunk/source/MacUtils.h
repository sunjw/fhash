//
//  MacUtils.h
//  fHashMacUI
//
//  Created by Sun Junwen on 15/7/20.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//

#ifndef _MAC_UTILS_H_
#define _MAC_UTILS_H_

#include <string>
#import <Foundation/Foundation.h>

namespace MacUtils
{
    std::string ConvertNSStringToUTF8String(NSString *nsstr);

}

#endif
