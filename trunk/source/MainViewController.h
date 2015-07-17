//
//  MainViewController.h
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface MainViewController : NSViewController

@property (weak) IBOutlet NSScrollView *mainScrollView;

@property (unsafe_unretained) IBOutlet NSTextView *mainTextView;

@property (weak) IBOutlet NSProgressIndicator *mainProgressIndicator;

@property (weak) IBOutlet NSButton *openButton;

@end

