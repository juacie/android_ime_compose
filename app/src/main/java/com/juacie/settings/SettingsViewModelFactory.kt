package com.juacie.settings

import com.juacie.data.LanguageRepository
import com.juacie.data.SettingsRepository

class SettingsViewModelFactory(
    private val settingRepository: SettingsRepository,
    private val languageRepository: LanguageRepository
) : androidx.lifecycle.ViewModelProvider.Factory {

    override fun <T : androidx.lifecycle.ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(SettingsViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return SettingsViewModel(settingRepository, languageRepository) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}