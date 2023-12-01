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
    var progInd: ESSProgressIndicator?
    var initValue: Double = 0.0
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

private var sAnim: ESSProgressBarAnimation?

@objc class ESSProgressIndicator: NSProgressIndicator {
    @objc var toValue:Double = 0.0
    var animateDisabled = false

    func cleanOldAnimation() {
        if sAnim != nil {
            let oldToValue = sAnim?.newValue ?? 0.0
            sAnim?.stop()
            sAnim = nil
            self.doubleValue = oldToValue
        }
    }

    @objc func jump(toDoubleValue val: Double) {
        cleanOldAnimation()
        animateDisabled = true
        toValue = val
        super.doubleValue = self.toValue
    }

    @objc func animate(toDoubleValue val: Double) {
        cleanOldAnimation()
        animateDisabled = false
        toValue = val
        sAnim = ESSProgressBarAnimation(
            progressBar: self,
            newDoubleValue: toValue)
        sAnim?.start()
    }
}
