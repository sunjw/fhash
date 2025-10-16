//
//  MainWindowController.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

@objc(MainWindowController) class MainWindowController: NoTitlebarWindowController, NSWindowDelegate {
    override func windowDidLoad() {
        super.windowDidLoad()

        // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
        window?.delegate = self

        self.windowFrameAutosaveName = "MainWindowFrame"
    }

    func getMainViewController() -> MainViewController? {
        var mainViewController: MainViewController?
        mainViewController = window?.contentViewController as? MainViewController
        return mainViewController
    }

    func windowShouldClose(_ sender: NSWindow) -> Bool {
        let mainViewController = getMainViewController()

        if mainViewController?.isCalculating() ?? false {
            mainViewController?.stopHashCalc(true)
        }

        return !(mainViewController?.isCalculating() ?? false)
    }

    func windowWillClose(_ notification: Notification) {
        let mainViewController = getMainViewController()
        mainViewController?.mainWindowWillClose = true
    }

}
