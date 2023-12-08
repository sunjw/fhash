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
    @IBOutlet var mainScrollView: NSScrollView!
    @IBOutlet var mainTextView: NSTextView!

    @IBOutlet var mainProgressIndicator: ESSProgressIndicator!

    @IBOutlet weak var openButton: NSButton!
    @IBOutlet weak var clearButton: NSButton!
    @IBOutlet weak var verifyButton: NSButton!

    @IBOutlet weak var upperCaseButton: NSButton!

    @IBOutlet weak var speedTextField: NSTextField!

    //@property (assign) sunjwbase::OsMutex *mainMtx;

    private var mainText: NSMutableAttributedString?
    private var nsAttrStrNoPreparing: NSAttributedString?

    @objc var tag: UInt = 0

    // private
    private var state: MainViewControllerState = .NONE

    private var mainFont: NSFont?

    private var selectedLink: String = ""

    private var calcStartTime: UInt64 = 0
    private var calcEndTime: UInt64 = 0

    private var upperCaseState = false

    //@property (assign) UIBridgeMacUI *uiBridgeMac;
    //@property (assign) ThreadData *thrdData;
    //@property (assign) pthread_t ptHash;
    private var hashBridge: HashBridge?

    //@synthesize mainMtx = _mainMtx;
    //@synthesize uiBridgeMac = _uiBridgeMac;

    //@synthesize thrdData = _thrdData;
    //@synthesize ptHash = _ptHash;

    // Not be called on exit.
    // Just for sure.
    deinit {
        // delete _mainMtx;
        // delete _uiBridgeMac;
        // delete _thrdData;
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        let fHashDelegate = NSApp.delegate as? fHashMacAppDelegate

        // Initiate.
        fHashDelegate?.mainViewController = self

        let mainView = view as? MainView
        mainView?.mainViewController = self

        // Register NSUserDefaults.
        let defaultsDictionary = [
            UPPERCASE_DEFAULT_KEY: Bool(false)
        ]
        UserDefaults.standard.register(defaults: defaultsDictionary)

        // Load NSUserDefaults.
        let defaultUpperCase = UserDefaults.standard.bool(forKey: UPPERCASE_DEFAULT_KEY)

        // Alloc c++ member.
        //_mainMtx = new OsMutex();
        //_uiBridgeMac = new UIBridgeMacUI(self);
        //_thrdData = new ThreadData();

        // Alloc bridge.
        hashBridge = HashBridge(controller: self)
        hashBridge?.didLoad()

        self.setViewControllerState(.NONE)

        //_thrdData->uiBridge = _uiBridgeMac;

        let fileMenu = self.getFileMenu()
        fileMenu?.autoenablesItems = false

        // Set buttons title.
        verifyButton.title = MacSwiftUtils.GetStringFromRes("MAINDLG_VERIFY")
        upperCaseButton.title = MacSwiftUtils.GetStringFromRes("MAINDLG_UPPER_HASH")

        // Set open button as default.
        openButton.keyEquivalent = "\r"

        // Set scroll view border type.
        mainScrollView.borderType = .noBorder

        // Set some text in text field.
        mainTextView.delegate = self
        mainTextView.textContainerInset = NSMakeSize(3.0, 2.0)

        if MacSwiftUtils.IsSystemEarlierThan(10, 15) {
            mainFont = NSFont(name: "Monaco", size: 12)
        } else {
            mainFont = .monospacedSystemFont(ofSize: 12, weight: .regular)
        }
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
        //mainProgressIndicator.maxValue = uiBridgeMac.getProgMax()

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
        // Save NSUserDefaults.
        let defaultUpperCase = (upperCaseButton.state == .on)
        UserDefaults.standard.set(
            defaultUpperCase,
            forKey: UPPERCASE_DEFAULT_KEY)

        // Close other windows.
        let windows = NSApplication.shared.windows
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
            // _thrdData->threadWorking = false;
            // _thrdData->stop = false;

            // _thrdData->uppercase = false;
            // _thrdData->totalSize = 0;

            // _thrdData->nFiles = 0;
            // _thrdData->fullPaths.clear();

            // _thrdData->resultList.clear();
            hashBridge?.clear()

            // _mainMtx->lock();
            mainText = NSMutableAttributedString()
            var strAppend = MacSwiftUtils.GetStringFromRes("MAINDLG_INITINFO")
            strAppend += "\n\n"
            MacSwiftUtils.AppendStringToNSMutableAttributedString(mainText, strAppend)
            // _mainMtx->unlock();

            mainProgressIndicator.jump(toDoubleValue: 0)
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

            // _thrdData->stop = false;
            hashBridge?.setStop(false)

            let openMenuItem = self.getOpenMenuItem()
            openMenuItem?.isEnabled = false

            speedTextField.stringValue = ""

            openButton.title = MacSwiftUtils.GetStringFromRes("MAINDLG_STOP")
            clearButton.isEnabled = false
            verifyButton.isEnabled = false
            upperCaseButton.isEnabled = false
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

    @objc func ableToCalcFiles() -> Bool {
        return !self.isCalculating()
    }

    @objc func isCalculating() -> Bool {
        return (state == .CALC_ING || state == .WAITING_EXIT)
    }

    @objc func openFiles() {
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

    @objc func performViewDragOperation(_ sender: NSDraggingInfo?) {
        if let pboard = sender?.draggingPasteboard {
            let fileNames = pboard.readObjects(forClasses: [NSURL.self], options: [:])
            self.startHashCalc(fileNames ?? [], isURL: true)
        }
    }

    private func updateUpperCaseState() {
        upperCaseState = (upperCaseButton.state == .on)
    }

    private func updateMainTextView(_ keepScrollPosition: Bool) {
        // _mainMtx->lock();

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

        if (MacSwiftUtils.GetSystemMajorVersion() == 14) {
            // Sonoma insets fix.
            let mainTextSize = mainText!.size()
            let mainScrollViewSize = mainScrollView.frame.size
            var scrollViewContentInsets = mainScrollView.contentInsets
            if mainTextSize.width > mainScrollViewSize.width {
                scrollViewContentInsets.left = 5.0
                scrollViewContentInsets.right = 5.0
            } else {
                scrollViewContentInsets.left = 0
                scrollViewContentInsets.right = 0
            }
            mainScrollView.contentInsets = scrollViewContentInsets
        }

        // _mainMtx->unlock();

        if !keepScrollPosition {
            // Scroll to end.
            mainTextView.scrollRangeToVisible(NSRange(location: mainTextView.string.count,
                                                      length: 0))
        }
    }

    private func updateMainTextView() {
        self.updateMainTextView(false)
    }

    private func calculateFinished() {
        self.setViewControllerState(.CALC_FINISH)
        // [self.mainProgressIndicator jumpToDoubleValue:_uiBridgeMac->getProgMax()];

        // Show calc speed.
        let calcDurationTime = calcEndTime - calcStartTime
        if calcDurationTime > 10 {
            // speed is Bytes/ms
            var calcSpeed = 0 // Double(thrdData.totalSize) / Double(calcDurationTime)
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
        self.mainProgressIndicator.jump(toDoubleValue: 0)

        //self.updateMainTextView()
    }

    @objc func startHashCalc(_ fileNames: [Any], isURL: Bool) {
        if !self.ableToCalcFiles() {
            return
        }

        if state == .NONE {
            // Clear up text.
            // _mainMtx->lock();
            mainText = NSMutableAttributedString()
            // _mainMtx->unlock();
        }

        // Get files path.
        let fileCount = fileNames.count
        // _thrdData->nFiles = (uint32_t)fileCount;
        // _thrdData->fullPaths.clear();

        // for (uint32_t i = 0; i < _thrdData->nFiles; ++i) {
        //     string strFileName;
        //     if (!isURL) {
        //         NSString *nsstrFileName = [fileNames objectAtIndex:i];
        //         strFileName = MacUtils::ConvertNSStringToUTF8String(nsstrFileName);
        //     } else {
        //         NSURL *nsurlFileName = [fileNames objectAtIndex:i];
        //         strFileName = MacUtils::ConvertNSStringToUTF8String([nsurlFileName path]);
        //     }
        //     _thrdData->fullPaths.push_back(strtotstr(strFileName));
        // }

        // Uppercase.
        self.updateUpperCaseState()
        // _thrdData->uppercase = _upperCaseState;
        hashBridge?.setUppercase(upperCaseState)

        mainProgressIndicator.jump(toDoubleValue: 0)

        // Ready to go.
        self.setViewControllerState(.CALC_ING)

        // pthread_create(&_ptHash,
        //                NULL,
        //                (void *(*)(void *))HashThreadFunc,
        //                _thrdData);
    }

    @objc func stopHashCalc(_ needExit: Bool) {
        if state == .CALC_ING {
            // _thrdData->stop = true;
            hashBridge?.setStop(true)

            if needExit {
                self.setViewControllerState(.WAITING_EXIT)
            }
        }
    }

    private func refreshResultText() {
        self.updateUpperCaseState()

        // _mainMtx->lock();
        mainText = NSMutableAttributedString()

        // ResultList::iterator itr = _thrdData->resultList.begin();
        // for(; itr != _thrdData->resultList.end(); ++itr)
        // {
        //     UIBridgeMacUI::AppendResultToNSMutableAttributedString(*itr, _upperCaseState, _mainText);
        // }
        // _mainMtx->unlock();

        self.updateMainTextView(true)
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
        DispatchQueue.main.async(execute: {
            // Copy old string.
            self.nsAttrStrNoPreparing = NSMutableAttributedString(attributedString: self.mainText!)

            var strAppend = MacSwiftUtils.GetStringFromRes("MAINDLG_WAITING_START")
            strAppend += "\n"
            MacSwiftUtils.AppendStringToNSMutableAttributedString(self.mainText, strAppend)

            self.updateMainTextView()
        })
    }

    @objc func onRemovePreparingCalc() {
        DispatchQueue.main.async(execute: {
            // Reset old string.
            self.mainText = NSMutableAttributedString(attributedString: self.nsAttrStrNoPreparing!)
        })
    }

    @objc func onCalcStop() {
        DispatchQueue.main.async(execute: {
            self.calculateStopped()
        })
    }

    @objc func onCalcFinish() {
        DispatchQueue.main.async(execute: {
            self.calculateFinished()
        })
    }

    @objc func onShowFileName(_ result: ResultDataSwift) {
        DispatchQueue.main.async(execute: { [result] in
            self.appendFileNameToNSMutableAttributedString(result, self.mainText!)
            self.updateMainTextView()
        })
    }

    @objc func onShowFileMeta(_ result: ResultDataSwift) {
        DispatchQueue.main.async(execute: { [result] in
            self.appendFileMetaToNSMutableAttributedString(result, self.mainText!)
            self.updateMainTextView()
        })
    }

    @objc func onShowFileHash(_ result: ResultDataSwift, uppercase: Bool) {
        DispatchQueue.main.async(execute: { [result] in
            self.appendFileHashToNSMutableAttributedString(result, uppercase, self.mainText!)
            self.updateMainTextView()
        })
    }

    @objc func onShowFileErr(_ result: ResultDataSwift) {
        DispatchQueue.main.async(execute: { [result] in
            self.appendFileErrToNSMutableAttributedString(result, self.mainText!)
            self.updateMainTextView()
        })
    }

    @objc func onUpdateProgWhole(_ value: Int) {
        DispatchQueue.main.async(execute: {
            let oldValue = Int(self.mainProgressIndicator.toValue)
            if value == oldValue {
                return
            }
            self.mainProgressIndicator.animate(toDoubleValue: Double(value))
        })
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
        tag = NSFindPanelAction.showFindPanel.rawValue
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

            let nsmenuHash = NSMenu(title: "HashMenu")
            nsmenuHash.insertItem(
                withTitle: MacSwiftUtils.GetStringFromRes("MAINDLG_HYPEREDIT_MENU_COPY"),
                action: #selector(self.menuCopyHash),
                keyEquivalent: "",
                at: 0)
            nsmenuHash.insertItem(NSMenuItem.separator(), at: 1)
            nsmenuHash.insertItem(
                withTitle: MacSwiftUtils.GetStringFromRes("MAINDLG_HYPEREDIT_MENU_SERACHGOOGLE"),
                action: #selector(self.menuSearchGoogle),
                keyEquivalent: "",
                at: 2)
            nsmenuHash.insertItem(
                withTitle: MacSwiftUtils.GetStringFromRes("MAINDLG_HYPEREDIT_MENU_SERACHVIRUSTOTAL"),
                action: #selector(self.menuSearchVirusTotal),
                keyEquivalent: "",
                at: 3)

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
        let nstrUrl = "https://www.virustotal.com/#/search/\(selectedLink)"
        let url = URL(string: nstrUrl)!
        NSWorkspace.shared.open(url)
    }
}
