// Data_Decode.h: interface for the CData_Decode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_DECODE_H__B8E9F9D5_2A33_4E44_8E85_1331FC7EB19E__INCLUDED_)
#define AFX_DATA_DECODE_H__B8E9F9D5_2A33_4E44_8E85_1331FC7EB19E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Data_Mem.h"

//-----------------------------------------------------------------------------
//
// Depack DATA
//
//-----------------------------------------------------------------------------
class CDepack_Data
{
private:
      SHORT                  iLen;
      SHORT                  iMaxLen;
      LPSTR                  pStr;
      
public:
      CDepack_Data(LPBYTE &pByte, USHORT MaxLen)
      {
           iLen =0;
           pStr ='\0';

           parseEx(pByte, MaxLen);
      }

      ~CDepack_Data()
      {
           free();
      }

      void free()
      {
           if ( pStr !='\0' )
           {
                delete pStr;
                pStr ='\0';
           }
           iLen =0;
      }

      //-----------------------------------------------------------------------
      void parse(LPBYTE &pByte)
      {
           parseEx(pByte, iMaxLen);
      }

      void parseEx(LPBYTE &pByte, USHORT MaxLen)
      {
           iMaxLen =MaxLen;

           free();

           if ( iMaxLen <=0 )
           {
                return;
           }

           iLen =(pByte[0] <<8) |pByte[1];

           iMaxLen -=(iLen +2);
           
           pStr =new CHAR[iLen +1];
           
           if ( pStr !='\0' )
           {
                pByte +=sizeof(USHORT);

                memcpy(pStr, (LPSTR)pByte, sizeof(CHAR) *iLen);

                pStr[iLen] ='\0';

                pByte +=iLen;
           }
      }

      //-----------------------------------------------------------------------
      LPCSTR getMem()
      {
           return pStr;
      }
      
      //-----------------------------------------------------------------------
      SHORT getLen()
      {
           return iLen;
      }

      SHORT getLessLen()
      {
           return iMaxLen;
      }
};
typedef CDepack_Data*                  LPCDepack_Data;


//-----------------------------------------------------------------------------
//
// Depack BYTE
//
//-----------------------------------------------------------------------------
class CDepack_Byte
{
private:
      SHORT                  iLen;
      SHORT                  iMaxLen;
      LPSTR                  pStr;
      
public:
      CDepack_Byte(LPBYTE pByte, USHORT MaxLen)
      {
           iLen =0;
           pStr ='\0';

           parseEx(pByte, MaxLen);
      }

      ~CDepack_Byte()
      {
           free();
      }

      void free()
      {
           if ( pStr !='\0' )
           {
                delete pStr;
                pStr ='\0';
           }
           iLen =0;
      }

      //-----------------------------------------------------------------------
      void parse(LPBYTE &pByte)
      {
           parseEx(pByte, iMaxLen);
      }

      void parseEx(LPBYTE &pByte, USHORT MaxLen)
      {
           iMaxLen =MaxLen;

           free();

           iLen =sizeof(pByte) / sizeof(BYTE);

           iMaxLen -=(iLen +2);

           for ( iLen =0; pByte[iLen]; iLen++ );
           
           pStr =new CHAR[iLen +1];
           
           if ( pStr !='\0' )
           {
                pByte +=sizeof(USHORT);

                memcpy(pStr, (LPSTR)pByte, sizeof(CHAR) *iLen);

                pStr[iLen] ='\0';

                pByte +=iLen;
           }
      }
      
      //-----------------------------------------------------------------------
      LPCSTR getMem()
      {
           return pStr;
      }
      
      //-----------------------------------------------------------------------
      SHORT getLen()
      {
           return iLen;
      }

      SHORT getLessLen()
      {
           return iMaxLen;
      }
};
typedef CDepack_Byte*                  LPCDepack_Byte;


//-----------------------------------------------------------------------------
//
// Pack BYTE
//
//-----------------------------------------------------------------------------
class CPack_Byte :public CData_MemSz
{
public:
      bool putString(LPCSTR pSz)
      {
           USHORT
           sLen =strlen(pSz);

           if ( alloc(sLen +sizeof(USHORT)) ==false )
           {
                return false;
           }
           
           *(USHORT*)(pMem +iLen) =(sLen>>8 |sLen<<8);

           iLen +=sizeof(USHORT);

           memcpy(pMem +iLen, pSz, sLen *sizeof(BYTE));
           
           iLen +=sLen;
           
           return true;
      }
      
      bool putString(LPCSTR pSz, USHORT sLen)
      {
           if ( alloc(sLen +sizeof(USHORT)) ==false )
           {
                return false;
           }
           
           *(USHORT*)(pMem +iLen) =(sLen>>8 |sLen<<8);

           iLen +=sizeof(USHORT);

           memcpy(pMem +iLen, pSz, sLen *sizeof(BYTE));

           iLen +=sLen;
           
           return true;
      }
};
typedef CPack_Byte*                    LPCPack_Byte;


#endif // !defined(AFX_DATA_DECODE_H__B8E9F9D5_2A33_4E44_8E85_1331FC7EB19E__INCLUDED_)
