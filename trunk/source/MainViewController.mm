//
//  MainViewController.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import "MainViewController.h"

#include <stdint.h>
#include <pthread.h>
#include <string>
#include "strhelper.h"
#include "OsUtils/OsThread.h"
#include "Utils.h"
#include "Global.h"
#include "HashEngine.h"

#import <Cocoa/Cocoa.h>
#import "MacUtils.h"
#import "ESSProgressIndicator.h"
#import "UIBridgeMacUI.h"
#import "fHashMacAppDelegate.h"
#import "MainWindowController.h"
#import "MainView.h"

using namespace std;
using namespace sunjwbase;

#define UPPERCASE_DEFAULT_KEY @"upperCaseKey"

enum MainViewControllerState {
    MAINVC_NONE = 0,        // clear state
    MAINVC_CALC_ING,        // calculating
    MAINVC_CALC_FINISH,     // calculating finished/stopped
    MAINVC_VERIFY,          // verfing
    MAINVC_WAITING_EXIT,    // waiting thread stop and exit
};

@interface MainViewController()

@property (assign) MainViewControllerState state;

@property (nonatomic, strong) NSFont *mainFont;

@property (nonatomic, strong) NSMutableParagraphStyle *mainParaStyle;
@property (assign) BOOL needParaFix;

@property (nonatomic, strong) NSString *selectedLink;

@property (assign) UIBridgeMacUI *uiBridgeMac;

@property (assign) uint64_t calcStartTime;
@property (assign) uint64_t calcEndTime;

@property (assign) BOOL upperCaseState;

@property (assign) ThreadData *thrdData;
@property (assign) pthread_t ptHash;

@end

@implementation MainViewController

@synthesize state = _state;

@synthesize mainMtx = _mainMtx;
@synthesize mainText = _mainText;

@synthesize tag = _tag;

@synthesize uiBridgeMac = _uiBridgeMac;

@synthesize calcStartTime = _calcStartTime;
@synthesize calcEndTime = _calcEndTime;

@synthesize upperCaseState = _upperCaseState;

@synthesize thrdData = _thrdData;
@synthesize ptHash = _ptHash;

// Not be called on exit.
// Just for sure.
- (void)dealloc {
    delete _mainMtx;
    delete _uiBridgeMac;
    delete _thrdData;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    fHashMacAppDelegate *fHashDelegate = (fHashMacAppDelegate *)[NSApp delegate];
    
    // Initiate.
    fHashDelegate.mainViewController = self;
    
    MainView *mainView = (MainView *)self.view;
    mainView.mainViewController = self;
    
    // Register NSUserDefaults.
    NSDictionary *defaultsDictionary = [NSDictionary
                                        dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], UPPERCASE_DEFAULT_KEY,
                                        nil];
    [[NSUserDefaults standardUserDefaults] registerDefaults:defaultsDictionary];
    
    // Load NSUserDefaults.
    BOOL defaultUpperCase = [[NSUserDefaults standardUserDefaults]
                             boolForKey:UPPERCASE_DEFAULT_KEY];
    
    // Alloc c++ member.
    _mainMtx = new OsMutex();
    _uiBridgeMac = new UIBridgeMacUI(self);
    _thrdData = new ThreadData();
    
    [self setViewControllerState:MAINVC_NONE];
    
    _thrdData->uiBridge = _uiBridgeMac;
    
    NSMenu *fileMenu = [self getFileMenu];
    [fileMenu setAutoenablesItems:NO];
    
    // Set buttons title.
    [self.verifyButton
     setTitle:GetNSStringFromResByKey(MAINDLG_VERIFY)];
    [self.upperCaseButton
     setTitle:GetNSStringFromResByKey(MAINDLG_UPPER_HASH)];
    
    // Set open button as default.
    [self.openButton setKeyEquivalent:@"\r"];
    
    // Set scroll view border type.
    self.mainScrollView.borderType = NSNoBorder;
    
    // Set some text in text field.
    self.mainTextView.delegate = self;
    [self.mainTextView setTextContainerInset:NSMakeSize(4.0, 4.0)];
    
    self.mainFont = [NSFont fontWithName:@"Monaco" size:12];
    if (self.mainFont == nil) {
        self.mainFont = self.mainTextView.font;
    }

    self.mainParaStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
    [self.mainParaStyle setLineSpacing:self.mainTextView.defaultParagraphStyle.lineSpacing + (float)4];

    [self.mainTextView setFont:self.mainFont];

    self.needParaFix = NO;
    NSString *prefLanguage = MacUtils::GetSystemPreferredLanguage();
    if ([prefLanguage hasPrefix:@"zh"] &&
        MacUtils::IsSystemEarlierThan10_11()) {
        // Below 10.11, Chinese fonts is not good.
        // Make a little tweak.
        self.needParaFix = YES;
    }

    if (self.needParaFix) {
        [self.mainTextView setDefaultParagraphStyle:self.mainParaStyle];
    }
    
    [self.mainTextView setUsesFindBar:YES];
    
    // Set TextView nowrap.
    [[self.mainTextView enclosingScrollView] setHasHorizontalScroller:YES];
    [self.mainTextView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
    [self.mainTextView setHorizontallyResizable:YES];
    [self.mainTextView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
    [[self.mainTextView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
    [[self.mainTextView textContainer] setWidthTracksTextView:NO];
    
    // Set progressbar.
    [self.mainProgressIndicator setMaxValue:_uiBridgeMac->getProgMax()];
    
    // Set checkbox.
    if (defaultUpperCase) {
        [self.upperCaseButton setState:NSOnState];
    } else {
        [self.upperCaseButton setState:NSOffState];
    }
    [self updateUpperCaseState];
    
    // Update main text.
    [self updateMainTextView];
}

- (void)viewWillDisappear {
    // Save NSUserDefaults.
    BOOL defaultUpperCase = ([self.upperCaseButton state] == NSOnState);
    [[NSUserDefaults standardUserDefaults] setBool:defaultUpperCase
                                            forKey:UPPERCASE_DEFAULT_KEY];
    
    // Close other windows.
    NSArray *windows = [[NSApplication sharedApplication] windows];
    NSInteger windowCount = windows.count;
    for (NSInteger i = 0; i < windowCount; ++i) {
        NSWindow *window = [windows objectAtIndex:i];
        NSWindowController *windowController = window.windowController;
        if (windowController == nil ||
            ![windowController isKindOfClass:[MainWindowController class]]) {
            // It looks like about panel or finder overlay window.
            // They will stop us exit, let's close them.
            [window close];
        }
    }
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)setViewControllerState:(MainViewControllerState)newState {
    switch (newState) {
        case MAINVC_NONE: {
            // Clear all.
            _thrdData->threadWorking = false;
            _thrdData->stop = false;
            
            _thrdData->uppercase = false;
            _thrdData->totalSize = 0;
            
            _thrdData->nFiles = 0;
            _thrdData->fullPaths.clear();
            
            _thrdData->resultList.clear();
            
            _mainMtx->lock();
            {
                _mainText = [[NSMutableAttributedString alloc] init];
                string strAppend = GetStringFromResByKey(MAINDLG_INITINFO);
                strAppend.append("\n\n");
                NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);

                MacUtils::AppendNSStringToNSMutableAttributedString(_mainText, nsstrAppend);
            }
            _mainMtx->unlock();
            
            [self.mainProgressIndicator jumpToDoubleValue:0];
            [self.speedTextField setStringValue:@""];
         
            // Passthrough to MAINVC_CALC_FINISH.
        }
        case MAINVC_CALC_FINISH: {
            _calcEndTime = Utils::GetCurrentMilliSec();
            
            // Set controls title.
            NSMenuItem *openMenuItem = [self getOpenMenuItem];
            [openMenuItem setEnabled:YES];
            
            [self.openButton
             setTitle:GetNSStringFromResByKey(MAINDLG_OPEN)];
            [self.clearButton
             setTitle:GetNSStringFromResByKey(MAINDLG_CLEAR)];
            [self.clearButton setEnabled:YES];
            [self.verifyButton setEnabled:YES];
            [self.upperCaseButton setEnabled:YES];
            
        } break;
        case MAINVC_CALC_ING: {
            _calcStartTime = Utils::GetCurrentMilliSec();
            
            _thrdData->stop = false;
            
            NSMenuItem *openMenuItem = [self getOpenMenuItem];
            [openMenuItem setEnabled:NO];
            
            [self.speedTextField setStringValue:@""];
            
            [self.openButton
             setTitle:GetNSStringFromResByKey(MAINDLG_STOP)];
            [self.clearButton setEnabled:NO];
            [self.verifyButton setEnabled:NO];
            [self.upperCaseButton setEnabled:NO];
            
        } break;
        case MAINVC_VERIFY: {
            
        } break;
        case MAINVC_WAITING_EXIT: {
            
        } break;
    }
    
    MainViewControllerState oldState = _state;
    _state = newState;
    
    if (_state == MAINVC_CALC_FINISH &&
        oldState == MAINVC_WAITING_EXIT) {
        // User want to close.
        [self.view.window close];
    }

}

- (NSMenu *)getFileMenu {
    NSMenu *mainMenu = [NSApp mainMenu];
    NSMenuItem *fileMenuItem = [mainMenu itemAtIndex:1];
    return fileMenuItem.submenu;
}

- (NSMenuItem *)getOpenMenuItem {
    NSMenu *mainMenu = [NSApp mainMenu];
    NSMenuItem *fileMenuItem = [mainMenu itemAtIndex:1];
    NSMenuItem *openMenuItem = [fileMenuItem.submenu itemAtIndex:0];
    return openMenuItem;
}

- (BOOL)ableToCalcFiles {
    return ![self isCalculating];
}

- (BOOL)isCalculating {
    return (_state == MAINVC_CALC_ING ||
            _state == MAINVC_WAITING_EXIT);
}

- (void)openFiles {
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    openPanel.showsResizeIndicator = YES;
    openPanel.showsHiddenFiles = NO;
    openPanel.canChooseDirectories = NO;
    openPanel.canCreateDirectories = YES;
    openPanel.allowsMultipleSelection = YES;
    openPanel.allowedFileTypes = nil; // all types
    
    [openPanel beginSheetModalForWindow:self.view.window completionHandler:
     ^(NSInteger result) {
         if (result == NSFileHandlingPanelOKButton) {
             NSArray* fileNames = [openPanel URLs];
             [self startHashCalc:fileNames isURL:YES];
         }
     }];
}

- (void)performViewDragOperation:(id<NSDraggingInfo>)sender {
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *fileNames = [pboard propertyListForType:NSFilenamesPboardType];
    
    [self startHashCalc:fileNames isURL:NO];
}

- (void)updateUpperCaseState {
    _upperCaseState = ([self.upperCaseButton state] == NSOnState);
}

- (void)updateMainTextView:(BOOL)keepScrollPosition {
    _mainMtx->lock();

    // Apply style to all text.
    [_mainText beginEditing];

    [_mainText addAttribute:NSFontAttributeName
                      value:self.mainFont
                      range:NSMakeRange(0, [_mainText length])];

    if (self.needParaFix) {
        [_mainText addAttribute:NSParagraphStyleAttributeName
                          value:self.mainParaStyle
                          range:NSMakeRange(0, [_mainText length])];
    }

    [_mainText endEditing];

    [[self.mainTextView textStorage] setAttributedString:_mainText];

    _mainMtx->unlock();
    
    if (!keepScrollPosition) {
        // Scroll to end.
        [self.mainTextView
         scrollRangeToVisible:NSMakeRange(self.mainTextView.string.length, 0)];
    }
}

- (void)updateMainTextView {
    [self updateMainTextView:NO];
}

- (void)calculateFinished {
    [self setViewControllerState:MAINVC_CALC_FINISH];
    [self.mainProgressIndicator jumpToDoubleValue:_uiBridgeMac->getProgMax()];
    
    // Show calc speed.
    uint64_t calcDurationTime = _calcEndTime - _calcStartTime;
    if (calcDurationTime > 10) {
        // speed is Bytes/ms
        double calcSpeed = (double)_thrdData->totalSize / calcDurationTime;
        calcSpeed = calcSpeed * 1000; // Bytes/s
        
        string strSpeed;
        strSpeed = Utils::ConvertSizeToShortSizeStr((uint64_t)calcSpeed, true);
        if (strSpeed != "") {
            strSpeed.append("/s");
        }
        
        NSString *nsstrSpeed = MacUtils::ConvertUTF8StringToNSString(strSpeed);
        [self.speedTextField setStringValue:nsstrSpeed];
        
    } else {
        [self.speedTextField setStringValue:@""];
    }
}

- (void)calculateStopped {
    string strAppend = "\n";
    //strAppend.append(MacUtils::GetStringFromRes(MAINDLG_CALCU_TERMINAL));
    //strAppend.append("\n\n");
    NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
    
    _mainMtx->lock();
    MacUtils::AppendNSStringToNSMutableAttributedString(_mainText, nsstrAppend);
    _mainMtx->unlock();
    
    [self setViewControllerState:MAINVC_CALC_FINISH];
    [self.mainProgressIndicator jumpToDoubleValue:0];
    
    //[self updateMainTextView];
}

- (void)startHashCalc:(NSArray *)fileNames isURL:(BOOL)isURL {
    if (![self ableToCalcFiles]) {
        return;
    }
    
    if (_state == MAINVC_NONE) {
        // Clear up text.
        _mainMtx->lock();
        _mainText = [[NSMutableAttributedString alloc] init];
        _mainMtx->unlock();
    }
    
    // Get files path.
    NSUInteger fileCount = [fileNames count];
    _thrdData->nFiles = (uint32_t)fileCount;
    _thrdData->fullPaths.clear();
    
    for (uint32_t i = 0; i < _thrdData->nFiles; ++i) {
        string strFileName;
        if (!isURL) {
            NSString *nsstrFileName = [fileNames objectAtIndex:i];
            strFileName = MacUtils::ConvertNSStringToUTF8String(nsstrFileName);
        } else {
            NSURL *nsurlFileName = [fileNames objectAtIndex:i];
            strFileName = MacUtils::ConvertNSStringToUTF8String([nsurlFileName path]);
        }
        _thrdData->fullPaths.push_back(strtotstr(strFileName));
    }
    
    // Uppercase.
    [self updateUpperCaseState];
    _thrdData->uppercase = _upperCaseState;
    
    [self.mainProgressIndicator jumpToDoubleValue:0];
    
    // Ready to go.
    [self setViewControllerState:MAINVC_CALC_ING];
    
    pthread_create(&_ptHash,
                   NULL,
                   (void *(*)(void *))HashThreadFunc,
                   _thrdData);

}

- (void)stopHashCalc:(BOOL)needExit {
    if (_state == MAINVC_CALC_ING) {
        _thrdData->stop = true;
        
        if (needExit) {
            [self setViewControllerState:MAINVC_WAITING_EXIT];
        }
    }
}

- (void)refreshResultText {
    [self updateUpperCaseState];
    
    _mainMtx->lock();
    {
        _mainText = [[NSMutableAttributedString alloc] init];
        
        ResultList::iterator itr = _thrdData->resultList.begin();
        for(; itr != _thrdData->resultList.end(); ++itr)
        {
            UIBridgeMacUI::AppendResultToNSMutableAttributedString(*itr, _upperCaseState, _mainText);
        }
    }
    _mainMtx->unlock();
    
    [self updateMainTextView:YES];
}

- (IBAction)openButtonClicked:(NSButton *)sender {
    if (_state == MAINVC_CALC_ING) {
        [self stopHashCalc:NO];
    } else {
        [self openFiles];
    }
}

- (IBAction)clearButtonClicked:(NSButton *)sender {
    if (_state == MAINVC_VERIFY) {
        
    } else {
        [self setViewControllerState:MAINVC_NONE];
        [self updateMainTextView];
    }
}

- (IBAction)verifyButtonClicked:(NSButton *)sender {
    _tag = NSFindPanelActionShowFindPanel;
    [self.mainTextView performTextFinderAction:self];
}

- (IBAction)uppercaseButtonClicked:(NSButton *)sender {
    if (_state == MAINVC_CALC_FINISH) {
        [self refreshResultText];
    }
}

- (BOOL)textView:(NSTextView *)aTextView
   clickedOnLink:(id)link
         atIndex:(NSUInteger)charIndex {
    if (aTextView == self.mainTextView) {
        self.selectedLink = (NSString *)link;

        NSPoint nsptMouseLoc;
        nsptMouseLoc = [NSEvent mouseLocation];
        NSPoint nsptMouseInView = [self.view.window convertScreenToBase:nsptMouseLoc];

        NSMenu *nsmenuHash = [[NSMenu alloc] initWithTitle:@"HashMenu"];
        [nsmenuHash insertItemWithTitle:GetNSStringFromResByKey(MAINDLG_HYPEREDIT_MENU_COPY)
                                 action:@selector(menuCopyHash)
                          keyEquivalent:@""
                                atIndex:0];
        [nsmenuHash insertItem:[NSMenuItem separatorItem] atIndex:1];
        [nsmenuHash insertItemWithTitle:GetNSStringFromResByKey(MAINDLG_HYPEREDIT_MENU_SERACHGOOGLE)
                                 action:@selector(menuSearchGoogle)
                          keyEquivalent:@""
                                atIndex:2];
        [nsmenuHash insertItemWithTitle:GetNSStringFromResByKey(MAINDLG_HYPEREDIT_MENU_SERACHVIRUSTOTAL)
                                 action:@selector(menuSearchVirusTotal)
                          keyEquivalent:@""
                                atIndex:3];

        [nsmenuHash popUpMenuPositioningItem:nil atLocation:nsptMouseInView inView:self.view];

        
        return YES;
    }

    return NO;
}

- (void)menuCopyHash {
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
    [pasteboard setString:self.selectedLink forType:NSStringPboardType];
}

- (void)menuSearchGoogle {
    NSString *nstrUrl = [NSString stringWithFormat:@"https://www.google.com/search?q=%@&ie=utf-8&oe=utf-8", self.selectedLink];
    NSURL *url = [NSURL URLWithString:nstrUrl];
    [[NSWorkspace sharedWorkspace] openURL:url];
}

- (void)menuSearchVirusTotal {
    NSString *nstrUrl = [NSString stringWithFormat:@"https://www.virustotal.com/en/search/?query=%@", self.selectedLink];
    NSURL *url = [NSURL URLWithString:nstrUrl];
    [[NSWorkspace sharedWorkspace] openURL:url];
}

@end
