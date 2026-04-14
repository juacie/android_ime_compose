package com.android.inputmethod.pinyin;

import android.view.inputmethod.InputConnection;

import java.util.List;

public interface DecoderRenderInterface {
    void onUpdateCandidateListAndCommit(String composing, List<String> candidates, String commit);

    void setInputConnection(InputConnection inputConnection, int imeOption);
    InputConnection getInputConnection();
    void onDelete();
    void onEnter();
    void onArrow(int keyEventCode, boolean isSelection);
    void onCopy();
    void onCut();
    void onSelectAll();
    void onSelectCancel();
    void onDoubleSpace(String commitText);

    enum STATUS {
        DEFAULT,
        SHITFTED,
        CAPSLOCK,
        ALTED,
        ALTLOCK
    };

    void onUpdateStatusIcon(STATUS status);
}
