//
//  HashBridge.h
//  fHashMacUI
//
//  Created by Sun Junwen on 2023/12/7.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class MainViewController;

@interface HashBridge : NSObject

- (instancetype)initWithController:(MainViewController *)mainViewController;

- (void)didLoad;
- (void)clear;

- (void)setStop:(bool)val;
- (void)setUppercase:(bool)val;
- (uint64_t)getTotalSize;

@end
