//
//  AboutSwiftUIWindowController.swift
//  fHash
//
//  Created by Sun Junwen on 2026/8/14.
//  Copyright © 2026 Sun Junwen. All rights reserved.
//

import Cocoa

class AboutSwiftUIWindowController: NoTitlebarWindowController {
    override func windowDidLoad() {
        super.windowDidLoad()

        // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
        window?.title = MacSwiftUtils.GetStringFromRes("ABOUTDLG_TITLE")
        window?.titlebarAppearsTransparent = true

        // Prepare for NSVisualEffectView/NSGlassEffectView background
        window?.isOpaque = false
        window?.backgroundColor = .clear
    }
}
