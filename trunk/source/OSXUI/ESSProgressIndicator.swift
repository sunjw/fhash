//
//  ESSProgressIndicator.swift
//  fHash
//
//  Created by Sun Junwen on 2023/12/1.
//  Copyright © 2023 Sun Junwen. All rights reserved.
//
import Cocoa

class ESSProgressBarAnimation: NSAnimation {
    var progInd: ESSProgressIndicator?
    var initValue = 0.0
    var newValue = 0.0

    init(progressBar ind: NSProgressIndicator?, ewDoubleValue val: Double) {
        super.init(duration: 0.3, animationCurve: .linear)
        progInd = ind
        initValue = (progInd as? NSNumber)?.doubleValue ?? 0.0
        newValue = val
        animationBlockingMode = .nonblockingThreaded

        return nil
    }

    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
    }

    func setCurrentProgress(_ currentProgress: NSAnimation.Progress) {
        super.currentProgress = currentProgress

        let delta = newValue - progInd.doubleValue

        if !progInd.animateDisabled && (delta * (newValue - initValue)) > 0 {
            let newDoubleValue = progInd.doubleValue + (delta * currentValue)
            progInd.doubleValue = newDoubleValue
        }
    }
}

class ESSProgressIndicator: NSProgressIndicator {
    var toValue = 0.0
    var animateDisabled = false

    func jump(toDoubleValue val: Double) {
    }

    func animate(toDoubleValue val: Double) {
    }
}
