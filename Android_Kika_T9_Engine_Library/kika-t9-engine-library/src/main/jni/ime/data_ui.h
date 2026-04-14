// data_ui.h: interface for the data_ui class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_UI_H__6EA03A1C_ED45_4FCC_9182_8457D5E941C8__INCLUDED_)
#define AFX_DATA_UI_H__6EA03A1C_ED45_4FCC_9182_8457D5E941C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>

//-----------------------------------------------------------------------------
#define HEAD_ID_DICT_SIZE         "KIKA-DICT-XXXXXXXX"
#define HEAD_ID_DICT_V3           "KIKA-DICT-V3"
#define HEAD_ID_DICT_V2_1         "KIKA-DICT-V2-1"
#define HEAD_ID_DICT_V2_1_PY      "KIKA-DICT-V2-1-PY"
#define HEAD_ID_DICT_V2_1_KR      "KIKA-DICT-V2-1-KR"
#define HEAD_ID_DICT_V2_1W        "KIKA-DICT-V2-1W"
#define HEAD_ID_DICT_V2_2W        "KIKA-DICT-V2-2W"
#define HEAD_ID_DICT_V2_2_1W      "KIKA-DICT-V2-2-1W"
#define HEAD_ID_DICT_V2_2_2W      "KIKA-DICT-V2-2-2W"
#define HEAD_ID_DICT_V2           "KIKA-DICT-V2"
#define HEAD_ID_PHRS_V2           "KIKA-PHRS-V2"
#define ENC_KEY_SZ                "KIKADATA"

typedef char                      MEM_CHR;
typedef MEM_CHR*                  LPMEM_CHR;
typedef const MEM_CHR*            LPCMEM_CHR;

typedef unsigned short            MEM_INT;
typedef unsigned int              MEM_INT32;

typedef MEM_INT*                  LPMEM_INT;
typedef const MEM_INT*            LPCMEM_INT;

typedef MEM_INT32*                LPMEM_INT32;
typedef const MEM_INT32*          LPCMEM_INT32;

#define INVALID_MEM_OFFSET        0
#define MAX_DICT_KLEN             60
#define MAX_GROUPING              10
#define MAX_ALLOC_MEM             64000000


//-----------------------------------------------------------------------------
enum FORMAT_ID
{
     FORMAT_V1 =1,
     FORMAT_V2
};

enum FMT_SORT_ID
{
     SORT_BY_CH =1,
     SORT_BY_EN,
};

//-----------------------------------------------------------------------------
class MEM_UI
{
private:
      LPMEM_CHR              pMem;

public:
      MEM_UI()
      {
           pMem =0;
      }

      ~MEM_UI()
      {
           clear();
      }

      void clear()
      {
           if ( pMem !=0 )
           {
                delete [] pMem;
                pMem =0;
           }
      }

      bool alloc(MEM_INT32 memSz)
      {
           pMem =new MEM_CHR[memSz +1];

           if ( pMem ==0 )
           {
                return false;
           }
           memset(pMem, 0, (memSz +1) *sizeof(MEM_CHR));

           return true;
      }

      LPMEM_CHR bmem()
      {
           return pMem;
      }
};
typedef MEM_UI                         LPMEM_UI;

//-----------------------------------------------------------------------------
struct PHRS_IDX
{
       MEM_INT               vLen;
       MEM_INT32             offs;

       PHRS_IDX()
       {
       }

       PHRS_IDX(MEM_INT vLen, MEM_INT32 offs)
       {
            this->vLen =vLen;
            this->offs =offs;
       }
};

//-----------------------------------------------------------------------------
struct IDIOM_UI
{
       wstring               wsKeys;
       MEM_INT32             iNext;

       IDIOM_UI()
       {
       }

       IDIOM_UI(const wchar_t *wsKeys, const MEM_INT32 iNext =INVALID_MEM_OFFSET)
       {
            this->wsKeys =wsKeys;
            this->iNext  =iNext;
       }

       bool operator ==(const IDIOM_UI& chk) const
       {
            return (wsKeys ==chk.wsKeys);
       }

       bool operator <(const IDIOM_UI& chk) const
       {
            return wsKeys <chk.wsKeys;
       }
};
typedef vector<IDIOM_UI>*              IDIOM_PTR;
typedef vector<IDIOM_UI>::iterator     IDIOM_ITR;

//-----------------------------------------------------------------------------
class phraseimp;

struct PHRS_UI
{
       int                   sID;
       wstring               wsKeys;
       wstring               wsPhrs;
       MEM_INT32             iNext;
       phraseimp*            pIme;

       PHRS_UI()
       {
       }

       PHRS_UI(phraseimp *_pIme, int _sID, const wstring _wsKeys, const wstring _wsPhrs, const MEM_INT32 _iNext =INVALID_MEM_OFFSET)
       {
            pIme   =_pIme;
            sID    =_sID;
            wsKeys =_wsKeys;
            wsPhrs =_wsPhrs;
            iNext =_iNext;
       }

       PHRS_UI(phraseimp *_pIme, int _sID, const wchar_t *_wsKeys, const wchar_t *_wsPhrs, const MEM_INT32 _iNext =INVALID_MEM_OFFSET)
       {
            pIme   =_pIme;
            sID    =_sID;
            wsKeys =_wsKeys;
            wsPhrs =_wsPhrs;
            iNext =_iNext;
       }

       PHRS_UI(phraseimp *_pIme, vector<PHRS_UI>::iterator _in)
       {
            pIme   =_pIme;
            sID    =_in->sID;
            wsKeys =_in->wsKeys;
            wsPhrs =_in->wsPhrs;
            iNext  =_in->iNext;
       }

       bool operator ==(const PHRS_UI& chk) const
       {
            bool
            match =(wsKeys ==chk.wsKeys);

            if ( match )
            {
                 return sID ==chk.sID && wsPhrs ==chk.wsPhrs;
            }
            return false;
       }

       bool operator <(const PHRS_UI& chk) const
       {
            if ( wsKeys ==chk.wsKeys )
            {
                 return sID <chk.sID;
            }
            return wsKeys <chk.wsKeys;
       }
};

typedef vector<PHRS_UI>*               PHRS_PTR;
typedef vector<PHRS_UI>::iterator      PHRS_ITR;

#if defined(__FOR_NEW_SET)
typedef set<PHRS_UI>::iterator         PHRS_GRP_ITR;
#else
typedef vector<PHRS_UI>::iterator      PHRS_GRP_ITR;
#endif

//-----------------------------------------------------------------------------
struct PHRS_V1_POS
{
       PHRS_ITR              itr;
       wstring               wsKeys;
       wstring               wsStrokes;

       bool operator ==(const PHRS_V1_POS& rd) const
       {
            if ( wsKeys.compare(rd.wsKeys) ==0 && wsStrokes.compare(rd.wsStrokes) ==0 )
            {
                 return true;
            }
            return false;
       }

       // for set<PHRS_V1_POS>
       PHRS_V1_POS()
       {
            wsKeys    =L"";
            wsStrokes =L"";
       }

       PHRS_V1_POS(PHRS_ITR _itr)
       {
            itr       =_itr;
            wsKeys    =itr->wsKeys;
            wsStrokes =itr->wsPhrs;
       }

       PHRS_V1_POS(const wstring _wsKeys, const wstring _wsStrokes)
       {
            wsKeys    =_wsKeys;
            wsStrokes =_wsStrokes;
       }

       // for set<PHRS_V1_POS>
       bool operator <(const PHRS_V1_POS & rd) const
       {
            if ( wsKeys.compare(rd.wsKeys) <0 )
                 return true;

            else if ( wsStrokes.compare(rd.wsStrokes) <0 )
                 return true;

            return false;
       }
};
typedef vector<PHRS_V1_POS>::iterator  PHRS_V1_POS_VTR;

#if defined(__FOR_NEW_SET)
typedef set<PHRS_V1_POS>::iterator     PHRS_V1_POS_ITR;
#else
typedef vector<PHRS_V1_POS>::iterator  PHRS_V1_POS_ITR;
#endif

#include "debug_util.h"
#include "string_util.h"

//-----------------------------------------------------------------------------
// for set<USER_V1_DAT>
struct USER_V1_DAT
{
       int                   sID;
       MEM_INT32             orderID;
       wstring               wsKeys;
       wstring               wsPhrs;

       //----------------------------------------------------------------------
       USER_V1_DAT(int _sID, const wstring _wsKeys, const wstring _wsPhrs, MEM_INT32 _orderID)
       {
            sID     =_sID;
            orderID =_orderID;
            wsKeys  =_wsKeys;
            wsPhrs  =_wsPhrs;
       }

       MEM_INT32 getOrderID()
       {
            return orderID;
       }

       void updateOrderID(MEM_INT32 _orderID)
       {
            orderID =_orderID;
       }

       bool operator ==(const USER_V1_DAT& rd) const
       {
            if ( wsKeys.compare(rd.wsKeys) ==0 && wsPhrs.compare(rd.wsPhrs) ==0 )
            {
                 return true;
            }
            return false;
       }

       bool operator <(const USER_V1_DAT & rd) const
       {
            string::size_type
            position1 =wsKeys.find(rd.wsKeys);

            if ( position1 ==0 )
            {
                 if ( orderID >rd.orderID )
                      return true;
                 else if ( rd.orderID >orderID )
                      return false;
            }
            string::size_type
            position2 =rd.wsKeys.find(wsKeys);

            if ( position2 ==0 )
            {
                 if ( orderID >rd.orderID )
                      return true;
                 else if ( rd.orderID >orderID )
                      return false;
            }

            else if ( wsKeys.compare(rd.wsKeys) <0 )
                 return true;
                 else if ( rd.wsKeys.compare(rd.wsKeys) >0 )
                 return false;

            else if ( orderID >rd.orderID )
                 return true;
            else if ( rd.orderID >orderID )
                 return false;

            else if ( wsPhrs.compare(rd.wsPhrs) <0 )
                 return true;
            return false;
       }
};
typedef set<USER_V1_DAT>::iterator     USER_V1_DAT_ITR;



//-----------------------------------------------------------------------------
// V2 Version
//-----------------------------------------------------------------------------
struct V2_DICT_UI_LOAD
{
       int                   sID;
       bool                  enable;
       MEM_INT32             iKeys;
       MEM_INT32             iPhrs;
       MEM_INT32             iNext;

       V2_DICT_UI_LOAD()
       {
            sID    =0;
            enable =true;
            iKeys  =INVALID_MEM_OFFSET;
            iPhrs  =INVALID_MEM_OFFSET;
            iNext  =INVALID_MEM_OFFSET;
       }

       V2_DICT_UI_LOAD(int _sID,
                       const MEM_INT32 _iKeys,
                       const MEM_INT32 _iPhrs,
                       const MEM_INT32 _iNext =INVALID_MEM_OFFSET)
       {
            sID    =_sID;
            enable =true;
            iKeys  =_iKeys;
            iPhrs  =_iPhrs;
            iNext  =_iNext;
       }
};
typedef vector<V2_DICT_UI_LOAD>::iterator
V2_DICT_UI_LOAD_PTR;

//-----------------------------------------------------------------------------
struct V2_DICT_UI :V2_DICT_UI_LOAD
{
       wstring               wKeys;
       wstring               wPhrs;

       V2_DICT_UI() :V2_DICT_UI_LOAD()
       {
            wKeys  =L"";
            wPhrs  =L"";
       }

       V2_DICT_UI(int sID,
                  const MEM_INT32 iKeys,
                  const MEM_INT32 iPhrs,
                  const MEM_INT32 iNext =INVALID_MEM_OFFSET) :
                  V2_DICT_UI_LOAD(sID, iKeys, iPhrs, iNext)
       {
            wKeys  =L"";
            wPhrs  =L"";
       }

       V2_DICT_UI(V2_DICT_UI_LOAD ui)
       {
            wKeys  =L"";
            wPhrs  =L"";
            sID    =ui.sID;
            enable =ui.enable;
            iKeys  =ui.iKeys;
            iPhrs  =ui.iPhrs;
            iNext  =ui.iNext;
       }
};
typedef V2_DICT_UI*                    V2_DICT_UI_PTR;
typedef vector<V2_DICT_UI>::iterator   V2_DICT_UI_ITR;




//-----------------------------------------------------------------------------
// Phrase
//-----------------------------------------------------------------------------
struct PHRS_V2_POS
{
       V2_DICT_UI_ITR        itr;
       wstring               wsKeys;
       wstring               wsStrokes;

       bool operator ==(const PHRS_V2_POS& rd) const
       {
            if ( wsKeys.compare(rd.wsKeys) ==0 && wsStrokes.compare(rd.wsStrokes) ==0 )
            {
                 return true;
            }
            return false;
       }

       // for set<PHRS_V2_POS>
       PHRS_V2_POS()
       {
            wsKeys    =L"";
            wsStrokes =L"";
       }

       PHRS_V2_POS(const wstring _wsKeys, const wstring _wsStrokes)
       {
            wsKeys    =_wsKeys;
            wsStrokes =_wsStrokes;
       }

       // for set<PHRS_V2_POS>
       bool operator <(const PHRS_V2_POS & rd) const
       {
            if ( wsKeys.compare(rd.wsKeys) ==0 && wsStrokes.compare(rd.wsStrokes) ==0 )
            {
                 return true;
            }
            return false;
       }
};
#if defined(__FOR_NEW_SET)
typedef set<PHRS_V2_POS>::iterator     PHRS_V2_POS_ITR;
#else
typedef vector<PHRS_V2_POS>::iterator  PHRS_V2_POS_ITR;
#endif





//-----------------------------------------------------------------------------
// V2W Version
//-----------------------------------------------------------------------------
struct V2W_DICT_UI_LOAD
{
       int                   sID;
       bool                  enable;
       MEM_INT32             iKeys;
       MEM_INT32             iPhrs;
       MEM_INT32             iQKey;
       MEM_INT32             iHKey;
       MEM_INT32             iHVoc;
       MEM_INT32             iNext;

       V2W_DICT_UI_LOAD()
       {
            sID    =0;
            enable =true;
            iKeys  =INVALID_MEM_OFFSET;
            iPhrs  =INVALID_MEM_OFFSET;
            iQKey  =INVALID_MEM_OFFSET;
            iHKey  =INVALID_MEM_OFFSET;
            iHVoc  =INVALID_MEM_OFFSET;
            iNext  =INVALID_MEM_OFFSET;
       }

       V2W_DICT_UI_LOAD(int _sID,
                        const MEM_INT32 _iKeys,
                        const MEM_INT32 _iPhrs,
                        const MEM_INT32 _iQKey,
                        const MEM_INT32 _iHKey,
                        const MEM_INT32 _iHVoc,
                        const MEM_INT32 _iNext =INVALID_MEM_OFFSET)
       {
            sID    =_sID;
            enable =true;
            iKeys  =_iKeys;
            iPhrs  =_iPhrs;
            iQKey  =_iQKey;
            iHKey  =_iHKey;
            iHVoc  =_iHVoc;
            iNext  =_iNext;
       }
};
typedef vector<V2W_DICT_UI_LOAD>::iterator
V2W_DICT_UI_LOAD_PTR;

//-----------------------------------------------------------------------------
struct V2W_DICT_UI :V2W_DICT_UI_LOAD
{
       wstring               wKeys;
       wstring               wPhrs;

       V2W_DICT_UI() :V2W_DICT_UI_LOAD()
       {
            wKeys  =L"";
            wPhrs  =L"";
       }

       V2W_DICT_UI(int sID,
                   const MEM_INT32 _iKeys,
                   const MEM_INT32 _iPhrs,
                   const MEM_INT32 _iQKey,
                   const MEM_INT32 _iHKey,
                   const MEM_INT32 _iHVoc,
                   const MEM_INT32 _iNext =INVALID_MEM_OFFSET) :
                   V2W_DICT_UI_LOAD(sID, _iKeys, _iPhrs, _iQKey, _iHKey, _iHVoc, _iNext)
       {
            wKeys  =L"";
            wPhrs  =L"";
       }

       V2W_DICT_UI(V2W_DICT_UI_LOAD ui)
       {
            wKeys  =L"";
            wPhrs  =L"";
            sID    =ui.sID;
            enable =ui.enable;
            iKeys  =ui.iKeys;
            iPhrs  =ui.iPhrs;
            iQKey  =ui.iQKey;
            iHKey  =ui.iHKey;
            iHVoc  =ui.iHVoc;
            iNext  =ui.iNext;
       }
};
typedef V2W_DICT_UI*                   V2W_DICT_UI_PTR;
typedef vector<V2W_DICT_UI>::iterator  V2W_DICT_UI_ITR;


//-----------------------------------------------------------------------------
struct PHRS_V2W_POS
{
       V2W_DICT_UI_ITR       itr;
       bool                  isQwerty;
       wstring               wsKeys;
       wstring               wsStrokes;

       bool operator ==(const PHRS_V2W_POS& rd) const
       {
            if ( wsKeys.compare(rd.wsKeys) ==0 && wsStrokes.compare(rd.wsStrokes) ==0 )
            {
                 return true;
            }
            return false;
       }

       // for set<PHRS_V2_POS>
       PHRS_V2W_POS()
       {
            wsKeys    =L"";
            wsStrokes =L"";
            isQwerty  =false;
       }

       PHRS_V2W_POS(bool _isQwerty)
       {
            wsKeys    =L"";
            wsStrokes =L"";
            isQwerty  =false;
       }

       PHRS_V2W_POS(const wstring _wsKeys, const wstring _wsStrokes, bool _isQwerty)
       {
            wsKeys    =_wsKeys;
            wsStrokes =_wsStrokes;
            isQwerty  =_isQwerty;
       }

       // for set<PHRS_V2_POS>
       bool operator <(const PHRS_V2W_POS & rd) const
       {
            if ( wsKeys.compare(rd.wsKeys) ==0 && wsStrokes.compare(rd.wsStrokes) ==0 )
            {
                 return true;
            }
            return false;
       }
};
#if defined(__FOR_NEW_SET)
typedef set<PHRS_V2W_POS>::iterator    PHRS_V2W_POS_ITR;
#else
typedef vector<PHRS_V2W_POS>::iterator PHRS_V2W_POS_ITR;
#endif



//-----------------------------------------------------------------------------
struct PHRS_V3_POS
{
       V2_DICT_UI_ITR        itr;
       wstring               wsKeys;
       wstring               wsStrokes;

       bool operator ==(const PHRS_V3_POS& rd) const
       {
            if ( wsKeys.compare(rd.wsKeys) ==0 && wsStrokes.compare(rd.wsStrokes) ==0 )
            {
                 return true;
            }
            return false;
       }
};
typedef vector<PHRS_V3_POS>::iterator  PHRS_V3_POS_ITR;



//-----------------------------------------------------------------------------
struct V2_2W_HWCHAR_IDX
{
       wchar_t               wCH;
       MEM_INT32             Pos;
       MEM_INT32             Total;

       bool operator <(const V2_2W_HWCHAR_IDX & rd) const
       {
            if ( wCH <rd.wCH )
                 return true;

            return false;
       }
};


//-----------------------------------------------------------------------------
struct V1_DICT_UI
{
       bool                  succ;
       char*                 wsMem;
       MEM_INT               memSz;
       LPMEM_INT             wsWLen;
       LPMEM_INT             wsKLen;
       LPMEM_INT             wsNLen;

       V1_DICT_UI()
       {
            wsWLen =0;
            wsKLen =0;
            wsMem  =0;
            succ   =false;
       }
       V1_DICT_UI(short wrdLen, short keyLen, short nwdLen)
       {
            wsWLen =0;
            wsKLen =0;
            wsNLen =0;
            wsMem  =0;

            memSz  =wrdLen +1 +keyLen +1 +nwdLen +1;
            memSz *=sizeof(MEM_INT);

            alloc(wrdLen, keyLen, nwdLen);
       }
       ~V1_DICT_UI()
       {
            free();
       }
       bool isSucc()
       {
            return succ;
       }
       void free()
       {
            if ( wsMem !=0 )
            {
                 delete wsMem;
                 wsMem =0;
            }
       }
       bool alloc(short wrdLen, short keyLen, short nwdLen)
       {
            free();

            wsMem =new char[memSz +1];
            if ( wsMem ==0 )
            {
                 return succ =false;
            }
            memset(wsMem, 0, (memSz +1));

            wsWLen =(LPMEM_INT)wsMem;

            int
            memOffs =((wrdLen +1) *sizeof(MEM_INT));
            wsKLen  =(LPMEM_INT)(wsMem +memOffs);

            memOffs =((wrdLen +1 +keyLen +1) *sizeof(MEM_INT));
            wsNLen  =(LPMEM_INT)(wsMem +memOffs);

            return succ =true;
       }
       char *bMem() const
       {
            return wsMem;
       }
       MEM_INT bMemSz() const
       {
            return memSz;
       }
};

#endif // !defined(AFX_DATA_UI_H__6EA03A1C_ED45_4FCC_9182_8457D5E941C8__INCLUDED_)
