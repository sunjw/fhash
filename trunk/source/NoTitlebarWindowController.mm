//
//  NoTitlebarWindowController.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 15/8/4.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//

#import "NoTitlebarWindowController.h"

@interface NoTitlebarWindowController ()

@end

@implementation NoTitlebarWindowController

- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    self.window.titleVisibility = NSWindowTitleHidden;
    self.window.titlebarAppearsTransparent = YES;
    self.window.styleMask |= NSWindowStyleMaskFullSizeContentView;
    self.window.movableByWindowBackground = YES;
}

@end
