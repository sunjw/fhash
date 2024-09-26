//
//  ESSProgressIndicator.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//
//
//  ESSProgressIndicator.m
//  Briefly
//
//  Created by Matthias Gansrigler on 06.05.2015.
//  Copyright (c) 2015 Eternal Storms Software. All rights reserved.
//
//  Edited by Sun Junwen on 2015/08/15.
//  Swift version created by Sun Junwen on 2023/12/1.
//

import Cocoa

private class ESSProgressBarAnimation: NSAnimation {
    private var progInd: ESSProgressIndicator?
    private var initValue: Double = 0.0
    var newValue: Double = 0.0

    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
    }

    init(progressBar ind: ESSProgressIndicator?, newDoubleValue val: Double) {
        super.init(duration: 0.3, animationCurve: .linear)
        progInd = ind
        initValue = progInd?.doubleValue ?? 0.0
        newValue = val
        animationBlockingMode = .nonblockingThreaded
    }

    override var currentProgress: NSAnimation.Progress {
        didSet {
            let delta = newValue - (progInd?.doubleValue ?? 0.0)

            if !(progInd?.animateDisabled ?? true) &&
                (delta * (newValue - initValue)) > 0 {
                let newDoubleValue = progInd?.doubleValue ?? 0.0 + (delta * Double(currentValue))
                progInd?.doubleValue = newDoubleValue
            }
        }
    }
}

@objc(ESSProgressIndicator) class ESSProgressIndicator: NSProgressIndicator {
    private var sAnim: ESSProgressBarAnimation?

    var toValue:Double = 0.0
    var animateDisabled = false

    func cleanOldAnimation() {
        if self.sAnim != nil {
            let oldToValue = self.sAnim?.newValue ?? 0.0
            self.sAnim?.stop()
            self.sAnim = nil
            self.doubleValue = oldToValue
        }
    }

    func jump(toDoubleValue val: Double) {
        cleanOldAnimation()
        animateDisabled = true
        toValue = val
        super.doubleValue = self.toValue
    }

    func animate(toDoubleValue val: Double) {
        cleanOldAnimation()
        animateDisabled = false
        toValue = val
        self.sAnim = ESSProgressBarAnimation(
            progressBar: self,
            newDoubleValue: toValue)
        self.sAnim?.start()
    }
}
