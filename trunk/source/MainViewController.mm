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
#include "Global.h"
#include "HashEngine.h"
#include "UIStrings.h"

#import "MacUtils.h"
#import "UIBridgeMacUI.h"
#import "fHashMacAppDelegate.h"
#import "MainView.h"

using namespace std;
using namespace sunjwbase;

enum MainViewControllerState {
    MAINVC_NONE = 0,        // clear state
    MAINVC_CALC_ING,        // calculating
    MAINVC_CALC_FINISH,     // calculating finished/stopped
    MAINVC_VERIFY,          // verfing
    MAINVC_WAITING_EXIT,    // waiting thread stop and exit
};

@interface MainViewController()

@property (assign) MainViewControllerState state;
@property (assign) UIBridgeMacUI *uiBridgeMac;
@property (assign) ThreadData *thrdData;
@property (assign) pthread_t ptHash;

@end

@implementation MainViewController

@synthesize state = _state;
@synthesize mainMtx = _mainMtx;
@synthesize mainText = _mainText;
@synthesize uiBridgeMac = _uiBridgeMac;
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
    
    // alloc c++ member.
    _mainMtx = new OsMutex();
    _uiBridgeMac = new UIBridgeMacUI(self);
    _thrdData = new ThreadData();
    
    [self setViewControllerState:MAINVC_NONE];
    
    _thrdData->uiBridge = _uiBridgeMac;
    
    NSMenu *fileMenu = [self getFileMenu];
    [fileMenu setAutoenablesItems:NO];
    
    // Set open button as default.
    [self.openButton setKeyEquivalent:@"\r"];
    
    // Set scroll view border type.
    self.mainScrollView.borderType = NSNoBorder;
    
    // Set some text in text field.
    [self.mainTextView setTextContainerInset:NSMakeSize(4.0, 4.0)];
    [self.mainTextView setFont:[NSFont fontWithName:@"Monaco" size:12]];
    // nowrap.
    [[self.mainTextView enclosingScrollView] setHasHorizontalScroller:YES];
    [self.mainTextView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
    [self.mainTextView setHorizontallyResizable:YES];
    [self.mainTextView setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
    [[self.mainTextView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
    [[self.mainTextView textContainer] setWidthTracksTextView:NO];
    
    // Set checkbox.
    [self.upperCaseButton setState:NSOffState];
    
    // Update main text.
    [self updateMainTextView];
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
                _mainText = [[NSMutableString alloc] init];
                string strAppend = MacUtils::GetStringFromRes(MAINDLG_INITINFO);
                strAppend.append("\n\n");
                NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
                
                [_mainText appendString:nsstrAppend];
            }
            _mainMtx->unlock();
            
            [self.mainProgressIndicator setDoubleValue:0];
         
            // Passthrough to MAINVC_CALC_FINISH.
        }
        case MAINVC_CALC_FINISH: {
            // Set controls title.
            NSMenuItem *openMenuItem = [self getOpenMenuItem];
            [openMenuItem setEnabled:YES];
            
            [self.openButton
             setTitle:MacUtils::GetNSStringFromRes(MAINDLG_OPEN)];
            [self.clearButton
             setTitle:MacUtils::GetNSStringFromRes(MAINDLG_CLEAR)];
            [self.clearButton setEnabled:YES];
            [self.verifyButton
             setTitle:MacUtils::GetNSStringFromRes(MAINDLG_VERIFY)];
            [self.verifyButton setEnabled:YES];
            [self.upperCaseButton
             setTitle:MacUtils::GetNSStringFromRes(MAINDLG_UPPER_HASH)];
            [self.upperCaseButton setEnabled:YES];
            
        } break;
        case MAINVC_CALC_ING: {
            _thrdData->stop = false;
            
            NSMenuItem *openMenuItem = [self getOpenMenuItem];
            [openMenuItem setEnabled:NO];
            
            [self.openButton
             setTitle:MacUtils::GetNSStringFromRes(MAINDLG_STOP)];
            [self.clearButton setEnabled:NO];
            [self.verifyButton setEnabled:NO];
            [self.upperCaseButton setEnabled:NO];
            
        } break;
        case MAINVC_VERIFY: {
            
        } break;
        case MAINVC_WAITING_EXIT: {
            
        } break;
    }
    
    _state = newState;
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
    return !(_state == MAINVC_CALC_ING ||
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

- (void)updateMainTextView {
    _mainMtx->lock();
    [self.mainTextView setString:_mainText];
    _mainMtx->unlock();
    
    // Scroll to end.
    [self.mainTextView
     scrollRangeToVisible:NSMakeRange(self.mainTextView.string.length, 0)];
}

- (void)calculateFinished {
    [self setViewControllerState:MAINVC_CALC_FINISH];
    [self.mainProgressIndicator setDoubleValue:100];
}

- (void)calculateStopped {
    /*string strAppend = "\n";
    strAppend.append(MacUtils::GetStringFromRes(MAINDLG_CALCU_TERMINAL));
    strAppend.append("\n\n");
    NSString *nsstrAppend = MacUtils::ConvertUTF8StringToNSString(strAppend);
    
    _mainMtx->lock();
    [_mainText appendString:nsstrAppend];
    _mainMtx->unlock();*/
    
    [self setViewControllerState:MAINVC_CALC_FINISH];
    [self.mainProgressIndicator setDoubleValue:0];
    
    //[self updateMainTextView];
}

- (void)startHashCalc:(NSArray *)fileNames isURL:(BOOL)isURL {
    if (![self ableToCalcFiles]) {
        return;
    }
    
    if (_state == MAINVC_NONE) {
        // Clear up text.
        _mainMtx->lock();
        _mainText = [[NSMutableString alloc] init];
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
    _thrdData->uppercase = ([self.upperCaseButton state] == NSOnState);
    
    // Ready to go.
    [self setViewControllerState:MAINVC_CALC_ING];
    
    pthread_create(&_ptHash,
                   NULL,
                   (void *(*)(void *))HashThreadFunc,
                   _thrdData);

}

- (void)stopHashCalc {
    if (_state == MAINVC_CALC_ING) {
        _thrdData->stop = true;
    }
}

- (IBAction)openButtonClicked:(NSButton *)sender {
    if (_state == MAINVC_CALC_ING) {
        [self stopHashCalc];
    } else {
        [self openFiles];
    }
}

- (IBAction)clearButtonClicked:(NSButton *)sender {
    if (_state == MAINVC_VERIFY) {
        
    } else {
        [self setViewControllerState:MAINVC_NONE];
        [self.mainProgressIndicator setDoubleValue:0];
        
        [self updateMainTextView];
    }
}

@end
