//
//  PaddingClipView.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

private let CLIPVIEW_PADDING_TOP = 22

@objc(PaddingClipView) class PaddingClipView: NSClipView {
    weak var mainViewController: MainViewController?

    override func setFrameOrigin(_ newOrigin: NSPoint) {
        if (MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            super.setFrameOrigin(NSPoint(x: 0, y: CGFloat(CLIPVIEW_PADDING_TOP)))
        } else {
            super.setFrameOrigin(newOrigin)
        }
    }

    override func setFrameSize(_ newSize: NSSize) {
        var sizeFixed = newSize
        if (MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            sizeFixed = NSMakeSize(newSize.width, newSize.height - CGFloat(CLIPVIEW_PADDING_TOP))
        }
        super.setFrameSize(sizeFixed)

        mainViewController?.clipViewSizeChange()
    }
}
