//
//  MainViewController.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import "MainViewController.h"

#include <stdio.h>
#include <string>
#include "UIStrings.h"
#include "strhelper.h"

#import "MacUtils.h"
#import "MainView.h"

using namespace std;
using namespace sunjwbase;

@implementation MainViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    string strTest = SHELL_EXT_ITEM_ZH_CN;
    strTest = utf8conv(strTest);
    printf("%s\n", strTest.c_str());
    // Save ourself into view.
    MainView *mainView = (MainView *)[self view];
    mainView.mainViewController = self;
    
    // Set open button as default.
    [self.openButton setKeyEquivalent:@"\r"];
    
    // Set scroll view border type.
    self.mainScrollView.borderType = NSNoBorder;
    
    // Set some text in text field.
    [self.mainTextView setTextContainerInset:NSMakeSize(4.0, 4.0)];
    [self.mainTextView setFont:[NSFont fontWithName:@"Monaco" size:12]];
    [self.mainTextView setString:@"sdfasdf\n"
     "sdfasdf\n"
     "sadfasdfas\n"
     "sdfasf\n"
     "sdfadsfasdfdfasf\n"
     "sdfasdfasfdfs\n\n\n\n\n\n"
     "sdfasfddasfasdfasf\n\n\n\n\n"
     "sdfasdfasfasdf\n\n\n\n"
     "iouwerieuytituoeto"];
    
    
    // Set progress line.
    [self.mainProgressIndicator setDoubleValue:50.0];
    
    // Set checkbox.
    [self.upperCaseButton setState:NSOffState];
    
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)performViewDragOperation:(id<NSDraggingInfo>)sender {
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *fileNames = [pboard propertyListForType:NSFilenamesPboardType];
    
    NSUInteger fileCount = [fileNames count];
    for (NSUInteger i = 0; i < fileCount; ++i) {
        NSString *nsstrfileName = [fileNames objectAtIndex:i];
        string strFileName = MacUtils::ConvertNSStringToUTF8String(nsstrfileName);
        printf("%s\n", strFileName.c_str());
    }
    
    
}

@end
