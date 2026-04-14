package com.juacie;

import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.InputConnection;

import com.android.inputmethod.pinyin.DecoderRenderInterface;
import com.android.inputmethod.pinyin.DecodingHandler;
import com.kikatech.inputmethod.pinyin.BuildConfig;

import java.util.Collections;

public class KikaZhuyinInputConnectionRender {

    private static final String TAG = "KikaZhuyinICRender";

    private final DecoderRenderInterface mRender;
    private final EvaluationCallback mCallback;

    private static final String sDoubleSpacePeriod = "。";

    private static final DecodingHandler.ZhuyinDecodingHandler sHandler = DecodingHandler.ZhuyinDecodingHandler.getInstance();

    public enum FuncKey {
        COPY,
        CUT,
        SELECT_ALL,
        SELECT_CANCEL,
        DOUBLE_SPACE
    }

    public KikaZhuyinInputConnectionRender(final DecoderRenderInterface render) {
        mRender = render;
        mCallback = new EvaluationCallback();

        reset( null, 0, false);
    }

    public void initSession(final String dictPath, final String userDictPath) {
        sHandler.init(dictPath, userDictPath);
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

    public void reset(final InputConnection inputConnection, final int imeOptions, final boolean isRestart) {
        sHandler.reset(isRestart, new DecodingHandler.EvaluationCallback() {
            @Override
            public void onCompleted(DecodingHandler.Result result) {
                mRender.setInputConnection(inputConnection, imeOptions);
                if (inputConnection == null) {
                    mRender.onUpdateCandidateListAndCommit("", Collections.<String>emptyList(), "");
                } else {
                    mRender.onUpdateCandidateListAndCommit(result.getComposing(), result.getCandidates(), "");
                }
            }
        });
    }

    public void onKey(final KeyEvent keyEvent) {
        sHandler.onKey(keyEvent.getKeyCode(), (char)keyEvent.getUnicodeChar(), keyEvent.getRepeatCount()==0, mCallback);
    }


    public void inputZhuyin(String s) {
        sHandler.inputZhuyin(s, mCallback);
    }

    public void updateTailZhuyin(String s) {
        sHandler.updateTailZhuyin(s, mCallback);
    }

    public void input(String s) {
        if (" ".equals(s)) { // input space to get predictions.
            sHandler.onKey(KeyEvent.KEYCODE_SPACE, ' ', true, mCallback);
            return;
        }
        sHandler.input(s, mCallback);
    }

    public void finishAndInput(String inputString) {
        sHandler.input(inputString, mCallback);
    }

    public String getComposingString() {
        return sHandler.getComposingString();
    }

//    public int getCandidateCount() {
//        return sHandler.getCandidateCount();
//    }

    public void onDelete() {
        sHandler.onKey(KeyEvent.KEYCODE_DEL, (char)0, true, mCallback);
    }

    public void onSelectCandidate(int candidateId) {
        if (candidateId >= 0)
            sHandler.onSelectCandidate(candidateId, mCallback);
        else // special handle of composing candidate
            sHandler.onKey(KeyEvent.KEYCODE_ENTER, (char)0, true, mCallback);

    }

    public void onArrow(final int keyEventCode, final boolean isSelection) {
        reset();
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
                        if (TextUtils.isEmpty(result.getComposing()))
                            mRender.onDoubleSpace(sDoubleSpacePeriod);
                        else
                            onKey(new KeyEvent(KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_SPACE));
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
            if(mRender.getInputConnection() == null || result == null)
                return;

            final int inputKeyCode = result.getInputKeyCode();

            if (BuildConfig.DEBUG)
                Log.d(TAG, String.format("inputKeyCode %d", inputKeyCode));


            if (result.isConsumed()) {
                mRender.onUpdateCandidateListAndCommit(result.getComposing(), result.getCandidates(), result.getCommit());

                if (BuildConfig.DEBUG)
                    Log.d(TAG, String.format("consumed, result.mCursor: %s, result.mComposing: %s", result.getCommit(), result.getComposing()));
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
