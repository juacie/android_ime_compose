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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "dictstructdef.h"

#ifdef ___BUILD_MODEL___
#include "../include/spellingtable.h"
#endif

#include "../include/spellingtrie.h"

namespace ime_pinyin {
    
    #ifdef ___BUILD_MODEL___
    static const char* zhuyin_exceptions[120][24] = {
        {"A", "AI", "AI1", "AI2", "AI3", "AI4", "AI5", "AN", "AN1", "AN2", "AN3", "AN4", "ANG", "ANG1", "ANG2", "ANG3", "ANG4", "AO", "AO1", "AO2", "AO3", "AO4", "", "", },
        {"AN", "ANG", "ANG1", "ANG2", "ANG3", "ANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"BA", "BAI", "BAI1", "BAI2", "BAI3", "BAI4", "BAN", "BAN1", "BAN3", "BAN4", "BANG", "BANG1", "BANG3", "BANG4", "BAO", "BAO1", "BAO2", "BAO3", "BAO4", "BAO5", "", "", "", "", },
        {"BAN", "BANG", "BANG1", "BANG3", "BANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"BEN", "BENG", "BENG1", "BENG2", "BENG3", "BENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"BIN", "BING", "BING1", "BING3", "BING4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"CA", "CAI", "CAI1", "CAI2", "CAI3", "CAI4", "CAN", "CAN1", "CAN2", "CAN3", "CAN4", "CANG", "CANG1", "CANG2", "CANG3", "CANG4", "CAO", "CAO1", "CAO2", "CAO3", "CAO4", "", "", "", },
        {"CAN", "CANG", "CANG1", "CANG2", "CANG3", "CANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"CE", "CEN", "CEN1", "CEN2", "CENG", "CENG1", "CENG2", "CENG3", "CENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"CEN", "CENG", "CENG1", "CENG2", "CENG3", "CENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ChA", "ChAI", "ChAI1", "ChAI2", "ChAI3", "ChAI4", "ChAN", "ChAN1", "ChAN2", "ChAN3", "ChAN4", "ChANG", "ChANG1", "ChANG2", "ChANG3", "ChANG4", "ChAO", "ChAO1", "ChAO2", "ChAO3", "ChAO4", "", "", "", },
        {"ChAN", "ChANG", "ChANG1", "ChANG2", "ChANG3", "ChANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ChE", "ChEN", "ChEN1", "ChEN2", "ChEN3", "ChEN4", "ChENG", "ChENG1", "ChENG2", "ChENG3", "ChENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ChEN", "ChENG", "ChENG1", "ChENG2", "ChENG3", "ChENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ChUA", "ChUAI", "ChUAI1", "ChUAI2", "ChUAI3", "ChUAI4", "ChUAN", "ChUAN1", "ChUAN2", "ChUAN3", "ChUAN4", "ChUANG", "ChUANG1", "ChUANG2", "ChUANG3", "ChUANG4", "", "", "", "", "", "", "", "", },
        {"ChUAN", "ChUANG", "ChUANG1", "ChUANG2", "ChUANG3", "ChUANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"DA", "DAI", "DAI1", "DAI3", "DAI4", "DAN", "DAN1", "DAN3", "DAN4", "DANG", "DANG1", "DANG3", "DANG4", "DANG5", "DAO", "DAO1", "DAO2", "DAO3", "DAO4", "", "", "", "", "", },
        {"DAN", "DANG", "DANG1", "DANG3", "DANG4", "DANG5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"DE", "DEI", "DEI3", "DENG", "DENG1", "DENG3", "DENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"DIA", "DIAN", "DIAN1", "DIAN3", "DIAN4", "DIAN5", "DIANG", "DIANG1", "DIAO", "DIAO1", "DIAO3", "DIAO4", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"DIAN", "DIANG", "DIANG1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"E", "EI", "EI4", "EN", "EN1", "EN3", "EN4", "EN5", "ENG", "ENG1", "ER", "ER1", "ER2", "ER3", "ER4", "ER5", "", "", "", "", "", "", "", "", },
        {"EN", "ENG", "ENG1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"FA", "FAN", "FAN1", "FAN2", "FAN3", "FAN4", "FANG", "FANG1", "FANG2", "FANG3", "FANG4", "FANG5", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"FAN", "FANG", "FANG1", "FANG2", "FANG3", "FANG4", "FANG5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"FEN", "FENG", "FENG1", "FENG2", "FENG3", "FENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"FO", "FONG", "FONG4", "FOU", "FOU1", "FOU2", "FOU3", "FOU4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"GA", "GAI", "GAI1", "GAI3", "GAI4", "GAN", "GAN1", "GAN3", "GAN4", "GANG", "GANG1", "GANG3", "GANG4", "GAO", "GAO1", "GAO3", "GAO4", "", "", "", "", "", "", "", },
        {"GAN", "GANG", "GANG1", "GANG3", "GANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"GE", "GEI", "GEI2", "GEI3", "GEN", "GEN1", "GEN2", "GEN3", "GEN4", "GENG", "GENG1", "GENG3", "GENG4", "", "", "", "", "", "", "", "", "", "", "", },
        {"GEN", "GENG", "GENG1", "GENG3", "GENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"GUA", "GUAI", "GUAI1", "GUAI3", "GUAI4", "GUAN", "GUAN1", "GUAN3", "GUAN4", "GUANG", "GUANG1", "GUANG3", "GUANG4", "", "", "", "", "", "", "", "", "", "", "", },
        {"GUAN", "GUANG", "GUANG1", "GUANG3", "GUANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"HA", "HAI", "HAI1", "HAI2", "HAI3", "HAI4", "HAI5", "HAN", "HAN1", "HAN2", "HAN3", "HAN4", "HANG", "HANG1", "HANG2", "HANG3", "HANG4", "HAO", "HAO1", "HAO2", "HAO3", "HAO4", "", "", },
        {"HAN", "HANG", "HANG1", "HANG2", "HANG3", "HANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"HE", "HEI", "HEI1", "HEI3", "HEN", "HEN1", "HEN2", "HEN3", "HEN4", "HENG", "HENG1", "HENG2", "HENG4", "", "", "", "", "", "", "", "", "", "", "", },
        {"HEN", "HENG", "HENG1", "HENG2", "HENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"HUA", "HUAI", "HUAI2", "HUAI4", "HUAN", "HUAN1", "HUAN2", "HUAN3", "HUAN4", "HUANG", "HUANG1", "HUANG2", "HUANG3", "HUANG4", "HUANG5", "", "", "", "", "", "", "", "", "", },
        {"HUAN", "HUANG", "HUANG1", "HUANG2", "HUANG3", "HUANG4", "HUANG5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"JI", "JIONG", "JIONG1", "JIONG3", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"JIA", "JIAN", "JIAN1", "JIAN3", "JIAN4", "JIAN5", "JIANG", "JIANG1", "JIANG3", "JIANG4", "JIAO", "JIAO1", "JIAO2", "JIAO3", "JIAO4", "", "", "", "", "", "", "", "", "", },
        {"JIAN", "JIANG", "JIANG1", "JIANG3", "JIANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"JIN", "JING", "JING1", "JING3", "JING4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"KA", "KAI", "KAI1", "KAI3", "KAI4", "KAN", "KAN1", "KAN3", "KAN4", "KAN5", "KANG", "KANG1", "KANG2", "KANG3", "KANG4", "KAO", "KAO1", "KAO3", "KAO4", "", "", "", "", "", },
        {"KAN", "KANG", "KANG1", "KANG2", "KANG3", "KANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"KE", "KEN", "KEN3", "KEN4", "KENG", "KENG1", "KENG3", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"KEN", "KENG", "KENG1", "KENG3", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"KUA", "KUAI", "KUAI1", "KUAI3", "KUAI4", "KUAN", "KUAN1", "KUAN3", "KUANG", "KUANG1", "KUANG2", "KUANG3", "KUANG4", "", "", "", "", "", "", "", "", "", "", "", },
        {"KUAN", "KUANG", "KUANG1", "KUANG2", "KUANG3", "KUANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"LA", "LAI", "LAI2", "LAI4", "LAN", "LAN2", "LAN3", "LAN4", "LANG", "LANG1", "LANG2", "LANG3", "LANG4", "LAO", "LAO1", "LAO2", "LAO3", "LAO4", "", "", "", "", "", "", },
        {"LAN", "LANG", "LANG1", "LANG2", "LANG3", "LANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"LE", "LEI", "LEI1", "LEI2", "LEI3", "LEI4", "LENG", "LENG1", "LENG2", "LENG3", "LENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"LIA", "LIAN", "LIAN2", "LIAN3", "LIAN4", "LIANG", "LIANG2", "LIANG3", "LIANG4", "LIANG5", "LIAO", "LIAO1", "LIAO2", "LIAO3", "LIAO4", "", "", "", "", "", "", "", "", "", },
        {"LIAN", "LIANG", "LIANG2", "LIANG3", "LIANG4", "LIANG5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"LIN", "LING", "LING1", "LING2", "LING3", "LING4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"LO", "LONG", "LONG2", "LONG3", "LONG4", "LONG5", "LOU", "LOU1", "LOU2", "LOU3", "LOU4", "LOU5", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"LU", "LUE", "LUE4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"MA", "MAI", "MAI2", "MAI3", "MAI4", "MAN", "MAN1", "MAN2", "MAN3", "MAN4", "MANG", "MANG2", "MANG3", "MAO", "MAO1", "MAO2", "MAO3", "MAO4", "", "", "", "", "", "", },
        {"MAN", "MANG", "MANG2", "MANG3", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ME", "MEI", "MEI2", "MEI3", "MEI4", "MEI5", "MEN", "MEN1", "MEN2", "MEN3", "MEN4", "MEN5", "MENG", "MENG1", "MENG2", "MENG3", "MENG4", "", "", "", "", "", "", "", },
        {"MEN", "MENG", "MENG1", "MENG2", "MENG3", "MENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"MIN", "MING", "MING2", "MING3", "MING4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"MO", "MOU", "MOU2", "MOU3", "MOU4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"NA", "NAI", "NAI2", "NAI3", "NAI4", "NAI5", "NAN", "NAN1", "NAN2", "NAN3", "NAN4", "NANG", "NANG1", "NANG2", "NANG3", "NANG4", "NANG5", "NAO", "NAO1", "NAO2", "NAO3", "NAO4", "", "", },
        {"NAN", "NANG", "NANG1", "NANG2", "NANG3", "NANG4", "NANG5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"NE", "NEI", "NEI3", "NEI4", "NEN", "NEN4", "NENG", "NENG2", "NENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"NEN", "NENG", "NENG2", "NENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"NIAN", "NIANG", "NIANG2", "NIANG4", "NIANG5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"NIN", "NING", "NING2", "NING3", "NING4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"NU", "NUE", "NUE4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"NUE", "NUEI", "NUEI2", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"O", "OU", "OU1", "OU2", "OU3", "OU4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"PA", "PAI", "PAI1", "PAI2", "PAI3", "PAI4", "PAN", "PAN1", "PAN2", "PAN3", "PAN4", "PANG", "PANG1", "PANG2", "PANG3", "PANG4", "PAO", "PAO1", "PAO2", "PAO3", "PAO4", "", "", "", },
        {"PAN", "PANG", "PANG1", "PANG2", "PANG3", "PANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"PEN", "PENG", "PENG1", "PENG2", "PENG3", "PENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"PIA", "PIAN", "PIAN1", "PIAN2", "PIAN3", "PIAN4", "PIAO", "PIAO1", "PIAO2", "PIAO3", "PIAO4", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"PIN", "PING", "PING1", "PING2", "PING3", "PING4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"PO", "POU", "POU1", "POU2", "POU3", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"QI", "QIONG", "QIONG1", "QIONG2", "QIONG3", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"QIA", "QIAN", "QIAN1", "QIAN2", "QIAN3", "QIAN4", "QIANG", "QIANG1", "QIANG2", "QIANG3", "QIANG4", "QIAO", "QIAO1", "QIAO2", "QIAO3", "QIAO4", "QIAO5", "", "", "", "", "", "", "", },
        {"QIAN", "QIANG", "QIANG1", "QIANG2", "QIANG3", "QIANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"QIN", "QING", "QING1", "QING2", "QING3", "QING4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"RAN", "RANG", "RANG1", "RANG2", "RANG3", "RANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"RE", "REN", "REN2", "REN3", "REN4", "RENG", "RENG1", "RENG2", "RENG3", "RENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"REN", "RENG", "RENG1", "RENG2", "RENG3", "RENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"SA", "SAI", "SAI1", "SAI4", "SAN", "SAN1", "SAN3", "SAN4", "SANG", "SANG1", "SANG3", "SANG4", "SAO", "SAO1", "SAO3", "SAO4", "SAO5", "", "", "", "", "", "", "", },
        {"SAN", "SANG", "SANG1", "SANG3", "SANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"SE", "SEI", "SEI1", "SEN", "SEN1", "SENG", "SENG1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"SEN", "SENG", "SENG1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ShA", "ShAI", "ShAI1", "ShAI3", "ShAI4", "ShAN", "ShAN1", "ShAN3", "ShAN4", "ShANG", "ShANG1", "ShANG3", "ShANG4", "ShANG5", "ShAO", "ShAO1", "ShAO2", "ShAO3", "ShAO4", "", "", "", "", "", },
        {"ShAN", "ShANG", "ShANG1", "ShANG3", "ShANG4", "ShANG5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ShE", "ShEI", "ShEI2", "ShEN", "ShEN1", "ShEN2", "ShEN3", "ShEN4", "ShEN5", "ShENG", "ShENG1", "ShENG2", "ShENG3", "ShENG4", "", "", "", "", "", "", "", "", "", "", },
        {"ShEN", "ShENG", "ShENG1", "ShENG2", "ShENG3", "ShENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ShUA", "ShUAI", "ShUAI1", "ShUAI3", "ShUAI4", "ShUAN", "ShUAN1", "ShUAN4", "ShUANG", "ShUANG1", "ShUANG3", "ShUANG4", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ShUAN", "ShUANG", "ShUANG1", "ShUANG3", "ShUANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"TA", "TAI", "TAI1", "TAI2", "TAI4", "TAI5", "TAN", "TAN1", "TAN2", "TAN3", "TAN4", "TANG", "TANG1", "TANG2", "TANG3", "TANG4", "TAO", "TAO1", "TAO2", "TAO3", "TAO4", "TAO5", "", "", },
        {"TAN", "TANG", "TANG1", "TANG2", "TANG3", "TANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"TE", "TEI", "TEI1", "TENG", "TENG2", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"WA", "WAI", "WAI1", "WAI3", "WAI4", "WAN", "WAN1", "WAN2", "WAN3", "WAN4", "WANG", "WANG1", "WANG2", "WANG3", "WANG4", "", "", "", "", "", "", "", "", "", },
        {"WAN", "WANG", "WANG1", "WANG2", "WANG3", "WANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"WEN", "WENG", "WENG1", "WENG3", "WENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"XI", "XIONG", "XIONG1", "XIONG2", "XIONG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"XIA", "XIAN", "XIAN1", "XIAN2", "XIAN3", "XIAN4", "XIANG", "XIANG1", "XIANG2", "XIANG3", "XIANG4", "XIAO", "XIAO1", "XIAO2", "XIAO3", "XIAO4", "", "", "", "", "", "", "", "", },
        {"XIAN", "XIANG", "XIANG1", "XIANG2", "XIANG3", "XIANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"XIN", "XING", "XING1", "XING2", "XING3", "XING4", "XING5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"Y", "YONG", "YONG1", "YONG2", "YONG3", "YONG4", "YU", "YU1", "YU2", "YU3", "YU4", "YUAN", "YUAN1", "YUAN2", "YUAN3", "YUAN4", "YUE", "YUE1", "YUE4", "YUN", "YUN1", "YUN2", "YUN3", "YUN4", },
        {"YA", "YAI", "YAI2", "YAN", "YAN1", "YAN2", "YAN3", "YAN4", "YANG", "YANG1", "YANG2", "YANG3", "YANG4", "YANG5", "YAO", "YAO1", "YAO2", "YAO3", "YAO4", "", "", "", "", "", },
        {"YAN", "YANG", "YANG1", "YANG2", "YANG3", "YANG4", "YANG5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"YIN", "YING", "YING1", "YING2", "YING3", "YING4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"YO", "YONG", "YONG1", "YONG2", "YONG3", "YONG4", "YOU", "YOU1", "YOU2", "YOU3", "YOU4", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ZA", "ZAI", "ZAI1", "ZAI3", "ZAI4", "ZAN", "ZAN1", "ZAN2", "ZAN3", "ZAN4", "ZANG", "ZANG1", "ZANG3", "ZANG4", "ZAO", "ZAO1", "ZAO2", "ZAO3", "ZAO4", "", "", "", "", "", },
        {"ZAN", "ZANG", "ZANG1", "ZANG3", "ZANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ZE", "ZEI", "ZEI2", "ZEN", "ZEN1", "ZEN3", "ZEN4", "ZENG", "ZENG1", "ZENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ZEN", "ZENG", "ZENG1", "ZENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ZhA", "ZhAI", "ZhAI1", "ZhAI2", "ZhAI3", "ZhAI4", "ZhAN", "ZhAN1", "ZhAN3", "ZhAN4", "ZhANG", "ZhANG1", "ZhANG3", "ZhANG4", "ZhAO", "ZhAO1", "ZhAO2", "ZhAO3", "ZhAO4", "", "", "", "", "", },
        {"ZhAN", "ZhANG", "ZhANG1", "ZhANG3", "ZhANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ZhE", "ZhEI", "ZhEI4", "ZhEN", "ZhEN1", "ZhEN3", "ZhEN4", "ZhENG", "ZhENG1", "ZhENG3", "ZhENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ZhEN", "ZhENG", "ZhENG1", "ZhENG3", "ZhENG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
        {"ZhUA", "ZhUAI", "ZhUAI1", "ZhUAI3", "ZhUAI4", "ZhUAN", "ZhUAN1", "ZhUAN3", "ZhUAN4", "ZhUANG", "ZhUANG1", "ZhUANG3", "ZhUANG4", "", "", "", "", "", "", "", "", "", "", "", },
        {"ZhUAN", "ZhUANG", "ZhUANG1", "ZhUANG3", "ZhUANG4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", },
    };
    #endif

SpellingTrie* SpellingTrie::instance_ = NULL;

// z/c/s is for Zh/Ch/Sh
const char SpellingTrie::kHalfId2Sc_[kFullSplIdStart + 1] =
    "0ABCcDEFGHIJKLMNOPQRSsTUVWXYZz";

// Bit 0 : is it a Shengmu char?
// Bit 1 : is it a Yunmu char? (one char is a Yunmu)
// Bit 2 : is it enabled in ShouZiMu(first char) mode?
unsigned char SpellingTrie::char_flags_[] = {
  // a    b      c     d     e     f     g
  0x02, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01,
  // h    i     j      k     l     m    n
  0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
  // o    p     q      r     s     t
  0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
  // u    v     w      x     y     z
  0x00, 0x00, 0x01, 0x01, 0x01, 0x01
};

int compare_spl(const void* p1, const void* p2) {
  return strcmp((const char*)(p1), (const char*)(p2));
}

    int compare_spl_homo(const void* p1, const void* p2) {
        SpellingHomo * h1 = (SpellingHomo*)p1;
        SpellingHomo * h2 = (SpellingHomo*)p2;
        
        if (h1->spelling_idx > h2->spelling_idx) {
            return 1;
        } else if (h1->spelling_idx < h2->spelling_idx) {
            return -1;
        }
        
        if (h1->level > h2->level) {
            return -1;
        } else if (h1->level < h2->level) {
            return 1;
        }
        
        return 0;
    }
    
SpellingTrie::SpellingTrie() {
  spelling_buf_ = NULL;
  spelling_size_ = 0;
  spelling_num_ = 0;
  spl_ym_ids_ = NULL;
  splstr_queried_ = NULL;
  splstr16_queried_ = NULL;
  root_ = NULL;
  dumb_node_ = NULL;
  splitter_node_ = NULL;
  instance_ = NULL;
  ym_buf_ = NULL;
  f2h_ = NULL;
    homo_buf_ = NULL;
    homo_buf_num_ = 0;
    homo_spl_exception_column_size_ = 0;
    homo_spl_exception_row_size_ = 0;
    homo_spl_exception_ids_ = NULL;

  szm_enable_shm(true);
  szm_enable_ym(true);

#ifdef ___BUILD_MODEL___
  node_num_ = 0;
#endif
}

SpellingTrie::~SpellingTrie() {
  if (NULL != spelling_buf_)
    delete [] spelling_buf_;

  if (NULL != splstr_queried_)
    delete [] splstr_queried_;

  if (NULL != splstr16_queried_)
    delete [] splstr16_queried_;

  if (NULL != spl_ym_ids_)
    delete [] spl_ym_ids_;

  if (NULL != root_) {
    free_son_trie(root_);
    delete root_;
  }

  if (NULL != dumb_node_) {
    delete dumb_node_;
  }

  if (NULL != splitter_node_) {
    delete splitter_node_;
  }

  if (NULL != instance_) {
      if (instance_ != this)
          delete instance_;
    instance_ = NULL;
  }

  if (NULL != ym_buf_)
    delete [] ym_buf_;

  if (NULL != f2h_)
    delete [] f2h_;
    
    if (NULL != homo_buf_)
        delete [] homo_buf_;
    
    if (NULL != homo_spl_exception_ids_)
        delete [] homo_spl_exception_ids_;
}

bool SpellingTrie::if_valid_id_update(uint16 *splid) const {
  if (NULL == splid || 0 == *splid)
    return false;

  if (*splid >= kFullSplIdStart)
    return true;
  if (*splid < kFullSplIdStart) {
    char ch = kHalfId2Sc_[*splid];
    if (ch > 'Z') {
      return true;
    } else {
      if (szm_is_enabled(ch)) {
        return true;
      } else if (is_yunmu_char(ch)) {
        assert(h2f_num_[*splid] > 0);
        *splid = h2f_start_[*splid];
        return true;
      }
    }
  }
  return false;
}

bool SpellingTrie::is_half_id(uint16 splid) const {
  if (0 == splid || splid >= kFullSplIdStart)
    return false;

  return true;
}

bool SpellingTrie::is_full_id(uint16 splid) const {
  if (splid < kFullSplIdStart || splid >= kFullSplIdStart + spelling_num_)
    return false;
  return true;
}

bool SpellingTrie::half_full_compatible(uint16 half_id, uint16 full_id) const {
  uint16 half_fr_full = full_to_half(full_id);

  if (half_fr_full == half_id)
    return true;

  // &~0x20 is used to conver the char to upper case.
  // So that Zh/Ch/Sh(whose char is z/c/s) can be matched with Z/C/S.
  char ch_f = (kHalfId2Sc_[half_fr_full] & (~0x20));
  char ch_h = kHalfId2Sc_[half_id];
  if (ch_f == ch_h)
    return true;

  return false;
}

bool SpellingTrie::is_half_id_yunmu(uint16 splid) const {
  if (0 == splid || splid >= kFullSplIdStart)
    return false;

  char ch = kHalfId2Sc_[splid];
  // If ch >= 'a', that means the half id is one of Zh/Ch/Sh
  if (ch >= 'a') {
    return false;
  }

  return char_flags_[ch - 'A'] & kHalfIdYunmuMask;
}

bool SpellingTrie::is_shengmu_char(char ch) const {
  return char_flags_[ch - 'A'] & kHalfIdShengmuMask;
}

bool SpellingTrie::is_yunmu_char(char ch) const {
  return char_flags_[ch - 'A'] & kHalfIdYunmuMask;
}

bool SpellingTrie::is_szm_char(char ch) const {
  return is_shengmu_char(ch) || is_yunmu_char(ch);
}

bool SpellingTrie::szm_is_enabled(char ch) const {
  return char_flags_[ch - 'A'] & kHalfIdSzmMask;
}

void SpellingTrie::szm_enable_shm(bool enable) {
  if (enable) {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      if (is_shengmu_char(ch))
        char_flags_[ch - 'A'] = char_flags_[ch - 'A'] | kHalfIdSzmMask;
    }
  } else {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      if (is_shengmu_char(ch))
        char_flags_[ch - 'A'] = char_flags_[ch - 'A'] & (kHalfIdSzmMask ^ 0xff);
    }
  }
}

void SpellingTrie::szm_enable_ym(bool enable) {
  if (enable) {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      if (is_yunmu_char(ch))
        char_flags_[ch - 'A'] = char_flags_[ch - 'A'] | kHalfIdSzmMask;
    }
  } else {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      if (is_yunmu_char(ch))
        char_flags_[ch - 'A'] = char_flags_[ch - 'A'] & (kHalfIdSzmMask ^ 0xff);
    }
  }
}

bool SpellingTrie::is_szm_enabled(char ch) const {
  return char_flags_[ch - 'A'] & kHalfIdSzmMask;
}

const SpellingTrie* SpellingTrie::get_cpinstance() {
  return &get_instance();
}

SpellingTrie& SpellingTrie::get_instance() {
  if (NULL == instance_)
    instance_ = new SpellingTrie();

  return *instance_;
}

uint16 SpellingTrie::half2full_num(uint16 half_id) const {
  if (NULL == root_ || half_id >= kFullSplIdStart)
    return 0;
  return h2f_num_[half_id];
}

uint16 SpellingTrie::half_to_full(uint16 half_id, uint16 *spl_id_start) const {
  if (NULL == spl_id_start || NULL == root_ || half_id >= kFullSplIdStart)
    return 0;

  *spl_id_start = h2f_start_[half_id];
  return h2f_num_[half_id];
}

uint16 SpellingTrie::full_to_half(uint16 full_id) const {
  if (NULL == root_ || full_id < kFullSplIdStart ||
      full_id > spelling_num_ + kFullSplIdStart)
    return 0;

  return f2h_[full_id - kFullSplIdStart];
}
    
    // Get spelling homo range by spelling idx
    uint16 SpellingTrie::get_spl_homo(uint16 spl_idx) const {
        for (int i = 0; i < homo_buf_num_; i++) {
            if (homo_buf_[i].spelling_idx == spl_idx)
                return homo_buf_[i].num;
        }
        return 1;
    }
    
void SpellingTrie::free_son_trie(SpellingNode* node) {
  if (NULL == node)
    return;

  for (size_t pos = 0; pos < node->num_of_son; pos++) {
    free_son_trie(node->first_son + pos);
  }

  if (NULL != node->first_son)
    delete [] node->first_son;
}

bool SpellingTrie::construct(const char* spelling_arr, size_t item_size,
                             size_t item_num, float score_amplifier,
                             unsigned char average_score) {
  if (spelling_arr == NULL)
    return false;

  memset(h2f_start_, 0, sizeof(uint16) * kFullSplIdStart);
  memset(h2f_num_, 0, sizeof(uint16) * kFullSplIdStart);

  // If the arr is the same as the buf, means this function is called by
  // load_table(), the table data are ready; otherwise the array should be
  // saved.
  if (spelling_arr != spelling_buf_) {
    if (NULL != spelling_buf_)
      delete [] spelling_buf_;
    spelling_buf_ = new char[item_size * item_num];
    if (NULL == spelling_buf_)
      return false;
    memcpy(spelling_buf_, spelling_arr, sizeof(char) * item_size * item_num);
  }

  spelling_size_ = item_size;
  spelling_num_ = item_num;

  score_amplifier_ = score_amplifier;
  average_score_ = average_score;

  if (NULL != splstr_queried_)
    delete [] splstr_queried_;
  splstr_queried_ = new char[spelling_size_];
  if (NULL == splstr_queried_)
    return false;

  if (NULL != splstr16_queried_)
    delete [] splstr16_queried_;
  splstr16_queried_ = new char16[spelling_size_];
  if (NULL == splstr16_queried_)
    return false;

  // First, sort the buf to ensure they are in ascendant order
  qsort(spelling_buf_, spelling_num_, spelling_size_, compare_spl);

#ifdef ___BUILD_MODEL___
  node_num_ = 1;
    
    // Init spelling homo array for building dict
    homo_buf_ = new SpellingHomo[kMaxSpellingHomo];
    memset(homo_buf_, 0, sizeof(SpellingHomo) * kMaxSpellingHomo);
    
    homo_spl_exception_column_size_ = sizeof(zhuyin_exceptions)/sizeof(zhuyin_exceptions[0]);
    homo_spl_exception_row_size_ = sizeof(zhuyin_exceptions[0])/sizeof(zhuyin_exceptions[0][0]);    homo_spl_exception_ids_ = new uint16[homo_spl_exception_column_size_*homo_spl_exception_row_size_];
    memset(homo_spl_exception_ids_, 0, sizeof(uint16) * homo_spl_exception_column_size_ * homo_spl_exception_row_size_);

#endif
    if (NULL!=root_)
        delete root_;
    
    if (NULL!=dumb_node_)
        delete dumb_node_;
    
    if (NULL!=splitter_node_)
        delete splitter_node_;
    
  root_ = new SpellingNode();
  memset(root_, 0, sizeof(SpellingNode));

  dumb_node_ = new SpellingNode();
  memset(dumb_node_, 0, sizeof(SpellingNode));
  dumb_node_->score = average_score_;

  splitter_node_ = new SpellingNode();
  memset(splitter_node_, 0, sizeof(SpellingNode));
  splitter_node_->score = average_score_;

  memset(level1_sons_, 0, sizeof(SpellingNode*) * kValidSplCharNum);

  root_->first_son = construct_spellings_subset(0, spelling_num_, 0, root_);
    
#ifdef ___BUILD_MODEL___
    qsort(homo_buf_, homo_buf_num_, sizeof(SpellingHomo), compare_spl_homo);
    build_homo_exception_table();
#endif
    
  // Root's score should be cleared.
  root_->score = 0;

  if (NULL == root_->first_son)
    return false;

  h2f_start_[0] = h2f_num_[0] = 0;

  if (!build_f2h())
    return false;

#ifdef ___BUILD_MODEL___
  if (kPrintDebug0) {
    printf("---SpellingTrie Nodes: %d\n", node_num_);
  }
  return build_ym_info();
#else
  return true;
#endif
}

#ifdef ___BUILD_MODEL___
const char* SpellingTrie::get_ym_str(const char *spl_str) {
  bool start_ZCS = false;
  if (is_shengmu_char(*spl_str)) {
    if ('Z' == *spl_str || 'C' == *spl_str || 'S' == *spl_str)
      start_ZCS = true;
    spl_str += 1;
    if (start_ZCS && 'h' == *spl_str)
      spl_str += 1;
  }
  return spl_str;
}

bool SpellingTrie::build_ym_info() {
  bool sucess;
  SpellingTable *spl_table = new SpellingTable();

  sucess = spl_table->init_table(kMaxPinyinSize - 1, 1024 * 2 * kMaxYmNum, false);
  assert(sucess);

  for (uint16 pos = 0; pos < spelling_num_; pos++) {
    const char *spl_str = spelling_buf_ + spelling_size_ * pos;
    spl_str = get_ym_str(spl_str);
    if ('\0' != spl_str[0]) {
      sucess = spl_table->put_spelling(spl_str, 0);
      assert(sucess);
    }
  }

  size_t ym_item_size;  // '\0' is included
  size_t ym_num;
  const char* ym_buf;
  ym_buf = spl_table->arrange(&ym_item_size, &ym_num);

  if (NULL != ym_buf_)
    delete [] ym_buf_;
  ym_buf_ = new char[ym_item_size * ym_num];
  if (NULL == ym_buf_) {
    delete spl_table;
    return false;
  }

  memcpy(ym_buf_, ym_buf, sizeof(char) * ym_item_size * ym_num);
  ym_size_ = ym_item_size;
  ym_num_ = ym_num;

  delete spl_table;

  // Generate the maping from the spelling ids to the Yunmu ids.
  if (spl_ym_ids_)
    delete spl_ym_ids_;
  spl_ym_ids_ = new uint16[spelling_num_ + kFullSplIdStart];
  if (NULL == spl_ym_ids_)
    return false;

  memset(spl_ym_ids_, 0, sizeof(uint16) * (spelling_num_ + kFullSplIdStart));

  for (uint16 id = 1; id < spelling_num_ + kFullSplIdStart; id++) {
    const char *str = get_spelling_str(id);

    str = get_ym_str(str);
    if ('\0' != str[0]) {
      uint16 ym_id = get_ym_id(str);
      spl_ym_ids_[id] = ym_id;
      assert(ym_id > 0);
    } else {
      spl_ym_ids_[id] = 0;
    }
  }
  return true;
}
    
    void SpellingTrie::add_homo_search_item (size_t level, int spelling_idx, int num) {
        SpellingHomo * current = homo_buf_ + homo_buf_num_;
        current->level = level;
        current->spelling_idx = spelling_idx;
        current->num = num;
        homo_buf_num_++;
        assert(homo_buf_num_ < kMaxSpellingHomo);
    }
    
    void SpellingTrie::try_add_homo_item (size_t level, const char char_for_node,
                                          size_t item_start_next, size_t i, SpellingNode *node_current) {
        // Update spelling homo array
        if (level >= 1 && char_for_node >= 'A' && char_for_node <= 'Z') {
            int spelling_idx = static_cast<uint16>(char_for_node - 'A' + 1);
            if ((i - item_start_next ) > 1) {
                int spelling_idx = node_current->spelling_idx;
                
                if (spelling_idx == 0)
                    spelling_idx = kFullSplIdStart + item_start_next;
                
                add_homo_search_item (level, spelling_idx, i - item_start_next);
            }
        }
    }

    /* internal use only */
    uint16 SpellingTrie::get_spl_id(const char* splstr) {
        if (strlen(splstr) == 0) {
            return 0;
        }
        
        if (strcmp(splstr, "Ch") == 0) {
            return 'C' - 'A' + 1 + 1;
        }
        if (strcmp(splstr, "Sh") == 0) {
            return 'S' - 'A' + 1 + 2;
        }
        if (strcmp(splstr, "Zh") == 0) {
            return 'Z' - 'A' + 1 + 3;
        }
        
        if (strlen(splstr) == 1) {
            /*
             1~29
             if (splid > 'C' - 'A' + 1)
             splid--;
             if (splid > 'S' - 'A' + 1)
             splid--;
             splstr_queried_[0] = 'A' + splid - 1;
             */
            uint16 splid = (uint16)splstr[0] + 1 - 'A';
            if (splid >= 'S' - 'A' + 1)
                splid++;
            if (splid >= 'C' - 'A' + 1)
                splid++;
            //const char* str = SpellingTrie::get_instance().get_spelling_str(splid);
            //assert(strcmp(str, splstr) == 0);
            return splid;
        } else {
            for (uint16 i = 0; i < spelling_num_; i++) {
                if (strcmp(splstr, spelling_buf_+i*spelling_size_) == 0) {
                    return i + kFullSplIdStart;
                }
            }
            return 0;
        }
    }
    
    void SpellingTrie::build_homo_exception_table() {
        for (size_t i = 0; i < homo_spl_exception_column_size_; i+=1) {
            const char* pinyin = zhuyin_exceptions[i][0];
            uint16 splid = get_spl_id(pinyin);
            if (splid == 0) {
                printf("warning: homo_exception_table find pinyin %s with splid 0\n", pinyin);
                continue;
            }
            
            homo_spl_exception_ids_[i*homo_spl_exception_row_size_] = splid;
            
            for (size_t j = 1; j < homo_spl_exception_row_size_; j++) {
                const char* child_pinyin = zhuyin_exceptions[i][j];
                uint16 child_splid = get_spl_id(child_pinyin);
                if (splid == 0) {
                    printf("warning: homo_exception_table find child pinyin %s with splid 0\n", child_pinyin);
                    continue;
                }
                
                homo_spl_exception_ids_[i*homo_spl_exception_row_size_ + j] = child_splid;
            }
        }
    }
    
#endif

SpellingNode* SpellingTrie::construct_spellings_subset(
    size_t item_start, size_t item_end, size_t level, SpellingNode* parent) {
  if (level >= spelling_size_ || item_end <= item_start || NULL == parent)
    return NULL;

  SpellingNode *first_son = NULL;
  uint16 num_of_son = 0;
  unsigned char min_son_score = 255;

  const char *spelling_last_start = spelling_buf_ + spelling_size_ * item_start;
  char char_for_node = spelling_last_start[level];
  assert(char_for_node >= 'A' && char_for_node <= 'Z' ||
         'h' == char_for_node || char_for_node >='1' && char_for_node <= '5');

  // Scan the array to find how many sons
  for (size_t i = item_start + 1; i < item_end; i++) {
    const char *spelling_current = spelling_buf_ + spelling_size_ * i;
    char char_current = spelling_current[level];
    if (char_current != char_for_node) {
      num_of_son++;
      char_for_node = char_current;
    }
  }
  num_of_son++;

  // Allocate memory
#ifdef ___BUILD_MODEL___
  node_num_ += num_of_son;
#endif
  first_son = new SpellingNode[num_of_son];
  memset(first_son, 0, sizeof(SpellingNode)*num_of_son);

  // Now begin construct tree
  size_t son_pos = 0;

  spelling_last_start = spelling_buf_ + spelling_size_ * item_start;
  char_for_node = spelling_last_start[level];

  bool spelling_endable = true;
  if (spelling_last_start[level + 1] != '\0')
    spelling_endable = false;

  size_t item_start_next = item_start;

  for (size_t i = item_start + 1; i < item_end; i++) {
    const char *spelling_current = spelling_buf_ + spelling_size_ * i;
    char char_current = spelling_current[level];
    assert(is_valid_spl_char(char_current));

    if (char_current != char_for_node) {
      // Construct a node
      SpellingNode *node_current = first_son + son_pos;
      node_current->char_this_node = char_for_node;

      // For quick search in the first level
      if (0 == level)
        level1_sons_[char_for_node - 'A'] = node_current;

      if (spelling_endable) {
        node_current->spelling_idx = kFullSplIdStart + item_start_next;
      }

      if (spelling_last_start[level + 1] != '\0' || i - item_start_next > 1) {
        size_t real_start = item_start_next;
        if (spelling_last_start[level + 1] == '\0')
          real_start++;

        node_current->first_son =
            construct_spellings_subset(real_start, i, level + 1,
                                       node_current);

        if (real_start == item_start_next + 1) {
          uint16 score_this = static_cast<unsigned char>(
              spelling_last_start[spelling_size_ - 1]);
          if (score_this < node_current->score)
            node_current->score = score_this;
        }
      } else {
        node_current->first_son = NULL;
        node_current->score = static_cast<unsigned char>(
            spelling_last_start[spelling_size_ - 1]);
      }

      if (node_current->score < min_son_score)
        min_son_score = node_current->score;

      bool is_half = false;
      if (level == 0 && is_szm_char(char_for_node)) {
        node_current->spelling_idx =
          static_cast<uint16>(char_for_node - 'A' + 1);

        if (char_for_node > 'C')
          node_current->spelling_idx++;
        if (char_for_node > 'S')
          node_current->spelling_idx++;

        h2f_num_[node_current->spelling_idx] = i - item_start_next;
        is_half = true;
      } else if (level == 1 && char_for_node == 'h') {
        char ch_level0 = spelling_last_start[0];
        uint16 part_id = 0;
        if (ch_level0 == 'C')
          part_id = 'C' - 'A' + 1 + 1;
        else if (ch_level0 == 'S')
          part_id = 'S' - 'A' + 1 + 2;
        else if (ch_level0 == 'Z')
          part_id = 'Z' - 'A' + 1 + 3;
        if (0 != part_id) {
          node_current->spelling_idx = part_id;
          h2f_num_[node_current->spelling_idx] = i - item_start_next;
          is_half = true;
        }
      }

#ifdef ___BUILD_MODEL___
        try_add_homo_item(level, char_for_node, item_start_next, i, node_current);
#endif
        
      if (is_half) {
        if (h2f_num_[node_current->spelling_idx] > 0)
          h2f_start_[node_current->spelling_idx] =
            item_start_next + kFullSplIdStart;
        else
          h2f_start_[node_current->spelling_idx] = 0;
      }

      // for next sibling
      spelling_last_start = spelling_current;
      char_for_node = char_current;
      item_start_next = i;
      spelling_endable = true;
      if (spelling_current[level + 1] != '\0')
        spelling_endable = false;

      son_pos++;
    }
  }

  // the last one
  SpellingNode *node_current = first_son + son_pos;
  node_current->char_this_node = char_for_node;

  // For quick search in the first level
  if (0 == level)
    level1_sons_[char_for_node - 'A'] = node_current;

  if (spelling_endable) {
    node_current->spelling_idx = kFullSplIdStart + item_start_next;
  }

  if (spelling_last_start[level + 1] != '\0' ||
      item_end - item_start_next > 1) {
    size_t real_start = item_start_next;
    if (spelling_last_start[level + 1] == '\0')
      real_start++;

    node_current->first_son =
        construct_spellings_subset(real_start, item_end, level + 1,
                                   node_current);

    if (real_start == item_start_next + 1) {
      uint16 score_this = static_cast<unsigned char>(
          spelling_last_start[spelling_size_ - 1]);
      if (score_this < node_current->score)
        node_current->score = score_this;
    }
  } else {
    node_current->first_son = NULL;
    node_current->score = static_cast<unsigned char>(
        spelling_last_start[spelling_size_ - 1]);
  }

  if (node_current->score < min_son_score)
    min_son_score = node_current->score;

  assert(son_pos + 1 == num_of_son);

  bool is_half = false;
  if (level == 0 && szm_is_enabled(char_for_node)) {
    node_current->spelling_idx = static_cast<uint16>(char_for_node - 'A' + 1);

    if (char_for_node > 'C')
      node_current->spelling_idx++;
    if (char_for_node > 'S')
      node_current->spelling_idx++;

    h2f_num_[node_current->spelling_idx] = item_end - item_start_next;
    is_half = true;
  } else if (level == 1 && char_for_node == 'h') {
    char ch_level0 = spelling_last_start[0];
    uint16 part_id = 0;
    if (ch_level0 == 'C')
      part_id = 'C' - 'A' + 1 + 1;
    else if (ch_level0 == 'S')
      part_id = 'S' - 'A' + 1 + 2;
    else if (ch_level0 == 'Z')
      part_id = 'Z' - 'A' + 1 + 3;
    if (0 != part_id) {
      node_current->spelling_idx = part_id;
      h2f_num_[node_current->spelling_idx] = item_end - item_start_next;
      is_half = true;
    }
  }
    
#ifdef ___BUILD_MODEL___
    try_add_homo_item(level, char_for_node, item_start_next, item_end, node_current);
#endif
    
  if (is_half) {
    if (h2f_num_[node_current->spelling_idx] > 0)
      h2f_start_[node_current->spelling_idx] =
        item_start_next + kFullSplIdStart;
    else
      h2f_start_[node_current->spelling_idx] = 0;
  }

  parent->num_of_son = num_of_son;
  parent->score = min_son_score;
  return first_son;
}

bool SpellingTrie::save_spl_trie(FILE *fp) {
  if (NULL == fp || NULL == spelling_buf_)
    return false;

  if (fwrite(&spelling_size_, sizeof(uint32), 1, fp) != 1)
    return false;

  if (fwrite(&spelling_num_, sizeof(uint32), 1, fp) != 1)
    return false;

  if (fwrite(&score_amplifier_, sizeof(float), 1, fp) != 1)
    return false;

  if (fwrite(&average_score_, sizeof(unsigned char), 1, fp) != 1)
    return false;

  if (fwrite(spelling_buf_, sizeof(char) * spelling_size_,
             spelling_num_, fp) != spelling_num_)
    return false;
    
    // Store spelling homo array
    if (fwrite(&homo_buf_num_, sizeof(uint32), 1, fp) != 1)
        return false;
    
    if (fwrite(homo_buf_, sizeof(SpellingHomo),
               homo_buf_num_, fp) != homo_buf_num_)
        return false;
    
    if (fwrite(&homo_spl_exception_column_size_, sizeof(uint32), 1, fp) != 1)
        return false;
    if (fwrite(&homo_spl_exception_row_size_, sizeof(uint32), 1, fp) != 1)
        return false;
    if (fwrite(homo_spl_exception_ids_, sizeof(uint16),
               homo_spl_exception_column_size_*homo_spl_exception_row_size_, fp) != homo_spl_exception_column_size_*homo_spl_exception_row_size_)
        return false;
    
  return true;
}

const unsigned char* SpellingTrie::load_spl_trie(const unsigned char *mem) {
    if (NULL == mem)
        return NULL;
    
    spelling_size_ = *(uint32*)mem;
    mem += sizeof(uint32);
    spelling_num_ = *(uint32*)mem;
    mem += sizeof(uint32);
    score_amplifier_ = *(float*)mem;
    mem += sizeof(float);
    average_score_ = *(unsigned char*)mem;
    mem += sizeof(unsigned char);
    
    if (NULL != spelling_buf_)
        delete [] spelling_buf_;
    
    spelling_buf_ = new char[spelling_size_ * spelling_num_];
    if (NULL == spelling_buf_)
        return NULL;
    
    memcpy(spelling_buf_, mem, spelling_size_ * spelling_num_);
    mem += spelling_size_ * spelling_num_;
    
    if (!construct(spelling_buf_, spelling_size_, spelling_num_, score_amplifier_, average_score_))
        return NULL;
    
    return mem;
}

bool SpellingTrie::load_spl_trie(FILE *fp) {
  if (NULL == fp)
    return false;

  if (fread(&spelling_size_, sizeof(uint32), 1, fp) != 1)
    return false;

  if (fread(&spelling_num_, sizeof(uint32), 1, fp) != 1)
    return false;

  if (fread(&score_amplifier_, sizeof(float), 1, fp) != 1)
    return false;

  if (fread(&average_score_, sizeof(unsigned char), 1, fp) != 1)
    return false;

  if (NULL != spelling_buf_)
    delete [] spelling_buf_;

  spelling_buf_ = new char[spelling_size_ * spelling_num_];
  if (NULL == spelling_buf_)
    return false;

  if (fread(spelling_buf_, sizeof(char) * spelling_size_,
            spelling_num_, fp) != spelling_num_)
    return false;

    if (spl_homo_enabled) {
        // Restore spelling homo array
        if (fread(&homo_buf_num_, sizeof(uint32), 1, fp) != 1)
            return false;
        
        if (NULL != homo_buf_)
            delete [] homo_buf_;
        
        homo_buf_ = new SpellingHomo[homo_buf_num_];
        if (NULL == homo_buf_)
            return false;
        
        if (fread(homo_buf_, sizeof(SpellingHomo),
                  homo_buf_num_, fp) != homo_buf_num_)
            return false;
        
        if (fread(&homo_spl_exception_column_size_, sizeof(uint32), 1, fp) != 1)
            return false;
        if (fread(&homo_spl_exception_row_size_, sizeof(uint32), 1, fp) != 1)
            return false;
        
        homo_spl_exception_ids_ = new uint16[homo_spl_exception_column_size_*homo_spl_exception_row_size_];
        if (homo_spl_exception_ids_ == NULL)
            return false;
        
        if (fread(homo_spl_exception_ids_, sizeof(uint16),
                  homo_spl_exception_column_size_*homo_spl_exception_row_size_, fp) != homo_spl_exception_column_size_*homo_spl_exception_row_size_)
            return false;
    }
    
  return construct(spelling_buf_, spelling_size_, spelling_num_,
                   score_amplifier_, average_score_);
}

bool SpellingTrie::build_f2h() {
  if (NULL != f2h_)
    delete [] f2h_;
  f2h_ = new uint16[spelling_num_];
  if (NULL == f2h_)
    return false;

  for (uint16 hid = 0; hid < kFullSplIdStart; hid++) {
    for (uint16 fid = h2f_start_[hid];
         fid < h2f_start_[hid] + h2f_num_[hid]; fid++)
      f2h_[fid - kFullSplIdStart] = hid;
  }

  return true;
}

size_t SpellingTrie::get_spelling_num() {
  return spelling_num_;
}

uint16 SpellingTrie::get_ym_id(const char *ym_str) {
  if (NULL == ym_str || NULL == ym_buf_)
    return 0;

  for (uint32 pos = 0; pos < ym_num_; pos++)
    if (strcmp(ym_buf_ + ym_size_ * pos, ym_str) == 0)
      return pos + 1;

  return 0;
}

const char* SpellingTrie::get_spelling_str(uint16 splid) {
  splstr_queried_[0] = '\0';

  if (splid >= kFullSplIdStart) {
    splid -= kFullSplIdStart;
    snprintf(splstr_queried_, spelling_size_, "%s",
             spelling_buf_ + splid * spelling_size_);
  } else {
    if (splid == 'C' - 'A' + 1 + 1) {
      snprintf(splstr_queried_, spelling_size_, "%s", "Ch");
    } else if (splid == 'S' - 'A' + 1 + 2) {
      snprintf(splstr_queried_, spelling_size_, "%s", "Sh");
    } else if (splid == 'Z' - 'A' + 1 + 3) {
      snprintf(splstr_queried_, spelling_size_, "%s", "Zh");
    } else {
      if (splid > 'C' - 'A' + 1)
        splid--;
      if (splid > 'S' - 'A' + 1)
        splid--;
      splstr_queried_[0] = 'A' + splid - 1;
      splstr_queried_[1] = '\0';
    }
  }
  return splstr_queried_;
}

const char16* SpellingTrie::get_spelling_str16(uint16 splid) {
  splstr16_queried_[0] = '\0';

  if (splid >= kFullSplIdStart) {
    splid -= kFullSplIdStart;
    for (size_t pos = 0; pos < spelling_size_; pos++) {
      splstr16_queried_[pos] = static_cast<char16>
          (spelling_buf_[splid * spelling_size_ + pos]);
    }
  } else {
    if (splid == 'C' - 'A' + 1 + 1) {
      splstr16_queried_[0] = static_cast<char16>('C');
      splstr16_queried_[1] = static_cast<char16>('h');
      splstr16_queried_[2] = static_cast<char16>('\0');
    } else if (splid == 'S' - 'A' + 1 + 2) {
      splstr16_queried_[0] = static_cast<char16>('S');
      splstr16_queried_[1] = static_cast<char16>('h');
      splstr16_queried_[2] = static_cast<char16>('\0');
    } else if (splid == 'Z' - 'A' + 1 + 3) {
      splstr16_queried_[0] = static_cast<char16>('Z');
      splstr16_queried_[1] = static_cast<char16>('h');
      splstr16_queried_[2] = static_cast<char16>('\0');
    } else {
      if (splid > 'C' - 'A' + 1)
        splid--;
      if (splid > 'S' - 'A' + 1)
        splid--;
      splstr16_queried_[0] = 'A' + splid - 1;
      splstr16_queried_[1] = '\0';
    }
  }
  return splstr16_queried_;
}

size_t SpellingTrie::get_spelling_str16(uint16 splid, char16 *splstr16,
                                        size_t splstr16_len) {
  if (NULL == splstr16 || splstr16_len < kMaxPinyinSize + 1) return 0;

  if (splid >= kFullSplIdStart) {
    splid -= kFullSplIdStart;
    for (size_t pos = 0; pos <= kMaxPinyinSize; pos++) {
      splstr16[pos] = static_cast<char16>
          (spelling_buf_[splid * spelling_size_ + pos]);
      if (static_cast<char16>('\0') == splstr16[pos]) {
        return pos;
      }
    }
  } else {
    if (splid == 'C' - 'A' + 1 + 1) {
      splstr16[0] = static_cast<char16>('C');
      splstr16[1] = static_cast<char16>('h');
      splstr16[2] = static_cast<char16>('\0');
      return 2;
    } else if (splid == 'S' - 'A' + 1 + 2) {
      splstr16[0] = static_cast<char16>('S');
      splstr16[1] = static_cast<char16>('h');
      splstr16[2] = static_cast<char16>('\0');
      return 2;
    } else if (splid == 'Z' - 'A' + 1 + 3) {
      splstr16[0] = static_cast<char16>('Z');
      splstr16[1] = static_cast<char16>('h');
      splstr16[2] = static_cast<char16>('\0');
      return 2;
    } else {
      if (splid > 'C' - 'A' + 1)
        splid--;
      if (splid > 'S' - 'A' + 1)
        splid--;
      splstr16[0] = 'A' + splid - 1;
      splstr16[1] = '\0';
      return 1;
    }
  }

  // Not reachable.
  return 0;
}
    
    bool SpellingTrie::is_spl_homo_exception(uint16 spl_idx, uint16 son_spl_idx) const {
        if (spl_idx == 0 || son_spl_idx == 0 || spl_idx == son_spl_idx)
            return false;
        
        for (size_t i = 0; i < homo_spl_exception_column_size_*homo_spl_exception_row_size_; i+=homo_spl_exception_row_size_) {
            if (spl_idx != homo_spl_exception_ids_[i])
                continue;
            for (size_t j = 0; j < homo_spl_exception_row_size_; j++) {
                if (son_spl_idx == homo_spl_exception_ids_[i+j]) {
                    //printf("%s, ", SpellingTrie::get_instance().get_spelling_str(spl_idx));
                    //printf("%s, true\n", SpellingTrie::get_instance().get_spelling_str(son_spl_idx));
                    return true;
                }
            }
            //printf("%s, ", SpellingTrie::get_instance().get_spelling_str(spl_idx));
            //printf("%s, false\n", SpellingTrie::get_instance().get_spelling_str(son_spl_idx));
            return false;
        }
        //printf("%s, ", SpellingTrie::get_instance().get_spelling_str(spl_idx));
        //printf("%s, false\n", SpellingTrie::get_instance().get_spelling_str(son_spl_idx));
        return false;
    }

}  // namespace ime_pinyin
