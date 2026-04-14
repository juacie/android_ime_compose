#include "stdafx.h"
#include "trans_key_korean.h"

#if defined(__CREATE_DATA_LOG)
 #include "WfxFile.h"
#endif

//#define _DEBUG_MSG
// #define __DEBUG_MSG_MEM

#define __NEW_RULE

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SPLIT_KR_CH                    '|'
#define SPLIT_KR_CHX0                  L"|"
#define SPLIT_KR_CHX1                  L"||"
#define SPLIT_KR_CHX2                  L"|||"
#define SPLIT_KR_CH2                   '#'


#if defined(__DEBUG_MSG_RUN_MS) || defined(__DEBUG_MSG_LOADDICT_MS) || defined(__DEBUG_MSG_MEM)
 #include <iostream>
 #if !defined(__TEST) && !defined(_WIN32)
  #if !defined(KIKA_LOGD)
   #include <android/log.h>
   #define KIKA_LOGD(args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_KIKA_TAG , ## args)
  #endif
 #endif
#endif


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
trans_key_korean::trans_key_korean()
{
     initData();
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
trans_key_korean::~trans_key_korean()
{
     clear();
}


//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void trans_key_korean::clear()
{
     mV.clear();
     mL.clear();
     mKrUni.clear();
     mKrSpecial.clear();

     mLBbase.clear();

     mL0.clear();
     mLV.clear();
     mLVL.clear();
     mLVLL.clear();
     mVowel5.clear();
     mKeepRule2.clear();
     mKeepRule8.clear();
     
     #if defined(__LINUX_BASE)
     mL0.shrink_to_fit();
     mLV.shrink_to_fit();
     mLVL.shrink_to_fit();
     mLVLL.shrink_to_fit();
     mVowel5.shrink_to_fit();
     mKeepRule2.shrink_to_fit();
     mKeepRule8.shrink_to_fit();
     #endif
     
     #if defined(__FOR_ANDROID) && defined(__ANDROID_MSG) && defined(__DEBUG_PY_INPUT)
     KIKA_LOGD("Kika-ENGINE, trans_key_korean_release\r\n");
     #endif
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
int  trans_key_korean::backspace(wchar_t* wsKeyin)
{
     if ( wsKeyin[0] !=0 )
     {
          wstring
          wKeyin =wsKeyin;

          int
          delLen =backspace(wKeyin);

          wcscpy(wsKeyin, wKeyin.c_str());

          return delLen;
     }
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
int  trans_key_korean::backspace(wstring &wKeyin)
{
     if ( wKeyin.size() >0 )
     {
          wchar_t
          CH_prev =wKeyin[wKeyin.size() -1],
          CH_next =wKeyin.size() >1 ?wKeyin[wKeyin.size() -2] :0,
          CH3 =wKeyin.size() >2 ?wKeyin[wKeyin.size() -3] :0;

          switch ( CH_prev )
          {
                   case 0x3161: // -
                   case 0x3163: // |
                   case 0x318D: // .
                   case 0xFF1A: // :
                        switch ( CH_next )
                        {
                                 case 0x3163: // |
                                 case 0x3161: // -
                                 case 0x318D: // .
                                 case 0xFF1A: // :
                                      switch ( CH3 )
                                      {
                                               case 0x3163: // |
                                               case 0x3161: // -
                                               case 0x318D: // .
                                               case 0xFF1A: // :
                                                    wKeyin.erase(wKeyin.size() -3, 3);
                                                    return 3;
                                      }
                                      wKeyin.erase(wKeyin.size() -2, 2);
                                      return 2;
                        }

                    default:
                        //
                        wKeyin.erase(wKeyin.size() -1, 1);

                        if ( mV.find(CH_prev) !=mV.end() )
                        {
                             //wKeyin.insert(wKeyin.size() -1, 1, 'X');
                        }
                        return 1;
          }
     }
     else
     {
          wKeyin.erase(wKeyin.size() -1, 1);
          return 1;
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct target_cmp_3
{
       bool operator()( mapData_Korean4 const &a, mapData_Korean4 const &b) const
       {
            if ( a.k1 <b.k1 )
                 return true;
            
            else if ( a.k1 ==b.k1 )
            {
                 if ( a.k2 <b.k2 )
                      return true;
                 
                 else if ( a.k2 ==b.k2 )
                 {
                      if ( a.k3 <b.k3 )
                           return true;
                 }
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void trans_key_korean::initData()
{
     clear();
     
     //------------------------------------------------------------------------
     // 子音
     //------------------------------------------------------------------------
     mL.insert(0x3131);
     mL.insert(0x3134);
     mL.insert(0x3137);
     mL.insert(0x3139);
     mL.insert(0x3141);
     mL.insert(0x3142);
     mL.insert(0x3145);
     mL.insert(0x3147);
     mL.insert(0x3148);
     mL.insert(0x314A);
     mL.insert(0x314B);
     mL.insert(0x314C);
     mL.insert(0x314D);
     mL.insert(0x314E);
     mL.insert(0x3132);
     mL.insert(0x3138);
     mL.insert(0x3143);
     mL.insert(0x3146);
     mL.insert(0x3149);
     
     //------------------------------------------------------------------------
     // 母音
     //------------------------------------------------------------------------
     //mV.insert(0x318D);
     //mV.insert(0xFF1A);

     mV.insert(0x314F);
     mV.insert(0x3151);
     mV.insert(0x3153);
     mV.insert(0x3155);
     mV.insert(0x3157);
     mV.insert(0x315B);
     mV.insert(0x315C);
     mV.insert(0x3160);
     mV.insert(0x3161);
     mV.insert(0x3163);
     mV.insert(0x3150);
     mV.insert(0x3154);
     mV.insert(0x3152);
     mV.insert(0x3156);
     mV.insert(0x3158);
     mV.insert(0x3159);
     mV.insert(0x315A);
     mV.insert(0x315E);
     mV.insert(0x315D);
     mV.insert(0x315F);
     mV.insert(0x3162);

     //------------------------------------------------------------------------
     MEM_INT32
     idx   =0,
     total =6384;

     //------------------------------------------------------------------------
     for ( idx =0; idx <total; idx++ )
     {
           if ( lvBase[idx].k1 ==0 )
                break;
           
           mLBbase.push_back(lvBase[idx]);
     }

     //------------------------------------------------------------------------
     for ( idx =0; idx <total; idx++ )
     {
           if ( lv0[idx].k1 ==0 )
                break;
           
           mL0.push_back(lv0[idx]);
           mKrUni.insert(lv0[idx].cb);
     }
     
     //------------------------------------------------------------------------
     total =19;

     for ( idx =0; idx <total; idx++ )
     {
           mVowel5.push_back(vowel5[idx]);
     }     
     
     //------------------------------------------------------------------------
     total =38;

     for ( idx =0; idx <total; idx++ )
     {
           mKeepRule2.push_back(nocut2[idx]);
     }     

     //------------------------------------------------------------------------
     total =10374;

     for ( idx =0; idx <total; idx++ )
     {
           mKeepRule8.push_back(nocut8[idx]);
     }
     
     #if defined(__CREATE_DATA_LOG)
     
     std::sort(mL0.begin(), mL0.end(), target_cmp_3());
     
     {
          CString
          fname =WfxCurrPath(_T("\\Data.log"));
          
          CWfxFileW
          fx;
          fx.SaveHeader(fname);
          
          for ( S_mapData_Korean4 ii =mL0.begin(); ii !=mL0.end(); ii++ )
          {
                CString
                CstrK;
                CstrK.Format(_T("0x%04x, 0x%04x, 0x%04x"), ii->k1, ii->k2, ii->k3);
                
                fx.Append(CstrK);
                fx.AppendLn(L",");
          }
     }
     
     #endif
     
     //------------------------------------------------------------------------
     for ( idx =0; idx <total; idx++ )
     {
           if ( lv[idx].k1 ==0 )
                break;
           
           mLV.push_back(lv[idx]);
           mKrUni.insert(lv[idx].cb);
     }
     
     #if defined(__CREATE_DATA_LOG)
     
     std::sort(mLV.begin(), mLV.end(), target_cmp_3());

     {
          CString
          fname =WfxCurrPath(_T("\\Data.log"));
          
          CWfxFileW
          fx;
          fx.SaveHeader(fname);
          
          for ( S_mapData_Korean4 ii =mLV.begin(); ii !=mLV.end(); ii++ )
          {
                CString
                CstrK;
                CstrK.Format(_T("0x%04x, 0x%04x, 0x%04x"), ii->k1, ii->k2, ii->k3);
                
                fx.Append(CstrK);
                fx.AppendLn(L",");
          }
     }
     
     #endif
     
     //------------------------------------------------------------------------
     for ( idx =0; idx <total; idx++ )
     {
           if ( lvl[idx].k1 ==0 )
                break;
           
           mLVL.push_back(lvl[idx]);
           mKrUni.insert(lvl[idx].cb);
           //mKrSpecial.insert(lvl[idx].cb);
     }
     
     //------------------------------------------------------------------------
     for ( idx =0; idx <total; idx++ )
     {
           if ( lvll[idx].k1 ==0 )
                break;
           
           mLVLL.push_back(lvll[idx]);
           mKrUni.insert(lvll[idx].cb);
           mKrSpecial.insert(lvll[idx].cb);
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
struct target_order
{
       mapData_Korean4       ui;
       
       target_order(mapData_Korean4 _ui) :ui(_ui)
       {
            
       }
       
       //bool operator()(mapData_Korean4 const &a, mapData_Korean4 const &b) const
       bool operator()(const mapData_Korean4& chk) const
       {
            if ( ui.k1 !=chk.k1 )
                 return false;
            
            if ( ui.k2 !=chk.k2 )
                 return false;
            
            return true;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct keyMatchFuzzy2
{
       mapData_Korean3       in;
       
       keyMatchFuzzy2(mapData_Korean3 _in) : in(_in)
       {
       }
       
       bool operator()(const mapData_Korean3& chk) const
       {
            if ( chk.k1 ==in.k1 )
            {
                 if ( chk.k2 ==in.k2 )
                 {
                      return true;
                 }
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct keyMatchFuzzy3
{
       mapData_Korean4       in;
       
       keyMatchFuzzy3(mapData_Korean4 _in) : in(_in)
       {
       }
       
       bool operator()(const mapData_Korean4& chk) const
       {
            if ( chk.k1 ==in.k1 )
            {
                 if ( chk.k2 ==in.k2 )
                 {
                      if ( chk.k3 ==in.k3 )
                      {
                           return true;
                      }
                 }
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct keyMatchFuzzy_L3
{
       mapData_Korean3       in;
       
       keyMatchFuzzy_L3(mapData_Korean3 _in) : in(_in)
       {
       }
       
       bool operator()(const mapData_Korean3& chk) const
       {
            if ( chk.cb ==in.cb )
            {
                 return true;
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct keyMatchFuzzy_L4
{
       mapData_Korean4       in;
       
       keyMatchFuzzy_L4(mapData_Korean4 _in) : in(_in)
       {
       }
       
       bool operator()(const mapData_Korean4& chk) const
       {
            if ( chk.cb ==in.cb )
            {
                 return true;
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct keyMatchFuzzy_Base
{
       mapData_Korean4_Base  in;
       
       keyMatchFuzzy_Base(mapData_Korean4_Base _in) : in(_in)
       {
       }
       
       bool operator()(const mapData_Korean4_Base& chk) const
       {
            if ( chk.cb ==in.cb )
            {
                 return true;
            }
            return false;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : 解柝出基本字根
//
//-----------------------------------------------------------------------------
int  trans_key_korean::deComposingText(wstring wKeyin, wstring &wOut)
{
     wOut.resize(0);
     
     size_t 
     i, ret =0;

     #if defined(__DEBUG_MSG)
     const wchar_t*p2 =wOut.c_str();
     #endif

     //------------------------------------------------------------------------
     #if !defined(__DISABLE_LL3)
     for ( i =0; i <wKeyin.size(); i++ )
     {
           wstring
           wCH;
           wCH +=wKeyin.at(i);

           ret =deComposingText_L4(&mL0, wCH, wOut);

           if ( ret ==0 )
           {
                wOut +=wCH;
           }
     }

           
     if ( wOut.size() >0 )
     {
          wKeyin =wOut;
          wOut.resize(0);
     }

     for ( i =0; i <wKeyin.size(); i++ )
     {
           wstring
           wCH;
           wCH +=wKeyin.at(i);

           #if defined(__DEBUG_MSG)
           const wchar_t*p1 =wCH.c_str();
           const wchar_t*p2 =wOut.c_str();
           #endif

           ret =deComposingText_L4(&mLVLL, wCH, wOut);

           if ( ret ==0 )
           {
                wOut +=wCH;
           }
     }

     if ( wOut.size() >0 )
     {
          wKeyin =wOut;
          wOut.resize(0);
     }
     #endif

     //------------------------------------------------------------------------
     #if !defined(__DISABLE_LL4)
     for ( i =0; i <wKeyin.size(); i++ )
     {
           wstring
           wCH;
           wCH +=wKeyin.at(i);

           ret =deComposingText_L3(&mLV, wCH, wOut);

           if ( ret ==0 )
           { 
                wOut +=wCH;
           }
     }

     if ( wOut.size() >0 )
     {
          wKeyin =wOut;
          wOut.resize(0);
     }

     for ( i =0; i <wKeyin.size(); i++ )
     {
           wstring
           wCH;
           wCH +=wKeyin.at(i);

           #if defined(__DEBUG_MSG)
           const wchar_t*p1 =wCH.c_str();
           #endif

           ret =deComposingText_L3(&mLVL, wCH, wOut);

           if ( ret ==0 )
           {
                ret =deComposingText_L4Base(&mLBbase, wCH, wOut);

                if ( ret ==0 )
                {
                     wOut +=wCH;
                }
           }
     }
     #endif

     return ret;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : 解柝出基本字根
//
//-----------------------------------------------------------------------------
int  trans_key_korean::deComposingText_L3(vector<mapData_Korean3>* p, wstring wKeyin, wstring &wOut)
{
     mapData_Korean3
     ui;
     ui.set(INVALID_DATA, INVALID_DATA, wKeyin.at(0));

     S_mapData_Korean3
     found =find_if(p->begin(), p->end(), keyMatchFuzzy_L3(ui));
     
     if ( found !=p->end() )
     {
          if ( found->k1 !=0 )
          {
               //wOut +=found->k1;
               wstring
               wCH; 
               wCH +=found->k1;
               
               if ( deComposingText_L3(p, wCH, wOut) ==0 )
               {
                    wOut +=wCH;
               }
          }
          if ( found->k2 !=0 )
          {
               //wOut +=found->k2;
               wstring
               wCH; 
               wCH +=found->k2;
               
               if ( deComposingText_L3(p, wCH, wOut) ==0 )
               {
                    wOut +=wCH;
               }
          }
          return found->k2 ==0 ? 1: 2;
     }
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : 解柝出基本字根
//
//-----------------------------------------------------------------------------
int  trans_key_korean::deComposingText_L4(vector<mapData_Korean4>* p, wstring wKeyin, wstring &wOut)
{
     mapData_Korean4
     ui;
     ui.set(INVALID_DATA, INVALID_DATA, INVALID_DATA, wKeyin.at(0));

     S_mapData_Korean4
     found =find_if(p->begin(), p->end(), keyMatchFuzzy_L4(ui));
     
     if ( found !=p->end() )
     {
          if ( found->k1 !=0 )
          {
               //wOut +=found->k1;
               wstring
               wCH; 
               wCH +=found->k1;
               
               if ( deComposingText_L4(p, wCH, wOut) ==0 )
               {
                    wOut +=wCH;
               }
          }
          if ( found->k2 !=0 )
          {
               //wOut +=found->k2;
               wstring
               wCH; 
               wCH +=found->k2;
               
               if ( deComposingText_L4(p, wCH, wOut) ==0 )
               {
                    wOut +=wCH;
               }
          }
          if ( found->k3 !=0 )
          {
               //wOut +=found->k3;
               wstring
               wCH; 
               wCH +=found->k3;
               
               if ( deComposingText_L4(p, wCH, wOut) ==0 )
               {
                    wOut +=wCH;
               }
          }
          return found->k3 ==0 ? (found->k2 ==0? 1: 2): 3;
     }
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : 解柝出基本字根
//
//-----------------------------------------------------------------------------
int  trans_key_korean::deComposingText_L4Base(vector<mapData_Korean4_Base>* p, wstring wKeyin, wstring &wOut)
{
     mapData_Korean4_Base
     ui;
     ui.set(wKeyin.at(0));

     S_mapData_Korean4_Base
     found =find_if(p->begin(), p->end(), keyMatchFuzzy_Base(ui));
     
     if ( found !=p->end() )
     {
          if ( found->k1 !=0 )
          {
               //wOut +=found->k1;
               wstring
               wCH; 
               wCH +=found->k1;
               
               if ( deComposingText_L4Base(p, wCH, wOut) ==0 )
               {
                    wOut +=wCH;
               }
          }
          if ( found->k2 !=0 )
          {
               //wOut +=found->k2;
               wstring
               wCH; 
               wCH +=found->k2;
               
               if ( deComposingText_L4Base(p, wCH, wOut) ==0 )
               {
                    wOut +=wCH;
               }
          }
          if ( found->k3 !=0 )
          {
               //wOut +=found->k3;
               wstring
               wCH; 
               wCH +=found->k3;
               
               if ( deComposingText_L4Base(p, wCH, wOut) ==0 )
               {
                    wOut +=wCH;
               }
          }
          return found->k3 ==0 ? (found->k2 ==0? 1: 2): 3;
     }
     return 0;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  trans_key_korean::getComposingText_L0(wstring wKeyin, wstring &wOut)
{
     wOut.resize(0);
     
     mapData_Korean4
     ui;
     ui.set(wKeyin);
     
     #if defined(__SET)
     
     set<mapData_Korean4>::iterator
     found =mL0.find(ui);
     
     if ( found !=mL0.end() )
     {
          wOut =found->cb;
     }
     
     #else
     
     S_mapData_Korean4
     found =find_if(mL0.begin(), mL0.end(), keyMatchFuzzy3(ui));
     
     if ( found !=mL0.end() )
     {
          wOut =found->cb;
     }
     
     #endif
     
     return found->k3 ==0 ? 2: 3;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  trans_key_korean::getComposingText_LV(wstring wKeyin, wstring &wOut)
{
     wOut.resize(0);
     
     mapData_Korean3
     ui;
     ui.set(wKeyin);
     
     #if defined(__SET)
     
     set<mapData_Korean4>::iterator
     found =mLV.find(ui);
     
     if ( found !=mLV.end() )
     {
          wOut =found->cb;
     }
     
     #else
     
     S_mapData_Korean3
     found =find_if(mLV.begin(), mLV.end(), keyMatchFuzzy2(ui));
     
     if ( found !=mLV.end() )
     {
          wOut =found->cb;
     }
     
     #endif
     
     return 2;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  trans_key_korean::getComposingText_LVL(wstring wKeyin, wstring &wOut)
{
     wOut.resize(0);
     
     mapData_Korean3
     ui;
     ui.set(wKeyin);
     
     #if defined(__SET)
     
     set<mapData_Korean3>::iterator
     found =mLV.find(ui);
     
     if ( found !=mLVL.end() )
     {
          wOut =found->cb;
     }
     
     #else
     
     S_mapData_Korean3
     found =find_if(mLVL.begin(), mLVL.end(), keyMatchFuzzy2(ui));
     
     if ( found !=mLVL.end() )
     {
          wOut =found->cb;
     }
     
     #endif
     
     return 2;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
int  trans_key_korean::getComposingText_LVLL(wstring wKeyin, wstring &wOut)
{
     wOut.resize(0);
     
     mapData_Korean4
     ui;
     ui.set(wKeyin);
     
     #if defined(__SET)
     
     set<mapData_Korean4>::iterator
     found =mLVLL.find(ui);
     
     if ( found !=mLVLL.end() )
     {
          wOut =found->cb;
     }
     
     #else
     
     S_mapData_Korean4
     found =find_if(mLVLL.begin(), mLVLL.end(), keyMatchFuzzy3(ui));
     
     if ( found !=mLVLL.end() )
     {
          wOut =found->cb;
     }
     
     #endif
     
     return found->k3 ==0 ? 2: 3;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
wstring trans_key_korean::getComposingText_Tail(wstring wKeyin)
{
     wstring
     wOut;
     
     wstring
     wKey =wKeyin;
     
     vector<MEM_INT>
     vFinal;
     
     mwCompText.resize(0);
     
     while ( wKeyin.size() )
     {
             bool
             succ =false;
             
             wstring
             wKey =wKeyin;
             
             while ( wKey.size() >0 )
             {
                     #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                     TRACE(_T("wKey =%ls\r\n"), wKey.c_str());
                     #endif
                     
                     for ( size_t i =0; i <wKey.size(); i++ )
                     {
                           #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                           TRACE(_T("Key(%d) =%x\r\n"), i, wKey.c_str()[i]);
                           #endif
                     }
                     
                     //----------------------------------------------------------------
                     getComposingText_L0(wKey, wOut);
                     
                     if ( !wOut.empty() )
                     {
                          #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                          TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                          #endif
                          
                          succ =true;
                          vFinal.push_back(wOut[0]);
                          wKeyin.erase(wKeyin.size() -wKey.size(), wKey.size());
                          break;
                     }
                     
                     //----------------------------------------------------------------
                     getComposingText_LVLL(wKey, wOut);
                     
                     if ( !wOut.empty() )
                     {
                          #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                          TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                          #endif
                          
                          succ =true;
                          vFinal.push_back(wOut[0]);
                          wKeyin.erase(wKeyin.size() -wKey.size(), wKey.size());
                          break;
                     }
                     
                     //----------------------------------------------------------------
                     getComposingText_LVL(wKey, wOut);
                     
                     if ( !wOut.empty() )
                     {
                          #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                          TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                          #endif
                          
                          succ =true;
                          vFinal.push_back(wOut[0]);
                          wKeyin.erase(wKeyin.size() -wKey.size(), wKey.size());
                          break;
                     }
                     
                     //----------------------------------------------------------------
                     getComposingText_LV(wKey, wOut);
                     
                     if ( !wOut.empty() )
                     {
                          #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                          TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                          #endif
                          
                          succ =true;
                          vFinal.push_back(wOut[0]);
                          wKeyin.erase(wKeyin.size() -wKey.size(), wKey.size());
                          break;
                     }
                     
                     wKey.erase(0, 1);
             }
             if ( succ ==false )
             {
                  MEM_INT
                  CH =wKeyin[wKeyin.size() -1];
                  
                  vFinal.push_back(CH);
                  
                  wKeyin.erase(wKeyin.size() -1, 1);
             }
             
             #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
             TRACE(_T("wKeyin =%ls, sz =%d\r\n"), wKeyin.c_str(), wKeyin.size());
             #endif
     }
     
     vector<MEM_INT>::iterator
     pi =vFinal.end();
     
     while ( pi !=vFinal.begin() )
     {
             MEM_INT
             CH =*--pi;
             
             mwCompText +=CH;//*--pi;
             
             #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
             TRACE(_T("ccc =%x\r\n"), CH);
             #endif
     }
     
     return mwCompText;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
wstring trans_key_korean::getComposingText_Head(wstring wKeyin)
{
     wstring
     wOut;
     
     wstring
     wKey =wKeyin;
     
     #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
     TRACE(_T("wKeyin Sz =%d\r\n"), wKeyin.size());
     #endif

     while ( 1 )
     {
             int
             match =0;
             
             mwCompText.resize(0);
             
             while ( wKeyin.size() )
             {
                     bool
                     succ =false;
                     
                     wstring
                     wKey =wKeyin;
                     
                     while ( wKey.size() >0 )
                     {
                             #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                             TRACE(_T("wKey =%ls\r\n"), wKey.c_str());
                             #endif
                             
                             for ( size_t i =0; i <wKey.size(); i++ )
                             {
                                   #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                   TRACE(_T("Key(%d) =%x\r\n"), i, wKey.c_str()[i]);
                                   #endif
                             }
                             
                             #if defined(__FIX_BASE)
                             
                             if ( wKey.size() >3 )
                             {
                                  if ( wKey[0] ==0x3161 && wKey[1] ==0xff1a && wKey[2] ==0x3163 && wKey[3] ==0x3163 )
                                  {
                                       wKey[0] =0x315c;
                                       wKey[1] =0x3154;
                                       wKey.erase(2, 2);
                                       wKeyin.erase(2, 2);
                                       
                                       #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                       TRACE(_T("mwCompText Sz =%d\r\n"), mwCompText.size());
                                       #endif
                                       
                                       continue;
                                  }
                             }
                             
                             #endif
                             
                             if ( wKey.size() >2 )
                             {
                                  if ( wKey[0] ==0x3161 && wKey[1] ==0xff1a && wKey[2] ==0x3163 )
                                  {
                                       #if !defined(__FIX_BASE)
                                       
                                       wKey[0] =0x315c;
                                       wKey[1] =0x318d;
                                       wKey[2] =0x3163;
                                       
                                       wKeyin[0] =0x315c;
                                       wKeyin[1] =0x318d;
                                       wKeyin[2] =0x3163;
                                       
                                       #else
                                       
                                       wKey[0] =0x315c;
                                       wKey[1] =0x3153;
                                       wKey.erase(2, 1);
                                       wKeyin.erase(2, 1);
                                       
                                       #endif
                                       
                                       #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                       TRACE(_T("mwCompText Sz =%d\r\n"), mwCompText.size());
                                       #endif
                                       
                                       continue;
                                  }
                             }
                             
                             //----------------------------------------------------------------
                             int
                             mLen =getComposingText_L0(wKey, wOut);
                             
                             if ( !wOut.empty() )
                             {
                                  #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                  TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                                  #endif
                                  
                                  succ =true;
                                  mwCompText +=wOut;
                                  wKeyin.erase(0, mLen);
                                  break;
                             }
                             
                             //----------------------------------------------------------------
                             mLen =getComposingText_LVLL(wKey, wOut);
                             
                             if ( !wOut.empty() )
                             {
                                  #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                  TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                                  #endif
                                  
                                  succ =true;
                                  mwCompText +=wOut;
                                  wKeyin.erase(0, mLen);
                                  break;
                             }
                             
                             //----------------------------------------------------------------
                             mLen =getComposingText_LV(wKey, wOut);
                             
                             if ( !wOut.empty() )
                             {
                                  #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                  TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                                  #endif
                                  
                                  succ =true;
                                  mwCompText +=wOut;
                                  wKeyin.erase(0, mLen);
                                  
                                  #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                  TRACE(_T("mwCompText Sz =%d\r\n"), mwCompText.size());
                                  #endif
                                  
                                  break;
                             }
                             
                             //----------------------------------------------------------------
                             mLen =getComposingText_LVL(wKey, wOut);
                             
                             if ( !wOut.empty() )
                             {
                                  #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                  TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                                  #endif
                                  
                                  succ =true;
                                  mwCompText +=wOut;
                                  wKeyin.erase(0, mLen);
                                  break;
                             }
                             
                             wKey.erase(wKey.size() -1, 1);
                             
                             #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                             TRACE(_T("wKey.size() =%d\r\n"), wKey.size());
                             #endif
                     }
                     if ( succ ==false )
                     {
                          mwCompText +=wKeyin[0];
                          wKeyin.erase(0, 1);
                     }
                     else
                     {
                          match++;
                     }
             }
             wKeyin =mwCompText;
             
             #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
             TRACE(_T("wKeyin.size() =%d\r\n"), wKeyin.size());
             #endif
             
             if ( match ==0 )
                  break;
     }
     
     return mwCompText;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
static
void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
{
     if ( from.empty() )
          return;

     size_t start_pos = 0;

     while ( (start_pos = str.find(from, start_pos)) != std::string::npos)
     {
             str.replace(start_pos, from.length(), to);
             start_pos += to.length();
     }
}

bool compareRule(const mapData_Korean8 &a, const mapData_Korean8 &b)
{
     for ( unsigned i =0; i <8; i++ )
     {
           if ( a.cb[i] !=b.cb[i] )
                return false;
     }
     return true;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct keepRule2
{
       mapData_Korean2       in;
       
       keepRule2(mapData_Korean2 _in) : in(_in)
       {
       }
       
       bool operator()(const mapData_Korean2& chk) const
       {
            for ( unsigned i =0; i <2; i++ )
            {
                  if ( in.cb[i] !=chk.cb[i] )
                       return false;
            }
            return true;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct IsVowel5
{
       mapData_Korean5       in;
       
       IsVowel5(mapData_Korean5 _in) : in(_in)
       {
       }
       
       bool operator()(const mapData_Korean5& chk) const
       {
            for ( unsigned i =0; i <5; i++ )
            {
                  if ( in.cb[i] !=chk.cb[i] )
                       return false;
            }
            return true;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
//
//-----------------------------------------------------------------------------
struct keepRule8
{
       mapData_Korean8       in;
       
       keepRule8(mapData_Korean8 _in) : in(_in)
       {
       }
       
       bool operator()(const mapData_Korean8& chk) const
       {
            for ( unsigned i =0; i <8; i++ )
            {
                  if ( in.cb[i] !=chk.cb[i] )
                       return false;
            }
            return true;
       }
};

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
std::wstring tail(std::wstring const& source, size_t const length) 
{
     if ( length >= source.size() ) 
     {
          return source; 
     }
     return source.substr(source.size() - length);
} // tail

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
wstring trans_key_korean::getComposingText(wstring wKeyinEx, bool chk_seperate, bool chk_ui_Action)
{
     wstring
     wOut,
     wKeyL;
     
     wstring
     wKeyin =L"";
     
     #if defined(__NEW_RULE2)
     
     wKeyin =wKeyinEx;
     
     #else
     
     mapData_Korean8
     uiX8;
     uiX8.set(wKeyinEx);
     
     S_mapData_Korean8
     found =find_if(mKeepRule8.begin(), mKeepRule8.end(), keepRule8(uiX8));
     
     #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
     const wchar_t *pp1 =wKeyinEx.c_str();
     const wchar_t *pp2 =wKeyin.c_str();
     #endif

     bool keepLastDot =false;

     if ( found ==mKeepRule8.end() )
     {
          if ( chk_seperate ==true && wKeyinEx.size() >=4 )
          {
               wchar_t
               sCH[2] ={0, 0};
               
               sCH[0] =wKeyinEx[0];
               wKeyin.append(sCH);

               sCH[0] =wKeyinEx[1];
               wKeyin.append(sCH);
               
               bool breakCode =false;
               
               for ( size_t x =2; x <wKeyinEx.size(); x++ )
               {
                     wchar_t
                     CH_prev =wKeyinEx[x -1], //前一字
                     CH_next =wKeyinEx[x];    //後一字
                     
                     if ( x ==wKeyinEx.size() -1 )
                     {
                          mapData_Korean2
                          uiX2;
                          uiX2.cb[0] =CH_prev;
                          uiX2.cb[1] =CH_next;
     
                          S_mapData_Korean2
                          found =find_if(mKeepRule2.begin(), mKeepRule2.end(), keepRule2(uiX2));

                          if ( found !=mKeepRule2.end() )
                          {
                               // 最後兩個字是結合之特殊字根, 表示不要斷開
                               keepLastDot =true;

                               sCH[0] =wKeyinEx[x];
                               wKeyin.append(sCH);

                               break;
                          }
                     }

                     if ( (CH_prev !=0x318D && CH_prev !=0xFF1A) )
                     {
                          mapData_Korean5
                          uiX5;

                          S_mapData_Korean5
                          found5 =mVowel5.end();

                          int ssize =wKeyinEx.size();//8832556621;

                          unsigned cn =5;
                          wstring
                          wVowel;

                          #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                          const wchar_t *pvowel =wVowel.c_str();
                          #endif

                          for ( ; cn >0; cn-- )
                          {
                                if ( wKeyinEx.size() >=4 +cn )
                                {
                                     wVowel =wKeyinEx.substr(x, cn);

                                     uiX5.set(wVowel);

                                     found5 =find_if(mVowel5.begin(), mVowel5.end(), IsVowel5(uiX5));
                                }
                                if ( found5 !=mVowel5.end() )
                                {
                                     //找到, 表示是結合母音
                                     break;
                                }
                          }
                          bool isVowel =(found5 !=mVowel5.end());

                          if ( isVowel ==false )
                          {
                               isVowel =(mV.find(CH_next) !=mV.end()); // next is Vowel (母音)
                          }

                          if ( (x -cn >0) && isVowel ==true && mV.find(CH_prev) ==mV.end() ) // prev is not Vowel, is Consonant (子音)
                          {
                               if ( chk_ui_Action ==true )
                               {
                                    #if !defined(__CUT_IT)
                                    
                                    wKeyinEx.erase(0, x -1);
                                    
                                    return getComposingText(wKeyinEx, chk_seperate, chk_ui_Action);

                                    #else

                                    wKeyin.insert(wKeyin.size() -1, 1, '\r');
                                    wKeyin.insert(wKeyin.size() -1, 1, '\n');

                                    #endif
                               }
                               else
                               {
                                    #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                    const wchar_t *pp1 =wKeyin.c_str();
                                    #endif

                                    // 插入分隔字元
                                    wKeyin.insert(wKeyin.size() -1, 1, SPLIT_KR_CH);

                                    #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                                    const wchar_t *pp2 =wKeyin.c_str();
                                    #endif
                               }
                               breakCode =true;
                          }
                     }

                     sCH[0] =wKeyinEx[x];
                     wKeyin.append(sCH);
                     
                     if ( breakCode ==true )
                     {
                          #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                          const wchar_t *pp1 =wKeyin.c_str();
                          #endif

                          // 之後的每個字根都要分開
                          wKeyin.insert(wKeyin.size() -1, 1, SPLIT_KR_CH2);

                          #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
                          const wchar_t *pp2 =wKeyin.c_str();
                          #endif
                     }
               }
          }
          else
          {
               if ( wKeyinEx.size() >1 )
               {
                    wchar_t
                    CH_prev =wKeyinEx[wKeyinEx.size() -2],
                    CH_next =wKeyinEx[wKeyinEx.size() -1];

                    mapData_Korean2
                    uiX2;
                    uiX2.cb[0] =CH_prev;
                    uiX2.cb[1] =CH_next;
     
                    S_mapData_Korean2
                    found =find_if(mKeepRule2.begin(), mKeepRule2.end(), keepRule2(uiX2));

                    if ( found !=mKeepRule2.end() )
                    {
                         // 最後兩個字是結合之特殊字根, 表示不要斷開
                         keepLastDot =true;
                    }
               }
               wKeyin =wKeyinEx;
          }
     }
     else
     {
          wKeyin =wKeyinEx;
     }
     
     #endif
     
     //------------------------------------------------------------------------

     while ( wKeyin.size() )
     {
             #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
             TRACE(_T("Origin wKeyin Sz=%d, K-Head =0x%x\r\n"), wKeyin.size(), wKeyin[0] );
             #endif
             
             // 取出[子音]
             while ( wKeyin.size() >0 && mL.find(wKeyin[0]) !=mL.end() )
             {
                     wKeyL +=wKeyin[0];
                     wKeyin.erase(0, 1);
             }
             
             #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
             TRACE(_T("Lead wKeyin Sz=%d, K-Head =0x%x\r\n"), wKeyin.size(), wKeyin[0] );
             #endif
             
             // 取出[母音]
             wstring
             wKeyV;
             
             while ( wKeyin.size() >0 && mL.find(wKeyin[0]) ==mL.end() )
             {
                     wKeyV +=wKeyin[0];
                     wKeyin.erase(0, 1);
             }
             
             // 結合[母音]
             if ( wKeyV.size() )
             {
                  wOut =getComposingText_Head(wKeyV);
                  wKeyL +=wOut;
             }
     }
     
     // 結合[子音+結合音]
     wOut =getComposingText_Head(wKeyL);
     
     #if defined(__NEW_RULE)
     
     if ( chk_seperate ==false )
     {
          return wOut;
     }

     //------------------------------------------------------------------------
     mapData_Korean8
     uiX8_2;
     uiX8_2.set(wKeyinEx);
     
     //S_mapData_Korean8
     found =find_if(mKeepRule8.begin(), mKeepRule8.end(), keepRule8(uiX8_2));
     
     if ( found !=mKeepRule8.end() )
     {
          return wOut;
     }
     
     //------------------------------------------------------------------------
     wstring
     wOut2;

     #if defined(_DEBUG) && defined(_WIN32) && defined(_DEBUG_MSG)
     const wchar_t* pp1 =wOut.c_str();
     const wchar_t* pp2 =wOut2.c_str();
     #endif
     
     for ( size_t i =0; i <wOut.size(); i++ )
     {
           MEM_INT
           cb =wOut[i];
           
           if ( wOut[i] ==SPLIT_KR_CH )
           {
                if ( i ==0 || (i >0 && wOut2[i] !=SPLIT_KR_CH) )
                     wOut2 +=wOut[i];
           }
           else
           {
                wOut2 +=wOut[i];
           }
           
           bool
           ok1 =wOut[i] !=SPLIT_KR_CH,
           ok2 =mKrUni.find(cb) !=mKrUni.end(),
           ok3 =mV.find(cb) !=mV.end(),
           ok4 =mL.find(cb) !=mL.end(),
           ok5 =(wOut[i] ==0x318D || wOut[i] ==0xFF1A);

           if ( ok1 && (ok2 || ok3 || ok4 || ok5) )
           {
                wchar_t
                iCH[2] ={0};
                iCH[0] =SPLIT_KR_CH;
                
                if ( i ==wOut.size() -2 )
                {
                     if ( keepLastDot ==true && (wOut[i +1] ==0x318D || wOut[i +1] ==0xFF1A) )
                     {
                          wOut2 +=wOut[i +1];
                          break;
                     }
                }
                if ( i !=wOut.size() -1 && wOut2[i] !=SPLIT_KR_CH && wOut[i +1] !=SPLIT_KR_CH2 && wOut[i] !=SPLIT_KR_CH2 )
                {
                     #if defined(__DOT_IS_KOREAN_STROKES)

                     wchar_t 
                     self_symbol[2] ={0x3139, 0}; // 
                     
                     int 
                     po =wKeyL.find(self_symbol);
                     
                     if ( po ==wOut.npos )
                     #endif

                     {
                          if ( chk_ui_Action ==true )
                          {
                               #if !defined(__CUT_IT)

                               wOut.erase(0, i +1);

                               return getComposingText(wOut, chk_seperate, chk_ui_Action);

                               #else

                               wOut2 +=0x0D;
                               wOut2 +=0x0A;

                               #endif
                          }
                          else
                          {
                               wOut2 +=iCH;
                          }
                     }
                }
           }
     }
     
     //883255662
     //412512612
     //412788
     //41208
     
     //replaceAll(wOut2, SPLIT_KR_CHX2, SPLIT_KR_CHX0);
     //replaceAll(wOut2, SPLIT_KR_CHX1, SPLIT_KR_CHX0);
     
     wOut2.erase(remove(wOut2.begin(), wOut2.end(), SPLIT_KR_CH2), wOut2.end());
     
     return wOut2;
     
     #else
     
     wOut.erase(remove(wOut.begin(), wOut.end(), SPLIT_KR_CH2), wOut.end());
     
     return wOut;
     
     #endif
}
