//
//  ResultDataSwift.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/7.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//

import Foundation

@objc(ResultDataSwift) class ResultDataSwift: NSObject {
    @objc static let RESULT_NONE = 0
    @objc static let RESULT_PATH = 1
    @objc static let RESULT_META = 2
    @objc static let RESULT_ALL = 3
    @objc static let RESULT_ERROR = 4

    @objc var state = RESULT_NONE // State
    @objc var strPath:String = "" // Path
    @objc var ulSize:UInt64 = 0 // Size
    @objc var strMDate:String = "" // Modified date
    @objc var strVersion:String = "" // Version
    @objc var strMD5:String = "" // MD5
    @objc var strSHA1:String = "" // SHA1
    @objc var strSHA256:String = "" // SHA256
    @objc var strSHA512:String = "" // SHA512
    @objc var strError:String = "" // Error string
}
