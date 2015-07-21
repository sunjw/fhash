//
//  MainViewController.h
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//
#include "OsUtils/OsThread.h"

#import <Cocoa/Cocoa.h>

@interface MainViewController : NSViewController

@property (unsafe_unretained) IBOutlet NSScrollView *mainScrollView;
@property (unsafe_unretained) IBOutlet NSTextView *mainTextView;
@property (unsafe_unretained) IBOutlet NSProgressIndicator *mainProgressIndicator;
@property (unsafe_unretained) IBOutlet NSButton *openButton;
@property (unsafe_unretained) IBOutlet NSButton *upperCaseButton;

@property (assign) sunjwbase::OsMutex *mainMtx;
@property (strong) NSMutableString *mainText;

- (void)performViewDragOperation:(id<NSDraggingInfo>)sender;

- (void)updateMainTextView;

- (void)calculateFinished;

- (void)calculateStopped;

@end

