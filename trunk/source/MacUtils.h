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
#include "strhelper.h"
#include "Global.h"

#import <Foundation/Foundation.h>

#define GetNSStringFromResByKey(key) MacUtils::GetNSStringFromRes(@"" #key "")
#define GetStringFromResByKey(key) MacUtils::GetStringFromRes(@"" #key "")

namespace MacUtils {

    /**
     * An objc class ref <-> c pointer wrapper.
     * Like a weak pointer, not change reference count.
     */
    template <class NSTYPE>
    class ObjcWeakPtr {
    public:
        ObjcWeakPtr(NSTYPE *obj);
        
        void set(NSTYPE *obj);
        NSTYPE *get() const;
        
    private:
        void *_ptr;
    };
    
    template <class NSTYPE>
    ObjcWeakPtr<NSTYPE>::ObjcWeakPtr(NSTYPE *obj) {
        set(obj);
    }
    
    template <class NSTYPE>
    void ObjcWeakPtr<NSTYPE>::set(NSTYPE *obj) {
        _ptr = (__bridge void *)obj;
    }
    
    template <class NSTYPE>
    NSTYPE *ObjcWeakPtr<NSTYPE>::get() const {
        return (__bridge NSTYPE *)_ptr;
    }
    
    /**
     * Get system version.
     */
    NSString *GetSystemVersion();
    
    /**
     * Get system major version.
     */
    NSInteger GetSystemMajorVersion();
    
    /**
     * Get system minor version.
     */
    NSInteger GetSystemMinorVersion();
    
    /**
     * System version is below sepecific version.
     */
    bool IsSystemEarlierThan(int majorVersion, int minorVersion);
    
    /**
     * System version is ealier than 10.10.
     */
    bool IsSystemEarlierThan10_10();
    
    /**
     * System version is ealier than 10.11.
     */
    bool IsSystemEarlierThan10_11();
    
    /**
     * Get system preferred languange full-name.
     */
    NSString *GetSystemPreferredLanguage();
    
    /**
     * Convert NSString to an UTF-8 std::string.
     */
    std::string ConvertNSStringToUTF8String(const NSString *nsstr);
    
    /**
     * Convert an UTF-8 std::string to NSString.
     */
    NSString *ConvertUTF8StringToNSString(const std::string stdstrUtf8);
    
    /**
     * Get specific named string as std::string.
     */
    std::string GetStringFromRes(NSString *nsstrKey);
    
    /**
     * Get specific named string as NSString.
     */
    NSString *GetNSStringFromRes(NSString *nsstrKey);

    /**
     * Convert NSString to plain NSAttributedString.
     */
    NSAttributedString *ConvertNSStringToNSAttributedString(NSString *str);

    /**
     * Append NSString to NSMutableAttributedString.
     */
    void AppendNSStringToNSMutableAttributedString(NSMutableAttributedString *base, NSString *str);
    
}

#endif
