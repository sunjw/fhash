//
//  AboutViewController.mm
//  fHashMacUI
//
//  Created by Sun Junwen on 15/8/4.
//  Copyright (c) 2015 Sun Junwen. All rights reserved.
//

#import "AboutViewController.h"

#include <string>
#include "strhelper.h"
#import "MacUtils.h"

using namespace std;
using namespace sunjwbase;

@interface AboutViewController ()

@end

@implementation AboutViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Do view setup here.
    [self.iconImageView setImage:[NSApp applicationIconImage]];
    
    NSString *nsstrAppVersion = [[[NSBundle mainBundle] infoDictionary]
                                 objectForKey:@"CFBundleShortVersionString"];
    
    NSString *nsstrAppBundleVersion = [[[NSBundle mainBundle] infoDictionary]
                                       objectForKey:@"CFBundleVersion"];
    
    string strAboutInfo;
    
    strAboutInfo.append(GetStringFromResByKey(ABOUTDLG_INFO_TITLE));
    strAboutInfo.append(MacUtils::ConvertNSStringToUTF8String(nsstrAppVersion));
    strAboutInfo.append(" (");
    strAboutInfo.append(MacUtils::ConvertNSStringToUTF8String(nsstrAppBundleVersion));
    strAboutInfo.append(")");
    strAboutInfo.append("\n");
    strAboutInfo.append(GetStringFromResByKey(ABOUTDLG_INFO_RIGHT));
    strAboutInfo.append("\n");
    strAboutInfo.append("\n");
    strAboutInfo.append(GetStringFromResByKey(ABOUTDLG_INFO_MD5));
    strAboutInfo.append("\n");
    strAboutInfo.append(GetStringFromResByKey(ABOUTDLG_INFO_SHA256));
    strAboutInfo.append("\n");
    strAboutInfo.append(GetStringFromResByKey(ABOUTDLG_INFO_CRC32));
    
    NSString *nsstrAboutInfo = MacUtils::ConvertUTF8StringToNSString(strAboutInfo);
    [self.infoTextField setStringValue:nsstrAboutInfo];
    
    // Set homepage.
    string strLinkText = GetStringFromResByKey(ABOUTDLG_PROJECT_SITE);
    strLinkText = strreplace(strLinkText, "<a>", "");
    strLinkText = strreplace(strLinkText, "</a>", "");
    NSString *linkText = MacUtils::ConvertUTF8StringToNSString(strLinkText);
    NSURL *url = [NSURL URLWithString:GetNSStringFromResByKey(ABOUTDLG_PROJECT_URL)];
    NSMutableAttributedString *hyperlinkString = [[NSMutableAttributedString alloc]
                                                  initWithString:linkText];
    [hyperlinkString beginEditing];
    [hyperlinkString addAttribute:NSLinkAttributeName
                            value:url
                            range:NSMakeRange(0, [hyperlinkString length])];
    [hyperlinkString addAttribute:NSForegroundColorAttributeName
                            value:[NSColor blueColor]
                            range:NSMakeRange(0, [hyperlinkString length])];
    [hyperlinkString endEditing];

    [self.homePageLinkTextField setAttributedStringValue:hyperlinkString];
    
    [self.closeButton setKeyEquivalent:@"\r"];
    [self.closeButton setTitle:GetNSStringFromResByKey(BUTTON_OK)];
}

- (IBAction)closeButtonClicked:(NSButton *)sender {
    [self.view.window close];
}

@end
