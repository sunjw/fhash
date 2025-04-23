//
//  MainView.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/11.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

@objc(MainView) class MainView: NSView {
    weak var mainViewController: MainViewController?

    override func awakeFromNib() {
        MainActor.assumeIsolated {
            registerForDraggedTypes([.fileURL])
        }
    }

    override func draggingEntered(_ sender: NSDraggingInfo) -> NSDragOperation {
        if mainViewController?.ableToCalcFiles() ?? false {
            return .copy
        } else {
            return []
        }
    }

    override func performDragOperation(_ sender: NSDraggingInfo) -> Bool {
        mainViewController?.performViewDragOperation(sender)
        return true
    }
}
