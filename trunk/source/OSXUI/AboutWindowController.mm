//
//  AboutWindowController.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 15/8/4.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//

#import "AboutWindowController.h"

#import "MacUtils.h"

@interface AboutWindowController ()

@end

@implementation AboutWindowController

- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    self.window.title = GetNSStringFromResByKey(ABOUTDLG_TITLE);
    //self.window.backgroundColor = [NSColor textBackgroundColor];
    
}

@end
