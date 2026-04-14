// Data_Mem2.h: interface for the CData_Mem2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_MEM_H__B30CFFA3_272F_467D_87E9_B1DAAFD61374__INCLUDED_)
#define AFX_DATA_MEM_H__B30CFFA3_272F_467D_87E9_B1DAAFD61374__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Data_Type.h"


//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
class CData_Mem
{
private:
      LPBYTE                pMem;
      USHORT                iLen;
      
public:
      CData_Mem()
      {
           iLen =0;
           pMem ='\0';
           
           set("");
      }

      CData_Mem(LPCSTR pSz)
      {
           iLen =0;
           pMem ='\0';
           
           if ( pSz !='\0' )
           {
                set(pSz);
           }
      }
      
      ~CData_Mem()
      {
           free();
      }

      void free()
      {
           if ( pMem !='\0' )
           {
                delete [] pMem;
                pMem ='\0';
                iLen =0;
           }
      }
      
      BOOL alloc(INT iLen)
      {
           free();
           
           pMem =new BYTE[iLen +1];
           
           if ( pMem !='\0' )
           {
                memset(pMem, 0, sizeof(CHAR) *iLen +1);
           }
           return pMem !='\0';
      }
      
      void set(LPCSTR pSz)
      {
           iLen =strlen(pSz);
           
           if ( alloc(iLen) )
           {
                iLen =strlen(pSz);
                
                memcpy(pMem, pSz, sizeof(CHAR) *iLen);
           }
      }
      
      void set(LPCBYTE pSz, USHORT sLen)
      {
           iLen =sLen;
           
           if ( alloc(iLen) )
           {
                iLen =sLen;
                
                memcpy(pMem, pSz, sizeof(CHAR) *iLen);
           }
      }
      
      LPSTR getMem()
      {
           if ( pMem =='\0' )
           {
                return (LPSTR)"";
           }
           return (LPSTR)pMem;
      }
      
      LPCSTR c_str()
      {
           return (LPCSTR)getMem();
      }

      LPCSTR getSz()
      {
           return (LPCSTR)getMem();
      }

      LPBYTE getByte()
      {
           return (LPBYTE)getMem();
      }

      USHORT getLen()
      {
           return iLen;
      }
      
      void operator =(LPCSTR pSz)
      {
           set(pSz);
      }
};
typedef CData_Mem*                     LPCData_Mem;


//-----------------------------------------------------------------------------
//
// Pack BYTE
//
//-----------------------------------------------------------------------------
#define EXTEND_MEM_SIZE                5000

class CData_MemSz
{
protected:
     LPBYTE                 pMem;
     USHORT                 iLen;
     USHORT                 iMaxLen;
     
public:
      CData_MemSz()
      {
           iLen    =0;
           iMaxLen =0;
           pMem    ='\0';
           
      }
      
      virtual ~CData_MemSz()
      {
           free();
      }
      
      void free()
      {
           if ( pMem !='\0' )
           {
                delete[] pMem;
                pMem ='\0';
           }
           iLen =0;
           iMaxLen =0;
      }
      
      bool alloc(USHORT sLen)
      {
           if ( pMem =='\0' )
           {
                iMaxLen =EXTEND_MEM_SIZE;
                
                USHORT
                newMaxlen =iMaxLen +1;
                
                pMem =new BYTE[newMaxlen];
                
                if ( pMem =='\0' )
                {
                     return false;
                }
                memset(pMem, 0, newMaxlen *sizeof(BYTE));
           }
           else if ( sLen +iLen > iMaxLen )
           {
                do
                {
                     USHORT
                     newMaxlen =iMaxLen +EXTEND_MEM_SIZE +1;
                     
                     LPBYTE
                     pNewByte =new BYTE[newMaxlen];
                     
                     if ( pNewByte =='\0' )
                     {
                          return false;
                     }
                     memset(pNewByte, 0, newMaxlen *sizeof(BYTE));
                     memcpy(pNewByte, pMem, iLen *sizeof(BYTE));
                     
                     delete[] pMem;
                     
                     pMem =pNewByte;
                     
                     iMaxLen +=EXTEND_MEM_SIZE;
                     
                } while ( sLen +iLen > iMaxLen );
           }
           return true;
      }
      
      virtual bool putString(LPCSTR pSz)
      {
           USHORT
           sLen =strlen(pSz);
           
           if ( alloc(sLen) ==false )
           {
                return false;
           }
           
           memcpy(pMem +iLen, pSz, sLen *sizeof(BYTE));
           
           iLen +=sLen;
           
           return true;
      }
      
      virtual bool putString(LPCSTR pSz, USHORT sLen)
      {
           if ( alloc(sLen) ==false )
           {
                return false;
           }
           
           memcpy(pMem +iLen, pSz, sLen *sizeof(BYTE));
           
           iLen +=sLen;
           
           return true;
      }
      
      bool putShort(USHORT value)
      {
           if ( alloc(sizeof(USHORT)) ==false )
           {
                return false;
           }
           *(USHORT*)(pMem +iLen) =(value>>8 |value<<8);
           
           iLen +=sizeof(USHORT);
           
           return true;
      }
      
      bool put(UCHAR value)
      {
           if ( alloc(sizeof(UCHAR)) ==false )
           {
                return false;
           }
           pMem[iLen] =value;
           
           iLen +=sizeof(UCHAR);
           
           return true;
      }
      
      //-----------------------------------------------------------------------
      LPCSTR getMem()
      {
           if ( pMem =='\0' )
           {
                return (LPSTR)"";
           }
           return (LPSTR)pMem;
      }
      
      //-----------------------------------------------------------------------
      LPCBYTE getByte()
      {
           if ( pMem =='\0' )
           {
                return (LPCBYTE)"";
           }
           return pMem;
      }

      //-----------------------------------------------------------------------
      LPCSTR c_str()
      {
           if ( pMem =='\0' )
           {
                return (LPCSTR)"";
           }
           return (LPCSTR)pMem;
      }
      
      //-----------------------------------------------------------------------
      USHORT getLen()
      {
           return iLen;
      }
};
typedef CData_MemSz*                   LPCData_MemSz;


#endif // !defined(AFX_DATA_MEM_H__B30CFFA3_272F_467D_87E9_B1DAAFD61374__INCLUDED_)
