package com.juacie

import android.content.Context
import android.view.inputmethod.InputMethodManager
import com.juacie.data.LanguageRepository
import com.juacie.data.SettingsRepository
import com.juacie.data.StickerRepository

class AppContainer(context: Context) {

    private val appContext = context.applicationContext
    private val imm =
        context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager


    val settingsRepository by lazy {
        SettingsRepository(appContext)
    }

    val stickerRepository by lazy {
        StickerRepository(appContext)
    }

    val languageRepository: LanguageRepository by lazy {
        LanguageRepository(imm = imm, packageName = appContext.packageName)
    }
}