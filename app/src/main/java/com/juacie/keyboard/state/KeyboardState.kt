//KeyboardState.kt
package com.juacie.keyboard.state

import com.juacie.keyboard.model.ImeLanguage
import com.juacie.keyboard.model.KeyboardMode

data class KeyboardState(
    val language: ImeLanguage = ImeLanguage(),
    val layoutConfig: LayoutConfig = LayoutConfig(),
    val inputType: Int = 0,
    val showLanguageMenu: Boolean = false,
    val animationShakeTick: Int = 0,
    val animationTick: Boolean = false,
)

data class LayoutConfig(
    val mode: KeyboardMode = KeyboardMode.LETTERS,
    val shiftState: ShiftState = ShiftState.OFF,
    val pageIndex: Int = 0,
    val showNumberRow: Boolean = false,
    val hasShift: Boolean = true,
)

enum class ShiftState {
    OFF,
    ON,        // 一次性
    CAPS_LOCK
}