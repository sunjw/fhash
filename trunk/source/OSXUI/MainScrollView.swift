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

            if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
                let scrollFixY: CGFloat = 26
                if super.isFindBarVisible {
                    // show
                    mainViewController?.mainScrollViewTopConstraint.constant = 28
                    mainViewController?.mainTextView.textContainerInset = MainViewController.MainTextViewInsetWithFindBarAfter26
                    if let enclosingScrollView = mainViewController?.mainTextView.enclosingScrollView {
                        enclosingScrollView.contentView.scroll(to:NSPoint(
                            x: enclosingScrollView.contentView.bounds.origin.x,
                            y: enclosingScrollView.contentView.bounds.origin.y - scrollFixY))
                        enclosingScrollView.reflectScrolledClipView(enclosingScrollView.contentView)
                    }
                } else {
                    // hide
                    mainViewController?.mainScrollViewTopConstraint.constant = 0
                    mainViewController?.mainTextView.textContainerInset = MainViewController.MainTextViewInsetAfter26
                    if let enclosingScrollView = mainViewController?.mainTextView.enclosingScrollView {
                        var hideScrollFixY = scrollFixY
                        if enclosingScrollView.contentView.bounds.origin.y == 0 {
                            hideScrollFixY = 0
                        }
                        enclosingScrollView.contentView.scroll(to:NSPoint(
                            x: enclosingScrollView.contentView.bounds.origin.x,
                            y: enclosingScrollView.contentView.bounds.origin.y + hideScrollFixY))
                        enclosingScrollView.reflectScrolledClipView(enclosingScrollView.contentView)
                    }
                }
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
