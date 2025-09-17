//
//  PaddingClipView.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

@objc(MainScrollView) class MainScrollView: NSScrollView {
    override func addSubview(_ view: NSView) {
        let name = String(describing: type(of: view))
        if name == "NSScrollPocket" {
            return
        }
        super.addSubview(view)
    }
}
