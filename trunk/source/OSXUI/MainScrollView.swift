//
//  PaddingClipView.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

@objc(MainScrollView) class MainScrollView: NSScrollView {
    weak var mainViewController: MainViewController?

    override func addSubview(_ view: NSView) {
        if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            let blockViewsName = ["Dummy"/*, "NSScrollPocket", "BackdropView", "_NSScrollViewContentBackgroundView"*/]
            let viewClassName = String(describing: type(of: view))
            // NSLog("MainScrollView.addSubview [%@]", viewClassName)
            if blockViewsName.contains(viewClassName) {
                return
            }
        }
        super.addSubview(view)
    }

    override var isFindBarVisible: Bool {
        set {
            // NSLog("isFindBarVisible=%d", newValue)
            super.isFindBarVisible = newValue
            if super.isFindBarVisible {
                // show
                mainViewController?.mainScrollViewTopConstraint.constant = 28
                mainViewController?.mainTextView.textContainerInset = NSMakeSize(3.0, 6.0)
            } else {
                // hide
                mainViewController?.mainScrollViewTopConstraint.constant = 0
                mainViewController?.mainTextView.textContainerInset = NSMakeSize(3.0, 24.0)
            }
        }
        get {
            return super.isFindBarVisible
        }
    }

    // override func findBarViewDidChangeHeight() {
    //     guard let findBarHeight = findBarView?.frame.height else { return }
    //     NSLog("findBarHeight=%f", findBarHeight)
    // }
}
