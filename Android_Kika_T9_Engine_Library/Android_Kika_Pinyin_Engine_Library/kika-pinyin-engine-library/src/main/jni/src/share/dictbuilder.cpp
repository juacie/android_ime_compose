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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <set>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>

#include "../include/dictbuilder.h"
#include "../include/dicttrie.h"
#include "../include/mystdlib.h"
#include "../include/ngram.h"
#include "../include/searchutility.h"
#include "../include/spellingtable.h"
#include "../include/spellingtrie.h"
#include "../include/splparser.h"
#include "../include/utf16reader.h"

namespace ime_pinyin {

#ifdef ___BUILD_MODEL___

using namespace std;

wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    
static const char* valid_pinyins[] = {
    "b", "p", "m", "f", "d", "t", "n", "l", "g", "k", "h", "j", "q", "x", "zh", "ch",
    "sh", "r", "zi", "ci", "si", "y", "w", "yu", "a", "o", "e", "ei", "ai", "ei", "ao", "ou",
    "an", "en", "ang", "eng", "er", "ba3", "ba1", "ba2", "ba4", "ba5", "bo3", "bo1", "bo2", "bo4", "bo5", "bai3",
    "bai1", "bai2", "bai4", "bei3", "bei1", "bei4", "bei5", "bao3", "bao1", "bao2", "bao4", "bao5", "ban3", "ban1", "ban4", "ben3",
    "ben1", "ben4", "bang3", "bang1", "bang4", "beng3", "beng1", "beng2", "beng4", "bi3", "bi1", "bi2", "bi4", "bie3", "bie1", "bie2",
    "bie4", "biao3", "biao1", "biao4", "bian3", "bian1", "bian4", "bian5", "bin1", "bin4", "bing3", "bing1", "bing4", "bu3", "bu1", "bu2",
    "bu4", "bu5", "pa1", "pa2", "pa4", "po3", "po1", "po2", "po4", "po5", "pai3", "pai1", "pai2", "pai4", "pei3", "pei1",
    "pei2", "pei4", "pao3", "pao1", "pao2", "pao4", "pou3", "pou1", "pou2", "pan3", "pan1", "pan2", "pan4", "pen3", "pen1", "pen2",
    "pang3", "pang1", "pang2", "pang4", "peng3", "peng1", "peng2", "peng4", "pi3", "pi1", "pi2", "pi4", "pie3", "pie1", "pie4", "piao3",
    "piao1", "piao2", "piao4", "pian1", "pian2", "pian4", "pin3", "pin1", "pin2", "pin4", "ping3", "ping1", "ping2", "pu3", "pu1", "pu2",
    "pu4", "ma3", "ma1", "ma2", "ma4", "ma5", "mo3", "mo1", "mo2", "mo4", "mo5", "me1", "me4", "me5", "mai3", "mai2",
    "mai4", "mei3", "mei2", "mei4", "mei5", "mao3", "mao1", "mao2", "mao4", "mou3", "mou2", "man3", "man2", "man4", "men3", "men1",
    "men2", "men4", "men5", "mang3", "mang2", "meng3", "meng2", "meng4", "mi3", "mi1", "mi2", "mi4", "mie1", "mie4", "miao3", "miao1",
    "miao2", "miao4", "miu1", "miu4", "mian3", "mian2", "mian4", "min3", "min2", "ming3", "ming2", "ming4", "mu3", "mu2", "mu4", "fa3",
    "fa1", "fa2", "fa4", "fo2", "fei3", "fei1", "fei2", "fei4", "fou3", "fou2", "fan3", "fan1", "fan2", "fan4", "fen3", "fen1",
    "fen2", "fen4", "fen5", "fang3", "fang1", "fang2", "fang4", "feng3", "feng1", "feng2", "feng4", "fiao4", "fu3", "fu1", "fu2", "fu4",
    "da3", "da1", "da2", "da4", "de2", "de5", "dai3", "dai1", "dai4", "dei3", "dao3", "dao1", "dao4", "dou3", "dou2", "dou1",
    "dou4", "dan3", "dan1", "dan4", "dang3", "dang1", "dang4", "deng3", "deng1", "deng4", "di3", "di1", "di2", "di4", "di5", "dia1",
    "die1", "die2", "diao3", "diao1", "diao4", "diu1", "dian3", "dian1", "dian4", "diang1", "ding3", "ding1", "ding4", "du3", "du1", "du2",
    "du4", "duo3", "duo1", "duo2", "duo4", "dui1", "dui4", "duan3", "duan1", "duan4", "dun3", "dun1", "dun4", "dong3", "dong1", "dong4",
    "ta3", "ta1", "ta4", "te4", "tai1", "tai2", "tai4", "tei1", "tao3", "tao1", "tao2", "tao4", "tou3", "tou1", "tou2", "tou4",
    "tou5", "tan3", "tan1", "tan2", "tan4", "tang3", "tang1", "tang2", "tang4", "teng2", "ti3", "ti1", "ti2", "ti4", "tie3", "tie1",
    "tie4", "tiao3", "tiao1", "tiao2", "tiao4", "tian3", "tian1", "tian2", "tian4", "ting3", "ting1", "ting2", "ting4", "tu3", "tu1", "tu2",
    "tu4", "tuo3", "tuo1", "tuo2", "tuo4", "tui3", "tui1", "tui2", "tui4", "tuan1", "tuan2", "tuan4", "tun3", "tun1", "tun2", "tun4",
    "tong3", "tong1", "tong2", "tong4", "na3", "na2", "na4", "na5", "ne1", "ne2", "ne4", "ne5", "nai3", "nai2", "nai4", "nei3",
    "nei4", "nao3", "nao1", "nao2", "nao4", "nou2", "nou4", "nan3", "nan1", "nan2", "nan4", "nen4", "nang3", "nang1", "nang2", "nang4",
    "nang5", "neng2", "neng4", "ni3", "ni2", "ni4", "nie1", "nie2", "nie4", "niao3", "niao4", "niu3", "niu1", "niu2", "niu4", "nian3",
    "nian1", "nian2", "nian4", "nin3", "nin2", "niang2", "niang4", "ning3", "ning2", "ning4", "nu3", "nu2", "nu4", "nuo3", "nuo2", "nuo4",
    "nuan3", "nun2", "nong2", "nong4", "nv3", "nv4", "nue4", "la3", "la1", "la2", "la4", "la5", "lo5", "le4", "le5", "lai2",
    "lai4", "lei3", "lei1", "lei2", "lei4", "lao3", "lao1", "lao2", "lao4", "lou3", "lou1", "lou2", "lou4", "lan3", "lan2", "lan4",
    "lang3", "lang1", "lang2", "lang4", "leng3", "leng2", "leng4", "li3", "li1", "li2", "li4", "lia3", "lie3", "lie1", "lie4", "liao3",
    "liao1", "liao2", "liao4", "liu3", "liu1", "liu2", "liu4", "lian3", "lian2", "lian4", "lin3", "lin2", "lin4", "liang3", "liang2", "liang4",
    "ling3", "ling1", "ling2", "ling4", "lu3", "lu1", "lu2", "lu4", "lu5", "luo3", "luo1", "luo2", "luo4", "luan3", "luan2", "luan4",
    "lun3", "lun1", "lun2", "lun4", "long3", "long2", "long4", "long5", "lv3", "lv2", "lv4", "lue4", "lvan3", "lvan2", "ga3", "ga1",
    "ga2", "ga4", "ga5", "ge3", "ge1", "ge2", "ge4", "ge5", "gai3", "gai1", "gai4", "gei3", "gao3", "gao1", "gao4", "gou3",
    "gou1", "gou4", "gan3", "gan1", "gan4", "gen3", "gen1", "gen2", "gen4", "gang3", "gang1", "gang4", "geng3", "geng1", "geng4", "gu3",
    "gu1", "gu2", "gu4", "gu5", "gua3", "gua1", "gua4", "guo3", "guo1", "guo2", "guo4", "guai3", "guai1", "guai4", "gui3", "gui1",
    "gui4", "guan3", "guan1", "guan4", "gun3", "gun4", "guang3", "guang1", "guang4", "gong3", "gong1", "gong4", "ka3", "ka1", "ka4", "ke3",
    "ke1", "ke2", "ke4", "kai3", "kai1", "kai4", "kao3", "kao1", "kao4", "kou3", "kou1", "kou4", "kan3", "kan1", "kan4", "kan5",
    "ken3", "ken4", "kang3", "kang1", "kang2", "kang4", "keng3", "keng1", "ku3", "ku1", "ku4", "kua3", "kua1", "kua4", "kuo3", "kuo4",
    "kuai3", "kuai1", "kuai4", "kui3", "kui1", "kui2", "kui4", "kuan3", "kuan1", "kun3", "kun1", "kun4", "kuang3", "kuang1", "kuang2", "kuang4",
    "kong3", "kong1", "kong4", "ha3", "ha1", "ha2", "he1", "he2", "he4", "hai3", "hai1", "hai2", "hai4", "hai5", "hei3", "hei1",
    "hao3", "hao1", "hao2", "hao4", "hou3", "hou1", "hou2", "hou4", "han3", "han1", "han2", "han4", "hen3", "hen2", "hen4", "hang3",
    "hang1", "hang2", "hang4", "heng1", "heng2", "heng4", "hu3", "hu1", "hu2", "hu4", "hu5", "hua1", "hua2", "hua4", "huo3", "huo1",
    "huo2", "huo4", "huo5", "huai2", "huai4", "hui3", "hui1", "hui2", "hui4", "huan3", "huan1", "huan2", "huan4", "hun3", "hun1", "hun2",
    "hun4", "huang3", "huang1", "huang2", "huang4", "hong3", "hong1", "hong2", "hong4", "ji3", "ji1", "ji2", "ji4", "jia3", "jia1", "jia2",
    "jia4", "jie3", "jie1", "jie2", "jie4", "jiao3", "jiao1", "jiao2", "jiao4", "jiu3", "jiu1", "jiu4", "jiu5", "jian3", "jian1", "jian4",
    "jin3", "jin1", "jin4", "jiang3", "jiang1", "jiang4", "jing3", "jing1", "jing4", "ju3", "ju1", "ju2", "ju4", "jue3", "jue1", "jue2",
    "jue4", "juan3", "juan1", "juan4", "jun3", "jun1", "jun4", "jiong3", "jiong1", "qi3", "qi1", "qi2", "qi4", "qia3", "qia1", "qia4",
    "qie3", "qie1", "qie2", "qie4", "qiao3", "qiao1", "qiao2", "qiao4", "qiao5", "qiu3", "qiu1", "qiu2", "qian3", "qian1", "qian2", "qian4",
    "qin3", "qin1", "qin2", "qin4", "qiang3", "qiang1", "qiang2", "qiang4", "qing3", "qing1", "qing2", "qing4", "qu3", "qu1", "qu2", "qu4",
    "que1", "que2", "que4", "quan3", "quan1", "quan2", "quan4", "qun1", "qun2", "qiong1", "qiong2", "xi3", "xi1", "xi2", "xi4", "xi5",
    "xia1", "xia2", "xia4", "xie3", "xie1", "xie2", "xie4", "xie5", "xiao3", "xiao1", "xiao2", "xiao4", "xiu3", "xiu1", "xiu4", "xian3",
    "xian1", "xian2", "xian4", "xin3", "xin1", "xin2", "xin4", "xin5", "xiang3", "xiang1", "xiang2", "xiang4", "xing3", "xing1", "xing2", "xing4",
    "xu3", "xu1", "xu2", "xu4", "xue3", "xue1", "xue2", "xue4", "xuan3", "xuan1", "xuan2", "xuan4", "xun1", "xun2", "xun4", "xun5",
    "xiong1", "xiong2", "xiong4", "zhi3", "zhi1", "zhi2", "zhi4", "zha3", "zha1", "zha2", "zha4", "zhe3", "zhe1", "zhe2", "zhe4", "zhe5",
    "zhai3", "zhai1", "zhai2", "zhai4", "zhao3", "zhao1", "zhao2", "zhao4", "zhou3", "zhou1", "zhou2", "zhou4", "zhou5", "zhan3", "zhan1", "zhan4",
    "zhen3", "zhen1", "zhen4", "zhang3", "zhang1", "zhang4", "zheng3", "zheng1", "zheng4", "zhu3", "zhu1", "zhu2", "zhu4", "zhua3", "zhua1", "zhuo1",
    "zhuo2", "zhuai3", "zhuai1", "zhuai4", "zhui3", "zhui1", "zhui4", "zhuan3", "zhuan1", "zhuan4", "zhun3", "zhun1", "zhun4", "zhuang3", "zhuang1", "zhuang4",
    "zhong3", "zhong1", "zhong4", "chi3", "chi1", "chi2", "chi4", "cha3", "cha1", "cha2", "cha4", "che3", "che1", "che4", "chai1", "chai2",
    "chai4", "chao3", "chao1", "chao2", "chao4", "chou3", "chou1", "chou2", "chou4", "chan3", "chan1", "chan2", "chan4", "chen3", "chen1", "chen2",
    "chen4", "chang3", "chang1", "chang2", "chang4", "cheng3", "cheng1", "cheng2", "cheng4", "chu3", "chu1", "chu2", "chu4", "chua1", "chuo1", "chuo4",
    "chuai3", "chuai2", "chuai4", "chui1", "chui2", "chui4", "chuan3", "chuan1", "chuan2", "chuan4", "chun3", "chun1", "chun2", "chuang3", "chuang1", "chuang2",
    "chuang4", "chong3", "chong1", "chong2", "chong4", "shi3", "shi1", "shi2", "shi4", "shi5", "sha3", "sha1", "sha2", "sha4", "she3", "she1",
    "she2", "she4", "shai3", "shai1", "shai4", "shei2", "shao3", "shao1", "shao2", "shao4", "shou3", "shou1", "shou2", "shou4", "shan3", "shan1",
    "shan4", "shen3", "shen1", "shen2", "shen4", "shen5", "shang3", "shang1", "shang4", "shang5", "sheng3", "sheng1", "sheng2", "sheng4", "shu3", "shu1",
    "shu2", "shu4", "shua3", "shua1", "shuo1", "shuo4", "shuai3", "shuai1", "shuai4", "shui3", "shui2", "shui4", "shuan1", "shuan4", "shun3", "shun4",
    "shuang3", "shuang1", "shuang4", "ri4", "re3", "re4", "rao3", "rao2", "rao4", "rou3", "rou2", "rou4", "ran3", "ran2", "ren3", "ren2",
    "ren4", "rang3", "rang2", "rang4", "reng3", "reng1", "reng2", "ru3", "ru2", "ru4", "ruo2", "ruo4", "rui3", "rui2", "rui4", "ruan3",
    "ruan2", "run1", "run4", "rong3", "rong2", "rong4", "zi3", "zi1", "zi4", "zi5", "za1", "za2", "ze3", "ze2", "ze4", "zai3",
    "zai1", "zai4", "zei2", "zao3", "zao1", "zao2", "zao4", "zou3", "zou1", "zou4", "zan3", "zan1", "zan2", "zan4", "zen3", "zen4",
    "zang3", "zang1", "zang4", "zeng1", "zeng4", "zu3", "zu1", "zu2", "zuo3", "zuo1", "zuo2", "zuo4", "zui3", "zui1", "zui4", "zuan3",
    "zuan1", "zuan4", "zun3", "zun1", "zun4", "zong3", "zong1", "zong4", "ci3", "ci1", "ci2", "ci4", "ca3", "ca1", "ca4", "ce4",
    "cai3", "cai1", "cai2", "cai4", "cao3", "cao1", "cao2", "cao4", "cou4", "can3", "can1", "can2", "can4", "cen1", "cen2", "cang1",
    "cang2", "cang4", "ceng1", "ceng2", "ceng4", "cu1", "cu2", "cu4", "cuo3", "cuo1", "cuo2", "cuo4", "cui3", "cui1", "cui2", "cui4",
    "cuan1", "cuan2", "cuan4", "cun3", "cun1", "cun2", "cun4", "cong1", "cong2", "cong4", "si3", "si1", "si4", "si5", "sa3", "sa1",
    "sa4", "se4", "sai1", "sai4", "sao3", "sao1", "sao4", "sao5", "sou3", "sou1", "sou4", "san3", "san1", "san4", "sen1", "sang3",
    "sang1", "sang4", "seng1", "su1", "su2", "su4", "suo3", "suo1", "suo4", "sui3", "sui1", "sui2", "sui4", "suan3", "suan1", "suan4",
    "sun3", "sun1", "sun4", "song3", "song1", "song4", "a1", "a4", "a5", "o3", "o1", "o2", "e3", "e1", "e2", "e4",
    "ai3", "ai1", "ai2", "ai4", "ai5", "ei4", "ao3", "ao1", "ao2", "ao4", "ou3", "ou1", "ou2", "ou4", "an3", "an1",
    "an2", "an4", "en1", "en4", "ang3", "ang1", "ang2", "ang4", "eng1", "er3", "er1", "er2", "er4", "yi3", "yi1", "yi2",
    "yi4", "ya3", "ya1", "ya2", "ya4", "ya5", "yo1", "ye3", "ye1", "ye2", "ye4", "ye5", "yai2", "yao3", "yao1", "yao2",
    "yao4", "you3", "you1", "you2", "you4", "yan3", "yan1", "yan2", "yan4", "yin3", "yin1", "yin2", "yin4", "yang3", "yang1", "yang2",
    "yang4", "ying3", "ying1", "ying2", "ying4", "wu3", "wu1", "wu2", "wu4", "wa3", "wa1", "wa2", "wa4", "wa5", "wo3", "wo1",
    "wo4", "wai3", "wai1", "wai4", "wei3", "wei1", "wei2", "wei4", "wan3", "wan1", "wan2", "wan4", "wen3", "wen1", "wen2", "wen4",
    "wang3", "wang1", "wang2", "wang4", "weng3", "weng1", "weng4", "yu3", "yu1", "yu2", "yu4", "yue1", "yue4", "yuan3", "yuan1", "yuan2",
    "yuan4", "yun3", "yun1", "yun2", "yun4", "yong3", "yong1", "yong2", "yong4", "pa5", "pen4", "pia1", "pian3", "ping4", "mou4", "man1",
    "meng1", "fou1", "fou4", "da5", "dao2", "tai5", "tao5", "tuan3", "na1", "nuei2", "nong3", "lou5", "leng1", "li5", "lie2", "gun1",
    "hen1", "huang5", "qi5", "qiong3", "xia3", "zhei4", "chai3", "chua3", "chuai1", "sha5", "shua4", "rang1", "reng4", "run2", "zen1", "zu4",
    "cou3", "cang3", "ceng3", "cu3", "sa5", "sei1", "suo2", "song2", "a2", "ei4", "en3", "en5", "fang5", "fu5", "fong4", "dang5",
    "dian5", "duo5", "nai5", "niang5", "lie5", "liang5", "gei2", "gong5", "hou5", "jie5", "jian5", "xing5", "shu5", "sou5", "suo5", "er5",
    "yang5"
};
    
    
    
static const size_t kReadBufLen = 512;
static const size_t kSplTableHashLen = 60000;

// Compare a SingleCharItem, first by Hanzis, then by spelling ids, then by
// frequencies.
int cmp_scis_hz_splid_freq(const void* p1, const void* p2) {
  const SingleCharItem *s1, *s2;
  s1 = static_cast<const SingleCharItem*>(p1);
  s2 = static_cast<const SingleCharItem*>(p2);

  if (s1->hz < s2->hz)
    return -1;
  if (s1->hz > s2->hz)
    return 1;

  if (s1->splid.half_splid < s2->splid.half_splid)
    return -1;
  if (s1->splid.half_splid > s2->splid.half_splid)
    return 1;

  if (s1->splid.full_splid < s2->splid.full_splid)
    return -1;
  if (s1->splid.full_splid > s2->splid.full_splid)
    return 1;

  if (s1->freq > s2->freq)
    return -1;
  if (s1->freq < s2->freq)
    return 1;
  return 0;
}

int cmp_scis_hz_splid(const void* p1, const void* p2) {
  const SingleCharItem *s1, *s2;
  s1 = static_cast<const SingleCharItem*>(p1);
  s2 = static_cast<const SingleCharItem*>(p2);

  if (s1->hz < s2->hz)
    return -1;
  if (s1->hz > s2->hz)
    return 1;

  if (s1->splid.half_splid < s2->splid.half_splid)
    return -1;
  if (s1->splid.half_splid > s2->splid.half_splid)
    return 1;

  if (s1->splid.full_splid < s2->splid.full_splid)
    return -1;
  if (s1->splid.full_splid > s2->splid.full_splid)
    return 1;

  return 0;
}

int cmp_lemma_entry_hzs(const void* p1, const void* p2) {
  size_t size1 = utf16_strlen(((const LemmaEntry*)p1)->hanzi_str);
  size_t size2 = utf16_strlen(((const LemmaEntry*)p2)->hanzi_str);
  if (size1 < size2)
    return -1;
  else if (size1 > size2)
    return 1;

  return utf16_strcmp(((const LemmaEntry*)p1)->hanzi_str,
                      ((const LemmaEntry*)p2)->hanzi_str);
}

int compare_char16(const void* p1, const void* p2) {
  if (*((const char16*)p1) < *((const char16*)p2))
    return -1;
  if (*((const char16*)p1) > *((const char16*)p2))
    return 1;
  return 0;
}

int compare_py(const void* p1, const void* p2) {
  int ret = utf16_strcmp(((const LemmaEntry*)p1)->spl_idx_arr,
                         ((const LemmaEntry*)p2)->spl_idx_arr);

  if (0 != ret)
    return ret;

  return static_cast<int>(((const LemmaEntry*)p2)->freq) -
         static_cast<int>(((const LemmaEntry*)p1)->freq);
}

// First hanzi, if the same, then Pinyin
int cmp_lemma_entry_hzspys(const void* p1, const void* p2) {
  size_t size1 = utf16_strlen(((const LemmaEntry*)p1)->hanzi_str);
  size_t size2 = utf16_strlen(((const LemmaEntry*)p2)->hanzi_str);
  if (size1 < size2)
    return -1;
  else if (size1 > size2)
    return 1;
  int ret = utf16_strcmp(((const LemmaEntry*)p1)->hanzi_str,
                         ((const LemmaEntry*)p2)->hanzi_str);

  if (0 != ret)
    return ret;

  ret = utf16_strcmp(((const LemmaEntry*)p1)->spl_idx_arr,
                     ((const LemmaEntry*)p2)->spl_idx_arr);
  return ret;
}

int compare_splid2(const void* p1, const void* p2) {
  int ret = utf16_strcmp(((const LemmaEntry*)p1)->spl_idx_arr,
                         ((const LemmaEntry*)p2)->spl_idx_arr);
  return ret;
}

DictBuilder::DictBuilder() {
  lemma_arr_ = NULL;
  lemma_num_ = 0;

  scis_ = NULL;
  scis_num_ = 0;

  lma_nodes_le0_ = NULL;
  lma_nodes_ge1_ = NULL;

  lma_nds_used_num_le0_ = 0;
  lma_nds_used_num_ge1_ = 0;

  homo_idx_buf_ = NULL;
  homo_idx_num_eq1_ = 0;
  homo_idx_num_gt1_ = 0;

  top_lmas_ = NULL;
  top_lmas_num_ = 0;

  spl_table_ = NULL;
  spl_parser_ = NULL;
}

DictBuilder::~DictBuilder() {
  free_resource();
}

bool DictBuilder::alloc_resource(size_t lma_num) {
  if (0 == lma_num)
    return false;

  free_resource();

  lemma_num_ = lma_num;
  lemma_arr_ = new LemmaEntry[lemma_num_];

  top_lmas_num_ = 0;
  top_lmas_ = new LemmaEntry[kTopScoreLemmaNum];

  // New the scis_ buffer to the possible maximum size.
  scis_num_ = lemma_num_ * kMaxLemmaSize;
  scis_ = new SingleCharItem[scis_num_];

  // The root and first level nodes is less than kMaxSpellingNum + 1
  lma_nds_used_num_le0_ = 0;
  lma_nodes_le0_ = new LmaNodeLE0[kMaxSpellingNum + 1];

  // Other nodes is less than lemma_num
  lma_nds_used_num_ge1_ = 0;
  lma_nodes_ge1_ = new LmaNodeGE1[lemma_num_*2];

  homo_idx_buf_ = new LemmaIdType[lemma_num_];
  spl_table_ = new SpellingTable();
  spl_parser_ = new SpellingParser();

  if (NULL == lemma_arr_ || NULL == top_lmas_ ||
      NULL == scis_ || NULL == spl_table_ ||
      NULL == spl_parser_ || NULL == lma_nodes_le0_ ||
      NULL == lma_nodes_ge1_ || NULL == homo_idx_buf_) {
    free_resource();
    return false;
  }

  memset(lemma_arr_, 0, sizeof(LemmaEntry) * lemma_num_);
  memset(scis_, 0, sizeof(SingleCharItem) * scis_num_);
  memset(lma_nodes_le0_, 0, sizeof(LmaNodeLE0) * (kMaxSpellingNum + 1));
  memset(lma_nodes_ge1_, 0, sizeof(LmaNodeGE1) * lemma_num_*2);
  memset(homo_idx_buf_, 0, sizeof(LemmaIdType) * lemma_num_);
  spl_table_->init_table(kMaxPinyinSize, kSplTableHashLen, true);

  return true;
}

char16* DictBuilder::read_valid_hanzis(const char *fn_validhzs, size_t *num) {
  if (NULL == fn_validhzs || NULL == num)
    return NULL;

  *num = 0;
  FILE *fp = fopen(fn_validhzs, "rb");
  if (NULL == fp)
    return NULL;

  char16 utf16header;
  if (fread(&utf16header, sizeof(char16), 1, fp) != 1 ||
      0xfeff != utf16header) {
    fclose(fp);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  *num = ftell(fp) / sizeof(char16);
  assert(*num >= 1);
  *num -= 1;

  char16 *hzs = new char16[*num];
  if (NULL == hzs) {
    fclose(fp);
    return NULL;
  }

  fseek(fp, 2, SEEK_SET);

  if (fread(hzs, sizeof(char16), *num, fp) != *num) {
    fclose(fp);
    delete [] hzs;
    return NULL;
  }
  fclose(fp);

  myqsort(hzs, *num, sizeof(char16), compare_char16);
  return hzs;
}

bool DictBuilder::hz_in_hanzis_list(const char16 *hzs, size_t hzs_len,
                                    char16 hz) {
  if (NULL == hzs)
    return false;

  char16 *found;
  found = static_cast<char16*>(
      mybsearch(&hz, hzs, hzs_len, sizeof(char16), compare_char16));
  if (NULL == found)
    return false;

  assert(*found == hz);
  return true;
}

// The caller makes sure that the parameters are valid.
bool DictBuilder::str_in_hanzis_list(const char16 *hzs, size_t hzs_len,
                                     const char16 *str, size_t str_len) {
  if (NULL == hzs || NULL == str)
    return false;

  for (size_t pos = 0; pos < str_len; pos++) {
    if (!hz_in_hanzis_list(hzs, hzs_len, str[pos]))
      return false;
  }
  return true;
}

void DictBuilder::get_top_lemmas() {
  top_lmas_num_ = 0;
  if (NULL == lemma_arr_)
    return;

  for (size_t pos = 0; pos < lemma_num_; pos++) {
    if (0 == top_lmas_num_) {
      top_lmas_[0] = lemma_arr_[pos];
      top_lmas_num_ = 1;
      continue;
    }

    if (lemma_arr_[pos].freq > top_lmas_[top_lmas_num_ - 1].freq) {
      if (kTopScoreLemmaNum > top_lmas_num_)
        top_lmas_num_ += 1;

      size_t move_pos;
      for (move_pos = top_lmas_num_ - 1; move_pos > 0; move_pos--) {
        top_lmas_[move_pos] = top_lmas_[move_pos - 1];
        if (0 == move_pos - 1 ||
            (move_pos - 1 > 0 &&
             top_lmas_[move_pos - 2].freq > lemma_arr_[pos].freq)) {
          break;
        }
      }
      assert(move_pos > 0);
      top_lmas_[move_pos - 1] = lemma_arr_[pos];
    } else if (kTopScoreLemmaNum > top_lmas_num_) {
      top_lmas_[top_lmas_num_] = lemma_arr_[pos];
      top_lmas_num_ += 1;
    }
  }

  if (kPrintDebug0) {
    printf("\n------Top Lemmas------------------\n");
    for (size_t pos = 0; pos < top_lmas_num_; pos++) {
      printf("--%d, idx:%06d, score:%.5f\n", pos, top_lmas_[pos].idx_by_hz,
             top_lmas_[pos].freq);
    }
  }
}

void DictBuilder::free_resource() {
  if (NULL != lemma_arr_)
    delete [] lemma_arr_;

  if (NULL != scis_)
    delete [] scis_;

  if (NULL != lma_nodes_le0_)
    delete [] lma_nodes_le0_;

  if (NULL != lma_nodes_ge1_)
    delete [] lma_nodes_ge1_;

  if (NULL != homo_idx_buf_)
    delete [] homo_idx_buf_;

  if (NULL != spl_table_)
    delete spl_table_;

  if (NULL != spl_parser_)
    delete spl_parser_;

  lemma_arr_ = NULL;
  scis_ = NULL;
  lma_nodes_le0_ = NULL;
  lma_nodes_ge1_ = NULL;
  homo_idx_buf_ = NULL;
  spl_table_ = NULL;
  spl_parser_ = NULL;

  lemma_num_ = 0;
  lma_nds_used_num_le0_ = 0;
  lma_nds_used_num_ge1_ = 0;
  homo_idx_num_eq1_ = 0;
  homo_idx_num_gt1_ = 0;
}

void print_error(char16* token, size_t token_length, int line_id, string msg) {
    u16string source((const char16_t*)token, token_length);
    cout << converter.to_bytes(source) << ", " << "line: "
    << line_id << ", " << msg << endl;
}

size_t DictBuilder::read_raw_dict(const char* fn_raw,
                                  const char *fn_validhzs,
                                  size_t max_item) {
  if (NULL == fn_raw) return 0;

  Utf16Reader utf16_reader;
  if (!utf16_reader.open(fn_raw, kReadBufLen * 10))
    return false;

  char16 read_buf[kReadBufLen];

  // Read the number of lemmas in the file
  size_t lemma_num = 240000;

  // allocate resource required
  if (!alloc_resource(lemma_num)) {
    utf16_reader.close();
  }

  // Read the valid Hanzi list.
  char16 *valid_hzs = NULL;
  size_t valid_hzs_num = 0;
  valid_hzs = read_valid_hanzis(fn_validhzs, &valid_hzs_num);
    
    if (valid_hzs == NULL || valid_hzs_num == 0) {
        cout << "warning: failed to read valid hanzi file " << fn_validhzs << endl;
    }
    
    
    std::set<string> valid_pinyin_set;
    for (size_t i = 0 ; i < sizeof(valid_pinyins)/sizeof(char*); i++) {
        valid_pinyin_set.insert(string(valid_pinyins[i]));
    }
    
    std::set<u16string> duplicated_checker;
    std::set<u16string> single_hanzi_checker;

  // Begin reading the lemma entries
  for (size_t i = 0, line_count = 0; i < max_item; i++, line_count++) {
    // read next entry
    if (!utf16_reader.readline(read_buf, kReadBufLen)) {
      lemma_num = i;
      break;
    }

    size_t token_size;
    char16 *token;
    char16 *to_tokenize = read_buf;
      u16string duplicated_check_record;

    // Get the Hanzi string
    token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
    if (NULL == token) {
      free_resource();
      utf16_reader.close();
      return false;
    }

    size_t lemma_size = utf16_strlen(token);

    if (lemma_size > kMaxLemmaSize || lemma_size > kMaxSentenceLength) {
        print_error(token, lemma_size, line_count,
                    string("exceed max sentence length ") + to_string(kMaxSentenceLength));
      i--;
      continue;
    }
/*
    if (lemma_size > 4) {
      i--;
      continue;
    }
*/
    // Copy to the lemma entry
    utf16_strcpy(lemma_arr_[i].hanzi_str, token);

    lemma_arr_[i].hz_str_len = token_size;
      
      duplicated_check_record += u16string((const char16_t*)token, token_size);

    // Get the freq string
    token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
    if (NULL == token) {
      free_resource();
      utf16_reader.close();
      return false;
    }
    lemma_arr_[i].freq = utf16_atof(token);

    if (lemma_size > 1 && lemma_arr_[i].freq < 60) {
        print_error(lemma_arr_[i].hanzi_str, lemma_arr_[i].hz_str_len, line_count,
                    string("frequency ") + to_string(lemma_arr_[i].freq) + string(" < 60 "));
      i--;
      continue;
    }

    // Get GBK mark, if no valid Hanzi list available, all items which contains
    // GBK characters will be discarded. Otherwise, all items which contains
    // characters outside of the valid Hanzi list will be discarded.
    token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
    assert(NULL != token);
    int gbk_flag = utf16_atoi(token);
      /* disable gbk flag check for zhuyin
    if (NULL == valid_hzs || 0 == valid_hzs_num) {
      if (0 != gbk_flag) {
          print_error(lemma_arr_[i].hanzi_str, lemma_arr_[i].hz_str_len, line_count,
                      string("word gbk flag is 0"));
        i--;
        continue;
      }
    } else {
        
      if (1 != gbk_flag && !str_in_hanzis_list(valid_hzs, valid_hzs_num,
          lemma_arr_[i].hanzi_str, lemma_arr_[i].hz_str_len)) {
          print_error(lemma_arr_[i].hanzi_str, lemma_arr_[i].hz_str_len, line_count,
                      string("word not in valid hanzi table"));
        i--;
        continue;
      }
    }
        */

    // Get spelling String
    bool spelling_not_support = false;
      bool single_hanzi_spelling_not_defined = false;
      bool spelling_not_valid_zhuyin_pinyin = false;
    for (size_t hz_pos = 0; hz_pos < (size_t)lemma_arr_[i].hz_str_len;
         hz_pos++) {
      // Get a Pinyin
      token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
      if (NULL == token) {
        free_resource();
        utf16_reader.close();
        return false;
      }
        
      assert(utf16_strlen(token) <= kMaxPinyinSize);

      utf16_strcpy_tochar(lemma_arr_[i].pinyin_str[hz_pos], token);

      format_spelling_str(lemma_arr_[i].pinyin_str[hz_pos]);
        
        // Put the pinyin (removed tone) to the spelling table if it contains tones
        // Ex: There is only CE4 and we need to add CE
        size_t pinyin_str_len = strlen(lemma_arr_[i].pinyin_str[hz_pos]);
        char char_last = lemma_arr_[i].pinyin_str[hz_pos][pinyin_str_len-1];
        if (char_last >= '1' && char_last <= '5') {
            char str_no_tone [kMaxPinyinSize + 1];
            strncpy(str_no_tone, lemma_arr_[i].pinyin_str[hz_pos], pinyin_str_len - 1);
            str_no_tone[pinyin_str_len - 1] = '\0';
            
            // Add a virtual spelling with freq = 0.0
            if (!spl_table_->put_spelling(str_no_tone, 0.0)) {
                spelling_not_support = true;
                break;
            }
        }

      // Put the pinyin to the spelling table
      if (!spl_table_->put_spelling(lemma_arr_[i].pinyin_str[hz_pos],
                                    lemma_arr_[i].freq)) {
        spelling_not_support = true;
        break;
      }
        
        duplicated_check_record += u16string((const char16_t*)"\x20\x00", 1);
        duplicated_check_record += u16string((const char16_t*)token, token_size);
        
        if (lemma_arr_[i].hz_str_len == 1) {
            u16string single_hanzi = u16string((const char16_t*)lemma_arr_[i].hanzi_str, 1);
            single_hanzi += u16string((const char16_t*)"\x20\x00", 1);
            single_hanzi += u16string((const char16_t*)token, token_size);
            single_hanzi_checker.insert(single_hanzi);
        } else {
            u16string single_hanzi = u16string((const char16_t*)lemma_arr_[i].hanzi_str+hz_pos, 1);
            single_hanzi += u16string((const char16_t*)"\x20\x00", 1);
            single_hanzi += u16string((const char16_t*)token, token_size);
            
            if (single_hanzi_checker.count(single_hanzi) == 0) {
                print_error(lemma_arr_[i].hanzi_str+hz_pos, 1, line_count, string(lemma_arr_[i].pinyin_str[hz_pos]) + string(", spelling not defined"));
                //single_hanzi_spelling_not_defined = true;
            }
            
            string pinyin;
            for (size_t p_pos = 0; p_pos < pinyin_str_len; p_pos++)
                pinyin += std::tolower(lemma_arr_[i].pinyin_str[hz_pos][p_pos]);
            
            if (valid_pinyin_set.count(pinyin) == 0) {

                print_error(lemma_arr_[i].hanzi_str+hz_pos, 1, line_count, string(lemma_arr_[i].pinyin_str[hz_pos]) + string(", spelling not valid pinyin from zhuyin"));
                
                spelling_not_valid_zhuyin_pinyin = true;
            }
        }
    }

    // The whole line must have been parsed fully, otherwise discard this one.
    token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
    if (spelling_not_support || NULL != token || single_hanzi_spelling_not_defined || spelling_not_valid_zhuyin_pinyin) {
        
        if (spelling_not_support || NULL != token)
            print_error(lemma_arr_[i].hanzi_str, lemma_arr_[i].hz_str_len, line_count, string("spelling not support"));
      i--;
      continue;
    } else {
        if (duplicated_checker.count(duplicated_check_record) > 0) {
            print_error(lemma_arr_[i].hanzi_str, lemma_arr_[i].hz_str_len, line_count, string("skip duplicated record"));
            std::cout << converter.to_bytes(duplicated_check_record) << endl;
            
            i--;
            continue;
        }
        
        duplicated_checker.insert(duplicated_check_record);
    }
  }

  delete [] valid_hzs;
  utf16_reader.close();

  printf("read succesfully, lemma num: %zu\n", lemma_num);

  return lemma_num;
}

bool DictBuilder::build_dict(const char *fn_raw,
                             const char *fn_validhzs,
                             DictTrie *dict_trie) {
  if (NULL == fn_raw || NULL == dict_trie)
    return false;

  lemma_num_ = read_raw_dict(fn_raw, fn_validhzs, 240000);
  if (0 == lemma_num_)
    return false;

  // Arrange the spelling table, and build a spelling tree
  // The size of an spelling. '\0' is included. If the spelling table is
  // initialized to calculate the spelling scores, the last char in the
  // spelling string will be score, and it is also included in spl_item_size.
  size_t spl_item_size;
  size_t spl_num;
  const char* spl_buf;
  spl_buf = spl_table_->arrange(&spl_item_size, &spl_num);
  if (NULL == spl_buf) {
    free_resource();
    return false;
  }

  SpellingTrie &spl_trie = SpellingTrie::get_instance();

  if (!spl_trie.construct(spl_buf, spl_item_size, spl_num,
                          spl_table_->get_score_amplifier(),
                          spl_table_->get_average_score())) {
    free_resource();
    return false;
  }

  printf("spelling tree construct successfully.\n");

  // Convert the spelling string to idxs
  for (size_t i = 0; i < lemma_num_; i++) {
    for (size_t hz_pos = 0; hz_pos < (size_t)lemma_arr_[i].hz_str_len;
         hz_pos++) {
      uint16 spl_idxs[2];
      uint16 spl_start_pos[3];
      bool is_pre = true;
      int spl_idx_num =
        spl_parser_->splstr_to_idxs(lemma_arr_[i].pinyin_str[hz_pos],
                                    strlen(lemma_arr_[i].pinyin_str[hz_pos]),
                                    spl_idxs, spl_start_pos, 2, is_pre);
      assert(1 == spl_idx_num);

      if (spl_trie.is_half_id(spl_idxs[0])) {
        uint16 num = spl_trie.half_to_full(spl_idxs[0], spl_idxs);
        assert(0 != num);
      }
      lemma_arr_[i].spl_idx_arr[hz_pos] = spl_idxs[0];
    }
  }

  // Sort the lemma items according to the hanzi, and give each unique item a
  // id
  sort_lemmas_by_hz();

  scis_num_ = build_scis();

  // Construct the dict list
  dict_trie->dict_list_ = new DictList();
  bool dl_success = dict_trie->dict_list_->init_list(scis_, scis_num_,
                                                     lemma_arr_, lemma_num_);
  assert(dl_success);

  // Construct the NGram information
  NGram& ngram = NGram::get_instance();
  ngram.build_unigram(lemma_arr_, lemma_num_,
                      lemma_arr_[lemma_num_ - 1].idx_by_hz + 1);

  // sort the lemma items according to the spelling idx string
  myqsort(lemma_arr_, lemma_num_, sizeof(LemmaEntry), compare_py);

  get_top_lemmas();

#ifdef ___DO_STATISTICS___
  stat_init();
#endif

  lma_nds_used_num_le0_ = 1;  // The root node
  bool dt_success = construct_subset(static_cast<void*>(lma_nodes_le0_),
                                     lemma_arr_, 0, lemma_num_, 0);
  if (!dt_success) {
    free_resource();
    return false;
  }

#ifdef ___DO_STATISTICS___
  stat_print();
#endif

  // Move the node data and homo data to the DictTrie
  dict_trie->root_ = new LmaNodeLE0[lma_nds_used_num_le0_];
  dict_trie->nodes_ge1_ = new LmaNodeGE1[lma_nds_used_num_ge1_];
  size_t lma_idx_num = homo_idx_num_eq1_ + homo_idx_num_gt1_ + top_lmas_num_;
  dict_trie->lma_idx_buf_ = new unsigned char[lma_idx_num * kLemmaIdSize];
  assert(NULL != dict_trie->root_);
  assert(NULL != dict_trie->lma_idx_buf_);
  dict_trie->lma_node_num_le0_ = lma_nds_used_num_le0_;
  dict_trie->lma_node_num_ge1_ = lma_nds_used_num_ge1_;
  dict_trie->lma_idx_buf_len_ = lma_idx_num * kLemmaIdSize;
  dict_trie->top_lmas_num_ = top_lmas_num_;

  memcpy(dict_trie->root_, lma_nodes_le0_,
         sizeof(LmaNodeLE0) * lma_nds_used_num_le0_);
  memcpy(dict_trie->nodes_ge1_, lma_nodes_ge1_,
         sizeof(LmaNodeGE1) * lma_nds_used_num_ge1_);

  for (size_t pos = 0; pos < homo_idx_num_eq1_ + homo_idx_num_gt1_; pos++) {
    id_to_charbuf(dict_trie->lma_idx_buf_ + pos * kLemmaIdSize,
                  homo_idx_buf_[pos]);
  }

  for (size_t pos = homo_idx_num_eq1_ + homo_idx_num_gt1_;
       pos < lma_idx_num; pos++) {
    LemmaIdType idx =
        top_lmas_[pos - homo_idx_num_eq1_ - homo_idx_num_gt1_].idx_by_hz;
    id_to_charbuf(dict_trie->lma_idx_buf_ + pos * kLemmaIdSize, idx);
  }

  if (kPrintDebug0) {
    printf("homo_idx_num_eq1_: %d\n", homo_idx_num_eq1_);
    printf("homo_idx_num_gt1_: %d\n", homo_idx_num_gt1_);
    printf("top_lmas_num_: %d\n", top_lmas_num_);
  }

  free_resource();

  if (kPrintDebug0) {
    printf("Building dict succeds\n");
  }
  return dt_success;
}

void DictBuilder::id_to_charbuf(unsigned char *buf, LemmaIdType id) {
  if (NULL == buf) return;
  for (size_t pos = 0; pos < kLemmaIdSize; pos++) {
    (buf)[pos] = (unsigned char)(id >> (pos * 8));
  }
}

void DictBuilder::set_son_offset(LmaNodeGE1 *node, size_t offset) {
  node->son_1st_off_l = static_cast<uint16>(offset);
  node->son_1st_off_h = static_cast<unsigned char>(offset >> 16);
}

void DictBuilder:: set_homo_id_buf_offset(LmaNodeGE1 *node, size_t offset) {
  node->homo_idx_buf_off_l = static_cast<uint16>(offset);
  node->homo_idx_buf_off_h = static_cast<unsigned char>(offset >> 16);

}

// All spelling strings will be converted to upper case, except that
// spellings started with "ZH"/"CH"/"SH" will be converted to
// "Zh"/"Ch"/"Sh"
void DictBuilder::format_spelling_str(char *spl_str) {
  if (NULL == spl_str)
    return;

  uint16 pos = 0;
  while ('\0' != spl_str[pos]) {
    if (spl_str[pos] >= 'a' && spl_str[pos] <= 'z')
      spl_str[pos] = spl_str[pos] - 'a' + 'A';
      
      // MUST KEEP the following logic or pinyin "zh'" will be "z h'"
    if (1 == pos && 'H' == spl_str[pos]) {
      if ('C' == spl_str[0] || 'S' == spl_str[0] || 'Z' == spl_str[0]) {
        spl_str[pos] = 'h';
      }
    }
    
    pos++;
  }
}

LemmaIdType DictBuilder::sort_lemmas_by_hz() {
  if (NULL == lemma_arr_ || 0 == lemma_num_)
    return 0;

  myqsort(lemma_arr_, lemma_num_, sizeof(LemmaEntry), cmp_lemma_entry_hzs);

  lemma_arr_[0].idx_by_hz = 1;
  LemmaIdType idx_max = 1;
  for (size_t i = 1; i < lemma_num_; i++) {
    if (utf16_strcmp(lemma_arr_[i].hanzi_str, lemma_arr_[i-1].hanzi_str)) {
      idx_max++;
      lemma_arr_[i].idx_by_hz = idx_max;
    } else {
      idx_max++;
      lemma_arr_[i].idx_by_hz = idx_max;
    }
  }
  return idx_max + 1;
}

size_t DictBuilder::build_scis() {
  if (NULL == scis_ || lemma_num_ * kMaxLemmaSize > scis_num_)
    return 0;

  SpellingTrie &spl_trie = SpellingTrie::get_instance();

  // This first one is blank, because id 0 is invalid.
  scis_[0].freq = 0;
  scis_[0].hz = 0;
  scis_[0].splid.full_splid = 0;
  scis_[0].splid.half_splid = 0;
  scis_num_ = 1;

  // Copy the hanzis to the buffer
  for (size_t pos = 0; pos < lemma_num_; pos++) {
    size_t hz_num = lemma_arr_[pos].hz_str_len;
    for (size_t hzpos = 0; hzpos < hz_num; hzpos++) {
      scis_[scis_num_].hz = lemma_arr_[pos].hanzi_str[hzpos];
      scis_[scis_num_].splid.full_splid = lemma_arr_[pos].spl_idx_arr[hzpos];
      scis_[scis_num_].splid.half_splid =
          spl_trie.full_to_half(scis_[scis_num_].splid.full_splid);
      if (1 == hz_num)
        scis_[scis_num_].freq = lemma_arr_[pos].freq;
      else
        scis_[scis_num_].freq = 0.000001;
      scis_num_++;
    }
  }

  myqsort(scis_, scis_num_, sizeof(SingleCharItem), cmp_scis_hz_splid_freq);

  // Remove repeated items
  size_t unique_scis_num = 1;
  for (size_t pos = 1; pos < scis_num_; pos++) {
    if (scis_[pos].hz == scis_[pos - 1].hz &&
        scis_[pos].splid.full_splid == scis_[pos - 1].splid.full_splid)
      continue;
    scis_[unique_scis_num] = scis_[pos];
    scis_[unique_scis_num].splid.half_splid =
        spl_trie.full_to_half(scis_[pos].splid.full_splid);
    unique_scis_num++;
  }

  scis_num_ = unique_scis_num;

  // Update the lemma list.
  for (size_t pos = 0; pos < lemma_num_; pos++) {
    size_t hz_num = lemma_arr_[pos].hz_str_len;
    for (size_t hzpos = 0; hzpos < hz_num; hzpos++) {
      SingleCharItem key;
      key.hz = lemma_arr_[pos].hanzi_str[hzpos];
      key.splid.full_splid = lemma_arr_[pos].spl_idx_arr[hzpos];
      key.splid.half_splid = spl_trie.full_to_half(key.splid.full_splid);

      SingleCharItem *found;
      found = static_cast<SingleCharItem*>(mybsearch(&key, scis_,
                                                     unique_scis_num,
                                                     sizeof(SingleCharItem),
                                                     cmp_scis_hz_splid));

      assert(found);

      lemma_arr_[pos].hanzi_scis_ids[hzpos] =
          static_cast<uint16>(found - scis_);
      lemma_arr_[pos].spl_idx_arr[hzpos] = found->splid.full_splid;
    }
  }

  return scis_num_;
}

static const unsigned short kMaxVirtTblNum = 1024;

bool DictBuilder::construct_subset(void* parent, LemmaEntry* lemma_arr,
                                   size_t item_start, size_t item_end,
                                   size_t level) {
  if (level >= kMaxLemmaSize || item_end <= item_start)
    return false;
    
    uint16 virt_spl_tbl[kMaxVirtTblNum];
    size_t virt_pos_tbl[kMaxVirtTblNum];
    size_t virt_spl_tbl_num = 0;
    size_t virt_spl_tbl_stat = 0;
    
  // 1. Scan for how many sons
  size_t parent_son_num = 0;
  // LemmaNode *son_1st = NULL;
  // parent.num_of_son = 0;

  LemmaEntry *lma_last_start = lemma_arr_ + item_start;
  uint16 spl_idx_node = lma_last_start->spl_idx_arr[level];

  // Scan for how many sons to be allocaed
  for (size_t i = item_start + 1; i< item_end; i++) {
    LemmaEntry *lma_current = lemma_arr + i;
    uint16 spl_idx_current = lma_current->spl_idx_arr[level];
    if (spl_idx_current != spl_idx_node) {
      parent_son_num++;
        
        if (spl_idx_current - spl_idx_node == 2 &&
            spl_table_->get_score_by_idx(spl_idx_current - 1 - kFullSplIdStart) == 0xff) {
            // found a virt spl
            parent_son_num++;
            virt_spl_tbl[virt_spl_tbl_num] = spl_idx_current - 1;
            virt_spl_tbl_num++;
            assert(virt_spl_tbl_num<kMaxVirtTblNum);
        }
        
      spl_idx_node = spl_idx_current;
    }
  }
  parent_son_num++;

#ifdef ___DO_STATISTICS___
  // Use to indicate whether all nodes of this layer have no son.
  bool allson_noson = true;

  assert(level < kMaxLemmaSize);
  if (parent_son_num > max_sonbuf_len_[level])
    max_sonbuf_len_[level] = parent_son_num;

  total_son_num_[level] += parent_son_num;
  total_sonbuf_num_[level] += 1;

  if (parent_son_num == 1)
    sonbufs_num1_++;
  else
    sonbufs_numgt1_++;
  total_lma_node_num_ += parent_son_num;
#endif

  // 2. Update the parent's information
  //    Update the parent's son list;
  LmaNodeLE0 *son_1st_le0 = NULL;  // only one of le0 or ge1 is used
  LmaNodeGE1 *son_1st_ge1 = NULL;  // only one of le0 or ge1 is used.
  if (0 == level) {                 // the parent is root
    (static_cast<LmaNodeLE0*>(parent))->son_1st_off =
      lma_nds_used_num_le0_;
      
      assert(lma_nds_used_num_le0_ < kMaxSpellingNum);
      
    son_1st_le0 = lma_nodes_le0_ + lma_nds_used_num_le0_;
    lma_nds_used_num_le0_ += parent_son_num;
      
      

    assert(parent_son_num <= 65535);
    (static_cast<LmaNodeLE0*>(parent))->num_of_son =
      static_cast<uint16>(parent_son_num);
  } else if (1 == level) {  // the parent is a son of root
    (static_cast<LmaNodeLE0*>(parent))->son_1st_off =
      lma_nds_used_num_ge1_;
      
      assert(lma_nds_used_num_ge1_ < lemma_num_*2);
      
    son_1st_ge1 = lma_nodes_ge1_ + lma_nds_used_num_ge1_;
    lma_nds_used_num_ge1_ += parent_son_num;
      
      

    assert(parent_son_num <= 65535);
    (static_cast<LmaNodeLE0*>(parent))->num_of_son =
      static_cast<uint16>(parent_son_num);
  } else {
    set_son_offset((static_cast<LmaNodeGE1*>(parent)),
                   lma_nds_used_num_ge1_);
      assert(lma_nds_used_num_ge1_ < lemma_num_*2);
      
    son_1st_ge1 = lma_nodes_ge1_ + lma_nds_used_num_ge1_;
    lma_nds_used_num_ge1_ += parent_son_num;
      
    assert(parent_son_num <= 255);
    (static_cast<LmaNodeGE1*>(parent))->num_of_son =
      (unsigned char)parent_son_num;
  }

  // 3. Now begin to construct the son one by one
  size_t son_pos = 0;

  lma_last_start = lemma_arr_ + item_start;
  spl_idx_node = lma_last_start->spl_idx_arr[level];

  size_t homo_num = 0;
  if (lma_last_start->spl_idx_arr[level + 1] == 0)
    homo_num = 1;

  size_t item_start_next = item_start;

  for (size_t i = item_start + 1; i < item_end; i++) {
    LemmaEntry* lma_current = lemma_arr_ + i;
    uint16 spl_idx_current = lma_current->spl_idx_arr[level];

    if (spl_idx_current == spl_idx_node) {
      if (lma_current->spl_idx_arr[level + 1] == 0)
        homo_num++;
    } else {
        bool is_virt = false;
        
        if (spl_idx_current - spl_idx_node == 2 &&
            spl_table_->get_score_by_idx(spl_idx_current - 1 - kFullSplIdStart) == 0xff) {
            is_virt = true;
            virt_pos_tbl[virt_spl_tbl_stat] = son_pos + 1;
            virt_spl_tbl_stat++;
        }
        
      // Construct a node
      LmaNodeLE0 *node_cur_le0 = NULL;  // only one of them is valid
      LmaNodeGE1 *node_cur_ge1 = NULL;
      if (0 == level) {
        node_cur_le0 = son_1st_le0 + son_pos;
        node_cur_le0->spl_idx = spl_idx_node;
        node_cur_le0->homo_idx_buf_off = homo_idx_num_eq1_ + homo_idx_num_gt1_;
        node_cur_le0->son_1st_off = 0;
        homo_idx_num_eq1_ += homo_num;
      } else {
        node_cur_ge1 = son_1st_ge1 + son_pos;
        node_cur_ge1->spl_idx = spl_idx_node;

        set_homo_id_buf_offset(node_cur_ge1,
                               (homo_idx_num_eq1_ + homo_idx_num_gt1_));
        set_son_offset(node_cur_ge1, 0);
        homo_idx_num_gt1_ += homo_num;
      }

      if (homo_num > 0) {
        LemmaIdType* idx_buf = homo_idx_buf_ + homo_idx_num_eq1_ +
              homo_idx_num_gt1_ - homo_num;
        if (0 == level) {
          assert(homo_num <= 65535);
          node_cur_le0->num_of_homo = static_cast<uint16>(homo_num);
        } else {
          assert(homo_num <= 255);
          node_cur_ge1->num_of_homo = (unsigned char)homo_num;
        }

        for (size_t homo_pos = 0; homo_pos < homo_num; homo_pos++) {
          idx_buf[homo_pos] = lemma_arr_[item_start_next + homo_pos].idx_by_hz;
        }

#ifdef ___DO_STATISTICS___
        if (homo_num > max_homobuf_len_[level])
          max_homobuf_len_[level] = homo_num;

        total_homo_num_[level] += homo_num;
#endif
      }

      if (i - item_start_next > homo_num) {
        void *next_parent;
        if (0 == level)
          next_parent = static_cast<void*>(node_cur_le0);
        else
          next_parent = static_cast<void*>(node_cur_ge1);
        construct_subset(next_parent, lemma_arr,
                         item_start_next + homo_num, i, level + 1);
#ifdef ___DO_STATISTICS___

        total_node_hasson_[level] += 1;
        allson_noson = false;
#endif
      }

        if (is_virt) {
            son_pos++;
        }
        
      // for the next son
      lma_last_start = lma_current;
      spl_idx_node = spl_idx_current;
      item_start_next = i;
      homo_num = 0;
      if (lma_current->spl_idx_arr[level + 1] == 0)
        homo_num = 1;

      son_pos++;
    }
  }

  // 4. The last one to construct
  LmaNodeLE0 *node_cur_le0 = NULL;  // only one of them is valid
  LmaNodeGE1 *node_cur_ge1 = NULL;
  if (0 == level) {
    node_cur_le0 = son_1st_le0 + son_pos;
    node_cur_le0->spl_idx = spl_idx_node;
    node_cur_le0->homo_idx_buf_off = homo_idx_num_eq1_ + homo_idx_num_gt1_;
    node_cur_le0->son_1st_off = 0;
    homo_idx_num_eq1_ += homo_num;
  } else {
    node_cur_ge1 = son_1st_ge1 + son_pos;
    node_cur_ge1->spl_idx = spl_idx_node;

    set_homo_id_buf_offset(node_cur_ge1,
                           (homo_idx_num_eq1_ + homo_idx_num_gt1_));
    set_son_offset(node_cur_ge1, 0);
    homo_idx_num_gt1_ += homo_num;
  }

  if (homo_num > 0) {
    LemmaIdType* idx_buf = homo_idx_buf_ + homo_idx_num_eq1_ +
          homo_idx_num_gt1_ - homo_num;
    if (0 == level) {
      assert(homo_num <= 65535);
      node_cur_le0->num_of_homo = static_cast<uint16>(homo_num);
    } else {
      assert(homo_num <= 255);
      node_cur_ge1->num_of_homo = (unsigned char)homo_num;
    }

    for (size_t homo_pos = 0; homo_pos < homo_num; homo_pos++) {
      idx_buf[homo_pos] = lemma_arr[item_start_next + homo_pos].idx_by_hz;
    }

#ifdef ___DO_STATISTICS___
    if (homo_num > max_homobuf_len_[level])
      max_homobuf_len_[level] = homo_num;

    total_homo_num_[level] += homo_num;
#endif
  }

  if (item_end - item_start_next > homo_num) {
    void *next_parent;
    if (0 == level)
      next_parent = static_cast<void*>(node_cur_le0);
    else
      next_parent = static_cast<void*>(node_cur_ge1);
    construct_subset(next_parent, lemma_arr,
                     item_start_next + homo_num, item_end, level + 1);
#ifdef ___DO_STATISTICS___

    total_node_hasson_[level] += 1;
    allson_noson = false;
#endif
  }

#ifdef ___DO_STATISTICS___
  if (allson_noson) {
    total_sonbuf_allnoson_[level] += 1;
    total_node_in_sonbuf_allnoson_[level] += parent_son_num;
  }
#endif
    
    if (0 == level) {
        for (int i = 0; i < virt_spl_tbl_stat; i++) {
            LmaNodeLE0 *node = son_1st_le0 + virt_pos_tbl[i];
            memcpy(node, node+1, sizeof(LmaNodeLE0));
            node->spl_idx--;
        }
    } else {
        for (int i = 0; i < virt_spl_tbl_stat; i++) {
            LmaNodeGE1 *node = son_1st_ge1 + virt_pos_tbl[i];
            memcpy(node, node+1, sizeof(LmaNodeGE1));
            node->spl_idx--;
        }
    }
    assert(virt_spl_tbl_num == virt_spl_tbl_stat);
    
  assert(son_pos + 1 == parent_son_num);
  return true;
}

#ifdef ___DO_STATISTICS___
void DictBuilder::stat_init() {
  memset(max_sonbuf_len_, 0, sizeof(size_t) * kMaxLemmaSize);
  memset(max_homobuf_len_, 0, sizeof(size_t) * kMaxLemmaSize);
  memset(total_son_num_, 0, sizeof(size_t) * kMaxLemmaSize);
  memset(total_node_hasson_, 0, sizeof(size_t) * kMaxLemmaSize);
  memset(total_sonbuf_num_, 0, sizeof(size_t) * kMaxLemmaSize);
  memset(total_sonbuf_allnoson_, 0, sizeof(size_t) * kMaxLemmaSize);
  memset(total_node_in_sonbuf_allnoson_, 0, sizeof(size_t) * kMaxLemmaSize);
  memset(total_homo_num_, 0, sizeof(size_t) * kMaxLemmaSize);

  sonbufs_num1_ = 0;
  sonbufs_numgt1_ = 0;
  total_lma_node_num_ = 0;
}

void DictBuilder::stat_print() {
  printf("\n------------STAT INFO-------------\n");
  printf("[root is layer -1]\n");
  printf(".. max_sonbuf_len per layer(from layer 0):\n   ");
  for (size_t i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", max_sonbuf_len_[i]);
  printf("-, \n");

  printf(".. max_homobuf_len per layer:\n   -, ");
  for (size_t i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", max_homobuf_len_[i]);
  printf("\n");

  printf(".. total_son_num per layer:\n   ");
  for (size_t i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_son_num_[i]);
  printf("-, \n");

  printf(".. total_node_hasson per layer:\n   1, ");
  for (size_t i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_node_hasson_[i]);
  printf("\n");

  printf(".. total_sonbuf_num per layer:\n   ");
  for (size_t i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_sonbuf_num_[i]);
  printf("-, \n");

  printf(".. total_sonbuf_allnoson per layer:\n   ");
  for (size_t i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_sonbuf_allnoson_[i]);
  printf("-, \n");

  printf(".. total_node_in_sonbuf_allnoson per layer:\n   ");
  for (size_t i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_node_in_sonbuf_allnoson_[i]);
  printf("-, \n");

  printf(".. total_homo_num per layer:\n   0, ");
  for (size_t i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_homo_num_[i]);
  printf("\n");

  printf(".. son buf allocation number with only 1 son: %d\n", sonbufs_num1_);
  printf(".. son buf allocation number with more than 1 son: %d\n",
         sonbufs_numgt1_);
  printf(".. total lemma node number: %d\n", total_lma_node_num_ + 1);
}
#endif  // ___DO_STATISTICS___

#endif  // ___BUILD_MODEL___
}  // namespace ime_pinyin
