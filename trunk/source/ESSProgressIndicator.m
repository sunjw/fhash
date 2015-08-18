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

@implementation ESSProgressIndicator

- (void)animateToDoubleValue:(double)val
{
	ESSProgressBarAnimation *anim = [[ESSProgressBarAnimation alloc] initWithProgressBar:self
																		  newDoubleValue:val];
	[anim startAnimation];
}

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
        self.progInd.doubleValue = self.progInd.doubleValue + (delta * currentProgress);
    }
}

@end