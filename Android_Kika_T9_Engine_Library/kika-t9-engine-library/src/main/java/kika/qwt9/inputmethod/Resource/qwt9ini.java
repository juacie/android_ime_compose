package kika.qwt9.inputmethod.Resource;

import android.text.TextUtils;

import java.util.ArrayList;

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CandidatesInfo
{
    public String mCandidate;
    public String mComposing;
    public boolean misUserData;

    public CandidatesInfo()
    {
        mCandidate = "";
        mComposing = "";
        misUserData = false;
    }
};

// -----------------------------------------------------------------------------
//
//
//
// -----------------------------------------------------------------------------
enum OPTION_ID
{
    opt_KeybdMode(0x1000),
    opt_ReportActivate(0x1001),
    opt_FullMatchKey(0x1002),
    opt_4LenPriority(0x1003);

    private int optID;

    OPTION_ID(int optID)
    {
        this.optID = optID;
    }

    int getValue()
    {
        return optID;
    }
}

// -----------------------------------------------------------------------------
//
//
//
// -----------------------------------------------------------------------------
enum KEYBD_MODE
{
    for_T9(1),
    for_Qwerty(2),
    for_FullMatchKey_On(1),
    for_FullMatchKey_Off(0);

    private int optID;

    private KEYBD_MODE(int optID)
    {
        this.optID = optID;
    }

    int getValue()
    {
        return optID;
    }
};

// -----------------------------------------------------------------------------
//
//
//
// -----------------------------------------------------------------------------
public class qwt9ini
{
    public enum IQQI_IME_ID
    {
        IQQI_IME_None(0),
        IQQI_IME_English_US(1),
        IQQI_IME_French_CA(2),
        IQQI_IME_Portuguese_BR(3),
        IQQI_IME_Spanish_US(4),
        IQQI_IME_Assamese(5),
        IQQI_IME_Bengali_IN(6),
        IQQI_IME_Bodo(7),
        IQQI_IME_Dogri(8),
        IQQI_IME_Gujarati(9),
        IQQI_IME_Hindi(10),
        IQQI_IME_Kannada(11),
        IQQI_IME_Kashmiri(12),
        IQQI_IME_Konkani(13),
        IQQI_IME_Malayalam(14),
        IQQI_IME_Manipuri(15),
        IQQI_IME_Marathi(16),
        IQQI_IME_Nepali(17),
        IQQI_IME_Oriya(18),
        IQQI_IME_Punjabi(19),
        IQQI_IME_Sanskrit(20),
        IQQI_IME_Santhali(21),
        IQQI_IME_Sindhi(22),
        IQQI_IME_Tamil(23),
        IQQI_IME_Telugu(24),
        IQQI_IME_Urdu(25),
        IQQI_IME_Afrikaans(26),
        IQQI_IME_Arabic(27),
        IQQI_IME_Chinese_CN(28),
        IQQI_IME_Dutch(29),
        IQQI_IME_English_GB(30),
        IQQI_IME_French_FR(31),
        IQQI_IME_German(32),
        IQQI_IME_Hungarian(33),
        IQQI_IME_Indonesian(34),
        IQQI_IME_Italian(35),
        IQQI_IME_Malay(36),
        IQQI_IME_Persian(37),
        IQQI_IME_Polish(38),
        IQQI_IME_Portuguese_PT(39),
        IQQI_IME_Romanian(40),
        IQQI_IME_Russian(41),
        IQQI_IME_Spanish_ES(42),
        IQQI_IME_Swahili(43),
        IQQI_IME_Thai(44),
        IQQI_IME_Turkish(45),
        IQQI_IME_Vietnamese(46),
        IQQI_IME_Zulu(47),
        IQQI_IME_Bengali_BD(48),
        IQQI_IME_Bulgarian(49),
        IQQI_IME_Croatian(50),
        IQQI_IME_Czech(51),
        IQQI_IME_Finnish(52),
        IQQI_IME_Greek(53),
        IQQI_IME_Kazakh(54),
        IQQI_IME_Khmer(55),
        IQQI_IME_Macedonian(56),
        IQQI_IME_Serbian(57),
        IQQI_IME_Sinhala(58),
        IQQI_IME_Slovak(59),
        IQQI_IME_Slovenian(60),
        IQQI_IME_Swedish(61),
        IQQI_IME_Tagalog(62),
        IQQI_IME_Ukrainian(63),
        IQQI_IME_Xhosa(64),
        IQQI_IME_Albanian(65),
        IQQI_IME_Armenian(66),
        IQQI_IME_Azerbaijani(67),
        IQQI_IME_Belarusian(68),
        IQQI_IME_Bosnian(69),
        IQQI_IME_Chinese_HK(70),
        IQQI_IME_Chinese_TW(71),
        IQQI_IME_Danish(72),
        IQQI_IME_Estonian(73),
        IQQI_IME_Georgian(74),
        IQQI_IME_Hebrew(75),
        IQQI_IME_Icelandic(76),
        IQQI_IME_Lao(77),
        IQQI_IME_Latvian(78),
        IQQI_IME_Lithuanian(79),
        IQQI_IME_Norwegian(80),
        IQQI_IME_Uzbek_Cyrillic(81),
        IQQI_IME_Basque(82),
        IQQI_IME_Galician(83),
        IQQI_IME_Malagasy(84),
        IQQI_IME_Yiddish(85),
        IQQI_IME_Korean(86),
        IQQI_IME_Catalan(87),
        IQQI_IME_Japanese_Kana(88),
        IQQI_IME_Japanese_Roma(89),
        IQQI_IME_Changjie(90),
        IQQI_IME_Quick_CJ(91),
        IQQI_IME_Farsi(92),
        IQQI_IME_Maithili(93),
        IQQI_IME_Burmese_Z(94),
        IQQI_IME_Burmese_U(95),
        IQQI_IME_Tibetan(96),
        IQQI_IME_Uyghur_tili(97),
        IQQI_IME_Jawa(98),
        IQQI_IME_Maori(99),
        IQQI_IME_Mongolian(100),
        IQQI_IME_Amharic(101);

        @SuppressWarnings("unused") private int optID;

        private IQQI_IME_ID(int optID)
        {
            this.optID = optID;
        }

        int getValue()
        {
            return optID;
        }
    }

    public native static void IQQI_Free();

    public native static int IQQI_Initial(IQQI_IME_ID _IME, String PredicDBPath, String UserDBPath,
                                          String PhrsDBPath, StringBuilder iME_ErrorList);

    // 中文 拼音/注音專用
    public native static int IQQI_Initial_Py(IQQI_IME_ID _IME, String PyPredicDBPath,
                                             String PyUserDBPath, String PredicDBPath,
                                             String UserDBPath, String PhrsDBPath,
                                             StringBuilder iME_ErrorList);

    // 取得輸入字根之分組(限中文 拼音/注音)
    public native static int IQQI_GetGrouping(IQQI_IME_ID _IME, String KeyCode, int Begin,
                                              int Number, CandidatesInfo[] Container);

    public native static int IQQI_GetCandidateCount(IQQI_IME_ID _IME, String KeyCode,
                                                    boolean FirstCandidateIsUserTyping, int Type);

    public native static int IQQI_GetCandidates(IQQI_IME_ID _IME, String KeyCode,
                                                boolean FirstCandidateIsUserTyping, int Type,
                                                int Begin, int Number, CandidatesInfo[] Container);

    public native static int IQQI_AddPhrase(IQQI_IME_ID _IME, String KeyCode, String Content);

    public native static int IQQI_RemovePhrase(IQQI_IME_ID _IME, String KeyCode, String Content);

    public native static int IQQI_LearnWord(IQQI_IME_ID _IME, String PinyinCode, String Content);

    public native static int IQQI_LearnWordEx(IQQI_IME_ID _IME, String PinyinCode, String Content,
                                              boolean ForImport, boolean IsMultiTap);

    public native static int IQQI_LearnWordUpdate(IQQI_IME_ID _IME, String PinyinCode,
                                                  String Content, boolean ForImport,
                                                  boolean IsMultiTap);

    public native static int IQQI_LearnWordSaveAs(IQQI_IME_ID _IME, String UserDBName,
                                                  String PinyinCode, String Content,
                                                  boolean ForImport, boolean IsMultiTap);

    public native static String IQQI_GetSelectedCandidateCode(IQQI_IME_ID _IME, int Location);

    public native static int IQQI_GetNextWordCandidates(IQQI_IME_ID _IME, String KeyCode, int Begin,
                                                        int Number, CandidatesInfo[] Container);

    public native static int IQQI_DeleLearnedWord(IQQI_IME_ID _IME, String PinyinCode,
                                                  String Content);

    public native static int IQQI_ResetLearnedWordFromFile(IQQI_IME_ID _IME, String UserDBName);

    public native static int IQQI_DeleLearnedWordFrom(IQQI_IME_ID _IME, String UserDBName,
                                                      String PinyinCode, String Content);

    public native static String IQQI_MultiTap_Input(IQQI_IME_ID _IME, int KeyCode, int PrevKeyCode,
                                                    int Count);

    public native static void IQQI_SetOption(OPTION_ID optKeybdmode, KEYBD_MODE forT9);

    public native static int IQQI_InitialDictionary(IQQI_IME_ID _IME, String DictFName);

    public native static int IQQI_IsVocabularyExist(IQQI_IME_ID _IME, String Voca);

    public native static int IQQI_WordExist(IQQI_IME_ID _IME, String Voca);

    public native static int IQQI_AppendVocabulary(IQQI_IME_ID _IME, String Voca);

    public native static int IQQI_DeleteVocabulary(IQQI_IME_ID _IME, String Voca);

    public native static int IQQI_IsLearnWordExist(IQQI_IME_ID _IME, String Voca);

    public native static int IQQI_GetLearnedWordFromFile(IQQI_IME_ID _IME, String UserDBName,
                                                         int Number, CandidatesInfo[] Container);

    public native static String IQQI_GetComposingText(IQQI_IME_ID _IME, String KeyCode, int Type, int aIndicator);

    public native static String IQQI_DeComposingKorean(IQQI_IME_ID _IME, String Voca,
                                                       boolean Seperator);

    public native static String IQQI_NMapKoreanCode(IQQI_IME_ID _IME, String Voca, boolean toComposing);

    public native static String IQQI_NMapKeyCode(IQQI_IME_ID _IME, String Voca);

    public native static String IQQI_KeyCodeNMap(IQQI_IME_ID _IME, String Voca);

    public native static String IQQI_ShiftKeyCode(IQQI_IME_ID _IME, String Voca);

    public native static Integer[][] IQQI_GetVirtualKeybdLayout(IQQI_IME_ID _IME, boolean isQwerty);

    public native static String IQQI_Version();

    //-------------------------------------------------------------------------
    /// for AutoCorrection
    //-------------------------------------------------------------------------
    public static native long createOnMemoryKikaNative(long formatVersion, String locale,
                                                       String[] attributeKeyStringArray,
                                                       String[] attributeValueStringArray);

    public static native long openKikaNative(String sourceDir, long dictOffset, long dictSize,
                                             boolean isUpdatable);

    public static native void getHeaderInfoKikaNative(long dict, int[] outHeaderSize,
                                                      int[] outFormatVersion,
                                                      ArrayList<int[]> outAttributeKeys,
                                                      ArrayList<int[]> outAttributeValues);

    public static native boolean flushKikaNative(long dict, String filePath);

    public static native boolean needsToRunGCKikaNative(long dict, boolean mindsBlockByGC);

    public static native boolean flushWithGCKikaNative(long dict, String filePath);

    public static native void closeKikaNative(long dict);

    public static native int getFormatVersionKikaNative(long dict);

    public static native int getProbabilityKikaNative(long dict, int[] word);

    public static native int getMaxProbabilityOfExactMatchesKikaNative(long dict, int[] word);

    public static native int getNgramProbabilityKikaNative(long dict,
                                                           int[][] prevWordCodePointArrays,
                                                           boolean[] isBeginningOfSentenceArray,
                                                           int[] word);

    public static native void getWordPropertyKikaNative(long dict, int[] word,
                                                        boolean isBeginningOfSentence,
                                                        int[] outCodePoints, boolean[] outFlags,
                                                        int[] outProbabilityInfo,
                                                        ArrayList<int[][]> outNgramPrevWordsArray,
                                                        ArrayList<boolean[]> outNgramPrevWordIsBeginningOfSentenceArray,
                                                        ArrayList<int[]> outNgramTargets,
                                                        ArrayList<int[]> outNgramProbabilityInfo,
                                                        ArrayList<int[]> outShortcutTargets,
                                                        ArrayList<Integer> outShortcutProbabilities);

    public static native int getNextWordKikaNative(long dict, int token, int[] outCodePoints,
                                                   boolean[] outIsBeginningOfSentence);

    public static native boolean addUnigramEntryKikaNative(long dict, int[] word, int probability,
                                                           int[] shortcutTarget,
                                                           int shortcutProbability,
                                                           boolean isBeginningOfSentence,
                                                           boolean isNotAWord,
                                                           boolean isPossiblyOffensive,
                                                           int timestamp);

    public static native boolean removeUnigramEntryKikaNative(long dict, int[] word);

    public static native boolean addNgramEntryKikaNative(long dict, int[][] prevWordCodePointArrays,
                                                         boolean[] isBeginningOfSentenceArray,
                                                         int[] word, int probability,
                                                         int timestamp);

    public static native boolean removeNgramEntryKikaNative(long dict,
                                                            int[][] prevWordCodePointArrays,
                                                            boolean[] isBeginningOfSentenceArray,
                                                            int[] word);

    public static native boolean updateEntriesForWordWithNgramContextKikaNative(long dict,
                                                                                int[][] prevWordCodePointArrays,
                                                                                boolean[] isBeginningOfSentenceArray,
                                                                                int[] word,
                                                                                boolean isValidWord,
                                                                                int count,
                                                                                int timestamp);

    public static native String getPropertyKikaNative(long dict, String query);

    public static native boolean isCorruptedKikaNative(long dict);

    public static native boolean migrateKikaNative(long dict, String dictFilePath,
                                                   long newFormatVersion);

    //-------------------------------------------------------------------------
    /// for Session
    //-------------------------------------------------------------------------
    public static native long setDicTraverseSessionKikaNative(String locale, long dictSize);

    public static native void initDicTraverseSessionKikaNative(long nativeDicTraverseSession,
                                                               long dictionary, int[] previousWord,
                                                               int previousWordLength);

    public static native void releaseDicTraverseSessionKikaNative(long nativeDicTraverseSession);

    public static native void getSuggestionsKikaNative(long dict, long proximityInfo,
                                                       long traverseSession, int[] xCoordinates,
                                                       int[] yCoordinates, int[] times,
                                                       int[] pointerIds, int[] inputCodePoints,
                                                       int inputSize, int[] suggestOptions,
                                                       boolean useRNN, int[][] wordCodePointArrays,
                                                       int[] RNNWordScoreArray,
                                                       boolean[] isBeginningOfSentenceArray,
                                                       int prevWordCount,
                                                       int[] outputSuggestionCount,
                                                       int[] outputCodePoints, int[] outputScores,
                                                       int[] outputIndices, int[] outputTypes,
                                                       int[] outputAutoCommitFirstWordConfidence,
                                                       float[] inOutWeightOfLangModelVsSpatialModel);

    // -------------------------------------------------------------------------
    // for BinaryDictionaryUtils
    // -------------------------------------------------------------------------
    public static native boolean createEmptyDictFileKikaNative(String filePath, long dictVersion,
                                                               String locale,
                                                               String[] attributeKeyStringArray,
                                                               String[] attributeValueStringArray);

    public static native float calcNormalizedScoreKikaNative(int[] before, int[] after, int score);

    public static native int setCurrentTimeForTestKikaNative(int currentTime);

    // -------------------------------------------------------------------------
    // for KikaProximityInfo
    // -------------------------------------------------------------------------
    public static native long setProximityInfoKikaNative(int displayWidth, int displayHeight,
                                                         int gridWidth, int gridHeight,
                                                         int mostCommonKeyWidth,
                                                         int mostCommonKeyHeight,
                                                         int[] proximityCharsArray, int keyCount,
                                                         int[] keyXCoordinates,
                                                         int[] keyYCoordinates, int[] keyWidths,
                                                         int[] keyHeights, int[] keyCharCodes,
                                                         float[] sweetSpotCenterXs,
                                                         float[] sweetSpotCenterYs,
                                                         float[] sweetSpotRadii);

    public static native void releaseProximityInfoKikaNative(long nativeProximityInfo);

    // -------------------------------------------------------------------------

    static
    {
        System.loadLibrary("IQQILib");
    }

    // -------------------------------------------------------------------------
    //
    // Return Value : version of Kika engine
    // Parameters : null
    // Remarks : get current version of Kika engine
    //
    // -------------------------------------------------------------------------
//    public static qwt9ini()
//    {
//        setT9();
//    }

    // -------------------------------------------------------------------------
    //
    // Return Value : version of Kika engine
    // Parameters : null
    // Remarks : get current version of Kika engine
    //
    // -------------------------------------------------------------------------
//    public static void finalize()
//    {
//        qwt9ini.IQQI_Free();
//    }

    // -------------------------------------------------------------------------
    //
    // Return Value : set Querty Mode
    // Parameters : null
    // Remarks : null
    //
    // -------------------------------------------------------------------------
    public static void setQwerty()
    {
        qwt9ini.IQQI_SetOption(OPTION_ID.opt_KeybdMode, KEYBD_MODE.for_Qwerty);
        //qwt9ini.IQQI_SetOption(OPTION_ID.opt_FullMatchKey, KEYBD_MODE.for_FullMatchKey_Off);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value : set T9 Mode
    // Parameters : null
    // Remarks : null
    //
    // -------------------------------------------------------------------------
    public static void setT9()
    {
        qwt9ini.IQQI_SetOption(OPTION_ID.opt_KeybdMode, KEYBD_MODE.for_T9);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value : 0 =OK, misc is error code
    // Parameters :
    // Remarks : initial Kika engine
    //
    // -------------------------------------------------------------------------
    public static int initial(IQQI_IME_ID _IME, String sDictName, String sUserName)
    {
        StringBuilder IME_ErrorList = new StringBuilder();

        return qwt9ini.IQQI_Initial(_IME, sDictName, sUserName, null, IME_ErrorList);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value : 0 =OK, misc is error code
    // Parameters :
    // Remarks : initial Kika engine
    //
    // -------------------------------------------------------------------------
    public static int initial_Py(IQQI_IME_ID _IME, String sPyDictName, String sPyUserName,
                                 String sDictName, String sUserName)
    {
        StringBuilder IME_ErrorList = new StringBuilder();

        return qwt9ini.IQQI_Initial_Py(_IME, sPyDictName, sPyUserName, sDictName, null, null,
                IME_ErrorList);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value : 0 =OK, misc is error code
    // Parameters :
    // Remarks : initial Kika engine
    //
    // -------------------------------------------------------------------------
    public static int initial_V1(IQQI_IME_ID _IME, String sDictName)
    {
        return 0;
    }

    // -------------------------------------------------------------------------
    //
    // Return Value : version of Kika engine
    // Parameters : null
    // Remarks : get current version of Kika engine
    //
    // -------------------------------------------------------------------------
    public static String Version()
    {
        return qwt9ini.IQQI_Version();
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : return the candidates of T9
    //
    // -------------------------------------------------------------------------
    public static int GetGrouping(IQQI_IME_ID _IME, String KeyCode, int Begin, int Number,
                                  String Candidates[])
    {
        CandidatesInfo[] Container = new CandidatesInfo[Number];
        for (int i = 0; i < Number; i++)
            Container[i] = new CandidatesInfo();

        int rv = qwt9ini.IQQI_GetGrouping(_IME, KeyCode, Begin, Number, Container);

        for (int i = 0; i < rv; i++)
        {
            if (Container[i].mCandidate.length() > 0)
            {
                Candidates[i] = Container[i].mCandidate;
            }
        }
        return rv;
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : return the candidates of T9
    //
    // -------------------------------------------------------------------------
    public static int GetCandidates(IQQI_IME_ID _IME, String KeyCode,
                                    boolean FirstCandidateIsUserTyping, int Type, int Begin,
                                    int Number, String Candidates[], String Composings[])
    {
        CandidatesInfo[] Container = new CandidatesInfo[Number];
        for (int i = 0; i < Number; i++)
            Container[i] = new CandidatesInfo();

        if (_IME == IQQI_IME_ID.IQQI_IME_Korean)
        {
            String KeyCodeKr = KeyCode;
            KeyCode = qwt9ini.IQQI_DeComposingKorean(_IME, KeyCodeKr, false);
        }
        int rv = qwt9ini
                .IQQI_GetCandidates(_IME, KeyCode, FirstCandidateIsUserTyping, 3, Begin, Number,
                        Container);

        for (int i = 0; i < rv; i++)
        {
            String candidate = Container[i].mCandidate;
            if (TextUtils.isEmpty(candidate))
                continue;

            Candidates[i] = candidate;

            if (Composings == null)
                continue;

            String composing = Container[i].mComposing;
            if (TextUtils.isEmpty(composing))
                continue;

            Composings[i] = composing;

        }
        return rv;
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : return the candidates of T9
    //
    // -------------------------------------------------------------------------
    public static int GetCandidateCount(IQQI_IME_ID _IME, String KeyCode,
                                        boolean FirstCandidateIsUserTyping, int Type)
    {
        return qwt9ini.IQQI_GetCandidateCount(_IME, KeyCode, FirstCandidateIsUserTyping, Type);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : return the next word candidates of T9
    //
    // -------------------------------------------------------------------------
    public static int GetNextWordCandidates(IQQI_IME_ID _IME, String KeyCode, int Begin, int Number,
                                            String Candidates[])
    {
        CandidatesInfo[] Container = new CandidatesInfo[Number];
        for (int i = 0; i < Number; i++)
            Container[i] = new CandidatesInfo();

        int rv = qwt9ini.IQQI_GetNextWordCandidates(_IME, KeyCode, Begin, Number, Container);

        for (int i = 0; i < rv; i++)
        {
            if (Container[i].mCandidate.length() > 0)
            {
                Candidates[i] = Container[i].mCandidate;
            }
        }
        return rv;
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : MultiTap input
    //
    // -------------------------------------------------------------------------
    public static String MultiTap_Input(IQQI_IME_ID _IME, int KeyCode, int PrevKeyCode, int Count)
    {
        return IQQI_MultiTap_Input(_IME, KeyCode, PrevKeyCode, Count);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks :
    //
    // -------------------------------------------------------------------------
    public static int LearnWord(IQQI_IME_ID _IME, String PinyinCode, String Content)
    {
        return IQQI_LearnWord(_IME, PinyinCode, Content);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : 注意, 韓文/日文/注音/倉頡/筆畫, PinyinCode不可為空白
    //
    // -------------------------------------------------------------------------
    public static int LearnWordEx(IQQI_IME_ID _IME, String PinyinCode, String Content)
    {
        int ret = 0;

        switch (_IME)
        {
            case IQQI_IME_Korean:
                String KeyCodeKr = PinyinCode;
                PinyinCode = qwt9ini.IQQI_DeComposingKorean(_IME, KeyCodeKr, false);
                ret = qwt9ini.LearnWordEx(_IME, PinyinCode, Content, true, true);
                return ret;

            case IQQI_IME_Changjie:
            case IQQI_IME_Chinese_CN:
            case IQQI_IME_Chinese_HK:
            case IQQI_IME_Chinese_TW:
                ret = qwt9ini.LearnWordEx(_IME, PinyinCode, Content, true, true);
                return ret;

            default:
                ret = qwt9ini.IQQI_LearnWordEx(_IME, "", Content, false, true);
        }
        return ret;
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks :
    //
    // -------------------------------------------------------------------------
    public static int LearnWordEx(IQQI_IME_ID _IME, String PinyinCode, String Content,
                                  boolean ForImport, boolean IsMultiTap)
    {
        return IQQI_LearnWordEx(_IME, PinyinCode, Content, ForImport, IsMultiTap);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks :
    //
    // -------------------------------------------------------------------------
    public static int LearnWordUpdate(IQQI_IME_ID _IME, String PinyinCode, String Content,
                                      boolean ForImport, boolean IsMultiTap)
    {
        return IQQI_LearnWordUpdate(_IME, PinyinCode, Content, ForImport, IsMultiTap);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks :
    //
    // -------------------------------------------------------------------------
    public static int LearnWordSaveAs(IQQI_IME_ID _IME, String UserDBName, String PinyinCode,
                                      String Content, boolean ForImport, boolean IsMultiTap)
    {
        return IQQI_LearnWordSaveAs(_IME, UserDBName, PinyinCode, Content, ForImport, IsMultiTap);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks :
    //
    // -------------------------------------------------------------------------
    public static int DeleLearnedWord(IQQI_IME_ID _IME, String PinyinCode, String Content)
    {
        return IQQI_DeleLearnedWord(_IME, PinyinCode, Content);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks :
    //
    // -------------------------------------------------------------------------
    public static int DeleLearnedWordFrom(IQQI_IME_ID _IME, String UserDBName, String PinyinCode,
                                          String Content)
    {
        return IQQI_DeleLearnedWordFrom(_IME, UserDBName, PinyinCode, Content);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks :
    //
    // -------------------------------------------------------------------------
    public static int ResetLearnedWordFromFile(IQQI_IME_ID _IME, String UserDBName)
    {
        return IQQI_ResetLearnedWordFromFile(_IME, UserDBName);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : return the candidates of T9
    //
    // -------------------------------------------------------------------------
    public static int GetLearnedWordFromFile(IQQI_IME_ID _IME, String UserDBName, int Number,
                                             String Candidates[])
    {
        CandidatesInfo[] Container = new CandidatesInfo[Number];
        for (int i = 0; i < Number; i++)
            Container[i] = new CandidatesInfo();

        int rv = qwt9ini.IQQI_GetLearnedWordFromFile(_IME, UserDBName, Number, Container);

        for (int i = 0; i < rv; i++)
        {
            if (Container[i].mCandidate.length() > 0)
            {
                Candidates[i] = Container[i].mCandidate;
            }
        }
        return rv;
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : Initial Dictionary data file
    //
    // -------------------------------------------------------------------------
    public static int InitialDictionary(IQQI_IME_ID _IME, String DictName)
    {
        return IQQI_InitialDictionary(_IME, DictName);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : return Vocabulary exist or not
    //
    // -------------------------------------------------------------------------
    public static int IsVocabularyExist(IQQI_IME_ID _IME, String Voca)
    {
        return IQQI_WordExist(_IME, Voca);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : Add Vocabulary
    //
    // -------------------------------------------------------------------------
    public static int AppendVocabulary(IQQI_IME_ID _IME, String Voca)
    {
        return IQQI_AppendVocabulary(_IME, Voca);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : Delete Vocabulary
    //
    // -------------------------------------------------------------------------
    public static int DeleteVocabulary(IQQI_IME_ID _IME, String Voca)
    {
        return IQQI_DeleteVocabulary(_IME, Voca);
    }

    // -------------------------------------------------------------------------
    //
    // Return Value :
    // Parameters :
    // Remarks : return Vocabulary exist or not
    //
    // -------------------------------------------------------------------------
    public static int IsLearnWordExist(IQQI_IME_ID _IME, String Voca)
    {
        return IQQI_IsLearnWordExist(_IME, Voca);
    }
}
