//
//  LiquidGlassUtils.swift
//  fHash
//
//  Created by Sun Junwen on 2026/4/24.
//  Copyright © 2026 Sun Junwen. All rights reserved.
//

import Cocoa
import QuartzCore

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

@objc(ScrollTopEdgeGaussianBlurView) class ScrollTopEdgeGaussianBlurView: NSView {
    private let blurRadius: CGFloat = 2
    private let tintColor: NSColor = .white

    private let tintLayer = CAGradientLayer()
    private let maskLayer = CAGradientLayer()

    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
        setup()
    }

    required init?(coder: NSCoder) {
        super.init(coder: coder)
        setup()
    }

    private func setup() {
        wantsLayer = true
        layerUsesCoreImageFilters = true

        guard let layer else {
            return
        }

        layer.masksToBounds = true

        // Workaround
        layer.backgroundColor = NSColor.white.withAlphaComponent(0.001).cgColor

        updateBackdropFilter()
        setupTintLayer()
        setupMaskLayer()
    }

    private func updateBackdropFilter() {
        guard let blurFilter = makeCAFilter(name: "gaussianBlur") else {
            layer?.backgroundFilters = nil
            return
        }

        blurFilter.setValue(blurRadius, forKey: "inputRadius")
        blurFilter.setValue(true, forKey: "inputNormalizeEdges")

        layer?.backgroundFilters = [blurFilter]
    }

    private func setupTintLayer() {
        updateTintLayer()

        tintLayer.startPoint = CGPoint(x: 0.5, y: 0.0)
        tintLayer.endPoint = CGPoint(x: 0.5, y: 1.0)

        layer?.addSublayer(tintLayer)
    }

    private func updateTintLayer() {
        // With this layer setup, y=0 behaves as bottom and y=1 as top.
        tintLayer.colors = [
            tintColor.withAlphaComponent(0.0).cgColor,
            tintColor.withAlphaComponent(0.95).cgColor
        ]
    }

    private func setupMaskLayer() {
        // Bottom fades out, top remains visible.
        maskLayer.colors = [
            NSColor.clear.cgColor,
            NSColor.white.cgColor,
            NSColor.white.cgColor
        ]
        maskLayer.locations = [0.0, 0.6, 1.0]
        maskLayer.startPoint = CGPoint(x: 0.5, y: 0.0)
        maskLayer.endPoint = CGPoint(x: 0.5, y: 1.0)

        layer?.mask = maskLayer
    }

    override func layout() {
        super.layout()

        CATransaction.begin()
        CATransaction.setDisableActions(true)

        tintLayer.frame = bounds
        maskLayer.frame = bounds

        CATransaction.commit()
    }

    private func makeCAFilter(name: String) -> NSObject? {
        guard let filterClass = NSClassFromString("CAFilter") as? NSObject.Type else {
            return nil
        }

        let selector = NSSelectorFromString("filterWithName:")

        guard filterClass.responds(to: selector) else {
            return nil
        }

        return filterClass.perform(selector, with: name)?
            .takeUnretainedValue() as? NSObject
    }

}
