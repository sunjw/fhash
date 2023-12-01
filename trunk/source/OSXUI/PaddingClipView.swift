//
//  PaddingClipView.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

let CLIPVIEW_PADDING_TOP = 22

@objc(PaddingClipView) class PaddingClipView: NSClipView {
    override func setFrameOrigin(_ newOrigin: NSPoint) {
        super.setFrameOrigin(NSPoint(x: 0, y: CGFloat(CLIPVIEW_PADDING_TOP)))
    }

    override func setFrameSize(_ newSize: NSSize) {
        let sizeFixed = NSMakeSize(newSize.width, newSize.height - CGFloat(CLIPVIEW_PADDING_TOP))
        super.setFrameSize(sizeFixed)
    }
}
