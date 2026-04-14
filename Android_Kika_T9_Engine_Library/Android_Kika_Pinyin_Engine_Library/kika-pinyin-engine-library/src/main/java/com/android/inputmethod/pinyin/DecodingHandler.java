package com.android.inputmethod.pinyin;

import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;

import com.kikatech.inputmethod.pinyin.BuildConfig;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Stack;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class DecodingHandler {
    private static final String TAG = "DecodingHandler";

    //private static final String sUserDictName = "pinyin.iqqi";

    private final DecodingInfo mDecodingInfo = new DecodingInfo();

    private static final ExecutorService sExecutor = Executors.newSingleThreadExecutor(); // for engine
    private static final Handler sHandler = new Handler(Looper.getMainLooper()); // for callback

    private int mCandidateFetchSize = 0;

    private String mCommitStr;

    private enum ImeState {
        STATE_IDLE, STATE_INPUT, STATE_COMPOSING, STATE_PREDICT
    }
    private ImeState mImeState;

    private static final DecodingHandler sDecodingHandler = new DecodingHandler();
    public static DecodingHandler getInstance() {
        return sDecodingHandler;
    }

    private DecodingHandler() {
    }



    static ExecutorService getEngineExecutor() {
        return sExecutor;
    }

    private static String sDictPath = "";
    private static String sUserDictPath = "";

    public static class Result {
        private final int mInputKeyCode;
        private final int mInputUnicodeChar;
        private final String mComposing;
        private final String mCommit;
        private List<String> mCandidates;
        private final boolean mIsConsumed;

        Result(int inputKeyCode, int inputUnicodeChar, String composing, String commit, List<String> candidates,
                boolean isConsumed) {
            mInputKeyCode = inputKeyCode;
            mInputUnicodeChar = inputUnicodeChar;
            mComposing = composing;
            mCommit = commit;
            mCandidates = candidates;
            mIsConsumed = isConsumed;
        }

        public List<String> getCandidates() {
            return mCandidates;
        }

        public String getComposing() {
            return mComposing;
        }

        public String getCommit() {
            return mCommit;
        }

        public boolean isConsumed() {
            return mIsConsumed;
        }

        public int getInputKeyCode() {
            return mInputKeyCode;
        }

        public int getInputUnicodeChar() {
            return mInputUnicodeChar;
        }
    }

    public interface EvaluationCallback {
        void onCompleted(Result result);
    }

    public boolean isInited() {
        return mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, false);
    }

    boolean _isInit(final String dictPath, final String userDictPath, boolean isZhuyin) {
        boolean hasInit = !TextUtils.isEmpty(dictPath) &&
                mDecodingInfo.mIPinyinDecoder.initPinyinEngine(dictPath, userDictPath, isZhuyin);
        if(BuildConfig.DEBUG) {
            Log.e(TAG, "_isInit:" + hasInit);
        }
        return hasInit;
    }

    void _init(final String dictPath, final String userDictPath) {
        mDecodingInfo.mIPinyinDecoder.initPinyinEngine(dictPath, userDictPath, false);
        if (mDecodingInfo.mIPinyinDecoder._isInited(dictPath, userDictPath, false)) {
            sDictPath = dictPath;
            sUserDictPath = userDictPath;
            resetToIdleState();
        }
    }

    public void init(final String dictPath, final String userDictPath) {
        sExecutor.submit(new Runnable() {
            @Override
            public void run() {
                _init(dictPath, userDictPath);
            }
        });
    }

    public void onKey(final int keyCode, final char keyChar, final EvaluationCallback callback) {
        sExecutor.submit(new Runnable() {
            @Override
            public void run() {
                if (!mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, false)) {
                    Log.w(TAG, "###onKey() Pinyin decoding engine is not initialized");
                    return;
                }

                final boolean isConsumed = processKeyCode(keyCode, keyChar);
                final String composing = getComposingString();
                final List<String> candidates = new ArrayList<>(getCandidates());

                if (candidates.isEmpty() && !TextUtils.isEmpty(composing))
                    candidates.add(composing);

                final Result result = new Result(keyCode, keyChar, composing, getCommitString(), candidates, isConsumed);
                cleanCommitString();

                sHandler.post(new Runnable() {
                    public void run() {
                        callback.onCompleted(result);
                    }
                });
            }
        });
    }

    boolean isFinishInput = true;

    public boolean isFinishedComposing() {
        return isFinishInput;
    }

    // the composing is finished after calling this
    public void input(final String s, final EvaluationCallback callback) {
        isFinishInput = s == null || s.equals("");
        if(BuildConfig.DEBUG) {
            Log.e(TAG, "input str:"+ s + " finish:"+ isFinishInput);
        }

        sExecutor.submit(new Runnable() {
            @Override
            public void run() {
                if (!mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, false))
                    return;

                final String commit;

                if (TextUtils.isEmpty(getComposingString())) {
                    // prediction words should not auto commit
                    commit = s;
                } else if (mDecodingInfo.mCandidatesList.size() > 0 && onSelectCandiate(0)) {
                    commit = getCommitString() + s;
                    cleanCommitString();
                } else {
                    commit = getComposingString() + s;
                }

                final Result result;

                if (s.equals("，")) {
                    final List<String> candidates = new ArrayList<>(sDecodingHandler.getCandidates());
                    result = new Result(0, 0,"", commit, candidates, true);
                    if (candidates.isEmpty())
                        resetToIdleState();
                } else {
                    result = new Result(0, 0,"", commit, Collections.<String>emptyList(), true);
                    resetToIdleState();
                }

                sHandler.post(new Runnable() {
                    public void run() {
                        callback.onCompleted(result);
                    }
                });
            }
        });
    }


    /*
    public void fetchMoreCandidates(int fetchSize) {
        if (fetchSize <= mDecodingInfo.mCandidatesList.size()
                || mDecodingInfo.mTotalChoicesNum <= mDecodingInfo.mCandidatesList.size())
            return;
        mDecodingInfo.fetchCandidates(fetchSize);
        mDecoderRender.onUpdateMoreCandidates(mDecodingInfo.mCandidatesList);
    }

    public boolean setActiveCandiatePos(int index) {
        if (index < 0 || index >= mDecodingInfo.mCandidatesList.size())
            return false;
        mDecodingInfo.setActiveCandiatePos(index);
        return true;
    }
    */

    public void onSelectCandiate(final int index, final EvaluationCallback callback) {
        sExecutor.submit(new Runnable() {
            @Override
            public void run() {
                if (!mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, false))
                    return;

                if (index < 0 || index >= mDecodingInfo.mCandidatesList.size()) {
                    // the selected word should be the composing string

                    final String commit = getComposingString();
                    if (TextUtils.isEmpty(commit))
                        return;

                    resetToIdleState();

                    final Result result = new Result(0, 0, "", commit, Collections.<String>emptyList(), true);

                    sHandler.post(new Runnable() {
                        public void run() {
                            callback.onCompleted(result);
                        }
                    });
                    return;
                }

                chooseAndUpdate(index);
                final List<String> candidates = new ArrayList<>(getCandidates());
                final Result result = new Result(0, 0, getComposingString(), getCommitString(), candidates, true);
                cleanCommitString();

                sHandler.post(new Runnable() {
                    public void run() {
                        callback.onCompleted(result);
                    }
                });
            }
        });
    }



    public int getCandidateCount() {
        int ret = mDecodingInfo.mCandidatesList.size();
        if (BuildConfig.DEBUG) {
            Log.d(TAG, "getCandidateCount:" + ret);
        }

        return ret;
    }

    private boolean onSelectCandiate(int index) {
        if (index < 0 || index >= mDecodingInfo.mCandidatesList.size())
            return false;
        chooseAndUpdate(index);
        return true;
    }


    public void passCallBack(final EvaluationCallback callback) {
        sExecutor.submit(new Runnable() {
            @Override
            public void run() {
                final String composing = getComposingString();
                final List<String> candidates = new ArrayList<>(getCandidates());

                final Result result = new Result(0, 0, composing, "", candidates, true);

                sHandler.post(new Runnable() {
                    public void run() {
                        callback.onCompleted(result);
                    }
                });
            }
        });
    }

    public void update(final boolean isRestart, final EvaluationCallback callback) {
        sExecutor.submit(new Runnable() {
            @Override
            public void run() {
                if (!mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, false))
                    return;

                if (isRestart) {
                    resetToIdleState();
                }
                final String composing = getComposingString();
                final List<String> candidates = new ArrayList<>(getCandidates());

                final Result result = new Result(0, 0, composing, "", candidates, true);

                if (callback != null) {
                    sHandler.post(new Runnable() {
                        public void run() {
                            callback.onCompleted(result);
                        }
                    });
                }
            }
        });
    }

    public void reset(final boolean isRestart, final EvaluationCallback callback) {
        sExecutor.submit(new Runnable() {
            @Override
            public void run() {
                if (!mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, false))
                    return;

                if (!isRestart)
                    resetToIdleState();

                final String composing = getComposingString();
                final List<String> candidates = new ArrayList<>(getCandidates());

                final Result result = new Result(0, 0, composing, "", candidates, true);

                if (callback != null) {
                    sHandler.post(new Runnable() {
                        public void run() {
                            callback.onCompleted(result);
                        }
                    });
                }

            }
        });
    }


    public static class ZhuyinDecodingHandler {
        private static final ZhuyinDecodingHandler sZhuyinDecodingHandler = new ZhuyinDecodingHandler();
        public static ZhuyinDecodingHandler getInstance() {
            return sZhuyinDecodingHandler;
        }
        private ZhuyinDecodingHandler() {}

        public String getComposingString() {
            return this._displayComposingGen();
        }

        private static String sDictPath = "";
        private static String sUserDictPath = "";

        private static final StringBuilder sZhuyinComposing = new StringBuilder();
        private static final int[] sSplStart = new int[sDecodingHandler.mDecodingInfo.CH_STRING_MAX + 1];

        public void init(final String dictPath, final String userDictPath) {
            sExecutor.submit(new Runnable() {
                @Override
                public void run() {
                    sDecodingHandler.mDecodingInfo.mIPinyinDecoder.initPinyinEngine(dictPath, userDictPath, true);
                    if (sDecodingHandler.mDecodingInfo.mIPinyinDecoder._isInited(dictPath, userDictPath, true)) {
                        sDictPath = dictPath;
                        sUserDictPath = userDictPath;
                        sDecodingHandler.resetToIdleState();
                        sZhuyinComposing.setLength(0);
                    }
                }
            });
        }

        public void passCallBack(final EvaluationCallback callback) {
            sDecodingHandler.passCallBack(callback);
        }

        public void reset(final boolean isRestart, final DecodingHandler.EvaluationCallback callback) {
            sExecutor.submit(new Runnable() {
                @Override
                public void run() {
                    if (!sDecodingHandler.mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, true))
                        return;

                    if (!isRestart) {
                        sDecodingHandler.resetToIdleState();
                        _resetZhuyinComposing();
                    }

                    final List<String> candidates = new ArrayList<>(sDecodingHandler.getCandidates());
                    final Result result = new Result(0, 0, _displayComposingGen(), "", candidates, true);

                    sHandler.post(new Runnable() {
                        public void run() {
                            callback.onCompleted(result);
                        }
                    });
                }
            });
        }

        public void inputZhuyin(final String s, final DecodingHandler.EvaluationCallback callback) {
            sExecutor.submit(new Runnable() {
                @Override
                public void run() {
                    if (!sDecodingHandler.mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, true))
                        return;

                    _addZhuyin(s, false, callback);
                }
            });
        }

        public void updateTailZhuyin(final String s, final DecodingHandler.EvaluationCallback callback) {
            sExecutor.submit(new Runnable() {
                @Override
                public void run() {
                    if (!sDecodingHandler.mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, true))
                        return;

                    if (!ZhuyinToPinyinConverter.sValidInput.matcher(s).matches())
                        return;

                    if (sZhuyinComposing.length() == 0)
                        return;

                    final String zhuyinStr = sZhuyinComposing.toString();
                    final String zhuyinStrNew = zhuyinStr.substring(0, zhuyinStr.length()-1) + s;
                    final String pinyinStrNew = ZhuyinToPinyinConverter.translate(zhuyinStrNew, sSplStart);
                    if (pinyinStrNew.length() == 0 || pinyinStrNew.length() >= DecodingInfo.PY_STRING_MAX - 1  // align isSplStrFull
                            || sSplStart[0] == 0 || sSplStart[0] > DecodingInfo.CH_STRING_MAX)
                        return;

                    sZhuyinComposing.setLength(sZhuyinComposing.length() - 1);
                    sZhuyinComposing.append(s);
                    _updateComposingAndInput(pinyinStrNew);

                    final String composing = sDecodingHandler.getComposingString();
                    if (TextUtils.isEmpty(composing) && sZhuyinComposing.length() > 0) {
                        if (BuildConfig.DEBUG)
                            Log.e(TAG, String.format("unmatched composing. updateTailZhuyin %s, %s", s, sZhuyinComposing.toString()));
                        sZhuyinComposing.setLength(0);
                    }

                    final List<String> candidates = new ArrayList<>(sDecodingHandler.getCandidates());

                    if (candidates.isEmpty() && !TextUtils.isEmpty(composing))
                        candidates.add(sZhuyinComposing.toString()); // possible??

                    final Result result = new Result(0, 0, _displayComposingGen(), sDecodingHandler.getCommitString(), candidates, true);
                    sDecodingHandler.cleanCommitString();

                    sHandler.post(new Runnable() {
                        public void run() {
                            callback.onCompleted(result);
                        }
                    });
                }
            });
        }

        public void onKey(final int keyCode, final char keyChar, final boolean isFirstDown, final DecodingHandler.EvaluationCallback callback) {
            sExecutor.submit(new Runnable() {
                @Override
                public void run() {
                    if (!sDecodingHandler.mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, true))
                        return;

                    final boolean isConsumed;
                    // must simulate key events to keep engine sub-composing status.
                    switch (keyCode) {
                        case KeyEvent.KEYCODE_DEL:
                            final Stack<Boolean> inputHistory = sDecodingHandler.mDecodingInfo.mInputHistory;
                            if (inputHistory.size() > 0 && inputHistory.peek()) {
                                // the last action is choice, just bypass the keyEvent and update results
                                isConsumed = sDecodingHandler.processKeyCode(keyCode, keyChar);
                            } else if (sZhuyinComposing.length() > 0) {
                                // delete 1 zhuyin = simulate multiple pinyin inputs
                                sZhuyinComposing.setLength(sZhuyinComposing.length() - 1);
                                final String pinyinStrNew = ZhuyinToPinyinConverter.translate(sZhuyinComposing.toString(), sSplStart);
                                /*
                                if (pinyinStrNew.length() >= sDecodingHandler.mDecodingInfo.PY_STRING_MAX - 1  // align isSplStrFull
                                        || sSplStart[0] > sDecodingHandler.mDecodingInfo.CH_STRING_MAX)
                                    return;
                                */
                                _updateComposingAndInput(pinyinStrNew);
                                isConsumed = true;
                            } else {
                                sDecodingHandler.resetToIdleState();
                                sZhuyinComposing.setLength(0);
                                isConsumed = false;
                            }
                            break;
                        case KeyEvent.KEYCODE_ENTER:
                            if (sZhuyinComposing.length() > 0) {
                                _onSelectCandidate(0, callback);
                                return;
                            }
                            isConsumed = false;
                            break;
                        case KeyEvent.KEYCODE_SPACE:
                            if (sZhuyinComposing.length() > 0) {
                                _addZhuyin("ˉ", isFirstDown, callback);
                                return;
                            }
                            isConsumed = false;
                            break;
                        default:
                            isConsumed = false;
                            break;
                    }

                    final String composing = sDecodingHandler.getComposingString();
                    if (TextUtils.isEmpty(composing) && sZhuyinComposing.length() > 0) {
                        if (BuildConfig.DEBUG)
                            Log.e(TAG, String.format("unmatched composing. onKey %d %c, %s", keyCode, keyChar, sZhuyinComposing.toString()));
                        sZhuyinComposing.setLength(0);
                    }

                    final List<String> candidates = new ArrayList<>(sDecodingHandler.getCandidates());

                    if (candidates.isEmpty() && !TextUtils.isEmpty(composing))
                        candidates.add(sZhuyinComposing.toString()); // possible?

                    final Result result = new Result(keyCode, keyChar, _displayComposingGen(), sDecodingHandler.getCommitString(), candidates, isConsumed);
                    sDecodingHandler.cleanCommitString();

                    sHandler.post(new Runnable() {
                        public void run() {
                            callback.onCompleted(result);
                        }
                    });
                }
            });
        }

        // the composing is finished after calling this
        public void input(final String s, final DecodingHandler.EvaluationCallback callback) {
            sExecutor.submit(new Runnable() {
                @Override
                public void run() {
                    if (!sDecodingHandler.mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, true))
                        return;

                    final String commit;

                    if (TextUtils.isEmpty(sDecodingHandler.getComposingString())) {
                        // prediction words should not auto commit
                        commit = s;
                    } else if (sDecodingHandler.mDecodingInfo.mCandidatesList.size() > 0 && sDecodingHandler.onSelectCandiate(0)) {
                        commit = sDecodingHandler.getCommitString() + s;
                        sDecodingHandler.cleanCommitString();
                    } else {
                        commit = sZhuyinComposing.toString() + s;
                    }

                    final Result result;

                    if (s.equals("，")) {
                        final List<String> candidates = new ArrayList<>(sDecodingHandler.getCandidates());
                        result = new Result(0, 0,"", commit, candidates, true);
                    } else {
                        result = new Result(0, 0,"", commit, Collections.<String>emptyList(), true);
                        sDecodingHandler.resetToIdleState();
                    }

                    _resetZhuyinComposing();

                    sHandler.post(new Runnable() {
                        public void run() {
                            callback.onCompleted(result);
                        }
                    });
                }
            });
        }

        public void onSelectCandidate(final int index, final DecodingHandler.EvaluationCallback callback) {
            // TODO: subcomposing selection

            sExecutor.submit(new Runnable() {
                @Override
                public void run() {
                    if (!sDecodingHandler.mDecodingInfo.mIPinyinDecoder._isInited(sDictPath, sUserDictPath, true))
                        return;

                    _onSelectCandidate(index, callback);
                }
            });
        }

        private void _addZhuyin(final String s, final boolean isFirstKeyDown,
                final DecodingHandler.EvaluationCallback callback) {
            if (!ZhuyinToPinyinConverter.sValidInput.matcher(s).matches())
                return;

            if (ZhuyinToPinyinConverter.sValidToneInput.matcher(s).matches()) {
                int length = sZhuyinComposing.length();
                if (length > 0) {
                    String lastZhuyin = sZhuyinComposing.substring(length - 1);

                    // 2 whitespace -> submit candidate 0
                    if (s.equals("ˉ") && lastZhuyin.equals("ˉ") && isFirstKeyDown)
                        _onSelectCandidate(0, callback);

                        // continuous tone inputs are not allowed
                    else if (ZhuyinToPinyinConverter.sValidToneInput.matcher(lastZhuyin).matches())
                        return;
                } else if (length == 0)
                    return;
            }

            final String zhuyinStrNew = sZhuyinComposing.toString() + s;
            final String pinyinStrNew = ZhuyinToPinyinConverter.translate(zhuyinStrNew, sSplStart);
            if (pinyinStrNew.length() == 0 || pinyinStrNew.length() >= DecodingInfo.PY_STRING_MAX - 1  // align isSplStrFull
                    || sSplStart[0] == 0 || sSplStart[0] > DecodingInfo.CH_STRING_MAX)
                return;

            sZhuyinComposing.append(s);
            _updateComposingAndInput(pinyinStrNew);

            final String composing = sDecodingHandler.getComposingString();
            if (TextUtils.isEmpty(composing) && sZhuyinComposing.length() > 0) {
                if (BuildConfig.DEBUG)
                    Log.e(TAG, String.format("unmatched composing. _addZhuyin %s, %s", s, sZhuyinComposing.toString()));
                sZhuyinComposing.setLength(0);
            }

            final List<String> candidates = new ArrayList<>(sDecodingHandler.getCandidates());

            if (candidates.isEmpty() && !TextUtils.isEmpty(composing))
                candidates.add(sZhuyinComposing.toString()); // possible??

            final Result result = new Result(0, 0, _displayComposingGen(), sDecodingHandler.getCommitString(),
                    candidates, true);
            sDecodingHandler.cleanCommitString();

            sHandler.post(new Runnable() {
                public void run() {
                    callback.onCompleted(result);
                }
            });
        }

        private void _onSelectCandidate(final int index, final DecodingHandler.EvaluationCallback callback) {
            if (index < 0 || index >= sDecodingHandler.mDecodingInfo.mCandidatesList.size()) {
                // the selected word should be the composing string

                final String commit = sZhuyinComposing.toString();
                if (TextUtils.isEmpty(commit))
                    return;

                sDecodingHandler.resetToIdleState();
                _resetZhuyinComposing();

                final Result result = new Result(0, 0, "", commit, Collections.<String>emptyList(), true);

                sHandler.post(new Runnable() {
                    public void run() {
                        callback.onCompleted(result);
                    }
                });
                return;
            }

            sDecodingHandler.chooseAndUpdate(index);

            // TODO: render sub composing words
            if (sDecodingHandler.getComposingString().length() == 0)
                _resetZhuyinComposing();

            final List<String> candidates = new ArrayList<>(sDecodingHandler.getCandidates());
            final Result result = new Result(0, 0, _displayComposingGen(), sDecodingHandler.getCommitString(),
                    candidates, true);
            sDecodingHandler.cleanCommitString();

            sHandler.post(new Runnable() {
                public void run() {
                    callback.onCompleted(result);
                }
            });
        }

        private void _updateTailZhuyin(String s) {

        }

        private void _updateComposingAndInput(final String pinyinStrNew) {
            final String pinyinStrOld = sDecodingHandler.mDecodingInfo.mSurface.toString();

            int indexOfDifference = ZhuyinToPinyinConverter.indexOfDifference(pinyinStrOld, pinyinStrNew);
            for (int i = pinyinStrOld.length(); i > indexOfDifference; i--)
                sDecodingHandler.processKeyCode(KeyEvent.KEYCODE_DEL, '\0');
            for (int i = indexOfDifference; i < pinyinStrNew.length(); i++)
                sDecodingHandler.processKeyCode(0, pinyinStrNew.charAt(i));
        }

        private String _displayComposingGen() {
            if (sZhuyinComposing.length() == 0)
                return BuildConfig.DEBUG? sDecodingHandler.getComposingString(): "";

            StringBuilder sb = new StringBuilder();

            if (0 == sDecodingHandler.mDecodingInfo.mSurfaceDecodedLen
                    || sDecodingHandler.mDecodingInfo.mTotalChoicesNum == 0 /* workaround */) {
                sb.append(sZhuyinComposing.toString());
            } else {
                String chStr = sDecodingHandler.mDecodingInfo.mFullSent.substring(0,
                        sDecodingHandler.mDecodingInfo.mFixedLen);
                if (chStr.length() >= sSplStart[0]) {
                    if (BuildConfig.DEBUG)
                        Log.e(TAG, String.format("_displayComposingGen unexpected chStr %s, %s, %s", chStr,
                                sZhuyinComposing.toString(),
                                sDecodingHandler.mDecodingInfo.getComposingStrForDisplay()));
                    sb.append(sZhuyinComposing.toString());
                } else {
                    sb.append(chStr);
                    int i;
                    for (i = chStr.length() + 1; i + 1 < sSplStart.length && i < sSplStart[0]; i++) {
                        sb.append(sZhuyinComposing.substring(sSplStart[i], sSplStart[i + 1]));
                        //sb.append("　");
                        sb.append(" ");
                    }
                    if (i < sSplStart.length)
                        sb.append(sZhuyinComposing.substring(sSplStart[i]));
                }
            }

            /*
            int i;
            for (i = 1; i + 1 < sSplStart.length && i < sSplStart[0]; i++) {
                sb.append(sZhuyinComposing.substring(sSplStart[i], sSplStart[i + 1]));
                //sb.append("　");
                sb.append(" ");
            }

            */

            return BuildConfig.DEBUG? sb.toString() + sDecodingHandler.getComposingString(): sb.toString();
        }

        private void _resetZhuyinComposing() {
            sZhuyinComposing.setLength(0);
            sSplStart[0] = 0;
        }
    }

    public String getComposingString() {
        return mDecodingInfo.mComposingStrDisplay;
    }

    private String getCommitString() {
        return mCommitStr;
    }

    private void cleanCommitString() {
        mCommitStr = "";
    }

    private List<String> getCandidates() {
        return mDecodingInfo.mCandidatesList;
    }

    /*
    private void setDefaultCandidateFetchSize(int size) {
        if (size <= 0)
            return;

        mDefaultCandidateFetchSize = size;
    }


    public boolean onKeyDown(KeyEvent event) {
        if (processKey(event)) return true;
        return false;
    }

    public boolean onKeyUp(KeyEvent event) {
        if (processKey(event)) return true;
        return false;
    }
    */


    private boolean processKeyCode(int keyCode, char keyChar) {
        if (mImeState == ImeState.STATE_IDLE) {
            return processStateIdle(keyCode, keyChar);
        } else if (mImeState == ImeState.STATE_INPUT) {
            return processStateInput(keyCode, keyChar);
        } else if (mImeState == ImeState.STATE_PREDICT) {
            return processStatePredict(keyCode, keyChar);
        } else if (mImeState == ImeState.STATE_COMPOSING) {
            return processStateEditComposing(keyCode, keyChar);
        }

        return false;
    }

    private boolean processStateIdle(int keyCode, char keyChar) {
        // In this status, when user presses keys in [a..z], the status will
        // change to input state.
        if (keyChar >= 'a' && keyChar <= 'z') {
            mImeState = ImeState.STATE_INPUT;
            processSurfaceChange(keyCode, keyChar);
            return true;
        }
        return false;

        /*else if (keyCode == KeyEvent.KEYCODE_DEL) {
            return mDecoderRender.onDeleteLeftOne();
        } else if (keyCode == KeyEvent.KEYCODE_ENTER) {
            mDecoderRender.onCommitText("\n");
            return true;
        } else if (keyChar == ',' || keyChar == '.' || keyChar == ' ') {
            mDecoderRender.onCommitText(getCommaPeriod(keyChar));
            return true;
        } else {
            mDecoderRender.onCommitText(String.valueOf((char) keyChar));
            return true;
        }
        // TODO: investigate following state
        /*
        else if (event.isAltPressed()) {
            char fullwidth_char = KeyMapDream.getChineseLabel(keyCode);
            if (0 != fullwidth_char) {
                if (realAction) {
                    String result = String.valueOf(fullwidth_char);
                    commitResultText(result);
                }
                return true;
            } else {
                if (keyCode >= KeyEvent.KEYCODE_A
                        && keyCode <= KeyEvent.KEYCODE_Z) {
                    return true;
                }
            }
        }  else {
            String result = String.valueOf((char) keyChar);
            commitResultText(result);
            return true;
        }
        return false; */
    }

    private boolean processSurfaceChange(int keyCode, char keyChar) {
        if (keyCode == KeyEvent.KEYCODE_DEL) {
            mDecodingInfo.prepareDeleteBeforeCursor();
            chooseAndUpdate(-1);
        } else {
            if (mDecodingInfo.isSplStrFull() || (keyChar == '\'' && mDecodingInfo.charBeforeCursorIsSeparator())) {
                // TODO: find better way to handle this
                //mDecoderRender.onUpdateComposing(mDecodingInfo.getComposingStrForDisplay(), mDecodingInfo.mCursorPos);
                //mDecoderRender.onUpdateCandidates(mDecodingInfo.mCandidatesList, mDecodingInfo.mTotalChoicesNum, -1);
            } else if ((keyChar >= 'a' && keyChar <= 'z') || keyChar == '\''
                    || (mDecodingInfo.mIPinyinDecoder.isZhuyinMode() && keyChar >= '1' && keyChar <= '5')
                    || ((keyChar == ' ') && ImeState.STATE_COMPOSING == mImeState)) {
                mDecodingInfo.addSplChar(keyChar);
                chooseAndUpdate(-1);
            }
        }

        return true;
    }

    private boolean processStateInput(int keyCode, char keyChar) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_SPACE:
                if (mDecodingInfo.mCandidatesList.isEmpty()) {
                    mCommitStr = mDecodingInfo.getComposingStr();
                    resetToIdleState();
                } else {
                    onSelectCandiate(0);
                }
                return true;
            case KeyEvent.KEYCODE_ENTER:
                //mDecoderRender.onCommitText(mDecodingInfo.getComposingStr());
                mCommitStr = mDecodingInfo.getComposingStr();
                resetToIdleState();
                return true;
            case KeyEvent.KEYCODE_BACK: // TODO: decide back key priority
                resetToIdleState();
                return true;
            default:
                if ((keyChar >= 'a' && keyChar <= 'z')
                        || (mDecodingInfo.mIPinyinDecoder.isZhuyinMode() && keyChar >= '1' && keyChar <= '5')
                        || keyChar == '\'' || keyCode == KeyEvent.KEYCODE_DEL)
                    return processSurfaceChange(keyCode, keyChar);

                String s;
                if (keyChar == ',' || keyChar == '.') {
                    s = getCommaPeriod(keyChar);
                } else {
                    s = String.valueOf(keyChar);
                }
                //mDecoderRender.onCommitText(mDecodingInfo.getCurrentFullSent(0) + s);
                mCommitStr = mDecodingInfo.getCurrentFullSent(0) + s;
                resetToIdleState();
                return true;
        }
    }

    private boolean processStatePredict(int keyCode, char keyChar) {
        // In this status, when user presses keys in [a..z], the status will
        // change to input state.
        if (keyChar >= 'a' && keyChar <= 'z') {
            mDecodingInfo.reset();
            mImeState = ImeState.STATE_INPUT;
            mDecodingInfo.addSplChar(keyChar);
            chooseAndUpdate(-1);
            return true;
        } else if (keyCode == KeyEvent.KEYCODE_DEL || keyCode == KeyEvent.KEYCODE_BACK) {
            resetToIdleState();
            return false;
        } else if (keyCode == KeyEvent.KEYCODE_ENTER) {
            //mDecoderRender.onCommitText("\n");
            resetToIdleState();
            return false;
        } else {
            //mDecoderRender.onCommitText(getCommaPeriod(keyChar));
            resetToIdleState();
            return false;
        }
    }

    // TODO: check space and 0~9 in processSurfaceChange logic and here
    private boolean processStateEditComposing(int keyCode, char keyChar) {
        if ((keyCode == KeyEvent.KEYCODE_ENTER) || keyCode == KeyEvent.KEYCODE_DPAD_CENTER
                || keyCode == KeyEvent.KEYCODE_SPACE) {
            mCommitStr = mDecodingInfo.getComposingStr();
            resetToIdleState();
            return true;
        } else if (keyCode == KeyEvent.KEYCODE_DEL || (keyChar >= 'a' && keyChar <= 'z')) {
            return processSurfaceChange(keyCode, keyChar);
        }

        mCommitStr = mDecodingInfo.getComposingStr() + keyChar;
        resetToIdleState();
        return true;
    }

    private void chooseAndUpdate(int candId) {
        if (ImeState.STATE_PREDICT != mImeState) {
            // Get result candidate list, if choice_id < 0, do a new decoding.
            // If choice_id >=0, select the candidate, and get the new candidate
            // list.
            mDecodingInfo.chooseDecodingCandidate(candId);

            // always move cursor to edge
            mDecodingInfo.moveCursorToEdge(false);
        } else {
            // Choose a prediction item.
            mDecodingInfo.choosePredictChoice(candId);
        }

        if (mDecodingInfo.getComposingStr().length() > 0) {
            String resultStr;
            resultStr = mDecodingInfo.getComposingStrActivePart();

            // choiceId >= 0 means user finishes a choice selection.
            // if mCandidatesList == 0, there are no valid words and drop the left composing
            // TODO: investigate if mCandidatesList == 0
            if (candId >= 0 && (mDecodingInfo.canDoPrediction() || mDecodingInfo.mCandidatesList.size() == 0)) {
                mCommitStr = resultStr;
                mImeState = ImeState.STATE_PREDICT;

                // Try to get the prediction list.
                // TODO: investigate why uses getTextBeforeCursor(3, 0) instead of selected word
                /*
                InputConnection ic = getCurrentInputConnection();
                CharSequence cs = ic.getTextBeforeCursor(3, 0);
                if (null != cs) {
                    mDecodingInfo.preparePredicts(cs);
                }
                */
                mDecodingInfo.mIPinyinDecoder.imFlushCache();
                mDecodingInfo.preparePredicts(resultStr);

                if (mDecodingInfo.mCandidatesList.size() > 0) {
                    //mDecoderRender.onUpdateComposing(mDecodingInfo.getComposingStrForDisplay(), 0);
                    mDecodingInfo.resetComposing();
                    //mDecoderRender.onUpdateComposing("", 0);
                    //mDecoderRender.onUpdateCandidates(mDecodingInfo.mCandidatesList, mDecodingInfo.mTotalChoicesNum,-1);
                } else {
                    resetToIdleState();
                }
            } else {
                // TODO: investigate following flow

                if (ImeState.STATE_IDLE == mImeState) {
                    if (mDecodingInfo.getSplStrDecodedLen() == 0) {
                        mImeState = ImeState.STATE_COMPOSING;
                    } else {
                        mImeState = ImeState.STATE_INPUT;
                    }
                } else {
                    if (mDecodingInfo.selectionFinished()) {
                        mImeState = ImeState.STATE_COMPOSING;
                    }
                }
                //mDecoderRender.onUpdateComposing(mDecodingInfo.getComposingStrForDisplay(), mDecodingInfo.mCursorPos);
                /*
                if (mDecodingInfo.mCandidatesList.size() == 0 && mDecodingInfo.getComposingStrForDisplay().length() > 0) {
                    // no candidates, use the composing as candidate
                    List<String> temp = new ArrayList<>();
                    temp.add(mDecodingInfo.getComposingStrForDisplay());
                    mDecoderRender.onUpdateCandidates(temp, -1);
                } else {
                    mDecoderRender.onUpdateCandidates(mDecodingInfo.mCandidatesList, -1);
                }
                */
                //mDecoderRender.onUpdateCandidates(mDecodingInfo.mCandidatesList, mDecodingInfo.mTotalChoicesNum,-1);
            }
        } else {
            resetToIdleState();
            mDecodingInfo.mIPinyinDecoder.imFlushCache();
        }
    }


    private void resetToIdleState() {
        mImeState = ImeState.STATE_IDLE;
        mDecodingInfo.reset();
    }


    private String getCommaPeriod(int keyChar) {
        String s;
        if (keyChar == ',')
            s = "\uff0c";
        else if (keyChar == '.')
            s = "\u3002";
        else
            s = String.valueOf((char)keyChar);

        return s;
    }


    class DecodingInfo {
        private static final String TAG = "DecodingInfo";
        /**
         * Maximum length of the Pinyin string
         * 最大的字符串的长度
         * must <= kMaxLemmaSize in pinyin engine
         */
        private static final int PY_STRING_MAX = 95 + 1;

        /**
          * must <= kMaxLemmaSize in pinyin engine
         */
        private static final int CH_STRING_MAX = 16;

        /**
         * Spelling (Pinyin) string. 拼音字符串
         */
        private StringBuffer mSurface;

        /**
         * Byte buffer used as the Pinyin string parameter for native function
         * call. 字符缓冲区作为拼音字符串参数给本地函数调用，它的长度为PY_STRING_MAX，最后一位为0
         */
        private byte mPyBuf[];

        /**
         * The length of surface string successfully decoded by engine.
         * 成功解码的字符串长度
         */
        private int mSurfaceDecodedLen;

        /**
         * Composing string. gaoyuan: 正在写的拼音字符串
         */
        private String mComposingStr;

        /**
         * Length of the active composing string. 活动的拼音字符串长度
         */
        private int mActiveCmpsLen;

        /**
         * Composing string for display, it is copied from mComposingStr, and
         * add spaces between spellings.
         * 显示的拼音字符串，是从mComposingStr复制过来的，并且在拼写之间加上了空格。
         */
        private String mComposingStrDisplay;

        /**
         * Length of the active composing string for display. 显示的拼音字符串的长度
         */
        private int mActiveCmpsDisplayLen;

        /**
         * The first full sentence choice. 第一个完整句子，第一个候选词。
         */
        private String mFullSent;

        /**
         * Number of characters which have been fixed. 固定的字符的数量
         */
        private int mFixedLen;

        /**
         * If this flag is true, selection is finished. 是否选择完成了？
         */
        private boolean mFinishSelection;

        /**
         * The starting position for each spelling. The first one is the number
         * of the real starting position elements. 每个拼写的开始位置，猜测：第一个元素是拼写的总数量？
         */
        private int mSplStart[];

        /**
         * Editing cursor in mSurface. 光标的位置
         */
        private int mCursorPos;

        /**
         * Pinyin decoding engine.
         */
        private final PinyinDecoder mIPinyinDecoder;

        /**
         * The total number of choices for display. The list may only contains
         * the first part. If user tries to navigate to next page which is not
         * in the result list, we need to get these items. 显示的可选择的总数
         */
        private int mTotalChoicesNum;

        /**
         * Candidate list. The first one is the full-sentence candidate. 候选词列表
         */
        List<String> mCandidatesList = new ArrayList<String>();

        private int mActiveCandiatePos;

        /**
         * The position to delete in Pinyin string. If it is less than 0, IME
         * will do an incremental search, otherwise IME will do a deletion
         * operation. if {@link #mIsPosInSpl} is true, IME will delete the whole
         * string for mPosDelSpl-th spelling, otherwise it will only delete
         * mPosDelSpl-th character in the Pinyin string. 在拼音字符串中的删除位置
         */
        int mPosDelSpl = -1;

        /**
         * If {@link #mPosDelSpl} is big than or equal to 0, this member is used
         * to indicate that whether the postion is counted in spelling id or
         * character. 如果 mPosDelSpl 大于等于 0，那么这个参数就用于表明是否是 拼写的id 或者 字符。
         */
        boolean mIsPosInSpl;

        // input history
        // true if
        final Stack<Boolean> mInputHistory = new Stack<>();

        DecodingInfo() {
            mIPinyinDecoder = PinyinDecoder.getInstance();
            mSurface = new StringBuffer();
            mSurfaceDecodedLen = 0;
        }

        void resetComposing() {
            mSurface.delete(0, mSurface.length());
            mSurfaceDecodedLen = 0;
            mCursorPos = 0;
            mFullSent = "";
            mFixedLen = 0;
            mFinishSelection = false;
            mComposingStr = "";
            mComposingStrDisplay = "";
            mActiveCmpsLen = 0;
            mActiveCmpsDisplayLen = 0;
            mInputHistory.clear();
        }

        /**
         * 重置
         */
        void reset() {
            mSurface.delete(0, mSurface.length());
            mSurfaceDecodedLen = 0;
            mCursorPos = 0;
            mFullSent = "";
            mFixedLen = 0;
            mFinishSelection = false;
            mComposingStr = "";
            mComposingStrDisplay = "";
            mActiveCmpsLen = 0;
            mActiveCmpsDisplayLen = 0;
            mInputHistory.clear();

            resetCandidates();

            // this call could refresh the order of candidates
            mIPinyinDecoder.imFlushCache();

            mIPinyinDecoder.imResetSearch();

            //mDecoderRender.onReset();
        }

        /**
         * 候选词列表是否为空
         *
         * @return
         */
        public boolean isCandidatesListEmpty() {
            return mCandidatesList.size() == 0;
        }

        /**
         * 拼写的字符串是否已满
         *
         * @return
         */
        boolean isSplStrFull() {
            return mSurface.length() >= PY_STRING_MAX - 1;
        }

        /**
         * 增加拼写字符
         *
         * @param ch
         */
        void addSplChar(char ch) {
            mSurface.insert(mCursorPos, ch);
            mCursorPos++;
        }

        private int indexOfDifference(String str1, String str2) {
            if (str1 == null || str2 == null) {
                return 0;
            }
            if (str1.equals(str2)) {
                return -1;
            }
            int i;
            for (i = 0; i < str1.length() && i < str2.length(); ++i) {
                if (str1.charAt(i) != str2.charAt(i)) {
                    break;
                }
            }
            if (i < str2.length() || i < str1.length()) {
                return i;
            }
            return -1;
        }

        private void updatePinyinString(String oldStr, String newStr) {

            int diffPos = indexOfDifference(oldStr, newStr);

            if (diffPos >= 0) {
                if (diffPos < oldStr.length()) {
                    delSplCharsAndUpdate(oldStr.length() - diffPos);
                }

                if (diffPos < newStr.length()) {
                    chooseDecodingCandidate(-1);

                    String strToAdd = newStr.substring(diffPos);
                    for (char c: strToAdd.toCharArray())
                        addSplChar(c);
                }
            }
        }

        // Prepare to delete before cursor. We may delete a spelling char if
        // the cursor is in the range of unfixed part, delete a whole spelling
        // if the cursor in inside the range of the fixed part.
        // This function only marks the position used to delete.

        /**
         * 删除前的准备。该函数只是标记要删除的位置。
         */
        void prepareDeleteBeforeCursor() {
            if (mCursorPos > 0) {
                for (int pos = 0; pos < mFixedLen; pos++) {
                    if (mSplStart.length <= pos + 2)
                        break;

                    if (mSplStart[pos + 2] >= mCursorPos
                            && mSplStart[pos + 1] < mCursorPos) {
                        // 删除一个拼写字符串
                        mPosDelSpl = pos;
                        mCursorPos = mSplStart[pos + 1];
                        mIsPosInSpl = true;
                        break;
                    }
                }

                if (mPosDelSpl < 0) {
                    // 删除一个字符
                    mPosDelSpl = mCursorPos - 1;
                    mCursorPos--;
                    mIsPosInSpl = false;
                }
            }
        }

        private void delSplCharsAndUpdate(int len) {
            for (int i = 0; i < len; i++) {
                prepareDeleteBeforeCursor();

                if (i != len - 1)
                    chooseDecodingCandidate(-1);
            }
        }

        /**
         * 获取拼音字符串长度
         *
         * @return
         */
        int length() {
            return mSurface.length();
        }

        /**
         * 获得拼音字符串中指定位置的字符
         *
         * @param index
         * @return
         */
        char charAt(int index) {
            return mSurface.charAt(index);
        }

        /**
         * 获得拼音字符串
         *
         * @return
         */
        StringBuffer getOrigianlSplStr() {
            return mSurface;
        }

        /**
         * 获得成功解码的字符串长度
         *
         * @return
         */
        int getSplStrDecodedLen() {
            return mSurfaceDecodedLen;
        }

        /**
         * 获得每个拼写字符串的开始位置
         *
         * @return
         */
        int[] getSplStart() {
            return mSplStart;
        }

        /**
         * 获取拼音字符串，有可能存在选中的候选词
         *
         * @return
         */
        String getComposingStr() {
            return mComposingStr;
        }

        /**
         * 获取活动的拼音字符串，就是选择了的候选词。
         *
         * @return
         */
        String getComposingStrActivePart() {
            assert (mActiveCmpsLen <= mComposingStr.length());
            return mComposingStr.substring(0, mActiveCmpsLen);
        }

        /**
         * 获得活动的拼音字符串长度
         *
         * @return
         */
        int getActiveCmpsLen() {
            return mActiveCmpsLen;
        }

        /**
         * 获取显示的拼音字符串
         *
         * @return
         */
        String getComposingStrForDisplay() {
            return mComposingStrDisplay;
        }

        /**
         * 显示的拼音字符串的长度
         *
         * @return
         */
        int getActiveCmpsDisplayLen() {
            return mActiveCmpsDisplayLen;
        }

        /**
         * 第一个完整句子
         *
         * @return
         */
        String getFullSent() {
            return mFullSent;
        }

        /**
         * 获取当前完整句子
         *
         * @param activeCandPos
         * @return
         */
        String getCurrentFullSent(int activeCandPos) {
            try {
                String retStr = mFullSent.substring(0, mFixedLen);
                retStr += mCandidatesList.get(activeCandPos);
                return retStr;
            } catch (Exception e) {
                return "";
            }
        }

        /**
         * 重置候选词列表
         */
        void resetCandidates() {
            mTotalChoicesNum = 0;
            mCandidatesList.clear();
            mActiveCandiatePos = -1;
        }


        void setActiveCandiatePos(int activeCandiatePos) {
            if (mTotalChoicesNum <= activeCandiatePos)
                return;
            mActiveCandiatePos = activeCandiatePos;

            //TODO: selection logic
        }

        /**
         * 判断 mComposingStr.length() == mFixedLen ？
         *
         * @return
         */
        boolean canDoPrediction() {
            return mComposingStr.length() == mFixedLen;
        }

        /**
         * 选择是否完成
         *
         * @return
         */
        boolean selectionFinished() {
            return mFinishSelection;
        }

        // After the user chooses a candidate, input method will do a
        // re-decoding and give the new candidate list.
        // If candidate id is less than 0, means user is inputting Pinyin,
        // not selecting any choice.

        /**
         * 如果candId〉0，就选择一个候选词，并且重新获取一个候选词列表，选择的候选词存放在mComposingStr中，通过mDecInfo.
         * getComposingStrActivePart()取出来。如果candId小于0 ，就对输入的拼音进行查询。
         *
         * @param candId
         */
        private void chooseDecodingCandidate(int candId) {
            // 只有 state 不是 predict 的时候才走下面的逻辑.
            resetCandidates();
            int totalChoicesNum = 0;
            try {
                if (candId < 0) {
                    if (length() == 0) {
                        totalChoicesNum = 0;
                    } else {
                        if (mPyBuf == null)
                            mPyBuf = new byte[PY_STRING_MAX];
                        for (int i = 0; i < length(); i++)
                            mPyBuf[i] = (byte) charAt(i);
                        mPyBuf[length()] = 0;

                        if (mPosDelSpl < 0) {
                            // 根据拼音查词
                            totalChoicesNum = mIPinyinDecoder.imSearch(mPyBuf, length());

                            // the engine could drop too long pinyin string
                            // should not update input history if the input is invalid
                            if (mSurface.toString().equals(mIPinyinDecoder.imGetPyStr(false))) {
                                mInputHistory.push(false);
                                if (BuildConfig.DEBUG)
                                    Log.d(TAG, "pinyin mInputHistory push false: " + mInputHistory);
                            }
                        } else {
                            // del search
                            Boolean lastActionIsChoice = false;
                            if (mInputHistory.size() > 0) {
                                lastActionIsChoice = mInputHistory.pop();
                                if (BuildConfig.DEBUG)
                                    Log.d(TAG, "pinyin mInputHistory pop " + lastActionIsChoice + ": " + mInputHistory);
                            }

                            if (lastActionIsChoice) {
                                totalChoicesNum = mIPinyinDecoder.imCancelLastChoice();
                                if (totalChoicesNum == 0) {
                                    if (BuildConfig.DEBUG) {
                                        Log.d(TAG, "pinyin mInputHistory workaround clear");
                                        Log.d(TAG, "pinyin imGetPyStr(false) " + mIPinyinDecoder.imGetPyStr(false));
                                        Log.d(TAG, "pinyin imGetPyStr(true) " + mIPinyinDecoder.imGetPyStr(true));
                                    }
                                    // workaround for fixing no precious result
                                    mIPinyinDecoder.imResetSearch();
                                    totalChoicesNum = mIPinyinDecoder.imSearch(mPyBuf, length());
                                    mInputHistory.clear();

                                }
                            } else {
                                boolean clear_fixed_this_step = true;
                                if (ImeState.STATE_COMPOSING == mImeState) {
                                    clear_fixed_this_step = false;
                                }
                                // FIX mIsPosInSpl 是什么时候赋值的?
                                totalChoicesNum = mIPinyinDecoder.imDelSearch(mPosDelSpl, mIsPosInSpl, clear_fixed_this_step);

                            }
                            mPosDelSpl = -1;
                        }
                    }
                } else {
                    totalChoicesNum = mIPinyinDecoder.imChoose(candId);
                    mInputHistory.push(true);

                    if (BuildConfig.DEBUG)
                        Log.d(TAG, "pinyin mInputHistory push true: " + mInputHistory);
                }
            } catch (Exception e) {
                if (BuildConfig.DEBUG)
                    Log.e(TAG, "pinyin chooseDecodingCandidate error", e);

                reset();
                return;
            }
            if (BuildConfig.DEBUG)
                Log.d(TAG, "pinyin chooseDecodingCandidate: " + candId);

            updateDecInfoForSearch(totalChoicesNum);
        }

        /**
         * 更新查询词库后的信息
         *
         * @param totalChoicesNum
         */
        private void updateDecInfoForSearch(int totalChoicesNum) {
            mTotalChoicesNum = totalChoicesNum;

            if (BuildConfig.DEBUG)
                Log.d(TAG, "pinyin updateDecInfoForSearch: " + totalChoicesNum);

            if (totalChoicesNum < 0) {
                mTotalChoicesNum = 0;
                return;
            }

            try {
                mSplStart = mIPinyinDecoder.imGetSplStart();
                String pyStr = mIPinyinDecoder.imGetPyStr(false);
                mSurfaceDecodedLen = mIPinyinDecoder.imGetPyStrLen(true);
                assert (mSurfaceDecodedLen <= pyStr.length());
                mFullSent = mIPinyinDecoder.imGetChoice(0);
                mFixedLen = mIPinyinDecoder.imGetFixedLen();

                // Update the surface string to the one kept by engine.
                mSurface.replace(0, mSurface.length(), pyStr);

                if (mCursorPos > mSurface.length())
                    mCursorPos = mSurface.length();

                if (mFixedLen <= mFullSent.length())
                    mComposingStr = mFullSent.substring(0, mFixedLen) + mSurface.substring(mSplStart[mFixedLen + 1]);
                else {
                    mComposingStr = mComposingStr.substring(0, mFixedLen) + mSurface.substring(mFixedLen, mSurface.length());
                    //mPosDelSpl = -1;
                }

                mActiveCmpsLen = mComposingStr.length();
                if (mSurfaceDecodedLen > 0) {
                    mActiveCmpsLen = mActiveCmpsLen - (mSurface.length() - mSurfaceDecodedLen);
                }

                // Prepare the display string.
                if (0 == mSurfaceDecodedLen || mTotalChoicesNum == 0 /* workaround */) {
                    mComposingStrDisplay = mComposingStr;
                    mActiveCmpsDisplayLen = mComposingStr.length();
                } else {
                    mComposingStrDisplay = mFullSent.substring(0, mFixedLen);
                    for (int pos = mFixedLen + 1; pos < mSplStart.length - 1; pos++) {
                        mComposingStrDisplay += mSurface.substring(mSplStart[pos], mSplStart[pos + 1]);
                        if (mSplStart[pos + 1] < mSurfaceDecodedLen) {
                            mComposingStrDisplay += " ";
                        }
                    }
                    mActiveCmpsDisplayLen = mComposingStrDisplay.length();
                    if (mSurfaceDecodedLen < mSurface.length()) {
                        mComposingStrDisplay += mSurface.substring(mSurfaceDecodedLen);
                    }
                }

                mFinishSelection = mSplStart.length == mFixedLen + 2;

                if (BuildConfig.DEBUG) {
                    Log.d(TAG, "pinyin updateDecInfoForSearch mComposingStr " + mComposingStr);
                    Log.d(TAG, "pinyin updateDecInfoForSearch mComposingStrDisplay " + mComposingStrDisplay);
                    Log.d(TAG, "pinyin updateDecInfoForSearch mSurface " + mSurface);
                }
            } catch (Exception e) {
                if (BuildConfig.DEBUG)
                    Log.e(TAG, "pinyin updateDecInfoForSearch error", e);
                reset();
                return;
            }
            // Prepare page 0.
            if (!mFinishSelection) {
                fetchCandidates(mCandidateFetchSize);
            }
        }

        /**
         * 选择预报候选词
         *
         * @param choiceId
         */
        private void choosePredictChoice(int choiceId) {
            if (ImeState.STATE_PREDICT != mImeState || choiceId < 0 || choiceId >= mCandidatesList.size()) {
                return;
            }

            String tmp = mCandidatesList.get(choiceId);
            if (tmp == null) {
                return;
            }

            resetCandidates();

            mCandidatesList.add(tmp);
            mSurface.replace(0, mSurface.length(), "");
            mCursorPos = 0;
            mFullSent = tmp;
            mFixedLen = tmp.length();
            mComposingStr = mFullSent;
            mActiveCmpsLen = mFixedLen;

            mFinishSelection = true;
        }

        /**
         * 获得指定的候选词
         *
         * @param candId
         * @return
         */
        public String getCandidate(int candId) {
            // Only loaded items can be gotten, so we use mCandidatesList.size()
            // instead mTotalChoiceNum.
            if (candId < 0 || candId > mCandidatesList.size()) {
                return null;
            }
            return mCandidatesList.get(candId);
        }

        /**
         * 准备预报候选词
         *
         * @param history
         */
        void preparePredicts(String history) {
            resetCandidates();

            try {
                if (history.length() > CH_STRING_MAX) {
                    // nativeImGetPredictsNum may hit bug if long string?
                    // TODO: check the bug
                    history = history.substring(history.length() - CH_STRING_MAX);
                }

                if (BuildConfig.DEBUG) {
                    Log.d(TAG, "imGetPredictsNum " + history);
                }

                mTotalChoicesNum = mIPinyinDecoder.imGetPredictsNum(history);
                fetchCandidates(mCandidateFetchSize);
            } catch (RuntimeException e) {
                if (BuildConfig.DEBUG)
                    Log.e(TAG, "pinyin preparePredicts error", e);
                reset();
                return;
            }
            mFinishSelection = false;
        }

        /*
            fetch candidates to satisfy fetchSize
        */
        void fetchCandidates(int fetchSize) {
            if (fetchSize == 0)
                fetchSize = mTotalChoicesNum;
            else if (fetchSize > mTotalChoicesNum)
                fetchSize = mTotalChoicesNum;

            int startIndex = mCandidatesList.size();
            if (startIndex >= fetchSize)
                return;

            try {
                if (ImeState.STATE_INPUT == mImeState || ImeState.STATE_IDLE == mImeState || ImeState.STATE_COMPOSING == mImeState){
                    mCandidatesList.addAll(mIPinyinDecoder.imGetChoiceList(startIndex, fetchSize - startIndex, mFixedLen));
                } else if (ImeState.STATE_PREDICT == mImeState) {
                    mCandidatesList.addAll(mIPinyinDecoder.imGetPredictList(startIndex, fetchSize - startIndex));
                }
            } catch (Exception e) {
                if (BuildConfig.DEBUG)
                    Log.e(TAG, "pinyin getCandidates error", e);
                reset();
            }
        }

        /**
         * 光标前面的字符是否是分隔符“'”
         *
         * @return
         */
        boolean charBeforeCursorIsSeparator() {
            int len = mSurface.length();
            if (mCursorPos > len)
                return false;
            return mCursorPos > 0 && mSurface.charAt(mCursorPos - 1) == '\'';
        }

        /**
         * 获取光标位置
         *
         * @return
         */
        public int getCursorPos() {
            return mCursorPos;
        }

        /**
         * 获取光标在拼音字符串中的位置
         *
         * @return
         */
        int getCursorPosInCmps() {
            int cursorPos = mCursorPos;

            for (int hzPos = 0; hzPos < mFixedLen; hzPos++) {
                if (mCursorPos >= mSplStart[hzPos + 2]) {
                    cursorPos -= mSplStart[hzPos + 2] - mSplStart[hzPos + 1];
                    cursorPos += 1;
                }
            }
            return cursorPos;
        }

        /**
         * 获取光标在显示的拼音字符串中的位置
         *
         * @return
         */
        int getCursorPosInCmpsDisplay() {
            int cursorPos = getCursorPosInCmps();
            // +2 is because: one for mSplStart[0], which is used for other
            // purpose(The length of the segmentation string), and another
            // for the first spelling which does not need a space before it.
            for (int pos = mFixedLen + 2; pos < mSplStart.length - 1; pos++) {
                if (mCursorPos <= mSplStart[pos]) {
                    break;
                } else {
                    cursorPos++;
                }
            }
            return cursorPos;
        }

        /**
         * 移动光标到末尾
         *
         * @param left
         */
        void moveCursorToEdge(boolean left) {
            if (left)
                mCursorPos = 0;
            else
                mCursorPos = mSurface.length();
        }

        // Move cursor. If offset is 0, this function can be used to adjust
        // the cursor into the bounds of the string.

        /**
         * 移动光标
         *
         * @param offset
         */
        void moveCursor(int offset) {
            if (offset > 1 || offset < -1)
                return;

            if (offset != 0) {
                int hzPos;
                for (hzPos = 0; hzPos <= mFixedLen; hzPos++) {
                    if (mCursorPos == mSplStart[hzPos + 1]) {
                        if (offset < 0) {
                            if (hzPos > 0) {
                                offset = mSplStart[hzPos]
                                        - mSplStart[hzPos + 1];
                            }
                        } else {
                            if (hzPos < mFixedLen) {
                                offset = mSplStart[hzPos + 2]
                                        - mSplStart[hzPos + 1];
                            }
                        }
                        break;
                    }
                }
            }
            mCursorPos += offset;
            if (mCursorPos < 0) {
                mCursorPos = 0;
            } else if (mCursorPos > mSurface.length()) {
                mCursorPos = mSurface.length();
            }
        }

        /**
         * 获取拼写字符串的数量
         *
         * @return
         */
        public int getSplNum() {
            return mSplStart[0];
        }

        /**
         * 获取固定的字符的数量
         *
         * @return
         */
        public int getFixedLen() {
            return mFixedLen;
        }
    }

}

