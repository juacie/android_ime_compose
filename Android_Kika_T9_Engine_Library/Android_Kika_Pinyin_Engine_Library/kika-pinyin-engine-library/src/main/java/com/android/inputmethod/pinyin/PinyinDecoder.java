/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.inputmethod.pinyin;

import android.text.TextUtils;
import android.util.Log;
import com.kikatech.inputmethod.pinyin.BuildConfig;

import java.io.File;
import java.util.List;
import java.util.Vector;

public class PinyinDecoder {
    private static final String TAG = "PinyinDecoder";

    private static final String LibraryName = "iqqipy";

    // 导入本地函数库
    static {
        try {
            System.loadLibrary(LibraryName);
        } catch (Exception ule) {
            if (BuildConfig.DEBUG)
                Log.e(TAG,"Could not load pinyin engine", ule);
        }
    }

    private static final PinyinDecoder sInstance = new PinyinDecoder();

    public static PinyinDecoder getInstance() {
        return sInstance;
    }

    private static String sDictPath = "";
    private static String sUserDictPath = "";
    private static boolean sIsZhuyin = false;
    private static boolean sIsInited = false;

    private PinyinDecoder() { }

    boolean _isInited(final String dictPath, String userDictPath, boolean isZhuyin) {
        return sIsInited && sDictPath.equals(dictPath) && sUserDictPath.equals(userDictPath) && sIsZhuyin == isZhuyin;
    }

    /**
     * 初始化拼音引擎
     */
    boolean initPinyinEngine(final String dictPath, String userDictPath, boolean isZhuyin) {
        if (_isInited(dictPath, userDictPath, isZhuyin))
            return true;

        if (sIsInited) {
            close();
        }

        try {
            // ensure user file exist
            if (!TextUtils.isEmpty(userDictPath)) {
                File userFile = new File(userDictPath);
                if (!userFile.exists()) {
                    try {
                        userFile.createNewFile();
                    } catch (Exception e) {
                        if (BuildConfig.DEBUG) {
                            Log.d(TAG, "createNewFile: " + userDictPath, e);
                        }

                        userDictPath = "";
                    }
                }
            }

            if (BuildConfig.DEBUG) {
                Log.d(TAG, "userDict: " + userDictPath);
            }

            nativeImSetDictComp(!isZhuyin);

            sIsInited = nativeImOpenDecoder(dictPath.getBytes(), userDictPath.getBytes());

            if (sIsInited) {
                sDictPath = dictPath;
                sUserDictPath = userDictPath;
                sIsZhuyin = isZhuyin;
            } else {
                sDictPath = "";
                sUserDictPath = "";
                sIsZhuyin = false;
            }
        } catch (Exception e) {
            if (BuildConfig.DEBUG) {
                Log.e(TAG, "failed to load dict file. " + dictPath + " " + userDictPath, e);
            }
        } finally {
            return sIsInited;
        }
    }

    void close() {
        try {
            nativeImCloseDecoder();
        } catch (Exception e) {
            if (BuildConfig.DEBUG) {
                Log.d(TAG, "nativeImCloseDecoder: ", e);
            }
        }
        sIsInited = false;
        sDictPath = "";
        sUserDictPath = "";
    }

    boolean isZhuyinMode() {
        return sIsZhuyin;
    }

    /**
     * 设置最大的长度
     */
    /*
    void setMaxLens(int maxSpsLen, int maxHzsLen) {
        nativeImSetMaxLens(maxSpsLen, maxHzsLen);
    }
    */

    /**
     * 根据拼音查询候选词
     */
    int imSearch(byte[] pyBuf, int pyLen) {
        return nativeImSearch(pyBuf, pyLen);
    }

    /**
     * 删除指定位置的拼音后进行查询
     */
    int imDelSearch(int pos, boolean is_pos_in_splid,
                           boolean clear_fixed_this_step) {
        return nativeImDelSearch(pos, is_pos_in_splid,
                clear_fixed_this_step);
    }

    /**
     * 重置拼音查询，应该是清除之前查询的数据
     */
    void imResetSearch() {
        nativeImResetSearch();
    }

    /**
     * 增加字母。
     *
     * @备注 目前没有使用。
     */
    int imAddLetter(byte ch) {
        return nativeImAddLetter(ch);
    }

    /**
     * 获取拼音字符串
     */
    String imGetPyStr(boolean decoded) {
        return nativeImGetPyStr(decoded);
    }

    /**
     * 获取拼音字符串的长度
     */
    int imGetPyStrLen(boolean decoded) {
        return nativeImGetPyStrLen(decoded);
    }

    /**
     * 获取每个拼写的开始位置，猜测：第一个元素是拼写的总数量？
     */
    int[] imGetSplStart() {
        return nativeImGetSplStart();
    }

    /**
     * 获取指定位置的候选词
     */
    String imGetChoice(int choiceId) {
        return nativeImGetChoice(choiceId);
    }

    /**
     * 获取多个候选词
     *
     * @备注 目前没有使用。
     */
    String[] imGetChoices(int choicesNum) {
        String[] retStr = new String[choicesNum];
        for (int i = 0; i < choicesNum; i++)
            retStr[i] = nativeImGetChoice(i);

        return retStr;
    }

    /**
     * 获取候选词列表。choicesStart位置的候选词从sentFixedLen开始截取。
     *
     * gaoyuan:
     * @return 查询出的个数, 问题: 查询出的字存在哪里
     */
    List<String> imGetChoiceList(int choicesStart, int choicesNum, int sentFixedLen) {
        Vector<String> choiceList = new Vector<>();
        for (int i = choicesStart; i < choicesStart + choicesNum; i++) {
            String retStr = nativeImGetChoice(i);
            if (0 == i)
                retStr = retStr.substring(sentFixedLen);
            choiceList.add(retStr);
        }
        return choiceList;
    }

    /**
     * 获取候选词的数量
     */
    int imChoose(int choiceId) {
        return nativeImChoose(choiceId);
    }

    /**
     * 取消最后的选择
     *
     * @备注 目前没有使用
     */
    int imCancelLastChoice() {
        return nativeImCancelLastChoice();
    }

    /**
     * 获取固定字符的长度
     */
    int imGetFixedLen() {
        return nativeImGetFixedLen();
    }

    /**
     * 取消输入
     *
     * @备注 目前没有使用
     */
    boolean imCancelInput() {
        return nativeImCancelInput();
    }

    /**
     * 刷新缓存
     *
     * @备注 目前没有使用
     */
    //could refresh the order of candidates, must ensure the state is NOT in composing
    void imFlushCache() {
        if (!sIsInited)
            return;
        nativeImFlushCache();
    }

    /**
     * 根据字符串 fixedStr 获取预报的候选词
     */
    int imGetPredictsNum(String fixedStr) {
        return nativeImGetPredictsNum(fixedStr);
    }

    /**
     * 获取指定位置的预报候选词
     */
    public String imGetPredictItem(int predictNo) {
        return nativeImGetPredictItem(predictNo);
    }

    /**
     * 获取候选词列表
     */
    List<String> imGetPredictList(int predictsStart, int predictsNum) {
        Vector<String> predictList = new Vector<String>();
        for (int i = predictsStart; i < predictsStart + predictsNum; i++) {
            predictList.add(nativeImGetPredictItem(i));
        }
        return predictList;
    }

    /**
     * 同步到用户词典，猜测：是不是记住用户的常用词。
     *
     * @备注 目前没有使用
     */
    /*
    String syncUserDict(String tomerge) {
        return nativeSyncUserDict(sUserDictPath.getBytes(), tomerge);
    }
    */

    /**
     * 开始用户词典同步
     *
     * @备注 目前没有使用
     */
    boolean syncBegin() {
        return nativeSyncBegin(sUserDictPath.getBytes());
    }

    /**
     * 同步结束
     *
     * @备注 目前没有使用
     */
    void syncFinish() {
        nativeSyncFinish();
    }

    /**
     * 同步存入Lemmas
     *
     * @备注 目前没有使用
     */
    int syncPutLemmas(String tomerge) {
        return nativeSyncPutLemmas(tomerge);
    }

    /**
     * 同步获取Lemmas
     *
     * @备注 目前没有使用
     */
    String syncGetLemmas() {
        return nativeSyncGetLemmas();
    }

    /**
     * 同步获取最后的数量
     *
     * @备注 目前没有使用
     */
    int syncGetLastCount() {
        return nativeSyncGetLastCount();
    }

    /**
     * 同步获取总数量
     *
     * @备注 目前没有使用
     */
    int syncGetTotalCount() {
        return nativeSyncGetTotalCount();
    }

    /**
     * 同步清空最后获取
     *
     * @备注 目前没有使用
     */
    void syncClearLastGot() {
        nativeSyncClearLastGot();
    }

    /**
     * 同步获取容量
     *
     * @备注 目前没有使用
     */
    int imSyncGetCapacity() {
        return nativeSyncGetCapacity();
    }

    native static boolean nativeImOpenDecoder(byte fn_dict[], byte fn_usr_dict[]);

    /**
     * JNI函数：设置最大的长度
     *
     * @param maxSpsLen
     * @param maxHzsLen
     */
    native static void nativeImSetMaxLens(int maxSpsLen, int maxHzsLen);

    /**
     * JNI函数：关闭解码器
     *
     * @return
     */
    native static boolean nativeImCloseDecoder();

    /**
     * JNI函数：根据拼音查询候选词
     *
     * @param pyBuf
     * @param pyLen
     * @return
     */
    native static int nativeImSearch(byte pyBuf[], int pyLen);

    /**
     * JNI函数：删除指定位置的拼音后进行查询
     *
     * @param pos
     * @param is_pos_in_splid
     * @param clear_fixed_this_step
     * @return
     */
    native static int nativeImDelSearch(int pos, boolean is_pos_in_splid,
                                        boolean clear_fixed_this_step);

    /**
     * JNI函数：重置拼音查询，应该是清除之前查询的数据
     */
    native static void nativeImResetSearch();

    /**
     * JNI函数：增加字母。
     *
     * @param ch
     * @return
     * @备注 目前没有使用。
     */
    native static int nativeImAddLetter(byte ch);

    /**
     * JNI函数：获取拼音字符串
     *
     * @param decoded
     * @return
     */
    native static String nativeImGetPyStr(boolean decoded);

    /**
     * JNI函数：获取拼音字符串的长度
     *
     * @param decoded
     * @return
     */
    native static int nativeImGetPyStrLen(boolean decoded);

    /**
     * JNI函数：获取每个拼写的开始位置，猜测：第一个元素是拼写的总数量？
     *
     * @return
     */
    native static int[] nativeImGetSplStart();

    /**
     * JNI函数：获取指定位置的候选词
     *
     * @param choiceId
     * @return
     */
    native static String nativeImGetChoice(int choiceId);

    /**
     * JNI函数：获取候选词的数量
     *
     * @param choiceId
     * @return
     */
    native static int nativeImChoose(int choiceId);

    /**
     * JNI函数：取消最后的选择
     *
     * @return
     * @备注 目前没有使用
     */
    native static int nativeImCancelLastChoice();

    /**
     * JNI函数：获取固定字符的长度
     *
     * @return
     */
    native static int nativeImGetFixedLen();

    /**
     * JNI函数：取消输入
     *
     * @return
     * @备注 目前没有使用
     */
    native static boolean nativeImCancelInput();

    /**
     * JNI函数：刷新缓存
     *
     * @return
     * @备注 目前没有使用
     */
    native static boolean nativeImFlushCache();

    /**
     * JNI函数：根据字符串 fixedStr 获取预报的候选词
     *
     * @param fixedStr
     * @return
     */
    native static int nativeImGetPredictsNum(String fixedStr);

    /**
     * JNI函数：获取指定位置的预报候选词
     *
     * @param predictNo
     * @return
     */
    native static String nativeImGetPredictItem(int predictNo);

    // Sync related

    /**
     * JNI函数：同步到用户词典，猜测：是不是记住用户的常用词。
     *
     * @param user_dict
     * @param tomerge
     * @return
     * @备注 目前没有使用
     */
    native static String nativeSyncUserDict(byte[] user_dict, String tomerge);

    /**
     * JNI函数：开始用户词典同步
     *
     * @param user_dict
     * @return
     * @备注 目前没有使用
     */
    native static boolean nativeSyncBegin(byte[] user_dict);

    /**
     * JNI函数：同步结束
     *
     * @return
     * @备注 目前没有使用
     */
    native static boolean nativeSyncFinish();

    /**
     * JNI函数：同步获取Lemmas
     *
     * @return
     * @备注 目前没有使用
     */
    native static String nativeSyncGetLemmas();

    /**
     * JNI函数：同步存入Lemmas
     *
     * @param tomerge
     * @return
     * @备注 目前没有使用
     */
    native static int nativeSyncPutLemmas(String tomerge);

    /**
     * JNI函数：同步获取最后的数量
     *
     * @return
     * @备注 目前没有使用
     */
    native static int nativeSyncGetLastCount();

    /**
     * JNI函数：同步获取总数量
     *
     * @return
     * @备注 目前没有使用
     */
    native static int nativeSyncGetTotalCount();

    /**
     * JNI函数：同步清空最后获取
     *
     * @return
     * @备注 目前没有使用
     */
    native static boolean nativeSyncClearLastGot();

    /**
     * JNI函数：同步获取容量
     *
     * @return
     * @备注 目前没有使用
     */
    native static int nativeSyncGetCapacity();

    /**
     * Enable dict file compatible mode
     * @param isComp
     */
    native static void nativeImSetDictComp(boolean isComp);
}
