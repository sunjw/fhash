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
        DockProgress.resetProgress()

        // Save NSUserDefaults.
        let defaultUpperCase = (mainViewController?.upperCaseButton.state == .on)
        UserDefaults.standard.set(
            defaultUpperCase,
            forKey: UPPERCASE_DEFAULT_KEY)

        // Close other windows.
        let windows = NSApp.windows
        let windowCount = windows.count
        for i in 0..<windowCount {
            let window = windows[i]
            let windowController = window.windowController
            if windowController == nil || !(windowController is MainWindowController) {
                // It looks like about panel or finder overlay window.
                // They will stop us exit, let's close them.
                window.close()
            }
        }
    }

}
