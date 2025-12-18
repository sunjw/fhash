//
//  TitlebarView.swift
//  fHash
//
//  Created by Sun Junwen on 2025/12/18.
//  Copyright © 2025 Sun Junwen. All rights reserved.
//

import Cocoa

@objc(TitlebarView) class TitlebarView: NSView {
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        let startColor = NSColor.textBackgroundColor.withAlphaComponent(1.0)
        let midColor = NSColor.textBackgroundColor.withAlphaComponent(0.8)
        let endColor = NSColor.textBackgroundColor.withAlphaComponent(0.0)

        let gradient = NSGradient(colors: [startColor, midColor, endColor],
                                          atLocations: [0.0, 0.75, 1.0],
                                          colorSpace: .deviceRGB)

        gradient?.draw(in: bounds, angle: 270)
    }

}
