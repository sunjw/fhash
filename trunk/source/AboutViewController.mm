//
//  AboutViewController.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 15/8/4.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//

#import "AboutViewController.h"

#include <string>
#import "MacUtils.h"

using namespace std;

@interface AboutViewController ()

@end

@implementation AboutViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Do view setup here.
    [self.iconImageView setImage:[NSApp applicationIconImage]];
    
    string strAboutInfo;
    strAboutInfo.append(GetStringFromResByKey(ABOUTDLG_INFO_TITLE));
    
    NSString *nsstrAboutInfo = MacUtils::ConvertUTF8StringToNSString(strAboutInfo);
    [self.infoTextField setStringValue:nsstrAboutInfo];
}

@end
