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
#include "MacUtils.h"

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
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSArray *fileNames = [pboard propertyListForType:NSFilenamesPboardType];
    
    NSUInteger fileCount = [fileNames count];
    for (NSUInteger i = 0; i < fileCount; ++i) {
        NSString *nsstrfileName = [fileNames objectAtIndex:i];
        string strFileName = MacUtils::ConvertNSStringToUTF8String(nsstrfileName);
        printf("%s\n", strFileName.c_str());
    }
    
    return YES;
}

@end
