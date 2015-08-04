//
//  fHashMacAppDelegate.h
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MainViewController.h"

@interface fHashMacAppDelegate : NSObject <NSApplicationDelegate>

@property (weak) MainViewController *mainViewController;

- (IBAction)openDocument:(id)sender;

- (void)handleFinderContextOpen:(NSPasteboard *)pboard
                       userData:(NSString *)userData
                          error:(NSString **)error;

@end

