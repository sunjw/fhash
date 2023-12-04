//
//  AboutWindowController.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/4.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

class AboutWindowController: NSWindowController {
    override func windowDidLoad() {
        super.windowDidLoad()

        // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
        window?.title = MacSwiftUtils.GetStringFromRes("ABOUTDLG_TITLE")
        //window?.backgroundColor = .textBackgroundColor
    }
}
