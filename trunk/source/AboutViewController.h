//
//  AboutViewController.h
//  fHashMacUI
//
//  Created by Sun Junwen on 15/8/4.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AboutViewController : NSViewController

@property (weak) IBOutlet NSImageView *iconImageView;
@property (weak) IBOutlet NSTextField *infoTextField;
@property (weak) IBOutlet NSButton *closeButton;

- (IBAction)closeButtonClicked:(NSButton *)sender;

@end