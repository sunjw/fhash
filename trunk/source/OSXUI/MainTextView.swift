//
//  MainTextView.swift
//  fHash
//
//  Created by Sun Junwen on 2026/10/26.
//  Copyright © 2026 Sun Junwen. All rights reserved.
//

import Cocoa

// NSTextView scrolls to its selection (centered) when its frame size changes or
// a live resize ends, which makes the content drift when resizing the window. Ignore it.
@objc(MainTextView) class MainTextView: NSTextView {
    private var isSettingFrameSize = false
    private var isEndingLiveResize = false

    override func setFrameSize(_ newSize: NSSize) {
        guard LiquidGlassUI.enableWindowResize() else {
            super.setFrameSize(newSize)
            return
        }

        isSettingFrameSize = true
        super.setFrameSize(newSize)
        isSettingFrameSize = false
    }

    override func viewDidEndLiveResize() {
        guard LiquidGlassUI.enableWindowResize() else {
            super.viewDidEndLiveResize()
            return
        }

        isEndingLiveResize = true
        super.viewDidEndLiveResize()
        isEndingLiveResize = false
    }

    override func scrollToVisible(_ rect: NSRect) -> Bool {
        if LiquidGlassUI.enableWindowResize() &&
            (isSettingFrameSize || isEndingLiveResize || (window?.inLiveResize ?? false)) {
            return false
        }
        return super.scrollToVisible(rect)
    }
}
