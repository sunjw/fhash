//
//  HyperlinkTextField.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/4.
//  Copyright © 2023 Sun Junwen. All rights reserved.
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
