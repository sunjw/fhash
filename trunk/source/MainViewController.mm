//
//  MainViewController.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import "MainViewController.h"

@implementation MainViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    
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
    
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

@end
