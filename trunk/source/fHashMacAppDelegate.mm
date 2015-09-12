//
//  fHashMacAppDelegate.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import "fHashMacAppDelegate.h"
#include <objc/runtime.h>

@interface fHashMacAppDelegate ()

@end

@implementation fHashMacAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    
    // Fix NSVisualEffectView compatibility on OS X < 10.10
    if (![NSVisualEffectView class]) {
        Class NSVisualEffectViewClass = objc_allocateClassPair([NSView class],
                                                               "NSVisualEffectView",
                                                               0);
        objc_registerClassPair(NSVisualEffectViewClass);
    }
    
    [NSApp setServicesProvider:self];
    NSUpdateDynamicServices();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    if ([self.mainViewController isCalculating]) {
        [self.mainViewController stopHashCalc:YES];
        return NSTerminateCancel;
    } else {
        return NSTerminateNow;
    }
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

- (void)application:(NSApplication *)sender openFiles:(NSArray *)filenames {
    [self.mainViewController startHashCalc:filenames isURL:NO];
    
    [sender replyToOpenOrPrint:NSApplicationDelegateReplySuccess];
}

- (IBAction)openDocument:(id)sender {
    // Menu bar "open"
    [self.mainViewController openFiles];
}

- (void)handleFinderContextOpen:(NSPasteboard *)pboard
                       userData:(NSString *)userData
                          error:(NSString **)error {
    NSArray* filenames = [pboard propertyListForType:NSFilenamesPboardType];
    [self.mainViewController startHashCalc:filenames isURL:NO];
}

@end
