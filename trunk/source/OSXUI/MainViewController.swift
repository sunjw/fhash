//
//  MainViewController.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/6.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Cocoa

let UPPERCASE_DEFAULT_KEY = "upperCaseKey"

private struct MainViewControllerState: OptionSet {
    let rawValue: Int

    static let NONE = MainViewControllerState(rawValue: 1 << 0) // clear state
    static let CALC_ING = MainViewControllerState(rawValue: 1 << 1) // calculating
    static let CALC_FINISH = MainViewControllerState(rawValue: 1 << 2) // calculating finished/stopped
    static let VERIFY = MainViewControllerState(rawValue: 1 << 3) // verfing
    static let WAITING_EXIT = MainViewControllerState(rawValue: 1 << 4) // waiting thread stop and exit
}

@objc(MainViewController) class MainViewController: NSViewController, NSTextViewDelegate {
    static let MainClipViewInsetAfter26 = NSEdgeInsets(top: 28, left: 0, bottom: 0, right: 0)
    static let MainClipViewInsetWithFindBarAfter26 = NSEdgeInsets(top: 34, left: 0, bottom: 0, right: 0)
    static let MainTextViewInsetAfter26 = NSMakeSize(3.0, 2.0)

    @IBOutlet weak var mainScrollView: MainScrollView!
    @IBOutlet weak var mainScrollViewTopConstraint: NSLayoutConstraint!

    @IBOutlet weak var mainClipView: PaddingClipView!

    @IBOutlet weak var mainTextView: NSTextView!

    @IBOutlet weak var mainProgressIndicator: NSProgressIndicator!

    @IBOutlet weak var openButton: NSButton!
    @IBOutlet weak var clearButton: NSButton!
    @IBOutlet weak var verifyButton: NSButton!

    @IBOutlet weak var upperCaseButton: NSButton!

    @IBOutlet weak var speedTextField: NSTextField!

    @objc var tag: Int = 0 // Must have @ojbc, it is used to open finder bar.

    var mainWindowWillClose: Bool = false

    private var mainText: NSMutableAttributedString?
    private var nsAttrStrNoPreparing: NSAttributedString?

    private var state: MainViewControllerState = .NONE

    private var mainFont: NSFont?

    private var selectedLink: String = ""

    private var calcStartTime: UInt64 = 0
    private var calcEndTime: UInt64 = 0

    private var upperCaseState = false

    private var inMainQueue: Int = 0
    private var outMainQueue: Int = 0
    private let maxDiffQueue = 3

    private var curFindPanelVisible = false

    private var hashBridge: HashBridge?

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        let fHashDelegate = NSApp.delegate as? fHashMacAppDelegate

        // Initiate.
        fHashDelegate?.mainViewController = self

        let mainView = view as? MainView
        mainView?.mainViewController = self
        mainScrollView.mainViewController = self
        mainClipView.mainViewController = self

        mainWindowWillClose = false

        // Register NSUserDefaults.
        let defaultsDictionary = [
            UPPERCASE_DEFAULT_KEY: Bool(false)
        ]
        UserDefaults.standard.register(defaults: defaultsDictionary)

        // Load NSUserDefaults.
        let defaultUpperCase = UserDefaults.standard.bool(forKey: UPPERCASE_DEFAULT_KEY)

        // Alloc bridge.
        hashBridge = HashBridge(controller: self)
        hashBridge?.didLoad()

        // Set DockProgress.
        DockProgress.style = .bar

        self.setViewControllerState(.NONE)

        let fileMenu = self.getFileMenu()
        fileMenu?.autoenablesItems = false

        // Set buttons title.
        verifyButton.title = MacSwiftUtils.GetStringFromRes("MAINDLG_VERIFY")
        upperCaseButton.title = MacSwiftUtils.GetStringFromRes("MAINDLG_UPPER_HASH")

        // Set open button as default.
        openButton.keyEquivalent = "\r"

        curFindPanelVisible = mainScrollView.isFindBarVisible

        // Set scroll view border type.
        mainScrollView.borderType = .noBorder

        // Set scroll view findbar position.
        if (MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            mainScrollView.findBarPosition = .belowContent
        } else {
            mainScrollView.findBarPosition = .aboveContent
        }

        // Set clip view insets.
        if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            mainClipView.automaticallyAdjustsContentInsets = false
            mainClipView.contentInsets = MainViewController.MainClipViewInsetAfter26
        }

        // Set some text in text field.
        mainTextView.delegate = self
        if (MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            mainTextView.textContainerInset = NSMakeSize(3.0, 2.0)
        } else {
            mainTextView.textContainerInset = MainViewController.MainTextViewInsetAfter26
        }

        mainFont = .monospacedSystemFont(ofSize: 12, weight: .regular)
        if mainFont == nil {
            mainFont = mainTextView.font
        }
        mainTextView.font = mainFont

        mainTextView.usesFindBar = true

        // Set TextView nowrap.
        mainTextView.enclosingScrollView?.hasHorizontalScroller = true
        mainTextView.maxSize = NSMakeSize(CGFloat.greatestFiniteMagnitude, CGFloat.greatestFiniteMagnitude)
        mainTextView.isHorizontallyResizable = true
        mainTextView.autoresizingMask = [.width, .height]
        mainTextView.textContainer?.containerSize = NSMakeSize(CGFloat.greatestFiniteMagnitude, CGFloat.greatestFiniteMagnitude)
        mainTextView.textContainer?.widthTracksTextView = false

        // Set TextView word wrap.
        // let contentSize = mainScrollView.contentSize
        // mainTextView.enclosingScrollView?.hasHorizontalScroller = false
        // mainTextView.minSize = NSMakeSize(0.0, contentSize.height)
        // mainTextView.maxSize = NSMakeSize(CGFloat.greatestFiniteMagnitude, CGFloat.greatestFiniteMagnitude)
        // mainTextView.isVerticallyResizable = true
        // mainTextView.isHorizontallyResizable = false
        // mainTextView.autoresizingMask = .width
        // mainTextView.textContainer?.containerSize = NSMakeSize(contentSize.width, CGFloat.greatestFiniteMagnitude)
        // mainTextView.textContainer?.widthTracksTextView = true

        // Set progressbar.
        let mainProgIndiFrame = mainProgressIndicator.frame
        mainProgressIndicator.setFrameSize(NSMakeSize(mainProgIndiFrame.size.width, 2))
        mainProgressIndicator.maxValue = Double(hashBridge!.getProgMax())

        // Set checkbox.
        if defaultUpperCase {
            upperCaseButton.state = .on
        } else {
            upperCaseButton.state = .off
        }
        self.updateUpperCaseState()

        // Update main text.
        self.updateMainTextView()
    }

    override func viewWillDisappear() {
        if !mainWindowWillClose {
            return // window minimize
        }

        DockProgress.resetProgress()

        // Save NSUserDefaults.
        let defaultUpperCase = (upperCaseButton.state == .on)
        UserDefaults.standard.set(
            defaultUpperCase,
            forKey: UPPERCASE_DEFAULT_KEY)

        // Close other windows.
        let windows = NSApp.windows
        let windowCount = windows.count
        for i in 0..<windowCount {
            let window = windows[i]
            let windowController = window.windowController
            if windowController == nil || !(windowController is MainWindowController) {
                // It looks like about panel or finder overlay window.
                // They will stop us exit, let's close them.
                window.close()
            }
        }
    }

    override var representedObject: Any? {
        didSet {
            // Update the view, if already loaded.
        }
    }

    private func setViewControllerState(_ newState: MainViewControllerState) {
        switch newState {
        case .NONE:
            // Clear all.
            hashBridge?.clear()

            mainText = NSMutableAttributedString()
            var strAppend = MacSwiftUtils.GetStringFromRes("MAINDLG_INITINFO")
            strAppend += "\n\n"
            MacSwiftUtils.AppendStringToNSMutableAttributedString(mainText, strAppend)

            mainProgressIndicator.doubleValue = 0
            DockProgress.resetProgress()

            speedTextField.stringValue = ""

            // Passthrough to MAINVC_CALC_FINISH.
            fallthrough
        case .CALC_FINISH:
            calcEndTime = MacSwiftUtils.GetCurrentMilliSec()

            // Set controls title.
            let openMenuItem = self.getOpenMenuItem()
            openMenuItem?.isEnabled = true

            openButton.title = MacSwiftUtils.GetStringFromRes("MAINDLG_OPEN")
            clearButton.title = MacSwiftUtils.GetStringFromRes("MAINDLG_CLEAR")
            clearButton.isEnabled = true
            verifyButton.isEnabled = true
            upperCaseButton.isEnabled = true
        case .CALC_ING:
            calcStartTime = MacSwiftUtils.GetCurrentMilliSec()

            hashBridge?.setStop(false)

            let openMenuItem = self.getOpenMenuItem()
            openMenuItem?.isEnabled = false

            speedTextField.stringValue = ""

            openButton.title = MacSwiftUtils.GetStringFromRes("MAINDLG_STOP")
            clearButton.isEnabled = false
            verifyButton.isEnabled = false
            upperCaseButton.isEnabled = false

            self.bringWindowToFront()
        // case .VERIFY:
        // case .WAITING_EXIT:
        default:
            break
        }

        let oldState = state
        state = newState

        if state == .CALC_FINISH && oldState == .WAITING_EXIT {
            // User want to close.
            view.window?.close()
        }
    }

    private func getFileMenu() -> NSMenu? {
        let mainMenu = NSApp.mainMenu
        let fileMenuItem = mainMenu?.item(at: 1)
        return fileMenuItem?.submenu
    }

    private func getOpenMenuItem() -> NSMenuItem? {
        let mainMenu = NSApp.mainMenu
        let fileMenuItem = mainMenu?.item(at: 1)
        let openMenuItem = fileMenuItem?.submenu?.item(at: 0)
        return openMenuItem
    }

    func ableToCalcFiles() -> Bool {
        return !self.isCalculating()
    }

    func isCalculating() -> Bool {
        return (state == .CALC_ING || state == .WAITING_EXIT)
    }

    func openFiles() {
        let openPanel = NSOpenPanel()
        openPanel.showsResizeIndicator = true
        openPanel.showsHiddenFiles = false
        openPanel.canChooseDirectories = false
        openPanel.canCreateDirectories = true
        openPanel.allowsMultipleSelection = true

        openPanel.beginSheetModal(for: view.window!) { result in
            if result == .OK {
                let fileNames = openPanel.urls
                self.startHashCalc(fileNames, isURL: true)
            }
        }
    }

    func performViewDragOperation(_ sender: NSDraggingInfo?) {
        if let pboard = sender?.draggingPasteboard {
            let fileNames = pboard.readObjects(forClasses: [NSURL.self], options: [:])
            self.startHashCalc(fileNames ?? [], isURL: true)
        }
    }

    private func updateUpperCaseState() {
        upperCaseState = (upperCaseButton.state == .on)
    }

    private func updateMainTextView(_ keepScrollPosition: Bool) {
        // Apply style to all text.
        mainText?.beginEditing()

        mainText?.addAttribute(
            .font,
            value: mainFont as Any,
            range: NSRange(location: 0, length: mainText!.length))

        mainText?.addAttribute(
            .foregroundColor,
            value: NSColor.textColor,
            range: NSRange(location: 0, length: mainText!.length))

        // word wrap
        // var paraStyle = NSParagraphStyle.default.mutableCopy() as! NSMutableParagraphStyle
        // paraStyle.lineBreakMode = .byCharWrapping
        // mainText?.addAttribute(
        //     .paragraphStyle,
        //     value: paraStyle,
        //     range: NSRange(location: 0, length: mainText!.length))

        mainText?.endEditing()

        mainTextView.textStorage?.setAttributedString(mainText!)

        if (!MacSwiftUtils.IsSystemEarlierThan(14, 0) &&
            MacSwiftUtils.IsSystemEarlierThan(15, 3)) {
            // Sonoma and later insets fix.
            let fixInset = 5.0
            let mainTextSize = mainText!.size()
            let mainScrollViewSize = mainScrollView.frame.size
            var scrollViewContentInsets = mainScrollView.contentInsets
            var scrollViewScrollerInsets: NSEdgeInsets?
            if mainTextSize.width > mainScrollViewSize.width {
                // Add inset.
                scrollViewContentInsets.left = fixInset
                scrollViewContentInsets.right = fixInset
                scrollViewScrollerInsets = mainScrollView.scrollerInsets
                scrollViewScrollerInsets?.left = -(fixInset)
                scrollViewScrollerInsets?.right = -(fixInset)
            } else {
                // Reset inset.
                scrollViewContentInsets.left = 0
                scrollViewContentInsets.right = 0
                scrollViewScrollerInsets = mainScrollView.scrollerInsets
                scrollViewScrollerInsets?.left = 0
                scrollViewScrollerInsets?.right = 0
            }
            mainScrollView.contentInsets = scrollViewContentInsets
            mainScrollView.scrollerInsets = scrollViewScrollerInsets!
        }

        if !keepScrollPosition {
            // Scroll to end.
            mainTextView.layoutManager?.ensureLayout(for: mainTextView.textContainer!)
            mainTextView.scrollRangeToVisible(NSRange(location: mainTextView.string.count,
                                                      length: 0))

            // Keep on the left.
            if let enclosingScrollView = mainTextView.enclosingScrollView {
                enclosingScrollView.contentView.scroll(to:NSPoint(
                    x: 0, y: enclosingScrollView.contentView.bounds.origin.y))
                enclosingScrollView.reflectScrolledClipView(enclosingScrollView.contentView)
            }
        }
    }

    private func updateMainTextView() {
        self.updateMainTextView(false)
    }

    private func canUpdateMainTextView() -> Bool {
        // NSLog("%@", ((self.inMainQueue - self.outMainQueue < self.maxDiffQueue) ? "true" : "false"))
        return (self.inMainQueue - self.outMainQueue < self.maxDiffQueue)
    }

    func findPanelVisibleChange(isVisible: Bool) {
        if (MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            return
        }

        if isVisible {
            // show
            mainScrollViewTopConstraint.constant = 28
            mainClipView.contentInsets = MainViewController.MainClipViewInsetWithFindBarAfter26
        } else {
            // hide
            mainScrollViewTopConstraint.constant = 0
            mainClipView.contentInsets = MainViewController.MainClipViewInsetAfter26
        }
        // if let enclosingScrollView = mainTextView.enclosingScrollView {
        //     NSLog("findPanelVisibleChange, y=%.2f", enclosingScrollView.contentView.bounds.origin.y)
        // }

    }

    func clipViewSizeChange() {
        if (MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
            return
        }

        var scrollNeedFix = true
        var becameShow = true

        let newFindPanelVisible = mainScrollView.isFindBarVisible
        if newFindPanelVisible == curFindPanelVisible {
            scrollNeedFix = false
        }
        if newFindPanelVisible && !curFindPanelVisible {
            // show find bar
            // NSLog("clipViewSizeChange, show find bar")
        }
        if !newFindPanelVisible && curFindPanelVisible {
            // hide find bar
            // NSLog("clipViewSizeChange, hide find bar")
            becameShow = false
        }

        let mainTextSize = mainText!.size()
        let mainScrollViewSize = mainScrollView.frame.size
        // NSLog("clipViewSizeChange, mainTextSize.height=%.2f, mainScrollViewSize.height=%.2f",
        //       mainTextSize.height, mainScrollViewSize.height)
        if mainTextSize.height < mainScrollViewSize.height {
            scrollNeedFix = false
        }

        if scrollNeedFix, let enclosingScrollView = self.mainTextView.enclosingScrollView {
            // NSLog("clipViewSizeChange, y=%.2f", enclosingScrollView.contentView.bounds.origin.y)
            var scrollFix: CGFloat = 0
            let bottomOffset = mainTextSize.height - mainScrollViewSize.height - enclosingScrollView.contentView.bounds.origin.y
            if becameShow && enclosingScrollView.contentView.bounds.origin.y < -18 {
                // NSLog("clipViewSizeChange, fix show top")
                scrollFix = -6
            }
            // NSLog("clipViewSizeChange, bottomOffset=%.2f", bottomOffset)
            if becameShow && bottomOffset <= 28 {
                // NSLog("clipViewSizeChange, fix show bottom")
                scrollFix = 28
            }

            if scrollFix != 0 {
                enclosingScrollView.contentView.scroll(to:NSPoint(
                    x: enclosingScrollView.contentView.bounds.origin.x,
                    y: enclosingScrollView.contentView.bounds.origin.y + scrollFix))
                enclosingScrollView.reflectScrolledClipView(enclosingScrollView.contentView)
            }
            // NSLog("clipViewSizeChange, after, y=%.2f", enclosingScrollView.contentView.bounds.origin.y)
        }

        curFindPanelVisible = newFindPanelVisible
    }

    private func calculateFinished() {
        self.setViewControllerState(.CALC_FINISH)

        let progMax = hashBridge!.getProgMax()
        self.mainProgressIndicator.doubleValue = Double(progMax)
        self.updateDockProgress(Int(progMax))

        // Show calc speed.
        let calcDurationTime = calcEndTime - calcStartTime
        if calcDurationTime > 10 {
            // speed is Bytes/ms
            var calcSpeed = Double(hashBridge!.getTotalSize()) / Double(calcDurationTime)
            calcSpeed = calcSpeed * 1000 // Bytes/s

            var strSpeed = ""
            strSpeed = MacSwiftUtils.ConvertSizeToShortSizeStr(UInt64(calcSpeed), true)
            if strSpeed != "" {
                strSpeed += "/s"
            }
            speedTextField.stringValue = strSpeed
        } else {
            speedTextField.stringValue = ""
        }
    }

    private func calculateStopped() {
        let strAppend = "\n"
        //strAppend += MacSwiftUtils.GetStringFromRes("MAINDLG_CALCU_TERMINAL")
        //strAppend += "\n\n"

        MacSwiftUtils.AppendStringToNSMutableAttributedString(self.mainText, strAppend)

        self.setViewControllerState(.CALC_FINISH)

        self.mainProgressIndicator.doubleValue = 0
        DockProgress.resetProgress()

        //self.updateMainTextView()
    }

    func startHashCalc(_ fileNames: [Any], isURL: Bool) {
        if !self.ableToCalcFiles() {
            return
        }

        if state == .NONE {
            // Clear up text.
            mainText = NSMutableAttributedString()
        }

        // Get files path.
        hashBridge?.addFiles(fileNames, isURL: isURL)

        // Uppercase.
        self.updateUpperCaseState()
        hashBridge?.setUppercase(upperCaseState)

        mainProgressIndicator.doubleValue = 0
        DockProgress.resetProgress()

        self.setViewControllerState(.CALC_ING)

        // Ready to go.
        inMainQueue = 0
        outMainQueue = 0
        hashBridge?.startHashThread()
    }

    func stopHashCalc(_ needExit: Bool) {
        if state == .CALC_ING {
            hashBridge?.setStop(true)

            if needExit {
                self.setViewControllerState(.WAITING_EXIT)
            }
        }
    }

    private func refreshResultText() {
        self.updateUpperCaseState()

        mainText = NSMutableAttributedString()

        let results:[Any] = hashBridge!.getResults()
        for result in results {
            let resultSwift = result as? ResultDataSwift
            self.appendResultToNSMutableAttributedString(resultSwift!, upperCaseState, mainText!)
        }

        self.updateMainTextView(true)
    }

    private func updateDockProgress(_ value: Int) {
        var dockProgress = (Double(value) / self.mainProgressIndicator.maxValue)
        if (dockProgress >= 1) {
            dockProgress = 0.99999 // 1 will disappear.
        }
        // NSLog("dockProgress=%.10f", dockProgress)
        DockProgress.progress = dockProgress
    }

    private func bringWindowToFront() {
        DispatchQueue.main.async(execute: {
            self.view.window?.deminiaturize(self)
            NSApp.activate(ignoringOtherApps: true)
        })
    }

    private func appendFileNameToNSMutableAttributedString(_ result: ResultDataSwift,
                                                           _ nsmutAttrString: NSMutableAttributedString) {
        var strAppend = MacSwiftUtils.GetStringFromRes("FILENAME_STRING")
        strAppend += " "
        strAppend += result.strPath
        strAppend += "\n"
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutAttrString, strAppend)
    }

    private func appendFileMetaToNSMutableAttributedString(_ result: ResultDataSwift,
                                                           _ nsmutAttrString: NSMutableAttributedString) {
        let strSizeByte = String(format: "%llu", result.ulSize)
        let strShortSize = MacSwiftUtils.ConvertSizeToShortSizeStr(result.ulSize)

        var strAppend = MacSwiftUtils.GetStringFromRes("FILESIZE_STRING")
        strAppend += " "
        strAppend += strSizeByte
        strAppend += " "
        strAppend += MacSwiftUtils.GetStringFromRes("BYTE_STRING")
        if strShortSize != "" {
            strAppend += " ("
            strAppend += strShortSize
            strAppend += ")"
        }
        strAppend += "\n"
        strAppend += MacSwiftUtils.GetStringFromRes("MODIFYTIME_STRING")
        strAppend += " "
        strAppend += result.strMDate
        strAppend += "\n"

        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutAttrString, strAppend)
    }

    private func appendFileHashToNSMutableAttributedString(_ result: ResultDataSwift,
                                                           _ uppercase: Bool,
                                                           _ nsmutAttrString: NSMutableAttributedString)
    {
        var strFileMD5 = "", strFileSHA1 = "", strFileSHA256 = "", strFileSHA512 = ""

        if uppercase {
            strFileMD5 = result.strMD5.uppercased()
            strFileSHA1 = result.strSHA1.uppercased()
            strFileSHA256 = result.strSHA256.uppercased()
            strFileSHA512 = result.strSHA512.uppercased()
        } else {
            strFileMD5 = result.strMD5.lowercased()
            strFileSHA1 = result.strSHA1.lowercased()
            strFileSHA256 = result.strSHA256.lowercased()
            strFileSHA512 = result.strSHA512.lowercased()
        }

        let nsmutStrHash = NSMutableAttributedString()

        nsmutStrHash.beginEditing()

        var oldLength:Int = 0

        // MD5
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, "MD5: ")
        oldLength = nsmutStrHash.length
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, strFileMD5)
        nsmutStrHash.addAttribute(.link,
                                  value: strFileMD5,
                                  range: NSRange(location: oldLength, length: strFileMD5.count))

        // SHA1
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, "\nSHA1: ")
        oldLength = nsmutStrHash.length
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, strFileSHA1)
        nsmutStrHash.addAttribute(.link,
                                  value: strFileSHA1,
                                  range: NSRange(location: oldLength, length: strFileSHA1.count))

        // SHA256
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, "\nSHA256: ")
        oldLength = nsmutStrHash.length
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, strFileSHA256)
        nsmutStrHash.addAttribute(.link,
                                  value: strFileSHA256,
                                  range: NSRange(location: oldLength, length: strFileSHA256.count))

        // SHA512
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, "\nSHA512: ")
        oldLength = nsmutStrHash.length
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, strFileSHA512)
        nsmutStrHash.addAttribute(.link,
                                  value: strFileSHA512,
                                  range: NSRange(location: oldLength, length: strFileSHA512.count))

        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutStrHash, "\n\n")

        nsmutStrHash.endEditing()

        nsmutAttrString.append(nsmutStrHash)
    }

    private func appendFileErrToNSMutableAttributedString(_ result: ResultDataSwift,
                                                          _ nsmutAttrString: NSMutableAttributedString) {
        let strAppend = result.strError + "\n\n"
        MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutAttrString, strAppend)
    }

    private func appendResultToNSMutableAttributedString(_ result: ResultDataSwift,
                                                         _ uppercase: Bool,
                                                         _ nsmutAttrString: NSMutableAttributedString) {
        if result.state == ResultDataSwift.RESULT_NONE {
            return
        }

        if result.state == ResultDataSwift.RESULT_ALL ||
            result.state == ResultDataSwift.RESULT_META ||
            result.state == ResultDataSwift.RESULT_ERROR ||
            result.state == ResultDataSwift.RESULT_PATH {
            self.appendFileNameToNSMutableAttributedString(result, nsmutAttrString)
        }

        if result.state == ResultDataSwift.RESULT_ALL ||
            result.state == ResultDataSwift.RESULT_META {
            self.appendFileMetaToNSMutableAttributedString(result, nsmutAttrString)
        }

        if (result.state == ResultDataSwift.RESULT_ALL) {
            self.appendFileHashToNSMutableAttributedString(result, uppercase, nsmutAttrString)
        }

        if (result.state == ResultDataSwift.RESULT_ERROR) {
            self.appendFileErrToNSMutableAttributedString(result, nsmutAttrString)
        }

        if result.state != ResultDataSwift.RESULT_ALL &&
            result.state != ResultDataSwift.RESULT_ERROR {
            let strAppend = "\n"
            MacSwiftUtils.AppendStringToNSMutableAttributedString(nsmutAttrString, strAppend)
        }
    }

    @objc func onPreparingCalc() {
        // Copy old string.
        self.nsAttrStrNoPreparing = NSMutableAttributedString(attributedString: self.mainText!)

        var strAppend = MacSwiftUtils.GetStringFromRes("MAINDLG_WAITING_START")
        strAppend += "\n"
        MacSwiftUtils.AppendStringToNSMutableAttributedString(self.mainText, strAppend)

        self.updateMainTextView()
    }

    @objc func onRemovePreparingCalc() {
        // Reset old string.
        self.mainText = NSMutableAttributedString(attributedString: self.nsAttrStrNoPreparing!)
    }

    @objc func onCalcStop() {
        self.calculateStopped()
    }

    @objc func onCalcFinish() {
        self.calculateFinished()
    }

    @objc func onShowFileName(_ result: ResultDataSwift) {
        inMainQueue += 1
        DispatchQueue.main.async(execute: { [result] in
            self.outMainQueue += 1
            self.appendFileNameToNSMutableAttributedString(result, self.mainText!)
            if self.canUpdateMainTextView() {
                self.updateMainTextView()
            }
        })
    }

    @objc func onShowFileMeta(_ result: ResultDataSwift) {
        inMainQueue += 1
        DispatchQueue.main.async(execute: { [result] in
            self.outMainQueue += 1
            self.appendFileMetaToNSMutableAttributedString(result, self.mainText!)
            if self.canUpdateMainTextView() {
                self.updateMainTextView()
            }
        })
    }

    @objc func onShowFileHash(_ result: ResultDataSwift, uppercase: Bool) {
        inMainQueue += 1
        DispatchQueue.main.async(execute: { [result] in
            self.outMainQueue += 1
            self.appendFileHashToNSMutableAttributedString(result, uppercase, self.mainText!)
            if self.canUpdateMainTextView() {
                self.updateMainTextView()
            }
        })
    }

    @objc func onShowFileErr(_ result: ResultDataSwift) {
        inMainQueue += 1
        DispatchQueue.main.async(execute: { [result] in
            self.outMainQueue += 1
            self.appendFileErrToNSMutableAttributedString(result, self.mainText!)
            if self.canUpdateMainTextView() {
                self.updateMainTextView()
            }
        })
    }

    @objc func onUpdateProgWhole(_ value: Int) {
        let oldValue = Int(self.mainProgressIndicator.doubleValue)
        if value == oldValue {
            return
        }

        self.mainProgressIndicator.doubleValue = Double(value)
        self.updateDockProgress(value)
    }

    @IBAction func openButtonClicked(_ sender: NSButton) {
        if state == .CALC_ING {
            self.stopHashCalc(false)
        } else {
            self.openFiles()
        }
    }

    @IBAction func clearButtonClicked(_ sender: NSButton) {
        if state == .VERIFY {
        } else {
            self.setViewControllerState(.NONE)
            self.updateMainTextView()
        }
    }

    @IBAction func verifyButtonClicked(_ sender: NSButton) {
        tag = NSTextFinder.Action.showFindInterface.rawValue
        mainTextView.performTextFinderAction(self)
    }

    @IBAction func uppercaseButtonClicked(_ sender: NSButton) {
        if state == .CALC_FINISH {
            self.refreshResultText()
        }
    }

    func textView(_ aTextView: NSTextView,
                  clickedOnLink link: Any,
                  at charIndex: Int) -> Bool {
        if aTextView == mainTextView {
            selectedLink = link as! String

            let nsptMouseLoc = NSEvent.mouseLocation
            let nsrtMouseInView = view.window!.convertFromScreen(NSRect(x: nsptMouseLoc.x, y: nsptMouseLoc.y, width: 0, height: 0))
            let nsptMouseInView = nsrtMouseInView.origin

            var nsmenuItem: NSMenuItem? = nil
            let nsmenuHash = NSMenu(title: "HashMenu")
            nsmenuItem = nsmenuHash.insertItem(
                withTitle: MacSwiftUtils.GetStringFromRes("MAINDLG_HYPEREDIT_MENU_COPY"),
                action: #selector(self.menuCopyHash),
                keyEquivalent: "",
                at: 0)
            if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
                nsmenuItem?.image = NSImage(systemSymbolName: "document.on.document", accessibilityDescription: nil)
            }
            nsmenuHash.insertItem(NSMenuItem.separator(), at: 1)
            nsmenuItem = nsmenuHash.insertItem(
                withTitle: MacSwiftUtils.GetStringFromRes("MAINDLG_HYPEREDIT_MENU_SERACHGOOGLE"),
                action: #selector(self.menuSearchGoogle),
                keyEquivalent: "",
                at: 2)
            if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
                nsmenuItem?.image = NSImage(systemSymbolName: "magnifyingglass", accessibilityDescription: nil)
            }
            nsmenuItem = nsmenuHash.insertItem(
                withTitle: MacSwiftUtils.GetStringFromRes("MAINDLG_HYPEREDIT_MENU_SERACHVIRUSTOTAL"),
                action: #selector(self.menuSearchVirusTotal),
                keyEquivalent: "",
                at: 3)
            if (!MacSwiftUtils.IsSystemEarlierThan(26, 0)) {
                nsmenuItem?.image = NSImage(systemSymbolName: "safari", accessibilityDescription: nil)
            }

            nsmenuHash.popUp(positioning: nil, at: nsptMouseInView, in: view)

            return true
        }

        return false
    }

    @objc func menuCopyHash() {
        let pasteboard = NSPasteboard.general
        pasteboard.clearContents()
        pasteboard.setString(selectedLink, forType: .string)
    }

    @objc func menuSearchGoogle() {
        let nstrUrl = "https://www.google.com/search?q=\(selectedLink)&ie=utf-8&oe=utf-8"
        let url = URL(string: nstrUrl)!
        NSWorkspace.shared.open(url)
    }

    @objc func menuSearchVirusTotal() {
        let nstrUrl = "https://www.virustotal.com/gui/search/\(selectedLink)"
        let url = URL(string: nstrUrl)!
        NSWorkspace.shared.open(url)
    }
}
