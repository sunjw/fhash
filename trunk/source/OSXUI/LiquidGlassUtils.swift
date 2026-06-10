//
//  TitlebarOverlayView.swift
//  fHash
//
//  Created by Sun Junwen on 2026/4/24.
//  Copyright © 2026 Sun Junwen. All rights reserved.
//

import Cocoa

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
        self.translatesAutoresizingMaskIntoConstraints = false

        toView.addSubview(self)

        NSLayoutConstraint.activate([
            self.topAnchor.constraint(equalTo: toView.topAnchor),
            self.leadingAnchor.constraint(equalTo: toView.leadingAnchor),
            self.trailingAnchor.constraint(equalTo: toView.trailingAnchor),
            self.heightAnchor.constraint(equalToConstant: TitlebarOverlayView.height)
        ])
    }
}
