package com.juacie;

import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.InputConnection;

import com.android.inputmethod.pinyin.DecodingHandler;
import com.android.inputmethod.pinyin.DecoderRenderInterface;

import java.util.Collections;

public class KikaPinyinInputConnectionRender {

    private static final String TAG = "KikaPinyinICRender";

    private final DecoderRenderInterface mRender;
    private final EvaluationCallback mCallback;

    private static final String sDoubleSpacePeriod = "。";

    public enum FuncKey {
        COPY,
        CUT,
        SELECT_ALL,
        SELECT_CANCEL,
        DOUBLE_SPACE
    }

    private static final DecodingHandler sHandler = DecodingHandler.getInstance();

    private boolean mEnableNextWordSuggestion;

    public KikaPinyinInputConnectionRender(final DecoderRenderInterface render) {
        mRender = render;
        mCallback = new EvaluationCallback();

        reset(null, 0, false, false);
    }

    public boolean isInit() {
        return sHandler != null && sHandler.isInited();
    }

    public void initSession(final String dictPath, final String userDictPath) {
        sHandler.init(dictPath, userDictPath);
    }

    public void update(final InputConnection inputConnection, final int imeOptions) {
        mRender.setInputConnection(inputConnection, imeOptions);
    }

    public void reset() {
        sHandler.reset(false, new DecodingHandler.EvaluationCallback() {
            @Override
            public void onCompleted(DecodingHandler.Result result) {
                if (mRender.getInputConnection() == null) {
                    mRender.onUpdateCandidateListAndCommit("", Collections.<String>emptyList(), "");
                } else {
                    mRender.onUpdateCandidateListAndCommit(result.getComposing(), result.getCandidates(), "");
                }
            }
        });
    }

    public void reset(final InputConnection inputConnection, final int imeOptions,
                      final boolean nextWordSuggestion, final boolean isRestart) {
        sHandler.reset(isRestart, new DecodingHandler.EvaluationCallback() {
            @Override
            public void onCompleted(DecodingHandler.Result result) {
                mRender.setInputConnection(inputConnection, imeOptions);
                mEnableNextWordSuggestion = nextWordSuggestion;

                if (inputConnection == null) {
                    mRender.onUpdateCandidateListAndCommit("", Collections.<String>emptyList(), "");
                } else {
                    mRender.onUpdateCandidateListAndCommit(result.getComposing(), result.getCandidates(), "");
                }
            }
        });
    }

    public void onKey(final KeyEvent keyEvent) {
        /*
        if (keyCode >= KeyEvent.KEYCODE_A && keyCode <= KeyEvent.KEYCODE_Z) {
            keyChar = keyCode - KeyEvent.KEYCODE_A + 'a';
        } else if (keyCode >= KeyEvent.KEYCODE_0 && keyCode <= KeyEvent.KEYCODE_9) {
            keyChar = keyCode - KeyEvent.KEYCODE_0 + '0';
        } else if (keyCode == KeyEvent.KEYCODE_COMMA) {
            keyChar = ',';
        } else if (keyCode == KeyEvent.KEYCODE_PERIOD) {
            keyChar = '.';
        } else if (keyCode == KeyEvent.KEYCODE_SPACE) {
            keyChar = ' ';
        } else if (keyCode == KeyEvent.KEYCODE_APOSTROPHE) {
            keyChar = '\'';
        }
        */
        sHandler.onKey(keyEvent.getKeyCode(), (char)keyEvent.getUnicodeChar(), mCallback);
    }

    public boolean isFinishedComposing() {
        return sHandler.isFinishedComposing();
    }


    public void input(String s) {
        if (s.length() == 1) {
            // string to keyChar to keyCode
            if (s.matches("[a-z]")) {
                char keyChar = s.charAt(0);
                int keyCode = keyChar + KeyEvent.KEYCODE_A - 'a';
                sHandler.onKey(keyCode, keyChar, mCallback);
                return;
            }
            if (s.matches("[0-9]")) {
                char keyChar = s.charAt(0);
                int keyCode = keyChar + KeyEvent.KEYCODE_0 - '0';
                sHandler.onKey(keyCode, keyChar, mCallback);
                return;
            }
            if (" ".equals(s)) { // input space to get predictions.
                sHandler.onKey(KeyEvent.KEYCODE_SPACE, ' ', mCallback);
                return;
            }
        }
        sHandler.input(s, mCallback);
    }

    public void finishAndInput(String s) {
        sHandler.input(s, mCallback);
    }

    public void onInputApostrophe() {
        sHandler.onKey(KeyEvent.KEYCODE_APOSTROPHE, '\'', mCallback);
    }

    public void onDelete() {
        sHandler.onKey(KeyEvent.KEYCODE_DEL, (char)0, mCallback);
    }

    public void onSelectCandidate(int candidateId) {
        if (candidateId >= 0)
            sHandler.onSelectCandiate(candidateId, mCallback);
        else // special handle of composing candidate
            sHandler.onKey(KeyEvent.KEYCODE_SPACE, ' ', mCallback);

    }

    public String getComposingString() {
        return sHandler.getComposingString();
    }

    public int getCandidateCount() {
        return sHandler.getCandidateCount();
    }
    public void onArrow(final int keyEventCode, final boolean isSelection) {
        sHandler.passCallBack(new DecodingHandler.EvaluationCallback() {
            @Override
            public void onCompleted(DecodingHandler.Result result) {
                if (mRender.getInputConnection() == null)
                    return;

                mRender.onArrow(keyEventCode, isSelection);
            }
        });
    }

    public void onFuncKey(final FuncKey funcKey) {
        sHandler.passCallBack(new DecodingHandler.EvaluationCallback() {
            @Override
            public void onCompleted(DecodingHandler.Result result) {
                if (mRender.getInputConnection() == null)
                    return;

                switch (funcKey) {
                    case COPY:
                        mRender.onCopy();
                        break;
                    case CUT:
                        mRender.onCut();
                        break;
                    case SELECT_ALL:
                        mRender.onSelectAll();
                        break;
                    case SELECT_CANCEL:
                        mRender.onSelectCancel();
                        break;
                    case DOUBLE_SPACE:
                        mRender.onDoubleSpace(sDoubleSpacePeriod);
                        break;
                    default:
                        break;
                }
            }
        });
    }

    public void onUpdateStatusIcon(DecoderRenderInterface.STATUS status) {
        mRender.onUpdateStatusIcon(status);
    }

    ////////////////////////////////////////////////////////////////////
    /* the following functions should be used in sHandler */
    private class EvaluationCallback implements DecodingHandler.EvaluationCallback {
        private static final String TAG = "EvaluationCallback";

        @Override
        public void onCompleted(DecodingHandler.Result result) {
            if(mRender.getInputConnection() == null || result == null) {
                Log.w(TAG, "###onCompleted() mRender.getInputConnection(): " + mRender.getInputConnection() + ", result: " + result);
                return;
            }

            final int inputKeyCode = result.getInputKeyCode();

//            if (BuildConfig.DEBUG)
                Log.d(TAG, String.format("###onCompleted() inputKeyCode %d, result.isConsumed(): %s", inputKeyCode, result.isConsumed()));

            if (result.isConsumed()) {
                final String commit = result.getCommit();
                final String composing = result.getComposing();

                if (!mEnableNextWordSuggestion && TextUtils.isEmpty(composing))
                    mRender.onUpdateCandidateListAndCommit(composing, Collections.<String>emptyList(), commit);
                else {
                    mRender.onUpdateCandidateListAndCommit(composing, result.getCandidates(), commit);
                }

//                if (BuildConfig.DEBUG) {
                    Log.d(TAG, String.format("consumed, result.mCursor: %s, result.mComposing: %s", result.getCommit(), result.getComposing()));
                    Log.d(TAG, String.format("commit:%s, composing:%s", commit, composing));
//                    Log.d(TAG, "candidates:"+ result.getCandidates());
//                }

            } else {
                // not consumed by engine, such key events should be handled outside
                // TODO: handle key events
                switch (inputKeyCode) {
                    case KeyEvent.KEYCODE_DEL:
                        mRender.onUpdateCandidateListAndCommit(result.getComposing(), result.getCandidates(), "");
                        mRender.onDelete();
                        break;
                    case KeyEvent.KEYCODE_ENTER:
                        mRender.onUpdateCandidateListAndCommit(result.getComposing(), result.getCandidates(), "");
                        mRender.onEnter();
                        break;
                    default:
                        int keyUnicodeChar = result.getInputUnicodeChar();
                        if (Character.isValidCodePoint(keyUnicodeChar) && keyUnicodeChar != 0)
                            mRender.onUpdateCandidateListAndCommit(result.getComposing(), result.getCandidates(), new String(Character.toChars(keyUnicodeChar)));
                        else
                            mRender.onUpdateCandidateListAndCommit(result.getComposing(), result.getCandidates(), "");
                        break;
                }
            }

        }
    }
}
