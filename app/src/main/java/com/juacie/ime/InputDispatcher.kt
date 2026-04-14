package com.juacie.ime

import com.juacie.core.ImeAction
import com.juacie.engine.IMEEngine
import com.juacie.keyboard.model.KeyboardLanguage

class InputDispatcher(
    private val engine: IMEEngine,
    private val language: KeyboardLanguage,
    private val renderer: IMERenderer
) {

    fun dispatch(action: ImeAction) {
        val output = engine.dispatch(action)
        renderer.render(output,language)
    }

}