//
//  MacSwiftUtils.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/4.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Foundation

class MacSwiftUtils {
    class func GetSystemVersion() -> String? {
        let systemVersionDictionary = NSDictionary(contentsOfFile: "/System/Library/CoreServices/SystemVersion.plist")
        let systemVersion = systemVersionDictionary?.object(forKey: "ProductVersion") as? String
        return systemVersion
    }

    class func GetSystemMajorVersion() -> Int {
        var majorVersion = 0

        let systemVersion = GetSystemVersion()
        let versionParts = systemVersion?.components(separatedBy: ".")
        if versionParts != nil && (versionParts?.count ?? 0) > 0 {
            let strMajorVersion = versionParts?[0]
            majorVersion = Int(strMajorVersion ?? "") ?? 0
        }

        return majorVersion
    }

    class func GetSystemMinorVersion() -> Int {
        var minorVersion = 0

        let systemVersion = GetSystemVersion()
        let versionParts = systemVersion?.components(separatedBy: ".")
        if versionParts != nil && (versionParts?.count ?? 0) > 1 {
            let strMinorVersion = versionParts?[1]
            minorVersion = Int(strMinorVersion ?? "") ?? 0
        }

        return minorVersion
    }

    class func IsSystemEarlierThan(_ majorVersion: Int, _ minorVersion: Int) -> Bool {
        let systemMajorVersion = GetSystemMajorVersion()
        let systemMinorVersion = GetSystemMinorVersion()
        return (systemMajorVersion < majorVersion ||
                (systemMajorVersion == majorVersion && systemMinorVersion < minorVersion))
    }

    class func IsSystemEqual(_ majorVersion: Int, _ minorVersion: Int) -> Bool {
        let systemMajorVersion = GetSystemMajorVersion()
        let systemMinorVersion = GetSystemMinorVersion()
        return (systemMajorVersion == majorVersion && systemMinorVersion == minorVersion)
    }

    class func GetSystemPreferredLanguage() -> String? {
        let language = NSLocale.preferredLanguages[0]
        return language
    }

    class func ConvertUTF8CStringToString(_ cstrUtf8: UnsafePointer<CChar>) -> String? {
        return String(utf8String: cstrUtf8)
    }

    class func GetStringFromRes(_ nsstrKey: String) -> String {
        var strLocalized = NSLocalizedString(nsstrKey, comment: "")
        // Fix Windows UI strings.
        // Fix zh-cn.
        for iTest in Unicode.Scalar("A").value...Unicode.Scalar("Z").value {
            let chTest = Character(Unicode.Scalar(iTest)!)
            var strTest = "(&"
            strTest += String(chTest)
            strTest += ")"
            strLocalized = strLocalized.replacingOccurrences(of: strTest, with: "") // Remove "(&X)"
        }
        // Fix en-us
        strLocalized = strLocalized.replacingOccurrences(of: "&", with: "") // Remove "&"

        return strLocalized
    }

    class func ConvertStringToNSAttributedString(_ str: String?) -> NSAttributedString? {
        return NSAttributedString(string: str ?? "")
    }

    class func AppendStringToNSMutableAttributedString(_ base: NSMutableAttributedString?, _ str: String?) {
        if let aStr = ConvertStringToNSAttributedString(str) {
            base?.append(aStr)
        }
    }
}
