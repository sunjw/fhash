//
//  AboutWindowController.m
//  fHashMacUI
//
//  Created by Sun Junwen on 15/8/4.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//

#import "AboutWindowController.h"

@interface AboutWindowController ()

@end

@implementation AboutWindowController

- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    self.window.titleVisibility = NSWindowTitleHidden;
    self.window.titlebarAppearsTransparent = YES;
    self.window.styleMask |= NSFullSizeContentViewWindowMask;
    self.window.movableByWindowBackground = YES;
}

@end
