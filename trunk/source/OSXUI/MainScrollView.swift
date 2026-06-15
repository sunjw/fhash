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
    private var scrollTopEdgeView: ScrollTopEdgeGaussianBlurView?

    override func addSubview(_ view: NSView) {
        let viewClassName = String(describing: type(of: view))
        // NSLog("MainScrollView.addSubview [%@]", viewClassName)

        if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            // NSScrollPocket may be added multiple times
            let targetViewNames = ["NSScrollPocket"]
            if targetViewNames.contains(viewClassName) {
                DispatchQueue.main.async(execute: { [view] in
                    self.setupScrollTopEdgeView(targetView: view, height: Int(view.frame.height))
                })
            }
        }

        // Try to intercept FindBar
        let targetViewNames = ["NSTextFinderBarView"]
        if targetViewNames.contains(viewClassName) {
            let findPatternSearchField = MacSwiftUtils.FindFirstViewFrom(view,
                                                                        withClassName: "NSFindPatternSearchField")
            if let findPatternSearchField,
               let findTextField = findPatternSearchField as? NSSearchField {
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

    private func setupScrollTopEdgeView(targetView: NSView, height: Int) {
        if scrollTopEdgeView == nil {
            scrollTopEdgeView = ScrollTopEdgeGaussianBlurView(
                frame: CGRect(x: 0, y: 0,
                              width: Int(bounds.width),
                              height: height))
            scrollTopEdgeView?.autoresizingMask = [.width]
        }

        guard let scrollTopEdgeView else { return }

        scrollTopEdgeView.removeFromSuperview()
        self.addSubview(scrollTopEdgeView, positioned: .below, relativeTo: targetView)

        NSLayoutConstraint.activate([
            scrollTopEdgeView.topAnchor.constraint(equalTo: topAnchor, constant: 0),
            scrollTopEdgeView.leadingAnchor.constraint(equalTo: leadingAnchor),
            scrollTopEdgeView.trailingAnchor.constraint(equalTo: trailingAnchor),
            scrollTopEdgeView.heightAnchor.constraint(equalToConstant: CGFloat(height))
        ])
    }
}
