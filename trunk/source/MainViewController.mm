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
#import "MainView.h"
#import "UIBridgeMacUI.h"

using namespace std;
using namespace sunjwbase;

enum MainViewControllerState {
    MAINVC_NONE = 0,    // clear state
    MAINVC_CALC_ING,    // calculating
    MAINVC_CALC_FINISH, // calculating finished/stopped
    MAINVC_VERIFY,      // verfing
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
    // Initiate.
    MainView *mainView = (MainView *)[self view];
    mainView.mainViewController = self;
    
    // alloc c++ member.
    _mainMtx = new OsMutex();
    _uiBridgeMac = new UIBridgeMacUI(self);
    _thrdData = new ThreadData();
    
    [self setViewControllerState:MAINVC_NONE];
    
    _thrdData->uiBridge = _uiBridgeMac;
    
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
    
    [self updateMainTextView];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)setViewControllerState:(MainViewControllerState)newState {
    switch (newState) {
        case MAINVC_NONE: {
            // clear all.
            _thrdData->threadWorking = false;
            _thrdData->stop = false;
            
            _thrdData->uppercase = false;
            _thrdData->nFiles = 0;
            _thrdData->totalSize = 0;
            
            _thrdData->fullPaths.clear();
            _thrdData->resultList.clear();
            
            _mainText = [[NSMutableString alloc] init];
            
            [self.mainProgressIndicator setDoubleValue:0];
            
        } break;
        case MAINVC_CALC_ING: {
            _thrdData->stop = false;
            
        } break;
        case MAINVC_CALC_FINISH: {
            
        } break;
        case MAINVC_VERIFY: {
            
        } break;
    }
    
    _state = newState;
}

- (void)performViewDragOperation:(id<NSDraggingInfo>)sender {
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *fileNames = [pboard propertyListForType:NSFilenamesPboardType];
    
    [self startHashCalc:fileNames];
}

- (void)updateMainTextView {
    _mainMtx->lock();
    [self.mainTextView setString:_mainText];
    _mainMtx->unlock();
    
    // Scroll to end.
    [self.mainTextView
     scrollRangeToVisible:NSMakeRange(self.mainTextView.string.length, 0)];
}

- (void)startHashCalc:(NSArray *)fileNames {
    // Get files path.
    NSUInteger fileCount = [fileNames count];
    _thrdData->nFiles = (uint32_t)fileCount;
    _thrdData->fullPaths.clear();
    
    for (uint32_t i = 0; i < _thrdData->nFiles; ++i) {
        NSString *nsstrfileName = [fileNames objectAtIndex:i];
        string strFileName = MacUtils::ConvertNSStringToUTF8String(nsstrfileName);
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

@end
