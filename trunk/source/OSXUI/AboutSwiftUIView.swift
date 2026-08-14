//
//  AboutSwiftUIView.swift
//  fHash
//
//  Created by Sun Junwen on 2026/8/14.
//  Copyright © 2026 Sun Junwen. All rights reserved.
//

import Cocoa
import SwiftUI

// Pure SwiftUI About view content, the same to AboutViewController.
struct AboutSwiftUIView: View {
    static let contentSize = NSSize(width: 430, height: 272)

    let onClose: () -> Void

    var body: some View {
        Color.clear
            .frame(width: Self.contentSize.width, height: Self.contentSize.height)
            .background(AboutGlassBackground())
            .overlay(alignment: .topLeading) {
                iconView
                    .padding(.leading, 20)
                    .padding(.top, 46)
            }
            .overlay(alignment: .topTrailing) {
                infoTextView
                    .padding(.trailing, 20)
                    .padding(.top, 48)
            }
            .overlay(alignment: .bottomLeading) {
                homePageLinkView
                    .padding(.leading, 20)
                    .padding(.bottom, 22)
            }
            .overlay(alignment: .bottomTrailing) {
                closeButtonView
                    .padding(.trailing, 20)
                    .padding(.bottom, 16)
            }
    }

    private var iconView: some View {
        Image(nsImage: NSApp.applicationIconImage ?? NSImage())
            .resizable()
            .aspectRatio(contentMode: .fit)
            .frame(width: 64, height: 64)
    }

    private var infoTextView: some View {
        Text(aboutInfoText)
            .font(.system(size: 13))
            .foregroundStyle(Color(nsColor: .labelColor))
            .frame(width: 308, height: 165, alignment: .topLeading)
    }

    private var homePageLinkView: some View {
        Link(homePageLinkTitle, destination: homePageURL)
            .font(.system(size: 13))
            .frame(height: 17)
    }

    @ViewBuilder
    private var closeButtonView: some View {
        if #available(macOS 26.0, *) {
            Button(MacSwiftUtils.GetStringFromRes("BUTTON_OK"), action: onClose)
                .keyboardShortcut(.defaultAction)
                .buttonStyle(.glassProminent)
                .controlSize(.extraLarge)
        }
    }

    private var aboutInfoText: String {
        let strAppVersion = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String
        let strAppBundleVersion = Bundle.main.infoDictionary?["CFBundleVersion"] as? String

        var strAboutInfo = ""
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_TITLE")
        strAboutInfo += " "
        strAboutInfo += (strAppVersion ?? "")
        strAboutInfo += " ("
        strAboutInfo += (strAppBundleVersion ?? "")
        strAboutInfo += ")"
        strAboutInfo += "\n"
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_RIGHT")
        strAboutInfo += "\n"
        strAboutInfo += "\n"
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_MD5")
        strAboutInfo += "\n"
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_SHA256")
        strAboutInfo += "\n"
        strAboutInfo += MacSwiftUtils.GetStringFromRes("ABOUTDLG_INFO_SHA512")
        return strAboutInfo
    }

    private var homePageLinkTitle: String {
        var strLinkText = MacSwiftUtils.GetStringFromRes("ABOUTDLG_PROJECT_SITE")
        strLinkText = strLinkText.replacingOccurrences(of: "<a>", with: "")
        strLinkText = strLinkText.replacingOccurrences(of: "</a>", with: "")
        return strLinkText
    }

    private var homePageURL: URL {
        URL(string: MacSwiftUtils.GetStringFromRes("ABOUTDLG_PROJECT_URL"))!
    }
}

private struct AboutGlassBackground: View {
    var body: some View {
        if #available(macOS 26.0, *) {
            Rectangle()
                .fill(.clear)
                .glassEffect(.regular, in: Rectangle())
        }
    }
}

#Preview {
    AboutSwiftUIView(onClose: {})
}

