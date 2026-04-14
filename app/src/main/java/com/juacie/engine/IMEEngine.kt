package com.juacie.engine

import com.juacie.core.EngineOutput
import com.juacie.core.EngineState
import com.juacie.core.ImeAction
import com.juacie.core.InputMode
import com.juacie.core.Reducer

class IMEEngine(private val reducer: Reducer) {
    private var state = EngineState()
    val currentState: EngineState
        get() = state

    fun dispatch(action: ImeAction): EngineOutput {
        state = reducer.reduce(state, action)

        val output = EngineOutput(
            composingText = state.composing.ifEmpty { null },
            candidates = if (state.mode == InputMode.Predicting) {
                state.predictingCandidates
            } else {
                state.candidates
            },
            selectedIndex = state.selectedIndex,
            mode = state.mode,
            commitText = state.commitText,
            deleteBeforeCursor = state.deleteBeforeCursor
        )

        // commit / delete 都是 event，回傳後清掉
        state = state.copy(
            commitText = null,
            deleteBeforeCursor = false
        )

        return output
    }
}