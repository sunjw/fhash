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

@objc(ScrollTopEdgeView) class ScrollTopEdgeView: NSView {
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // let drawingRect = bounds
        // let path = NSBezierPath(rect: drawingRect)
        // NSColor.white.setFill()
        // path.fill()

        let baseColor = NSColor.textBackgroundColor
        // 0.56 -> 0.8, 0.42 -> 1, 0.455 -> 0.95, 0.49 -> 0.9, 0.525 -> 0.85
        // 0.455 -> 0.89, 0.42 - > 0.91, 0.385 -> 0.93, 0.35 -> 0.95
        let gradient = NSGradient(colors: [baseColor.withAlphaComponent(1.0),
                                           baseColor.withAlphaComponent(0.925),
                                           baseColor.withAlphaComponent(0.915),
                                           baseColor.withAlphaComponent(0.905),
                                           baseColor.withAlphaComponent(0.89),
                                           baseColor.withAlphaComponent(0.87),
                                           baseColor.withAlphaComponent(0.84),
                                           baseColor.withAlphaComponent(0.8),
                                           baseColor.withAlphaComponent(0.0)],
                                  atLocations: [0.0, 0.35, 0.385, 0.42, 0.455, 0.49, 0.525, 0.56, 1.0],
                                  colorSpace: .deviceRGB)

        gradient?.draw(in: bounds, angle: 270)
    }

}
