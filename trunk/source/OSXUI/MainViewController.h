//
//  MainViewController.h
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//
#include "OsUtils/OsThread.h"

#import <Cocoa/Cocoa.h>
#import "ESSProgressIndicator.h"

@interface MainViewController : NSViewController <NSTextViewDelegate>

@property (unsafe_unretained) IBOutlet NSScrollView *mainScrollView;
@property (unsafe_unretained) IBOutlet NSTextView *mainTextView;

@property (unsafe_unretained) IBOutlet ESSProgressIndicator *mainProgressIndicator;

@property (weak) IBOutlet NSButton *openButton;
@property (weak) IBOutlet NSButton *clearButton;
@property (weak) IBOutlet NSButton *verifyButton;

@property (weak) IBOutlet NSButton *upperCaseButton;

@property (weak) IBOutlet NSTextField *speedTextField;

@property (assign) sunjwbase::OsMutex *mainMtx;
@property (strong) NSMutableAttributedString *mainText;

@property (assign) NSUInteger tag;

- (BOOL)ableToCalcFiles;

- (BOOL)isCalculating;

- (void)openFiles;

- (void)performViewDragOperation:(id<NSDraggingInfo>)sender;

- (void)updateMainTextView;

- (void)calculateFinished;

- (void)calculateStopped;

- (void)startHashCalc:(NSArray *)fileNames isURL:(BOOL)isURL;

- (void)stopHashCalc:(BOOL)needExit;

- (IBAction)openButtonClicked:(NSButton *)sender;

- (IBAction)clearButtonClicked:(NSButton *)sender;
- (IBAction)verifyButtonClicked:(NSButton *)sender;

- (IBAction)uppercaseButtonClicked:(NSButton *)sender;

@end

