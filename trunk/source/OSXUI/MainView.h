//
//  MainView.h
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MainViewController.h"

@interface MainView : NSView <NSDraggingDestination>

@property (weak) MainViewController *mainViewController;

@end
