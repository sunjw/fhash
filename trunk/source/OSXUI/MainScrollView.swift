//
//  MainScrollView.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

// NSTextView scrolls to its selection (centered) when its frame size changes or
// a live resize ends, which makes the content drift when resizing the window. Ignore it.
@objc(MainTextView) class MainTextView: NSTextView {
    private var isSettingFrameSize = false
    private var isEndingLiveResize = false

    override func setFrameSize(_ newSize: NSSize) {
        isSettingFrameSize = true
        super.setFrameSize(newSize)
        isSettingFrameSize = false
    }

    override func viewDidEndLiveResize() {
        isEndingLiveResize = true
        super.viewDidEndLiveResize()
        isEndingLiveResize = false
    }

    override func scrollToVisible(_ rect: NSRect) -> Bool {
        if isSettingFrameSize || isEndingLiveResize || (window?.inLiveResize ?? false) {
            return false
        }
        return super.scrollToVisible(rect)
    }
}

@objc(MainScrollView) class MainScrollView: NSScrollView {
    weak var mainViewController: MainViewController?
    private var scrollTopEdgeView: ScrollTopEdgeGaussianBlurView?
    private var scrollTopEdgeHeight: CGFloat = 0

    override func addSubview(_ view: NSView) {
        let viewClassName = String(describing: type(of: view))
        // NSLog("MainScrollView.addSubview [%@]", viewClassName)

        if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            // NSScrollPocket may be added multiple times
            if LiquidGlassUI.enableFakeSoftEdge() {
                let targetViewNames = ["NSScrollPocket"]
                if targetViewNames.contains(viewClassName) {
                    DispatchQueue.main.async(execute: { [view] in
                        //self.setupScrollTopEdgeView(targetView: view, height: Int(view.frame.height))
                        self.setupScrollTopEdgeView(targetView: view, height: 66)
                    })
                }
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
        scrollTopEdgeHeight = CGFloat(height)

        if scrollTopEdgeView == nil {
            scrollTopEdgeView = ScrollTopEdgeGaussianBlurView(
                frame: CGRect(x: 0, y: 0,
                              width: Int(bounds.width),
                              height: height))
        }

        guard let scrollTopEdgeView else { return }

        scrollTopEdgeView.removeFromSuperview()
        self.addSubview(scrollTopEdgeView, positioned: .below, relativeTo: targetView)

        self.updateScrollTopEdgeViewFrame()
        self.updateScrollTopEdgeViewVisible()
    }

    // NSScrollView lays out its subviews by itself, so use frame instead of constraints.
    private func updateScrollTopEdgeViewFrame() {
        guard let scrollTopEdgeView else { return }
        scrollTopEdgeView.frame = CGRect(x: 0,
                                         y: isFlipped ? 0 : bounds.height - scrollTopEdgeHeight,
                                         width: bounds.width,
                                         height: scrollTopEdgeHeight)
    }

    override func tile() {
        super.tile()
        self.updateScrollTopEdgeViewFrame()
    }

    func updateScrollTopEdgeViewVisible() {
        guard let scrollTopEdgeView, let mainViewController else { return }
        // mainClipView.bounds.origin.y = -50 is not scrolled.
        let scrolled = mainViewController.mainClipView.bounds.origin.y > -48
        let newAlpha: CGFloat = scrolled ? 1.0 : 0.0
        //NSLog("updateScrollTopEdgeViewVisible, newAlpha=%.1f", newAlpha)
        scrollTopEdgeView.alphaValue = newAlpha
    }
}
