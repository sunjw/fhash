//
//  PaddingClipView.m
//  fHash
//
//  Created by Sun Junwen on 2018/8/14.
//  Copyright © 2018 Sun Junwen. All rights reserved.
//

#import "PaddingClipView.h"

const int CLIPVIEW_PADDING_TOP = 22;

@implementation PaddingClipView

- (void)setFrameOrigin:(NSPoint)newOrigin {
    [super setFrameOrigin:NSMakePoint(0, CLIPVIEW_PADDING_TOP)];
}

@end
