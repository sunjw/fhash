//
//  MainScrollView.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

@objc(MainScrollView) class MainScrollView: NSScrollView {
    weak var mainViewController: MainViewController?

    private var titleViewAdded = false

    override func addSubview(_ view: NSView) {
        if (!MacSwiftUtils.IsSystemEarlierThan(26, 0) && !titleViewAdded) {
            let targetViewNames = ["NSScrollPocket"]
            let viewClassName = String(describing: type(of: view))
            // NSLog("MainScrollView.addSubview [%@]", viewClassName)
            if targetViewNames.contains(viewClassName) {
                titleViewAdded = true
                DispatchQueue.main.async(execute: {
                    self.setupTitlebarView(view)
                })
            }
        }
        super.addSubview(view)
    }

    override var isFindBarVisible: Bool {
        set {
            // NSLog("isFindBarVisible=%d", newValue)
            super.isFindBarVisible = newValue
            mainViewController?.findPanelVisibleChange(isVisible: super.isFindBarVisible)
        }
        get {
            return super.isFindBarVisible
        }
    }

    private func setupTitlebarView(_ targetView: NSView) {
        let titlebarViewHeight = 28
        let titlebarView = TitlebarView(frame: CGRect(x: 0,
                                                         y: 0,
                                                         width: Int(bounds.width),
                                                         height: titlebarViewHeight))
        titlebarView.autoresizingMask = [.width]

        self.addSubview(titlebarView, positioned: .below, relativeTo: targetView)

        NSLayoutConstraint.activate([
            titlebarView.topAnchor.constraint(equalTo: topAnchor, constant: 0),
            titlebarView.leadingAnchor.constraint(equalTo: leadingAnchor),
            titlebarView.trailingAnchor.constraint(equalTo: trailingAnchor),
            titlebarView.heightAnchor.constraint(equalToConstant: CGFloat(titlebarViewHeight))
        ])
    }
}
