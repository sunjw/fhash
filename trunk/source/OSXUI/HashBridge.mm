//
//  HashBridge.mm
//  fHash
//
//  Created by Sun Junwen on 2023/12/7.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

#import "HashBridge.h"

#include <stdint.h>
#include <pthread.h>
#include <string>
#include "Common/Global.h"
#include "Common/HashEngine.h"

#import <Cocoa/Cocoa.h>
#import "fHash-Swift.h"
#import "UIBridgeMacSwift.h"

using namespace std;

@interface HashBridge()

@property (assign) UIBridgeMacSwift *uiBridgeSwift;
@property (assign) ThreadData *thrdData;
@property (assign) pthread_t ptHash;

@end

@implementation HashBridge

@synthesize uiBridgeSwift = _uiBridgeSwift;
@synthesize thrdData = _thrdData;
@synthesize ptHash = _ptHash;

// Not be called on exit.
// Just for sure.
- (void)dealloc {
    delete _uiBridgeSwift;
    delete _thrdData;
}

- (instancetype)initWithController:(MainViewController *)mainViewController {
    self = [super init];
    if (self) {
        _uiBridgeSwift = new UIBridgeMacSwift(mainViewController);
        _thrdData = new ThreadData();
    }
    return self;
}

- (void)didLoad {
    _thrdData->uiBridge = _uiBridgeSwift;
}

- (void)clear {
    _thrdData->threadWorking = false;
    _thrdData->stop = false;

    _thrdData->uppercase = false;
    _thrdData->totalSize = 0;

    _thrdData->nFiles = 0;
    _thrdData->fullPaths.clear();

    _thrdData->resultList.clear();
}

- (void)setStop:(bool)val {
    _thrdData->stop = val;
}

@end
