package com.android.inputmethod.pinyin;

import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.concurrent.ExecutorService;

import kika.qwt9.inputmethod.Resource.BuildConfig;
import kika.qwt9.inputmethod.Resource.qwt9ini;

public class PinyinT9DecodingHandler {
    private static final String TAG = "PinyinT9DecodingHandler";

    // must use the same pinyin engine thread
    // TODO: pass as argument
    private static final ExecutorService sExecutor = DecodingHandler.getEngineExecutor();// for engine
    private static final Handler sHandler = new Handler(Looper.getMainLooper()); // for callback

    private static final DecodingHandler _sDecodingHandler = DecodingHandler.getInstance();
    private static final int MAX_T9COMPOSING_LIMIT = 18;
    private static final int MAX_BIGRAM_CANDIDATE_LIMIT = 120;

    private static final String[][] t9ToUpperStringTable = {
            {}, // 0
            {}, // 1
            {"A", "B", "C"}, // 2
            {"D", "E", "F"}, // 3
            {"G", "H", "I"}, // 4
            {"J", "K", "L"}, // 5
            {"M", "N", "O"}, // 6
            {"P", "Q", "R", "S"}, // 7
            {"T", "U", "V"}, // 8
            {"W", "X", "Y", "Z"}, // 9
    };

    private static String sDictPath = "";
    private static String sUserDictPath = "";

    private static final StringBuilder sT9Composing = new StringBuilder();
    private static final StringBuilder sT9CommitText = new StringBuilder();

    private static final ArrayList<String> sCandidates = new ArrayList<>();
    private static final ArrayList<String> sComposings = new ArrayList<>();
    private static int sCandiateIndex = 0;

    private static final ArrayList<String[]> sPinyinComposings = new ArrayList<>();

    private static final ArrayList<String> sPinyinCollection = new ArrayList<>();
    private static final ArrayList<String> sSelectedPinyins = new ArrayList<>();
    private static String sSelectedPinyin = "";

    private static final ArrayList<String> sFilteredCandidates = new ArrayList<>();
    private static final ArrayList<String> sFilteredComposings = new ArrayList<>();
    private static final ArrayList<String> sFilteredPinyinCollection = new ArrayList<>();


    private static final String[] sEmptyArray = new String[] {};


    public interface EvaluationCallback {
        void onCompleted(Result result);
    }

    private static final PinyinT9DecodingHandler sDecodingHandler = new PinyinT9DecodingHandler();
    public static PinyinT9DecodingHandler getInstance() {
        return sDecodingHandler;
    }
    private PinyinT9DecodingHandler() {}


    public static class Result {
        private final int mInputKeyCode;
        private final String mT9Composing;
        private final String mCommit;
        private final String[] mCandidates;
        private final String[] mComposings;
        private final String[] mPinyinCollections;
        private final String[] mSelectedPinyins;
        private final int mCandidateIndex;
        private final boolean mIsConsumed;

        Result(final int inputKeyCode, final String t9Composing, final String commit,
               final String[] candidates, final String[] composings,
               final String[] pinyinCollections, final String[] selectedPinyins,
               final int candidateIndex, final boolean isConsumed) {
            mInputKeyCode = inputKeyCode;
            mT9Composing = t9Composing;
            mCommit = commit;
            mCandidates = candidates == null? sEmptyArray: candidates;
            mComposings = composings == null? sEmptyArray: composings;
            mPinyinCollections = pinyinCollections == null? sEmptyArray: pinyinCollections;
            mSelectedPinyins = selectedPinyins == null? sEmptyArray: selectedPinyins;
            mCandidateIndex = candidateIndex;
            mIsConsumed = isConsumed;
        }

        public int getInputKeyCode() {
            return mInputKeyCode;
        }

        public String getT9Composing() {
            return mT9Composing;
        }

        public String getCommit() {
            return mCommit;
        }

        public String[] getCandidates() {
            return mCandidates;
        }

        public String[] getComposings() {
            return mComposings;
        }

        public String[] getPinyinCollections() {
            return mPinyinCollections;
        }

        public String[] getSelectedPinyins() {
            return mSelectedPinyins;
        }

        public int getCandidateIndex() {
            return mCandidateIndex;
        }

        public boolean isConsumed() {
            return mIsConsumed;
        }
    }


    public void init(final String dictPath, final String userDictPath) {
        sExecutor.submit(() -> {
            boolean isInit = _sDecodingHandler._isInit(dictPath, userDictPath, false);
            if (!isInit)
                _sDecodingHandler._init(dictPath, userDictPath);

            isInit = _sDecodingHandler._isInit(dictPath, userDictPath, false);
            int status = 0;

            if (isInit) {
                status = qwt9ini.initial_Py(qwt9ini.IQQI_IME_ID.IQQI_IME_Chinese_CN, dictPath, userDictPath, null, null);
                sDictPath = dictPath;
                sUserDictPath = userDictPath;
            }

            if (BuildConfig.DEBUG)
                if (!isInit || status != 0)
                    Log.e(TAG, String.format("qwt9ini.initial_Py %b %d", isInit, status));
        });
    }

    public void reset(final EvaluationCallback callback) {
        sExecutor.submit(() -> {
            if (!_sDecodingHandler._isInit(sDictPath, sUserDictPath, false))
                return;

            _reset();

            final Result result = new Result(0, "", "",
                    null, null, null , null,
                    0,true);

            sHandler.post(() -> callback.onCompleted(result));
        });
    }

    private void _reset() {
        sT9Composing.setLength(0);
        sT9CommitText.setLength(0);

        sCandidates.clear();
        sComposings.clear();
        sPinyinComposings.clear();
        sCandiateIndex = 0;

        sPinyinCollection.clear();
        sSelectedPinyins.clear();

        sFilteredCandidates.clear();
        sFilteredComposings.clear();
        sFilteredPinyinCollection.clear();
    }

    public void t9Input(final int keyCode, final EvaluationCallback callback) {
        sExecutor.submit(() -> {
            if (!_sDecodingHandler._isInit(sDictPath, sUserDictPath, false)) {
                final Result result = new Result(keyCode, "", "",
                        null, null, null , null,
                        0, false);

                sHandler.post(() -> callback.onCompleted(result));
                return;
            }

            switch (keyCode) {
                case KeyEvent.KEYCODE_DEL:
                    _onDelete(KeyEvent.KEYCODE_DEL, callback);
                    return;
                case KeyEvent.KEYCODE_1:
                    if (sT9Composing.length() == 0 || sT9Composing.charAt(sT9Composing.length()-1) == '1')
                        return;
                case KeyEvent.KEYCODE_2:
                case KeyEvent.KEYCODE_3:
                case KeyEvent.KEYCODE_4:
                case KeyEvent.KEYCODE_5:
                case KeyEvent.KEYCODE_6:
                case KeyEvent.KEYCODE_7:
                case KeyEvent.KEYCODE_8:
                case KeyEvent.KEYCODE_9:
                    if (sT9Composing.length() >= MAX_T9COMPOSING_LIMIT)
                        return;
                    sT9Composing.append(String.valueOf((char)(keyCode-KeyEvent.KEYCODE_0+'0')));
                    _processComposingUpdate(keyCode, callback);
                    return;
                case KeyEvent.KEYCODE_ENTER:
                    _onEnter(keyCode, callback);
                    return;
                case KeyEvent.KEYCODE_SPACE:
                    _onSpace(keyCode, callback);
                    return;
                default:
                    return;

            }
        });
    }

    public void finishAndCommit(final String input, final EvaluationCallback callback) {
        sExecutor.submit(() -> {
            if (_sDecodingHandler._isInit(sDictPath, sUserDictPath, false) && sT9Composing.length() > 0) {

                String commit = input;

                if (sSelectedPinyins.isEmpty()) {
                    if (sCandidates.size() > sCandiateIndex)
                        commit = sCandidates.get(sCandiateIndex) + input;
                } else {
                    if (sFilteredCandidates.size() > sCandiateIndex)
                        commit = sFilteredCandidates.get(sCandiateIndex) + input;
                }

                _reset();

                final Result result = new Result(0, "", commit,
                        null, null, null, null,
                        0, true);

                sHandler.post(() -> callback.onCompleted(result));

                return;
            }

            _reset();

            final Result result = new Result(0, "", input,
                    null, null, null, null,
                    0, true);

            sHandler.post(() -> callback.onCompleted(result));
        });
    }

    public void onSelectCandidate(final String candidate, final int id, final EvaluationCallback callback) {
        sExecutor.submit(() -> {
            if (!_sDecodingHandler._isInit(sDictPath, sUserDictPath, false))
                return;

            if (sCandidates.size() <= id || !candidate.equals(sCandidates.get(id))) {
                return;
            }

            if (sT9Composing.length() > 0) {
                String pinyin = "";
                int deleteCount = 0;

                // calculate deleteCount.
                if (candidate.length() == 1) {
                    if (sSelectedPinyin.length() > 0) {
                        deleteCount = sSelectedPinyin.length();
                    } else {
                        // find pinyin and maxLength.
                        int maxLength = 0;
                        for (int i = 0; i < sCandidates.size(); i++) {
                            String _candidate = sCandidates.get(i);
                            String _composing = sComposings.get(i);
                            if (_candidate.contains(candidate)) {
                                String _pinyin = _composing.split(" ")[0];
                                if (_pinyin.length() >= pinyin.length()) pinyin = _pinyin;
                                maxLength = Math.max(maxLength, _pinyin.length());
                            }
                        }
                        deleteCount = maxLength;
                    }
                } else {
                    String[] compo = sComposings.get(id).split(" ");
                    for (int i = 0; i < candidate.length(); i++) {
                        deleteCount += compo[i].trim().length();
                    }
                }

                //sSelectedPinyin = "";

                sT9Composing.delete(0, deleteCount);
                _processComposingUpdate(-1, candidate, callback);
                return;
            }

            _processNextWordSuggestions(0, candidate, callback);
        });
    }

    public void onSelectPinyin(final String pinyin, final int id, final EvaluationCallback callback) {
        sExecutor.submit(() -> {
            if (!_sDecodingHandler._isInit(sDictPath, sUserDictPath, false))
                return;


            if (TextUtils.isEmpty(pinyin) || sCandidates.isEmpty() || sT9Composing.length() == 0) {
                return;
            }
            if (sSelectedPinyins.isEmpty()) {
                if (sPinyinCollection.size() <= id || !pinyin.equals(sPinyinCollection.get(id))) {
                    return;
                }
            } else {
                if (sFilteredPinyinCollection.size() <= id || !pinyin.equals(sFilteredPinyinCollection.get(id))) {
                    return;
                }
            }

            // filter candidates by pinyin and collect next pinyin collection
            sFilteredCandidates.clear();
            sFilteredComposings.clear();
            sFilteredPinyinCollection.clear();

            int composingIndex = 0;
            int start = 0;
            int end = 0;
            String[] pinyins = pinyin.split(" ");

            for (int i = 0; i < pinyins.length; i++) {
                if (i != 0) {
                    start = composingIndex;
                }
                end = composingIndex += pinyins[i].length();
            }

            String selected = pinyin;
            String composing = sT9Composing.substring(start, end);
            sSelectedPinyin = selected.trim();
            _processPinyinUpdate(-1, pinyin, composing, callback);

        });
    }

    private void _onEnter(final int keyCode, final EvaluationCallback callback) {
        if (sT9Composing.length() == 0
                || sComposings.size() <= sCandiateIndex /* should not happen */) {

            _reset();

            final Result result = new Result(keyCode, "", "",
                    null, null, null , null,
                    0, false);

            sHandler.post(() -> callback.onCompleted(result));

            return;
        }

        String commit;

        if (sSelectedPinyins.isEmpty()) {
            commit = sComposings.get(sCandiateIndex);
        } else {
            commit = sFilteredComposings.get(sCandiateIndex);
        }

        commit = commit.replace(" ", "");

        sT9CommitText.append(commit);

        final Result result = new Result(keyCode, "", /*commit*/sT9CommitText.toString(),
                null, null, null , null,
                0, true);

        _reset();

        sHandler.post(() -> callback.onCompleted(result));
    }

    private void _onSpace(final int keyCode, final EvaluationCallback callback) {
        if (sT9Composing.length() > 0 ) {
            if (sSelectedPinyins.isEmpty() && sCandidates.size() > sCandiateIndex) {
                final String commit = sCandidates.get(sCandiateIndex);
                sT9CommitText.append(commit);
                _processNextWordSuggestions(keyCode, /*commit*/sT9CommitText.toString(), callback);
            } else if (!sSelectedPinyins.isEmpty() && sFilteredCandidates.size() > sCandiateIndex) {
                final String commit = sFilteredCandidates.get(sCandiateIndex);
                sT9CommitText.append(commit);
                _processNextWordSuggestions(keyCode, /*commit*/sT9CommitText.toString(), callback);
            }

            sT9CommitText.setLength(0);

            return;
        }

        final Result result = new Result(keyCode, "", " ",
                null, null, null , null,
                0, false);

        _reset();

        sHandler.post(() -> callback.onCompleted(result));

    }

    private void _processNextWordSuggestions(final int keyCode, final String commit, final EvaluationCallback callback) {
        _reset();

        final String[] candidates = new String[MAX_BIGRAM_CANDIDATE_LIMIT];

        qwt9ini.GetNextWordCandidates(
                qwt9ini.IQQI_IME_ID.IQQI_IME_Chinese_CN,
                commit,
                0,
                MAX_BIGRAM_CANDIDATE_LIMIT,
                candidates);

        for (final String candidate: candidates) {
            if (TextUtils.isEmpty(candidate))
                break;
            sCandidates.add(candidate);
        }

        final Result result = new Result(keyCode, "", commit,
                sCandidates.toArray(new String[0]), null, null , null,
                0, true);

        sHandler.post(() -> callback.onCompleted(result));
    }

    private void _onDelete(final int keyCode, final EvaluationCallback callback) {
        if (sT9Composing.length() == 0) {

            final Result result = new Result(keyCode, "", /*""*/sT9CommitText.toString(),
                    null, null, null , null,
                    0, false);

            sHandler.post(() -> callback.onCompleted(result));

            return;
        }

        if (sSelectedPinyins.size() > 0) {
            sSelectedPinyins.clear();
            sFilteredCandidates.clear();
            sFilteredComposings.clear();
            sFilteredPinyinCollection.clear();
            sCandiateIndex = 0;

            final Result result = new Result(keyCode, sT9Composing.toString(), "",
                    sCandidates.toArray(new String[0]), sComposings.toArray(new String[0]),
                    sPinyinCollection.toArray(new String[0]), sSelectedPinyins.toArray(new String[0]),
                    sCandiateIndex, true);

            sHandler.post(() -> callback.onCompleted(result));

            return;
        }

        sT9Composing.setLength(sT9Composing.length() - 1);

        _processComposingUpdate(keyCode, callback);
    }

    private void _processComposingUpdate(final int keyCode, final EvaluationCallback callback) {
        _processComposingUpdate(keyCode, "", callback);
    }

    private void _processComposingUpdate(final int keyCode, final String commit, final EvaluationCallback callback) {
        String composing = sT9Composing.toString();

        sSelectedPinyin = "";
        sCandidates.clear();
        sComposings.clear();
        sPinyinComposings.clear();
        sCandiateIndex = 0;
        sPinyinCollection.clear();
        sSelectedPinyins.clear();
        sFilteredCandidates.clear();
        sFilteredComposings.clear();
        sFilteredPinyinCollection.clear();

        if (composing.length() == 0) {
            sT9CommitText.append(commit);
            if (BuildConfig.DEBUG)
                Log.i(TAG, "### _processComposingUpdate: sT9CommitText: " + sT9CommitText.toString() + ", commit: " + commit);
            _processNextWordSuggestions(0, sT9CommitText.toString(), callback);
        } else {
            final int totalCandidateCount = qwt9ini.GetCandidateCount(
                    qwt9ini.IQQI_IME_ID.IQQI_IME_Chinese_CN,
                    composing, false, 0);

            if (totalCandidateCount < 0) {
                if (BuildConfig.DEBUG)
                    Log.e(TAG, String.format("qwt9ini.GetCandidateCount %d %s", totalCandidateCount, composing));

                _reset();

                final Result result = new Result(keyCode, "", commit,
                        null, null, null , null,
                        0, false);

                sHandler.post(() -> callback.onCompleted(result));

                return;
            }

            final String[] candidates = new String[totalCandidateCount];
            final String[] composings = new String[totalCandidateCount];

            qwt9ini.GetCandidates(
                    qwt9ini.IQQI_IME_ID.IQQI_IME_Chinese_CN,
                    composing, false,
                    0, 0, totalCandidateCount,
                    candidates, composings);

            sCandidates.addAll(Arrays.asList(candidates));
            // seems like we don't need double space hint here
            for (final String s: composings)
                sComposings.add(s.replaceAll("  ", " "));

            // update pinyin collections
            final HashSet<String> pinyinCollection = new HashSet<>();
            for (final String pinyinComposing : composings) {
                if (TextUtils.isEmpty(pinyinComposing))
                    continue;

                final String[] pinyins = pinyinComposing.split(" ");
                sPinyinComposings.add(pinyins);
                if (pinyins.length == 0 || TextUtils.isEmpty(pinyins[0]))
                    continue;
                pinyinCollection.add(pinyins[0]);
            }

            sPinyinCollection.addAll(pinyinCollection);
            sPinyinCollection.sort((o1, o2) -> {
                if (o1.length() != o2.length())
                    return o2.length() - o1.length();
                return o1.compareTo(o2);
            });

            // 移除大寫英文字母拼音選項
//            final char c = composing.charAt(0);
//            Log.e("hank", "3. _processComposingUpdate: " + c);
//            if (c >= '2' && c <= '9') {
//                final String[] uppers = t9ToUpperStringTable[(int) c - '0'];
//                sPinyinCollection.addAll(Arrays.asList(uppers));
//            }

            sT9CommitText.append(commit);

            final Result result = new Result(keyCode, composing, /*commit*/sT9CommitText.toString(),
                    candidates, composings,
                    sPinyinCollection.toArray(new String[0]),
                    sSelectedPinyins.toArray(new String[0]),
                    0, true);

            sHandler.post(() -> callback.onCompleted(result));
        }
    }

    private void _processPinyinUpdate(final int keyCode, final String pinyin, final String newComposing, final EvaluationCallback callback) {
        String composing = sT9Composing.toString();
        composing = newComposing;

        sCandidates.clear();
        sComposings.clear();
        sPinyinComposings.clear();
        sCandiateIndex = 0;
//        sPinyinCollection.clear();
        sSelectedPinyins.clear();
        sFilteredCandidates.clear();
        sFilteredComposings.clear();
        sFilteredPinyinCollection.clear();

        if (composing.length() == 0) {

//            sT9CommitText.append(commit);

            final Result result = new Result(keyCode, "", /*commit*/sT9CommitText.toString(),
                    null, null, null , null,
                    0, true);

            sHandler.post(() -> callback.onCompleted(result));

            sT9CommitText.setLength(0);
        } else {
            final int totalCandidateCount = qwt9ini.GetCandidateCount(
                    qwt9ini.IQQI_IME_ID.IQQI_IME_Chinese_CN,
                    composing, false, 0);

            if (totalCandidateCount < 0) {
                if (BuildConfig.DEBUG)
                    Log.e(TAG, String.format("qwt9ini.GetCandidateCount %d %s", totalCandidateCount, composing));

                _reset();

                final Result result = new Result(keyCode, "", pinyin,
                        null, null, null , null,
                        0, false);

                sHandler.post(() -> callback.onCompleted(result));

                return;
            }

            final String[] candidates = new String[totalCandidateCount];
            final String[] composings = new String[totalCandidateCount];

            qwt9ini.GetCandidates(
                    qwt9ini.IQQI_IME_ID.IQQI_IME_Chinese_CN,
                    composing, false,
                    0, 0, totalCandidateCount,
                    candidates, composings);

            sCandidates.addAll(Arrays.asList(candidates));
            // seems like we don't need double space hint here
            for (final String s: composings)
                sComposings.add(s.replaceAll("  ", " "));


            // update pinyin collections
            final HashSet<String> pinyinCollection = new HashSet<>();
            for (final String pinyinComposing : composings) {
                if (TextUtils.isEmpty(pinyinComposing))
                    continue;

                final String[] pinyins = pinyinComposing.split(" ");
                sPinyinComposings.add(pinyins);
                if (pinyins.length == 0 || TextUtils.isEmpty(pinyins[0]))
                    continue;
                pinyinCollection.add(pinyins[0]);
            }

            // filter
            final int pinyinIndex = sSelectedPinyins.size() + 1;
            if (BuildConfig.DEBUG) Log.i(TAG, "### _processPinyinUpdate: filter: " + pinyin);
            for (int i = 0; i < sComposings.size(); i++) {
                final String compo = sComposings.get(i);
                if (compo.startsWith(pinyin)) {
                    sFilteredCandidates.add(sCandidates.get(i));
                    sFilteredComposings.add(compo);

                    final String[] pinyins = sPinyinComposings.get(i);
                    if (pinyinIndex < pinyins.length && !TextUtils.isEmpty(pinyins[pinyinIndex]))
                        pinyinCollection.add(pinyins[pinyinIndex]);
                }
            }

            sCandiateIndex = 0;
            sCandidates.clear();
            sCandidates.addAll(sFilteredCandidates);
            sComposings.clear();
            sComposings.addAll(sFilteredComposings);
            sFilteredPinyinCollection.clear();
            sFilteredPinyinCollection.addAll(sPinyinCollection);

            final Result result = new Result(keyCode, composing, /*commit*/sT9CommitText.toString(),
                    //candidates, composings,
                    //sPinyinCollection.toArray(new String[0]),
                    //sSelectedPinyins.toArray(new String[0]),
                    sFilteredCandidates.toArray(new String[0]), sFilteredComposings.toArray(new String[0]),
                    sFilteredPinyinCollection.toArray(new String[0]), sSelectedPinyins.toArray(new String[0]),
                    sFilteredPinyinCollection.indexOf(pinyin)/*0*/, true);

            sHandler.post(() -> callback.onCompleted(result));
        }
    }

}
