//
//  MainScrollView.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

@objc(MainScrollView) class MainScrollView: NSScrollView {
    static let TitlebarViewHeight = 32

    weak var mainViewController: MainViewController?
    private var titlebarView: TitlebarView?

    override func addSubview(_ view: NSView) {
        let viewClassName = String(describing: type(of: view))
        // NSLog("MainScrollView.addSubview [%@]", viewClassName)

        if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            // NSScrollPocket may be added multiple times
            let targetViewNames = ["Dummy", /*"NSScrollPocket"*/]
            if targetViewNames.contains(viewClassName) {
                DispatchQueue.main.async(execute: { [view] in
                    self.setupTitlebarView(view)
                })
            }
        }

        // Try to intercept FindBar
        let targetViewNames = ["NSTextFinderBarView"]
        if targetViewNames.contains(viewClassName) {
            let findPatternSearchField = MacSwiftUtils.FindFirstViewFrom(view,
                                                                        withClassName: "NSFindPatternSearchField")
            if let findPatternSearchField,
               let findTextField = findPatternSearchField as? NSTextField {
                findTextField.delegate = mainViewController
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
        if titlebarView == nil {
            titlebarView = TitlebarView(frame: CGRect(x: 0,
                                                      y: 0,
                                                      width: Int(bounds.width),
                                                      height: MainScrollView.TitlebarViewHeight))
            titlebarView?.autoresizingMask = [.width]
        }

        guard let titlebarView else { return }

        titlebarView.removeFromSuperview()
        self.addSubview(titlebarView, positioned: .below, relativeTo: targetView)

        NSLayoutConstraint.activate([
            titlebarView.topAnchor.constraint(equalTo: topAnchor, constant: 0),
            titlebarView.leadingAnchor.constraint(equalTo: leadingAnchor),
            titlebarView.trailingAnchor.constraint(equalTo: trailingAnchor),
            titlebarView.heightAnchor.constraint(equalToConstant: CGFloat(MainScrollView.TitlebarViewHeight))
        ])
    }
}
