package com.juacie.core

interface Reducer {
    fun reduce(state: EngineState, action: ImeAction): EngineState
}