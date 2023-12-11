//
//  fHashMacAppDelegate.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/11.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

@NSApplicationMain
class fHashMacAppDelegate: NSObject, NSApplicationDelegate {
    weak var mainViewController: MainViewController?

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application

        NSApp.servicesProvider = self
        NSUpdateDynamicServices()
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func applicationShouldTerminate(_ sender: NSApplication) -> NSApplication.TerminateReply {
        if mainViewController?.isCalculating() ?? false {
            mainViewController?.stopHashCalc(true)
            return .terminateCancel
        } else {
            return .terminateNow
        }
    }

    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true
    }

    func application(_ sender: NSApplication, openFiles filenames: [String]) {
        mainViewController?.startHashCalc(filenames, isURL: false)

        sender.reply(toOpenOrPrint: .success)
    }

    func handleFinderContextOpen(_ pboard: NSPasteboard?,
                                 userData: String?,
                                 error: String?) {
        if let fileNames = pboard?.readObjects(forClasses: [NSURL.self], options: [:]) {
            mainViewController?.startHashCalc(fileNames, isURL: true)
        }
    }

    @IBAction func openDocument(_ sender: Any) {
        // Menu bar "open"
        mainViewController?.openFiles()
    }
}
