//
//  HashBridge.h
//  fHashMacUI
//
//  Created by Sun Junwen on 2023/12/7.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class MainViewControllerX;

@interface HashBridge : NSObject

- (instancetype)initWithController:(MainViewControllerX *)mainViewController;

@end