package com.juacie.settings

import androidx.annotation.DrawableRes
import com.juacie.ime.BuildConfig
import com.juacie.ime.R
import com.juacie.ime.util.ToolObj.expireTimestamp

enum class KeyboardHeight(val horizontalScale: Float, val verticalScale: Float) {
    SMALL(0.55f, 0.25f),
    MEDIUM(0.6f, 0.3f),
    LARGE(0.65f, 0.35f),
    EXTRA(0.7f, 0.4f)
}

enum class CandidateHeight(val scale: Float) {
    SMALL(0.4f),
    MEDIUM(0.45f),
    LARGE(0.5f),
}

enum class ThemeColor {
    BLUE,
    GREEN,
    PURPLE,
    RED,
    BLACK,
    WHITE
}

enum class BackgroundImage(
    val label: String, @DrawableRes val resId: Int?, val expireDateStr: String? = null
) {
    NONE("None", null, null),
    BG_BLUE(
        "Blue",
        R.drawable.bg_blue,
        BuildConfig.BgExpireDate
    ),
    BG_ORANGE(
        "Orange",
        R.drawable.bg_orange,
        BuildConfig.BgExpireDate
    ),
    ;

    private val expireTimestamp: Long? = expireDateStr?.expireTimestamp()

    fun isExpired(now: Long = System.currentTimeMillis()): Boolean {
        if(expireTimestamp == null) return false
        return listOfNotNull(expireTimestamp, appExpireTimestamp)
            .any { now > it }
    }

    fun isAvailable(): Boolean = !isExpired()

    companion object {
        private val appExpireTimestamp: Long? = BuildConfig.AppExpireDate.expireTimestamp()

        fun availableEntries(): List<BackgroundImage> = entries.filter { it.isAvailable() }
        fun fallback(): BackgroundImage = NONE
    }
}