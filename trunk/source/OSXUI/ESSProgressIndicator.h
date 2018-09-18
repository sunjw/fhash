//
//  ESSProgressIndicator.h
//  Briefly
//
//  Created by Matthias Gansrigler on 06.05.2015.
//  Copyright (c) 2015 Eternal Storms Software. All rights reserved.
//
//  Edited by Sun Junwen on 08.15.2015.
//

#import <Cocoa/Cocoa.h>

@interface ESSProgressIndicator : NSProgressIndicator

- (BOOL)isAnimateDisabled;

- (void)jumpToDoubleValue:(double)val;

- (void)animateToDoubleValue:(double)val;

@end
