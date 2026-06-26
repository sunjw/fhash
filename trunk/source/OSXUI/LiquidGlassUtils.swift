//
//  LiquidGlassUtils.swift
//  fHash
//
//  Created by Sun Junwen on 2026/4/24.
//  Copyright © 2026 Sun Junwen. All rights reserved.
//

import Cocoa

public enum LiquidGlassUI {
    private static let forceNo = false

    static func enableLargeRounded() -> Bool {
        if (forceNo || MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            return false
        }
        return true
    }
}

@objc(TitlebarOverlayView) class TitlebarOverlayView: NSView {
    static let height: CGFloat = 48

    override var isOpaque: Bool {
        false
    }

    override var mouseDownCanMoveWindow: Bool {
        true
    }

    override func resetCursorRects() {
        discardCursorRects()
        addCursorRect(bounds, cursor: .arrow)
    }

    func setupTitlebarOverlay(_ toView: MainView) {
        translatesAutoresizingMaskIntoConstraints = false

        toView.addSubview(self)

        NSLayoutConstraint.activate([
            topAnchor.constraint(equalTo: toView.topAnchor),
            leadingAnchor.constraint(equalTo: toView.leadingAnchor),
            trailingAnchor.constraint(equalTo: toView.trailingAnchor),
            heightAnchor.constraint(equalToConstant: TitlebarOverlayView.height)
        ])
    }
}
