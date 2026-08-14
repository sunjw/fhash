//
//  AboutSwiftUIWindowController.swift
//  fHash
//
//  Created by Sun Junwen on 2026/8/14.
//  Copyright © 2026 Sun Junwen. All rights reserved.
//

import Cocoa
import SwiftUI

class AboutSwiftUIWindowController: NoTitlebarWindowController {
    static let shared = AboutSwiftUIWindowController()

    private convenience init() {
        let window = NSWindow(
            contentRect: NSRect(origin: .zero, size: AboutSwiftUIView.contentSize),
            styleMask: [.titled, .closable],
            backing: .buffered,
            defer: false)
        window.isReleasedWhenClosed = false
        window.center()
        self.init(window: window)

        // init(window:) skips nib loading, call windowDidLoad() by ourself.
        windowDidLoad()
    }

    override func windowDidLoad() {
        super.windowDidLoad()

        // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
        window?.title = MacSwiftUtils.GetStringFromRes("ABOUTDLG_TITLE")
        window?.titlebarAppearsTransparent = true

        // Prepare for NSVisualEffectView/NSGlassEffectView background
        window?.isOpaque = false
        window?.backgroundColor = .clear

        let aboutSwiftHostingController = NSHostingController(
            rootView: AboutSwiftUIView(onClose: { [weak self] in
                self?.window?.close()
            })
        )
        aboutSwiftHostingController.safeAreaRegions = []
        window?.contentViewController = aboutSwiftHostingController
    }
}
