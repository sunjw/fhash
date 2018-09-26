//
//  ESSProgressIndicator.m
//  Briefly
//
//  Created by Matthias Gansrigler on 06.05.2015.
//  Copyright (c) 2015 Eternal Storms Software. All rights reserved.
//
//  Edited by Sun Junwen on 08.15.2015.
//

#import "ESSProgressIndicator.h"

@interface ESSProgressBarAnimation : NSAnimation

@property (strong) ESSProgressIndicator *progInd;
@property (assign) double initValue;
@property (assign) double newValue;

- (instancetype)initWithProgressBar:(NSProgressIndicator *)ind
					 newDoubleValue:(double)val;

@end

@implementation ESSProgressBarAnimation

- (instancetype)initWithProgressBar:(ESSProgressIndicator *)ind
					 newDoubleValue:(double)val
{
    
    
	if (self = [super initWithDuration:0.3 animationCurve:NSAnimationLinear])
	{
		self.progInd = ind;
        self.initValue = self.progInd.doubleValue;
		self.newValue = val;
		self.animationBlockingMode = NSAnimationNonblockingThreaded;
		return self;
	}
	
	return nil;
}

- (void)setCurrentProgress:(NSAnimationProgress)currentProgress
{
	[super setCurrentProgress:currentProgress];
	
	double delta = self.newValue - self.progInd.doubleValue;

    if (!self.progInd.animateDisabled &&
        (delta * (self.newValue - self.initValue)) > 0) {
        double newDoubleValue = self.progInd.doubleValue + (delta * self.currentValue);
        self.progInd.doubleValue = newDoubleValue;
    }
}

@end


static ESSProgressBarAnimation *sAnim = nil;

@interface ESSProgressIndicator ()

@end

@implementation ESSProgressIndicator

- (void)cleanOldAnimation
{
    if (sAnim != nil) {
        double oldToValue = sAnim.newValue;
        [sAnim stopAnimation];
        sAnim = nil;
        self.doubleValue = oldToValue;
    }

}

- (void)jumpToDoubleValue:(double)val
{
    [self cleanOldAnimation];
    self.animateDisabled = YES;
    self.toValue = val;
    [super setDoubleValue:self.toValue];
}

- (void)animateToDoubleValue:(double)val
{
    [self cleanOldAnimation];
    self.animateDisabled = NO;
    self.toValue = val;
    sAnim = [[ESSProgressBarAnimation alloc] initWithProgressBar:self
                                                  newDoubleValue:self.toValue];
    [sAnim startAnimation];
}

@end
