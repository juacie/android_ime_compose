// imebase.cpp: implementation of the imebase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "imebase.h"

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      : Construction/Destruction
//
//-----------------------------------------------------------------------------
imebase::imebase()
{
    m_imeX =&m_v1;
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
imebase::~imebase()
{
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void imebase::attach(PHRS_IME_ID vID)
{
     switch ( vID )
     {
              default:
              case phrs_v1:
                   m_imeX =&m_v1;
                   break;
              case phrs_v2:
                   m_imeX =&m_v2;
                   break;
              case phrs_v2_1:
                   m_imeX =&m_v2_1;
                   break;
              case phrs_v2_1w:
                   m_imeX =&m_v2_1w;
                   break;
              case phrs_v2_2w:
                   m_imeX =&m_v2_2w;
                   break;
              case phrs_v3:
                   m_imeX =&m_v3;
                   break;
     }
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
void imebase::attach(imebase &_base)
{
     clear();

     if ( _base.isV2() )
     {
          m_imeX =&m_v2;
     }
     else if ( _base.isV2() )
     {
          m_imeX =&m_v2;
     }
     else if ( _base.isV2_1() )
     {
          m_imeX =&m_v2_1;
     }
     else if ( _base.isV2_1w() )
     {
          m_imeX =&m_v2_1w;
     }
     else if ( _base.isV2_2w() )
     {
          m_imeX =&m_v2_2w;
     }
     else if ( _base.isV3() )
     {
          m_imeX =&m_v3;
     }
     else
     {
          m_imeX =&m_v1;
     }
     m_imeX->setPreLoad(_base->getPreLoad());
     m_imeX->attachWildKLen(_base->getWildKLen());
}

//-----------------------------------------------------------------------------
//
// Return Value :
// Parameters   :
// Remarks      :
//
//-----------------------------------------------------------------------------
bool imebase::loadDict(const char* dictName)
{
     clear();
     setlocale( LC_ALL, "");

     try
     {
          FILE
          *fp =fopen(dictName, "rb");
     
          if ( fp )
          {
               char 
               headSz[sizeof(HEAD_ID_DICT_SIZE) +1] ={0};

               int
               ReadSz =fread(headSz, sizeof(HEAD_ID_DICT_SIZE) *sizeof(char), 1, fp);

               fclose(fp);

               if ( memcmp(headSz, HEAD_ID_DICT_V2_1, sizeof(HEAD_ID_DICT_V2_1)) ==0 )
               {
                    m_imeX =&m_v2_1;
                    m_imeX->setPreLoad(true);

                    if ( m_v2_1.loadDict(dictName) ==true )
                    {
                         return true;
                    }
               }
               else if ( memcmp(headSz, HEAD_ID_DICT_V2_1W, sizeof(HEAD_ID_DICT_V2_1W)) ==0 )
               {
                    m_imeX =&m_v2_1w;
                    m_imeX->setPreLoad(true);

                    if ( m_v2_1w.loadDict(dictName) ==true )
                    {
                         return true;
                    }
               }
               else if ( memcmp(headSz, HEAD_ID_DICT_V2_2W, sizeof(HEAD_ID_DICT_V2_2W)) ==0 )
               {
                    m_imeX =&m_v2_2w;
                    m_imeX->setPreLoad(true);

                    if ( m_v2_2w.loadDict(dictName) ==true )
                    {
                         return true;
                    }
               }
               else if ( memcmp(headSz, HEAD_ID_DICT_V2, sizeof(HEAD_ID_DICT_V2)) ==0 )
               {
                    m_imeX =&m_v2;

                    if ( m_v2.loadDict(dictName) ==true )
                    {
                         return true;
                    }
               }
               else if ( memcmp(headSz, HEAD_ID_DICT_V3, sizeof(HEAD_ID_DICT_V3)) ==0 )
               {
                    m_imeX =&m_v3;

                    if ( m_v3.loadDict(dictName) ==true )
                    {
                         return true;
                    }
               }
               m_imeX =&m_v1;

               return m_v1.loadDict(dictName);
          }
     }
     catch ( ... )
     {
     }
     return false;
}
