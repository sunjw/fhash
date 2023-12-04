//
//  HyperlinkTextField.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/4.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//
//
//  HyperlinkTextField.m
//  NSTextFieldHyperlinks
//
//  Created by Toomas Vahter on 25.12.12.
//  Copyright (c) 2012 Toomas Vahter. All rights reserved.
//
//  This content is released under the MIT License (http://www.opensource.org/licenses/mit-license.php).
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  Swift version created by Sun Junwen on 2023/12/4.
//

import Cocoa

@objc(HyperlinkTextField) class HyperlinkTextField: NSTextField {
    private struct HyperlinkInfo {
        let range: NSRange
        let link: URL
        let rect: NSRect
    }

    private var hyperlinkInfos: [HyperlinkInfo] {
        var hyperlinkInfos: [HyperlinkInfo] = []
        let stringRange = NSRange(location: 0, length: attributedStringValue.length)
        let textView = self.textView
        attributedStringValue.enumerateAttribute(.link,
                                                 in: stringRange,
                                                 options: [],
                                                 using: { value, range, stop in
            guard let url = value as? NSURL else { return }

            var rects: [NSRect] = []
            if let layoutManager = textView?.layoutManager, let textContainer = textView?.textContainer {
                let glyphRange = layoutManager.glyphRange(forCharacterRange: range, actualCharacterRange: nil)
                layoutManager.enumerateEnclosingRects(forGlyphRange: glyphRange,
                                                      withinSelectedGlyphRange: glyphRange,
                                                      in: textContainer,
                                                      using: { (rect, stop) in
                    rects.append(rect)
                })
            }

            for rect in rects {
                hyperlinkInfos.append(HyperlinkInfo(range: range,
                                                    link: url as URL,
                                                    rect: rect))
            }
        })

        return hyperlinkInfos
    }

    var textView: NSTextView? {
        // Font used for displaying and frame calculations must match
        let attributedString = NSMutableAttributedString(attributedString: attributedStringValue)
        let font = attributedString.attribute(.font, at: 0, effectiveRange: nil) as? NSFont

        if font == nil {
            attributedString.addAttribute(.font, value: self.font as Any, range: NSRange(location: 0, length: attributedString.length))
        }

        let textViewFrame = cell?.titleRect(forBounds: bounds)
        let textView = NSTextView(frame: textViewFrame ?? NSRect.zero)
        textView.textStorage?.setAttributedString(attributedString)

        return textView
    }

    private func _hyperlinkTextFieldInit() {
        isEditable = false
        isSelectable = false
    }

    override init(frame: NSRect) {
        super.init(frame: frame)
        _hyperlinkTextFieldInit()
    }

    required init?(coder: NSCoder) {
        super.init(coder: coder)
        _hyperlinkTextFieldInit()
    }

    override func resetCursorRects() {
        super.resetCursorRects()
        _resetHyperlinkCursorRects()
    }

    private func _resetHyperlinkCursorRects() {
        for info in hyperlinkInfos {
            addCursorRect(info.rect, cursor: .pointingHand)
        }
    }

    // MARK: -
    // MARK: Mouse Events

    override func mouseUp(with theEvent: NSEvent) {
        let textView = self.textView
        let localPoint = convert(theEvent.locationInWindow, from: nil)
        var index: Int = NSNotFound
        if let layoutManager = textView?.layoutManager, let textContainer = textView?.textContainer {
            index = layoutManager.characterIndex(for: localPoint,
                                                 in: textContainer,
                                                 fractionOfDistanceBetweenInsertionPoints: nil)
        }

        if index != NSNotFound {
            for info in hyperlinkInfos {
                if NSLocationInRange(index, info.range) {
                    NSWorkspace.shared.open(info.link)
                }
            }
        }
    }
}
