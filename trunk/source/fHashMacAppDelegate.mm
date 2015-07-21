//
//  fHashMacAppDelegate.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import "fHashMacAppDelegate.h"

@interface fHashMacAppDelegate ()

@end

@implementation fHashMacAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    
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

@end
