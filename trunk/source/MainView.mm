//
//  MainView.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 7/15/15.
//  Copyright © 2015 Sun Junwen. All rights reserved.
//

#import "MainView.h"

#include <stdio.h>
#include <string>

#import "MacUtils.h"

using namespace std;

@implementation MainView

- (void)awakeFromNib {
    [self registerForDraggedTypes:
     [NSArray arrayWithObjects:NSFilenamesPboardType, nil]];
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender {
    return NSDragOperationCopy;
}

- (BOOL)performDragOperation:(id<NSDraggingInfo>)sender {
    [self.mainViewController performViewDragOperation:sender];
    
    return YES;
}

@end
