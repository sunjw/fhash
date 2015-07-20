//
//  MainViewController.h
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface MainViewController : NSViewController

@property (unsafe_unretained) IBOutlet NSScrollView *mainScrollView;
@property (unsafe_unretained) IBOutlet NSTextView *mainTextView;
@property (unsafe_unretained) IBOutlet NSProgressIndicator *mainProgressIndicator;
@property (unsafe_unretained) IBOutlet NSButton *openButton;
@property (unsafe_unretained) IBOutlet NSButton *upperCaseButton;

- (void)performViewDragOperation:(id<NSDraggingInfo>)sender;

@end

