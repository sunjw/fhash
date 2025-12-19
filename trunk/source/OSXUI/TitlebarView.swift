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

        let baseColor = NSColor.textBackgroundColor
        let gradient = NSGradient(colors: [baseColor.withAlphaComponent(1.0),
                                           baseColor.withAlphaComponent(0.8),
                                           baseColor.withAlphaComponent(0.0)],
                                  atLocations: [0.0, 0.5, 1.0],
                                  colorSpace: .deviceRGB)

        gradient?.draw(in: bounds, angle: 270)
    }

}
