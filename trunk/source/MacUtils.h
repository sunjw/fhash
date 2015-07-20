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

namespace MacUtils {
    
    // Convert NSString to an UTF-8 std::string.
    std::string ConvertNSStringToUTF8String(NSString *nsstr);
    
    // Convert an UTF-8 std::string to NSString
    NSString *ConvertUTF8StringToNSString(std::string stdstrUtf8);

    // An objc class ref <-> c pointer wrapper.
    // Like a weak pointer, not change reference count.
    template <class NSTYPE>
    class ObjcWeakPtr {
    public:
        ObjcWeakPtr(NSTYPE *obj);
        
        void set(NSTYPE *obj);
        NSTYPE *get();
        
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
    NSTYPE *ObjcWeakPtr<NSTYPE>::get() {
        return (__bridge NSTYPE *)_ptr;
    }
}

#endif
