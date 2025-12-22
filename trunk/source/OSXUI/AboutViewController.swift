//
//  AboutViewController.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/5.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

@objc(AboutViewController) class AboutViewController: NSViewController {
    @IBOutlet weak var mainView: NSView!
    @IBOutlet weak var iconImageView: NSImageView!
    @IBOutlet weak var infoTextField: NSTextField!
    @IBOutlet weak var homePageLinkTextField: HyperlinkTextField!
    @IBOutlet weak var closeButton: NSButton!

    override func viewDidLoad() {
        super.viewDidLoad()

        // Setup NSVisualEffectView/NSGlassEffectView background.
        _ = MacSwiftUtils.SetupEffectViewBackground(mainView)

        // Do view setup here.
        iconImageView.image = NSApp.applicationIconImage

        let strAppVersion = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String
        let strAppBundleVersion = Bundle.main.infoDictionary?["CFBundleVersion"] as? String

        var strAboutInfo = ""
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_TITLE")
        strAboutInfo += " "
        strAboutInfo += (strAppVersion ?? "")
        strAboutInfo += " ("
        strAboutInfo += (strAppBundleVersion ?? "")
        strAboutInfo += ")"
        strAboutInfo += "\n"
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_RIGHT")
        strAboutInfo += "\n"
        strAboutInfo += "\n"
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_MD5")
        strAboutInfo += "\n"
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_SHA256")
        strAboutInfo += "\n"
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_SHA512")
        infoTextField.stringValue = strAboutInfo

        // Set homepage.
        var strLinkText = MacSwiftUtils.GetStringFromRes("ABOUTDLG_PROJECT_SITE")
        strLinkText = strLinkText.replacingOccurrences(of: "<a>", with: "")
        strLinkText = strLinkText.replacingOccurrences(of:"</a>", with: "")
        let url = URL(string: MacSwiftUtils.GetStringFromRes("ABOUTDLG_PROJECT_URL"))!
        let hyperlinkString = NSMutableAttributedString(
            string: strLinkText)
        hyperlinkString.beginEditing()
        hyperlinkString.addAttribute(
            .link,
            value: url,
            range: NSRange(location: 0, length: hyperlinkString.length))
        hyperlinkString.endEditing()

        homePageLinkTextField.attributedStringValue = hyperlinkString

        closeButton.keyEquivalent = "\r"
        closeButton.title = MacSwiftUtils.GetStringFromRes("BUTTON_OK")
        if (MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            closeButton.controlSize = .regular
        }
    }

    @IBAction func closeButtonClicked(_ sender: NSButton) {
        view.window?.close()
    }
}

