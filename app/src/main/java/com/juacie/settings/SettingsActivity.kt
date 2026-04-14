package com.juacie.settings

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import com.juacie.ImeApplication
import com.juacie.settings.ui.SettingsScreen

class SettingsActivity : ComponentActivity() {

    private val container by lazy {
        (application as ImeApplication).container
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        enableEdgeToEdge()

        setContent {
            SettingsScreen(
                settingRepository = container.settingsRepository,
                languageRepository = container.languageRepository,
                onBack = { finish() }
            )
        }
    }
}